#include "kmc_ode.h"
#include "kmc_system.h"   // only needed for the ODESystem(const System&) constructor
#include <cmath>
#include <algorithm>
#include <iostream>
#include <fstream>

#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

namespace py = pybind11;

namespace {

// Continuous generalization of System::binomial(n,k): the falling factorial
// n(n-1)...(n-k+1)/k!, clamped to zero once any factor would go negative.
// For integer n this equals binomial(n,k) exactly; for the continuous
// populations used here it interpolates smoothly and reproduces the same
// saturation/exclusion behaviour (a nearly-empty site can't sustain a
// same-species bimolecular step) that the stochastic engine gets from
// sampling without replacement -- so the *same* rate constant gives
// consistent propensities in both engines.
double contFallingFactorial(double n, int nu)
{
    if(nu <= 0) return 1.0;
    if(n <= 0.0) return 0.0;
    double res = 1.0;
    for(int i=0;i<nu;++i){
        double term = n - i;
        if(term <= 0.0) return 0.0;
        res *= term;
    }
    double fact = 1.0;
    for(int i=2;i<=nu;++i) fact *= i;
    return res / fact;
}

// Dense Gaussian elimination with partial pivoting for (I - h J) k = rhs.
// State dimension for a single-site mean-field model is small, so plain
// O(n^3) elimination is plenty fast and keeps this self-contained.
bool solveLinear(std::vector<double> A, std::vector<double> b, std::vector<double>& x, int n)
{
    for(int col=0; col<n; ++col){
        int piv = col;
        double best = std::abs(A[(size_t)col*n+col]);
        for(int r=col+1; r<n; ++r){
            double v = std::abs(A[(size_t)r*n+col]);
            if(v > best){ best = v; piv = r; }
        }
        if(best < 1e-12) return false;
        if(piv != col){
            for(int c=0;c<n;++c) std::swap(A[(size_t)col*n+c], A[(size_t)piv*n+c]);
            std::swap(b[col], b[piv]);
        }
        double diag = A[(size_t)col*n+col];
        for(int r=col+1; r<n; ++r){
            double factor = A[(size_t)r*n+col] / diag;
            if(factor == 0.0) continue;
            for(int c=col;c<n;++c) A[(size_t)r*n+c] -= factor*A[(size_t)col*n+c];
            b[r] -= factor*b[col];
        }
    }
    x.assign(n, 0.0);
    for(int r=n-1; r>=0; --r){
        double sum = b[r];
        for(int c=r+1; c<n; ++c) sum -= A[(size_t)r*n+c]*x[c];
        x[r] = sum / A[(size_t)r*n+r];
    }
    return true;
}

} // namespace

// ----------------- ODESystem -----------------

ODESystem::ODESystem(const System& sys)
    : species(sys.species.begin(), sys.species.end())
    , reactions(sys.reactions)
    , reactor(sys.reactor)
    , names(sys.names)
{
    reaction_extent.assign(reactions.size(), 0.0);
    for(size_t i=0;i<names.size();++i) name_to_idx[names[i]] = (int)i;
    buildIndexMaps();
}

int ODESystem::getSpeciesIndex(const std::string& name) const
{
    auto it = name_to_idx.find(name);
    if(it==name_to_idx.end())
        throw std::runtime_error("Unknown species: " + name);
    return it->second;
}

void ODESystem::buildIndexMaps()
{
    surf_idx.clear(); surf_pos.clear();
    gas_idx.clear();  gas_pos.clear();

    int pos = 0;
    for(size_t idx=0; idx<species.size(); ++idx){
        if(reactor.isGas((int)idx)) continue;          // gas handled separately (as pressures)
        surf_idx.push_back((int)idx);
        surf_pos[(int)idx] = pos++;
    }
    for(auto& [idx, feed_p] : reactor.gas_species){
        if(reactor.reservoir.count(idx)) continue;      // pinned (infinite) reservoir gas -- not part of the state
        gas_idx.push_back(idx);
        gas_pos[idx] = pos++;
    }

    n_surf = (int)surf_idx.size();
    n_gas  = (int)gas_idx.size();
    extent_offset = pos;
    state_size = pos + (int)reactions.size();
}

