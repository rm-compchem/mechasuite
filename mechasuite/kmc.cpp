#include "kmc.h"
#include <cmath>
#include <stdexcept>
#include <numeric>
#include <random>
#include <iostream>
#include <algorithm>
#include <functional>
#include <iostream>
#include <fstream>

#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include "kmc.h"

namespace py = pybind11;

// Add to kmc.h: an enum + a way to retrieve the last stop reason
enum class StopReason { Running, ReachedTEnd, NoPropensity, MaxStepReached };
// ----------------- Reactor -----------------
// too agresive
// void Reactor::applyFlow(std::vector<int>& sp, double dt) 
// {
//     if(closed_system || residence_time <= 0) return;
//     for(auto& [idx,target] : gas_species){
//         if (reservoir.count(idx)){
//             sp[idx] = reservoir[idx];
//             continue;
//         }

//         double relax = (target - sp[idx])*dt/residence_time;
//         sp[idx] += static_cast<int>(relax);
//         if(sp[idx]<0) sp[idx]=0;
//     }
// }

void writeHistory(const std::string& filename, double time, const std::vector<int>& species)
{
   std::ofstream outfile;
   outfile.open (filename,  std::ios::app );
   if(!outfile.is_open()) {
      std::cout << "error opening logile\n";
      return;
   }
   
   outfile << time << "  " ;
   for( auto i: species){
   	outfile << i << "  ";
   }
   outfile << "\n";
   outfile.close();
}

