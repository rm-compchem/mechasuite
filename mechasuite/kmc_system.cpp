#include "kmc_system.h"
#include <cmath>
#include <stdexcept>
#include <numeric>
#include <random>
#include <iostream>
#include <algorithm>
#include <fstream>

#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

namespace py = pybind11;

// ----------------- logging (System-specific) -----------------
static void writeHistory(const std::string& filename, double time, System& sys)
{
   std::ofstream outfile;
   outfile.open(filename, std::ios::app);
   if(!outfile.is_open()){ std::cout << "error opening logile\n"; return; }

   outfile << time << "  ";
   for(auto i: sys.species) outfile << i << "  ";
   outfile << "\n";
   outfile.close();

   outfile.open("kmc_gas.log", std::ios::app);
   if(!outfile.is_open()){ std::cout << "error opening logile\n"; return; }

   outfile << time << "  ";
   for(auto& [idx, feed_p] : sys.reactor.gas_species)
       outfile << sys.reactor.partial_pressure[idx] << "  ";
   outfile << "\n";
   outfile.close();
}

static void initHistoryFile(const std::string& filename, System& sys)
{
   std::ofstream outfile;
   outfile.open(filename, std::ios::out);
   if(!outfile.is_open()){ std::cout << "error opening logile\n"; return; }
   outfile << "Time  ";
   for(const auto& name : sys.names) outfile << name << " ";
   outfile << "\n";
   outfile.close();

   outfile.open("kmc_gas.log", std::ios::out);
   if(!outfile.is_open()){ std::cout << "error opening logile\n"; return; }
   outfile << "Time  ";
   for(auto& [idx, feed_p] : sys.reactor.gas_species) outfile << sys.names[idx] << " ";
   outfile << "\n";
   outfile.close();
}

// Records a periodic snapshot: time, full species-sized history row (for
// steady-state detection), reaction extents, and the text log lines.
//
// NOTE (bugfix during the KMC/ODE split): `history`/`times` were declared
// on System and read by isSteadyState, but the push_back calls that would
// have populated them were commented out -- so isSteadyState always saw an
// empty trajectory and steady-state early-stopping silently never fired for
// SSA/tau/adaptive. Restored below.
static void recordSnapshot(System& sys, double time)
{
    sys.times.push_back(time);
    sys.history.push_back(std::vector<double>(sys.species.begin(), sys.species.end()));
    sys.extent_history.push_back(sys.reaction_extent);
    writeHistory(sys.logfile, time, sys);
}

