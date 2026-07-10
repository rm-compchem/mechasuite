#!/usr/bin/python3

import numpy as np
from scipy.integrate import odeint
from scipy import stats
import matplotlib.pyplot as plt
import matplotlib as mpl
from cycler import cycler
import sys
import json
import yaml
import re
import os
from mechasuite import kmc as kmc_core

INPUTFILE = ""

class Step(object):
    def __init__(self):
        self.reacs = []
        self.prods = []
        self.reac_coefs = []
        self.prod_coefs = []
        self.kfs = []
        self.krs = []
        self.index = 0

    def elem_from_string(self, string, type="r"):
        numbers = ["0", "1", "2", "3", "4", "5", "6", "7", "8", "9", ".", ","]
        string = string.lstrip()
        string = string.rstrip()
        coefstr, simbol = "", ""
        integer = True
        for i in string:
            if integer and i in numbers:
                coefstr += i
            else:
                integer = False
                simbol += i
        if not coefstr:
            coefstr = 1
        if type == "r":
            self.reacs.append(simbol)
            self.reac_coefs.append(float(coefstr))
        else:
            self.prods.append(simbol)
            self.prod_coefs.append(float(coefstr))
        return simbol, coefstr

    def ks_from_string(self, string):
        try:
            kf, kr = string.split()
            kf, kr = float(kf), float(kr)
        except ValueError:
            return False
        else:
            self.kfs.append(kf)
            self.krs.append(kr)
            return True

    def add_ks(self, kf, kr):
        self.kfs.append(kf)
        self.krs.append(kr)

    def coeff(self, spec):
        if spec in self.reacs:
            return self.reac_coefs[self.reacs.index(spec)]
        elif spec in self.prods:
            return self.prod_coefs[self.prods.index(spec)]
        else:
            return 0


