import numpy as np
import matplotlib.pyplot as plt
import matplotlib as mpl
from cycler import cycler
import sys
import json
import yaml
import re
import os
from mechasuite import mk as mk_core

class Logger:
    def __init__(self):
        # override existing file
        with open("mechakinetics.log", "w") as f:
            pass
        self.outfile = open("mechakinetics.log", "a")
    
    def log(self, *args):
        for arg in args:
            print(arg, end=" ")
            #if not isinstance(arg, str):
            #    arg = str(arg)
            self.outfile.write(f"{arg} ")
        print()
        self.outfile.write("\n")
    
    # make sure file is closed
    def __del__(self):
        self.outfile.close()

def is_steady_state(times, history, t_check, window, tol=0.02):
    times = np.asarray(times)
    history = np.asarray(history)   # shape (n_snapshots, n_species)
    if t_check < 2*window:
        return False

    def avg_over(t0, t1):
        mask = (times >= t0) & (times <= t1)
        return history[mask].mean(axis=0) if mask.any() else None

    prev = avg_over(t_check - 2*window, t_check - window)
    curr = avg_over(t_check - window, t_check)
    if prev is None or curr is None:
        return False

    denom = np.maximum(prev, 1.0)          # avoid divide-by-zero for empty species
    rel_change = np.abs(curr - prev) / denom
    return np.all(rel_change < tol)

def find_steady_state_onset(times, history, window, tol=0.02):
    for t in times:
        if is_steady_state(times, history, t, window, tol):
            return t
    return None   # never reached steady state within the simulated time

def compute_tof(system, species, t_start, t_end, n_sites):
    # computes the tof as the net_prod = num_formation - num_consumption
    # NOTE: works unmodified for either a KMC `System` or an `ODESystem` --
    # both expose getSpeciesIndex/reactions/times/extent_history in the
    # same shape.
    tof = 0.0
    if species is None: return tof
    spec_idx = system.getSpeciesIndex(species)

    reac_form_idx = []
    reac_consum_idx = []
    for n, reac in enumerate(system.reactions):
        if spec_idx in reac.reactants:
            reac_consum_idx.append(n)
        if spec_idx in reac.products:
            reac_form_idx.append(n)

    times = np.asarray(system.times)
    extent_history = np.asarray(system.extent_history)   # shape (n_snapshots, n_reactions)
    i0 = np.searchsorted(times, t_start)
    i1 = min(np.searchsorted(times, t_end), len(times)-1)
    if i1 <= i0 or i1 >= len(times):
        return tof
    
    for reaction_idx in reac_form_idx:
        tof += extent_history[i1, reaction_idx] - extent_history[i0, reaction_idx]
    for reaction_idx in reac_consum_idx:
        tof -= extent_history[i1, reaction_idx] - extent_history[i0, reaction_idx]

    dt = times[i1] - times[i0]
    tof = tof / dt / n_sites
    #if tof < 0: tof = 0
    return tof

def compute_scaleup(sim_site_count, real_sites=None,
                     area_m2=None, site_density_per_m2=1e19,
                     mass_g=None, molar_mass=None, dispersion=None):
    if real_sites is None:
        if area_m2 is not None:
            real_sites = area_m2 * site_density_per_m2
        elif mass_g is not None and molar_mass is not None and dispersion is not None:
            N_A = 6.02214076e23
            real_sites = (mass_g / molar_mass) * N_A * dispersion
        else:
            raise ValueError("Provide real_sites, or area_m2, or mass/molar_mass/dispersion")
    return real_sites / sim_site_count

def apparent_activation_energy(temperatures, tofs):
    R = 8.314
    x = 1.0 / np.asarray(temperatures)
    y = np.log(np.asarray(tofs))
    slope, intercept = np.polyfit(x, y, 1)
    return -slope * R   # J/mol

def get_species_dic_from_reac_str(lofstr):
    # converts "2I+2P=3X" in a list: [I, P, X]
    allspec = {}
    for eqstr in lofstr:
        sp = eqstr.split("=")
        coeftuple = re.findall(r'(\d*)([A-Za-z_]\w*)', sp[0])
        for spec_pair in coeftuple:
            allspec[spec_pair[1]] = 0
        coeftuple = re.findall(r'(\d*)([A-Za-z_]\w*)', sp[1])
        for spec_pair in coeftuple:
            allspec[spec_pair[1]] = 0
    return allspec

