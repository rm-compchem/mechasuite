NA = 6.022140857e23  # Avogadro's number
R = {
    "eV": 8.625594481086727e-05,
    "J": 8.3144598,
    "kJ": 0.0083144598,
    "cal": 1.98910521531,
    "kcal": 0.00198910521,
    "Ha": 3.1698433169882826e-06,
}
Kb = {
    "J": 1.38064852e-23,
    "kJ": 1.38064852e-26,
    "eV": 1.43231364885e-28,
    "cal": 3.30298688995e-24,
    "kcal": 3.30298688995e-27,
    "Ha": 5.263648632848294e-30,
}

h = {
    "J":6.62607004e-34,
    "kJ":6.62607004e-37,
    "eV":6.874172680380285e-39,
    "cal":1.5851842200956938e-34,
    "kcal":1.5851842200956938e-37,
    "Ha": 2.526153760496971e-40
}

hbar = {
    "J":   1.054571817e-34,          
    "kJ":  1.054571817e-37,          
    "eV":  6.582119565476075e-16,    
    "cal": 2.5204871343212236e-35,   
    "kcal":2.5204871343212235e-38,   
    "Ha":  2.418884325103622e-17     
}


conv = {
    "J": {
        "J": 1.0,
        "kJ": 0.001,
        "cal": 0.239005671681584,
        "kcal": 0.000239005671681584,
        "eV": 1.0364267568082e-05,
        "Ha": 3.80879832413e-07,
    },
    "cal": {
        "cal": 1,
        "kcal": 0.001,
        "J": 4.18400112835921,
        "kJ": 0.00418400112835921,
        "eV": 43364107199473e-5,
        "Ha": 1.593601648585e-06,
    },
    "kJ": {
        "J": 1000,
        "kJ": 1,
        "cal": 239.005671681584,
        "kcal": 0.239005671681584,
        "eV": 0.010364267568082,
        "Ha": 3.80879832413e-04,
    },
    "kcal": {
        "cal": 1000,
        "kcal": 1,
        "J": 4184.001128359209,
        "kJ": 4.18400112835921,
        "eV": 0.043364107199473,
        "Ha": 0.001593601648585,
    },
    "eV": {
        "eV": 1,
        "J": 96485.3515630563,
        "kJ": 96.4853515630563,
        "kcal":23.0605462577621,
        "cal": 23060.342609859,
        "Ha": 0.036749324533634,
    },
    "Ha": {
        "Ha": 1.0,
        "kJ": 2625.50,
        "J": 2625500,
        "kcal": 627.509391,
        "cal": 627509.391,
        "eV": 27.2113845,
    }
}


colors = {}

colors["H"] = (0.6, 0.6, 0.6, 1.0);
colors["He"] = (0.85, 1.0, 1.0, 1.0);
colors["Li"] = (0.8, 0.5, 1.0, 1.0);
colors["Be"] = (0.75, 1.0, 0.0, 1.0);
colors["B"] = (1.0, 1.0, 0.71, 1.0);
colors["C"] = (0.4, 0.4, 0.4, 1.0);
colors["N"] = (0.05, 0.05, 1.0, 1.0);
colors["O"] = (1.0, 0.05, 0.05, 1.0);
colors["F"] = (0.5, 0.7, 1.0, 1.0);

colors["Na"] = (0.67, 0.36, 0.95, 1.0);
colors["Mg"] = (0.54, 1.0, 0.0, 1.0);
colors["Al"] = (0.75, 0.65, 0.65, 1.0);
colors["Si"] = (0.5, 0.6, 0.6, 1.0);
colors["Si"] = (0.5, 0.6, 0.6, 1.0);
colors["P"] = (1.0, 0.5, 0.0, 1.0);
colors["S"] = (0.7, 0.7, 0.0, 1.0);
colors["Cl"] = (0.12, 0.94, 0.12, 1.0);

