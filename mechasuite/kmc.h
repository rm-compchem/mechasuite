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

    // NOTE: history / reaction_extent / extent_history are now double-valued
    // (were vector<int> / long long / vector<long long>). Nothing else relied
    // on the integer type, and this lets the ODE path (fractional extents,
    // fractional mean-field populations) share the exact same fields/plumbing
    // (isSteadyState, compute_tof, logging) as the stochastic engines.
    std::vector<std::vector<double>> history;
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

    // ---------------------------------------------------------------
    // Deterministic mean-field ODE solver.
    //
    // No spatial correlations => the master equation's thermodynamic
    // (N -> infinity) limit is just the mean-field rate ODEs for the
    // exact same Reaction/Reactor objects used by SSA/tau/adaptive above.
    // This solves that limit directly instead of averaging over many
    // stochastic trajectories.
    // ---------------------------------------------------------------

    // Packed ODE state vector y:
    //   y[0 .. ode_n_surf-1]                     -> surface species amounts
    //   y[ode_n_surf .. ode_n_surf+ode_n_gas-1]   -> free (non-reservoir) gas partial pressures [Pa]
    //   y[ode_extent_offset .. end]               -> cumulative reaction extents (one per reaction)
    std::vector<int> ode_surf_idx;
    std::unordered_map<int,int> ode_surf_pos;
    std::vector<int> ode_gas_idx;
    std::unordered_map<int,int> ode_gas_pos;
    int ode_n_surf = 0;
    int ode_n_gas  = 0;
    int ode_extent_offset = 0;
    int ode_state_size = 0;

    void buildODEIndexMaps();
    int  odeStateSize() const { return ode_state_size; }

    // dy/dt = f(y). Reuses the exact same rate law as computePropensity
    // (continuous generalization of the combinatorial `binomial` factor,
    // see contFallingFactorial in kmc.cpp) and the exact same gas<->pressure
    // bookkeeping as applyReactionEvent / Reactor::updateGasPressures.
    void computeODERHS(const std::vector<double>& y, std::vector<double>& dydt) const;

    // Numerical (finite-difference) Jacobian of computeODERHS at y, given f0=f(y).
    // J is row-major, size odeStateSize()^2.
    void computeODEJacobian(const std::vector<double>& y, const std::vector<double>& f0,
                             std::vector<double>& J) const;

    // One linearized-implicit ("Rosenbrock-Euler" / W-method) step of size h,
    // applied in place to y. L-stable, so it stays stable through the fast
    // adsorption/desorption pre-equilibria typical of single-site surface
    // kinetics without needing tiny steps the way an explicit method would.
    // Returns false if the linear solve is singular (caller should shrink h).
    bool rosenbrockEulerStep(std::vector<double>& y, double h) const;

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

    // Deterministic mean-field ODE integration of the same reaction network.
    // rtol/atol: adaptive step-size error tolerances (relative / absolute,
    //   mixed units in the state vector -- surface counts and gas Pa share
    //   one atol, so if that's an issue for your system tighten rtol instead).
    // h_init/h_min/h_max: initial, minimum, maximum step size [s]
    //   (h_max<=0 -> t_end/20).
    // max_steps: cap on step *attempts* (not just accepted steps), safety net.
    void runODE(double t_end, double rtol=1e-6, double atol=1e-6,
                double h_init=1e-8, double h_min=1e-14, double h_max=-1.0,
                size_t max_steps=2000000);
};
