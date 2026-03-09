Usage
#####

Getting Started
========================
If you have installed MechaSuite as a conda package, always activate the *ms* environment first::

  $ conda activate ms


Then, MechaData graphical user interface (GUI) can be open by::

  $ mechadata
  # or opening directly a reaction mechanism from JSON file
  $ mechadata.py ${MS}/mechasuite/examples/example_2/fluorination.json


Here, *${MS}* denotes the path to the directory where MechaSuite is installed. 
Likewise, MechaData GUI can be open by typing the following command::

  $ mechaedit
  # or opening directly a geometry file (CIF, XYZ, POSCAR or OUTCAR)
  $ mechaedit ${MS}/mechasuite/examples/example_2/SN2/SN2-TBAF.xyz

For MechaKin usage, indicate the JSON with the reaction mechanism::

  $ mechakinetics ${MS}/mechasuite/examples/example_1/rn.json


Preprocessing scripts
=====================
Importing individual calculations to an already created mechanism can be done by providing the corresponding calculation directory. However, some preprocessing is convenient to avoid errors in trying the determine the format of the output of such calculations. To that end, we provide sample scripts that create a file named *data* inside each calculation directory containing information about how *mechadata* should read the files in the directory. Each line in the *.data* file represents a configuration entry, specified as a *tag: value* pair, interpreted as a \href{https://yaml.org/}{yaml} file. The following shows that the program of the QM calculation is VASP, the file with the electronic energy is OSZICAR, unit for the energy is in eV, the multiplicity of the calculation is singlet, the file with the geometry is CONTCAR, and that this is an optimization calculation rather than a transition state.  Other possible values are provided after the \# symbol::

  program:      vasp    # gaussian, orca
  energy_file:  OSZICAR # gaussian or orca output file name or even just numerical value
  struct_file:  POSCAR  # any other xyz file
  unit:         eV      # kcal, kJ, Ha
  spin:         0       # 1, 2, etc
  tp:           min     # ts, ref
  pg:           solid   # C1, Cs, C2, C2v, C3v, C2h, Coov, D2h, D3h, D5h, Dooh, d3d, Td, Oh


MechaData module
================

MechaData distinguishes itself through several design and functional advantages. Its key strengths include:
 * Spreadsheet-like interface: The column-based layout allows users to organize entire mechanisms, with each column representing a reaction mechanism (Figure S2a). This design offers a user-friendly interface that facilitates the analysis of reaction pathways, allowing users to focus their effort not on deciphering raw data, but on interpreting the underlying chemical processes and gaining meaningful mechanistic insight.

 * Automatic reference energy calculations: The reliable generation and comparison of reaction pathways depend on the precise calculation of relative energies based on appropriate reference species (Figure S2b), a procedure that is often laborious and susceptible to human error. One of MechaData’s most distinctive features is its ability to automatically compute relative energies for all intermediates, aligning them to user-defined reference states. This reduces manual effort and ensures consistency across large datasets. 

 * Built-in thermochemical tools: Vibrational frequency data can be scaled, edited, and directly used to calculate entropies, enthalpies, and Gibbs free energies. The platform seamlessly supports post-processing of this data, its visualization via integrated plotting tools, and its application in microkinetic modeling, enabling users to transition effortlessly from thermochemical calculations to kinetic analysis.
    
 * Integrated visualization: As another unique feature of MechaData, molecular struc-tures can be visualized directly within the interface (Figure S2d) or in the advanced MechaEdit module. This functionality enables users to easily inspect the system states at each step of the reaction pathway without the need to manage multiple geometry files or relying on external visualization packages. Such accessibility provides an intuitive, researcher-oriented experience that prioritizes the mechanistic understanding over manual file handling.

 * No programming required: Unlike many comparable tools, MechaData provides a fully featured graphical user interface (GUI, Figure S2a-d), requiring no coding knowledge and making the platform accessible to a broader community of chemists, including experimentalists seeking mechanistic insights. At the same time, users who prefer to work programmatically can export the mechanisms in JSON, allowing further customization or integration into computational workflows. Also, the post-processed data can be easily exported in structured formats (csv and xlsx files), facilitating reproducibility, data sharing and further analysis.

 * Direct plotting of free energy profiles: A key innovation of MechaData is its ability to generate free energy diagrams directly from the organized data, without the need for external plotting tools. This enables both reproducibility and customization as the diagrams can be visualized and readily adjusted (including style, formatting, labels and layout) before being exported as high-quality images using the Matplotlib python library.

.. figure:: Fig2.png

   Figure 2. MechaData interface. a) Main spreadsheet for organizing reaction mechanisms. b) Relative energy panel and c) reaction energy panel. d) Embedded visualizer for molecular structures. e) MechaEdit interface.