colors["K"] = (0.56, 0.25, 0.83, 1.0);
colors["ca"] = (0.24, 1.0, 0.0, 1.0);
colors["Sc"] = (0.9, 0.9, 0.9, 1.0);
colors["Ti"] = (0.74, 0.74, 0.74, 1.0);
colors["V"] = (0.65, 0.65, 0.65, 1.0);
colors["Cr"] = (0.54, 0.6, 0.78, 1.0);
colors["Mn"] = (0.61, 0.48, 0.78, 1.0);
colors["Fe"] = (0.88, 0.4, 0.2, 1.0);
colors["Co"] = (0.5, 0.0, 0.5, 1.0);
colors["Ni"] = (0.14, 0.45, 0.19, 1.0);
# colors["Cu"] = (0.78, 0.5, 0.2, 1.0);
colors["Cu"] = (0.0, 0.5, 0.0, 1.0);
colors["Zn"] = (0.48, 0.5, 0.69, 1.0);
colors["Ga"] = (0.8, 0.8, 0.8, 1.0);
colors["Ge"] = (0.8, 0.8, 0.8, 1.0);
colors["As"] = (0.8, 0.8, 0.8, 1.0);
colors["Se"] = (0.8, 0.8, 0.8, 1.0);
colors["Br"] = (0.65, 0.16, 0.16, 1.0);
colors["Kr"] = (0.8, 0.8, 0.8, 1.0);

colors["Rb"] = (0.8, 0.8, 0.8, 1.0);
colors["Sr"] = (0.8, 0.8, 0.8, 1.0);
colors["Y"] = (0.8, 0.8, 0.8, 1.0);
colors["Zr"] = (0.8, 0.8, 0.8, 1.0);
colors["Nb"] = (0.8, 0.8, 0.8, 1.0);
colors["Mo"] = (0.8, 0.8, 0.8, 1.0);
colors["Tc"] = (0.8, 0.8, 0.8, 1.0);
colors["Ru"] = (0.8, 0.8, 0.8, 1.0);
colors["Pd"] = (0.0, 0.41, 0.52, 1.0);
colors["Ag"] = (1.0, 1.0, 1.0, 1.0);
colors["Cd"] = (0.8, 0.8, 0.8, 1.0);
colors["In"] = (0.8, 0.8, 0.8, 1.0);
colors["Sn"] = (0.8, 0.8, 0.8, 1.0);
colors["Sb"] = (0.8, 0.8, 0.8, 1.0);
colors["Te"] = (0.8, 0.8, 0.8, 1.0);
colors["I"] = (0.58, 0.0, 0.58, 1.0);
colors["Xe"] = (0.8, 0.8, 0.8, 1.0);

colors["Cs"] = (0.8, 0.8, 0.8, 1.0);
colors["Ba"] = (0.8, 0.8, 0.8, 1.0);
colors["Lu"] = (0.8, 0.8, 0.8, 1.0);
colors["Hf"] = (0.8, 0.8, 0.8, 1.0);
colors["Ta"] = (0.8, 0.8, 0.8, 1.0);
colors["W"] = (0.8, 0.8, 0.8, 1.0);
colors["Re"] = (0.8, 0.8, 0.8, 1.0);
colors["Os"] = (0.8, 0.8, 0.8, 1.0);
colors["Ir"] = (0.8, 0.8, 0.8, 1.0);
colors["Pt"] = (0.96, 0.93, 0.82, 1.0);
colors["Au"] = (0.93, 0.94, 0.117, 1.0);
colors["Hg"] = (0.8, 0.8, 0.8, 1.0);
colors["Tl"] = (0.8, 0.8, 0.8, 1.0);
colors["Pb"] = (0.8, 0.8, 0.8, 1.0);
colors["Bi"] = (0.8, 0.8, 0.8, 1.0);
colors["Po"] = (0.8, 0.8, 0.8, 1.0);
colors["At"] = (0.8, 0.8, 0.8, 1.0);
colors["Rn"] = (0.8, 0.8, 0.8, 1.0);

colors["Fr"] = (0.8, 0.8, 0.8, 1.0);
colors["Ra"] = (0.8, 0.8, 0.8, 1.0);
colors["Lr"] = (0.8, 0.8, 0.8, 1.0);
colors["Rf"] = (0.8, 0.8, 0.8, 1.0);
colors["Db"] = (0.8, 0.8, 0.8, 1.0);

