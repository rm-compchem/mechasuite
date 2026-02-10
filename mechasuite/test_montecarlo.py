#!/usr/bin/env python

from kmc import *
import json
import sys

with open(sys.argv[1]) as f:
    data = json.load(f)

kmc = kMC()
kmc.temperature = data["temperature"]
kmc.pressure = data["pressure"]

kmc.N_sites = data["surface"]["sites"]
kmc.n_empty = kmc.N_sites

for k, v in data["initial_values"].items():
    kmc.add_surface_species(k, v)

for k, v in data["mec"].items():
    ksp = k.split("=")
    A, B = ksp[0]+"*", ksp[1]+"*"
    kf = v["298"][0]
    kr = v["298"][1]
    kmc.add_reaction(Reaction(ReactionType.Surface, A, B, kf, 1))
    kmc.add_reaction(Reaction(ReactionType.Surface, B, A, kr, 1))

for k, v in data["adsorption"].items():
    gas = k
    surf = gas + "*"
    kmc.add_reaction(Reaction(ReactionType.Adsorption, surf,"", v["kf"], 1))
    kmc.add_reaction(Reaction(ReactionType.Desorption, surf,"", v["kr"], 1))

kmc.run(10)