def apply_rate_constant_rescaling(r1, r2, max_rate):
    """
    Chatterjee & Vlachos RCR: cap the fastest process the KMC clock has to
    resolve, without touching the thermodynamics of that step. If either
    direction of a reversible pair exceeds max_rate, scale BOTH directions
    by the same factor -- Keq = kf/kr is preserved exactly, so equilibrium
    coverage is untouched; only the *speed of equilibration* is capped.

    NOTE: this is a KMC-clock-resolution trick only. run_kmc() below skips
    it entirely when solver is "ode"/"kmc_ode" -- the ODE integrator steps
    however small it needs to (adaptively), so rescaling there would just
    quietly change the true kinetics rather than fix a numerical problem.
    """
    if r2 is None:
        if r1.rate > max_rate:
            r1.rate = max_rate      # irreversible step, no Keq to protect
        return
    fastest = max(r1.rate, r2.rate)
    if fastest > max_rate:
        factor = max_rate / fastest
        r1.rate *= factor
        r2.rate *= factor

def parse_reaction_kmc(eq, T, rdict, system, gas_species_names, max_rate=None):
    P_standard = 1e5
    rate = rdict[T]
    r1 = mk_core.Reaction()
    r2 = None
    lhs, rhs = eq.split("=")

    def counts(side):
        d = {}
        for n, s in re.findall(r'(\d*)([A-Za-z_]\w*)', side):
            nu = int(n) if n else 1
            idx = system.getSpeciesIndex(s)
            d[idx] = d.get(idx, 0) + nu
        return d

    def side_species(side):
        return re.findall(r'[A-Za-z_]\w*', side)

    r1.reactants = counts(lhs)
    r1.products  = counts(rhs)
    r1.rate = float(rate[0])
    # Pressure-driven propensity only matters when GAS appears as a
    # reactant (adsorption-type step). Desorption-type steps (gas only on
    # the product side) don't need the flag -- propensity there is governed
    # by the discrete adsorbed-species reactant, ordinary binomial path.
    r1.adsorption = any(s in gas_species_names for s in side_species(lhs))

    if len(rate) > 1:
        r2 = mk_core.Reaction()
        r2.reactants, r2.products = r1.products, r1.reactants
        r2.rate = float(rate[1])
        r2.adsorption = any(s in gas_species_names for s in side_species(rhs))

    # add units of pressure to the constants so propensities become
    # adimensional when multiplied by pressure in adsorptions and desorptions
    if r1.adsorption:
        r1.rate /= P_standard
    if r2 is not None and r2.adsorption:
        r2.rate /= P_standard

    if max_rate is not None:
        apply_rate_constant_rescaling(r1, r2, max_rate)

    return r1, r2

def residence_time_from_flow(molar_flow_rate, reactor_volume, pressure, temperature):
    R = 8.314  # J/(mol·K)
    """molar_flow_rate: mol/s, reactor_volume: m^3, pressure: Pa, temperature: K"""
    Q = molar_flow_rate * R * temperature / pressure   # volumetric flow, m^3/s
    return reactor_volume / Q 

def pressure_to_count(P, volume, temperature):
    kB = 1.380649e-23
    return int(round(P * volume / (kB * temperature)))

def load_history(filename):
    with open(filename) as f:
        header = f.readline()
        names = header.split()[1:]
    data = np.loadtxt(filename, skiprows=1)
    return names, data[:, 0], data[:, 1:].T