colors["La"] = (0.8, 0.8, 0.8, 1.0);
colors["Ce"] = (0.8, 0.8, 0.8, 1.0);
colors["Rh"] = (0.8, 0.8, 0.8, 1.0);

radios = {}
radios["H"] = 0.50;
radios["He"] = 0.31;
radios["Li"] = 1.28;
radios["Be"] = 0.96;
radios["B"] = 0.84;
radios["C"] = 0.80;
radios["N"] = 0.85;
radios["O"] = 0.98;
radios["F"] = 0.80;
radios["Ne"] = 0.69;
radios["Na"] = 1.66;
radios["Mg"] = 1.41;
radios["Al"] = 0.85;
radios["Si"] = 0.98;
radios["P"] = 0.98;
radios["S"] = 0.88;
radios["Cl"] = 0.95;
radios["Ar"] = 1.06;
radios["K"] = 2.03;
radios["Ca"] = 1.76;
radios["Sc"] = 1.70;
radios["Ti"] = 1.60;
radios["V"] = 1.53;
radios["Cr"] = 1.39;
radios["Mn"] = 1.39;
radios["Fe"] = 1.32;
radios["Co"] = 1.30;
radios["Ni"] = 1.30;
radios["Cu"] = 1.30;
radios["Zn"] = 1.31;
radios["Ga"] = 1.22;
radios["Ge"] = 1.20;
radios["As"] = 1.19;
radios["Se"] = 1.20;
radios["Br"] = 1.20;
radios["Kr"] = 1.16;
radios["Rb"] = 2.20;
radios["Sr"] = 1.95;
radios["Y"] = 1.90;
radios["Zr"] = 1.75;
radios["Nb"] = 1.64;
radios["Mo"] = 1.54;
radios["Tc"] = 1.47;
radios["Ru"] = 1.46;
radios["Rh"] = 1.52;
radios["Pd"] = 1.30;
radios["Ag"] = 1.45;
radios["Cd"] = 1.44;
radios["In"] = 1.42;
radios["Sn"] = 1.39;
radios["Sb"] = 1.39;
radios["Te"] = 1.38;
radios["I"] = 1.39;
radios["Xe"] = 1.40;
radios["Cs"] = 2.44;
radios["Ba"] = 2.15;
radios["La"] = 2.07;
radios["Ce"] = 2.04;
radios["Pr"] = 2.03;
radios["Nd"] = 2.01;
radios["Sm"] = 1.98;
radios["Eu"] = 1.98;
radios["Gd"] = 1.96;
radios["Tb"] = 1.94;
radios["Dy"] = 1.92;
radios["Ho"] = 1.92;
radios["Er"] = 1.89;
radios["Tm"] = 1.90;
radios["Yb"] = 1.87;
radios["Lu"] = 1.87;
radios["Hf"] = 1.75;
radios["Ta"] = 1.70;
radios["W"] = 1.62;
radios["Re"] = 1.51;
radios["Os"] = 1.44;
radios["Ir"] = 1.41;
radios["Pt"] = 1.30;
radios["Au"] = 1.30;
radios["Hg"] = 1.30;
radios["Tl"] = 1.45;
radios["Pb"] = 1.44;
radios["Bi"] = 1.51;
radios["Po"] = 1.50;
radios["At"] = 1.50;
radios["Rn"] = 1.50;
radios["Fr"] = 2.60;
radios["Ra"] = 2.21;