void initHistoryFile(const std::string& filename, const std::vector<std::string>& names)
{
   std::ofstream outfile;
   outfile.open (filename,  std::ios::out );
   if(!outfile.is_open()) {
      std::cout << "error opening logile\n";
      return;
   }
   
   outfile << "Time  ";
   for (const auto name:names){
      outfile << name << " ";
   }
   
   outfile << "\n";
   outfile.close();
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

double System::computePropensity(const Reaction& r) const 
{
    double a = r.rate;
    for(auto& [idx, nu] : r.reactants){
        a *= binomial(species[idx], nu);
        if(a==0.0) return 0.0;
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
        if(accum>=thresh){ mu=i; break; }
    }

    Reaction& R = reactions[mu];
    std::vector<int> changed;
    for(auto& [idx,nu]:R.reactants){ species[idx]-=nu; changed.push_back(idx);}
    for(auto& [idx,nu]:R.products){ species[idx]+=nu; changed.push_back(idx);}
    // updatePropensities(changed);

    reactor.applyFlow(species, dt, rng);
    if(step % saveFreq == 0)
    {
        history.push_back(species);
        times.push_back(time);

        writeHistory(logfile, time, species); 
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
    for(size_t i=0;i<reactions.size();++i){
        pois = std::poisson_distribution<int>(propensities[i]*tau);
        int nfirings = pois(rng);
        if(nfirings>0) any_event=true;
        for(auto& [idx,nu]:reactions[i].reactants){ species[idx]-= nu*nfirings; changed.push_back(idx);}
        for(auto& [idx,nu]:reactions[i].products){ species[idx]+= nu*nfirings; changed.push_back(idx);}
    }
    time += tau;
    reactor.applyFlow(species,tau, rng);
    if(step % saveFreq == 0)
    {
        history.push_back(species);
        times.push_back(time);

        writeHistory(logfile, time, species); 
    } 
    updatePropensities(changed);
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

    // Fire the one critical reaction only if its draw actually fell inside this step
    if(tauCrit < tau && aCrit>0.0){
        std::uniform_real_distribution<double> U2(0.0,1.0);
        double thresh = U2(rng)*aCrit, accum=0.0;
        for(size_t j=0;j<reactions.size();++j){
            if(!critical[j]) continue;
            accum += propensities[j];
            if(accum>=thresh){
                for(auto& [idx,nu]: reactions[j].reactants){ species[idx]-=nu; changed.push_back(idx);}
                for(auto& [idx,nu]: reactions[j].products){  species[idx]+=nu; changed.push_back(idx);}
                break;
            }
        }
    }

    // Non-critical reactions leap over tauUsed (the true elapsed time),
    // not the nominal bound tau — this was the other bug.
    for(size_t j=0;j<reactions.size();++j){
        if(critical[j] || propensities[j]<=0.0) continue;
        std::poisson_distribution<int> pois(propensities[j]*tauUsed);
        int k = pois(rng);
        for(auto& [idx,nu]: reactions[j].reactants){ species[idx]-= nu*k; changed.push_back(idx);}
        for(auto& [idx,nu]: reactions[j].products){  species[idx]+= nu*k; changed.push_back(idx);}
    }
    for(int idx: changed) if(species[idx] < 0) species[idx] = 0;

    time += tauUsed;   // was: time += tau
    reactor.applyFlow(species, tauUsed, rng);

    if(step % saveFreq == 0){
        history.push_back(species);
        times.push_back(time);
        writeHistory(logfile, time, species);
    }
    updatePropensities(changed);
    return true;
}

// ----------------- KMC -----------------
void KMC::runSSA(double t_end, size_t max_steps)
{
   // init
   // override any existing file
   initHistoryFile(system.logfile, system.names);
   system.computeAllPropensities();

    double time=0.0;
    while(time<t_end){
        if(!system.stepSSA(time,rng)) break;
	    if(max_steps > 0 && system.step > max_steps) break;

	    system.step ++;

        // interrup with ctrl-c
        if(system.step % 1000 == 0 && PyErr_CheckSignals() != 0)
            throw py::error_already_set();

    }
    //for(size_t i=0;i<max_steps && time<t_end;i++)
    //    if(!system.stepSSA(time,rng)) break;
}

void KMC::runTau(double tau, double t_end)
{
   // override any existing file
   initHistoryFile(system.logfile, system.names);
   // override any existing file

    double time=0.0;
    while(time<t_end){
        if(!system.stepTau(time,tau,rng)) break;

	    system.step ++;

        // interrup with ctrl-c
        if(PyErr_CheckSignals() != 0)
            throw py::error_already_set();
    
    }
}

void KMC::runAdaptive(double t_end, size_t max_steps, double eps, int nc, int nSSAFallback)
{
  initHistoryFile(system.logfile, system.names);
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
    }
    std::cout << "Completed normally, t=" << time << "\n";
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
        .def("applyFlow", &Reactor::applyFlow)
        .def("isGas", &Reactor::isGas)
    ;

    py::class_<System>(m, "System")
        .def(py::init<>())
        .def_readwrite("species", &System::species)
        .def_readwrite("reactions", &System::reactions)
        .def_readwrite("reactor", &System::reactor)
        .def_readwrite("history", &System::history)
        .def_readwrite("times", &System::times)
        .def_readwrite("save_freq", &System::saveFreq)
        .def_readwrite("step", &System::step)
        .def_readwrite("logfile", &System::logfile)
        //.def_property_readonly("names", [](System &self) -> System& { return self.system; })
        .def_readonly("names", &System::names)
        .def("addSpecies", &System::addSpecies)
        .def("getSpeciesIndex", &System::getSpeciesIndex)
        .def("addReaction", &System::addReaction)
        .def("computeAllPropensities", &System::computeAllPropensities)
        .def("updatePropensities", &System::updatePropensities)
        .def("stepSSA", &System::stepSSA)
        .def("stepTau", &System::stepTau)
        .def_static("binomial", &System::binomial)
    ;

    py::class_<KMC>(m, "KMC")
        .def(py::init<System&, unsigned>(), py::arg("system"), py::arg("seed")=42)
        .def_property_readonly("system", [](KMC &self) -> System& { return self.system; })
        .def("runSSA", &KMC::runSSA)
        .def("runTau", &KMC::runTau)
        .def("runAdaptive", &KMC::runAdaptive,
            py::arg("t_end"),  py::arg("max_steps"), py::arg("eps")=0.03, py::arg("nc")=10, py::arg("nSSAFallback")=100)
    ;
}
