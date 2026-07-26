#pragma once
#include "kmc_common.h"
#include <vector>
#include <string>
#include <unordered_map>
#include <map>
#include <random>

// ============================================================
// System: discrete-state holder for the stochastic engine (SSA /
// tau-leaping / adaptive tau-leaping). Owns integer species counts and
// drives propensity-based physics. Nothing here knows the ODE engine exists.
// ============================================================
class System 
{
public:
    std::vector<int> species; // discrete count of each species
    std::vector<Reaction> reactions;
    Reactor reactor;
    DependencyGraph dep_graph;
    std::vector<double> propensities;
    std::vector<double> next_fire_time;

    std::vector<std::vector<double>> history;   // full species-sized snapshots, for steady-state detection
    std::vector<double> times;
    std::vector<std::string> names; // list of species names

    std::vector<double> reaction_extent;
    std::vector<std::vector<double>> extent_history;

    long saveFreq = 100;
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

    // steady-state check: thin wrapper around the shared implementation,
    // kept as a method for API symmetry with ODESystem::isSteadyState.
    bool isSteadyState(double window, double tol) const {
        return isSteadyStateFromHistory(times, history, window, tol);
    }

    std::vector<double> computeHOR() const;
    std::vector<bool> identifyCritical(const std::vector<double>& propensities, int nc) const;
    double computeTauCGPT(const std::vector<double>& propensities,
                            const std::vector<bool>& critical,
                            const std::vector<double>& g, double eps, double a0) const;

    void applyReactionEvent(size_t, int, std::vector<int>&, std::map<int,double>&);
    void syncGasReporting();
    void printReactions() const;

    bool stepAdaptive(double& time, std::mt19937& rng,
                    double eps, int nc, int nSSAFallback);

    // stochastic algorithms
    bool stepSSA(double& time, std::mt19937& rng);
    bool stepTau(double& time, double tau, std::mt19937& rng);

private:
    std::unordered_map<std::string,int> name_to_idx;
};

// ============================================================
// KMC: drives System through SSA / tau-leaping / adaptive tau-leaping.
// ============================================================
class KMC 
{
public:
    KMC(System& sys, unsigned seed=42) : system(sys), rng(seed) {}

    System& system;
    std::mt19937 rng;
    SteadyStateTracker steady;

    void runSSA(double t_end, size_t max_steps);
    void runTau(double tau, double t_end);
    void runAdaptive(double t_end, size_t max_steps, double eps=0.03, int nc=10, int nSSAFallback=100);
};
