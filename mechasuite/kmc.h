#pragma once
#include <vector>
#include <string>
#include <unordered_map>
#include <map>
#include <random>
#include <set>

struct Reaction 
{
    std::unordered_map<int,int> reactants; // idx in species vector -> stoichiometry
    std::unordered_map<int,int> products;  // idx in species vector -> stoichiometry
    double rate;                           // forward rate
    bool reversible = false;               // use backward rate
    bool adsorption = false;               // gas->surface
    bool desorption = false;               // surface->gas
    // double Keq = 1.0;                      // equilibrium constant for detailed balance
};

struct Reactor 
{
    bool closed_system = true;
    double residence_time = -1.0; // PFR/CSTR
    // std::unordered_map<int,int> gas_species; // species index and count
    std::unordered_map<int,double> gas_species; // species index and count
    std::unordered_map<int,int> reservoir; // species index and count
    std::map<int,double> partial_pressure;   // Pa, indexed by gas species idx — constant, not simulated
    double volume = 1e-5;        // m^3, gas-phase volume of the reactor
    double temperature = 300.0;  // K
    double scaleup = 1.0;   // real_sites / simulated_sites

    void applyFlow(std::vector<int>& species, double dt, std::mt19937& rng);
    bool isGas(int idx) const { return gas_species.find(idx) != gas_species.end(); }
    void updateGasPressures(double, const std::map<int,double>& );
};

struct DependencyGraph 
{
    std::unordered_map<int,std::set<size_t>> species_to_reactions;
};

class System 
{
public:
    std::vector<int> species; // concentration/count of species
    std::vector<Reaction> reactions;
    Reactor reactor;
    DependencyGraph dep_graph;
    std::vector<double> propensities;
    std::vector<double> next_fire_time;

    std::vector<std::vector<int>> history;
    std::vector<double> times;
    std::vector<std::string> names; // list of species names

    std::vector<long long> reaction_extent;
    std::vector<std::vector<long long>> extent_history;

    int saveFreq = 100;
    size_t step = 0;

    std::string logfile = "kmc.log";

    System() = default;

    int addSpecies(const std::string& name, int initial);
    int getSpeciesIndex(const std::string& name) const;
    void addReaction(const Reaction& r);
    void buildDependencyGraph();
    void computeAllPropensities();
    void updatePropensities(const std::vector<int>& changed_species);
    double computePropensity(const Reaction& r) const;
    static double binomial(unsigned n, unsigned k);

    // new functions
    bool isSteadyState(double window, double tol) const;
    std::vector<double> computeHOR() const;
    std::vector<bool> identifyCritical(const std::vector<double>& propensities, int nc) const;
    double computeTauCGPT(const std::vector<double>& propensities,
                            const std::vector<bool>& critical,
                            const std::vector<double>& g, double eps, double a0) const;

    void applyReactionEvent(size_t, int, std::vector<int>&,std::map<int,double>&);
    void syncGasReporting();
    void printReactions() const;

    bool stepAdaptive(double& time, std::mt19937& rng,
                    double eps, int nc, int nSSAFallback);
    // new functions


    // stochastic algorithms
    bool stepSSA(double& time, std::mt19937& rng);
    bool stepTau(double& time, double tau, std::mt19937& rng);
private:
    std::unordered_map<std::string,int> name_to_idx;
};

class KMC 
{
public:
    KMC(System& sys, unsigned seed=42) : system(sys), rng(seed) {}

    System& system;
    std::mt19937 rng;
    double steadyWindow=0.0; // in seconds
    size_t checkFreq = 1000;
    double steadyTol = 0.05; // percentage of noise
    double steadyOnset = -1;
    bool checkSteady(double& time);
    void runSSA(double t_end, size_t max_steps);
    void runTau(double tau, double t_end);
    void runAdaptive(double t_end, size_t max_steps, double eps=0.03, int nc=10, int nSSAFallback=100);
};
