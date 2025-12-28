#!/usr/bin/python3
#we use numpy to diagonalize the inertia matrix
import numpy as np
import sys
import math
from mechasuite.globs import conv, masas, sigma

# some constants here as per the NIST (CODATA 2014)
NA = 6.022140857e23 #Avogadro's number
R = 8.3144598 #J mol-1 K-1
Kb = 1.38064852e-23 #J K-1
h = 6.62607004e-34 #J s 
pi = 3.1415927
Jtocal = 1/4.184
JtoeV = 6.241509126e18/NA
JtoHa = 2.293712317e17
c = 299792458
AtoBohr = 1.889725989
umatoKg = 1.660539040e-27 
metoKg = 9.10938356e-31
umatome = umatoKg/metoKg
atmtoPa = 101325

# dictionary definitions:

# global final output arrays

global Qtrans, Qrot, Qvib, Qvibg, Qelect, Qtot, no_neg_freqs
Qtrans, Qrot, Qvib, Qvibg, Qelect, Qtot = [], [], [], [], [], []

# global final output arrays

# for the mass of the elements as per Nist (CIAAW (2015))
# when several are proposed I take the average


# --- FIRST LET'S READ SOME INPUT FILES :) ----


# function to load a xyz file and save the total number of atoms (n_atoms),
# the mass of each element in an array (mis), the total mass of the system
# (MM) and the coordinates in a bidimensional numpy array (xyzcoor).
def loadxyz(xyzfile):
    global n_atoms, mis, MM, xyzcoor

    xyzcoor, mis = [], []
    MM = 0
 
    f = open(xyzfile)
    l = f.readlines()
    f.close()

    try:
        n_atoms = int(l[0]) 
        for i in range(2,n_atoms+2):
            l[i] = l[i].split()
            element = l[i][0]
            mis.append(masas[element])
            MM += masas[element]
            """
            VARIANTE UNO PARA GUARDAR LAS COORDENADAS
            xyz = [float(j) for j in l[i][1:4]]
            xyzcoor.append(xyz)
            """
            xyzcoor.append(np.array(l[i][1:4], dtype=float))
    except:
        print( "{:#^68}".format("   ERROR!   "))
        print (" Bad input format in " + str(xyzfile) + " file."  )
        print (" Please provide a valid .xyz file or")
        print (" add the approppriate mass to the code (line 27)")
        print ("{:#^68}".format("#"))
        sys.exit()
    
    xyzcoor = np.array(xyzcoor)
    xyzcoor = xyzcoor*AtoBohr # in au (Bohr)


# function that reads the OUTCAR from a freq calculation
# and extracts the energy in eV and the frequencies in Hz.
def loadoutcar(outcarf):
    global freqs, E, n_freq

    f = open(outcarf)
    lineas = f.readlines()
    f.close()
    freqs = []
    # condicion para leer energia
    er = True
    n_freq = 0

    for num, linea in enumerate(lineas):

        if "entropy=" in linea and er:
            E = float(linea.split()[6])
            er = False

        if "THz" in linea and "cm-1" in linea and not er:
            try:
                freq = (float(linea.split()[3]))*1000000000000
                if freq <= float(50*c*100):
                       print ("Low frequency changed to 50 cm-1: " + str(int(freq/(c*100))))
                       freqs.append(float(50*c*100))
                       n_freq = int(linea.split()[0])
                else:
                       if freq in freqs:
                           break
                       freqs.append(freq)
                       n_freq = int(linea.split()[0])

            except:
                print ("Negative frequency changed to 50 cm-1")
                freqs.append(float(50*c*100))
                pass
    print("energy :", E)

    # This is to introduce manually the frequencies, for instance to
    # compare with Gaussian
    # freqs_cm = [765.9450, 1315.3459, 1552.7314, 765.9450, 1315.3459, 1552.7314, 765.9450, 1315.3459, 1552.7314]
    # freqs_cm = [765.9450, 1315.3459]
    # freqs = []
    # for i in freqs_cm:
    #    freqs.append(i*c*100) 
    # print (" ")
    # print ("Frequencies were introduced MANUALLY to check the program")
    # print (" ")
    # print ("In Hz:   " + str(freqs))
    # print ("In cm-1: " + str(freqs_cm))


# function to load a "thermo_input" file containning details for the
# desired temperatures, presure/volume and point group (PG)

def loadthermoinput(thermo_input):
    global temps, Final_temps, coorfile, spin, spin_mult, unitE
    global V, P, PG, Other, pressurecalc, XYZ, rows, warnings, user_modes

    temps, Final_temps, PGarray, Other = [], [], [], []
    PG, coorfile, spin_mult, unitE  = "", "", "", ""
    # spin multiplicity (1/2 for doublet, 1 for triplet, etc)
    V, P, Tini, Tfin, step, user_modes = 0, 0, 0, 0, 0, 0
    temperature = False
    pointgroup = False
    pressure = False
    volume = False
    XYZ = False
    spin = False
    unit = False
    rows = False
    modes = False

    warnT = False
    warnPG = False
    warnP = False
    warnV = False
    warnPV = False
    pressurecalc = False
    warnXYZ = False
    warnspin = False
    warnunit = False
    warnmodes = False

    warnings = True

    f = open(thermo_input)
    l = f.readlines()
    f.close()