cylinders = {}
cylinders["H"] = 0.15;
cylinders["He"] = 0.15;
cylinders["Li"] = 0.18;
cylinders["Be"] = 0.18;
cylinders["B"] = 0.18;
cylinders["C"] = 0.20;
cylinders["N"] = 0.20;
cylinders["O"] = 0.20;
cylinders["F"] = 0.20;
cylinders["Ne"] = 0.18;
cylinders["Na"] = 0.20;
cylinders["Mg"] = 0.20;
cylinders["Al"] = 0.20;
cylinders["Si"] = 0.20;
cylinders["P"] = 0.20;
cylinders["S"] = 0.20;
cylinders["Cl"] = 0.20;
cylinders["Ar"] = 0.18;
cylinders["K"] = 0.20;
cylinders["Ca"] = 0.20;
cylinders["Sc"] = 0.20;
cylinders["Ti"] = 0.20;
cylinders["V"] = 0.20;
cylinders["Cr"] = 0.20;
cylinders["Mn"] = 0.20;
cylinders["Fe"] = 0.20;
cylinders["Co"] = 0.20;
cylinders["Ni"] = 0.20;
cylinders["Cu"] = 0.20;
cylinders["Zn"] = 0.20;
cylinders["Ga"] = 0.20;
cylinders["Ge"] = 0.20;
cylinders["As"] = 0.20;
cylinders["Se"] = 0.20;
cylinders["Br"] = 0.20;
cylinders["Kr"] = 0.18;
cylinders["Rb"] = 0.20;
cylinders["Sr"] = 0.20;
cylinders["Y"] = 0.20;
cylinders["Zr"] = 0.20;
cylinders["Nb"] = 0.20;
cylinders["Mo"] = 0.20;
cylinders["Tc"] = 0.20;
cylinders["Ru"] = 0.20;
cylinders["Rh"] = 0.20;
cylinders["Pd"] = 0.20;
cylinders["Ag"] = 0.20;
cylinders["Cd"] = 0.20;
cylinders["In"] = 0.20;
cylinders["Sn"] = 0.20;
cylinders["Sb"] = 0.20;
cylinders["Te"] = 0.20;
cylinders["I"] = 0.20;
cylinders["Xe"] = 0.18;
cylinders["Cs"] = 0.20;
cylinders["Ba"] = 0.20;
cylinders["La"] = 0.20;
cylinders["Ce"] = 0.20;
cylinders["Pr"] = 0.20;
cylinders["Nd"] = 0.20;
cylinders["Sm"] = 0.20;
cylinders["Eu"] = 0.20;
cylinders["Gd"] = 0.20;
cylinders["Tb"] = 0.20;
cylinders["Dy"] = 0.20;
cylinders["Ho"] = 0.20;
cylinders["Er"] = 0.20;
cylinders["Tm"] = 0.20;
cylinders["Yb"] = 0.20;
cylinders["Lu"] = 0.20;
cylinders["Hf"] = 0.20;
cylinders["Ta"] = 0.20;
cylinders["W"] = 0.20;
cylinders["Re"] = 0.20;
cylinders["Os"] = 0.20;
cylinders["Ir"] = 0.20;
cylinders["Pt"] = 0.20;
cylinders["Au"] = 0.20;
cylinders["Hg"] = 0.20;
cylinders["Tl"] = 0.20;
cylinders["Pb"] = 0.20;
cylinders["Bi"] = 0.20;
cylinders["Po"] = 0.20;
cylinders["At"] = 0.20;
cylinders["Rn"] = 0.18;
cylinders["Fr"] = 0.20;
cylinders["Ra"] = 0.20;


