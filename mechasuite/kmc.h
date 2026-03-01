#pragma once
#include <vector>
#include <string>
#include <unordered_map>
#include <random>
#include <set>

struct Reaction 
{
    std::unordered_map<int,int> reactants; // species idx -> stoichiometry
    std::unordered_map<int,int> products;  // species idx -> stoichiometry
    double rate;                           // forward rate
    bool reversible = false;               // use backward rate
    bool adsorption = false;               // gas->surface
    bool desorption = false;               // surface->gas
    double Keq = 1.0;                      // equilibrium constant for detailed balance
};

struct Reactor 
{
    bool closed_system = true;
    double residence_time = -1.0; // PFR/CSTR
    std::unordered_map<int,int> gas_species; // target counts
    std::unordered_map<int,int> reservoir; // target counts
    void applyFlow(std::vector<int>& species, double dt, std::mt19937& rng);
    bool isGas(int idx) const { return gas_species.find(idx) != gas_species.end(); }
};

struct DependencyGraph 
{
    std::unordered_map<int,std::set<size_t>> species_to_reactions;
};

class System 
{
public:
    std::vector<int> species;
    std::vector<Reaction> reactions;
    Reactor reactor;
    DependencyGraph dep_graph;
    std::vector<double> propensities;
    std::vector<double> next_fire_time;

    std::vector<std::vector<int>> history;
    std::vector<double> times;

    System() = default;

    int addSpecies(const std::string& name, int initial);
    int getSpeciesIndex(const std::string& name) const;
    void addReaction(const Reaction& r);
    void buildDependencyGraph();
    void computeAllPropensities();
    void updatePropensities(const std::vector<int>& changed_species);
    double computePropensity(const Reaction& r) const;
    static double binomial(unsigned n, unsigned k);

    // stochastic algorithms
    bool stepSSA(double& time, std::mt19937& rng);
    bool stepTau(double& time, double tau, std::mt19937& rng);
private:
    std::vector<std::string> names;
    std::unordered_map<std::string,int> name_to_idx;
};

class KMC 
{
public:
    System& system;
    std::mt19937 rng;
    KMC(System& sys, unsigned seed=42) : system(sys), rng(seed) {}
    void runSSA(double t_end, size_t max_steps=1e7);
    void runTau(double tau, double t_end);
};