# we start finding line by line where is the input data we need
    for linea in l:
    # first we look for temperatures
    # lstrip eliminates blanks from the left
    # lower eliminates caps in the string
    # startswith determines wether the string starts with something

        if linea.lstrip().lower().startswith("t"):
            if temperature:
                if warnT:
                    pass
                else:
                    print ("\n" + "{:*^68}".format("   WARNING!   "))
                    print (" Found more than one line for temperatures.")
                    print (" Only the last one with be considered.")
                    print (" Please check the " + str(thermo_input) + " file.")
                    print ("{:*^68}".format("*") + "\n")
                    warnT = True
                    pass
            else:
                pass
            try:
                temps = linea.split("=")[1].split()
                temperature = True
            except:
                try:
                    temps = linea.split(":")[1].split()
                    temperature = True
                except:
                    m = linea.split()
                    temps = []
                    for i in range(1,len(m)):
                        temps.append(m[i])
                    temperature = True
            # temps are re-read and intervals detected later (line 375),
            # and the pertinent format errors are coded there.

    # now we look for the point group
        elif linea.lstrip().lower().startswith("pg"):
            if pointgroup:
                if warnPG:
                    pass
                else:
                    print("\n" + "{:*^68}".format("   WARNING!   "))
                    print(" Found more than one line for point group (PG).")
                    print(" Only the last one with be considered.")
                    print(" Please check the " + str(thermo_input) + " file.")
                    print("{:*^68}".format("*") + "\n")
                    warnPG = True
                    pass
            else:
                pass
            try:
                PG = linea.split("=")[1].split()[0]
            except:
                try:
                    PG = linea.split(":")[1].split()[0]
                except:
                    try:
                        PG = linea.split()[1]
                    except:
                        PG = "solid"
            pointgroup = True
        
    # now we look for pressure and / or volume

        elif linea.lstrip().lower().startswith("p"):
            if pressure:
                if warnP:
                    pass
                else:
                    print("\n" + "{:*^68}".format("   WARNING!   "))
                    print(" Found more than one line for pressure.")
                    print(" Only the last one with be considered.")
                    print(" Please check the " + str(thermo_input) + " file.")
                    print("{:*^68}".format("*") + "\n")
                    warnP = True
                    pass
            elif volume:
                if warnPV:
                    pass
                else:
                    print ("\n" + "{:*^68}".format("   WARNING!   "))
                    print (" Found input for both volume and pressure.")
                    print (" Only the last one with be considered.")
                    print (" Please check the " + str(thermo_input) + " file.")
                    print ("{:*^68}".format("*") + "\n")
                    warnPV = True
                    pass
            try:
                P = float(linea.split("=")[1].split()[0])
                pressure = True
                pressurecalc = True
            except:
                try:
                    P = float(linea.split(":")[1].split()[0])
                    pressure = True
                    pressurecalc = True
                except:
                    try:
                        m = linea.split()
                        P = float(m[1])
                        pressure = True
                        pressurecalc = True
                    except:
                        if pressure or volume:
                            pass
                        else:
                            print("\n" + "{:#^68}".format("   ERROR!   "))
                            print(" Bad input format for pressure. ")
                            print(" Please check the " + str(thermo_input) + " file.")
                            print("{:#^68}".format("#") + "\n")
                            sys.exit()

        elif linea.lstrip().lower().startswith("v"):
            if volume:
                if warnV:
                    pass
                else:
                    print ("\n" + "{:*^68}".format("   WARNING!   "))
                    print (" Found more than one line for volume.")
                    print (" Only the last one with be considered.")
                    print (" Please check the " + str(thermo_input) + " file.")
                    print ("{:*^68}".format("*") + "\n")
                    warnV = True
                    pass
            elif pressure:
                if warnPV:
                    pass
                else:
                    print ("\n" + "{:*^68}".format("   WARNING!   "))
                    print (" Found input for both volume and pressure.")
                    print (" Only the last one with be considered.")
                    print (" Please check the " + str(thermo_input) + " file.")
                    print ("{:*^68}".format("*") + "\n")
                    warnPV = True
                    pass
            try:
                V = float(linea.split("=")[1].split()[0])
                pressurecalc = False
                volume = True
            except:
                try:
                    V = float(linea.split(":")[1].split()[0])
                    pressurecalc = False
                    volume = True
                except:
                    try:
                        m = linea.split()
                        V = float(m[1])
                        pressurecalc = False
                        volume = True
                    except:
                        if pressure or volume:
                            pass
                        else:
                            print ("\n" + "{:#^68}".format("   ERROR!   "))
                            print (" Bad input format for volume. ")
                            print (" Please check the " + str(thermo_input) + " file.")
                            print ("{:#^68}".format("#") + "\n")
                            sys.exit()
    
    # now we look for different names for the input files for the xyz file
        elif linea.lstrip().lower().startswith("xyz"):
            if XYZ:
                if warnXYZ:
                    pass
                else:
                    print ("\n" + "{:*^68}".format("   WARNING!   "))
                    print (" Found more than one line for the input .xyz file.")
                    print (" Only the last one with be considered.")
                    print (" Please check the " + str(thermo_input) + " file.")
                    print ("{:*^68}".format("*") + "\n") 
                    warnXYZ = True
                    pass
            else:
                pass
            try:
                coorfile = linea.split("=")[1].split()[0]
                XYZ = True
            except:
                try:
                    coorfile = linea.split(":")[1].split()[0]
                    XYZ = True
                except:
                    try:
                        m = linea.split()
                        XYZ = m[1]
                        XYZ = True
                    except:
                        pass

    # and we check if electronic entropy is to be calculated based on
    # the spin multiplicity of the groundstate
        elif linea.lstrip().lower().startswith("s"):
            if spin:
                if warnspin:
                    pass
                else:
                    print("\n" + "{:*^68}".format("   WARNING!   "))
                    print(" Found more than one line for the input spin.")
                    print(" Only the last one with be considered.")
                    print(" Please check the " + str(thermo_input) + " file.")
                    print("{:*^68}".format("*") + "\n")
                    warnspin = True
                    pass
            else:
                pass
            try:
                spin_mult = linea.split("=")[1].split()[0]
                spin = True
            except:
                try:
                    spin_mult = linea.split(":")[1].split()[0]
                    spin = True
                except:
                    try:
                        m = linea.split()
                        spin_mult = m[1]
                        spin = True
                    except:
                        pass

    # now we define Other specifications for the output
    # firstly which are the units to be used
        elif linea.lstrip().lower().startswith("u"):
            if unit:
                if warnunit:
                    pass
                else:
                    print("\n" + "{:*^68}".format("   WARNING!   "))
                    print(" Found more than one line for the desired units in output.")
                    print(" Only the last one with be considered.")
                    print(" Please check the " + str(thermo_input) + " file.")
                    print("{:*^68}".format("*") + "\n")
                    warnunit = True
                    pass
            else:
                pass
            try:
                unitE = linea.split("=")[1].split()[0]
                unit = True
            except:
                try:
                    unitE = linea.split(":")[1].split()[0]
                    unit = True
                except:
                    try:
                        m = linea.split()
                        unitE = m[1]
                        unit = True
                    except:
                        pass

    # finally we select whether we prefer Rows instead of columns, which is the default
        elif linea.lstrip().lower().startswith("row"):
            try:
                rows = True
            except:
                pass

    # we may want consistent calculations with a certain number of vib modes
    # then we should include it in the thermo_input file
        elif linea.lstrip().lower().startswith("m"):
            if modes:
                if warnmodes:
                    pass
                else:
                    print("\n" + "{:*^68}".format("   WARNING!   "))
                    print(" Found more than one line for the number of modes in output.")
                    print(" Only the last one with be considered.")
                    print(" Please check the " + str(thermo_input) + " file.")
                    print("{:*^68}".format("*") + "\n")
                    warnmodes = True
                    pass
            else:
                pass
            try:
                user_modes = linea.split("=")[1].split()[0]
                modes = True
            except:
                try:
                    user_modes = linea.split(":")[1].split()[0]
                    modes = True
                except:
                    try:
                        m = linea.split()
                        user_modes = m[1]
                        modes = True
                    except:
                        pass

    # we add a non-verbose option to disable warnings
    #    elif linea.lstrip().lower().startswith("nowarn"):
     #       try:
      #          warnings = False
       #     except:
        #        pass