class Mechanim(object):
    def __init__(self):
        self.steps = []
        self.temps = []
        self.species = []
        self.init_values = {}
        self.v0 = []
        self.t = np.linspace(0, 0.01, 10000)
        self.temp_index = 0
        self.conc = []
        self.rates = []
        self.orders = []
        self.inputfile = ""
        self.plotdic = {}

    def set_t(self, ti=0, tf=1, p=100):
        self.t = np.linspace(ti, tf, p)

    def update_init_values(self):
        for sp in self.species:
            if sp not in self.init_values:
                self.init_values[sp] = [0]
        self.v0 = [self.init_values[spec][0] for spec in self.species]

    def step_from_string(self, string):
        step = Step()
        reacs, prods = string.split("=")
        reacs = reacs.split("+")
        prods = prods.split("+")
        for r in reacs:
            species, _ = step.elem_from_string(r, "r")
            if species not in self.species:
                self.species.append(species)
        for p in prods:
            species, _ = step.elem_from_string(p, "p")
            if species not in self.species:
                self.species.append(species)
        self.steps.append(step)
        step.index = len(self.steps)

    def time_from_string(self, line):
        line = line.lower()
        line = line.replace("time", "")
        lsplit = line.split()
        if len(lsplit) == 3:
            f, l, p = float(lsplit[0]), float(lsplit[1]), int(lsplit[2])
            self.t = np.linspace(f, l, p)
            return True

        print("BAD INPUT FOR TIME. Format 'time first last npoints'. Ex: time 0 10 1000")
        return False

    def step(self, index):
        if len(self.steps) > index:
            return self.steps[index]
        else:
            return None

    def solve(self, y, t):
        dxs = []
        seqs = []
        for spec in self.species:
            dx = 0
            seq = ""
            for step in self.steps:
                dx1, dx2 = 1, 1
                seq1, seq2 = "", ""
                if spec in step.reacs:
                    for r, coef in zip(step.reacs, step.reac_coefs):
                        # get the index of r in species which is the same of y
                        ni = self.species.index(r)
                        # multiplico la constante por la concentracion n en y
                        dx1 *= y[ni]**coef
                        seq1 += r+"^"+str(coef)+"*"
                    seq1 = "-"+str(step.kfs[self.temp_index]) + seq1
                    dx1 *= -step.kfs[self.temp_index]
                    for r, coef in zip(step.prods, step.prod_coefs):
                        # get the index of r in species which is the same of y
                        ni = self.species.index(r)
                        # multiplico la constante por la concentracion n en y
                        seq2 += r + "^" + str(coef) + "*"
                        dx2 *= y[ni]**coef
                    seq2 = str(step.kfs[self.temp_index]) + seq2
                    dx2 *= step.krs[self.temp_index]
                    seq += " + "+str(step.coeff(spec))+"("+seq1 + " + " + seq2+")"
                    # print(spec, seq)
                    dx += (dx1+dx2)*step.coeff(spec)
                elif spec in step.prods:
                    for r, coef in zip(step.reacs, step.reac_coefs):
                        # get the index of r in species which is the same of y
                        ni = self.species.index(r)
                        # multiplico la constante por la concentracion n en y
                        dx1 *= y[ni]**coef
                        seq1 += r + "^" + str(coef) + "*"
                    seq1 = str(step.kfs[self.temp_index]) + seq1
                    dx1 *= step.kfs[self.temp_index]
                    for r, coef in zip(step.prods, step.prod_coefs):
                        # get the index of r in species which is the same of y
                        ni = self.species.index(r)
                        # multiplico la constante por la concentracion n en y
                        dx2 *= y[ni]**coef
                        seq2 += r + "^" + str(coef) + "*"
                    dx2 *= -step.krs[self.temp_index]
                    seq2 = "-"+str(step.kfs[self.temp_index]) + seq2
                    dx += (dx1+dx2)*step.coeff(spec)
                    seq += " + "+str(step.coeff(spec)) + "(" + seq1 + " + " + seq2 + ")"
                    # print(spec, seq)
                # else:
                #     print(spec, "not found in ", str(step.index))
                #     raise LookupError("Species "+spec+" not found in step !!!!!")

            dxs.append(dx)
            # print(spec, seq)
            seqs.append(seq)
        #print(len(seqs), seqs)
        if len(dxs) != len(y):
            raise RuntimeError("len of equations do not match number of species!")
        # print("------------------------------------------------------------------------")
        return dxs

    def write(self):
        out = os.path.splitext(INPUTFILE)[0]
        out += ".out"
        header = "t(s) "
        for spec in self.species:
                header += f"  {spec}  "
        
        arr = np.hstack((self.t.reshape(-1, 1), self.conc))
        np.savetxt(out,  arr, header=header)

    def simulate(self):
        self.conc = odeint(self.solve, self.v0, self.t)
        self.rates = [self.solve(i, 0) for i in self.conc]

    def plot(self):
        plot_stride=self.plotdic.get("slice", 1)
        plt.rcParams["font.size"] = 14
        lc = ['#123a4a', '#d18b2c', '#8b1e2d', '#4a90c0', '#2f6f6d', '#d65f4f', '#2c4f9e', '#7fa33b', '#7b3fa0', '#6b6b6b']
        mpl.rcParams['axes.prop_cycle'] = cycler('color', lc)
        plt.ylabel("Concentration")
        plt.xlabel("Time (s)")
        plt.ticklabel_format(style="sci", scilimits=[0, 0])
        for i, spec in enumerate(self.species):
            if max(self.conc[::, i]) > 0.000:
                plt.plot(self.t[::plot_stride], self.conc[::plot_stride, i], label=spec, linewidth=2.5)
        plt.legend()
        plt.show()

    def get_order(self):
        for index, spec in enumerate(self.species):
            v, c = [], []
            value0 = self.init_values[spec][0]
            if value0 == 0:
                value0 = 1
            for n in range(1, 8):
                value0 *= n
                self.v0[index] = value0
                self.simulate()
                # v.append(self.rates[1][-1])
                # c.append(self.conc[1][index])
                v.append(np.log(abs(self.rates[4][-1])))
                c.append(np.log(self.conc[4][index]))
            slope, n, r, p, std_err = stats.linregress(c, v)
            print(spec, slope, r**2)
            self.orders.append(slope)
            # plt.plot(v, c, label=spec)
        # plt.legend()
        # plt.show()

    def get_app_ae(self):
        if len(self.temps) <= 2:
            return
        v = []
        iT = []
        for n, temp in enumerate(self.temps):
            self.temp_index = n
            self.simulate()
            v.append(np.log(abs(self.rates[90][-1])))
            iT.append(1/temp)
        slope, n, r, p, std_err = stats.linregress(iT, v)
        print(self.species)
        #print("Ea: ", slope*-8.31, n, (n-1-np.log(1.38064852e-23)-np.log(1000)+np.log(6.62607004e-34))/8.31,  r**2) 
        # print("Ea: ", slope*-8.31, n, (n-1-np.log(1.38064852e-23)-np.log(298)+np.log(6.62607004e-34))*8.31,  r**2) # n ya es el logaritmo del factor preexponencial
        # NEEDS TO BE REVIEWED!!! CHECK THE UNITS
        print(f"Ea = {(slope*-8.31)/1e3} kJ/mol\n\
                Intercept = {n}\n\
                Pre-exponential factor (298 K) = {(n-1-np.log(1.38064852e-23)-np.log(298)+np.log(6.62607004e-34))*8.31} s^-1\n\
                R^2 = {r**2}")
        plt.plot(iT, v)
        plt.legend()
        plt.show()