def run_kmc(data):
    logger = Logger()
    solver = data.get("solver", "kmc")
    is_ode = solver in ("kmc_ode", "ode")

    sys = mk_core.System()

    # init species and count
    allspec = get_species_dic_from_reac_str(list(data["mec"].keys())) # gets {A: 0, B:0 ...}
    allspec.update(data["surface_count"]) # init number of particles, gas are updated with partial pressure
    total_surface_species = sum(data["surface_count"].values())
    logger.log("total surface count ", total_surface_species)
    for s, v in allspec.items():
        sys.addSpecies(s,v)

    # reactor
    reactor = mk_core.Reactor()
    reactor.closed_system = data.get("closed", True)
    reactor.volume = float(data.get("reactor_volume", 1e-5))      # m^3
    reactor.temperature = data.get("temperature", 300.0)   # K

    # Feed composition as partial pressures.
    P_tot = data.get("pressure", 1e5)              # 1 atm default
    fracs = data.get("gas_mole_fraction", {})
    feed_p = {}
    if fracs and sum(fracs.values()) < 1:
        raise ValueError("PARTIAL PERSSURE MUST ADD UP TO 1")
    if fracs:
        feed_p = {s: P_tot*gfrac for s, gfrac in fracs.items()}
        logger.log("Using the following pressures in Pa: ", feed_p)
        reactor.gas_species = {sys.getSpeciesIndex(s): pi for s, pi in feed_p.items()}
        reactor.partial_pressure = dict(reactor.gas_species)     # start at feed composition

    reactor.reservoir = {sys.getSpeciesIndex(s): v for s, v in data.get("reservoir", {}).items()}

    reactor.residence_time = data.get("residence_time") or residence_time_from_flow(
        float(data.get("molar_flow_rate", 1e-5)),
        reactor.volume,
        data.get("total_pressure", 1e5),
        reactor.temperature,
    )

    sim_sites = sum(data["surface_count"].values())
    reactor.scaleup = data.get("scaleup") or compute_scaleup(
                    sim_sites, **data.get("catalyst_loading", {})
                    )
    logger.log(f"using reactor scaleup of: {reactor.scaleup:e}")

    sys.reactor = reactor
    sys.save_freq = data.get("save_freq", 100)
    sys.logfile = data.get("logfile", "kmc.log")

    # reactions
    # max_rate / RCR is a KMC-clock-resolution trick (see docstring of
    # apply_rate_constant_rescaling above) -- skip it entirely for the ODE
    # solver so its rates are never silently distorted.
    max_rate = data.get("max_rate", None)
    if max_rate and is_ode:
        logger.log("NOTE: ignoring 'max_rate' for the ODE solver (rate rescaling "
              "is only meaningful for the stochastic KMC clock).")
        max_rate = None
    if max_rate: max_rate = float(max_rate)
    for equation, rdict in data["mec"].items():
        r1, r2 = parse_reaction_kmc(equation, reactor.temperature, rdict, sys, feed_p.keys(), max_rate)
        sys.addReaction(r1)
        if r2 is not None:
            sys.addReaction(r2)

    logger.log(sys.species, sys.names, reactor.gas_species, reactor.partial_pressure)
    logger.log("residence time: ", reactor.residence_time)

    # ------------------------------------------------------------
    # Pick the engine. `sim_system` and `engine` are the two names the
    # rest of this function needs, regardless of which pair got built --
    # KMC driving `sys` directly, or MeanFieldODE driving a fresh
    # ODESystem built from `sys`.
    # ------------------------------------------------------------
    if is_ode:
        sim_system = mk_core.ODESystem(sys)
        sim_system.save_freq = sys.save_freq
        sim_system.logfile = data.get("logfile", "kmc.log")

        engine = mk_core.MeanFieldODE(sim_system)
        engine.steady_window = data.get("steady_window", 0)
        engine.steady_check_freq = data.get("steady_check_freq", 1000)
        engine.steady_tol = data.get("steady_tolerance", 0.05)

        # Deterministic mean-field limit of the exact same reaction network
        # (no spatial correlations -> valid for isolated, independent Cu
        # sites). Much faster than averaging many stochastic trajectories,
        # at the cost of losing site-to-site fluctuation information.
        engine.run(
            data["simulation_time"],
            rtol=data.get("ode_rtol", 1e-6),
            atol=data.get("ode_atol", 1e-6),
            h_init=data.get("ode_h_init", 1e-8),
            h_min=data.get("ode_h_min", 1e-14),
            h_max=data.get("ode_h_max", -1.0),
            max_steps=int(data.get("ode_max_steps", 2_000_000)),
        )
    else:
        sim_system = sys
        engine = mk_core.KMC(sys, 123)
        engine.steady_window = data.get("steady_window", 0)
        engine.steady_check_freq = data.get("steady_check_freq", 1000)
        engine.steady_tol = data.get("steady_tolerance", 0.05)

        if solver == "kmc_tau":
            tau = data.get("tau", 0.1)
            engine.runTau(tau, data["simulation_time"])
        elif solver == "kmc_adaptive":
            engine.runAdaptive(data["simulation_time"], int(1e+18))
        else:
            engine.runSSA(data["simulation_time"], int(1e+18))

    logger.log(f"kmc steps: {sim_system.step}  ")

    # tof
    tof = None
    if "tof" in data:
        t_ss = 0
        if engine.steady_onset > 0: t_ss = engine.steady_onset

        tof = compute_tof(sim_system,
                      data.get("tof", {}).get("species"),
                      t_ss, data.get("simulation_time"),
                      data.get("tof", {}).get("sites", 1))
    logger.log("TOF ", tof)
    logger.log("final partial pressure:", sim_system.reactor.partial_pressure)
    # report total surface count
    gas_idx = set(sim_system.reactor.gas_species.keys()) | set(sim_system.reactor.reservoir.keys())
    final_sum_surf = 0
    for n, species in enumerate(sim_system.species):
        if n in gas_idx:
            continue
        final_sum_surf += species
    logger.log("final surf sum: ", final_sum_surf) 
    sys.print_reactions()
    print("reaction extent: ", sim_system.reaction_extent)
    #print(sim_system.extent_history)
    
    # plotting
    if not data.get("plot", False): return tof
    fig, (ax_gas, ax_surf) = plt.subplots(1, 2, figsize=(10, 5), sharex=True)

    gas_logfile = sim_system.gasLogFilename() if is_ode else "kmc_gas.log"

    if gas_idx:
       labels, time, pressure_history = load_history(gas_logfile)
       pressure_history /= pressure_history.sum(axis=0, keepdims=True)  # instantaneous outlet mole fraction
       for label, ph in zip(labels, pressure_history):
           ax_gas.plot(time, ph, label=label)

       ax_gas.set_ylabel("partial pressure (Pa)")
       ax_gas.set_title("Gas phase: feed (in) vs outlet (out)")
       ax_gas.legend()

    labels, time, surface_history = load_history(sim_system.logfile)
    for name, sh in zip(labels, surface_history):
        idx = sim_system.getSpeciesIndex(name)
        if idx in gas_idx:
            continue
        ax_surf.plot(time, sh/total_surface_species, label=name)

    ax_surf.set_xlabel("time")
    ax_surf.set_ylabel("Coverage")
    ax_surf.set_title("Surface species")
    ax_surf.set_xlim(0, data["simulation_time"])
    ax_surf.legend()

    plt.tight_layout()
    plt.show()
    
    return tof

