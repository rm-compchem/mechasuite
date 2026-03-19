Getting Started
###############



If you have installed MechaSuite as a conda package, always activate the *ms* environment first::

  $ conda activate ms


Then, MechaData graphical user interface (GUI) can be open by::

  $ mechadata


Or by directly opening a reaction mechanism from JSON file::

  $ mechadata ${MS}/mechasuite/examples/example_2/fluorination.json


Here, *${MS}* denotes the path to the directory where MechaSuite is installed. 

Likewise, MechaData GUI can be open by typing the following command::

  $ mechaedit


Or by directly opening a geometry file (CIF, XYZ, POSCAR or OUTCAR)::

  $ mechaedit ${MS}/mechasuite/examples/example_2/SN2/SN2-TBAF.xyz


For MechaKin usage, indicate the JSON with the reaction mechanism::

  $ mechakinetics ${MS}/mechasuite/examples/example_1/rn.json


Example1: Hypothetical sytem. First-order reaction 
===================================================

This example illustrates the general environment of Mechadata. The hypothetical reaction is a simple two-step mechanism occurring in a batch reactor, where the concentration of reactants changes over time until reaching an equilibrium. The reaction network is defined as follows:

R <=> I

I <=> P

where *R*, *I* and *P* are the hypothetical reactant, intermediate and product, respectively. The rate constants for both steps are set to be equal by assigning identical forward Gibbs free energies of activation, such as ΔG_act1_f  =  ΔG_act2_f = 17 kcal/mol. Using Eyring's equation, the corresponding forward rate constants at 298 K are k1f = k2f = 2.17 s^-1. To make the steps irreversible, the reverse constants are deliberately chosen to be small, k1r = k2r  10^-15 s^-1. This can be achieved by setting the ΔG for the reverse steps considerably higher than the forward ones, like ΔG_act1_r = ΔG_act2_r = 57 kcal/mol.

\Cref{fig:example1}a illustrates the main interface of mechadata, displaying the central spreadsheet that lists the hypothetical minima species (*R*, *I*, and *P*) together with the corresponding transition states (*TS1* and *TS2*) and their associated hypothetical energies. Their relative energies with respect to the global reference (denoted as *global_ref*), along with the corresponding activation and reaction energies, are displayed in the smaller spreadsheets on the right (relative energy and reaction energy panels in \Cref{fig:Mechadata}b,c). In this example, the *global_ref* serves merely as an arbitrary zero-energy reference for illustration.

 .. %Fig 4 - fig:example1

The interactive plotting interface displaying the corresponding energy profile derived from the reaction mechanism is shown in \Cref{fig:example1}b. The graphical elements representing minima and transition states, as well as their labels, can be customized in terms of style, color, line width, and position. Once the desired settings have been defined, a publication-ready version of the plot can be generated using Matplotlib (see the second example in the manuscript).

The concentration profiles in \Cref{fig:example1}c were obtained by numerically solving the system of differential equations using *mechakinetics*. The results illustrate that the concentration of *R* decreases over time, the intermediate *I* first increases and then declines after approximately 0.1 seconds, and the product *P* increases exponentially, behavior characteristic of a first-order reaction with respect to *R*.

Beyond their illustrative role, this and many other hypothetical examples provide a valuable framework for educational purposes. By systematically varying kinetic and thermodynamic parameters, users can explore how individual elementary steps influence the overall behavior of a reaction network. Such interactive exploration facilitates an intuitive understanding of reaction kinetics, sensitivity to model parameters, and the interplay between mechanism and observable rates, making these examples particularly well suited for teaching and training in microkinetic modeling.




Preprocessing scripts
=====================
Importing individual calculations to an already created mechanism can be done by providing the corresponding calculation directory. 

However, some preprocessing is convenient to avoid errors in trying the determine the format of the output of such calculations. 

To that end, we provide sample scripts that create a file named *.data* inside each calculation directory containing information about how *mechadata* should read the files in the directory. 

Each line in the *.data* file represents a configuration entry, specified as a *tag: value* pair, interpreted as a `yaml <https://yaml.org/>`_ file. 

The following shows that the program of the QM calculation is VASP, where the file with the electronic energy is OSZICAR, or the energy can be manually set with *energy* option.
The file with the geometry is CONTCAR, and the calculated vibrational frequencies in OUTCAR file.
The unit for the energy is in eV, the total spin moment of the calculation is zero, this is an optimization calculation rather than a transition state, and that the point group is for a solid.

Other possible values are provided after the \# symbol::

  program:      vasp    # gaussian, orca
  energy_file:  OSZICAR # gaussian or orca output file name or even just numerical value
  energy:       -25.3   # define energy manually
  struct_file:  CONTCAR # any other xyz file
  freq_file:    OUTCAR  # output with vibrational frequency     
  unit:         eV      # kcal, kJ, Ha
  spin:         0       # 1, 2, etc
  tp:           min     # ts, ref
  pg:           solid   # C1, Cs, C2, C2v, C3v, C2h, Coov, D2h, D3h, D5h, Dooh, d3d, Td, Oh