def mec_from_dic(mec_dic):
    mec = Mechanim()
    stepindex = 0
    for step, kinval in mec_dic["mec"].items():
        mec.step_from_string(step)
        for temp, constants in kinval.items():
            if stepindex == 0:
                mec.temps.append(float(temp))
            step = mec.step(stepindex)
            step.add_ks(constants[0], constants[1])
        stepindex += 1
    mec.time_from_string(mec_dic["time"])
    
    for symbol, val in mec_dic["initial_values"].items():
        mec.init_values[symbol] = [val]
    mec.plotdic = mec_dic.get("plot", {})
    return mec

def mec_from_file(filename):
    if filename.endswith(".json"):
        with open(filename) as f:
            mec_dic = json.load(f)
            return mec_from_dic(mec_dic)

    with open(filename) as f:
        l = f.readlines()
    readmec = False
    mec = Mechanim()
    stepindex = 1
    step = None
    readks = False
    readinit = False
    for line in l:
        if line == "\n":
            continue
        if not line:
            continue
        if line.startswith("#"):
            continue
        if "End Mechanism" in line:
            readmec = False
            continue
        if "Mechanism" in line:
            readmec = True
            continue

        if readmec:
            try:
                mec.step_from_string(line)
                continue
            except:
                continue

        if "Temperature" in line:
            readks = True
            stepindex = 0
            temp = line.split()[1]
            mec.temps.append(float(temp))
            continue

        if "initial values" in line.lower():
            readks = False
            readinit = True
            continue

        if "time" in line.lower():
            mec.time_from_string(line)
            continue

        if readks:
            step = mec.step(stepindex)
            if step is None:
                readks = False
                continue
            step.ks_from_string(line)
            stepindex += 1
            continue

        if readinit:
            line = line.split()
            symbol = line[0]
            values = [float(v) for v in line[1:]]
            mec.init_values[symbol] = values

    return mec

def run_microkinetics(indic):
    #mec = mec_from_file(f)
    mec = mec_from_dic(indic)
    mec.update_init_values()
    mec.simulate()
    mec.write()
    mec.plot()


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

def compute_tof(times, extent_history, reaction_idx, t_start, t_end, n_sites):
    times = np.asarray(times)
    extent_history = np.asarray(extent_history)   # shape (n_snapshots, n_reactions)
    i0 = np.searchsorted(times, t_start)
    i1 = min(np.searchsorted(times, t_end), len(times)-1)
    if i1 <= i0 or i1 >= len(times):
        return 0.0
    dN = extent_history[i1, reaction_idx] - extent_history[i0, reaction_idx]
    dt = times[i1] - times[i0]
    return (dN / dt) / n_sites if dt > 0 else 0.0

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

