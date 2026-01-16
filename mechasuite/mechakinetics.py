#!/usr/bin/python3

import numpy as np
from scipy.integrate import odeint
from scipy import stats
import matplotlib.pyplot as plt
import matplotlib as mpl
from cycler import cycler
import sys
import json


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

    def simulate(self):
        self.conc = odeint(self.solve, self.v0, self.t)
        self.rates = [self.solve(i, 0) for i in self.conc]

    def plot(self):
        plt.rcParams["font.size"] = 14
        lc = ['#123a4a', '#d18b2c', '#8b1e2d', '#4a90c0', '#2f6f6d', '#d65f4f', '#2c4f9e', '#7fa33b', '#7b3fa0', '#6b6b6b']
        mpl.rcParams['axes.prop_cycle'] = cycler('color', lc)
        plt.ylabel("Concentration")
        plt.xlabel("Time (s)")
        plt.ticklabel_format(style="sci", scilimits=[0, 0])
        for i, spec in enumerate(self.species):
            if max(self.conc[:, i]) > 0.005:
               plt.plot(self.t, self.conc[:, i], label=spec, linewidth=2.5)
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


def main():
    try:
        f = sys.argv[1]
    except IndexError:
        f = "reaction_network.csv"

    mec = mec_from_file(f)
    mec.update_init_values()
    mec.simulate()
    mec.plot()
    #mec.get_app_ae()
    #mec.get_order()
    # print("species: ", mec.species)
    # print(mec.temps)
    # print(mec.init_values)
    # for step in mec.steps:
    #     print("reacs", step.reacs)
    #     print("reacs coef", step.reac_coefs)
    #     print("prods", step.prods)
    #     print("prod coef", step.prod_coefs)
    #     print("kf", step.kfs)
    #     print("kr", step.krs)

    # totalcont = []
    # for i in range(len(mec.species)):
    #     totalcont.append([])
    # t0 = 0
    # span = 10
    # for i in range(1):
    #     mec.set_t(t0, t0+span, 10000)
    #     mec.simulate()
    #     t0 += span
    #     for i, c in enumerate(mec.concenctrations):
    #         mec.v0[i] = c[-1]
    #         totalcont[i] += list(c)
    #
    # print(totalcont[0][-1])
    # mec.simulate()

main()
