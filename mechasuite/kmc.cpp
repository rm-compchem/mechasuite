#include "kmc.h"
#include <cmath>
#include <stdexcept>
#include <numeric>
#include <random>
#include <iostream>
#include <algorithm>
#include <functional>

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
    // backward rate for reversible
    // if(r.reversible){
    //     double prod = 1.0;
    //     for(auto& [idx, nu] : r.products){
    //         prod *= binomial(species[idx], nu);
    //         if(prod==0.0) return 0.0;
    //     }
    //     a += r.rate_backward * prod / std::max(r.Keq,1e-12);
    // }
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

    reactor.applyFlow(species, dt, rng);
    history.push_back(species);
    times.push_back(time);

    updatePropensities(changed);
    return true;
}

// ----------------- Tau-leaping -----------------
bool System::stepTau(double& time, double tau, std::mt19937& rng)
{
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
    history.push_back(species);
    times.push_back(time);
    updatePropensities(changed);
    return any_event;
}

// ----------------- KMC -----------------
void KMC::runSSA(double t_end, size_t max_steps)
{
    double time=0.0;
    size_t count = 0;
    while(time<t_end){
        if(!system.stepSSA(time,rng)) break;
	if(max_steps > 0 && count > max_steps) break;

	count ++;
    }
    //for(size_t i=0;i<max_steps && time<t_end;i++)
    //    if(!system.stepSSA(time,rng)) break;
}

void KMC::runTau(double tau, double t_end){
    double time=0.0;
    while(time<t_end){
        if(!system.stepTau(time,tau,rng)) break;
    }
}






#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include "kmc.h"

namespace py = pybind11;

PYBIND11_MODULE(kmc, m) {
    py::class_<Reaction>(m, "Reaction")
        .def(py::init<>())
        .def_readwrite("reactants", &Reaction::reactants)
        .def_readwrite("products", &Reaction::products)
        .def_readwrite("rate", &Reaction::rate)
        // .def_readwrite("reversible", &Reaction::reversible)
        .def_readwrite("adsorption", &Reaction::adsorption)
        .def_readwrite("desorption", &Reaction::desorption)
        .def_readwrite("Keq", &Reaction::Keq);

    py::class_<Reactor>(m, "Reactor")
        .def(py::init<>())
        .def_readwrite("closed_system", &Reactor::closed_system)
        .def_readwrite("residence_time", &Reactor::residence_time)
        .def_readwrite("gas_species", &Reactor::gas_species)
        .def_readwrite("reservoir", &Reactor::reservoir)
        .def("applyFlow", &Reactor::applyFlow)
        .def("isGas", &Reactor::isGas);

    py::class_<System>(m, "System")
        .def(py::init<>())
        .def_readwrite("species", &System::species)
        .def_readwrite("reactions", &System::reactions)
        .def_readwrite("reactor", &System::reactor)
        .def_readwrite("history", &System::history)
        .def_readwrite("times", &System::times)
        .def("addSpecies", &System::addSpecies)
        .def("getSpeciesIndex", &System::getSpeciesIndex)
        .def("addReaction", &System::addReaction)
        .def("computeAllPropensities", &System::computeAllPropensities)
        .def("updatePropensities", &System::updatePropensities)
        .def("stepSSA", &System::stepSSA)
        .def("stepTau", &System::stepTau)
        .def_static("binomial", &System::binomial);

    py::class_<KMC>(m, "KMC")
        .def(py::init<System&, unsigned>(), py::arg("system"), py::arg("seed")=42)
        .def_property_readonly("system", [](KMC &self) -> System& { return self.system; })
        .def("runSSA", &KMC::runSSA)
        .def("runTau", &KMC::runTau);
}