User interface and functionality
---------------------------------

The GUI of *mechadata* is designed to facilitate the setup, execution, and analysis of reaction network simulations through an intuitive, user-centric layout. It combines spreadsheet-style controls with interactive visual tools, while the menu structure is organized into logical categories for data management, visualization, plotting, and mechanistic editing. This section provides an overview of the main components of the GUI and describes the functionality of the available menus.

File Menu
-----------------------------------

  * (1) **Import Mechanism From Folder:** Loads a reaction mechanism from a directory containing several folders (one per calculation) with quantum chemical output files. *mechasuite* automatically extracts relevant thermochemical and structural data for each intermediate and transition state. It is advisable to run the preprocessing scripts first, to ensure the QM software and filenames are set properly. 

  * (2) **Import Mechanism From File:** Loads a reaction mechanism from a JSON file to the selected mechanism (column).
  
  * (3/4) **Save / Save As:** Saves the current workspace as a JSON file, including all mechanisms, structures, references, and calculated data. Useful for preserving project state.

  * (5) **Export to Excel:** Exports the reaction data sheet to an Excel file for external analysis or reporting.

  * (7-9) **Open / Close:** Opens an existing mechanism project or closes the current workspace.

.. figure:: Fig3.png

  Figure 3. Overview of *mechadata* GUI showing the available menus a–c) and the filtering options d), which allow to filter the calculations by temperature and type. 


Edit Menu
-------------------------------------
  * **(10) New Mechanism:** Creates a new reaction mechanism column within the workspace. Each column represents an independent mechanism and serves as a container for intermediates, transition states, and references.

View Menu 
---------
  * **(11) Plot:** Generates plots of energy profiles or kinetic trends based on the currently selected mechanism and data. Users can visualize free energy diagrams, reaction coordinates, or custom thermodynamic plots.


Right-Click Options on Columns (Mechanism-Level Controls)
---------------------------------------------------------
  * **(14) New Item:** Adds a new row to the current mechanism for a structure, intermediate, or transition state.
  
  * **(15) Order Items:** Change the order of the items in a column.

  * **(16) Merge Items:** Merges intermediates to simplify the mechanism representation. 
 
  * **(17) Import Intermediate:** Adds new species to the mechanism by importing them from external quantum chemical output.

  * **(18) Add Reference:** Adds a new energy reference by combining several reference items.

  * **(19) Convert Units:** Converts energy units (e.g., Hartree to kJ/mol) across the mechanism for consistency.

  * **(20) Change Mechanism Name:** Renames the mechanism column.

  * **(21/22) Delete Plot / Delete References:** Removes specific plots or reference entries from the mechanism.


.. figures s3 and s4

Right-Click Options on Row Entries (Species-Level Controls)}
------------------------------------------------------------

  * **(27-30) Frequencies:** Opens a panel to load, view, scale, or edit vibrational frequencies from quantum chemical calculations.

  * **(31-33) Thermochemical Analysis:** Computes thermodynamic parameters (G, H, S) based on vibrational and structural data.

  * **(34-36) Plot:** Generates energy profiles or add selected intermediates to existing free energy profiles.

  * **(37/38) Add Reference / Add Reactant:** Designates the selected species as a reference or adds it as a reactant to the mechanism.

  * **(39) Set Type (Minimum, Transition State, Reference):** Categorizes the species for use in kinetics and visualization.
  * 
  * **(40) Set Point Group:** Assigns a symmetry point group, which may be used in entropy corrections.

  * **(41) Set Spin Multiplicity:** Assigns spin multiplicity for electronic entropy.

  * **(42) Edit Comment:** Adds or modifies user annotations for the species.
  * 
  * **(43) Invert Selection:** Toggles current selections. Useful for batch operations.

  * **(44) Load Structure:** Imports molecular geometry data from a file.

  * **(45) Copy to Mechanism (Column):** Duplicates the selected species into another column.

  * **(46) Duplicate Item:** Creates a copy of the current entry.

  * **(47) Merge Items:** Create a new intermediate by grouping individual intermediates or transition states. The effect would be to sum up individual energies.

  * **(48) Update Item:** Updates the calculation results, energy, geometry, vibrational frequencies, etc., from the calculation folder. The calculation folder must have the same name as the intermediate, and be located in the same working directory.

  * **(49) Update Item From Folder:** Updates the calculation results, energy, geometry, vibrational frequencies, etc., from the specified folder.

  * **(50) Update Selection From Folder:** Updates the calculation results, energy, geometry, vibrational frequencies, etc., from the specified folder. The folder must contain subfolders matching the names of the selected items.
  * 
  * **(52) Export Thermochemical CSV:** Saves thermodynamic data for the selected species to a CSV file.

  * **(53) Remove 1/3 of Translation Entropy:** Applies a correction often used for adsorbed species in surface chemistry.\cite{Rzepa2020Jul}

  * **(56) Import Structure Visual State:** Modifies, for the selected items, the rotation matrix used to show the geometry in the embedded visualizer.

  * **(57) Show Structure in MechaEdit:** Opens the species in *MechaEdit*, the advanced structure editor, for detailed modification or inspection.