def parse_reaction_kmc(eq, rdict, system, gas_species_names):
    P_standard = 1e5
    rate = rdict["298"]
    r1 = kmc_core.Reaction()
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
        r2 = kmc_core.Reaction()
        r2.reactants, r2.products = r1.products, r1.reactants
        r2.rate = float(rate[1])
        r2.adsorption = any(s in gas_species_names for s in side_species(rhs))

    # add units of pressure to the constants so propensities become
    # adimensional when multiplied by pressure in adsorptions and desorptions
    if r1.adsorption:
        r1.rate /= P_standard
    if r2 is not None and r2.adsorption:
        r2.rate /= P_standard

    return r1, r2

def build_gas_species_map(sys, data):
    """
    Full gas-phase population->target map for Reactor.applyFlow, including
    species that are only ever *produced* (e.g. desorption products) and
    never fed. The ones listed not listed in gas_count get 0.
    """
    feed = data.get("gas_count", {})
    all_gas_names = data.get("gas_species", list(feed.keys()))
    return {sys.getSpeciesIndex(name): feed.get(name, 0) for name in all_gas_names}

def residence_time_from_flow(molar_flow_rate, reactor_volume, pressure, temperature):
    R = 8.314  # J/(mol·K)
    """molar_flow_rate: mol/s, reactor_volume: m^3, pressure: Pa, temperature: K"""
    Q = molar_flow_rate * R * temperature / pressure   # volumetric flow, m^3/s
    return reactor_volume / Q 

def pressure_to_count(P, volume, temperature):
    kB = 1.380649e-23
    return int(round(P * volume / (kB * temperature)))

def load_gas_history():
    with open("kmc_gas.log") as f:
        header = f.readline()
        names = header.split()[1:]
    data = np.loadtxt("kmc_gas.log", skiprows=1)
    return names, data[:, 0], data[:, 1:].T

