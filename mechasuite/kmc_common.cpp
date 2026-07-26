#include "kmc_common.h"
#include <cmath>
#include <algorithm>
#include <iostream>

// Exact update of every gas species' partial pressure over the elapsed
// time dt of this KMC step. Two additive, physically separate terms:
//  1 transport: exact analytic solution of dP/dt=(feed-P)/tau, valid for
//      any dt, no noise, no stiffness.
//  2 reaction: the *actual* stoichiometric consequence of whatever fired
//      this step, applied at the end of the interval -- matches Gillespie
//      semantics (event occurs at t+dt).
void Reactor::updateGasPressures(double dt, const std::map<int,double>& delta_from_reactions)
{
    if(volume <= 0.0) return;
    for(auto& [idx, feed_p] : gas_species){
        if(reservoir.count(idx)) continue;   // pinned externally, skip integration

        double Pi = partial_pressure.count(idx) ? partial_pressure[idx] : feed_p;

        if(!closed_system && residence_time > 0.0)
            Pi = feed_p + (Pi - feed_p) * std::exp(-dt/residence_time);

        auto it = delta_from_reactions.find(idx);
        if(it != delta_from_reactions.end()) Pi += it->second;
        if(Pi < 0.0) Pi = 0.0;   // fp-roundoff guard only, not a physical clamp

        partial_pressure[idx] = Pi;
    }
}

void Reactor::applyFlow(std::vector<int>& sp, double dt, std::mt19937& rng)
{
    // ---- 1. Closed system  nothing happens
    if(closed_system) return;

    // ---- 2. Loop over gas species
    for(const auto& [idx,target] : gas_species)
    {
        auto res = reservoir.find(idx);
        if(res != reservoir.end())
        {
            sp[idx] = res->second;   // grand-canonical: enforce constant population
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

        if(sp[idx] < target) sp[idx] += delta;   // inflow
        else                 sp[idx] -= delta;   // outflow
        if(sp[idx] < 0) sp[idx] = 0;
    }
}

bool isSteadyStateFromHistory(const std::vector<double>& times,
                               const std::vector<std::vector<double>>& history,
                               double window, double tol)
{
    if(times.empty() || history.empty()) return false;
    double t_now = times.back();
    if(t_now < 2.0*window) return false;   // not enough elapsed time to judge yet

    auto idx_at = [&](double t){
        return static_cast<size_t>(std::lower_bound(times.begin(), times.end(), t) - times.begin());
    };
    size_t i0 = idx_at(t_now - 2.0*window);
    size_t i1 = idx_at(t_now - window);
    size_t i2 = times.size();
    if(i1 <= i0 || i2 <= i1) return false;   // window too fine relative to save-frequency resolution

    size_t nsp = history[0].size();
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

// NOTE (bugfix during the KMC/ODE split): the original checkSteady reset
// `onset` to -1 on *every* call where `step % checkFreq != 0` -- i.e. on
// almost every step, since it's called every iteration but only meant to
// actually re-evaluate every checkFreq steps. That made the "two
// consecutive confirmed checks -> stop" condition essentially unreachable
// for any checkFreq > 1 (onset was wiped out again before the next check
// could ever see it). Fixed here: `onset` is only touched on an actual
// check (step % checkFreq == 0), not on every call in between.
bool SteadyStateTracker::update(size_t step, double time,
                                 const std::vector<double>& times,
                                 const std::vector<std::vector<double>>& history)
{
    if(window <= 0.0) return false;   // steady-state stopping disabled

    if(checkFreq == 0){
        std::cout << "WARNING: steady_check_freq is 0. Setting steady_check_freq to 10000\n";
        checkFreq = 10000;
        return false;
    }

    if(step % checkFreq != 0) return false;   // not a check step -- leave `onset` untouched

    if(isSteadyStateFromHistory(times, history, window, tol)){
        if(onset < 0.0) onset = time;   // first confirmation -- remember when, keep going
        else            return true;    // confirmed again on the next check -- stop
    } else {
        onset = -1.0;                   // an actual check failed -- reset
    }
    return false;
}