This structured interface enables users to manage complex quantum chemical data with clarity and flexibility, supporting advanced workflows for thermochemical analysis and kinetic modeling in a research context.



Calculation of relative energies
=================================
In the second example of the manuscript, the first three minima in every column, %(highlighted by the light \hl{purple box in} \Cref{fig:spreadsheet2}a) ethyl bromide, TBOH, and TBAF, are not directly involved in the reaction pathways but are used as reference species to compute the relative energies of all other intermediates. They form a basis to calculate the relative energies of all intermediates and transition states of a mechanism.
A corresponding reference energy can be then calculated as a linear combination of the energies of these reference geometries (Equation \ref{glob_ref}).

.. math::
  \textrm{E}_{ref} = a \  \textrm{E}_{CH_3CH_2Br} \  + \ b \  \textrm{E}_{TBAF} \ + \  c \  \textrm{E}_{TBOH}


\noindent Here, *a*, *b*, and *c* are coefficients automatically determined by \mechadata through the solution of a system of linear equations.
Subsequently, the relative energy of any structure $X$ ($\textrm{E}_{rel}(X)$, Equation \ref{rel_en}) is then computed by subtracting the reference energy ($\textrm{E}_{ref}$, using Equation \ref{glob_ref}) to the absolute energy of $X$ ($\textrm{E}_{abs}(X)$).

.. math:: 
   \textrm{E}_{rel}(X) = \textrm{E}_{abs}(X) - \textrm{E}_{ref}


In this example, we define an intermediate, denoted *REACTANT*, corresponding to the minimum-energy state preceding the SN2 and E2 transition states. *REACTANT* is constructed by merging two separate calculations, the geometry optimization of ethyl bromide and the optimization of TBAF microsolvated by 0, 1, 2, or 3 TBOH molecules. Consequently, the absolute energy of *REACTANT* is given by the sum of the energies obtained from these individual optimizations.
For *REACTANT* without TBOH, the reference coefficients in Equation \ref{glob_ref} are *a=b=1* and *c=0*.
These coefficients may vary depending on the atomic composition of each minimum and transition-state structure in the mechanism, ensuring that all energies remain comparable. For example, when \textit{REACTANT} is defined by merging optimized ethyl bromide with optimized TBAF in the presence of one TBOH molecule, the coefficients become *a=b=c=1*. If two TBOH molecules are present, the coefficient *c* increases accordingly to 2.

Importantly, the reference coefficients associated with a given intermediate are independent of how the structure is generated, being identical whether the intermediate comes from a single optimization including all molecules or from merging individually optimized geometries.

By referencing every structure in the mechanism to this common basis and through the appropriate coefficients, all species can be consistently included within the same free-energy profile. This merging and referencing scheme provides a flexible definition of intermediates, avoids unnecessary additional calculations, and removes the need for manual coefficient assignment when computing relative energies. Moreover, because reaction and activation energies are evaluated relative to a common reference basis, overall energy consistency is inherently ensured.

*mechaedit* module
==================
Although the basic geometry visualization capabilities are integrated into \mechadata, \mechaedit is specifically designed for detailed structural analysis and editing. In addition to the menu bar (\Cref{fig:mechaedit}a), the GUI is composed of a toolbar (\Cref{fig:mechaedit}b), the main drawing canvas (\Cref{fig:mechaedit}c), and a side panel (\Cref{fig:mechaedit}d). The most relevant options are described below.

..    \includegraphics[width=\linewidth]{Figs/Figure_S3.png}