# Now we read the temperature intervals and save all temperatures in an array
        
    for temp in temps:
        try:
            T = float(temp)
            Final_temps.append(T)
        except:
            try:
                scan = temp.replace("-"," ")
                scan = scan.replace(","," ")
                Tini = float(scan.split()[0])
                Tfin = float(scan.split()[1])
                step = float(scan.split()[2])
                Final_temps.append(Tini)
                while (Tfin > Tini):
                    Tini = Tini + step
                    #este if es opcional para que no se pase de la Tfin que ponemos
                    if (Tini > Tfin):
                        Final_temps.append(Tfin)
                    else:
                        Final_temps.append(Tini)
            except:
                print ("\n" + "{:#^68}".format("   ERROR!   ")) 
                print (" Bad input format in temperatures.") 
                print (" Please check the " + str(thermo_input) + " file.") 
                print ("{:#^68}".format("#") + "\n") 
                sys.exit()

# Here we decide whether the pointgroup read is acceptable.
# Notice that a blank point group will default to "solid".

    if pointgroup:
        if PG == "=" or PG == ":" or PG == "":
            PG = "solid"
    else:
        print("\n" + "{:*^68}".format("   WARNING!   "))
        print(" No input found for point group (PG) in " + str(thermo_input) + " file.")
        print(' "Solid" selected by default.')
        print("{:*^68}".format("*") + "\n")
        PG = "solid"

    if PG.lower() == "solid":
        pass
    else:
        try:
            s = float(sigma[PG])
        except:
            print("\n" + "{:#^68}".format("   ERROR!   "))
            print(" Point group not found in sigma database.")
            print(" Please check the " + str(thermo_input) + " file")
            print(" or add the sigma value to the code (line 41).")
            print("{:#^68}".format("#") + "\n")
            sys.exit()

# Here we decide whether the spin read is acceptable.
# Notice that a blank spin will skip the electronic entropy calculation.

    if spin:
        try:
            spin_mult = float(spin_mult)
        except:
            print("\n" + "{:*^68}".format("   WARNING!   "))
            print(" Bad input format for the spin multiplicity. ")
            print(" Electronic entropy will NOT be calculated.")
            print(" Please check the " + str(thermo_input) + " file")
            print("{:*^68}".format("*") + "\n")
            spin = False
    else:
        print("\n" + "{:*^68}".format("   WARNING!   "))
        print(" No spin multiplicity found in the " + str(thermo_input) + " file")
        print(" Electronic entropy will NOT be calculated (equivalent to a singlet).")
        print("{:*^68}".format("*") + "\n")

# Finally here we determine if we have the minimum data we need to
# continue with the calculations and otherwise provide default values.

    if Final_temps == []:
        print ("\n" + "{:*^68}".format("   WARNING!   ")) 
        print (" No input temperatures found in " + str(thermo_input) + " file.") 
        print (" T = 298.15 K selected by default.") 
        print ("{:*^68}".format("*") + "\n") 
        #sys.exit()
        Final_temps = [298.15]

    if pressure:
        pass
    else:
        if volume:
            pass
        else:
            print ("\n" + "{:*^68}".format("   WARNING!   ")) 
            print (" No input found for pressure nor volume in " + str(thermo_input) + " file.") 
            print (" P = 1 atm selected by default.") 
            print ("{:*^68}".format("*") + "\n") 
            #sys.exit()
            P = 1
            pressurecalc = True



# ---- NOW WE CALCULATE STUFF! :D ----


