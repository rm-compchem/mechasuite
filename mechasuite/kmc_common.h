#pragma once
#include <vector>
#include <string>
#include <unordered_map>
#include <map>
#include <random>
#include <set>

// ============================================================
// Data shared by both the stochastic (KMC) and deterministic
// (mean-field ODE) engines: the reaction network and the reactor
// it runs in. Neither struct knows or cares which engine is using it.
// ============================================================

struct Reaction
{
    std::unordered_map<int,int> reactants; // idx in species vector -> stoichiometry
    std::unordered_map<int,int> products;  // idx in species vector -> stoichiometry
    double rate;                           // forward rate
    bool reversible = false;               // use backward rate
    bool adsorption = false;               // gas->surface
    bool desorption = false;               // surface->gas
};

struct Reactor
{
    bool closed_system = true;
    double residence_time = -1.0; // PFR/CSTR
    std::unordered_map<int,double> gas_species;   // species index -> feed partial pressure [Pa]
    std::unordered_map<int,int> reservoir;        // species index -> pinned population (gas: infinite reservoir)
    std::map<int,double> partial_pressure;        // Pa, indexed by gas species idx -- current state
    double volume = 1e-5;        // m^3, gas-phase volume of the reactor
    double temperature = 300.0;  // K
    double scaleup = 1.0;        // real_sites / simulated_sites

    bool isGas(int idx) const { return gas_species.find(idx) != gas_species.end(); }

    // Discrete-time-step helpers used ONLY by the stochastic engine (System's
    // step functions): applyFlow mutates a discrete species vector using a
    // Poisson draw, updateGasPressures applies an exact analytic transport
    // term plus a discrete per-step reaction delta. The ODE engine integrates
    // the reactor's continuous analogue of both directly inside
    // ODESystem::computeRHS instead of calling these -- but the Reactor
    // *data* (closed_system, residence_time, gas_species, ...) is exactly
    // the same physical description either way, hence living here.
    void applyFlow(std::vector<int>& species, double dt, std::mt19937& rng);
    void updateGasPressures(double dt, const std::map<int,double>& delta_from_reactions);
};

struct DependencyGraph
{
    std::unordered_map<int,std::set<size_t>> species_to_reactions;
};

// ============================================================
// Steady-state detection, shared verbatim by KMC and MeanFieldODE.
//
// Both engines maintain a (times, history) trajectory of identical shape
// (history[i] = full species-sized snapshot at times[i]), so the windowed
// comparison itself needs exactly one implementation.
// ============================================================

// Compares the windowed average of species populations over
// [t_now-2*window, t_now-window] against [t_now-window, t_now]. Declares
// steady state when every species' relative change between the two windows
// is below tol. Works for a closed system settling to equilibrium and an
// open flow system settling to a driven steady state alike -- it only asks
// "have populations stopped drifting", not "is net rate zero".
bool isSteadyStateFromHistory(const std::vector<double>& times,
                               const std::vector<std::vector<double>>& history,
                               double window, double tol);

struct SteadyStateTracker
{
    double window = 0.0;      // seconds; <=0 disables steady-state early-stopping
    size_t checkFreq = 1000;  // only re-evaluate every `checkFreq` steps/snapshots
    double tol = 0.05;        // relative-change tolerance
    double onset = -1.0;      // <0 => not (yet) confirmed steady

    // Call once per step/snapshot with the engine's own step counter, current
    // time, and its (times, history) trajectory so far. Returns true once
    // steady state has been confirmed on two consecutive checks in a row
    // (i.e. the caller should stop the run).
    bool update(size_t step, double time,
                const std::vector<double>& times,
                const std::vector<std::vector<double>>& history);
};