// ----------------- System -----------------
void System::applyReactionEvent(size_t rxn_idx, int nfirings,
                                 std::vector<int>& changed_surface,
                                 std::map<int,double>& gas_delta_pressure)
{
    if(nfirings<=0) return;

    const Reaction& R = reactions[rxn_idx];
    constexpr double kB = 1.380649e-23;

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

void System::syncGasReporting()
{
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

void System::addReaction(const Reaction& r) { reactions.push_back(r); }

double System::binomial(unsigned n, unsigned k)
{
    if(k>n) return 0.0;
    if(k==0 || k==n) return 1.0;
    double res = 1.0;
    for(unsigned i=1;i<=k;++i){ res *= (n - (k-i)); res /= i; }
    return res;
}

double System::computePropensity(const Reaction& r) const
{
    double a = r.rate;
    for(auto& [idx, nu] : r.reactants){
        if(r.adsorption && reactor.isGas(idx)){
            auto it = reactor.partial_pressure.find(idx);
            double p = (it != reactor.partial_pressure.end()) ? it->second : 0.0;
            a *= std::pow(p, nu);
        } else {
            a *= binomial(species[idx], nu);
            if(a==0.0) return 0.0;
        }
    }
    return a;
}

void System::computeAllPropensities()
{
    propensities.clear();
    for(auto& r: reactions) propensities.push_back(computePropensity(r));
}

void System::updatePropensities(const std::vector<int>& changed_species)
{
    std::set<size_t> to_update;
    for(int s: changed_species){
        auto it = dep_graph.species_to_reactions.find(s);
        if(it!=dep_graph.species_to_reactions.end())
            to_update.insert(it->second.begin(),it->second.end());
    }
    for(size_t i: to_update) propensities[i] = computePropensity(reactions[i]);
}

std::vector<double> System::computeHOR() const
{
    std::vector<double> g(species.size(), 0.0);
    for(const auto& r: reactions){
        int order = 0;
        for(auto& [idx,nu]: r.reactants) order += nu;
        for(auto& [idx,nu]: r.reactants){
            double gi;
            if(order==1)                  gi = 1.0;
            else if(order==2 && nu==1)    gi = 2.0;
            else if(order==2 && nu==2)    gi = 2.0 + 1.0/std::max(1, species[idx]-1);
            else if(order==3 && nu==1)    gi = 3.0;
            else                          gi = static_cast<double>(order);
            g[idx] = std::max(g[idx], gi);
        }
    }
    return g;
}

std::vector<bool> System::identifyCritical(const std::vector<double>& propensities, int nc) const
{
    std::vector<bool> critical(reactions.size(), false);
    for(size_t j=0;j<reactions.size();++j){
        if(propensities[j] <= 0.0) continue;
        long L = std::numeric_limits<long>::max();
        for(auto& [idx,nu]: reactions[j].reactants)
            L = std::min(L, static_cast<long>(species[idx]/nu));
        if(L < nc) critical[j] = true;
    }
    return critical;
}

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
    if(tau == std::numeric_limits<double>::max()) tau = 100.0 / a0;
    return tau;
}

void System::printReactions() const
{
    for(size_t i=0;i<reactions.size();++i){
        std::cout << i << ": ";
        for(auto& [idx,nu]: reactions[i].reactants) std::cout << nu << names[idx] << "->";
        for(auto& [idx,nu]: reactions[i].products)  std::cout << nu << names[idx] << "\n";
    }
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

    reaction_extent[mu]++;
    std::vector<int> changed;
    std::map<int,double> gas_delta;
    applyReactionEvent(mu, 1, changed, gas_delta);
    reactor.updateGasPressures(dt, gas_delta);
    syncGasReporting();

    if(step % saveFreq == 0) recordSnapshot(*this, time);  // revisar AQUI

    return true;
}

// ----------------- Tau-leaping -----------------
bool System::stepTau(double& time, double tau, std::mt19937& rng)
{
    computeAllPropensities();
    std::poisson_distribution<int> pois;
    bool any_event=false;
    std::vector<int> changed;
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

    if(step % saveFreq == 0) recordSnapshot(*this, time);

    return any_event;
}

// ----------------- Adaptive tau-leaping -----------------
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
    double tau    = computeTauCGPT(propensities, critical, g, eps, a0);

    if(tau < 10.0/a0){
        for(int i=0;i<nSSAFallback;++i)
            if(!stepSSA(time, rng)) break;
        return true;
    }

    double aCrit = 0.0;
    for(size_t j=0;j<reactions.size();++j) if(critical[j]) aCrit += propensities[j];

    double tauCrit = std::numeric_limits<double>::max();
    if(aCrit>0.0){
        std::uniform_real_distribution<double> U(0.0,1.0);
        tauCrit = -std::log(U(rng))/aCrit;
    }
    double tauUsed = std::min(tau, tauCrit);

    std::vector<int> changed;
    std::map<int,double> gas_delta;

    if(tauCrit < tau && aCrit>0.0){
        std::uniform_real_distribution<double> U2(0.0,1.0);
        double thresh = U2(rng)*aCrit, accum=0.0;
        for(size_t j=0;j<reactions.size();++j){
            if(!critical[j]) continue;
            accum += propensities[j];
            if(accum>=thresh){
                applyReactionEvent(j, 1, changed, gas_delta);
                break;
            }
        }
    }

    for(size_t i=0;i<reactions.size();++i){
        if(critical[i] || propensities[i]<=0.0) continue;
        std::poisson_distribution<int> pois(propensities[i]*tauUsed);
        int nfirings = pois(rng);
        applyReactionEvent(i, nfirings, changed, gas_delta);
    }

    for(int idx: changed) if(species[idx] < 0) species[idx] = 0;

    time += tauUsed;
    reactor.updateGasPressures(tauUsed, gas_delta);
    syncGasReporting();

    if(step % saveFreq == 0) recordSnapshot(*this, time);

    return true;
}

// ----------------- KMC -----------------
void KMC::runSSA(double t_end, size_t max_steps)
{
    std::cout << "Running standard Stochastic Simulation Algorithm (SSA) \n";
    initHistoryFile(system.logfile, system);
    system.reaction_extent.assign(system.reactions.size(), 0.0);
    system.extent_history.clear();
    system.times.clear();
    system.history.clear();
    steady.onset = -1.0;

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

        // interrupt steady state REVISAR AQUI
        if(steady.update(system.step, time, system.times, system.history)) break;
    }
}

void KMC::runTau(double tau, double t_end)
{
    std::cout << "Running tau-leaping Simulation \n";
    initHistoryFile(system.logfile, system);
    system.reaction_extent.assign(system.reactions.size(), 0.0);
    system.extent_history.clear();
    system.times.clear();
    system.history.clear();
    steady.onset = -1.0;

    double time=0.0;
    while(time<t_end){
        if(!system.stepTau(time,tau,rng)) break;
        system.step++;

        // interrup with ctrl-c
        if(PyErr_CheckSignals() != 0) throw py::error_already_set();

        // interrupt steady state REVISAR AQUI
        if(steady.update(system.step, time, system.times, system.history)) break;
    }
}

void KMC::runAdaptive(double t_end, size_t max_steps, double eps, int nc, int nSSAFallback)
{
   std::cout << "Running Adaptive simulation (Calculating adaptive tau-leaping) \n";
    initHistoryFile(system.logfile, system);
    system.reaction_extent.assign(system.reactions.size(), 0.0);
    system.extent_history.clear();
    system.times.clear();
    system.history.clear();
    steady.onset = -1.0;

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

        // interrupt steady state REVISAR AQUI
        if(steady.update(system.step, time, system.times, system.history)) break;
    }
    std::cout << "Completed normally, t=" << time << "\n";
}