spheres = {}
spheres["H"] = 0.25;
spheres["He"] = 0.25;
spheres["Li"] = 0.60;
spheres["Be"] = 0.50;
spheres["B"] = 0.35;
spheres["C"] = 0.38;
spheres["N"] = 0.35;
spheres["O"] = 0.36;
spheres["F"] = 0.30;
spheres["Ne"] = 0.30;
spheres["Na"] = 0.80;
spheres["Mg"] = 0.70;
spheres["Al"] = 0.30;
spheres["Si"] = 0.30;
spheres["P"] = 0.30;
spheres["S"] = 0.32;
spheres["Cl"] = 0.30;
spheres["Ar"] = 0.32;
spheres["K"] = 1.00;
spheres["Ca"] = 1.00;
spheres["Sc"] = 1.00;
spheres["Ti"] = 1.00;
spheres["V"] = 1.00;
spheres["Cr"] = 0.90;
spheres["Mn"] = 0.85;
spheres["Fe"] = 0.80;
spheres["Co"] = 0.65;
spheres["Ni"] = 1.00;
spheres["Cu"] = 1.00;
spheres["Zn"] = 1.00;
spheres["Ga"] = 0.60;
spheres["Ge"] = 0.50;
spheres["As"] = 0.45;
spheres["Se"] = 0.40;
spheres["Br"] = 0.40;
spheres["Kr"] = 0.35;
spheres["Rb"] = 1.00;
spheres["Sr"] = 1.00;
spheres["Y"] = 1.00;
spheres["Zr"] = 1.00;
spheres["Nb"] = 1.00;
spheres["Mo"] = 0.90;
spheres["Tc"] = 0.90;
spheres["Ru"] = 0.90;
spheres["Rh"] = 1.00;
spheres["Pd"] = 1.00;
spheres["Ag"] = 1.00;
spheres["Cd"] = 1.00;
spheres["In"] = 0.80;
spheres["Sn"] = 0.70;
spheres["Sb"] = 0.60;
spheres["Te"] = 0.55;
spheres["I"] = 0.50;
spheres["Xe"] = 0.45;
spheres["Cs"] = 1.00;
spheres["Ba"] = 1.00;
spheres["La"] = 1.00;
spheres["Ce"] = 1.00;
spheres["Pr"] = 1.00;
spheres["Nd"] = 1.00;
spheres["Sm"] = 1.00;
spheres["Eu"] = 1.00;
spheres["Gd"] = 1.00;
spheres["Tb"] = 1.00;
spheres["Dy"] = 1.00;
spheres["Ho"] = 1.00;
spheres["Er"] = 1.00;
spheres["Tm"] = 1.00;
spheres["Yb"] = 1.00;
spheres["Lu"] = 1.00;
spheres["Hf"] = 1.00;
spheres["Ta"] = 1.00;
spheres["W"] = 1.00;
spheres["Re"] = 1.00;
spheres["Os"] = 1.00;
spheres["Ir"] = 1.00;
spheres["Pt"] = 1.00;
spheres["Au"] = 1.00;
spheres["Hg"] = 0.65;
spheres["Tl"] = 0.70;
spheres["Pb"] = 0.70;
spheres["Bi"] = 0.65;
spheres["Po"] = 0.60;
spheres["At"] = 0.55;
spheres["Rn"] = 0.50;
spheres["Fr"] = 1.00;
spheres["Ra"] = 1.00;



masas = {
     "H": 1.008,
     "Li": 6.94,
     "Be": 9.0122,
     "B": 10.81,
     "C": 12.01,
     "N": 14.007,
     "O": 15.999,
     "F": 18.9984,
     "Na":22.98976928,
     "Mg":24.3055,
     "Al":26.981538,
     "Si":28.085,
     "P":30.973761998,
     "S":32.0675,
     "Cl":35.451,
     "K":39.0983,
     "Ca":40.079,
     "Sc":44.955908,
     "Ti":47867,
     "V":50.9415,
     "Cr":51.9961,
     "Mn":54.938044,
     "Fe":55.845,
     "Co":58.933194,
     "Ni":58.6934,
     "Cu":63.546,
     "Zn":65.38,
     "Ga":69.723,
     "Ge":72.630,
     "As":74.921595,
     "Se":78.971,
     "Br":79.904,
     "Rb":85.4678,
     "Sr":87.62,
     "Y":88.90584,
     "Zr":91.224,
     "Nb":92.90637,
     "Mo":95.95,
     "Ru":101.07,
     "Rh":102.90550,
     "Pd":106.42,
     "Ag":107.8682,
     "Cd":112.414,
     "In":114.818,
     "Sn":118.710,
     "Sb":121.760,
     "Te":127.60,
     "I":126.9045,
     "Cs":132.90545196,
     "Ba":137.327,
     "La":138.90547,
     "Ce":140.116,
     "Pr":140.90766,
     "Nd":144.242,
     "W":183.84,
     "Re":186.207,
     "Os":190.23,
     "Ir":192.217,
     "Pt":195.084,
     "Au":196.966569,
     "Hg":200.592,
     "Tl":204.3835,
     "Pb":207.2,
     "Bi":208.98040,
     "U":238.02891,
}

# for the symmetric number of each point group
sigma = {
    'C1'  :1,
    'Cs'  :1,
    'C2'  :2,
    'C2v' :2,
    'C3v' :3,
    'C2h' :2,
    'Coov':1,
    'D2h' :4,
    'D3h' :6,
    'D5h' :10,
    'Dooh':2,
    'D3d' :6,
    'Td'  :12,
    'Oh'  :24,
}