Toolbar
-------
    The following description corresponds to the functionality of the buttons in the Toolbar panel (\Cref{fig:mechaedit}b).


  * **Open:** opens a new geometry file.

  * **Save:** saves the visualized geometry to the currently opened file with the same name.

  * **Save as:** saves the visualized geometry to a different file.

  * **Edit:** activates the edit mode, in which atoms that are selected can be dragged (while moving the cursor with the mouse left button clicked) or rotated (while moving the cursor with the mouse right button clicked.

  * **Lattice:** toggles the visualization of the unit cell on or off.

  * **Specular:** toggles on and off the specular effect on the drawn objects (spheres, cylinders, etc.).

  * **Track:** enables track mode for the currently selected atoms. After selecting two atoms sequentially, their interatomic distance is tracked by activating this option. When three atoms are selected sequentially, the distances (in selection order) and the angle are tracked. When four atoms are selected sequentially, the distances (in selection order), angles, and the torsion angle are tracked. After the track mode is enabled, the selection can be modified and any change made with the edit mode is continuously tracked, and their values (distances, angles, and torsion angles) are updated in the bottom bar of \mechaedit window.

  * **Crop:** activates the crop mode, in which a rectangular region of the drawing canvas can be defined before image rendering and export. It is useful for controlling the size and which part of the structure will be included in the saved image.

Drawing canvas
--------------

In the drawing canvas (\Cref{fig:mechaedit}c), the structure can be rotated by holding the left mouse button and moving the mouse. The view can be translated by holding the right mouse button and moving the mouse. Individual atoms can be selected using Ctrl + left click, while a range of atoms can be selected by holding Ctrl and dragging with the left mouse button. Selected atoms can be deleted using the Delete key. Selected atoms can be hidden with Ctrl + H shortcut.


Side panel
----------
The side panel (\Cref{fig:mechaedit}d) contains a set of slide bars and buttons designed to intuitively modify the properties of the canvas elements (atomic properties, visual properties) and canvas behavior). Besides, the included coordinates editor allows selecting and modifying the atomic coordinates individually. In addition, a frequency and frames listbox can be added to the side panel. The frequency listbox can be enabled/disabled from the tool menu. The frames listbox is automatically enabled if the opened file is a trajectory.

Menu Options
------------

File Menu
.........

The File menu contains options for loading, saving, and exporting chemical structures.

    **New:** creates a new, empty structure.

    **Open:** loads a structure file into the visualization canvas.

    **Save:** saves the current structure to the active file.

    **Save As:** saves the current structure to a new file location.

    **Render (POV-Ray):** exports the current structure for high-quality rendering using \href{https://www.povray.org/}{POV-Ray}, suitable for publication-quality images.

    **Close:** closes the program.

Edit Menu}
..........

The Edit menu provides basic editing operations for selected atoms.

  **Copy:** copies the currently selected atoms to the clipboard.

  **Paste:**  inserts previously copied atoms into the structure.

  **Center:** centers the structure in the drawing canvas.


Selection Menu
...............
The Selection menu provides tools to create and modify atom selections.

   **Select all:** selects all atoms in the current structure.

   **Invert selection:** reverses the current selection state.

   **Select by bonds:** expands the selection based on bonding connectivity.

   **Select inside sphere:** selects all atoms inside a given radius from a previously selected atom.

   **Select groups:** selects previously created groups (in tools menu).

View Menu
..........

The View menu controls the representation of the structure.

    **Restore defaults:** changes the sphere and cylinder radii, and colors to the predefined values.

    **Models:** allows changing to a licorice or wire representation.

    **Atom:** allow changing atomic properties and representation.

    **Background color:** changes the background color of the canvas.

    **Styles:** allows changing predefined styles from files.

    **Set zoom:** allows defining a zoom ratio with respect to the default zoom to control how far the structure is visualized.


Insert Menu
...........
The Insert menu allows the insertion of structures in the current representation.

   **Fragment:** inserts a structure from a file into the current visualization.

   **Download IZA:** downloads a zeolite CIF file from the IZA website https://europe.iza-structure.org/IZA-SC/ftc_table.php.


Tools Menu
...........
The Tools menu provides interactive analysis and measurement features.

   **Fix Selected:** sets a fixed flag internally to *true* so that the saved structure in VASP (POSCAR) format contains the selective dynamics flags.

   **Unfix Selected:** sets a fixed flag internally to *false* so that the saved structure in VASP (POSCAR) format contains the selective dynamics flags.

   **Translate:** allows translating the selected atoms by adding an input vector, or to specified x, y, or z positions.

   **Rotate:** rotates the whole structure around the z axis or bonds (by preselecting the two atoms of the bond).

   **Repeat:** replicates the unit cell along the x, y and/or z direction by the specified amount. There must be a unit cell defined in the structure.

   **Coordinates:** shows or hides the coordinates listbox in the side panel.

   **Create group:** with this option, atoms can be grouped for future selection and manipulations.


Crystallography
.................

   **Set Unit Cell:** opens a dialog to define or modify unit cell parameters.

Animation Menu
...............