def shifttomc():
    # function that shifts the coordinates so that they refer to
    # the center of mass (cm)

    global xyzcoor, MM, masas
    global shiftedxyz, mis

    Xcm, Ycm, Zcm = 0, 0, 0
    shiftedxyz = []

    for xyz, mi in zip(xyzcoor, mis):
        x, y, z = xyz
        Xcm += mi*x
        Ycm += mi*y
        Zcm += mi*z

    Xcm /= MM
    Ycm /= MM
    Zcm /= MM

    for x, y, z in xyzcoor:
        shiftedxyz.append([x - Xcm, y - Ycm, z - Zcm])

    shiftedxyz = np.array(shiftedxyz)
    print(MM)
    print(shiftedxyz)


def inertia():
    global shiftedxyz, masas, autoval, IA, IB, IC, I
    Ixx, Iyy, Izz, Ixy, Ixz, Iyz = 0, 0, 0, 0, 0, 0
    IA, IB, IC, I = 0, 0, 0, 0

    if PG == "Coov" or PG == "Dooh":
        for xyz, mi in zip(shiftedxyz, mis):
            x, y, z = xyz
            I += mi*((math.sqrt(x**2+y**2+z**2))**2)

    else:
        for xyz, mi in zip(shiftedxyz, mis):
            x, y, z = xyz
            Ixx += mi*(y**2+z**2)
            Iyy += mi*(x**2+z**2)
            Izz += mi*(x**2+y**2)
            Ixy += -mi*x*y
            Iyz += -mi*y*z
            Ixz += -mi*x*z

        inertiamat = np.matrix([[Ixx, Ixy, Ixz],
                                [Ixy, Iyy, Iyz],
                                [Ixz, Iyz, Izz]])

        autoval, autovec = np.linalg.eig(inertiamat)

        IA, IB, IC = autoval # in au
    print("IA", IA)
    print("IB", IB)
    print("IC", IC)
    print("I", I)


def trans():
    # function to calculate the internal translational energy (Utrans) and
    # the translational entropy (Strans):

    global Utrans, Strans, Qtrans
    Qtrans = []
    mult = 0

    Utrans, Strans, Outprint = [], [], []

    print(" Calculating TRANSLATIONAL contributions")

    if pressurecalc:
        print("   Selected pressure (atm): " + str(P))
    else:
        print("   Selected volume (L): " + str(V))
    print(" ")
    for T in Final_temps:
        Utrans.append(1.5*R*T)
        if pressurecalc:
            mult = float((Kb*T)/(P*atmtoPa))
        else:
            mult = float((V/NA)/1000)
        Strans.append(R*(math.log((((2*pi*((MM/1000)/NA)*Kb*T)/(h**2))**1.5)*mult)+2.5))
        Qtrans.append((((2*pi*((MM/1000)/NA)*Kb*T)/(h**2))**1.5)*mult)


def rot():
    # function to calculate the internal rotational energy (Urot)
    # and the rotational entropy (Srot).

    global Urot, Srot, Qrot
    Qrot = []
    Urot, Srot, Outprint = [], [], []
    s = 0

    s = float(sigma[PG])
    #print(R, I, umatome, Kb, JtoHa, s)
    if PG == "Coov" or PG == "Dooh":
        print(" Calculating ROTATIONAL contributions for a LINEAR molecule")
        for T in Final_temps:
            Urot.append(R*T)
            # in au, for which h=2*pi
            Srot.append(R*(math.log(I*umatome*(2*Kb*JtoHa*T)/s)+1))
            Qrot.append(I*umatome*(2*Kb*JtoHa*T)/s)

    else:
        print(" Calculating ROTATIONAL contributions for a NON-LINEAR molecule")
        for T in Final_temps:
            Urot.append(1.5*R*T)
            # in au, for which h=2*pi
            Srot.append(R*(math.log(math.sqrt(IA*IB*IC*(umatome**3)*pi*((2*Kb*JtoHa*T)**3))/s)+1.5))
            Qrot.append(math.sqrt(IA*IB*IC*(umatome**3)*pi*((2*Kb*JtoHa*T)**3))/s)

    print("   Selected point group: " + str(PG))
    print(" ")


def vib():
    global ZPVE, ZPVET, UvibT, SvibT, n_modes, Final_freqs, Qvib, Qvibg, outcarfile, thermoinput
    Qvib, Qvibg = [], []
    ZPVET, UvibT, SvibT, Final_freqs = [], [], [], []
    n_modes = 0

    print(" ")
    if user_modes == 0:
        if PG == "Coov" or PG == "Dooh":
            n_modes = 3*n_atoms - 5
            print(" Calculating VIBRATIONAL contributions for a LINEAR molecule")
        elif PG.lower() == "solid":
            print(" Calculating VIBRATIONAL contributions for a SOLID (PERIODIC) system")
            n_modes = n_freq
            if n_modes != 3*n_atoms:
                print("\n" + "{:*^68}".format("   WARNING!   "))
                # print(" Found less than 3N vibrational modes: Check " + str(outcarfile) + " file")
                # print(" or select appropriate point group (PG) in " + str(thermoinput) + " file." + "\n")
                print(" (Negative frequencies are not included in the calculation)")
                print("{:*^68}".format("*") + "\n")
        else:
            n_modes = 3*n_atoms - 6
            print(" Calculating VIBRATIONAL contributions for a NON-LINEAR molecule")
    else:
        print(" Calculating VIBRATIONAL contribution for selected number of modes:")
        n_modes = int(user_modes)
        print("#modes: " + str(n_modes))

    # print "   Number of vibrational modes: " + str(n_modes)
    # print "   Frequencies(cm-1): "
    try:
        for f in range(0, n_modes):
            Final_freqs.append(freqs[f]/(c*100))
    except:
        print("\n" + "{:#^68}".format("   ERROR!   "))
        print(" Unexpected number of frequencies: Check the " + str(outcarfile) + " file,")
        print(" select appropriate point group (PG) in the " + str(thermoinput) + " file")
        print(" or manually indicate the number of MODES in the" + str(thermoinput) + " file.")
        print(" #modes: " + str(n_modes))
        print(" #freqs: " + str(n_freq))
        print("{:#^68}".format("#") + "\n")
        # sys.exit()

    # print "{:^20.4f}".format(Final_freqs[f])
    for T in Final_temps:
        ZPVE = 0
        Uvib = 0
        Svib = 0
        qvib = 1
        qvibg = 1
        for f in range(0, n_modes):
            if no_neg_freqs and freqs[f] < 0:
                print("Ignoring imaginary frequency:", freqs[f])
                continue
            ZPVE += 0.5*h*freqs[f]*NA
            Uvib += R*((h*freqs[f])/(Kb*(math.e**((h*freqs[f])/(Kb*T))-1)))
            Svib += R*((h*freqs[f])/(Kb*T*((math.e**((h*freqs[f])/(Kb*T)))-1))-math.log(1-math.e**((-h*freqs[f])/(Kb*T))))
            qvib *= 1/(1 - math.e**-((h*freqs[f])/(Kb*T)))
            qvibg *= (math.e**-((h*freqs[f])/(Kb*T)))/(1 - math.e**-((h*freqs[f])/(Kb*T)))
        ZPVET.append(ZPVE)
        UvibT.append(Uvib)
        SvibT.append(Svib)
        Qvib.append(qvib)
        Qvibg.append(qvibg)
    print(" ")