def run_kmc_ea(data):
    from scipy import stats
    temperatures = data.get("temperatures_ae")
    data["plot"] = False

    tofs, iT = [], []
    for temp in temperatures:
        data["temperature"] = temp
        tof = run_kmc(data)
        tofs.append(tof)
        
        iT.append(1/temp)
    slope, n, r, p, std_err = stats.linregress(iT, tofs)
    print(f"Ea = {(slope*-8.31)/1e3} kJ/mol\n\
            Intercept = {n}\n\
            Pre-exponential factor (298 K) = {(n-1-np.log(1.38064852e-23)-np.log(298)+np.log(6.62607004e-34))*8.31} s^-1\n\
            R^2 = {r**2}")
    plt.plot(iT, tofs)
    plt.legend()
    plt.show()

def main():
    try:
        filename = sys.argv[1]
    except IndexError:
        print(f"Usage: python {sys.argv[0]} input_file")
        sys.exit(0)

    mec_dic = {}
    if filename.endswith(".json"):
        with open(filename) as f:
            mec_dic = json.load(f)
    else: # try yaml       
        with open(filename) as f:
            mec_dic = yaml.safe_load(f)

    if "temperatures_ae" in mec_dic:
        run_kmc_ea(mec_dic)
    else:
        # default to kmc
        run_kmc(mec_dic)

    
if __name__== "__main__":
    main()
