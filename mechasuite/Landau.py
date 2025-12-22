import numpy as np
import math
import sys
import argparse
import os
import mechasuite.globs as globs
from scipy.integrate import quad

# -------------------------------
# Physical Constants
# -------------------------------
pi = np.pi
PLANCK_J = 6.62607015e-34             # Planck's constant (J·s)
HBAR_J = 1.054571817e-34       # Reduced Planck constant (J·s)
KB_J = 1.380649e-23            # Boltzmann constant (J/K)
R_SI = 8.314462618                # Gas constant (J/mol·K)
U_TO_KG = 1.66053906660e-27    # Atomic mass unit to kg
CM_TO_J = 1.98644586e-23       # Conversion: 1 cm⁻¹ to J
# use globs.NA for Avogadro's number
HETOJ_MOL = 2625.5002e3         # 1 Hartree to J/mol
BHTOM = 5.29177e-11            # Bohr to meter
GRADSTO_SI = HETOJ_MOL / BHTOM   # Gradient conversion to J/mol/m
EV_TO_J = 1.602176634e-19        # eV to J
KCAL_TO_EV = 0.0433641           # kcal to eV
KCAL_TO_J = 4184                 # kcal to J
# Functions to make the integral
def LZ_prob(E, H12, dF, mu, u):
    """
    Calculate the Landau-Zener probability.
    E: Energy in units 'u' (e.g. 'eV' or 'kcal') -- note: these are per-mol units in globs.conv
    H12: Coupling term (cm^-1)
    dF: Gradient difference (same units you previously used, will be converted)
    mu: Reduced mass (a.m.u.)
    u: units string as in globs.conv (e.g. 'eV', 'kcal', 'Ha')
    """
    if E <= 0:
        return 0.0
    # convert energy E to Joules per particle (divide J/mol by NA)
    E_J = float(E) #* globs.conv[u]["J"] / globs.NA
    # H12 expected in cm^-1 -> convert to J per particle
    H12_J = float(H12) * CM_TO_J
    # dF: convert to J (per mol -> per particle) / m
    df = float(dF) * GRADSTO_SI / globs.NA
    # reduced mass (a.m.u.) to kg
    mu_kg = float(mu) * U_TO_KG
    # guard against zero division
    if df == 0 or mu_kg == 0 or E_J <= 0:
        return 0.0
    gamma = (2.0 * np.pi * H12_J**2) / (HBAR_J * df * math.sqrt(2.0 * E_J / mu_kg))
    return math.exp(-gamma)

def max_boltz_distr(E, T, u):
    """
    Calculate the maximum Boltzmann distribution.
    E: Energy in units 'u'
    T: Temperature (K)
    """
    if E <= 0:
        return 0.0
    # convert to Joules per particle
    E_J = float(E) #* globs.conv[u]["J"] / globs.NA
    return math.exp(-E_J / (KB_J * T)) * (1.0 / (KB_J * T))

def integrand_single(E, H12, dF, mu, T, u):
    """
    Integrand for the Landau-Zener integral.
    E: Energy
    H12: Coupling term
    dF: Gradient difference
    mu: Reduced mass
    T: Temperature
    u: energy unit string
    """
    return (1.0 - LZ_prob(E, H12, dF, mu, u)) * max_boltz_distr(E, T, u)

def integrand_double(E, H12, dF, mu, T, u):
    """
    Integrand for the Landau-Zener integral (double passage).
    """
    p = LZ_prob(E, H12, dF, mu, u)
    return (1.0 - p**2) * max_boltz_distr(E, T, u)

# Calculation of the Landau-Zener rate
def landau_zener_rate(dG, H12, dF, mu, T, u, model="single"):
    """
    Calculate the Landau-Zener rate.
    H12: Coupling term in cm-1
    dF: Gradient difference in Ha/B
    mu: Reduced mass in a.m.u.
    T: Temperature in K
    u: units for energy (string)
    model: "single" or "double"
    """
    if model not in ["single", "double"]:
        raise ValueError("Model must be 'single' or 'double'")

    # Pre-factor: use local Planck and Boltzmann constants (SI)
    pre_factor = (KB_J * T) / PLANCK_J

    # Integral limits (E in units 'u')
    E_min = 0.0
    # convert 40*kT (J) into units 'u' for the integration upper limit
    # set integration upper limit as 40*kT in units 'u'
    E_max = (40.0 * KB_J * T)# / (globs.conv[u]["J"])

    # Choose the integrand
    integrand = integrand_single if model == "single" else integrand_double

    # integrate (pass unit 'u' through args)
    args = (H12, dF, mu, T, u)
    integral, _ = quad(integrand, E_min, E_max, args=args)

    # dG -> Joules for the Arrhenius-like prefactor
    dG_J = float(dG) * globs.conv[u]["J"]
    k_TS = pre_factor * math.exp(-dG_J / (R_SI * T))
    k_LZ = k_TS * integral
    print("Landau Zener probability integral:", integral)

    return k_LZ  # in s⁻¹
