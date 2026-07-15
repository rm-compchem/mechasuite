#include "kmc.h"
#include <cmath>
#include <stdexcept>
#include <numeric>
#include <random>
#include <iostream>
#include <algorithm>
#include <functional>
#include <fstream>

#include <pybind11/pybind11.h>
#include <pybind11/stl.h>


namespace py = pybind11;

// Add to kmc.h: an enum + a way to retrieve the last stop reason
enum class StopReason { Running, ReachedTEnd, NoPropensity, MaxStepReached };

namespace {

// Continuous generalization of System::binomial(n,k): the falling factorial
// n(n-1)...(n-k+1)/k!, clamped to zero once any factor would go negative.
// For integer n this equals binomial(n,k) exactly; for the continuous
// populations used by the ODE (mean-field limit) it interpolates smoothly
// and reproduces the same saturation/exclusion behaviour (a nearly-empty
// site can't sustain a same-species bimolecular step) that the stochastic
// engine gets from sampling without replacement -- so the *same* rate
// constant gives consistent propensities in both engines.
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

// Dense Gaussian elimination with partial pivoting for (I - h*J) k = rhs.
// State dimension for a single-site mean-field model is small (a handful to
// a few dozen unknowns), so plain O(n^3) elimination is plenty fast and
// keeps this self-contained (no external linear-algebra dependency).
bool solveLinear(std::vector<double> A, std::vector<double> b, std::vector<double>& x, int n)
{
    for(int col=0; col<n; ++col){
        int piv = col;
        double best = std::abs(A[(size_t)col*n+col]);
        for(int r=col+1; r<n; ++r){
            double v = std::abs(A[(size_t)r*n+col]);
            if(v > best){ best = v; piv = r; }
        }
        if(best < 1e-12) return false; // singular / too ill-conditioned to trust
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

// ----------------- Reactor -----------------
void writeHistory(const std::string& filename, double time, System& sys)
{
   std::ofstream outfile;
   outfile.open (filename,  std::ios::app );
   if(!outfile.is_open()) {
      std::cout << "error opening logile\n";
      return;
   }
   
   outfile << time << "  " ;
   for( auto i: sys.species){
   	outfile << i << "  ";
   }
   outfile << "\n";
   outfile.close();

  // gas
   outfile.open ("kmc_gas.log",  std::ios::app );
   if(!outfile.is_open()) {
      std::cout << "error opening logile\n";
      return;
   }
   
   outfile << time << "  " ;
   for( auto& [idx, feed_p] : sys.reactor.gas_species){
   	outfile << sys.reactor.partial_pressure[idx] << "  ";
   }
   outfile << "\n";
   outfile.close();
}

void initHistoryFile(const std::string& filename, System& sys)
{
   std::ofstream outfile;
   outfile.open (filename,  std::ios::out );
   if(!outfile.is_open()) {
      std::cout << "error opening logile\n";
      return;
   }
   
   outfile << "Time  ";
   for (const auto name:sys.names){
      outfile << name << " ";
   }
   
   outfile << "\n";
   outfile.close();


   // write gas
   outfile.open ("kmc_gas.log",  std::ios::out );
   if(!outfile.is_open()) {
      std::cout << "error opening logile\n";
      return;
   }
   
   outfile << "Time  ";
   for (auto& [idx, feed_p] : sys.reactor.gas_species){
      outfile << sys.names[idx] << " ";
   }
   
   outfile << "\n";
   outfile.close();   
}

// Exact update of every gas species' partial pressure over the elapsed
// time dt of this KMC step. Two additive, physically separate terms:
//  1 transport: exact analytic solution of dP/dt=(feed-P)/tau, valid for
//      any dt, no noise, no stiffness.
//  2 reaction: the *actual* stoichiometric consequence of whatever fired
//      this step (from applyReactionEvent below), applied at the end of
//      the interval -- matches Gillespie semantics (event occurs at t+dt).
void Reactor::updateGasPressures(double dt, const std::map<int,double>& delta_from_reactions)
{
    if(volume <= 0.0) return;
    for(auto& [idx, feed_p] : gas_species){
        if(reservoir.count(idx)) continue;   // pinned externally, skip integration

        double Pi = partial_pressure.count(idx) ? partial_pressure[idx] : feed_p;

        if(!closed_system && residence_time > 0.0)
            // Pi - feed_p is how far the current pressure is from the feed (steady-state target) value.
            // exp(-dt/residence_time) is the fraction of that gap that survives after time dt — it decays exponentially with time constant τ. At dt=0 it's 1 (no change yet); as dt→∞ it's 0 (gap fully closed).
            // So the new pressure is: feed value, plus whatever fraction of the old gap hasn't relaxed away yet.
            Pi = feed_p + (Pi - feed_p) * std::exp(-dt/residence_time); 

        auto it = delta_from_reactions.find(idx);
        if(it != delta_from_reactions.end()) Pi += it->second;
        if(Pi < 0.0) Pi = 0.0;   // fp-roundoff guard only, not a physical clamp

        partial_pressure[idx] = Pi;
    }
}

void Reactor::applyFlow(std::vector<int>& sp,
                        double dt,
                        std::mt19937& rng)
{
    // ---- 1. Closed system → nothing happens
    if(closed_system) return;

    // ---- 2. Loop over gas species
    for(const auto& [idx,target] : gas_species)
    {
        // ------------------------------------
        // GRAND CANONICAL RESERVOIR MODE
        // ------------------------------------
        // Species population fixed externally
        auto res = reservoir.find(idx);
        if(res != reservoir.end())
        {
            sp[idx] = res->second;   // enforce constant population
            continue;
        }

        // ------------------------------------
        // FLOW MODE (CSTR / PFR segment)
        // ------------------------------------
        if(residence_time <= 0) continue;

        // Mean exchange with feed
        double mean = std::abs(target - sp[idx]) * dt / residence_time;
        if(mean <= 0.0) continue;

        // Poisson stochastic transfer
        std::poisson_distribution<int> P(mean);
        int delta = P(rng);

        if(sp[idx] < target)
            sp[idx] += delta;   // inflow
        else
            sp[idx] -= delta;   // outflow

        if(sp[idx] < 0) sp[idx] = 0;
    }
}

// ----------------- System -----------------
// System.cpp
// Applies nfirings of reaction rxn_idx. Surface/site species update
// species[] exactly as before (genuine discrete KMC state). Gas-phase
// species (reactor.isGas(idx)) do NOT touch species[] -- their mass is
// accumulated into gas_delta_pressure (Pa, via PV=NkT) for
// Reactor::updateGasPressures to apply exactly, since gas is now a
// continuum, not a particle count.
void System::applyReactionEvent(size_t rxn_idx, int nfirings,
                                 std::vector<int>& changed_surface,
                                 std::map<int,double>& gas_delta_pressure)
{
    if(nfirings<=0) return;
    
    const Reaction& R = reactions[rxn_idx];
    constexpr double kB = 1.380649e-23;

    // contribution to pressure change of one molecule/atom
    double per_molecule_Pa = (reactor.volume>0.0) 
                        ? reactor.scaleup * kB*reactor.temperature/reactor.volume : 0.0;

    for(auto& [idx,nu] : R.reactants){
        if(reactor.isGas(idx)) gas_delta_pressure[idx] -= nu*nfirings*per_molecule_Pa;
        else { species[idx] -= nu*nfirings; changed_surface.push_back(idx); }
    }
    for(auto& [idx,nu] : R.products){
        if(reactor.isGas(idx)) gas_delta_pressure[idx] += nu*nfirings*per_molecule_Pa;
        else { species[idx] += nu*nfirings; changed_surface.push_back(idx); }
    }
    reaction_extent[rxn_idx] += nfirings;
}

// Purely cosmetic: mirrors the continuous pressure into an equivalent
// molecule count in species[], so your existing history/logging/plotting
// code (which reads System::species) keeps working with zero changes.
// Never read back into propensities or dynamics.
void System::syncGasReporting()
{
    // update gas count in species
    constexpr double kB = 1.380649e-23;
    for(auto& [idx, feed_p] : reactor.gas_species){
        double Pi = reactor.partial_pressure.count(idx) ? reactor.partial_pressure[idx] : 0.0;
        species[idx] = static_cast<int>(std::round(
                    Pi * reactor.volume / (kB*reactor.temperature * reactor.scaleup)));
    }
}

int System::addSpecies(const std::string& name, int initial)
{
    if(name_to_idx.count(name)) return name_to_idx[name];
    int idx = species.size();
    species.push_back(initial);
    names.push_back(name);
    name_to_idx[name] = idx;
    return idx;
}

int System::getSpeciesIndex(const std::string& name) const 
{
    auto it = name_to_idx.find(name);
    if(it==name_to_idx.end())
        throw std::runtime_error("Unknown species: " + name);
    return it->second;
}

void System::addReaction(const Reaction& r)
{
    reactions.push_back(r);
}

double System::binomial(unsigned n, unsigned k)
{
    /*
    computes the binomial coefficient "n choose k" — the number of ways
    to pick k items from a set of n, i.e. n! / (k! · (n-k)!).

    to get the correct combinatorial factor for reactions that consume more 
    than one copy of the same species — e.g. for 2A -> B, the propensity 
    isn't just proportional to the population of A, it's proportional to 
    C(n_A, 2) (the number of distinct pairs of A molecules available to react), 
    which is standard stochastic mass-action kinetics.
    */

    if(k>n) return 0.0;
    if(k==0 || k==n) return 1.0;
    double res = 1.0;
    for(unsigned i=1;i<=k;++i){
        res *= (n - (k-i));
        res /= i;
    }
    return res;
}

// double System::computePropensity(const Reaction& r) const 
// {
//     double a = r.rate;
//     for(auto& [idx, nu] : r.reactants){
//         a *= binomial(species[idx], nu);
//         if(a==0.0) return 0.0;
//     }
//     return a;
// }

double System::computePropensity(const Reaction& r) const 
{
    double a = r.rate;
    for(auto& [idx, nu] : r.reactants){
        if(r.adsorption && reactor.isGas(idx)){
            // Gas reservoir is effectively infinite — driven by pressure,
            // not by a simulated particle count.
            auto it = reactor.partial_pressure.find(idx);
            double p = (it != reactor.partial_pressure.end()) ? it->second : 0.0;
            a *= std::pow(p, nu);   // first-order in pressure for nu=1 (the normal case)
            // std::cout << "calculating rate for gas "<< names[idx] << " " << a;
        } else {
            a *= binomial(species[idx], nu);
            if(a==0.0) return 0.0; // early exit, it is zero anyways
        }
    }
    return a;
}

void System::computeAllPropensities()
{
    propensities.clear();
    for(auto& r: reactions)
        propensities.push_back(computePropensity(r));
}

void System::updatePropensities(const std::vector<int>& changed_species)
{
    // update the reaction rates of only the species involved in the event
    // that was executed last.

    std::set<size_t> to_update;
    for(int s: changed_species){
        auto it = dep_graph.species_to_reactions.find(s);
        if(it!=dep_graph.species_to_reactions.end()){
            to_update.insert(it->second.begin(),it->second.end());
        }
    }
    for(size_t i: to_update){
        propensities[i] = computePropensity(reactions[i]);
    }
}

// Compares the windowed average of species populations over
// [t_now-2*window, t_now-window] against [t_now-window, t_now].
// Declares steady state when every species' relative change between
// the two windows is below tol. Works for both a closed system settling
// to equilibrium and an open flow system settling to a driven steady state —
// it only asks "have populations stopped drifting", not "is net rate zero".
bool System::isSteadyState(double window, double tol) const
{
    if(times.empty()) return false;
    double t_now = times.back();
    if(t_now < 2.0*window) return false;   // not enough elapsed time to judge yet

    auto idx_at = [&](double t){
        return static_cast<size_t>(std::lower_bound(times.begin(), times.end(), t) - times.begin());
    };
    size_t i0 = idx_at(t_now - 2.0*window);
    size_t i1 = idx_at(t_now - window);
    size_t i2 = times.size();
    if(i1 <= i0 || i2 <= i1) return false;   // window too fine relative to saveFreq resolution

    size_t nsp = species.size();
    std::vector<double> avg_prev(nsp,0.0), avg_curr(nsp,0.0);

    for(size_t i=i0;i<i1;++i)
        for(size_t s=0;s<nsp;++s) avg_prev[s] += history[i][s];
    for(size_t s=0;s<nsp;++s) avg_prev[s] /= double(i1-i0);

    for(size_t i=i1;i<i2;++i)
        for(size_t s=0;s<nsp;++s) avg_curr[s] += history[i][s];
    for(size_t s=0;s<nsp;++s) avg_curr[s] /= double(i2-i1);

    for(size_t s=0;s<nsp;++s){
        double denom = std::max(avg_prev[s], 1.0);   // guards near-zero populations
        if(std::abs(avg_curr[s]-avg_prev[s]) / denom >= tol) return false;
    }
    return true;
}

// Computes the "highest order reactant" factor g_i for each species.
// This measures how sensitive species i's future propensity is to a change
// in its own population *right now* — needed because bounding tau based on
// "population changes by less than eps%" isn't quite enough for bimolecular/
// higher-order reactions, where propensity depends nonlinearly (e.g. binomial
// coefficients) on the population. g_i corrects for that nonlinearity.
std::vector<double> System::computeHOR() const
{
    std::vector<double> g(species.size(), 0.0);

    for(const auto& r: reactions){
        // total reaction order = sum of stoichiometric coefficients on the reactant side
        int order = 0;
        for(auto& [idx,nu]: r.reactants) order += nu;

        for(auto& [idx,nu]: r.reactants){
            double gi;
            if(order==1)                  gi = 1.0;                 // A -> ...        (linear, no correction needed)
            else if(order==2 && nu==1)    gi = 2.0;                 // A + B -> ...     (bimolecular, different species)
            else if(order==2 && nu==2)    gi = 2.0 + 1.0/std::max(1, species[idx]-1); // 2A -> ... (same species twice — correction shrinks as population grows)
            else if(order==3 && nu==1)    gi = 3.0;                 // A + B + C -> ...
            else                          gi = static_cast<double>(order); // rare higher-order cases, approximate

            // a species can appear as a reactant in several reactions —
            // keep the most restrictive (largest) g seen for it
            g[idx] = std::max(g[idx], gi);
        }
    }
    return g;
}

// Flags reactions as "critical" if they're close to running out of a reactant
// right now. A critical reaction is excluded from the Poisson leap below and
// instead only allowed to fire at most once per step (SSA-style) — this is
// what prevents species counts from going negative, since a Poisson draw has
// no built-in awareness of how much reactant is actually available.
std::vector<bool> System::identifyCritical(const std::vector<double>& propensities, int nc) const
{
    std::vector<bool> critical(reactions.size(), false);

    for(size_t j=0;j<reactions.size();++j){
        if(propensities[j] <= 0.0) continue;  // can't fire anyway, not a risk

        // L = the largest number of times this reaction could fire before
        // some reactant hits zero (the limiting reactant sets this)
        long L = std::numeric_limits<long>::max();
        for(auto& [idx,nu]: reactions[j].reactants)
            L = std::min(L, static_cast<long>(species[idx]/nu));

        // if fewer than nc firings would exhaust a reactant, treat it as critical
        // (nc ~ 10 is the standard default — "fewer than 10 firings left" is
        // considered too risky to trust to a Poisson draw)
        if(L < nc) critical[j] = true;
    }
    return critical;
}

// Computes the largest tau that keeps every species' expected relative change
// under eps (e.g. 3%) over the leap — this is the actual "leap condition" check,
// done numerically instead of guessed by hand. Only non-critical reactions
// contribute, since critical ones are handled separately (fired at most once,
// SSA-style) in stepAdaptive.
// Same as before, but never returns an unbounded tau — if no non-critical
// reaction constrains it, fall back to a generous multiple of the mean
// SSA waiting time (1/a0) instead of leaving it at DBL_MAX.
double System::computeTauCGPT(const std::vector<double>& propensities,
                               const std::vector<bool>& critical,
                               const std::vector<double>& g, double eps,
                               double a0) const
{
    std::vector<double> mu(species.size(),0.0), sigma2(species.size(),0.0);
    for(size_t j=0;j<reactions.size();++j){
        if(critical[j] || propensities[j]<=0.0) continue;
        auto apply = [&](int idx,int nu){
            mu[idx]     += nu * propensities[j];
            sigma2[idx] += nu*nu * propensities[j];
        };
        for(auto& [idx,nu]: reactions[j].reactants) apply(idx,-nu);
        for(auto& [idx,nu]: reactions[j].products)  apply(idx, nu);
    }

    double tau = std::numeric_limits<double>::max();
    for(size_t i=0;i<species.size();++i){
        if(g[i]<=0.0 || species[i]<=0) continue;
        double bound = std::max(eps*species[i]/g[i], 1.0);
        if(std::abs(mu[i])>0.0) tau = std::min(tau, bound/std::abs(mu[i]));
        if(sigma2[i]>0.0)       tau = std::min(tau, bound*bound/sigma2[i]);
    }

    // Nothing constrained it (e.g. every active reaction is critical this step) —
    // don't return DBL_MAX. Fall back to a bound tied to the overall reaction
    // rate scale, so the critical-reaction draw below is what actually governs
    // the step, not this leap bound.
    if(tau == std::numeric_limits<double>::max())
        tau = 100.0 / a0;

    return tau;
}

void System::printReactions() const
{
    for(size_t i=0;i<reactions.size();++i){
        std::cout << i << ": ";
        for(auto& [idx,nu]: reactions[i].reactants)
            std::cout << nu << names[idx] << "->";
        for(auto& [idx,nu]: reactions[i].products)
            std::cout << nu << names[idx] << "\n";
    }
}

// ================================================================
// Deterministic mean-field ODE solver
// ================================================================

void System::buildODEIndexMaps()
{
    ode_surf_idx.clear();
    ode_surf_pos.clear();
    ode_gas_idx.clear();
    ode_gas_pos.clear();

    int pos = 0;
    for(size_t idx=0; idx<species.size(); ++idx){
        if(reactor.isGas((int)idx)) continue;          // gas handled separately (as pressures)
        ode_surf_idx.push_back((int)idx);
        ode_surf_pos[(int)idx] = pos++;
    }
    for(auto& [idx, feed_p] : reactor.gas_species){
        if(reactor.reservoir.count(idx)) continue;      // pinned (infinite) reservoir gas -- not part of the state
        ode_gas_idx.push_back(idx);
        ode_gas_pos[idx] = pos++;
    }

    ode_n_surf = (int)ode_surf_idx.size();
    ode_n_gas  = (int)ode_gas_idx.size();
    ode_extent_offset = pos;
    ode_state_size = pos + (int)reactions.size();
}

// Mean-field dy/dt. Mirrors computePropensity + applyReactionEvent exactly,
// just with continuous populations instead of discrete counts/firings, so
// the same rate constants give consistent kinetics in both engines:
//   - gas reactant in an adsorption-type step: rate *= p^nu  (same as computePropensity)
//   - surface reactant: rate *= contFallingFactorial(n, nu)  (continuum limit of binomial(n,nu))
//   - gas species' derivative gets the reaction's per-molecule pressure contribution,
//     exactly the conversion applyReactionEvent uses (scaleup * kB * T / V)
//   - open-system gas species relax toward feed with time constant residence_time,
//     the continuum analogue of Reactor::updateGasPressures' analytic transport term
//   - reservoir-pinned gas species are excluded from the state (held fixed), matching
//     the `if(reservoir.count(idx)) continue;` skip in updateGasPressures
void System::computeODERHS(const std::vector<double>& y, std::vector<double>& dydt) const
{
    dydt.assign(y.size(), 0.0);

    constexpr double kB = 1.380649e-23;
    const double per_molecule_Pa = (reactor.volume > 0.0)
        ? reactor.scaleup * kB * reactor.temperature / reactor.volume : 0.0;

    auto surfAmount = [&](int idx)->double{
        auto it = ode_surf_pos.find(idx);
        return (it != ode_surf_pos.end()) ? y[it->second] : 0.0;
    };
    auto gasPressure = [&](int idx)->double{
        auto it = ode_gas_pos.find(idx);
        if(it != ode_gas_pos.end()) return y[it->second];
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

        dydt[ode_extent_offset + j] += rate;

        for(auto& [idx,nu] : r.reactants){
            if(reactor.isGas(idx)){
                auto it = ode_gas_pos.find(idx);
                if(it != ode_gas_pos.end()) dydt[it->second] -= nu*rate*per_molecule_Pa;
            } else {
                auto it = ode_surf_pos.find(idx);
                if(it != ode_surf_pos.end()) dydt[it->second] -= nu*rate;
            }
        }
        for(auto& [idx,nu] : r.products){
            if(reactor.isGas(idx)){
                auto it = ode_gas_pos.find(idx);
                if(it != ode_gas_pos.end()) dydt[it->second] += nu*rate*per_molecule_Pa;
            } else {
                auto it = ode_surf_pos.find(idx);
                if(it != ode_surf_pos.end()) dydt[it->second] += nu*rate;
            }
        }
    }

    if(!reactor.closed_system && reactor.residence_time > 0.0){
        for(auto& [idx, feed_p] : reactor.gas_species){
            auto it = ode_gas_pos.find(idx);
            if(it == ode_gas_pos.end()) continue;   // reservoir-pinned, skip
            double Pi = y[it->second];
            dydt[it->second] += (feed_p - Pi) / reactor.residence_time;
        }
    }
}

void System::computeODEJacobian(const std::vector<double>& y, const std::vector<double>& f0,
                                 std::vector<double>& J) const
{
    const int n = (int)y.size();
    J.assign((size_t)n*n, 0.0);
    std::vector<double> yp = y, fp(n);

    for(int i=0;i<n;++i){
        double yi = yp[i];
        double eps = 1e-6 * std::max(1.0, std::abs(yi));
        yp[i] = yi + eps;
        computeODERHS(yp, fp);
        for(int r=0;r<n;++r) J[(size_t)r*n + i] = (fp[r]-f0[r]) / eps;
        yp[i] = yi;
    }
}

bool System::rosenbrockEulerStep(std::vector<double>& y, double h) const
{
    const int n = (int)y.size();
    std::vector<double> f0(n);
    computeODERHS(y, f0);

    std::vector<double> J;
    computeODEJacobian(y, f0, J);

    // (I - h*J) k = f0 -- linearly-implicit ("W-method") Euler step. L-stable,
    // so it stays stable through fast adsorption/desorption pre-equilibria
    // without needing the tiny steps an explicit method would be forced into.
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

// ----------------- SSA -----------------
bool System::stepSSA(double& time, std::mt19937& rng)
{
   computeAllPropensities();

    double a0 = std::accumulate(propensities.begin(), propensities.end(),0.0);
    if(a0<=0.0) return false;

    std::uniform_real_distribution<double> U(0.0,1.0);
    double r1 = U(rng), r2 = U(rng);
    double dt = -std::log(r1)/a0;
    time += dt;

    double thresh = r2*a0;
    double accum = 0.0;
    size_t mu=0;
    for(size_t i=0;i<propensities.size();++i){
        accum += propensities[i];
        if(accum>=thresh){ mu=i; break; } // faster events will happen more frequently
    }

    Reaction& R = reactions[mu];
    reaction_extent[mu]++;

    std::vector<int> changed;
    // for(auto& [idx,nu]:R.reactants){ species[idx]-=nu; changed.push_back(idx);}
    // for(auto& [idx,nu]:R.products){ species[idx]+=nu; changed.push_back(idx);}
    // updatePropensities(changed);
    // reactor.applyFlow(species, dt, rng);

std::map<int,double> gas_delta; // zero-initialized per use via operator[], but explicit here for clarity
applyReactionEvent(mu, 1, changed, gas_delta); // one firing, second arg
reactor.updateGasPressures(dt, gas_delta);
syncGasReporting();

    if(step % saveFreq == 0)
    {
        // history.push_back(species);
        // times.push_back(time);
        extent_history.push_back(reaction_extent);

        writeHistory(logfile, time, *this); 
    } 

    return true;
}

// ----------------- Tau-leaping -----------------
bool System::stepTau(double& time, double tau, std::mt19937& rng)
{
    // printf("doing tau %lf tau: %lf\n", time, tau);
    computeAllPropensities();
    std::poisson_distribution<int> pois;
    bool any_event=false;
    std::vector<int> changed;
    // for(size_t i=0;i<reactions.size();++i){
    //     pois = std::poisson_distribution<int>(propensities[i]*tau);
    //     int nfirings = pois(rng);
    //     if(nfirings>0) any_event=true;
    //     for(auto& [idx,nu]:reactions[i].reactants){ species[idx]-= nu*nfirings; changed.push_back(idx);}
    //     for(auto& [idx,nu]:reactions[i].products){ species[idx]+= nu*nfirings; changed.push_back(idx);}

    //     reaction_extent[i] += nfirings;   // or += k in stepAdaptive
    // }

    // for(int idx: changed) if(species[idx] < 0) species[idx] = 0; // correct for possible negative overfiring
    // time += tau;
    // reactor.applyFlow(species,tau, rng);

std::map<int,double> gas_delta;
for(size_t i=0;i<reactions.size();++i){
    pois = std::poisson_distribution<int>(propensities[i]*tau);
    int nfirings = pois(rng);
    if(nfirings>0) any_event=true;
    applyReactionEvent(i, nfirings, changed, gas_delta);
}
time += tau;
reactor.updateGasPressures(tau, gas_delta);
syncGasReporting();


    if(step % saveFreq == 0)
    {
        // history.push_back(species);
        // times.push_back(time);
        extent_history.push_back(reaction_extent);

        writeHistory(logfile, time, *this); 
    } 
    // updatePropensities(changed);
    return any_event;
}

// One adaptive step: figures out tau on its own every call, handles
// "currently fast" reactions safely, and falls back to exact SSA when
// nothing can be safely leaped at all (e.g. right after a species count
// crashes near zero, or when everything present is critical).
bool System::stepAdaptive(double& time, std::mt19937& rng, double eps, int nc, int nSSAFallback)
{
    computeAllPropensities();
    double a0 = std::accumulate(propensities.begin(), propensities.end(), 0.0);
    if(a0<=0.0) return false;

    auto g        = computeHOR();
    auto critical = identifyCritical(propensities, nc);
    double tau    = computeTauCGPT(propensities, critical, g, eps, a0);  // now bounded

    if(tau < 10.0/a0){
        for(int i=0;i<nSSAFallback;++i)
            if(!stepSSA(time, rng)) break;
        return true;
    }

    double aCrit = 0.0;
    for(size_t j=0;j<reactions.size();++j) if(critical[j]) aCrit += propensities[j];

    // Draw when the *next critical* reaction would fire — this may be
    // much sooner than the leap bound tau.
    double tauCrit = std::numeric_limits<double>::max();
    if(aCrit>0.0){
        std::uniform_real_distribution<double> U(0.0,1.0);
        tauCrit = -std::log(U(rng))/aCrit;
    }

    // The actual elapsed time for this step is whichever comes first —
    // this is the fix: previously we always used `tau` here regardless.
    double tauUsed = std::min(tau, tauCrit);

    std::vector<int> changed;
std::map<int,double> gas_delta;

    // Fire the one critical reaction only if its draw actually fell inside this step
    if(tauCrit < tau && aCrit>0.0){
        std::uniform_real_distribution<double> U2(0.0,1.0);
        double thresh = U2(rng)*aCrit, accum=0.0;
        for(size_t j=0;j<reactions.size();++j){
            if(!critical[j]) continue;
            accum += propensities[j];
            if(accum>=thresh){
                // for(auto& [idx,nu]: reactions[j].reactants){ species[idx]-=nu; changed.push_back(idx);}
                // for(auto& [idx,nu]: reactions[j].products){  species[idx]+=nu; changed.push_back(idx);}
applyReactionEvent(j, 1, changed, gas_delta); // one firing, second arg
                break;
            }
        }
    }

    // Non-critical reactions leap over tauUsed (the true elapsed time),
    // not the nominal bound tau — this was the other bug.
    for(size_t i=0;i<reactions.size();++i){
        if(critical[i] || propensities[i]<=0.0) continue;
        std::poisson_distribution<int> pois(propensities[i]*tauUsed);
        int nfirings = pois(rng);
    applyReactionEvent(i, nfirings, changed, gas_delta);


    }

    for(int idx: changed) if(species[idx] < 0) species[idx] = 0; // correct for possible overfireing

    time += tauUsed;   // was: time += tau
    // reactor.applyFlow(species, tauUsed, rng);
reactor.updateGasPressures(tauUsed, gas_delta);
syncGasReporting();

    if(step % saveFreq == 0){
        // history.push_back(species);
        // times.push_back(time);
        extent_history.push_back(reaction_extent);

        writeHistory(logfile, time, *this);
    }
    // updatePropensities(changed);
    return true;
}

// ----------------- KMC -----------------
bool KMC::checkSteady(double& time)
{
    if(checkFreq == 0){
        std::cout << "WARNING: steady_check_freq is 0. Setting steady_check_freq to 10000\n";
        checkFreq = 10000;
        return false;
    }

    if(steadyWindow>0.0 && system.step % checkFreq == 0 && system.isSteadyState(steadyWindow, steadyTol)){
        if(steadyOnset < 0.0){
            // std::cout << "Steady state reached at t=" << time << " (step " << system.step << ")\n";
            steadyOnset = time;
        }
        else{
            // std::cout << "Steady state reached at t=" << time << " (step " << system.step << ") — stopping early.\n";
            return true;                
        }
    }
    else { steadyOnset = -1.0; } // reset to -1, probably the first one was spurius
    return false;
}

void KMC::runSSA(double t_end, size_t max_steps)
{
   std::cout << "Running standard Stochastic Simulation Algorithm (SSA) \n";

   // init
   // override any existing file
   initHistoryFile(system.logfile, system);
system.reaction_extent.assign(system.reactions.size(), 0);
system.extent_history.clear();

    double time=0.0;
    while(time<t_end){
        if(!system.stepSSA(time,rng)){
            std::cout << "STOPPED: a0<=0 (dead end) at t=" << time
                      << " step=" << system.step << "\n";
            return;
        }

	    if(max_steps > 0 && system.step > max_steps){
            std::cout << "STOPPED: max_step reached at t=" << time << "\n";
            return;
        }

	    system.step ++;

        // interrup with ctrl-c
        if(system.step % 1000 == 0 && PyErr_CheckSignals() != 0)
            throw py::error_already_set();

        // interrupt steady state
        if(checkSteady(time)) break;

    }
    //for(size_t i=0;i<max_steps && time<t_end;i++)
    //    if(!system.stepSSA(time,rng)) break;
}

void KMC::runTau(double tau, double t_end)
{
   std::cout << "Running tau-leaping Simulation \n";
   // init
   // override any existing file
   initHistoryFile(system.logfile, system);
system.reaction_extent.assign(system.reactions.size(), 0);
system.extent_history.clear();

    double time=0.0;
    while(time<t_end){
        if(!system.stepTau(time,tau,rng)) break;

	    system.step ++;

        // interrup with ctrl-c
        if(PyErr_CheckSignals() != 0)
            throw py::error_already_set();

        // interrupt steady state
        if(checkSteady(time)) break;

    }
}

void KMC::runAdaptive(double t_end, size_t max_steps, double eps, int nc, int nSSAFallback)
{
   std::cout << "Running Adaptive simulation (Calculating adaptive tau-leaping) \n";

    // init
    // override existing file
    initHistoryFile(system.logfile, system);
system.reaction_extent.assign(system.reactions.size(), 0);
system.extent_history.clear();

    double time = 0.0;

    while(time < t_end){
        if(!system.stepAdaptive(time, rng, eps, nc, nSSAFallback)){
            std::cout << "STOPPED: a0<=0 (dead end) at t=" << time
                      << " step=" << system.step << "\n";
            std::cout << "Final species:\n";
            for(size_t i=0;i<system.species.size();++i)
                if(system.species[i]>0)
                    std::cout << "  " << system.names[i] << " = " << system.species[i] << "\n";
            return;
        }
        system.step++;

        if(max_steps>0 && system.step > max_steps){
            std::cout << "STOPPED: max_step reached at t=" << time << "\n";
            return;
        }

        if(PyErr_CheckSignals() != 0) throw py::error_already_set();

        // interrupt steady state
        if(checkSteady(time)) break;

    }
    std::cout << "Completed normally, t=" << time << "\n";
}

// ----------------- Deterministic ODE run -----------------
void KMC::runODE(double t_end, double rtol, double atol,
                  double h_init, double h_min, double h_max, size_t max_steps)
{
    std::cout << "Running deterministic mean-field ODE integration "
                 "(linearized-implicit, stiff-capable)\n";

    system.buildODEIndexMaps();
    const int n = system.odeStateSize();

    std::vector<double> y(n, 0.0);
    for(int i=0;i<system.ode_n_surf;++i)
        y[i] = static_cast<double>(system.species[system.ode_surf_idx[i]]);
    for(int i=0;i<system.ode_n_gas;++i){
        int idx = system.ode_gas_idx[i];
        auto it = system.reactor.partial_pressure.find(idx);
        y[system.ode_n_surf+i] = (it != system.reactor.partial_pressure.end()) ? it->second : 0.0;
    }
    // reaction extents start at zero (default-initialized above)

    initHistoryFile(system.logfile, system);
    system.reaction_extent.assign(system.reactions.size(), 0.0);
    system.extent_history.clear();
    system.times.clear();
    system.history.clear();
    system.step = 0;
    steadyOnset = -1.0;

    if(h_max <= 0.0) h_max = std::max(t_end/20.0, h_init);

    auto recordSnapshot = [&](double time, const std::vector<double>& yv){
        for(int i=0;i<system.ode_n_surf;++i)
            system.species[system.ode_surf_idx[i]] = static_cast<int>(std::llround(yv[i]));
        for(int i=0;i<system.ode_n_gas;++i)
            system.reactor.partial_pressure[system.ode_gas_idx[i]] = yv[system.ode_n_surf+i];
        system.syncGasReporting();

        for(size_t j=0;j<system.reactions.size();++j)
            system.reaction_extent[j] = yv[system.ode_extent_offset+j];

        system.times.push_back(time);
        system.extent_history.push_back(system.reaction_extent);

        std::vector<double> row(system.species.size());
        for(size_t i=0;i<system.species.size();++i) row[i] = static_cast<double>(system.species[i]);
        system.history.push_back(row);

        writeHistory(system.logfile, time, system);
        system.step++;
    };

    recordSnapshot(0.0, y);

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
                recordSnapshot(t, y);
                if(checkSteady(t)) break;
            }
        }
        h = h_new;

        if(PyErr_CheckSignals() != 0) throw py::error_already_set();
    }

    if(system.times.empty() || system.times.back() < t) recordSnapshot(t, y);

    std::cout << "ODE integration completed: t=" << t
              << "  accepted steps=" << naccept
              << "  attempts=" << nattempt << "\n";
}


PYBIND11_MODULE(kmc, m) {
    py::class_<Reaction>(m, "Reaction")
        .def(py::init<>())
        .def_readwrite("reactants", &Reaction::reactants)
        .def_readwrite("products", &Reaction::products)
        .def_readwrite("rate", &Reaction::rate)
        // .def_readwrite("reversible", &Reaction::reversible)
        .def_readwrite("adsorption", &Reaction::adsorption)
        .def_readwrite("desorption", &Reaction::desorption)
        // .def_readwrite("Keq", &Reaction::Keq);
    ;

    py::class_<Reactor>(m, "Reactor")
        .def(py::init<>())
        .def_readwrite("closed_system", &Reactor::closed_system)
        .def_readwrite("residence_time", &Reactor::residence_time)
        .def_readwrite("gas_species", &Reactor::gas_species)
        .def_readwrite("reservoir", &Reactor::reservoir)
        .def_readwrite("partial_pressure", &Reactor::partial_pressure)   // if not already exposed via Reactor bindings
        // .def("applyFlow", &Reactor::applyFlow)
        .def("isGas", &Reactor::isGas)

        .def_readwrite("volume", &Reactor::volume)
        .def_readwrite("temperature", &Reactor::temperature)
        .def_readwrite("scaleup", &Reactor::scaleup)
        // .def("updateGasPressures", &Reactor::updateGasPressures)
    ;

    py::class_<System>(m, "System")
        .def(py::init<>())
        .def_readwrite("species", &System::species)
        .def_readwrite("reactions", &System::reactions)
        .def_readwrite("reactor", &System::reactor)
        .def_readwrite("history", &System::history)
        .def_readwrite("times", &System::times)
        .def_readwrite("reaction_extent", &System::reaction_extent)
        .def_readwrite("extent_history", &System::extent_history)
        .def_readwrite("save_freq", &System::saveFreq)
        .def_readwrite("step", &System::step)
        .def_readwrite("logfile", &System::logfile)
        //.def_property_readonly("names", [](System &self) -> System& { return self.system; })
        .def_readonly("names", &System::names)
        .def("addSpecies", &System::addSpecies)
        .def("getSpeciesIndex", &System::getSpeciesIndex)
        .def("addReaction", &System::addReaction)
        .def("print_reactions", &System::printReactions)
        .def("computeAllPropensities", &System::computeAllPropensities)
        .def("updatePropensities", &System::updatePropensities)
        .def("stepSSA", &System::stepSSA)
        .def("stepTau", &System::stepTau)
        .def_static("binomial", &System::binomial)
    ;

    py::class_<KMC>(m, "KMC")
        .def(py::init<System&, unsigned>(), py::arg("system"), py::arg("seed")=42)
        .def_property_readonly("system", [](KMC &self) -> System& { return self.system; })
        .def_readwrite("steady_window", &KMC::steadyWindow)
        .def_readwrite("steady_tol", &KMC::steadyTol)
        .def_readwrite("steady_check_freq", &KMC::checkFreq)
        .def_readonly("steady_onset", &KMC::steadyOnset)
        .def("runSSA", &KMC::runSSA)
        .def("runTau", &KMC::runTau)
        .def("runAdaptive", &KMC::runAdaptive,
            py::arg("t_end"),  py::arg("max_steps"), py::arg("eps")=0.03, py::arg("nc")=10, py::arg("nSSAFallback")=100)
        .def("runODE", &KMC::runODE,
            py::arg("t_end"), py::arg("rtol")=1e-6, py::arg("atol")=1e-6,
            py::arg("h_init")=1e-8, py::arg("h_min")=1e-14, py::arg("h_max")=-1.0,
            py::arg("max_steps")=2000000)
    ;
}
