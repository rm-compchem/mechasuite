#pragma once
#include "kmc_common.h"
#include <vector>
#include <string>
#include <unordered_map>
#include <map>

class System; // fwd decl only -- ODESystem can be *built from* a System
              // snapshot, but never reaches back into one afterwards.

// ============================================================
// ODESystem: continuous-state holder for the deterministic mean-field
// engine. Owns double-valued "amounts" (site counts / gas pressures) and
// the RHS/Jacobian physics. Fully independent of System/KMC after
// construction -- no shared mutable state, no shared log files.
//
// The public shape (species, reactions, times, extent_history,
// getSpeciesIndex, ...) deliberately mirrors System's closely enough that
// Python helpers written against a KMC System (e.g. compute_tof) work
// unmodified against an ODESystem too.
// ============================================================
class ODESystem
{
public:
    // Builds the ODE's initial condition from an already-configured System:
    // copies the reaction network, reactor configuration (including current
    // feed/partial pressures), species names, and current species counts.
    // This is the normal way to get one -- Python still builds a single
    // `kmc_core.System()` regardless of which solver it ends up running,
    // and only wraps it in an ODESystem if it picks the ODE solver.
    explicit ODESystem(const System& sys);

    std::vector<double> species;         // continuous surface amounts; gas indices mirror partial_pressure cosmetically (see syncGasReporting)
    std::vector<Reaction> reactions;
    Reactor reactor;
    std::vector<std::string> names;

    std::vector<double> times;
    std::vector<std::vector<double>> history;         // full species-sized snapshots, for steady-state detection
    std::vector<double> reaction_extent;
    std::vector<std::vector<double>> extent_history;

    long saveFreq = 100;
    size_t step = 0;
    std::string logfile = "kmc_ode.log";              // deliberately distinct default from KMC's kmc.log

    int getSpeciesIndex(const std::string& name) const;

    bool isSteadyState(double window, double tol) const {
        return isSteadyStateFromHistory(times, history, window, tol);
    }

    // Packed ODE state vector y:
    //   y[0 .. n_surf-1]                -> surface species amounts
    //   y[n_surf .. n_surf+n_gas-1]     -> free (non-reservoir) gas partial pressures [Pa]
    //   y[extent_offset .. end]          -> cumulative reaction extents (one per reaction)
    std::vector<int> surf_idx;
    std::unordered_map<int,int> surf_pos;
    std::vector<int> gas_idx;
    std::unordered_map<int,int> gas_pos;
    int n_surf = 0;
    int n_gas  = 0;
    int extent_offset = 0;

    void buildIndexMaps();
    int  stateSize() const { return state_size; }

    // dy/dt = f(y). Continuous generalization of System::computePropensity
    // (see contFallingFactorial in kmc_ode.cpp for the combinatorial-factor
    // analogue) plus the reactor's gas<->pressure bookkeeping, mirroring
    // applyReactionEvent / Reactor::updateGasPressures in continuous form.
    void computeRHS(const std::vector<double>& y, std::vector<double>& dydt) const;

    // Numerical (finite-difference) Jacobian of computeRHS at y, given f0=f(y).
    // J is row-major, size stateSize()^2.
    void computeJacobian(const std::vector<double>& y, const std::vector<double>& f0,
                          std::vector<double>& J) const;

    // One linearized-implicit ("Rosenbrock-Euler" / W-method) step of size h,
    // applied in place to y. L-stable, so it stays stable through the fast
    // adsorption/desorption pre-equilibria typical of single-site surface
    // kinetics without needing tiny steps an explicit method would need.
    // Returns false if the linear solve is singular (caller should shrink h).
    bool rosenbrockEulerStep(std::vector<double>& y, double h) const;

    // Build y0 from the currently-committed state (species / partial_pressure
    // / reaction_extent).
    std::vector<double> packState() const;

    // Unpack y into species/partial_pressure/reaction_extent, mirror gas
    // pressures cosmetically into species[], push a (times, history,
    // extent_history) snapshot, and append a log line. This is the ODE
    // analogue of the "commit" that happens inside System's step functions.
    void unpackAndCommit(double time, const std::vector<double>& y);

    void initLogFile() const;
    void appendLogEntry(double time) const;
    std::string gasLogFilename() const;   // e.g. "kmc_ode.log" -> "kmc_ode_gas.log"

private:
    void syncGasReporting();
    std::unordered_map<std::string,int> name_to_idx;
    int state_size = 0;
};

// ============================================================
// MeanFieldODE: drives ODESystem through the adaptive linearized-implicit
// integration. Directly mirrors KMC's role for System.
// ============================================================
class MeanFieldODE
{
public:
    explicit MeanFieldODE(ODESystem& sys) : system(sys) {}

    ODESystem& system;
    SteadyStateTracker steady;

    // rtol/atol: adaptive step-size error tolerances (relative / absolute;
    //   mixed units share one state vector -- surface counts and gas Pa
    //   share one atol, so tighten rtol first if that becomes an issue).
    // h_init/h_min/h_max: initial, minimum, maximum step size [s]
    //   (h_max<=0 -> t_end/20).
    // max_steps: cap on step *attempts* (not just accepted steps), safety net.
    void run(double t_end, double rtol=1e-6, double atol=1e-6,
             double h_init=1e-8, double h_min=1e-14, double h_max=-1.0,
             size_t max_steps=2000000);
};