The Animation menu allows playback of dynamic structural data.

   **Play:** plays an available animation or trajectory.

   **Play Frequencies:** animates vibrational modes, typically obtained from frequency calculations.

   **Time Delay:** adjusts the delay between animation frames to control playback speed.


Notes
......

Most menu actions operate on the currently selected atoms. Atom selection is performed directly in the drawing canvas using mouse and keyboard controls. Available visualization, animation, and analysis features depend on the data present in the loaded structure.


Mechakinetics module
=====================

The *Mechakinetics* module accepts input data in JSON format, which defines the chemical reactions, temperature-dependent rate data, simulation time parameters, and initial species concentrations. The input file can be generated in the *mechadata* plotting interface (right-click and *export reaction network*) and customized as needed.  As shown in \Cref{fig:json-input}, it has the following sections:


  * Reaction Data: The *"mec"* field contains the chemical reactions and their kinetic parameters. Each key corresponds to a reaction, that is written in the general format  *"aA+bB=cC+dD"*.  Within each reaction, the rates are provided as a dictionary of temperatures (in Kelvin) and a list containing two numbers, e.g: *"298": [2.17, 4.83e-15]*. The first number is the forward reaction rate and the second the reverse reaction rate (both in s^{-1}), calculated using Eyring's equation. Multiple temperatures can be included if the reaction rate needs to be evaluated across a temperature range.
  
  * Simulation Time: The *"time"* field defines the time span (in seconds) for the kinetics simulation as a string of three values: *"time": "t\_start t\_end t\_step"*.
  
  * Initial Values: The *"initial_values"* field defines the starting concentrations of reacting species, for example, *"initial_values": \{   "A": 1, "B": 1   \}*.

Here, A and B start with a concentration of 1 (in arbitrary units consistent across all species). All other species are assumed to start at zero unless explicitly specified.
\end{itemize}

The user is encouraged to see other examples of these input files in the \href{https://github.com/rm-compchem/mechasuite/tree/main/examples}{examples} folder of the source code, such as \textit{rn.json}, and \textit{reaction\_network\_0tboh.json}.


Example: General workflow in *MechaSuite*
==========================================

This example illustrates the general environment of Mechadata. The hypothetical reaction is a simple two-step mechanism occurring in a batch reactor, where the concentration of reactants changes over time until reaching an equilibrium. The reaction network is defined as follows:

R <=> I

I <=> P

where *R*, *I* and *P* are the hypothetical reactant, intermediate and product, respectively. The rate constants for both steps are set to be equal by assigning identical forward Gibbs free energies of activation, such as ΔG_act1_f  =  ΔG_act2_f = 17 kcal/mol. Using Eyring's equation, the corresponding forward rate constants at 298 K are k1f = k2f = 2.17 s^-1. To make the steps irreversible, the reverse constants are deliberately chosen to be small, k1r = k2r  10^-15 s^-1. This can be achieved by setting the ΔG for the reverse steps considerably higher than the forward ones, like ΔG_act1_r = ΔG_act2_r = 57 kcal/mol.

\Cref{fig:example1}a illustrates the main interface of mechadata, displaying the central spreadsheet that lists the hypothetical minima species (*R*, *I*, and *P*) together with the corresponding transition states (*TS1* and *TS2*) and their associated hypothetical energies. Their relative energies with respect to the global reference (denoted as *global_ref*), along with the corresponding activation and reaction energies, are displayed in the smaller spreadsheets on the right (relative energy and reaction energy panels in \Cref{fig:Mechadata}b,c). In this example, the *global_ref* serves merely as an arbitrary zero-energy reference for illustration.

 .. %Fig 4 - fig:example1

The interactive plotting interface displaying the corresponding energy profile derived from the reaction mechanism is shown in \Cref{fig:example1}b. The graphical elements representing minima and transition states, as well as their labels, can be customized in terms of style, color, line width, and position. Once the desired settings have been defined, a publication-ready version of the plot can be generated using Matplotlib (see the second example in the manuscript).

The concentration profiles in \Cref{fig:example1}c were obtained by numerically solving the system of differential equations using *mechakinetics*. The results illustrate that the concentration of *R* decreases over time, the intermediate *I* first increases and then declines after approximately 0.1 seconds, and the product *P* increases exponentially, behavior characteristic of a first-order reaction with respect to *R*.

Beyond their illustrative role, this and many other hypothetical examples provide a valuable framework for educational purposes. By systematically varying kinetic and thermodynamic parameters, users can explore how individual elementary steps influence the overall behavior of a reaction network. Such interactive exploration facilitates an intuitive understanding of reaction kinetics, sensitivity to model parameters, and the interplay between mechanism and observable rates, making these examples particularly well suited for teaching and training in microkinetic modeling.