# function to calculate electronic entropy (note that Uelec = 0)
# we use the common approximation in which it depends only on spin
# and does not change with temperature


def vib2():
    # function to calculate the ZPVE, Uvib and Svib:

    global ZPVE, ZPVET, UvibT, SvibT, Qvib, Qvibg
    Qvib, Qvibg = [], []
    ZPVET, UvibT, SvibT = [], [], []

    print (" ") 
    if user_modes == 0:
        if PG == "Coov" or PG == "Dooh":
            print(" Calculating VIBRATIONAL contributions for a LINEAR molecule")
        elif PG.lower() == "solid":
            print(" Calculating VIBRATIONAL contributions for a SOLID (PERIODIC) system")
        else:
            print(" Calculating VIBRATIONAL contributions for a NON-LINEAR molecule")
    else:
        print(" Calculating VIBRATIONAL contribution for selected number of modes")

    for T in Final_temps:
        ZPVE = 0
        Uvib = 0
        Svib = 0
        qvib = 1
        qvibg = 1
        for freq in freqs:
            ZPVE += 0.5*h*freq*NA
            Uvib += R*((h*freq)/(Kb*(math.e**((h*freq)/(Kb*T))-1)))
            Svib += R*((h*freq)/(Kb*T*((math.e**((h*freq)/(Kb*T)))-1))-math.log(1-math.e**((-h*freq)/(Kb*T))))
            qvib *= 1/(1 - math.e**-((h*freq)/(Kb*T)))
            qvibg *= (math.e**-((h*freq)/(Kb*T)))/(1 - math.e**-((h*freq)/(Kb*T)))
        ZPVET.append(ZPVE)
        UvibT.append(Uvib)
        SvibT.append(Svib)
        Qvib.append(qvib)
        Qvibg.append(qvibg)
    print(" ")


def elec():
    # function to calculate electronic entropy (note that Uelec = 0)
    # we use the common approximation in which it depends only on spin
    # and does not change with temperature

    global Qelect
    global SelecT, spin
    SelecT = []
    Qelect = []

    if spin:
        s_number = (spin_mult)/2
        Selec = R*math.log(2*spin_mult + 1)
    else:
        Selec = 0

    for T in Final_temps:
        SelecT.append(Selec)
        qspin = 2*s_number + 1
        Qelect.append(qspin)


# function to calculate the total contributions of U and S
# and the corresponding H and G
def total():
    global Utot, Stot, HT, GT, Utrans, Strans, Urot, Srot, UvibT, SvibT, E, ZPVE, SelecT
    global Qtrans, Qrot, Qvib, Qvibg, Qelect, Qtot
    Utot, Stot, HT, GT = [], [], [], []
    Qtot = []
    
    if PG == "solid" or PG == "SOLID" or PG == "Solid":
        # I could print U or SvibT directly too but I think it is clearer this way
        Utrans, Urot, Strans, Srot = [], [], [], []
        for i in UvibT:
            Utrans.append(0.0)
            Urot.append(0.0)
            Strans.append(0.0)
            Srot.append(0.0)
            Qtrans.append(1.0)
            Qrot.append(1.0)
    else:
        if PG == "atom" or PG =="ATOM" or PG == "Atom":
            # I could print U or Strans directly too but I think it is clearer this way
            ZPVE = 0
            Urot, UvibT, Srot, SvibT = [], [], [], []
            for i in Utrans:
                UvibT.append(0.0)
                Urot.append(0.0)
                SvibT.append(0.0)
                Srot.append(0.0)
                Qrot.append(1.0)
                Qvib.append(1.0)
                Qvibg.append(1.0)

    for A, B, C in zip(Utrans, Urot, UvibT):
        Utot.append(E*conv[unitE]["J"] + ZPVE + A + B + C)
        print("ZPVE", ZPVE)
    for A, B, C, D in zip(Strans, Srot, SvibT, SelecT):
        Stot.append(A + B + C + D)
    for T, U in zip(Final_temps, Utot):
        HT.append(U+R*T)
    for H, T, S in zip(HT, Final_temps, Stot):
        GT.append(H-T*S)
    for qvib, qrot, qtrans, qelect in zip(Qvibg, Qrot, Qtrans, Qelect):
        Qtot.append(qvib*qrot*qtrans*qelect)


