from mechasuite.pyposcar import *


def read_oszicar_energy(oszicar):
    energy = 0.0
    with open(oszicar) as f:
        lineas = f.readlines()
    for line in lineas:
        if "F=" in line:
            energy = float(line.split()[4])
            spin = round(float(line.split()[9]))
    return energy, spin

def read_orca_pg(outfile):
    pg = None
    with open(outfile) as f:
        lines = f.readlines()
    for line in lines:
        if "Point Group" in line:
            pg = line.split()[2][:-1]
    if pg == "C(inf)v":
        pg = "Coov"
    elif pg == "D(inf)h":
        pg = "Dooh"
    return pg

def read_orca_energy(outfile):
    energy = 0.0
    with open(outfile) as f:
        lines = f.readlines()
    for line in lines:
        if "FINAL SINGLE POINT ENERGY" in line:
            energy = float(line.split()[-1])
    return energy

def read_outcar_freqs(outcar):
    freqs = []
    unit = None
    rf = False
    with open(outcar) as f:
        lineas = f.readlines()

    for num, linea in enumerate(lineas):
        if "Eigenvectors and eigenvalues" in linea:
            if not rf:
                rf = True
            else:
                break
        if "THz" in linea and "cm-1" in linea and rf:
            try:
                freqs.append(float(linea.split()[7]))
            except:
                freqs.append(-float(linea.split()[6]))
    unit = "cm-1"
    return freqs, unit

def read_mfreq(mfreqfile, vibfile):
    freqs = []
    try:
        with open(mfreqfile) as f:
            lines = f.readlines()
    except FileNotFoundError:
        return []

    for i, line in enumerate(lines):
        #print(line)
        if "Frequencies" in line: # skip first line
            while lines[i+1].split()[0] != "The":
                print(line.split())
                i += 1
                line = lines[i]
            # now read frequencies until blank line
                freqs += [float(f) for f in line.split()]
            print(freqs)

        elif "The Reduced Mass orthogonal to the seam of crossing is" in line:
            redmass = float(lines[i+1].split()[0])
            #print("Red mass: ", redmass)
        elif "The geometric mean of the norms of the two" in line:
            grad = float(lines[i+1].split()[0])
            #print("Geom mean: ", geommean)
        elif "The norm of the difference gradient" in line:
            grad_diff =float(lines[i+1].split()[0])
            #print("Geom mean: ", geommean)
    unit = "cm-1"
    if vibfile:
        try:
            with open(vibfile) as f:
                lines = f.readlines()
        except FileNotFoundError:
            return freqs, unit, redmass, grad, grad_diff


        

    return freqs, unit, redmass, grad, grad_diff


def read_orca_freqs(outfile):
    freqs = []
    import re
    float_re = re.compile(r'[-+]?\d*\.\d+|[-+]?\d+')
    freq_line_re = re.compile(r'^\s*\d+\s*:')
    try:
        with open(outfile, 'r') as f:
            lines = f.readlines()
    except FileNotFoundError:
        return []

    started = False
    for line in lines:
        if "VIBRATIONAL FREQUENCIES" in line.upper():
            started = True
            continue
        if not started:
            continue

        # skip leading blank lines until the block with indexed lines begins
        if line.strip() == "":
            # if we already collected frequencies, a blank line ends the block
            if freqs:
                break
            else:
                continue

        if freq_line_re.match(line):
            # take text after the first ':' so we don't pick the index number
            rest = line.split(':', 1)[1]
            m = float_re.search(rest)
            if m:
                try:
                    freqs.append(float(m.group()))
                except ValueError:
                    continue
            continue

        # if we hit a non-indexed line after collecting frequencies, stop
        if freqs:
            break

    return freqs

def read_tp_from_outcar(outcar):
    with open(outcar) as f:
        lines = f.readlines()
    tp = "m"
    for num, line in enumerate(lines):
        if "Running the Dimer method" in line:
            tp = "ts"
            break
        if num == 2000:
            break
    print(tp)
    return tp


def read_xyz(infile):
    try:
        with open(infile) as f:
            lines = f.readlines()
    except FileNotFoundError:
        print("No such file or directory: ", infile)
        return [], [], 0

    labels = []
    coors = []
    try:
        nat = int(lines[0])
        for i in range(2, nat+2):
            line = lines[i].split()
            labels.append(line[0])
            coor = [float(c) for c in line[1:4]]
            coors.append(coor)
    except Exception as e:
        print("Error reading xyz coordinates from:"+infile)
        print(e)
        labels = []
        coors = []
        nat = 0

    return nat, labels, coors


def read_poscar(poscar):
    p = Poscar()
    try:
        p.load_poscar(poscar)
    except Exception as e:
        print(e)
        return 0, [], [], []

    p.extend_label()
    print(p.coortype)
    if p.coortype == "Direct":
        p.xyz = p.frac_cart(p.coor)
    else:
        p.xyz = p.coor

    return p.totalat, p.label_extended, p.xyz, list(p.celda)


def mult_split(string):
    if "-" not in string:
        return []

    temps = []
    string = string.split("-")
    if len(string) != 2:
        return []
    string[1] = string[1].split(":")
    try:
        inittemp = float(string[0])
    except ValueError:
        return []
    try:
        endtemp = float(string[1][0])
    except ValueError:
        return []

    if len(string[1]) == 2:
        try:
            step = int(string[1][1])
        except ValueError:
            return []
    else:
        step = 1.0

    while inittemp < endtemp:
        temps.append(inittemp)
        inittemp += step
    temps.append(endtemp)
    return temps


def unfold_temps(tstring):
    temps = []
    if tstring == "":
        return []

    tstring = tstring.split(",")
    for i in tstring:
        rang = False
        try:
            t = float(i)
        except ValueError:
            rang = True
        else:
            temps.append(t)

        if rang:
            unfoldedtemp = mult_split(i)
            if unfoldedtemp:
                temps += unfoldedtemp

    return temps


def read_thermo_input():
    modes = 0
    ts = False
    with open("thermo_input") as f:
        lines = f.readlines()

    for line in lines:
        if "T" in line:
            temps = line.split()[-1]
            temps = unfold_temps(temps)
        if "pg" in line.lower():
            pg = line.split()[-1]
        if "modes" in line.lower():
            modes = line.split()[-1]
        if "ts" in line.lower():
            ts = True

    return temps, pg, int(modes), ts


def read_energy_from_gaussian(ef):
    E = 0
    with open(ef) as f:
        for line in f:
            if "HF=" in line:
                ls = line.split('\\')
                for i in ls:
                    if "HF=" in i:
                        E = i.replace("HF=", "")
                        E = float(E)
    return E


def read_freqs_gaussian(ef):
    vibs = []
    with open(ef) as f:
        for line in f:
            if "Frequencies --" in line:
                freqs = line.split()[2:]
                for freq in freqs:
                    vibs.append(float(freq))
    return vibs
