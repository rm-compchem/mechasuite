MechaSuite is an open-source modular software designed to streamline the analysis of quantum chemical reaction mechanisms. MechaSuite combines an intuitive data manager (MechaData), a molecular geometry editor (MechaEdit), and
a microkinetic modeling engine (MechaKinetics). It facilitates the calculation of thermodynamic and kinetic parameters from quantum chemical outputs, the visualization and editing of molecular structures, and the simulation of complex reaction networks.
This integration enables chemists to transition seamlessly from ab initio calculations to kinetic predictions in a user-friendly and efficient environment. MechaSuite is primarily implemented in Python, with its high-performance 3D visualization engine written
in C++ for optimal rendering and interactivity.

# Installation
## Using conda
The root directory contains the conda recipe for building and installing the conda package. Use the following commands to build and install mechasuite provided you have [miniconda](https://www.anaconda.com/docs/getting-started/miniconda/main) install:
```
conda install conda-build
git clone git@github.com:rm-compchem/mechasuite.git
cd mechasuite
conda-build conda-recipe -c conda-forge
conda create -n ms
conda install mechasuite --use-local
```
## Installing only python modules
If you are only interested in installing and using the python base modules, mechadata.py and mechakinetics.py, execute the following commands:
```
git clone git@github.com:rm-compchem/mechasuite.git
cd mechasuite
pip install -r requirements.txt
pip install .
```


