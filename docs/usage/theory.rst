Theoretical Background
#######################

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