# function to create the output file of the program
def output(thermooutput):
    global E, ZPVE, unitE
    Final_temps_row2 = ""

    f = open(thermooutput, "w")
    
    print >> f, "{:^68}".format("Thermo script output (2.0 version)")
    print >> f, "{:-^68}".format("-")
    print >> f, "{:^68}".format("Input summary")
    print >> f, "{:-^68}".format("-")
    if pressurecalc:
        print >> f, " Selected pressure (atm): " + str(P)
    else:
        print >> f, " Selected volume (L): " + str(V)
    print >> f, " Selected point group: " + str(PG)
    if PG == "Coov" or PG == "Dooh":
        print >> f, "   LINEAR molecule"
    else:
        if PG == "solid" or PG == "SOLID" or PG == "Solid":
            print >> f, "   SOLID (PERIODIC) system"
        else:
            if PG == "atom" or PG == "ATOM" or PG == "Atom":
                print >> f, "   Single ATOM"
            else:
                print >> f, "   NON-LINEAR molecule"
    if PG == "atom" or PG == "ATOM" or PG == "Atom":
        pass
    else:
        print >> f, " Number of vibrational modes: " + str(n_modes)
        print >> f, " Frequencies (cm-1): "
        for i in Final_freqs:
            print >> f, "{:^18.4f}".format(i)
    
    print >> f, " "
    print >> f, "{:-^68}".format("-")
    print >> f, "{:^68}".format("Thermochemistry")
    print >> f, "{:-^68}".format("-")
    if unitE.lstrip().lower() == "j" or unitE.lstrip().lower() == "kj":
        unitE = "kJ"
        form = "10.4f"
        E = E*(1/JtoeV)/1000
        ZPVE = ZPVE/1000
    else:
        if unitE.lstrip().lower() == "ev":
            unitE = "eV"
            form = "10.6f"
            #E = E
            ZPVE = ZPVE*JtoeV
        else:
            unitE = "kcal"
            form = "10.5f"
            E = E*(1/JtoeV)*Jtocal/1000
            ZPVE = ZPVE*Jtocal/1000
    print >> f, " (Temperatures in K, energies in " + unitE +"/mol and entropies in " + unitE + "/mol K)"
    print >> f, " "
    print >> f, " Electronic energy: " + "{:<{}}".format(E, form)
    print >> f, " ZPVE: " + "{:<{}}".format(ZPVE, form)
    print >> f, " (ZPVE included in Uvib below)"
    print >> f, " "
    print >> f, " Translational, rotational and vibrational contributions to U and S:"
    print >> f, " "
    if rows:
        Final_temps_row = str("  {:<6}".format("T"))
        Utrans_row = str("  {:<6}".format("Utrans"))
        Urot_row = str("  {:<6}".format("Urot"))
        Uvib_row = str("  {:<6}".format("Uvib"))
        Strans_row = str("  {:<6}".format("Strans"))
        Srot_row = str("  {:<6}".format("Srot"))
        Svib_row = str("  {:<6}".format("Svib"))
        Selec_row = str("  {:<6}".format("Selec"))
        Final_temps_row2 = str("  {:<6}".format("T")) 
        Utot_row = str("  {:<6}".format("Utot"))
        Stot_row = str("  {:<6}".format("Stot"))
        H_row = str("  {:<6}".format("H"))
        G_row = str("  {:<6}".format("G"))

    else:
        print >> f, "{:^11}{:^10}{:^10}{:^10}{:^10}{:^9}{:^11}{:^10}".format("T", "Utrans", "Urot", "Uvib", "Strans", "Srot", "Svib", "Selec")

    for A, B, C, D, F, G, L, M, N, O, Q, Z in zip(Final_temps, Utrans, Urot, UvibT, Strans, Srot, SvibT, Utot, Stot, HT, GT, SelecT):
        if unitE.lstrip().lower() == "j" or unitE.lstrip().lower() == "kj":
            B = B/1000
            C = C/1000
            D = ZPVE+D/1000
            F = F/1000
            G = G/1000
            L = L/1000
            Z = Z/1000
            form = "10.4f"
            M = M/1000
            N = N/1000
            O = O/1000
            Q = Q/1000
            form2 = "13.4f"
            if rows:
                pass
            else:
                print >> f, "{:^11.2f}{:^10.4f}{:^10.4f}{:^10.4f}{:^10.4f}{:^10.4f}{:^10.4f}{:^10.4f}".format(A, B, C, D, F, G, L, Z)
        else:
            if unitE.lstrip().lower() == "ev":
                B = B*JtoeV
                C = C*JtoeV
                D = ZPVE+D*JtoeV
                F = F*JtoeV
                G = G*JtoeV
                L = L*JtoeV
                Z = Z*JtoeV
                form = "10.6f"
                M = M*JtoeV
                N = N*JtoeV
                O = O*JtoeV
                Q = Q*JtoeV
                form2 = "13.6f"
                if rows:
                    pass
                else:
                    print >> f, "{:^11.2f}{:^10.6f}{:^10.6f}{:^10.6f}{:^10.6f}{:^10.6f}{:^10.6f}{:^10.6f}".format(A, B, C, D, F, G, L, Z)
            else:
                B = B*Jtocal/1000
                C = C*Jtocal/1000
                D = ZPVE+D*Jtocal/1000
                F = F*Jtocal/1000
                G = G*Jtocal/1000
                L = L*Jtocal/1000
                Z = Z*Jtocal/1000
                form = "10.5f"
                M = M*Jtocal/1000
                N = N*Jtocal/1000
                O = O*Jtocal/1000
                Q = Q*Jtocal/1000
                form2 = "13.5f"
                if rows:
                    pass
                else:
                    print >> f, "{:^11.2f}{:^10.5f}{:^10.5f}{:^10.5f}{:^10.5f}{:^10.5f}{:^10.5f}{:^10.5f}".format(A, B, C, D, F, G, L, Z)

        if rows:
            Final_temps_row = Final_temps_row + str("{:>10.2f}".format(A))
            Utrans_row = Utrans_row + str("{:>{}}".format(B, form))
            Urot_row = Urot_row + str("{:>{}}".format(C, form))
            Uvib_row = Uvib_row + str("{:>{}}".format(D, form))
            Strans_row = Strans_row + str("{:>{}}".format(F, form))
            Srot_row = Srot_row + str("{:>{}}".format(G, form))
            Svib_row = Svib_row + str("{:>{}}".format(L, form))
            Selec_row = Selec_row + str("{:>{}}".format(Z, form))
            Final_temps_row2 = Final_temps_row2 + str("{:>13.2f}".format(A))
            Utot_row = Utot_row + str("{:>{}}".format(M, form2))
            Stot_row = Stot_row + str("{:>{}}".format(N, form2))
            H_row = H_row + str("{:>{}}".format(O, form2))
            G_row = G_row + str("{:>{}}".format(Q, form2))
        
    if rows:
        print >> f, Final_temps_row + "\n" + Utrans_row + "\n" + Urot_row + "\n" + Strans_row + "\n" + Srot_row + "\n" + Svib_row + "\n" + Selec_row
    
    print >> f, " "
    print >> f, "{:^68}".format("----------------------------------------------------------")
    print >> f, "{:^68}".format("|   Total values of U and S and corresponding H and G:   |")
    print >> f, "{:^68}".format("----------------------------------------------------------")
    print >> f, "{:^68}".format("(Enthalpies calculated as Utot + RT)")
    print >> f, " "
    
    if rows:
        print >> f, Final_temps_row2 + "\n" + Utot_row + "\n" + Stot_row + "\n" + H_row + "\n" + G_row
    else:
        print >> f, "{:^12}{:^14}{:^12}{:^14}{:^14}".format("T", "Utot", "Stot", "H", "G")
        for A, B, C, D, F in zip(Final_temps, Utot, Stot, HT, GT):
            if unitE.lstrip().lower() == "j" or unitE.lstrip().lower() == "kj":
                print >> f, "{:^12.2f}{:^14.4f}{:^12.4f}{:^14.4f}{:^14.4f}".format(A, B/1000, C/1000, D/1000, F/1000)
            else:
                if unitE.lstrip().lower() == "ev":
                    print >> f, "{:^12.2f}{:^14.6f}{:^12.6f}{:^14.6f}{:^14.6f}".format(A, B*JtoeV, C*JtoeV, D*JtoeV, F*JtoeV)
                else:
                    print >> f, "{:^12.2f}{:^14.5f}{:^12.5f}{:^14.5f}{:^14.5f}".format(A, B*Jtocal/1000, C*Jtocal/1000, D*Jtocal/1000, F*Jtocal/1000)
    print >> f, " "

    f.close()

