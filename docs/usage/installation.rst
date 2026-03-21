Installation
############

Using conda
===========
The root directory contains the conda recipe for building and installing the conda package. 
As MechaSuite is distributed as a conda package, it can be used across Windows, macOS, and 
Linux systems.

With `miniconda <https://www.anaconda.com/docs/getting-started/miniconda/main>`_  already 
installed, MechaSuite can be build using the following commands::

 
  $ conda activate base
  $ conda install conda-build
  $ git clone https://github.com/rm-compchem/mechasuite.git
  $ cd mechasuite
  $ conda-build conda-recipe -c conda-forge


Then, create a conda environment called *ms* and install MechaSuite::


  $ conda create -n ms -c conda-forge
  $ conda activate ms
  $ conda install mechasuite --use-local -c conda-forge
  

Installing only python modules
==============================
If you are only interested in installing and using the python-based modules, *mechadata* and 
*mechakinetics*, execute the following commands::

  $ git clone https://github.com/rm-compchem/mechasuite.git
  $ cd mechasuite
  $ pip install -r requirements.txt
  $ pip install .


