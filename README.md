<p align="center">
  <img src="imgs/icon2.png" alt="Project Logo" width="200">
</p>

<h1 align="center">Mechasuite</h1>

MechaSuite is an open-source modular software designed to streamline the analysis of quantum chemical reaction mechanisms. MechaSuite combines an intuitive data manager (MechaData), a molecular geometry editor (MechaEdit), and
a microkinetic modeling engine (MechaKinetics). It facilitates the calculation of thermodynamic and kinetic parameters from quantum chemical outputs, the visualization and editing of molecular structures, and the simulation of complex reaction networks.
This integration enables chemists to transition seamlessly from ab initio calculations to kinetic predictions in a user-friendly and efficient environment. MechaSuite is primarily implemented in Python, with its high-performance 3D visualization engine written
in C++ for optimal rendering and interactivity.

# Installation
## Using conda
The root directory contains the conda recipe for building and installing the conda package. As MechaSuite is distributed as a conda package, it can be used across Windows, macOS, and Linux systems.
With [miniconda](https://www.anaconda.com/docs/getting-started/miniconda/main)  already installed, MechaSuite can be build and installed using the following commands:

```
  # Building MechaSuite
  $ conda activate base
  $ conda install conda-build
  $ git clone git@github.com:rm-compchem/mechasuite.git
  $ cd mechasuite
  $ conda-build conda-recipe -c conda-forge
  # Create a conda environment called 'ms' and install MechaSuite
  $ conda create -n ms
  $ conda activate ms
  $ conda install mechasuite --use-local
```
## Installing only python modules
If you are only interested in installing and using the python base modules, mechadata.py and mechakinetics.py, execute the following commands:
```
git clone git@github.com:rm-compchem/mechasuite.git
cd mechasuite
pip install -r requirements.txt
pip install .
```

# Using MechaSuite modules
If you have installed MechaSuite as a conda package, always activate the _ms_ environment first:
```
  $ conda activate ms
```

Then, MechaData graphical user interface (GUI) can be open by:
```
  $ mechadata.py
  # or opening directly a reaction mechanism from JSON file
  $ mechadata.py ${MS}/mechasuite/examples/example_2/fluorination.json
```

Here, _${MS}_ denotes the path to the directory where \mechasuite is installed. 
Likewise, MechaData GUI can be open by typing the following command:
```
  $ mechaedit
  # or opening directly a geometry file (CIF, XYZ, POSCAR or OUTCAR)
  $ mechaedit ${MS}/mechasuite/examples/example_2/SN2/SN2-TBAF.xyz
```

For MechaKin usage, indicate the JSON with the reaction mechanism:
```
  $ mechakinetics.py ${MS}/mechasuite/examples/example_1/rn.json
```

## Preprocessing scripts
Importing individual calculations to an already created mechanism can be done by providing the corresponding calculation directory. However, some preprocessing is convenient to avoid errors in trying the determine the format of the output of such calculations. To that end, we provide sample scripts that create a file named _data_ inside each calculation directory containing information about how _mechadata.py_ should read the files in the directory. Each line in the _.data_ file represents a configuration entry, specified as a _tag value_ pair. The following shows that the program of the QM calculation is VASP, the file with the electronic energy is OSZICAR, unit for the energy is in eV, the multiplicity of the calculation is singlet, the file with the geometry is CONTCAR, and that this is an optimization calculation rather than a transition state.  Other possible values are provided after the \# symbol.

```
program vasp    # gaussian, orca
energy  OSZICAR # gaussian or orca output file name or even just numerical value
struct  POSCAR  # any other xyz file
unit    eV      # kcal, kJ, Ha
spin    0       # 1, 2, etc
tp      min     # ts, ref
pg      solid   # C1, Cs, C2, C2v, C3v, C2h, Coov, D2h, D3h, D5h, Dooh, d3d, Td, Oh
```