#main program and actual calling of functions


def main():
    # header
    print ("{:-^68}".format("-")) 
    print ("{:^68}".format("Thermochemical script (2.0 version)")) 
    print ("{:-^68}".format("-")) 
    
    # the "thermo_input" file in current directory is mandatory and
    # the first one to be read.
    thermoinput = "thermo_input"
    
    # loading thermo_input to determine which files to load and read point
    # group to calculate the approppriate contributions.
    loadthermoinput(thermoinput)

    outcarfile = 'OUTCAR.freq'
    
    for i in Other:
        if "Coor=" in i:
            coorfile = str(i[5:].split())
            coorfile = coorfile.replace("[", "")
            coorfile = coorfile.replace("]", "")
            coorfile = coorfile.replace("'", "")
        else:
            coorfile = "poscar.xyz"
        if "Freq=" in i:
            outcarfile = str(i[5:].split())
            outcarfile = outcarfile.replace("[", "")
            outcarfile = outcarfile.replace("]", "")
            outcarfile = outcarfile.replace("'", "")
            print(outcarfile)
        else:
            outcarfile = 'OUTCAR.freq'
    
    if XYZ:
        pass
    else:
        coorfile = "poscar.xyz"
    
    loadxyz(coorfile)

    if PG == "solid" or PG == "SOLID" or PG == "Solid":
        loadoutcar(outcarfile)
        vib() 
    else:
        if PG == "atom" or PG == "ATOM" or PG == "Atom":
            trans()
        else:
            shifttomc()
            inertia()
            trans()
            rot()
            loadoutcar(outcarfile)
            vib()
    
    elec()
    
    # summing contributions and elaborating output
    total()
    
    try:
        thermooutput = sys.argv[1]
    except:
        thermooutput = "thermo.out"
    
    output(thermooutput)
    
    # ending message
    print("{:-^68}".format("-"))
    print(" FINISHED! Output saved as " + str(thermooutput))
    print("{:-^68}".format("-"))


def set_freq(frequencies, Energy, nnfreq=False):
    global freqs, E, user_modes, n_freq, no_neg_freqs
    E = Energy
    no_neg_freqs = nnfreq

    if PG.lower() == "solid":
        freqs = [50.0*29979245800 if f < 50
                 else float(f)*29979245800
                 for f in frequencies]
    else:
        if not no_neg_freqs:
            print ("no_neg_freq is false will multipliy by 50")
            freqs = [50.0 * 29979245800 if f < 50
                     else float(f) * 29979245800
                     for f in frequencies]
        else:
            print ("no_neg_freq is True will not multipliy by 50")
            freqs = [float(f) * 29979245800
                     for f in frequencies]

    n_freq = len(freqs)