void ODESystem::computeRHS(const std::vector<double>& y, std::vector<double>& dydt) const
{
    dydt.assign(y.size(), 0.0);

    constexpr double kB = 1.380649e-23;
    const double per_molecule_Pa = (reactor.volume > 0.0)
        ? reactor.scaleup * kB * reactor.temperature / reactor.volume : 0.0;

    auto surfAmount = [&](int idx)->double{
        auto it = surf_pos.find(idx);
        return (it != surf_pos.end()) ? y[it->second] : 0.0;
    };
    auto gasPressure = [&](int idx)->double{
        auto it = gas_pos.find(idx);
        if(it != gas_pos.end()) return y[it->second];
        auto pit = reactor.partial_pressure.find(idx);   // pinned reservoir gas: fixed at its stored value
        return (pit != reactor.partial_pressure.end()) ? pit->second : 0.0;
    };

    for(size_t j=0; j<reactions.size(); ++j){
        const Reaction& r = reactions[j];
        double rate = r.rate;

        for(auto& [idx,nu] : r.reactants){
            if(r.adsorption && reactor.isGas(idx)){
                double p = gasPressure(idx);
                rate *= std::pow(p, nu);
            } else {
                double n = surfAmount(idx);
                rate *= contFallingFactorial(n, nu);
            }
            if(rate == 0.0) break;
        }
        if(rate == 0.0) continue;

        dydt[extent_offset + j] += rate;

        for(auto& [idx,nu] : r.reactants){
            if(reactor.isGas(idx)){
                auto it = gas_pos.find(idx);
                if(it != gas_pos.end()) dydt[it->second] -= nu*rate*per_molecule_Pa;
            } else {
                auto it = surf_pos.find(idx);
                if(it != surf_pos.end()) dydt[it->second] -= nu*rate;
            }
        }
        for(auto& [idx,nu] : r.products){
            if(reactor.isGas(idx)){
                auto it = gas_pos.find(idx);
                if(it != gas_pos.end()) dydt[it->second] += nu*rate*per_molecule_Pa;
            } else {
                auto it = surf_pos.find(idx);
                if(it != surf_pos.end()) dydt[it->second] += nu*rate;
            }
        }
    }

    // Open system (CSTR/PFR): exponential relaxation of the *free* gas
    // species toward their feed pressure -- continuum analogue of
    // Reactor::updateGasPressures' analytic transport term.
    if(!reactor.closed_system && reactor.residence_time > 0.0){
        for(auto& [idx, feed_p] : reactor.gas_species){
            auto it = gas_pos.find(idx);
            if(it == gas_pos.end()) continue;   // reservoir-pinned, skip
            double Pi = y[it->second];
            dydt[it->second] += (feed_p - Pi) / reactor.residence_time;
        }
    }
}

void ODESystem::computeJacobian(const std::vector<double>& y, const std::vector<double>& f0,
                                 std::vector<double>& J) const
{
    const int n = (int)y.size();
    J.assign((size_t)n*n, 0.0);
    std::vector<double> yp = y, fp(n);

    for(int i=0;i<n;++i){
        double yi = yp[i];
        double eps = 1e-6 * std::max(1.0, std::abs(yi));
        yp[i] = yi + eps;
        computeRHS(yp, fp);
        for(int r=0;r<n;++r) J[(size_t)r*n + i] = (fp[r]-f0[r]) / eps;
        yp[i] = yi;
    }
}

bool ODESystem::rosenbrockEulerStep(std::vector<double>& y, double h) const
{
    const int n = (int)y.size();
    std::vector<double> f0(n);
    computeRHS(y, f0);

    std::vector<double> J;
    computeJacobian(y, f0, J);

    std::vector<double> A((size_t)n*n, 0.0);
    for(int r=0;r<n;++r){
        for(int c=0;c<n;++c) A[(size_t)r*n+c] = -h*J[(size_t)r*n+c];
        A[(size_t)r*n+r] += 1.0;
    }

    std::vector<double> k;
    if(!solveLinear(A, f0, k, n)) return false;

    for(int i=0;i<n;++i){
        double yn = y[i] + h*k[i];
        y[i] = (yn < 0.0) ? 0.0 : yn;   // physical non-negativity guard
    }
    return true;
}

void ODESystem::syncGasReporting()
{
    constexpr double kB = 1.380649e-23;
    for(auto& [idx, feed_p] : reactor.gas_species){
        double Pi = reactor.partial_pressure.count(idx) ? reactor.partial_pressure[idx] : 0.0;
        species[idx] = Pi * reactor.volume / (kB*reactor.temperature * reactor.scaleup);
    }
}

std::vector<double> ODESystem::packState() const
{
    std::vector<double> y(state_size, 0.0);
    for(int i=0;i<n_surf;++i) y[i] = species[surf_idx[i]];
    for(int i=0;i<n_gas;++i){
        int idx = gas_idx[i];
        auto it = reactor.partial_pressure.find(idx);
        y[n_surf+i] = (it != reactor.partial_pressure.end()) ? it->second : 0.0;
    }
    for(size_t j=0;j<reactions.size();++j) y[extent_offset+j] = reaction_extent[j];
    return y;
}