def run_kmc(data):
    solver = data.get("solver", "kmc") 
    sys = kmc_core.System()
    
    # init species and count
    allspec = get_species_dic_from_reac_str(list(data["mec"].keys())) # gets {A: 0, B:0 ...}
    allspec.update(data["surface_count"]) # init number of particles, gas are updated with partial pressure
    # allspec.update(data.get("gas_count", {}))  # becarefull to populate also the outlet gases
    for s, v in allspec.items():
        sys.addSpecies(s,v)

    # reactor
    reactor = kmc_core.Reactor()
    reactor.closed_system = data.get("closed", True)
    reactor.volume = data.get("reactor_volume", 1e-5)      # m^3
    reactor.temperature = data.get("temperature", 300.0)   # K

    # populate gas pressure. What is absent
    # Feed composition as partial pressures.
    P_tot = data.get("pressure", 1e5)              # 1 atm default
    fracs = data.get("gas_mole_fraction", {})
    if sum(fracs.values()) < 1:
        raise ValueError("PARTIAL PERSSURE MUST ADD UP TO 1")
    
    feed_p = {s: P_tot*gfrac for s, gfrac in fracs.items()}
    
    # gas_species similar to fee_p, gas species are the target
    # in the loop partial_pressure is updated with gas_species as reference.
    reactor.gas_species = {sys.getSpeciesIndex(s): pi for s, pi in feed_p.items()}
    reactor.partial_pressure = dict(reactor.gas_species)     # start at feed composition

    reactor.reservoir = {sys.getSpeciesIndex(s): v for s, v in data.get("reservoir", {}).items()}

    reactor.residence_time = data.get("residence_time") or residence_time_from_flow(
        data.get("molar_flow_rate", 1e-5), # 7×10⁻⁷ – 7×10⁻⁵ mol/s≈ 1–100 sccm (1 sccm ≈ 7.43×10⁻⁷ mol/s at 0 °C, 1 atm)
        reactor.volume,# 1×10⁻⁶ – 1×10⁻⁴ m³ (1–100 mL)
        data.get("total_pressure", 1e5), #1e5 Pa (1 atm) typical; up to ~1e6 Pa
        reactor.temperature,
    )

    sys.reactor = reactor
    sys.save_freq = data.get("save_freq", 100)
    sys.logfile = data.get("logfile", "kmc.log")

    # reactions
    for equation, rdict in data["mec"].items():
        r1, r2 = parse_reaction_kmc(equation, rdict, sys, feed_p.keys())
        sys.addReaction(r1)
        if r2 is not None:
            sys.addReaction(r2)

    # KMC engine
    kmc = kmc_core.KMC(sys, 123)
    kmc.steady_window = data.get("steady_window", 0) # in seconds to compare backwards
    kmc.steady_check_freq = data.get("steady_check_freq", 1000) # every how many to check steady state
    kmc.steady_tol = data.get("steady_tolerance", 0.05) # percentage of noise

    print(sys.species)
    print(sys.names)
    sys.print_reactions()
    print(reactor.gas_species)
    print(reactor.partial_pressure)
    print("res time: ", reactor.residence_time)
    # quit()

    if solver == "kmc_tau":
        tau = data.get("tau", 0.1)
        kmc.runTau(tau, data["simulation_time"])
    elif solver == "kmc_adaptive":
        kmc.runAdaptive(data["simulation_time"], int(1e+18))
    else:
        kmc.runSSA(data["simulation_time"], int(1e+18))

    names = sys.names
    hist = sys.history
    times = sys.times
    extent_history = sys.extent_history
    print(f"kmc steps: {sys.step}  ")
    
    # tof
    if "tof" in data:
        # t_ss = find_steady_state_onset(times, hist, window=0.05, tol=0.02) # very slow
        t_ss = 0
        if kmc.steady_onset > 0: t_ss = kmc.steady_onset

        tof = compute_tof(times, 
                      extent_history, 
                      data.get("tof", {}).get("reaction_idx", 0), 
                      t_ss, data.get("simulation_time"), 
                      data.get("tof", {}).get("sites", 1))
        print("TOF ", tof)
    # print(extent_history)
    print(reactor.gas_species)
    print("final partial pressure:", reactor.partial_pressure)

    # plotting
    gas_idx = set(reactor.gas_species.keys()) | set(reactor.reservoir.keys())

    fig, (ax_gas, ax_surf) = plt.subplots(1, 2, figsize=(10, 5), sharex=True)

    # --- Gas-phase species: partial pressure, feed (in) vs current outlet (out) ---
    labels, time, pressure_history = load_gas_history()
    print(len(labels), len(time), pressure_history.shape)
    for label, ph in zip(labels, pressure_history):
        ax_gas.plot(time, ph, label=label)

    ax_gas.set_ylabel("partial pressure (Pa)")
    ax_gas.set_title("Gas phase: feed (in) vs outlet (out)")
    ax_gas.legend()

    # --- Surface species: discrete population ---
    for i, name in enumerate(names):
        idx = sys.getSpeciesIndex(name)
        if idx in gas_idx:
            continue
        ax_surf.plot(times, [h[i] for h in hist], label=name)

    ax_surf.set_xlabel("time")
    ax_surf.set_ylabel("population")
    ax_surf.set_title("Surface species")
    ax_surf.set_xlim(0, data["simulation_time"])
    ax_surf.legend()

    plt.tight_layout()
    plt.show()

def main():
    global INPUTFILE
    try:
        filename = sys.argv[1]
    except IndexError:
        print(f"Usage: python {sys.argv[0]} input_file")
        sys.exit(0)

    INPUTFILE = filename

    mec_dic = {}
    if filename.endswith(".json"):
        with open(filename) as f:
            mec_dic = json.load(f)
    else: # try yaml       
        with open(filename) as f:
            mec_dic = yaml.safe_load(f)

    # default to kmc
    runtype = mec_dic.get("solver", "mk")
    if runtype.startswith("kmc"):
        run_kmc(mec_dic)
    elif runtype in ["diff_eq", "microkinetics", "mk"]:
        run_microkinetics(mec_dic)
    else:
        print("Bad solver. Please specify either kmc or microkinetics in input file")
        sys.exit(0)

    
if __name__== "__main__":
    main()