def set_input(ts, ipg="solid", ip=1.0, iv=None, ispin_mult = None,
              ispin=False, unit="kcal/mol", iprecalc=True, imodes=0):

    global Final_temps, spin_mult, unitE, spin
    global V, P, PG, Other, pressurecalc, user_modes
    global outcarfile, xyzfile, thermo_input, thermoinput

    outcarfile = None
    xyzfile = None
    thermo_input = None
    thermoinput = None

    Final_temps = ts
    PG = ipg
    P = ip
    V = iv
    spin = ispin
    spin_mult = ispin_mult
    unitE = unit
    pressurecalc = iprecalc
    user_modes = imodes

    if not pressurecalc:
        if iv is None:
            pressurecalc = True

    if spin and spin_mult is None:
        spin = False


def loadstringxyz(xyzstring):
    global n_atoms, mis, MM, xyzcoor

    xyzcoor, mis = [], []
    MM = 0

    l = xyzstring.splitlines()

    try:
        n_atoms = int(l[0])
        for i in range(2, n_atoms + 2):
            l[i] = l[i].split()
            element = l[i][0]
            mis.append(masas[element])
            MM += masas[element]
            """
            VARIANTE UNO PARA GUARDAR LAS COORDENADAS
            xyz = [float(j) for j in l[i][1:4]]
            xyzcoor.append(xyz)
            """
            xyzcoor.append(np.array(l[i][1:4], dtype=float))
    except:
        print ("{:#^68}".format("   ERROR!   ")) 
        print (" Bad input format in " + str(xyzfile) + " file.") 
        print (" Please provide a valid .xyz file or") 
        print (" add the approppriate mass to the code (line 27)") 
        print ("{:#^68}".format("#")) 
        sys.exit()

    xyzcoor = np.array(xyzcoor)
    xyzcoor = xyzcoor * AtoBohr  # in au (Bohr)


def xyz_setup(labels, xyzcoors):
    global n_atoms, mis, MM, xyzcoor

    xyzcoor, mis = [], []
    MM = 0

    n_atoms = len(labels)
    for label, xyz in zip(labels, xyzcoors):
        mis.append(masas[label])
        MM += masas[label]
        xyzcoor.append(np.array(xyz, dtype=float))

    xyzcoor = np.array(xyzcoor)
    xyzcoor = xyzcoor * AtoBohr  # in au (Bohr)


def calc_all():
    global Utot, Stot, HT, GT, Utrans, Strans, Urot, Srot, UvibT, SvibT, E, ZPVET, SelecT
    global props
    props = {}

    if PG == "solid" or PG == "SOLID" or PG == "Solid":
        vib()
    elif PG == "atom" or PG == "ATOM" or PG == "Atom":
        trans()
    else:
        shifttomc()
        inertia()
        trans()
        rot()
        vib()

    elec()

    # summing contributions and elaborating output
    total()

    zpe = E + ZPVET[0]*conv["J"][unitE]
    props["g"] = {}
    props["h"] = {}

    props["stot"] = {}
    props["srot"] = {}
    props["svib"] = {}
    props["strans"] = {}

    props["etot"] = {}
    props["erot"] = {}
    props["evib"] = {}
    props["etrans"] = {}
    props["select"] = {}

    props["qtot"] = {}
    props["qrot"] = {}
    props["qtrans"] = {}
    props["qvib"] = {}
    props["qvibg"] = {}
    props["qelect"] = {}

    outUnit = "kJ"

    print("ZPE(kJ)  ", ZPVET[0]*conv["J"][outUnit])
    for n, temp in enumerate(Final_temps):
        # temp = str(temp)
        props["g"][temp] = GT[n] * conv["J"][unitE]
        print("G ({})       ".format(outUnit), props["g"][temp] * conv[unitE][outUnit])

        props["h"][temp] = HT[n] * conv["J"][unitE]
        print("H ({})       ".format(outUnit), props["h"][temp] * conv[unitE][outUnit])

        props["stot"][temp] = Stot[n] * conv["J"][unitE]
        print("Stot({})      ".format(outUnit), props["stot"][temp] * conv[unitE][outUnit])
        
        props["svib"][temp] = SvibT[n] * conv["J"][unitE]
        print("Svib({})      ".format(outUnit), props["svib"][temp] * conv[unitE][outUnit])
        
        # if Strans[n] != 0.0:
        props["strans"][temp] = Strans[n] * conv["J"][unitE]
        print("Strans({})    ".format(outUnit), props["strans"][temp] * conv[unitE][outUnit])
        
        # if Srot[n] != 0.0:
        props["srot"][temp] = Srot[n] * conv["J"][unitE]
        print("Srot({})      ".format(outUnit), props["srot"][temp] * conv[unitE][outUnit])
        
        props["select"][temp] = SelecT[n] * conv["J"][unitE]
        print("Select({})    ", props["select"][temp] * conv[unitE][outUnit])

        props["etot"][temp] = Utot[n] * conv["J"][unitE]
        print("Etot({})      ".format(outUnit), props["etot"][temp] * conv[unitE][outUnit])
        
        props["evib"][temp] = UvibT[n] * conv["J"][unitE]
        print("Evib({})      ".format(outUnit), props["evib"][temp] * conv[unitE][outUnit])
        
        # if Utrans[n] != 0.0:
        props["etrans"][temp] = Utrans[n] * conv["J"][unitE]
        print("Etrans({})    ".format(outUnit), props["etrans"][temp] * conv[unitE][outUnit])
        
        # if Urot[n] != 0.0:
        props["erot"][temp] = Urot[n] * conv["J"][unitE]
        print("Erot({})       ".format(outUnit), props["erot"][temp] * conv[unitE][outUnit])

        props["qtot"][temp] = Qtot[n]
        props["qrot"][temp] = Qrot[n]
        props["qtrans"][temp] = Qtrans[n]
        props["qvib"][temp] = Qvib[n]
        props["qvibg"][temp] = Qvibg[n]
        props["qelect"][temp] = Qelect[n]

    return zpe, props


if __name__ == "__main__":
    main()