void ODESystem::unpackAndCommit(double time, const std::vector<double>& y)
{
    for(int i=0;i<n_surf;++i) species[surf_idx[i]] = y[i];
    for(int i=0;i<n_gas;++i) reactor.partial_pressure[gas_idx[i]] = y[n_surf+i];
    syncGasReporting();
    for(size_t j=0;j<reactions.size();++j) reaction_extent[j] = y[extent_offset+j];

    times.push_back(time);
    extent_history.push_back(reaction_extent);
    history.push_back(species);

    appendLogEntry(time);
    step++;
}

std::string ODESystem::gasLogFilename() const
{
    auto pos = logfile.rfind(".log");
    if(pos != std::string::npos) return logfile.substr(0,pos) + "_gas.log";
    return logfile + "_gas.log";
}

void ODESystem::initLogFile() const
{
    std::ofstream outfile(logfile, std::ios::out);
    if(!outfile.is_open()){ std::cout << "error opening logfile\n"; return; }
    outfile << "Time  ";
    for(const auto& name : names) outfile << name << " ";
    outfile << "\n";
    outfile.close();

    std::ofstream gout(gasLogFilename(), std::ios::out);
    if(!gout.is_open()){ std::cout << "error opening logfile\n"; return; }
    gout << "Time  ";
    for(auto& [idx, feed_p] : reactor.gas_species) gout << names[idx] << " ";
    gout << "\n";
    gout.close();
}

void ODESystem::appendLogEntry(double time) const
{
    std::ofstream outfile(logfile, std::ios::app);
    if(outfile.is_open()){
        outfile << time << "  ";
        for(auto v : species) outfile << v << "  ";
        outfile << "\n";
    }
    std::ofstream gout(gasLogFilename(), std::ios::app);
    if(gout.is_open()){
        gout << time << "  ";
        for(auto& [idx, feed_p] : reactor.gas_species){
            auto it = reactor.partial_pressure.find(idx);
            gout << (it != reactor.partial_pressure.end() ? it->second : 0.0) << "  ";
        }
        gout << "\n";
    }
}

// ----------------- MeanFieldODE -----------------
void MeanFieldODE::run(double t_end, double rtol, double atol,
                        double h_init, double h_min, double h_max, size_t max_steps)
{
    std::cout << "Running deterministic mean-field ODE integration "
                 "(linearized-implicit, stiff-capable)\n";

    system.buildIndexMaps();
    const int n = system.stateSize();

    system.initLogFile();
    system.times.clear();
    system.history.clear();
    system.extent_history.clear();
    system.step = 0;
    steady.onset = -1.0;

    if(h_max <= 0.0) h_max = std::max(t_end/20.0, h_init);

    std::vector<double> y = system.packState();
    system.unpackAndCommit(0.0, y);   // record the initial condition as snapshot 0

    double t = 0.0, h = h_init;
    size_t nattempt = 0, naccept = 0;

    while(t < t_end && nattempt < max_steps){
        if(t + h > t_end) h = t_end - t;

        std::vector<double> y_full = y;
        bool ok = system.rosenbrockEulerStep(y_full, h);

        std::vector<double> y_half = y;
        ok = ok && system.rosenbrockEulerStep(y_half, 0.5*h);
        ok = ok && system.rosenbrockEulerStep(y_half, 0.5*h);

        ++nattempt;

        if(!ok){
            h *= 0.5;
            if(h < h_min){
                std::cout << "STOPPED: singular Jacobian / step collapsed at t=" << t << "\n";
                break;
            }
            continue;
        }

        double err_norm = 0.0;
        for(int i=0;i<n;++i){
            double sc = atol + rtol*std::max(std::abs(y_full[i]), std::abs(y_half[i]));
            double e = (y_half[i]-y_full[i]) / (sc>0.0 ? sc : 1.0);
            err_norm += e*e;
        }
        err_norm = std::sqrt(err_norm/std::max(n,1));

        // order-1 method -> optimal step-size exponent is 1/(p+1) = 1/2
        double fac = 0.9*std::pow(1.0/std::max(err_norm,1e-10), 0.5);
        fac = std::min(5.0, std::max(0.2, fac));
        double h_new = std::min(h_max, std::max(h_min, h*fac));

        bool accept = (err_norm <= 1.0) || (h <= h_min*1.0001);
        if(accept){
            y = y_half;   // Richardson-extrapolated (more accurate) state
            t += h;
            ++naccept;
            if(naccept % (size_t)std::max(system.saveFreq, 1L) == 0 || t >= t_end){
                system.unpackAndCommit(t, y);
                if(steady.update(system.step, t, system.times, system.history)) break;
            }
        }
        h = h_new;

        if(PyErr_CheckSignals() != 0) throw py::error_already_set();
    }

    if(system.times.empty() || system.times.back() < t) system.unpackAndCommit(t, y);

    std::cout << "ODE integration completed: t=" << t
              << "  accepted steps=" << naccept
              << "  attempts=" << nattempt << "\n";
}
