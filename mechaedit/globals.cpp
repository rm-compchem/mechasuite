#include "globals.h"


using namespace std;

QMap<QString, float> radios;
QMap<QString, float> spheres;
QMap<QString, float> cylinders;
QMap<QString, float> masas;
QMap<QString, array<float,4>> colors;
array<SPACEGROUP, 230> spaceGroups;
QString settings_folder = "",applicationFolder="";
QString fragmentsFolder="";
double totalMemory = 0;
double availableMemory = 0;
double memUsage = 0;

array<float, 12> clw;
array<array<float, 4>, 12> clc;

array<string,73> element_symbols = {
    "H", "He", "Li", "Be", "B", "C", "N", "O", "F", "Ne", "Na", "Mg", "Al", "Si", "P", "S", "Cl", "Ar", "K", "Ca", "Sc", "Ti", "V",
    "Cr", "Mn", "Fe", "Co", "Ni", "Cu", "Zn", "Ga", "Ge", "As", "Se", "Br", "Kr", "Rb", "Sr", "Y", "Zr", "Nb", "Mo", "Tc", "Ru",
    "Rh", "Pd", "Ag", "Cd", "In", "Sn", "Sb", "Te", "I", "Xe", "Cs", "Ba", "Hf", "Ta", "W", "Re", "Os", "Ir", "Pt", "Au", "Hg",
    "Tl", "Pb", "Bi", "Po", "At", "Rn","Fr", "Ra"
};

void setSpaceGroups(){
    SPACEGROUP sp;
    sp.name = "P1";
    sp.number = 1;
    sp.symOperations.clear();
    sp.symOperations << "x,y,z";
    spaceGroups[0] = sp;

    sp.name = "P-1";
    sp.number = 2;
    sp.symOperations.clear();
    sp.symOperations << "x,y,z" <<"-x,-y,-z";
    spaceGroups[1] = sp;

    sp.name = "P2";
    sp.number = 3;
    sp.symOperations.clear();
    sp.symOperations << "x,y,z" <<"-x,y,-z";
    spaceGroups[2] = sp;

    sp.name = "P21";
    sp.number = 4;
    sp.symOperations.clear();
    sp.symOperations << "x,y,z" <<"-x,y+1/2,-z";
    spaceGroups[3] = sp;

    sp.name = "C2";
    sp.number = 5;
    sp.symOperations.clear();
    sp.symOperations << "x,y,z"
            <<"-x,y,-z"
            <<"x+1/2,y+1/2,z"
            <<"-x+1/2,y+1/2,-z";
    spaceGroups[4] = sp;

    sp.name = "Pm";
    sp.number = 6;
    sp.symOperations.clear();
    sp.symOperations << "x,y,z"
                        <<"x,-y,z";
    spaceGroups[5] = sp;

    sp.name = "Pc";
    sp.number = 7;
    sp.symOperations.clear();
    sp.symOperations << "x,y,z"
                        <<"x,-y,z+1/2";
    spaceGroups[6] = sp;

    sp.name = "Cm";
    sp.number = 8;
    sp.symOperations.clear();
    sp.symOperations << "x,y,z"
                     <<"x,-y,z"
                     <<"x+1/2,y+1/2,z"
                     <<"x+1/2,-y+1/2,z";
    spaceGroups[7] = sp;

    sp.name = "Cc";
    sp.number = 9;
    sp.symOperations.clear();
    sp.symOperations << "x,y,z"
                     <<"x,-y,z+1/2"
                     <<"x+1/2,y+1/2,z"
                     <<"x+1/2,-y+1/2,z+1/2";
    spaceGroups[8] = sp;

    sp.name = "Cc";
    sp.number = 10;
    sp.symOperations.clear();
    sp.symOperations << "x,y,z"
            <<"-x,y,-z"
            <<"-x,-y,-z"
            <<"x,-y,z";
    spaceGroups[9] = sp;

    //---------------------
    sp.name = "C2/m";
    sp.number = 12;
    sp.symOperations.clear();
    sp.symOperations << "x,y,z"
            <<"-x,y,-z"
            <<"-x,-y,-z"
            <<"x,-y,z"
            <<"x+1/2,y+1/2,z"
            <<"-x+1/2,y+1/2,-z"
            <<"-x+1/2,-y+1/2,-z"
            <<"x+1/2,-y+1/2,z";
    spaceGroups[11] = sp;
    //-----------------------
    sp.name = "P2/c";
    sp.number = 13;
    sp.symOperations.clear();
    sp.symOperations << "x,y,z" << "-x,y,-z+1/2" << "-x,-y,-z" <<"x,-y,z+1/2";
    spaceGroups[12] = sp;
    //--------------------
    sp.name = "P21/c";
    sp.number = 14;
    sp.symOperations.clear();
    sp.symOperations << "x,y,z" <<"-x,y+1/2,-z+1/2" << "-x,-y,-z" << "x,-y+1/2,z+1/2";
    spaceGroups[13] = sp;
    //--------------------
    sp.name = "C2/c";
    sp.number = 15;
    sp.symOperations.clear();
    sp.symOperations <<"x,y,z"
                      <<"-x,y,-z+1/2"
                      <<"-x,-y,-z"
                      <<"x,-y,z+1/2"
                      <<"x+1/2,y+1/2,z"
                      <<"-x+1/2,y+1/2,-z+1/2"
                      <<"-x+1/2,-y+1/2,-z"
                      <<"8 x+1/2,-y+1/2,z+1/2";
    spaceGroups[14] = sp;
    //--------------------
    sp.name = "P222";
    sp.number = 16;
    sp.symOperations.clear();
    sp.symOperations <<"x,y,z"<<"-x,-y,z"<<"-x,y,-z"<<"x,-y,-z";
    spaceGroups[15] = sp;
    //--------------------
    sp.name = "P2221";
    sp.number = 17;
    sp.symOperations.clear();
    sp.symOperations <<"x,y,z"<<"-x,-y,z+1/2"<<"-x,y,-z+1/2"<<"x,-y,-z";
    spaceGroups[16] = sp;
    //--------------------
    sp.name = "P21212";
    sp.number = 18;
    sp.symOperations.clear();
    sp.symOperations <<"x,y,z"<<"-x,-y,z"<<"-x+1/2,y+1/2,-z"<<"x+1/2,-y+1/2,-z";
    spaceGroups[17] = sp;

    sp.name = "";
    sp.number = 19;
    sp.symOperations.clear();
    sp.symOperations
                     <<"x,y,z"
                     <<"-x+1/2,-y,z+1/2"
                     <<"-x,y+1/2,-z+1/2"
                     <<"x+1/2,-y+1/2,-z";
    spaceGroups[18] = sp;
    //-----------------------
    sp.name = "";
    sp.number = 20;
    sp.symOperations.clear();
    sp.symOperations
                     <<"x,y,z"
                     <<"-x,-y,z+1/2"
                     <<"-x,y,-z+1/2"
                     <<"x,-y,-z"
                     <<"x+1/2,y+1/2,z"
                     <<"-x+1/2,-y+1/2,z+1/2"
                     <<"-x+1/2,y+1/2,-z+1/2"
                     <<"x+1/2,-y+1/2,-z";
    spaceGroups[19] = sp;
    //-----------------------
    sp.name = "";
    sp.number = 21;
    sp.symOperations.clear();
    sp.symOperations
                     <<"x,y,z"
                     <<"-x,-y,z"
                     <<"-x,y,-z"
                     <<"x,-y,-z"
                     <<"x+1/2,y+1/2,z"
                     <<"-x+1/2,-y+1/2,z"
                     <<"-x+1/2,y+1/2,-z"
                     <<"x+1/2,-y+1/2,-z";
    spaceGroups[20] = sp;
    //-----------------------
    sp.name = "";
    sp.number = 22;
    sp.symOperations.clear();
    sp.symOperations
                     <<"x,y,z"
                     <<"-x,-y,z"
                     <<"-x,y,-z"
                     <<"x,-y,-z"
                     <<"x,y+1/2,z+1/2"
                     <<"-x,-y+1/2,z+1/2"
                     <<"-x,y+1/2,-z+1/2"
                     <<"x,-y+1/2,-z+1/2"
                     <<"x+1/2,y,z+1/2"
                     <<"-x+1/2,-y,z+1/2"
                     <<"-x+1/2,y,-z+1/2"
                     <<"x+1/2,-y,-z+1/2"
                     <<"x+1/2,y+1/2,z"
                     <<"-x+1/2,-y+1/2,z"
                     <<"-x+1/2,y+1/2,-z"
                     <<"x+1/2,-y+1/2,-z";
    spaceGroups[21] = sp;
    //-----------------------
    sp.name = "";
    sp.number = 23;
    sp.symOperations.clear();
    sp.symOperations
                     <<"x,y,z"
                     <<"-x,-y,z"
                     <<"-x,y,-z"
                     <<"x,-y,-z"
                     <<"x+1/2,y+1/2,z+1/2"
                     <<"-x+1/2,-y+1/2,z+1/2"
                     <<"-x+1/2,y+1/2,-z+1/2"
                     <<"x+1/2,-y+1/2,-z+1/2";
    spaceGroups[22] = sp;
    //-----------------------
    sp.name = "";
    sp.number = 24;
    sp.symOperations.clear();
    sp.symOperations
                     <<"x,y,z"
                     <<"-x+1/2,-y,z+1/2"
                     <<"-x,y+1/2,-z+1/2"
                     <<"x+1/2,-y+1/2,-z"
                     <<"x+1/2,y+1/2,z+1/2"
                     <<"-x,-y+1/2,z"
                     <<"-x+1/2,y,-z"
                     <<"x,-y,-z+1/2";
    spaceGroups[23] = sp;
    //-----------------------
    sp.name = "";
    sp.number = 25;
    sp.symOperations.clear();
    sp.symOperations
                     <<"x,y,z"
                     <<"-x,-y,z"
                     <<"x,-y,z"
                     <<"-x,y,z";
    spaceGroups[24] = sp;
    //-----------------------
    sp.name = "";
    sp.number = 26;
    sp.symOperations.clear();
    sp.symOperations
                     <<"x,y,z"
                     <<"-x,-y,z+1/2"
                     <<"x,-y,z+1/2"
                     <<"-x,y,z";
    spaceGroups[25] = sp;
    //-----------------------
    sp.name = "";
    sp.number = 27;
    sp.symOperations.clear();
    sp.symOperations
                     <<"x,y,z"
                     <<"-x,-y,z"
                     <<"x,-y,z+1/2"
                     <<"-x,y,z+1/2";
    spaceGroups[26] = sp;
    //-----------------------
    sp.name = "";
    sp.number = 28;
    sp.symOperations.clear();
    sp.symOperations
                     <<"x,y,z"
                     <<"-x,-y,z"
                     <<"x+1/2,-y,z"
                     <<"-x+1/2,y,z";
    spaceGroups[27] = sp;
    //-----------------------
    sp.name = "";
    sp.number = 29;
    sp.symOperations.clear();
    sp.symOperations
                     <<"x,y,z"
                     <<"-x,-y,z+1/2"
                     <<"x+1/2,-y,z"
                     <<"-x+1/2,y,z+1/2";
    spaceGroups[28] = sp;
    //-----------------------
    sp.name = "";
    sp.number = 30;
    sp.symOperations.clear();
    sp.symOperations
                     <<"x,y,z"
                     <<"-x,-y,z"
                     <<"x,-y+1/2,z+1/2"
                     <<"-x,y+1/2,z+1/2";
    spaceGroups[29] = sp;
    //-----------------------
    sp.name = "";
    sp.number = 31;
    sp.symOperations.clear();
    sp.symOperations
                     <<"x,y,z"
                     <<"-x+1/2,-y,z+1/2"
                     <<"x+1/2,-y,z+1/2"
                     <<"-x,y,z";
    spaceGroups[30] = sp;
    //-----------------------
    sp.name = "";
    sp.number = 32;
    sp.symOperations.clear();
    sp.symOperations
                     <<"x,y,z"
                     <<"-x,-y,z"
                     <<"x+1/2,-y+1/2,z"
                     <<"-x+1/2,y+1/2,z";
    spaceGroups[31] = sp;
    //-----------------------
    sp.name = "";
    sp.number = 33;
    sp.symOperations.clear();
    sp.symOperations
                     <<"x,y,z"
                     <<"-x,-y,z+1/2"
                     <<"x+1/2,-y+1/2,z"
                     <<"-x+1/2,y+1/2,z+1/2";
    spaceGroups[32] = sp;
    //-----------------------
    sp.name = "";
    sp.number = 34;
    sp.symOperations.clear();
    sp.symOperations
                     <<"x,y,z"
                     <<"-x,-y,z"
                     <<"x+1/2,-y+1/2,z+1/2"
                     <<"-x+1/2,y+1/2,z+1/2";
    spaceGroups[33] = sp;
    //-----------------------
    sp.name = "";
    sp.number = 35;
    sp.symOperations.clear();
    sp.symOperations
                     <<"x,y,z"
                     <<"-x,-y,z"
                     <<"x,-y,z"
                     <<"-x,y,z"
                     <<"x+1/2,y+1/2,z"
                     <<"-x+1/2,-y+1/2,z"
                     <<"x+1/2,-y+1/2,z"
                     <<"-x+1/2,y+1/2,z";
    spaceGroups[34] = sp;
    //-----------------------
    sp.name = "";
    sp.number = 36;
    sp.symOperations.clear();
    sp.symOperations
                     <<"x,y,z"
                     <<"-x,-y,z+1/2"
                     <<"x,-y,z+1/2"
                     <<"-x,y,z"
                     <<"x+1/2,y+1/2,z"
                     <<"-x+1/2,-y+1/2,z+1/2"
                     <<"x+1/2,-y+1/2,z+1/2"
                     <<"-x+1/2,y+1/2,z";
    spaceGroups[35] = sp;
    //-----------------------
    sp.name = "";
    sp.number = 37;
    sp.symOperations.clear();
    sp.symOperations
                     <<"x,y,z"
                     <<"-x,-y,z"
                     <<"x,-y,z+1/2"
                     <<"-x,y,z+1/2"
                     <<"x+1/2,y+1/2,z"
                     <<"-x+1/2,-y+1/2,z"
                     <<"x+1/2,-y+1/2,z+1/2"
                     <<"-x+1/2,y+1/2,z+1/2";
    spaceGroups[36] = sp;
    //-----------------------
    sp.name = "";
    sp.number = 38;
    sp.symOperations.clear();
    sp.symOperations
                     <<"x,y,z"
                     <<"-x,-y,z"
                     <<"x,-y,z"
                     <<"-x,y,z"
                     <<"x,y+1/2,z+1/2"
                     <<"-x,-y+1/2,z+1/2"
                     <<"x,-y+1/2,z+1/2"
                     <<"-x,y+1/2,z+1/2";
    spaceGroups[37] = sp;
    //-----------------------
    sp.name = "";
    sp.number = 39;
    sp.symOperations.clear();
    sp.symOperations
                     <<"x,y,z"
                     <<"-x,-y,z"
                     <<"x,-y+1/2,z"
                     <<"-x,y+1/2,z"
                     <<"x,y+1/2,z+1/2"
                     <<"-x,-y+1/2,z+1/2"
                     <<"x,-y,z+1/2"
                     <<"-x,y,z+1/2";
    spaceGroups[38] = sp;
    //-----------------------
    sp.name = "";
    sp.number = 40;
    sp.symOperations.clear();
    sp.symOperations
                     <<"x,y,z"
                     <<"-x,-y,z"
                     <<"x+1/2,-y,z"
                     <<"-x+1/2,y,z"
                     <<"x,y+1/2,z+1/2"
                     <<"-x,-y+1/2,z+1/2"
                     <<"x+1/2,-y+1/2,z+1/2"
                     <<"-x+1/2,y+1/2,z+1/2";
    spaceGroups[39] = sp;
    //-----------------------
    sp.name = "";
    sp.number = 41;
    sp.symOperations.clear();
    sp.symOperations
                     <<"x,y,z"
                     <<"-x,-y,z"
                     <<"x+1/2,-y+1/2,z"
                     <<"-x+1/2,y+1/2,z"
                     <<"x,y+1/2,z+1/2"
                     <<"-x,-y+1/2,z+1/2"
                     <<"x+1/2,-y,z+1/2"
                     <<"-x+1/2,y,z+1/2";
    spaceGroups[40] = sp;
    //-----------------------
    sp.name = "";
    sp.number = 42;
    sp.symOperations.clear();
    sp.symOperations
                     <<"x,y,z"
                     <<"-x,-y,z"
                     <<"x,-y,z"
                     <<"-x,y,z"
                     <<"x,y+1/2,z+1/2"
                     <<"-x,-y+1/2,z+1/2"
                     <<"x,-y+1/2,z+1/2"
                     <<"-x,y+1/2,z+1/2"
                     <<"x+1/2,y,z+1/2"
                     <<"-x+1/2,-y,z+1/2"
                     <<"x+1/2,-y,z+1/2"
                     <<"-x+1/2,y,z+1/2"
                     <<"x+1/2,y+1/2,z"
                     <<"-x+1/2,-y+1/2,z"
                     <<"x+1/2,-y+1/2,z"
                     <<"-x+1/2,y+1/2,z";
    spaceGroups[41] = sp;
    //-----------------------
    sp.name = "";
    sp.number = 43;
    sp.symOperations.clear();
    sp.symOperations
                     <<"x,y,z"
                     <<"-x,-y,z"
                     <<"x+1/4,-y+1/4,z+1/4"
                     <<"-x+1/4,y+1/4,z+1/4"
                     <<"x,y+1/2,z+1/2"
                     <<"-x,-y+1/2,z+1/2"
                     <<"x+1/4,-y+3/4,z+3/4"
                     <<"-x+1/4,y+3/4,z+3/4"
                     <<"x+1/2,y,z+1/2"
                     <<"-x+1/2,-y,z+1/2"
                     <<"x+3/4,-y+1/4,z+3/4"
                     <<"-x+3/4,y+1/4,z+3/4"
                     <<"x+1/2,y+1/2,z"
                     <<"-x+1/2,-y+1/2,z"
                     <<"x+3/4,-y+3/4,z+1/4"
                     <<"-x+3/4,y+3/4,z+1/4";
    spaceGroups[42] = sp;
    //-----------------------
    sp.name = "";
    sp.number = 44;
    sp.symOperations.clear();
    sp.symOperations
                     <<"x,y,z"
                     <<"-x,-y,z"
                     <<"x,-y,z"
                     <<"-x,y,z"
                     <<"x+1/2,y+1/2,z+1/2"
                     <<"-x+1/2,-y+1/2,z+1/2"
                     <<"x+1/2,-y+1/2,z+1/2"
                     <<"-x+1/2,y+1/2,z+1/2";
    spaceGroups[43] = sp;
    //-----------------------
    sp.name = "";
    sp.number = 45;
    sp.symOperations.clear();
    sp.symOperations
                     <<"x,y,z"
                     <<"-x,-y,z"
                     <<"x+1/2,-y+1/2,z"
                     <<"-x+1/2,y+1/2,z"
                     <<"x+1/2,y+1/2,z+1/2"
                     <<"-x+1/2,-y+1/2,z+1/2"
                     <<"x,-y,z+1/2"
                     <<"-x,y,z+1/2";
    spaceGroups[44] = sp;
    //-----------------------
    sp.name = "";
    sp.number = 46;
    sp.symOperations.clear();
    sp.symOperations
                     <<"x,y,z"
                     <<"-x,-y,z"
                     <<"x+1/2,-y,z"
                     <<"-x+1/2,y,z"
                     <<"x+1/2,y+1/2,z+1/2"
                     <<"-x+1/2,-y+1/2,z+1/2"
                     <<"x,-y+1/2,z+1/2"
                     <<"-x,y+1/2,z+1/2";
    spaceGroups[45] = sp;
    //-----------------------
    sp.name = "";
    sp.number = 47;
    sp.symOperations.clear();
    sp.symOperations
                     <<"x,y,z"
                     <<"-x,-y,z"
                     <<"-x,y,-z"
                     <<"x,-y,-z"
                     <<"-x,-y,-z"
                     <<"x,y,-z"
                     <<"x,-y,z"
                     <<"-x,y,z";
    spaceGroups[46] = sp;
    //-----------------------
    sp.name = "";
    sp.number = 48;
    sp.symOperations.clear();
    sp.symOperations
                     <<"x,y,z"
                     <<"-x+1/2,-y+1/2,z"
                     <<"-x+1/2,y,-z+1/2"
                     <<"x,-y+1/2,-z+1/2"
                     <<"-x,-y,-z"
                     <<"x+1/2,y+1/2,-z"
                     <<"x+1/2,-y,z+1/2"
                     <<"-x,y+1/2,z+1/2";
    spaceGroups[47] = sp;
    //-----------------------
    sp.name = "";
    sp.number = 49;
    sp.symOperations.clear();
    sp.symOperations
                     <<"x,y,z"
                     <<"-x,-y,z"
                     <<"-x,y,-z+1/2"
                     <<"x,-y,-z+1/2"
                     <<"-x,-y,-z"
                     <<"x,y,-z"
                     <<"x,-y,z+1/2"
                     <<"-x,y,z+1/2";
    spaceGroups[48] = sp;
    //-----------------------
    sp.name = "";
    sp.number = 50;
    sp.symOperations.clear();
    sp.symOperations
                     <<"x,y,z"
                     <<"-x+1/2,-y+1/2,z"
                     <<"-x+1/2,y,-z"
                     <<"x,-y+1/2,-z"
                     <<"-x,-y,-z"
                     <<"x+1/2,y+1/2,-z"
                     <<"x+1/2,-y,z"
                     <<"-x,y+1/2,z";
    spaceGroups[49] = sp;
    //-----------------------
    sp.name = "";
    sp.number = 51;
    sp.symOperations.clear();
    sp.symOperations
                     <<"x,y,z"
                     <<"-x+1/2,-y,z"
                     <<"-x,y,-z"
                     <<"x+1/2,-y,-z"
                     <<"-x,-y,-z"
                     <<"x+1/2,y,-z"
                     <<"x,-y,z"
                     <<"-x+1/2,y,z";
    spaceGroups[50] = sp;
    //-----------------------
    sp.name = "";
    sp.number = 52;
    sp.symOperations.clear();
    sp.symOperations
                     <<"x,y,z"
                     <<"-x+1/2,-y,z"
                     <<"-x+1/2,y+1/2,-z+1/2"
                     <<"x,-y+1/2,-z+1/2"
                     <<"-x,-y,-z"
                     <<"x+1/2,y,-z"
                     <<"x+1/2,-y+1/2,z+1/2"
                     <<"-x,y+1/2,z+1/2";
    spaceGroups[51] = sp;
    //-----------------------
    sp.name = "";
    sp.number = 53;
    sp.symOperations.clear();
    sp.symOperations
                     <<"x,y,z"
                     <<"-x+1/2,-y,z+1/2"
                     <<"-x+1/2,y,-z+1/2"
                     <<"x,-y,-z"
                     <<"-x,-y,-z"
                     <<"x+1/2,y,-z+1/2"
                     <<"x+1/2,-y,z+1/2"
                     <<"-x,y,z";
    spaceGroups[52] = sp;
    //-----------------------
    sp.name = "";
    sp.number = 54;
    sp.symOperations.clear();
    sp.symOperations
                     <<"x,y,z"
                     <<"-x+1/2,-y,z"
                     <<"-x,y,-z+1/2"
                     <<"x+1/2,-y,-z+1/2"
                     <<"-x,-y,-z"
                     <<"x+1/2,y,-z"
                     <<"x,-y,z+1/2"
                     <<"-x+1/2,y,z+1/2";
    spaceGroups[53] = sp;
    //-----------------------
    sp.name = "";
    sp.number = 55;
    sp.symOperations.clear();
    sp.symOperations
                     <<"x,y,z"
                     <<"-x,-y,z"
                     <<"-x+1/2,y+1/2,-z"
                     <<"x+1/2,-y+1/2,-z"
                     <<"-x,-y,-z"
                     <<"x,y,-z"
                     <<"x+1/2,-y+1/2,z"
                     <<"-x+1/2,y+1/2,z";
    spaceGroups[54] = sp;
    //-----------------------
    sp.name = "";
    sp.number = 56;
    sp.symOperations.clear();
    sp.symOperations
                     <<"x,y,z"
                     <<"-x+1/2,-y+1/2,z"
                     <<"-x,y+1/2,-z+1/2"
                     <<"x+1/2,-y,-z+1/2"
                     <<"-x,-y,-z"
                     <<"x+1/2,y+1/2,-z"
                     <<"x,-y+1/2,z+1/2"
                     <<"-x+1/2,y,z+1/2";
    spaceGroups[55] = sp;
    //-----------------------
    sp.name = "";
    sp.number = 57;
    sp.symOperations.clear();
    sp.symOperations
                     <<"x,y,z"
                     <<"-x,-y,z+1/2"
                     <<"-x,y+1/2,-z+1/2"
                     <<"x,-y+1/2,-z"
                     <<"-x,-y,-z"
                     <<"x,y,-z+1/2"
                     <<"x,-y+1/2,z+1/2"
                     <<"-x,y+1/2,z";
    spaceGroups[56] = sp;
    //-----------------------
    sp.name = "";
    sp.number = 58;
    sp.symOperations.clear();
    sp.symOperations
                     <<"x,y,z"
                     <<"-x,-y,z"
                     <<"-x+1/2,y+1/2,-z+1/2"
                     <<"x+1/2,-y+1/2,-z+1/2"
                     <<"-x,-y,-z"
                     <<"x,y,-z"
                     <<"x+1/2,-y+1/2,z+1/2"
                     <<"-x+1/2,y+1/2,z+1/2";
    spaceGroups[57] = sp;
    //-----------------------
    sp.name = "";
    sp.number = 59;
    sp.symOperations.clear();
    sp.symOperations
                     <<"x,y,z"
                     <<"-x+1/2,-y+1/2,z"
                     <<"-x,y+1/2,-z"
                     <<"x+1/2,-y,-z"
                     <<"-x,-y,-z"
                     <<"x+1/2,y+1/2,-z"
                     <<"x,-y+1/2,z"
                     <<"-x+1/2,y,z";
    spaceGroups[58] = sp;
    //-----------------------
    sp.name = "";
    sp.number = 60;
    sp.symOperations.clear();
    sp.symOperations
                     <<"x,y,z"
                     <<"-x+1/2,-y+1/2,z+1/2"
                     <<"-x,y,-z+1/2"
                     <<"x+1/2,-y+1/2,-z"
                     <<"-x,-y,-z"
                     <<"x+1/2,y+1/2,-z+1/2"
                     <<"x,-y,z+1/2"
                     <<"-x+1/2,y+1/2,z";
    spaceGroups[59] = sp;
    //-----------------------
    sp.name = "";
    sp.number = 61;
    sp.symOperations.clear();
    sp.symOperations
                     <<"x,y,z"
                     <<"-x+1/2,-y,z+1/2"
                     <<"-x,y+1/2,-z+1/2"
                     <<"x+1/2,-y+1/2,-z"
                     <<"-x,-y,-z"
                     <<"x+1/2,y,-z+1/2"
                     <<"x,-y+1/2,z+1/2"
                     <<"-x+1/2,y+1/2,z";
    spaceGroups[60] = sp;
    //-----------------------
    sp.name = "";
    sp.number = 62;
    sp.symOperations.clear();
    sp.symOperations
                     <<"x,y,z"
                     <<"-x+1/2,-y,z+1/2"
                     <<"-x,y+1/2,-z"
                     <<"x+1/2,-y+1/2,-z+1/2"
                     <<"-x,-y,-z"
                     <<"x+1/2,y,-z+1/2"
                     <<"x,-y+1/2,z"
                     <<"-x+1/2,y+1/2,z+1/2";
    spaceGroups[61] = sp;
    //-----------------------
    sp.name = "";
    sp.number = 63;
    sp.symOperations.clear();
    sp.symOperations
                     <<"x,y,z"
                     <<"-x,-y,z+1/2"
                     <<"-x,y,-z+1/2"
                     <<"x,-y,-z"
                     <<"-x,-y,-z"
                     <<"x,y,-z+1/2"
                     <<"x,-y,z+1/2"
                     <<"-x,y,z"
                     <<"x+1/2,y+1/2,z"
                     <<"-x+1/2,-y+1/2,z+1/2"
                     <<"-x+1/2,y+1/2,-z+1/2"
                     <<"x+1/2,-y+1/2,-z"
                     <<"-x+1/2,-y+1/2,-z"
                     <<"x+1/2,y+1/2,-z+1/2"
                     <<"x+1/2,-y+1/2,z+1/2"
                     <<"-x+1/2,y+1/2,z";
    spaceGroups[62] = sp;
    //-----------------------
    sp.name = "";
    sp.number = 64;
    sp.symOperations.clear();
    sp.symOperations
                     <<"x,y,z"
                     <<"-x,-y+1/2,z+1/2"
                     <<"-x,y+1/2,-z+1/2"
                     <<"x,-y,-z"
                     <<"-x,-y,-z"
                     <<"x,y+1/2,-z+1/2"
                     <<"x,-y+1/2,z+1/2"
                     <<"-x,y,z"
                     <<"x+1/2,y+1/2,z"
                     <<"-x+1/2,-y,z+1/2"
                     <<"-x+1/2,y,-z+1/2"
                     <<"x+1/2,-y+1/2,-z"
                     <<"-x+1/2,-y+1/2,-z"
                     <<"x+1/2,y,-z+1/2"
                     <<"x+1/2,-y,z+1/2"
                     <<"-x+1/2,y+1/2,z";
    spaceGroups[63] = sp;
    //-----------------------
    sp.name = "";
    sp.number = 65;
    sp.symOperations.clear();
    sp.symOperations
                     <<"x,y,z"
                     <<"-x,-y,z"
                     <<"-x,y,-z"
                     <<"x,-y,-z"
                     <<"-x,-y,-z"
                     <<"x,y,-z"
                     <<"x,-y,z"
                     <<"-x,y,z"
                     <<"x+1/2,y+1/2,z"
                     <<"-x+1/2,-y+1/2,z"
                     <<"-x+1/2,y+1/2,-z"
                     <<"x+1/2,-y+1/2,-z"
                     <<"-x+1/2,-y+1/2,-z"
                     <<"x+1/2,y+1/2,-z"
                     <<"x+1/2,-y+1/2,z"
                     <<"-x+1/2,y+1/2,z";
    spaceGroups[64] = sp;
    //-----------------------
    sp.name = "";
    sp.number = 66;
    sp.symOperations.clear();
    sp.symOperations
                     <<"x,y,z"
                     <<"-x,-y,z"
                     <<"-x,y,-z+1/2"
                     <<"x,-y,-z+1/2"
                     <<"-x,-y,-z"
                     <<"x,y,-z"
                     <<"x,-y,z+1/2"
                     <<"-x,y,z+1/2"
                     <<"x+1/2,y+1/2,z"
                     <<"-x+1/2,-y+1/2,z"
                     <<"-x+1/2,y+1/2,-z+1/2"
                     <<"x+1/2,-y+1/2,-z+1/2"
                     <<"-x+1/2,-y+1/2,-z"
                     <<"x+1/2,y+1/2,-z"
                     <<"x+1/2,-y+1/2,z+1/2"
                     <<"-x+1/2,y+1/2,z+1/2";
    spaceGroups[65] = sp;
    //-----------------------
    sp.name = "";
    sp.number = 67;
    sp.symOperations.clear();
    sp.symOperations
                     <<"x,y,z"
                     <<"-x,-y+1/2,z"
                     <<"-x,y+1/2,-z"
                     <<"x,-y,-z"
                     <<"-x,-y,-z"
                     <<"x,y+1/2,-z"
                     <<"x,-y+1/2,z"
                     <<"-x,y,z"
                     <<"x+1/2,y+1/2,z"
                     <<"-x+1/2,-y,z"
                     <<"-x+1/2,y,-z"
                     <<"x+1/2,-y+1/2,-z"
                     <<"-x+1/2,-y+1/2,-z"
                     <<"x+1/2,y,-z"
                     <<"x+1/2,-y,z"
                     <<"-x+1/2,y+1/2,z";
    spaceGroups[66] = sp;
    //-----------------------
    sp.name = "";
    sp.number = 68;
    sp.symOperations.clear();
    sp.symOperations
                     <<"x,y,z"
                     <<"-x+1/2,-y,z"
                     <<"-x,y,-z+1/2"
                     <<"x+1/2,-y,-z+1/2"
                     <<"-x,-y,-z"
                     <<"x+1/2,y,-z"
                     <<"x,-y,z+1/2"
                     <<"-x+1/2,y,z+1/2"
                     <<"x+1/2,y+1/2,z"
                     <<"-x,-y+1/2,z"
                     <<"-x+1/2,y+1/2,-z+1/2"
                     <<"x,-y+1/2,-z+1/2"
                     <<"-x+1/2,-y+1/2,-z"
                     <<"x,y+1/2,-z"
                     <<"x+1/2,-y+1/2,z+1/2"
                     <<"-x,y+1/2,z+1/2";
    spaceGroups[67] = sp;
    //-----------------------
    sp.name = "";
    sp.number = 69;
    sp.symOperations.clear();
    sp.symOperations
                     <<"x,y,z"
                     <<"-x,-y,z"
                     <<"-x,y,-z"
                     <<"x,-y,-z"
                     <<"-x,-y,-z"
                     <<"x,y,-z"
                     <<"x,-y,z"
                     <<"-x,y,z"
                     <<"x,y+1/2,z+1/2"
                     <<"-x,-y+1/2,z+1/2"
                     <<"-x,y+1/2,-z+1/2"
                     <<"x,-y+1/2,-z+1/2"
                     <<"-x,-y+1/2,-z+1/2"
                     <<"x,y+1/2,-z+1/2"
                     <<"x,-y+1/2,z+1/2"
                     <<"-x,y+1/2,z+1/2"
                     <<"x+1/2,y,z+1/2"
                     <<"-x+1/2,-y,z+1/2"
                     <<"-x+1/2,y,-z+1/2"
                     <<"x+1/2,-y,-z+1/2"
                     <<"-x+1/2,-y,-z+1/2"
                     <<"x+1/2,y,-z+1/2"
                     <<"x+1/2,-y,z+1/2"
                     <<"-x+1/2,y,z+1/2"
                     <<"x+1/2,y+1/2,z"
                     <<"-x+1/2,-y+1/2,z"
                     <<"-x+1/2,y+1/2,-z"
                     <<"x+1/2,-y+1/2,-z"
                     <<"-x+1/2,-y+1/2,-z"
                     <<"x+1/2,y+1/2,-z"
                     <<"x+1/2,-y+1/2,z"
                     <<"-x+1/2,y+1/2,z";
    spaceGroups[68] = sp;
    //-----------------------
    sp.name = "";
    sp.number = 70;
    sp.symOperations.clear();
    sp.symOperations
                     <<"x,y,z"
                     <<"-x+3/4,-y+3/4,z"
                     <<"-x+3/4,y,-z+3/4"
                     <<"x,-y+3/4,-z+3/4"
                     <<"-x,-y,-z"
                     <<"x+1/4,y+1/4,-z"
                     <<"x+1/4,-y,z+1/4"
                     <<"-x,y+1/4,z+1/4"
                     <<"x,y+1/2,z+1/2"
                     <<"-x+3/4,-y+1/4,z+1/2"
                     <<"-x+3/4,y+1/2,-z+1/4"
                     <<"x,-y+1/4,-z+1/4"
                     <<"-x,-y+1/2,-z+1/2"
                     <<"x+1/4,y+3/4,-z+1/2"
                     <<"x+1/4,-y+1/2,z+3/4"
                     <<"-x,y+3/4,z+3/4"
                     <<"x+1/2,y,z+1/2"
                     <<"-x+1/4,-y+3/4,z+1/2"
                     <<"-x+1/4,y,-z+1/4"
                     <<"x+1/2,-y+3/4,-z+1/4"
                     <<"-x+1/2,-y,-z+1/2"
                     <<"x+3/4,y+1/4,-z+1/2"
                     <<"x+3/4,-y,z+3/4"
                     <<"-x+1/2,y+1/4,z+3/4"
                     <<"x+1/2,y+1/2,z"
                     <<"-x+1/4,-y+1/4,z"
                     <<"-x+1/4,y+1/2,-z+3/4"
                     <<"x+1/2,-y+1/4,-z+3/4"
                     <<"-x+1/2,-y+1/2,-z"
                     <<"x+3/4,y+3/4,-z"
                     <<"x+3/4,-y+1/2,z+1/4"
                     <<"-x+1/2,y+3/4,z+1/4";
    spaceGroups[69] = sp;
    //-----------------------
    sp.name = "";
    sp.number = 71;
    sp.symOperations.clear();
    sp.symOperations
                     <<"x,y,z"
                     <<"-x,-y,z"
                     <<"-x,y,-z"
                     <<"x,-y,-z"
                     <<"-x,-y,-z"
                     <<"x,y,-z"
                     <<"x,-y,z"
                     <<"-x,y,z"
                     <<"x+1/2,y+1/2,z+1/2"
                     <<"-x+1/2,-y+1/2,z+1/2"
                     <<"-x+1/2,y+1/2,-z+1/2"
                     <<"x+1/2,-y+1/2,-z+1/2"
                     <<"-x+1/2,-y+1/2,-z+1/2"
                     <<"x+1/2,y+1/2,-z+1/2"
                     <<"x+1/2,-y+1/2,z+1/2"
                     <<"-x+1/2,y+1/2,z+1/2";
    spaceGroups[70] = sp;
    //-----------------------
    sp.name = "";
    sp.number = 72;
    sp.symOperations.clear();
    sp.symOperations
                     <<"x,y,z"
                     <<"-x,-y,z"
                     <<"-x+1/2,y+1/2,-z"
                     <<"x+1/2,-y+1/2,-z"
                     <<"-x,-y,-z"
                     <<"x,y,-z"
                     <<"x+1/2,-y+1/2,z"
                     <<"-x+1/2,y+1/2,z"
                     <<"x+1/2,y+1/2,z+1/2"
                     <<"-x+1/2,-y+1/2,z+1/2"
                     <<"-x,y,-z+1/2"
                     <<"x,-y,-z+1/2"
                     <<"-x+1/2,-y+1/2,-z+1/2"
                     <<"x+1/2,y+1/2,-z+1/2"
                     <<"x,-y,z+1/2"
                     <<"-x,y,z+1/2";
    spaceGroups[71] = sp;
    //-----------------------
    sp.name = "";
    sp.number = 73;
    sp.symOperations.clear();
    sp.symOperations
                     <<"x,y,z"
                     <<"-x+1/2,-y,z+1/2"
                     <<"-x,y+1/2,-z+1/2"
                     <<"x+1/2,-y+1/2,-z"
                     <<"-x,-y,-z"
                     <<"x+1/2,y,-z+1/2"
                     <<"x,-y+1/2,z+1/2"
                     <<"-x+1/2,y+1/2,z"
                     <<"x+1/2,y+1/2,z+1/2"
                     <<"-x,-y+1/2,z"
                     <<"-x+1/2,y,-z"
                     <<"x,-y,-z+1/2"
                     <<"-x+1/2,-y+1/2,-z+1/2"
                     <<"x,y+1/2,-z"
                     <<"x+1/2,-y,z"
                     <<"-x,y,z+1/2";
    spaceGroups[72] = sp;
    //-----------------------
    sp.name = "";
    sp.number = 74;
    sp.symOperations.clear();
    sp.symOperations
                     <<"x,y,z"
                     <<"-x,-y+1/2,z"
                     <<"-x,y+1/2,-z"
                     <<"x,-y,-z"
                     <<"-x,-y,-z"
                     <<"x,y+1/2,-z"
                     <<"x,-y+1/2,z"
                     <<"-x,y,z"
                     <<"x+1/2,y+1/2,z+1/2"
                     <<"-x+1/2,-y,z+1/2"
                     <<"-x+1/2,y,-z+1/2"
                     <<"x+1/2,-y+1/2,-z+1/2"
                     <<"-x+1/2,-y+1/2,-z+1/2"
                     <<"x+1/2,y,-z+1/2"
                     <<"x+1/2,-y,z+1/2"
                     <<"-x+1/2,y+1/2,z+1/2";
    spaceGroups[73] = sp;
    //-----------------------
    sp.name = "";
    sp.number = 75;
    sp.symOperations.clear();
    sp.symOperations
                     <<"x,y,z"
                     <<"-x,-y,z"
                     <<"-y,x,z"
                     <<"y,-x,z";
    spaceGroups[74] = sp;
    //-----------------------
    sp.name = "";
    sp.number = 76;
    sp.symOperations.clear();
    sp.symOperations
                     <<"x,y,z"
                     <<"-x,-y,z+1/2"
                     <<"-y,x,z+1/4"
                     <<"y,-x,z+3/4";
    spaceGroups[75] = sp;
    //-----------------------
    sp.name = "";
    sp.number = 77;
    sp.symOperations.clear();
    sp.symOperations
                     <<"x,y,z"
                     <<"-x,-y,z"
                     <<"-y,x,z+1/2"
                     <<"y,-x,z+1/2";
    spaceGroups[76] = sp;
    //-----------------------
    sp.name = "";
    sp.number = 78;
    sp.symOperations.clear();
    sp.symOperations
                     <<"x,y,z"
                     <<"-x,-y,z+1/2"
                     <<"-y,x,z+3/4"
                     <<"y,-x,z+1/4";
    spaceGroups[77] = sp;
    //-----------------------
    sp.name = "";
    sp.number = 79;
    sp.symOperations.clear();
    sp.symOperations
                     <<"x,y,z"
                     <<"-x,-y,z"
                     <<"-y,x,z"
                     <<"y,-x,z"
                     <<"x+1/2,y+1/2,z+1/2"
                     <<"-x+1/2,-y+1/2,z+1/2"
                     <<"-y+1/2,x+1/2,z+1/2"
                     <<"y+1/2,-x+1/2,z+1/2";
    spaceGroups[78] = sp;
    //-----------------------
    sp.name = "";
    sp.number = 80;
    sp.symOperations.clear();
    sp.symOperations
                     <<"x,y,z"
                     <<"-x+1/2,-y+1/2,z+1/2"
                     <<"-y,x+1/2,z+1/4"
                     <<"y+1/2,-x,z+3/4"
                     <<"x+1/2,y+1/2,z+1/2"
                     <<"-x,-y,z"
                     <<"-y+1/2,x,z+3/4"
                     <<"y,-x+1/2,z+1/4";
    spaceGroups[79] = sp;
    //-----------------------
    sp.name = "";
    sp.number = 81;
    sp.symOperations.clear();
    sp.symOperations
                     <<"x,y,z"
                     <<"-x,-y,z"
                     <<"y,-x,-z"
                     <<"-y,x,-z";
    spaceGroups[80] = sp;
    //-----------------------
    sp.name = "";
    sp.number = 82;
    sp.symOperations.clear();
    sp.symOperations
                     <<"x,y,z"
                     <<"-x,-y,z"
                     <<"y,-x,-z"
                     <<"-y,x,-z"
                     <<"x+1/2,y+1/2,z+1/2"
                     <<"-x+1/2,-y+1/2,z+1/2"
                     <<"y+1/2,-x+1/2,-z+1/2"
                     <<"-y+1/2,x+1/2,-z+1/2";
    spaceGroups[81] = sp;
    //-----------------------
    sp.name = "";
    sp.number = 83;
    sp.symOperations.clear();
    sp.symOperations
                     <<"x,y,z"
                     <<"-x,-y,z"
                     <<"-y,x,z"
                     <<"y,-x,z"
                     <<"-x,-y,-z"
                     <<"x,y,-z"
                     <<"y,-x,-z"
                     <<"-y,x,-z";
    spaceGroups[82] = sp;
    //-----------------------
    sp.name = "";
    sp.number = 84;
    sp.symOperations.clear();
    sp.symOperations
                     <<"x,y,z"
                     <<"-x,-y,z"
                     <<"-y,x,z+1/2"
                     <<"y,-x,z+1/2"
                     <<"-x,-y,-z"
                     <<"x,y,-z"
                     <<"y,-x,-z+1/2"
                     <<"-y,x,-z+1/2";
    spaceGroups[83] = sp;
    //-----------------------
    sp.name = "";
    sp.number = 85;
    sp.symOperations.clear();
    sp.symOperations
                     <<"x,y,z"
                     <<"-x+1/2,-y+1/2,z"
                     <<"-y+1/2,x,z"
                     <<"y,-x+1/2,z"
                     <<"-x,-y,-z"
                     <<"x+1/2,y+1/2,-z"
                     <<"y+1/2,-x,-z"
                     <<"-y,x+1/2,-z";
    spaceGroups[84] = sp;
    //-----------------------
    sp.name = "";
    sp.number = 86;
    sp.symOperations.clear();
    sp.symOperations
                     <<"x,y,z"
                     <<"-x+1/2,-y+1/2,z"
                     <<"-y,x+1/2,z+1/2"
                     <<"y+1/2,-x,z+1/2"
                     <<"-x,-y,-z"
                     <<"x+1/2,y+1/2,-z"
                     <<"y,-x+1/2,-z+1/2"
                     <<"-y+1/2,x,-z+1/2";
    spaceGroups[85] = sp;
    //-----------------------
    sp.name = "";
    sp.number = 87;
    sp.symOperations.clear();
    sp.symOperations
                     <<"x,y,z"
                     <<"-x,-y,z"
                     <<"-y,x,z"
                     <<"y,-x,z"
                     <<"-x,-y,-z"
                     <<"x,y,-z"
                     <<"y,-x,-z"
                     <<"-y,x,-z"
                     <<"x+1/2,y+1/2,z+1/2"
                     <<"-x+1/2,-y+1/2,z+1/2"
                     <<"-y+1/2,x+1/2,z+1/2"
                     <<"y+1/2,-x+1/2,z+1/2"
                     <<"-x+1/2,-y+1/2,-z+1/2"
                     <<"x+1/2,y+1/2,-z+1/2"
                     <<"y+1/2,-x+1/2,-z+1/2"
                     <<"-y+1/2,x+1/2,-z+1/2";
    spaceGroups[86] = sp;
    //-----------------------
    sp.name = "";
    sp.number = 88;
    sp.symOperations.clear();
    sp.symOperations
                     <<"x,y,z"
                     <<"-x+1/2,-y,z+1/2"
                     <<"-y+3/4,x+1/4,z+1/4"
                     <<"y+3/4,-x+3/4,z+3/4"
                     <<"-x,-y,-z"
                     <<"x+1/2,y,-z+1/2"
                     <<"y+1/4,-x+3/4,-z+3/4"
                     <<"-y+1/4,x+1/4,-z+1/4"
                     <<"x+1/2,y+1/2,z+1/2"
                     <<"-x,-y+1/2,z"
                     <<"-y+1/4,x+3/4,z+3/4"
                     <<"y+1/4,-x+1/4,z+1/4"
                     <<"-x+1/2,-y+1/2,-z+1/2"
                     <<"x,y+1/2,-z"
                     <<"y+3/4,-x+1/4,-z+1/4"
                     <<"-y+3/4,x+3/4,-z+3/4";
    spaceGroups[87] = sp;
    //-----------------------
    sp.name = "";
    sp.number = 89;
    sp.symOperations.clear();
    sp.symOperations
                     <<"x,y,z"
                     <<"-x,-y,z"
                     <<"-y,x,z"
                     <<"y,-x,z"
                     <<"-x,y,-z"
                     <<"x,-y,-z"
                     <<"y,x,-z"
                     <<"-y,-x,-z";
    spaceGroups[88] = sp;
    //-----------------------
    sp.name = "";
    sp.number = 90;
    sp.symOperations.clear();
    sp.symOperations
                     <<"x,y,z"
                     <<"-x,-y,z"
                     <<"-y+1/2,x+1/2,z"
                     <<"y+1/2,-x+1/2,z"
                     <<"-x+1/2,y+1/2,-z"
                     <<"x+1/2,-y+1/2,-z"
                     <<"y,x,-z"
                     <<"-y,-x,-z";
    spaceGroups[89] = sp;
    //-----------------------
    sp.name = "";
    sp.number = 91;
    sp.symOperations.clear();
    sp.symOperations
                     <<"x,y,z"
                     <<"-x,-y,z+1/2"
                     <<"-y,x,z+1/4"
                     <<"y,-x,z+3/4"
                     <<"-x,y,-z"
                     <<"x,-y,-z+1/2"
                     <<"y,x,-z+3/4"
                     <<"-y,-x,-z+1/4";
    spaceGroups[90] = sp;
    //-----------------------
    sp.name = "";
    sp.number = 92;
    sp.symOperations.clear();
    sp.symOperations
                     <<"x,y,z"
                     <<"-x,-y,z+1/2"
                     <<"-y+1/2,x+1/2,z+1/4"
                     <<"y+1/2,-x+1/2,z+3/4"
                     <<"-x+1/2,y+1/2,-z+1/4"
                     <<"x+1/2,-y+1/2,-z+3/4"
                     <<"y,x,-z"
                     <<"-y,-x,-z+1/2";
    spaceGroups[91] = sp;
    //-----------------------
    sp.name = "";
    sp.number = 93;
    sp.symOperations.clear();
    sp.symOperations
                     <<"x,y,z"
                     <<"-x,-y,z"
                     <<"-y,x,z+1/2"
                     <<"y,-x,z+1/2"
                     <<"-x,y,-z"
                     <<"x,-y,-z"
                     <<"y,x,-z+1/2"
                     <<"-y,-x,-z+1/2";
    spaceGroups[92] = sp;
    //-----------------------
    sp.name = "";
    sp.number = 94;
    sp.symOperations.clear();
    sp.symOperations
                     <<"x,y,z"
                     <<"-x,-y,z"
                     <<"-y+1/2,x+1/2,z+1/2"
                     <<"y+1/2,-x+1/2,z+1/2"
                     <<"-x+1/2,y+1/2,-z+1/2"
                     <<"x+1/2,-y+1/2,-z+1/2"
                     <<"y,x,-z"
                     <<"-y,-x,-z";
    spaceGroups[93] = sp;
    //-----------------------
    sp.name = "";
    sp.number = 95;
    sp.symOperations.clear();
    sp.symOperations
                     <<"x,y,z"
                     <<"-x,-y,z+1/2"
                     <<"-y,x,z+3/4"
                     <<"y,-x,z+1/4"
                     <<"-x,y,-z"
                     <<"x,-y,-z+1/2"
                     <<"y,x,-z+1/4"
                     <<"-y,-x,-z+3/4";
    spaceGroups[94] = sp;
    //-----------------------
    sp.name = "";
    sp.number = 96;
    sp.symOperations.clear();
    sp.symOperations
                     <<"x,y,z"
                     <<"-x,-y,z+1/2"
                     <<"-y+1/2,x+1/2,z+3/4"
                     <<"y+1/2,-x+1/2,z+1/4"
                     <<"-x+1/2,y+1/2,-z+3/4"
                     <<"x+1/2,-y+1/2,-z+1/4"
                     <<"y,x,-z"
                     <<"-y,-x,-z+1/2";
    spaceGroups[95] = sp;
    //-----------------------
    sp.name = "";
    sp.number = 97;
    sp.symOperations.clear();
    sp.symOperations
                     <<"x,y,z"
                     <<"-x,-y,z"
                     <<"-y,x,z"
                     <<"y,-x,z"
                     <<"-x,y,-z"
                     <<"x,-y,-z"
                     <<"y,x,-z"
                     <<"-y,-x,-z"
                     <<"x+1/2,y+1/2,z+1/2"
                     <<"-x+1/2,-y+1/2,z+1/2"
                     <<"-y+1/2,x+1/2,z+1/2"
                     <<"y+1/2,-x+1/2,z+1/2"
                     <<"-x+1/2,y+1/2,-z+1/2"
                     <<"x+1/2,-y+1/2,-z+1/2"
                     <<"y+1/2,x+1/2,-z+1/2"
                     <<"-y+1/2,-x+1/2,-z+1/2";
    spaceGroups[96] = sp;
    //-----------------------
    sp.name = "";
    sp.number = 98;
    sp.symOperations.clear();
    sp.symOperations
                     <<"x,y,z"
                     <<"-x+1/2,-y+1/2,z+1/2"
                     <<"-y,x+1/2,z+1/4"
                     <<"y+1/2,-x,z+3/4"
                     <<"-x+1/2,y,-z+3/4"
                     <<"x,-y+1/2,-z+1/4"
                     <<"y+1/2,x+1/2,-z+1/2"
                     <<"-y,-x,-z"
                     <<"x+1/2,y+1/2,z+1/2"
                     <<"-x,-y,z"
                     <<"-y+1/2,x,z+3/4"
                     <<"y,-x+1/2,z+1/4"
                     <<"-x,y+1/2,-z+1/4"
                     <<"x+1/2,-y,-z+3/4"
                     <<"y,x,-z"
                     <<"-y+1/2,-x+1/2,-z+1/2";
    spaceGroups[97] = sp;
    //-----------------------
    sp.name = "";
    sp.number = 99;
    sp.symOperations.clear();
    sp.symOperations
                     <<"x,y,z"
                     <<"-x,-y,z"
                     <<"-y,x,z"
                     <<"y,-x,z"
                     <<"x,-y,z"
                     <<"-x,y,z"
                     <<"-y,-x,z"
                     <<"y,x,z";
    spaceGroups[98] = sp;
    //-----------------------
    sp.name = "";
    sp.number = 100;
    sp.symOperations.clear();
    sp.symOperations
                     <<"x,y,z"
                     <<"-x,-y,z"
                     <<"-y,x,z"
                     <<"y,-x,z"
                     <<"x+1/2,-y+1/2,z"
                     <<"-x+1/2,y+1/2,z"
                     <<"-y+1/2,-x+1/2,z"
                     <<"y+1/2,x+1/2,z";
    spaceGroups[99] = sp;
    //-----------------------
    sp.name = "";
    sp.number = 101;
    sp.symOperations.clear();
    sp.symOperations
                     <<"x,y,z"
                     <<"-x,-y,z"
                     <<"-y,x,z+1/2"
                     <<"y,-x,z+1/2"
                     <<"x,-y,z+1/2"
                     <<"-x,y,z+1/2"
                     <<"-y,-x,z"
                     <<"y,x,z";
    spaceGroups[100] = sp;
    //-----------------------
    sp.name = "";
    sp.number = 102;
    sp.symOperations.clear();
    sp.symOperations
                     <<"x,y,z"
                     <<"-x,-y,z"
                     <<"-y+1/2,x+1/2,z+1/2"
                     <<"y+1/2,-x+1/2,z+1/2"
                     <<"x+1/2,-y+1/2,z+1/2"
                     <<"-x+1/2,y+1/2,z+1/2"
                     <<"-y,-x,z"
                     <<"y,x,z";
    spaceGroups[101] = sp;
    //-----------------------
    sp.name = "";
    sp.number = 103;
    sp.symOperations.clear();
    sp.symOperations
                     <<"x,y,z"
                     <<"-x,-y,z"
                     <<"-y,x,z"
                     <<"y,-x,z"
                     <<"x,-y,z+1/2"
                     <<"-x,y,z+1/2"
                     <<"-y,-x,z+1/2"
                     <<"y,x,z+1/2";
    spaceGroups[102] = sp;
    //-----------------------
    sp.name = "";
    sp.number = 104;
    sp.symOperations.clear();
    sp.symOperations
                     <<"x,y,z"
                     <<"-x,-y,z"
                     <<"-y,x,z"
                     <<"y,-x,z"
                     <<"x+1/2,-y+1/2,z+1/2"
                     <<"-x+1/2,y+1/2,z+1/2"
                     <<"-y+1/2,-x+1/2,z+1/2"
                     <<"y+1/2,x+1/2,z+1/2";
    spaceGroups[103] = sp;
    //-----------------------
    sp.name = "";
    sp.number = 105;
    sp.symOperations.clear();
    sp.symOperations
                     <<"x,y,z"
                     <<"-x,-y,z"
                     <<"-y,x,z+1/2"
                     <<"y,-x,z+1/2"
                     <<"x,-y,z"
                     <<"-x,y,z"
                     <<"-y,-x,z+1/2"
                     <<"y,x,z+1/2";
    spaceGroups[104] = sp;
    //-----------------------
    sp.name = "";
    sp.number = 106;
    sp.symOperations.clear();
    sp.symOperations
                     <<"x,y,z"
                     <<"-x,-y,z"
                     <<"-y,x,z+1/2"
                     <<"y,-x,z+1/2"
                     <<"x+1/2,-y+1/2,z"
                     <<"-x+1/2,y+1/2,z"
                     <<"-y+1/2,-x+1/2,z+1/2"
                     <<"y+1/2,x+1/2,z+1/2";
    spaceGroups[105] = sp;
    //-----------------------
    sp.name = "";
    sp.number = 107;
    sp.symOperations.clear();
    sp.symOperations
                     <<"x,y,z"
                     <<"-x,-y,z"
                     <<"-y,x,z"
                     <<"y,-x,z"
                     <<"x,-y,z"
                     <<"-x,y,z"
                     <<"-y,-x,z"
                     <<"y,x,z"
                     <<"x+1/2,y+1/2,z+1/2"
                     <<"-x+1/2,-y+1/2,z+1/2"
                     <<"-y+1/2,x+1/2,z+1/2"
                     <<"y+1/2,-x+1/2,z+1/2"
                     <<"x+1/2,-y+1/2,z+1/2"
                     <<"-x+1/2,y+1/2,z+1/2"
                     <<"-y+1/2,-x+1/2,z+1/2"
                     <<"y+1/2,x+1/2,z+1/2";
    spaceGroups[106] = sp;
    //-----------------------
    sp.name = "";
    sp.number = 108;
    sp.symOperations.clear();
    sp.symOperations
                     <<"x,y,z"
                     <<"-x,-y,z"
                     <<"-y,x,z"
                     <<"y,-x,z"
                     <<"x,-y,z+1/2"
                     <<"-x,y,z+1/2"
                     <<"-y,-x,z+1/2"
                     <<"y,x,z+1/2"
                     <<"x+1/2,y+1/2,z+1/2"
                     <<"-x+1/2,-y+1/2,z+1/2"
                     <<"-y+1/2,x+1/2,z+1/2"
                     <<"y+1/2,-x+1/2,z+1/2"
                     <<"x+1/2,-y+1/2,z"
                     <<"-x+1/2,y+1/2,z"
                     <<"-y+1/2,-x+1/2,z"
                     <<"y+1/2,x+1/2,z";
    spaceGroups[107] = sp;
    //-----------------------
    sp.name = "";
    sp.number = 109;
    sp.symOperations.clear();
    sp.symOperations
                     <<"x,y,z"
                     <<"-x+1/2,-y+1/2,z+1/2"
                     <<"-y,x+1/2,z+1/4"
                     <<"y+1/2,-x,z+3/4"
                     <<"x,-y,z"
                     <<"-x+1/2,y+1/2,z+1/2"
                     <<"-y,-x+1/2,z+1/4"
                     <<"y+1/2,x,z+3/4"
                     <<"x+1/2,y+1/2,z+1/2"
                     <<"-x,-y,z"
                     <<"-y+1/2,x,z+3/4"
                     <<"y,-x+1/2,z+1/4"
                     <<"x+1/2,-y+1/2,z+1/2"
                     <<"-x,y,z"
                     <<"-y+1/2,-x,z+3/4"
                     <<"y,x+1/2,z+1/4";
    spaceGroups[108] = sp;
    //-----------------------
    sp.name = "";
    sp.number = 110;
    sp.symOperations.clear();
    sp.symOperations
                     <<"x,y,z"
                     <<"-x+1/2,-y+1/2,z+1/2"
                     <<"-y,x+1/2,z+1/4"
                     <<"y+1/2,-x,z+3/4"
                     <<"x,-y,z+1/2"
                     <<"-x+1/2,y+1/2,z"
                     <<"-y,-x+1/2,z+3/4"
                     <<"y+1/2,x,z+1/4"
                     <<"x+1/2,y+1/2,z+1/2"
                     <<"-x,-y,z"
                     <<"-y+1/2,x,z+3/4"
                     <<"y,-x+1/2,z+1/4"
                     <<"x+1/2,-y+1/2,z"
                     <<"-x,y,z+1/2"
                     <<"-y+1/2,-x,z+1/4"
                     <<"y,x+1/2,z+3/4";
    spaceGroups[109] = sp;
    //-----------------------
    sp.name = "";
    sp.number = 111;
    sp.symOperations.clear();
    sp.symOperations
                     <<"x,y,z"
                     <<"-x,-y,z"
                     <<"y,-x,-z"
                     <<"-y,x,-z"
                     <<"-x,y,-z"
                     <<"x,-y,-z"
                     <<"-y,-x,z"
                     <<"y,x,z";
    spaceGroups[110] = sp;
    //-----------------------
    sp.name = "";
    sp.number = 112;
    sp.symOperations.clear();
    sp.symOperations
                     <<"x,y,z"
                     <<"-x,-y,z"
                     <<"y,-x,-z"
                     <<"-y,x,-z"
                     <<"-x,y,-z+1/2"
                     <<"x,-y,-z+1/2"
                     <<"-y,-x,z+1/2"
                     <<"y,x,z+1/2";
    spaceGroups[111] = sp;
    //-----------------------
    sp.name = "";
    sp.number = 113;
    sp.symOperations.clear();
    sp.symOperations
                     <<"x,y,z"
                     <<"-x,-y,z"
                     <<"y,-x,-z"
                     <<"-y,x,-z"
                     <<"-x+1/2,y+1/2,-z"
                     <<"x+1/2,-y+1/2,-z"
                     <<"-y+1/2,-x+1/2,z"
                     <<"y+1/2,x+1/2,z";
    spaceGroups[112] = sp;
    //-----------------------
    sp.name = "";
    sp.number = 114;
    sp.symOperations.clear();
    sp.symOperations
                     <<"x,y,z"
                     <<"-x,-y,z"
                     <<"y,-x,-z"
                     <<"-y,x,-z"
                     <<"-x+1/2,y+1/2,-z+1/2"
                     <<"x+1/2,-y+1/2,-z+1/2"
                     <<"-y+1/2,-x+1/2,z+1/2"
                     <<"y+1/2,x+1/2,z+1/2";
    spaceGroups[113] = sp;
    //-----------------------
    sp.name = "";
    sp.number = 115;
    sp.symOperations.clear();
    sp.symOperations
                     <<"x,y,z"
                     <<"-x,-y,z"
                     <<"y,-x,-z"
                     <<"-y,x,-z"
                     <<"x,-y,z"
                     <<"-x,y,z"
                     <<"y,x,-z"
                     <<"-y,-x,-z";
    spaceGroups[114] = sp;
    //-----------------------
    sp.name = "";
    sp.number = 116;
    sp.symOperations.clear();
    sp.symOperations
                     <<"x,y,z"
                     <<"-x,-y,z"
                     <<"y,-x,-z"
                     <<"-y,x,-z"
                     <<"x,-y,z+1/2"
                     <<"-x,y,z+1/2"
                     <<"y,x,-z+1/2"
                     <<"-y,-x,-z+1/2";
    spaceGroups[115] = sp;
    //-----------------------
    sp.name = "";
    sp.number = 117;
    sp.symOperations.clear();
    sp.symOperations
                     <<"x,y,z"
                     <<"-x,-y,z"
                     <<"y,-x,-z"
                     <<"-y,x,-z"
                     <<"x+1/2,-y+1/2,z"
                     <<"-x+1/2,y+1/2,z"
                     <<"y+1/2,x+1/2,-z"
                     <<"-y+1/2,-x+1/2,-z";
    spaceGroups[116] = sp;
    //-----------------------
    sp.name = "";
    sp.number = 118;
    sp.symOperations.clear();
    sp.symOperations
                     <<"x,y,z"
                     <<"-x,-y,z"
                     <<"y,-x,-z"
                     <<"-y,x,-z"
                     <<"x+1/2,-y+1/2,z+1/2"
                     <<"-x+1/2,y+1/2,z+1/2"
                     <<"y+1/2,x+1/2,-z+1/2"
                     <<"-y+1/2,-x+1/2,-z+1/2";
    spaceGroups[117] = sp;
    //-----------------------
    sp.name = "";
    sp.number = 119;
    sp.symOperations.clear();
    sp.symOperations
                     <<"x,y,z"
                     <<"-x,-y,z"
                     <<"y,-x,-z"
                     <<"-y,x,-z"
                     <<"x,-y,z"
                     <<"-x,y,z"
                     <<"y,x,-z"
                     <<"-y,-x,-z"
                     <<"x+1/2,y+1/2,z+1/2"
                     <<"-x+1/2,-y+1/2,z+1/2"
                     <<"y+1/2,-x+1/2,-z+1/2"
                     <<"-y+1/2,x+1/2,-z+1/2"
                     <<"x+1/2,-y+1/2,z+1/2"
                     <<"-x+1/2,y+1/2,z+1/2"
                     <<"y+1/2,x+1/2,-z+1/2"
                     <<"-y+1/2,-x+1/2,-z+1/2";
    spaceGroups[118] = sp;
    //-----------------------
    sp.name = "";
    sp.number = 120;
    sp.symOperations.clear();
    sp.symOperations
                     <<"x,y,z"
                     <<"-x,-y,z"
                     <<"y,-x,-z"
                     <<"-y,x,-z"
                     <<"x,-y,z+1/2"
                     <<"-x,y,z+1/2"
                     <<"y,x,-z+1/2"
                     <<"-y,-x,-z+1/2"
                     <<"x+1/2,y+1/2,z+1/2"
                     <<"-x+1/2,-y+1/2,z+1/2"
                     <<"y+1/2,-x+1/2,-z+1/2"
                     <<"-y+1/2,x+1/2,-z+1/2"
                     <<"x+1/2,-y+1/2,z"
                     <<"-x+1/2,y+1/2,z"
                     <<"y+1/2,x+1/2,-z"
                     <<"-y+1/2,-x+1/2,-z";
    spaceGroups[119] = sp;
    //-----------------------
    sp.name = "";
    sp.number = 121;
    sp.symOperations.clear();
    sp.symOperations
                     <<"x,y,z"
                     <<"-x,-y,z"
                     <<"y,-x,-z"
                     <<"-y,x,-z"
                     <<"-x,y,-z"
                     <<"x,-y,-z"
                     <<"-y,-x,z"
                     <<"y,x,z"
                     <<"x+1/2,y+1/2,z+1/2"
                     <<"-x+1/2,-y+1/2,z+1/2"
                     <<"y+1/2,-x+1/2,-z+1/2"
                     <<"-y+1/2,x+1/2,-z+1/2"
                     <<"-x+1/2,y+1/2,-z+1/2"
                     <<"x+1/2,-y+1/2,-z+1/2"
                     <<"-y+1/2,-x+1/2,z+1/2"
                     <<"y+1/2,x+1/2,z+1/2";
    spaceGroups[120] = sp;
    //-----------------------
    sp.name = "";
    sp.number = 122;
    sp.symOperations.clear();
    sp.symOperations
                     <<"x,y,z"
                     <<"-x,-y,z"
                     <<"y,-x,-z"
                     <<"-y,x,-z"
                     <<"-x+1/2,y,-z+3/4"
                     <<"x+1/2,-y,-z+3/4"
                     <<"-y+1/2,-x,z+3/4"
                     <<"y+1/2,x,z+3/4"
                     <<"x+1/2,y+1/2,z+1/2"
                     <<"-x+1/2,-y+1/2,z+1/2"
                     <<"y+1/2,-x+1/2,-z+1/2"
                     <<"-y+1/2,x+1/2,-z+1/2"
                     <<"-x,y+1/2,-z+1/4"
                     <<"x,-y+1/2,-z+1/4"
                     <<"-y,-x+1/2,z+1/4"
                     <<"y,x+1/2,z+1/4";
    spaceGroups[121] = sp;
    //-----------------------
    sp.name = "";
    sp.number = 123;
    sp.symOperations.clear();
    sp.symOperations
                     <<"x,y,z"
                     <<"-x,-y,z"
                     <<"-y,x,z"
                     <<"y,-x,z"
                     <<"-x,y,-z"
                     <<"x,-y,-z"
                     <<"y,x,-z"
                     <<"-y,-x,-z"
                     <<"-x,-y,-z"
                     <<"x,y,-z"
                     <<"y,-x,-z"
                     <<"-y,x,-z"
                     <<"x,-y,z"
                     <<"-x,y,z"
                     <<"-y,-x,z"
                     <<"y,x,z";
    spaceGroups[122] = sp;
    //-----------------------
    sp.name = "";
    sp.number = 124;
    sp.symOperations.clear();
    sp.symOperations
                     <<"x,y,z"
                     <<"-x,-y,z"
                     <<"-y,x,z"
                     <<"y,-x,z"
                     <<"-x,y,-z+1/2"
                     <<"x,-y,-z+1/2"
                     <<"y,x,-z+1/2"
                     <<"-y,-x,-z+1/2"
                     <<"-x,-y,-z"
                     <<"x,y,-z"
                     <<"y,-x,-z"
                     <<"-y,x,-z"
                     <<"x,-y,z+1/2"
                     <<"-x,y,z+1/2"
                     <<"-y,-x,z+1/2"
                     <<"y,x,z+1/2";
    spaceGroups[123] = sp;
    //-----------------------
    sp.name = "";
    sp.number = 125;
    sp.symOperations.clear();
    sp.symOperations
                     <<"x,y,z"
                     <<"-x+1/2,-y+1/2,z"
                     <<"-y+1/2,x,z"
                     <<"y,-x+1/2,z"
                     <<"-x+1/2,y,-z"
                     <<"x,-y+1/2,-z"
                     <<"y,x,-z"
                     <<"-y+1/2,-x+1/2,-z"
                     <<"-x,-y,-z"
                     <<"x+1/2,y+1/2,-z"
                     <<"y+1/2,-x,-z"
                     <<"-y,x+1/2,-z"
                     <<"x+1/2,-y,z"
                     <<"-x,y+1/2,z"
                     <<"-y,-x,z"
                     <<"y+1/2,x+1/2,z";
    spaceGroups[124] = sp;
    //-----------------------
    sp.name = "";
    sp.number = 126;
    sp.symOperations.clear();
    sp.symOperations
                     <<"x,y,z"
                     <<"-x+1/2,-y+1/2,z"
                     <<"-y+1/2,x,z"
                     <<"y,-x+1/2,z"
                     <<"-x+1/2,y,-z+1/2"
                     <<"x,-y+1/2,-z+1/2"
                     <<"y,x,-z+1/2"
                     <<"-y+1/2,-x+1/2,-z+1/2"
                     <<"-x,-y,-z"
                     <<"x+1/2,y+1/2,-z"
                     <<"y+1/2,-x,-z"
                     <<"-y,x+1/2,-z"
                     <<"x+1/2,-y,z+1/2"
                     <<"-x,y+1/2,z+1/2"
                     <<"-y,-x,z+1/2"
                     <<"y+1/2,x+1/2,z+1/2";
    spaceGroups[125] = sp;
    //-----------------------
    sp.name = "";
    sp.number = 127;
    sp.symOperations.clear();
    sp.symOperations
                     <<"x,y,z"
                     <<"-x,-y,z"
                     <<"-y,x,z"
                     <<"y,-x,z"
                     <<"-x+1/2,y+1/2,-z"
                     <<"x+1/2,-y+1/2,-z"
                     <<"y+1/2,x+1/2,-z"
                     <<"-y+1/2,-x+1/2,-z"
                     <<"-x,-y,-z"
                     <<"x,y,-z"
                     <<"y,-x,-z"
                     <<"-y,x,-z"
                     <<"x+1/2,-y+1/2,z"
                     <<"-x+1/2,y+1/2,z"
                     <<"-y+1/2,-x+1/2,z"
                     <<"y+1/2,x+1/2,z";
    spaceGroups[126] = sp;
    //-----------------------
    sp.name = "";
    sp.number = 128;
    sp.symOperations.clear();
    sp.symOperations
                     <<"x,y,z"
                     <<"-x,-y,z"
                     <<"-y,x,z"
                     <<"y,-x,z"
                     <<"-x+1/2,y+1/2,-z+1/2"
                     <<"x+1/2,-y+1/2,-z+1/2"
                     <<"y+1/2,x+1/2,-z+1/2"
                     <<"-y+1/2,-x+1/2,-z+1/2"
                     <<"-x,-y,-z"
                     <<"x,y,-z"
                     <<"y,-x,-z"
                     <<"-y,x,-z"
                     <<"x+1/2,-y+1/2,z+1/2"
                     <<"-x+1/2,y+1/2,z+1/2"
                     <<"-y+1/2,-x+1/2,z+1/2"
                     <<"y+1/2,x+1/2,z+1/2";
    spaceGroups[127] = sp;
    //-----------------------
    sp.name = "";
    sp.number = 129;
    sp.symOperations.clear();
    sp.symOperations
                     <<"x,y,z"
                     <<"-x+1/2,-y+1/2,z"
                     <<"-y+1/2,x,z"
                     <<"y,-x+1/2,z"
                     <<"-x,y+1/2,-z"
                     <<"x+1/2,-y,-z"
                     <<"y+1/2,x+1/2,-z"
                     <<"-y,-x,-z"
                     <<"-x,-y,-z"
                     <<"x+1/2,y+1/2,-z"
                     <<"y+1/2,-x,-z"
                     <<"-y,x+1/2,-z"
                     <<"x,-y+1/2,z"
                     <<"-x+1/2,y,z"
                     <<"-y+1/2,-x+1/2,z"
                     <<"y,x,z";
    spaceGroups[128] = sp;
    //-----------------------
    sp.name = "";
    sp.number = 130;
    sp.symOperations.clear();
    sp.symOperations
                     <<"x,y,z"
                     <<"-x+1/2,-y+1/2,z"
                     <<"-y+1/2,x,z"
                     <<"y,-x+1/2,z"
                     <<"-x,y+1/2,-z+1/2"
                     <<"x+1/2,-y,-z+1/2"
                     <<"y+1/2,x+1/2,-z+1/2"
                     <<"-y,-x,-z+1/2"
                     <<"-x,-y,-z"
                     <<"x+1/2,y+1/2,-z"
                     <<"y+1/2,-x,-z"
                     <<"-y,x+1/2,-z"
                     <<"x,-y+1/2,z+1/2"
                     <<"-x+1/2,y,z+1/2"
                     <<"-y+1/2,-x+1/2,z+1/2"
                     <<"y,x,z+1/2";
    spaceGroups[129] = sp;
    //-----------------------
    sp.name = "";
    sp.number = 131;
    sp.symOperations.clear();
    sp.symOperations
                     <<"x,y,z"
                     <<"-x,-y,z"
                     <<"-y,x,z+1/2"
                     <<"y,-x,z+1/2"
                     <<"-x,y,-z"
                     <<"x,-y,-z"
                     <<"y,x,-z+1/2"
                     <<"-y,-x,-z+1/2"
                     <<"-x,-y,-z"
                     <<"x,y,-z"
                     <<"y,-x,-z+1/2"
                     <<"-y,x,-z+1/2"
                     <<"x,-y,z"
                     <<"-x,y,z"
                     <<"-y,-x,z+1/2"
                     <<"y,x,z+1/2";
    spaceGroups[130] = sp;
    //-----------------------
    sp.name = "";
    sp.number = 132;
    sp.symOperations.clear();
    sp.symOperations
                     <<"x,y,z"
                     <<"-x,-y,z"
                     <<"-y,x,z+1/2"
                     <<"y,-x,z+1/2"
                     <<"-x,y,-z+1/2"
                     <<"x,-y,-z+1/2"
                     <<"y,x,-z"
                     <<"-y,-x,-z"
                     <<"-x,-y,-z"
                     <<"x,y,-z"
                     <<"y,-x,-z+1/2"
                     <<"-y,x,-z+1/2"
                     <<"x,-y,z+1/2"
                     <<"-x,y,z+1/2"
                     <<"-y,-x,z"
                     <<"y,x,z";
    spaceGroups[131] = sp;
    //-----------------------
    sp.name = "";
    sp.number = 133;
    sp.symOperations.clear();
    sp.symOperations
                     <<"x,y,z"
                     <<"-x+1/2,-y+1/2,z"
                     <<"-y+1/2,x,z+1/2"
                     <<"y,-x+1/2,z+1/2"
                     <<"-x+1/2,y,-z"
                     <<"x,-y+1/2,-z"
                     <<"y,x,-z+1/2"
                     <<"-y+1/2,-x+1/2,-z+1/2"
                     <<"-x,-y,-z"
                     <<"x+1/2,y+1/2,-z"
                     <<"y+1/2,-x,-z+1/2"
                     <<"-y,x+1/2,-z+1/2"
                     <<"x+1/2,-y,z"
                     <<"-x,y+1/2,z"
                     <<"-y,-x,z+1/2"
                     <<"y+1/2,x+1/2,z+1/2";
    spaceGroups[132] = sp;
    //-----------------------
    sp.name = "";
    sp.number = 134;
    sp.symOperations.clear();
    sp.symOperations
                     <<"x,y,z"
                     <<"-x+1/2,-y+1/2,z"
                     <<"-y+1/2,x,z+1/2"
                     <<"y,-x+1/2,z+1/2"
                     <<"-x+1/2,y,-z+1/2"
                     <<"x,-y+1/2,-z+1/2"
                     <<"y,x,-z"
                     <<"-y+1/2,-x+1/2,-z"
                     <<"-x,-y,-z"
                     <<"x+1/2,y+1/2,-z"
                     <<"y+1/2,-x,-z+1/2"
                     <<"-y,x+1/2,-z+1/2"
                     <<"x+1/2,-y,z+1/2"
                     <<"-x,y+1/2,z+1/2"
                     <<"-y,-x,z"
                     <<"y+1/2,x+1/2,z";
    spaceGroups[133] = sp;
    //-----------------------
    sp.name = "";
    sp.number = 135;
    sp.symOperations.clear();
    sp.symOperations
                     <<"x,y,z"
                     <<"-x,-y,z"
                     <<"-y,x,z+1/2"
                     <<"y,-x,z+1/2"
                     <<"-x+1/2,y+1/2,-z"
                     <<"x+1/2,-y+1/2,-z"
                     <<"y+1/2,x+1/2,-z+1/2"
                     <<"-y+1/2,-x+1/2,-z+1/2"
                     <<"-x,-y,-z"
                     <<"x,y,-z"
                     <<"y,-x,-z+1/2"
                     <<"-y,x,-z+1/2"
                     <<"x+1/2,-y+1/2,z"
                     <<"-x+1/2,y+1/2,z"
                     <<"-y+1/2,-x+1/2,z+1/2"
                     <<"y+1/2,x+1/2,z+1/2";
    spaceGroups[134] = sp;
    //-----------------------
    sp.name = "";
    sp.number = 136;
    sp.symOperations.clear();
    sp.symOperations
                     <<"x,y,z"
                     <<"-x,-y,z"
                     <<"-y+1/2,x+1/2,z+1/2"
                     <<"y+1/2,-x+1/2,z+1/2"
                     <<"-x+1/2,y+1/2,-z+1/2"
                     <<"x+1/2,-y+1/2,-z+1/2"
                     <<"y,x,-z"
                     <<"-y,-x,-z"
                     <<"-x,-y,-z"
                     <<"x,y,-z"
                     <<"y+1/2,-x+1/2,-z+1/2"
                     <<"-y+1/2,x+1/2,-z+1/2"
                     <<"x+1/2,-y+1/2,z+1/2"
                     <<"-x+1/2,y+1/2,z+1/2"
                     <<"-y,-x,z"
                     <<"y,x,z";
    spaceGroups[135] = sp;
    //-----------------------
    sp.name = "";
    sp.number = 137;
    sp.symOperations.clear();
    sp.symOperations
                     <<"x,y,z"
                     <<"-x+1/2,-y+1/2,z"
                     <<"-y+1/2,x,z+1/2"
                     <<"y,-x+1/2,z+1/2"
                     <<"-x,y+1/2,-z"
                     <<"x+1/2,-y,-z"
                     <<"y+1/2,x+1/2,-z+1/2"
                     <<"-y,-x,-z+1/2"
                     <<"-x,-y,-z"
                     <<"x+1/2,y+1/2,-z"
                     <<"y+1/2,-x,-z+1/2"
                     <<"-y,x+1/2,-z+1/2"
                     <<"x,-y+1/2,z"
                     <<"-x+1/2,y,z"
                     <<"-y+1/2,-x+1/2,z+1/2"
                     <<"y,x,z+1/2";
    spaceGroups[136] = sp;
    //-----------------------
    sp.name = "";
    sp.number = 138;
    sp.symOperations.clear();
    sp.symOperations
                     <<"x,y,z"
                     <<"-x+1/2,-y+1/2,z"
                     <<"-y+1/2,x,z+1/2"
                     <<"y,-x+1/2,z+1/2"
                     <<"-x,y+1/2,-z+1/2"
                     <<"x+1/2,-y,-z+1/2"
                     <<"y+1/2,x+1/2,-z"
                     <<"-y,-x,-z"
                     <<"-x,-y,-z"
                     <<"x+1/2,y+1/2,-z"
                     <<"y+1/2,-x,-z+1/2"
                     <<"-y,x+1/2,-z+1/2"
                     <<"x,-y+1/2,z+1/2"
                     <<"-x+1/2,y,z+1/2"
                     <<"-y+1/2,-x+1/2,z"
                     <<"y,x,z";
    spaceGroups[137] = sp;
    //-----------------------
    sp.name = "";
    sp.number = 139;
    sp.symOperations.clear();
    sp.symOperations
                     <<"x,y,z"
                     <<"-x,-y,z"
                     <<"-y,x,z"
                     <<"y,-x,z"
                     <<"-x,y,-z"
                     <<"x,-y,-z"
                     <<"y,x,-z"
                     <<"-y,-x,-z"
                     <<"-x,-y,-z"
                     <<"x,y,-z"
                     <<"y,-x,-z"
                     <<"-y,x,-z"
                     <<"x,-y,z"
                     <<"-x,y,z"
                     <<"-y,-x,z"
                     <<"y,x,z"
                     <<"x+1/2,y+1/2,z+1/2"
                     <<"-x+1/2,-y+1/2,z+1/2"
                     <<"-y+1/2,x+1/2,z+1/2"
                     <<"y+1/2,-x+1/2,z+1/2"
                     <<"-x+1/2,y+1/2,-z+1/2"
                     <<"x+1/2,-y+1/2,-z+1/2"
                     <<"y+1/2,x+1/2,-z+1/2"
                     <<"-y+1/2,-x+1/2,-z+1/2"
                     <<"-x+1/2,-y+1/2,-z+1/2"
                     <<"x+1/2,y+1/2,-z+1/2"
                     <<"y+1/2,-x+1/2,-z+1/2"
                     <<"-y+1/2,x+1/2,-z+1/2"
                     <<"x+1/2,-y+1/2,z+1/2"
                     <<"-x+1/2,y+1/2,z+1/2"
                     <<"-y+1/2,-x+1/2,z+1/2"
                     <<"y+1/2,x+1/2,z+1/2";
    spaceGroups[138] = sp;
    //-----------------------
    sp.name = "";
    sp.number = 140;
    sp.symOperations.clear();
    sp.symOperations
                     <<"x,y,z"
                     <<"-x,-y,z"
                     <<"-y,x,z"
                     <<"y,-x,z"
                     <<"-x,y,-z+1/2"
                     <<"x,-y,-z+1/2"
                     <<"y,x,-z+1/2"
                     <<"-y,-x,-z+1/2"
                     <<"-x,-y,-z"
                     <<"x,y,-z"
                     <<"y,-x,-z"
                     <<"-y,x,-z"
                     <<"x,-y,z+1/2"
                     <<"-x,y,z+1/2"
                     <<"-y,-x,z+1/2"
                     <<"y,x,z+1/2"
                     <<"x+1/2,y+1/2,z+1/2"
                     <<"-x+1/2,-y+1/2,z+1/2"
                     <<"-y+1/2,x+1/2,z+1/2"
                     <<"y+1/2,-x+1/2,z+1/2"
                     <<"-x+1/2,y+1/2,-z"
                     <<"x+1/2,-y+1/2,-z"
                     <<"y+1/2,x+1/2,-z"
                     <<"-y+1/2,-x+1/2,-z"
                     <<"-x+1/2,-y+1/2,-z+1/2"
                     <<"x+1/2,y+1/2,-z+1/2"
                     <<"y+1/2,-x+1/2,-z+1/2"
                     <<"-y+1/2,x+1/2,-z+1/2"
                     <<"x+1/2,-y+1/2,z"
                     <<"-x+1/2,y+1/2,z"
                     <<"-y+1/2,-x+1/2,z"
                     <<"y+1/2,x+1/2,z";
    spaceGroups[139] = sp;
    //-----------------------
    sp.name = "";
    sp.number = 141;
    sp.symOperations.clear();
    sp.symOperations
                     <<"x,y,z"
                     <<"-x+1/2,-y,z+1/2"
                     <<"-y+1/4,x+3/4,z+1/4"
                     <<"y+1/4,-x+1/4,z+3/4"
                     <<"-x+1/2,y,-z+1/2"
                     <<"x,-y,-z"
                     <<"y+1/4,x+3/4,-z+1/4"
                     <<"-y+1/4,-x+1/4,-z+3/4"
                     <<"-x,-y,-z"
                     <<"x+1/2,y,-z+1/2"
                     <<"y+3/4,-x+1/4,-z+3/4"
                     <<"-y+3/4,x+3/4,-z+1/4"
                     <<"x+1/2,-y,z+1/2"
                     <<"-x,y,z"
                     <<"-y+3/4,-x+1/4,z+3/4"
                     <<"y+3/4,x+3/4,z+1/4"
                     <<"x+1/2,y+1/2,z+1/2"
                     <<"-x,-y+1/2,z"
                     <<"-y+3/4,x+1/4,z+3/4"
                     <<"y+3/4,-x+3/4,z+1/4"
                     <<"-x,y+1/2,-z"
                     <<"x+1/2,-y+1/2,-z+1/2"
                     <<"y+3/4,x+1/4,-z+3/4"
                     <<"-y+3/4,-x+3/4,-z+1/4"
                     <<"-x+1/2,-y+1/2,-z+1/2"
                     <<"x,y+1/2,-z"
                     <<"y+1/4,-x+3/4,-z+1/4"
                     <<"-y+1/4,x+1/4,-z+3/4"
                     <<"x,-y+1/2,z"
                     <<"-x+1/2,y+1/2,z+1/2"
                     <<"-y+1/4,-x+3/4,z+1/4"
                     <<"y+1/4,x+1/4,z+3/4";
    spaceGroups[140] = sp;
    //-----------------------
    sp.name = "";
    sp.number = 142;
    sp.symOperations.clear();
    sp.symOperations
                     <<"x,y,z"
                     <<"-x+1/2,-y,z+1/2"
                     <<"-y+1/4,x+3/4,z+1/4"
                     <<"y+1/4,-x+1/4,z+3/4"
                     <<"-x+1/2,y,-z"
                     <<"x,-y,-z+1/2"
                     <<"y+1/4,x+3/4,-z+3/4"
                     <<"-y+1/4,-x+1/4,-z+1/4"
                     <<"-x,-y,-z"
                     <<"x+1/2,y,-z+1/2"
                     <<"y+3/4,-x+1/4,-z+3/4"
                     <<"-y+3/4,x+3/4,-z+1/4"
                     <<"x+1/2,-y,z"
                     <<"-x,y,z+1/2"
                     <<"-y+3/4,-x+1/4,z+1/4"
                     <<"y+3/4,x+3/4,z+3/4"
                     <<"x+1/2,y+1/2,z+1/2"
                     <<"-x,-y+1/2,z"
                     <<"-y+3/4,x+1/4,z+3/4"
                     <<"y+3/4,-x+3/4,z+1/4"
                     <<"-x,y+1/2,-z+1/2"
                     <<"x+1/2,-y+1/2,-z"
                     <<"y+3/4,x+1/4,-z+1/4"
                     <<"-y+3/4,-x+3/4,-z+3/4"
                     <<"-x+1/2,-y+1/2,-z+1/2"
                     <<"x,y+1/2,-z"
                     <<"y+1/4,-x+3/4,-z+1/4"
                     <<"-y+1/4,x+1/4,-z+3/4"
                     <<"x,-y+1/2,z+1/2"
                     <<"-x+1/2,y+1/2,z"
                     <<"-y+1/4,-x+3/4,z+3/4"
                     <<"y+1/4,x+1/4,z+1/4";
    spaceGroups[141] = sp;
    //-----------------------
    sp.name = "";
    sp.number = 143;
    sp.symOperations.clear();
    sp.symOperations
                     <<"x,y,z"
                     <<"-y,x-y,z"
                     <<"-x+y,-x,z";
    spaceGroups[142] = sp;
    //-----------------------
    sp.name = "";
    sp.number = 144;
    sp.symOperations.clear();
    sp.symOperations
                     <<"x,y,z"
                     <<"-y,x-y,z+1/3"
                     <<"-x+y,-x,z+2/3";
    spaceGroups[143] = sp;
    //-----------------------
    sp.name = "";
    sp.number = 145;
    sp.symOperations.clear();
    sp.symOperations
                     <<"x,y,z"
                     <<"-y,x-y,z+2/3"
                     <<"-x+y,-x,z+1/3";
    spaceGroups[144] = sp;
    //-----------------------
    sp.name = "";
    sp.number = 146;
    sp.symOperations.clear();
    sp.symOperations
                     <<"x,y,z"
                     <<"-y,x-y,z"
                     <<"-x+y,-x,z"
                     <<"x+2/3,y+1/3,z+1/3"
                     <<"-y+2/3,x-y+1/3,z+1/3"
                     <<"-x+y+2/3,-x+1/3,z+1/3"
                     <<"x+1/3,y+2/3,z+2/3"
                     <<"-y+1/3,x-y+2/3,z+2/3"
                     <<"-x+y+1/3,-x+2/3,z+2/3";
    spaceGroups[145] = sp;
    //-----------------------
    sp.name = "";
    sp.number = 147;
    sp.symOperations.clear();
    sp.symOperations
                     <<"x,y,z"
                     <<"-y,x-y,z"
                     <<"-x+y,-x,z"
                     <<"-x,-y,-z"
                     <<"y,-x+y,-z"
                     <<"x-y,x,-z";
    spaceGroups[146] = sp;
    //-----------------------
    sp.name = "";
    sp.number = 148;
    sp.symOperations.clear();
    sp.symOperations
                     <<"x,y,z"
                     <<"-y,x-y,z"
                     <<"-x+y,-x,z"
                     <<"-x,-y,-z"
                     <<"y,-x+y,-z"
                     <<"x-y,x,-z"
                     <<"x+2/3,y+1/3,z+1/3"
                     <<"-y+2/3,x-y+1/3,z+1/3"
                     <<"-x+y+2/3,-x+1/3,z+1/3"
                     <<"-x+2/3,-y+1/3,-z+1/3"
                     <<"y+2/3,-x+y+1/3,-z+1/3"
                     <<"x-y+2/3,x+1/3,-z+1/3"
                     <<"x+1/3,y+2/3,z+2/3"
                     <<"-y+1/3,x-y+2/3,z+2/3"
                     <<"-x+y+1/3,-x+2/3,z+2/3"
                     <<"-x+1/3,-y+2/3,-z+2/3"
                     <<"y+1/3,-x+y+2/3,-z+2/3"
                     <<"x-y+1/3,x+2/3,-z+2/3";
    spaceGroups[147] = sp;
    //-----------------------
    sp.name = "";
    sp.number = 149;
    sp.symOperations.clear();
    sp.symOperations
                     <<"x,y,z"
                     <<"-y,x-y,z"
                     <<"-x+y,-x,z"
                     <<"-y,-x,-z"
                     <<"-x+y,y,-z"
                     <<"x,x-y,-z";
    spaceGroups[148] = sp;
    //-----------------------
    sp.name = "";
    sp.number = 150;
    sp.symOperations.clear();
    sp.symOperations
                     <<"x,y,z"
                     <<"-y,x-y,z"
                     <<"-x+y,-x,z"
                     <<"y,x,-z"
                     <<"x-y,-y,-z"
                     <<"-x,-x+y,-z";
    spaceGroups[149] = sp;
    //-----------------------
    sp.name = "";
    sp.number = 151;
    sp.symOperations.clear();
    sp.symOperations
                     <<"x,y,z"
                     <<"-y,x-y,z+1/3"
                     <<"-x+y,-x,z+2/3"
                     <<"-y,-x,-z+2/3"
                     <<"-x+y,y,-z+1/3"
                     <<"x,x-y,-z";
    spaceGroups[150] = sp;
    //-----------------------
    sp.name = "";
    sp.number = 152;
    sp.symOperations.clear();
    sp.symOperations
                     <<"x,y,z"
                     <<"-y,x-y,z+1/3"
                     <<"-x+y,-x,z+2/3"
                     <<"y,x,-z"
                     <<"x-y,-y,-z+2/3"
                     <<"-x,-x+y,-z+1/3";
    spaceGroups[151] = sp;
    //-----------------------
    sp.name = "";
    sp.number = 153;
    sp.symOperations.clear();
    sp.symOperations
                     <<"x,y,z"
                     <<"-y,x-y,z+2/3"
                     <<"-x+y,-x,z+1/3"
                     <<"-y,-x,-z+1/3"
                     <<"-x+y,y,-z+2/3"
                     <<"x,x-y,-z";
    spaceGroups[152] = sp;
    //-----------------------
    sp.name = "";
    sp.number = 154;
    sp.symOperations.clear();
    sp.symOperations
                     <<"x,y,z"
                     <<"-y,x-y,z+2/3"
                     <<"-x+y,-x,z+1/3"
                     <<"y,x,-z"
                     <<"x-y,-y,-z+1/3"
                     <<"-x,-x+y,-z+2/3";
    spaceGroups[153] = sp;
    //-----------------------
    sp.name = "";
    sp.number = 155;
    sp.symOperations.clear();
    sp.symOperations
                     <<"x,y,z"
                     <<"-y,x-y,z"
                     <<"-x+y,-x,z"
                     <<"y,x,-z"
                     <<"x-y,-y,-z"
                     <<"-x,-x+y,-z"
                     <<"x+2/3,y+1/3,z+1/3"
                     <<"-y+2/3,x-y+1/3,z+1/3"
                     <<"-x+y+2/3,-x+1/3,z+1/3"
                     <<"y+2/3,x+1/3,-z+1/3"
                     <<"x-y+2/3,-y+1/3,-z+1/3"
                     <<"-x+2/3,-x+y+1/3,-z+1/3"
                     <<"x+1/3,y+2/3,z+2/3"
                     <<"-y+1/3,x-y+2/3,z+2/3"
                     <<"-x+y+1/3,-x+2/3,z+2/3"
                     <<"y+1/3,x+2/3,-z+2/3"
                     <<"x-y+1/3,-y+2/3,-z+2/3"
                     <<"-x+1/3,-x+y+2/3,-z+2/3";
    spaceGroups[154] = sp;
    //-----------------------
    sp.name = "";
    sp.number = 156;
    sp.symOperations.clear();
    sp.symOperations
                     <<"x,y,z"
                     <<"-y,x-y,z"
                     <<"-x+y,-x,z"
                     <<"-y,-x,z"
                     <<"-x+y,y,z"
                     <<"x,x-y,z";
    spaceGroups[155] = sp;
    //-----------------------
    sp.name = "";
    sp.number = 157;
    sp.symOperations.clear();
    sp.symOperations
                     <<"x,y,z"
                     <<"-y,x-y,z"
                     <<"-x+y,-x,z"
                     <<"y,x,z"
                     <<"x-y,-y,z"
                     <<"-x,-x+y,z";
    spaceGroups[156] = sp;
    //-----------------------
    sp.name = "";
    sp.number = 158;
    sp.symOperations.clear();
    sp.symOperations
                     <<"x,y,z"
                     <<"-y,x-y,z"
                     <<"-x+y,-x,z"
                     <<"-y,-x,z+1/2"
                     <<"-x+y,y,z+1/2"
                     <<"x,x-y,z+1/2";
    spaceGroups[157] = sp;
    //-----------------------
    sp.name = "";
    sp.number = 159;
    sp.symOperations.clear();
    sp.symOperations
                     <<"x,y,z"
                     <<"-y,x-y,z"
                     <<"-x+y,-x,z"
                     <<"y,x,z+1/2"
                     <<"x-y,-y,z+1/2"
                     <<"-x,-x+y,z+1/2";
    spaceGroups[158] = sp;
    //-----------------------
    sp.name = "";
    sp.number = 160;
    sp.symOperations.clear();
    sp.symOperations
                     <<"x,y,z"
                     <<"-y,x-y,z"
                     <<"-x+y,-x,z"
                     <<"-y,-x,z"
                     <<"-x+y,y,z"
                     <<"x,x-y,z"
                     <<"x+2/3,y+1/3,z+1/3"
                     <<"-y+2/3,x-y+1/3,z+1/3"
                     <<"-x+y+2/3,-x+1/3,z+1/3"
                     <<"-y+2/3,-x+1/3,z+1/3"
                     <<"-x+y+2/3,y+1/3,z+1/3"
                     <<"x+2/3,x-y+1/3,z+1/3"
                     <<"x+1/3,y+2/3,z+2/3"
                     <<"-y+1/3,x-y+2/3,z+2/3"
                     <<"-x+y+1/3,-x+2/3,z+2/3"
                     <<"-y+1/3,-x+2/3,z+2/3"
                     <<"-x+y+1/3,y+2/3,z+2/3"
                     <<"x+1/3,x-y+2/3,z+2/3";
    spaceGroups[159] = sp;
    //-----------------------
    sp.name = "";
    sp.number = 161;
    sp.symOperations.clear();
    sp.symOperations
                     <<"x,y,z"
                     <<"-y,x-y,z"
                     <<"-x+y,-x,z"
                     <<"-y,-x,z+1/2"
                     <<"-x+y,y,z+1/2"
                     <<"x,x-y,z+1/2"
                     <<"x+2/3,y+1/3,z+1/3"
                     <<"-y+2/3,x-y+1/3,z+1/3"
                     <<"-x+y+2/3,-x+1/3,z+1/3"
                     <<"-y+2/3,-x+1/3,z+5/6"
                     <<"-x+y+2/3,y+1/3,z+5/6"
                     <<"x+2/3,x-y+1/3,z+5/6"
                     <<"x+1/3,y+2/3,z+2/3"
                     <<"-y+1/3,x-y+2/3,z+2/3"
                     <<"-x+y+1/3,-x+2/3,z+2/3"
                     <<"-y+1/3,-x+2/3,z+1/6"
                     <<"-x+y+1/3,y+2/3,z+1/6"
                     <<"x+1/3,x-y+2/3,z+1/6";
    spaceGroups[160] = sp;
    //-----------------------
    sp.name = "";
    sp.number = 162;
    sp.symOperations.clear();
    sp.symOperations
                     <<"x,y,z"
                     <<"-y,x-y,z"
                     <<"-x+y,-x,z"
                     <<"-y,-x,-z"
                     <<"-x+y,y,-z"
                     <<"x,x-y,-z"
                     <<"-x,-y,-z"
                     <<"y,-x+y,-z"
                     <<"x-y,x,-z"
                     <<"y,x,z"
                     <<"x-y,-y,z"
                     <<"-x,-x+y,z";
    spaceGroups[161] = sp;
    //-----------------------
    sp.name = "";
    sp.number = 163;
    sp.symOperations.clear();
    sp.symOperations
                     <<"x,y,z"
                     <<"-y,x-y,z"
                     <<"-x+y,-x,z"
                     <<"-y,-x,-z+1/2"
                     <<"-x+y,y,-z+1/2"
                     <<"x,x-y,-z+1/2"
                     <<"-x,-y,-z"
                     <<"y,-x+y,-z"
                     <<"x-y,x,-z"
                     <<"y,x,z+1/2"
                     <<"x-y,-y,z+1/2"
                     <<"-x,-x+y,z+1/2";
    spaceGroups[162] = sp;
    //-----------------------
    sp.name = "";
    sp.number = 164;
    sp.symOperations.clear();
    sp.symOperations
                     <<"x,y,z"
                     <<"-y,x-y,z"
                     <<"-x+y,-x,z"
                     <<"y,x,-z"
                     <<"x-y,-y,-z"
                     <<"-x,-x+y,-z"
                     <<"-x,-y,-z"
                     <<"y,-x+y,-z"
                     <<"x-y,x,-z"
                     <<"-y,-x,z"
                     <<"-x+y,y,z"
                     <<"x,x-y,z";
    spaceGroups[163] = sp;
    //-----------------------
    sp.name = "";
    sp.number = 165;
    sp.symOperations.clear();
    sp.symOperations
                     <<"x,y,z"
                     <<"-y,x-y,z"
                     <<"-x+y,-x,z"
                     <<"y,x,-z+1/2"
                     <<"x-y,-y,-z+1/2"
                     <<"-x,-x+y,-z+1/2"
                     <<"-x,-y,-z"
                     <<"y,-x+y,-z"
                     <<"x-y,x,-z"
                     <<"-y,-x,z+1/2"
                     <<"-x+y,y,z+1/2"
                     <<"x,x-y,z+1/2";
    spaceGroups[164] = sp;
    //-----------------------
    sp.name = "";
    sp.number = 166;
    sp.symOperations.clear();
    sp.symOperations
                     <<"x,y,z"
                     <<"-y,x-y,z"
                     <<"-x+y,-x,z"
                     <<"y,x,-z"
                     <<"x-y,-y,-z"
                     <<"-x,-x+y,-z"
                     <<"-x,-y,-z"
                     <<"y,-x+y,-z"
                     <<"x-y,x,-z"
                     <<"-y,-x,z"
                     <<"-x+y,y,z"
                     <<"x,x-y,z"
                     <<"x+2/3,y+1/3,z+1/3"
                     <<"-y+2/3,x-y+1/3,z+1/3"
                     <<"-x+y+2/3,-x+1/3,z+1/3"
                     <<"y+2/3,x+1/3,-z+1/3"
                     <<"x-y+2/3,-y+1/3,-z+1/3"
                     <<"-x+2/3,-x+y+1/3,-z+1/3"
                     <<"-x+2/3,-y+1/3,-z+1/3"
                     <<"y+2/3,-x+y+1/3,-z+1/3"
                     <<"x-y+2/3,x+1/3,-z+1/3"
                     <<"-y+2/3,-x+1/3,z+1/3"
                     <<"-x+y+2/3,y+1/3,z+1/3"
                     <<"x+2/3,x-y+1/3,z+1/3"
                     <<"x+1/3,y+2/3,z+2/3"
                     <<"-y+1/3,x-y+2/3,z+2/3"
                     <<"-x+y+1/3,-x+2/3,z+2/3"
                     <<"y+1/3,x+2/3,-z+2/3"
                     <<"x-y+1/3,-y+2/3,-z+2/3"
                     <<"-x+1/3,-x+y+2/3,-z+2/3"
                     <<"-x+1/3,-y+2/3,-z+2/3"
                     <<"y+1/3,-x+y+2/3,-z+2/3"
                     <<"x-y+1/3,x+2/3,-z+2/3"
                     <<"-y+1/3,-x+2/3,z+2/3"
                     <<"-x+y+1/3,y+2/3,z+2/3"
                     <<"x+1/3,x-y+2/3,z+2/3";
    spaceGroups[165] = sp;
    //-----------------------
    sp.name = "";
    sp.number = 167;
    sp.symOperations.clear();
    sp.symOperations
                     <<"x,y,z"
                     <<"-y,x-y,z"
                     <<"-x+y,-x,z"
                     <<"y,x,-z+1/2"
                     <<"x-y,-y,-z+1/2"
                     <<"-x,-x+y,-z+1/2"
                     <<"-x,-y,-z"
                     <<"y,-x+y,-z"
                     <<"x-y,x,-z"
                     <<"-y,-x,z+1/2"
                     <<"-x+y,y,z+1/2"
                     <<"x,x-y,z+1/2"
                     <<"x+2/3,y+1/3,z+1/3"
                     <<"-y+2/3,x-y+1/3,z+1/3"
                     <<"-x+y+2/3,-x+1/3,z+1/3"
                     <<"y+2/3,x+1/3,-z+5/6"
                     <<"x-y+2/3,-y+1/3,-z+5/6"
                     <<"-x+2/3,-x+y+1/3,-z+5/6"
                     <<"-x+2/3,-y+1/3,-z+1/3"
                     <<"y+2/3,-x+y+1/3,-z+1/3"
                     <<"x-y+2/3,x+1/3,-z+1/3"
                     <<"-y+2/3,-x+1/3,z+5/6"
                     <<"-x+y+2/3,y+1/3,z+5/6"
                     <<"x+2/3,x-y+1/3,z+5/6"
                     <<"x+1/3,y+2/3,z+2/3"
                     <<"-y+1/3,x-y+2/3,z+2/3"
                     <<"-x+y+1/3,-x+2/3,z+2/3"
                     <<"y+1/3,x+2/3,-z+1/6"
                     <<"x-y+1/3,-y+2/3,-z+1/6"
                     <<"-x+1/3,-x+y+2/3,-z+1/6"
                     <<"-x+1/3,-y+2/3,-z+2/3"
                     <<"y+1/3,-x+y+2/3,-z+2/3"
                     <<"x-y+1/3,x+2/3,-z+2/3"
                     <<"-y+1/3,-x+2/3,z+1/6"
                     <<"-x+y+1/3,y+2/3,z+1/6"
                     <<"x+1/3,x-y+2/3,z+1/6";
    spaceGroups[166] = sp;
    //-----------------------
    sp.name = "";
    sp.number = 168;
    sp.symOperations.clear();
    sp.symOperations
                     <<"x,y,z"
                     <<"-y,x-y,z"
                     <<"-x+y,-x,z"
                     <<"-x,-y,z"
                     <<"y,-x+y,z"
                     <<"x-y,x,z";
    spaceGroups[167] = sp;
    //-----------------------
    sp.name = "";
    sp.number = 169;
    sp.symOperations.clear();
    sp.symOperations
                     <<"x,y,z"
                     <<"-y,x-y,z+1/3"
                     <<"-x+y,-x,z+2/3"
                     <<"-x,-y,z+1/2"
                     <<"y,-x+y,z+5/6"
                     <<"x-y,x,z+1/6";
    spaceGroups[168] = sp;
    //-----------------------
    sp.name = "";
    sp.number = 170;
    sp.symOperations.clear();
    sp.symOperations
                     <<"x,y,z"
                     <<"-y,x-y,z+2/3"
                     <<"-x+y,-x,z+1/3"
                     <<"-x,-y,z+1/2"
                     <<"y,-x+y,z+1/6"
                     <<"x-y,x,z+5/6";
    spaceGroups[169] = sp;
    //-----------------------
    sp.name = "";
    sp.number = 171;
    sp.symOperations.clear();
    sp.symOperations
                     <<"x,y,z"
                     <<"-y,x-y,z+2/3"
                     <<"-x+y,-x,z+1/3"
                     <<"-x,-y,z"
                     <<"y,-x+y,z+2/3"
                     <<"x-y,x,z+1/3";
    spaceGroups[170] = sp;
    //-----------------------
    sp.name = "";
    sp.number = 172;
    sp.symOperations.clear();
    sp.symOperations
                     <<"x,y,z"
                     <<"-y,x-y,z+1/3"
                     <<"-x+y,-x,z+2/3"
                     <<"-x,-y,z"
                     <<"y,-x+y,z+1/3"
                     <<"x-y,x,z+2/3";
    spaceGroups[171] = sp;
    //-----------------------
    sp.name = "";
    sp.number = 173;
    sp.symOperations.clear();
    sp.symOperations
                     <<"x,y,z"
                     <<"-y,x-y,z"
                     <<"-x+y,-x,z"
                     <<"-x,-y,z+1/2"
                     <<"y,-x+y,z+1/2"
                     <<"x-y,x,z+1/2";
    spaceGroups[172] = sp;
    //-----------------------
    sp.name = "";
    sp.number = 174;
    sp.symOperations.clear();
    sp.symOperations
                     <<"x,y,z"
                     <<"-y,x-y,z"
                     <<"-x+y,-x,z"
                     <<"x,y,-z"
                     <<"-y,x-y,-z"
                     <<"-x+y,-x,-z";
    spaceGroups[173] = sp;
    //-----------------------
    sp.name = "";
    sp.number = 175;
    sp.symOperations.clear();
    sp.symOperations
                     <<"x,y,z"
                     <<"-y,x-y,z"
                     <<"-x+y,-x,z"
                     <<"-x,-y,z"
                     <<"y,-x+y,z"
                     <<"x-y,x,z"
                     <<"-x,-y,-z"
                     <<"y,-x+y,-z"
                     <<"x-y,x,-z"
                     <<"x,y,-z"
                     <<"-y,x-y,-z"
                     <<"-x+y,-x,-z";
    spaceGroups[174] = sp;
    //-----------------------
    sp.name = "";
    sp.number = 176;
    sp.symOperations.clear();
    sp.symOperations
                     <<"x,y,z"
                     <<"-y,x-y,z"
                     <<"-x+y,-x,z"
                     <<"-x,-y,z+1/2"
                     <<"y,-x+y,z+1/2"
                     <<"x-y,x,z+1/2"
                     <<"-x,-y,-z"
                     <<"y,-x+y,-z"
                     <<"x-y,x,-z"
                     <<"x,y,-z+1/2"
                     <<"-y,x-y,-z+1/2"
                     <<"-x+y,-x,-z+1/2";
    spaceGroups[175] = sp;
    //-----------------------
    sp.name = "";
    sp.number = 177;
    sp.symOperations.clear();
    sp.symOperations
                     <<"x,y,z"
                     <<"-y,x-y,z"
                     <<"-x+y,-x,z"
                     <<"-x,-y,z"
                     <<"y,-x+y,z"
                     <<"x-y,x,z"
                     <<"y,x,-z"
                     <<"x-y,-y,-z"
                     <<"-x,-x+y,-z"
                     <<"-y,-x,-z"
                     <<"-x+y,y,-z"
                     <<"x,x-y,-z";
    spaceGroups[176] = sp;
    //-----------------------
    sp.name = "";
    sp.number = 178;
    sp.symOperations.clear();
    sp.symOperations
                     <<"x,y,z"
                     <<"-y,x-y,z+1/3"
                     <<"-x+y,-x,z+2/3"
                     <<"-x,-y,z+1/2"
                     <<"y,-x+y,z+5/6"
                     <<"x-y,x,z+1/6"
                     <<"y,x,-z+1/3"
                     <<"x-y,-y,-z"
                     <<"-x,-x+y,-z+2/3"
                     <<"-y,-x,-z+5/6"
                     <<"-x+y,y,-z+1/2"
                     <<"x,x-y,-z+1/6";
    spaceGroups[177] = sp;
    //-----------------------
    sp.name = "";
    sp.number = 179;
    sp.symOperations.clear();
    sp.symOperations
                     <<"x,y,z"
                     <<"-y,x-y,z+2/3"
                     <<"-x+y,-x,z+1/3"
                     <<"-x,-y,z+1/2"
                     <<"y,-x+y,z+1/6"
                     <<"x-y,x,z+5/6"
                     <<"y,x,-z+2/3"
                     <<"x-y,-y,-z"
                     <<"-x,-x+y,-z+1/3"
                     <<"-y,-x,-z+1/6"
                     <<"-x+y,y,-z+1/2"
                     <<"x,x-y,-z+5/6";
    spaceGroups[178] = sp;
    //-----------------------
    sp.name = "";
    sp.number = 180;
    sp.symOperations.clear();
    sp.symOperations
                     <<"x,y,z"
                     <<"-y,x-y,z+2/3"
                     <<"-x+y,-x,z+1/3"
                     <<"-x,-y,z"
                     <<"y,-x+y,z+2/3"
                     <<"x-y,x,z+1/3"
                     <<"y,x,-z+2/3"
                     <<"x-y,-y,-z"
                     <<"-x,-x+y,-z+1/3"
                     <<"-y,-x,-z+2/3"
                     <<"-x+y,y,-z"
                     <<"x,x-y,-z+1/3";
    spaceGroups[179] = sp;
    //-----------------------
    sp.name = "";
    sp.number = 181;
    sp.symOperations.clear();
    sp.symOperations
                     <<"x,y,z"
                     <<"-y,x-y,z+1/3"
                     <<"-x+y,-x,z+2/3"
                     <<"-x,-y,z"
                     <<"y,-x+y,z+1/3"
                     <<"x-y,x,z+2/3"
                     <<"y,x,-z+1/3"
                     <<"x-y,-y,-z"
                     <<"-x,-x+y,-z+2/3"
                     <<"-y,-x,-z+1/3"
                     <<"-x+y,y,-z"
                     <<"x,x-y,-z+2/3";
    spaceGroups[180] = sp;
    //-----------------------
    sp.name = "";
    sp.number = 182;
    sp.symOperations.clear();
    sp.symOperations
                     <<"x,y,z"
                     <<"-y,x-y,z"
                     <<"-x+y,-x,z"
                     <<"-x,-y,z+1/2"
                     <<"y,-x+y,z+1/2"
                     <<"x-y,x,z+1/2"
                     <<"y,x,-z"
                     <<"x-y,-y,-z"
                     <<"-x,-x+y,-z"
                     <<"-y,-x,-z+1/2"
                     <<"-x+y,y,-z+1/2"
                     <<"x,x-y,-z+1/2";
    spaceGroups[181] = sp;
    //-----------------------
    sp.name = "";
    sp.number = 183;
    sp.symOperations.clear();
    sp.symOperations
                     <<"x,y,z"
                     <<"-y,x-y,z"
                     <<"-x+y,-x,z"
                     <<"-x,-y,z"
                     <<"y,-x+y,z"
                     <<"x-y,x,z"
                     <<"-y,-x,z"
                     <<"-x+y,y,z"
                     <<"x,x-y,z"
                     <<"y,x,z"
                     <<"x-y,-y,z"
                     <<"-x,-x+y,z";
    spaceGroups[182] = sp;
    //-----------------------
    sp.name = "";
    sp.number = 184;
    sp.symOperations.clear();
    sp.symOperations
                     <<"x,y,z"
                     <<"-y,x-y,z"
                     <<"-x+y,-x,z"
                     <<"-x,-y,z"
                     <<"y,-x+y,z"
                     <<"x-y,x,z"
                     <<"-y,-x,z+1/2"
                     <<"-x+y,y,z+1/2"
                     <<"x,x-y,z+1/2"
                     <<"y,x,z+1/2"
                     <<"x-y,-y,z+1/2"
                     <<"-x,-x+y,z+1/2";
    spaceGroups[183] = sp;
    //-----------------------
    sp.name = "";
    sp.number = 185;
    sp.symOperations.clear();
    sp.symOperations
                     <<"x,y,z"
                     <<"-y,x-y,z"
                     <<"-x+y,-x,z"
                     <<"-x,-y,z+1/2"
                     <<"y,-x+y,z+1/2"
                     <<"x-y,x,z+1/2"
                     <<"-y,-x,z+1/2"
                     <<"-x+y,y,z+1/2"
                     <<"x,x-y,z+1/2"
                     <<"y,x,z"
                     <<"x-y,-y,z"
                     <<"-x,-x+y,z";
    spaceGroups[184] = sp;
    //-----------------------
    sp.name = "";
    sp.number = 186;
    sp.symOperations.clear();
    sp.symOperations
                     <<"x,y,z"
                     <<"-y,x-y,z"
                     <<"-x+y,-x,z"
                     <<"-x,-y,z+1/2"
                     <<"y,-x+y,z+1/2"
                     <<"x-y,x,z+1/2"
                     <<"-y,-x,z"
                     <<"-x+y,y,z"
                     <<"x,x-y,z"
                     <<"y,x,z+1/2"
                     <<"x-y,-y,z+1/2"
                     <<"-x,-x+y,z+1/2";
    spaceGroups[185] = sp;
    //-----------------------
    sp.name = "";
    sp.number = 187;
    sp.symOperations.clear();
    sp.symOperations
                     <<"x,y,z"
                     <<"-y,x-y,z"
                     <<"-x+y,-x,z"
                     <<"x,y,-z"
                     <<"-y,x-y,-z"
                     <<"-x+y,-x,-z"
                     <<"-y,-x,z"
                     <<"-x+y,y,z"
                     <<"x,x-y,z"
                     <<"-y,-x,-z"
                     <<"-x+y,y,-z"
                     <<"x,x-y,-z";
    spaceGroups[186] = sp;
    //-----------------------
    sp.name = "";
    sp.number = 188;
    sp.symOperations.clear();
    sp.symOperations
                     <<"x,y,z"
                     <<"-y,x-y,z"
                     <<"-x+y,-x,z"
                     <<"x,y,-z+1/2"
                     <<"-y,x-y,-z+1/2"
                     <<"-x+y,-x,-z+1/2"
                     <<"-y,-x,z+1/2"
                     <<"-x+y,y,z+1/2"
                     <<"x,x-y,z+1/2"
                     <<"-y,-x,-z"
                     <<"-x+y,y,-z"
                     <<"x,x-y,-z";
    spaceGroups[187] = sp;
    //-----------------------
    sp.name = "";
    sp.number = 189;
    sp.symOperations.clear();
    sp.symOperations
                     <<"x,y,z"
                     <<"-y,x-y,z"
                     <<"-x+y,-x,z"
                     <<"x,y,-z"
                     <<"-y,x-y,-z"
                     <<"-x+y,-x,-z"
                     <<"y,x,-z"
                     <<"x-y,-y,-z"
                     <<"-x,-x+y,-z"
                     <<"y,x,z"
                     <<"x-y,-y,z"
                     <<"-x,-x+y,z";
    spaceGroups[188] = sp;
    //-----------------------
    sp.name = "";
    sp.number = 190;
    sp.symOperations.clear();
    sp.symOperations
                     <<"x,y,z"
                     <<"-y,x-y,z"
                     <<"-x+y,-x,z"
                     <<"x,y,-z+1/2"
                     <<"-y,x-y,-z+1/2"
                     <<"-x+y,-x,-z+1/2"
                     <<"y,x,-z"
                     <<"x-y,-y,-z"
                     <<"-x,-x+y,-z"
                     <<"y,x,z+1/2"
                     <<"x-y,-y,z+1/2"
                     <<"-x,-x+y,z+1/2";
    spaceGroups[189] = sp;
    //-----------------------
    sp.name = "";
    sp.number = 191;
    sp.symOperations.clear();
    sp.symOperations
                     <<"x,y,z"
                     <<"-y,x-y,z"
                     <<"-x+y,-x,z"
                     <<"-x,-y,z"
                     <<"y,-x+y,z"
                     <<"x-y,x,z"
                     <<"y,x,-z"
                     <<"x-y,-y,-z"
                     <<"-x,-x+y,-z"
                     <<"-y,-x,-z"
                     <<"-x+y,y,-z"
                     <<"x,x-y,-z"
                     <<"-x,-y,-z"
                     <<"y,-x+y,-z"
                     <<"x-y,x,-z"
                     <<"x,y,-z"
                     <<"-y,x-y,-z"
                     <<"-x+y,-x,-z"
                     <<"-y,-x,z"
                     <<"-x+y,y,z"
                     <<"x,x-y,z"
                     <<"y,x,z"
                     <<"x-y,-y,z"
                     <<"-x,-x+y,z";
    spaceGroups[190] = sp;
    //-----------------------
    sp.name = "";
    sp.number = 192;
    sp.symOperations.clear();
    sp.symOperations
                     <<"x,y,z"
                     <<"-y,x-y,z"
                     <<"-x+y,-x,z"
                     <<"-x,-y,z"
                     <<"y,-x+y,z"
                     <<"x-y,x,z"
                     <<"y,x,-z+1/2"
                     <<"x-y,-y,-z+1/2"
                     <<"-x,-x+y,-z+1/2"
                     <<"-y,-x,-z+1/2"
                     <<"-x+y,y,-z+1/2"
                     <<"x,x-y,-z+1/2"
                     <<"-x,-y,-z"
                     <<"y,-x+y,-z"
                     <<"x-y,x,-z"
                     <<"x,y,-z"
                     <<"-y,x-y,-z"
                     <<"-x+y,-x,-z"
                     <<"-y,-x,z+1/2"
                     <<"-x+y,y,z+1/2"
                     <<"x,x-y,z+1/2"
                     <<"y,x,z+1/2"
                     <<"x-y,-y,z+1/2"
                     <<"-x,-x+y,z+1/2";
    spaceGroups[191] = sp;
    //-----------------------
    sp.name = "";
    sp.number = 193;
    sp.symOperations.clear();
    sp.symOperations
                     <<"x,y,z"
                     <<"-y,x-y,z"
                     <<"-x+y,-x,z"
                     <<"-x,-y,z+1/2"
                     <<"y,-x+y,z+1/2"
                     <<"x-y,x,z+1/2"
                     <<"y,x,-z+1/2"
                     <<"x-y,-y,-z+1/2"
                     <<"-x,-x+y,-z+1/2"
                     <<"-y,-x,-z"
                     <<"-x+y,y,-z"
                     <<"x,x-y,-z"
                     <<"-x,-y,-z"
                     <<"y,-x+y,-z"
                     <<"x-y,x,-z"
                     <<"x,y,-z+1/2"
                     <<"-y,x-y,-z+1/2"
                     <<"-x+y,-x,-z+1/2"
                     <<"-y,-x,z+1/2"
                     <<"-x+y,y,z+1/2"
                     <<"x,x-y,z+1/2"
                     <<"y,x,z"
                     <<"x-y,-y,z"
                     <<"-x,-x+y,z";
    spaceGroups[192] = sp;
    //-----------------------
    sp.name = "";
    sp.number = 194;
    sp.symOperations.clear();
    sp.symOperations
                     <<"x,y,z"
                     <<"-y,x-y,z"
                     <<"-x+y,-x,z"
                     <<"-x,-y,z+1/2"
                     <<"y,-x+y,z+1/2"
                     <<"x-y,x,z+1/2"
                     <<"y,x,-z"
                     <<"x-y,-y,-z"
                     <<"-x,-x+y,-z"
                     <<"-y,-x,-z+1/2"
                     <<"-x+y,y,-z+1/2"
                     <<"x,x-y,-z+1/2"
                     <<"-x,-y,-z"
                     <<"y,-x+y,-z"
                     <<"x-y,x,-z"
                     <<"x,y,-z+1/2"
                     <<"-y,x-y,-z+1/2"
                     <<"-x+y,-x,-z+1/2"
                     <<"-y,-x,z"
                     <<"-x+y,y,z"
                     <<"x,x-y,z"
                     <<"y,x,z+1/2"
                     <<"x-y,-y,z+1/2"
                     <<"-x,-x+y,z+1/2";
    spaceGroups[193] = sp;
    //-----------------------
    sp.name = "";
    sp.number = 195;
    sp.symOperations.clear();
    sp.symOperations
                     <<"x,y,z"
                     <<"-x,-y,z"
                     <<"-x,y,-z"
                     <<"x,-y,-z"
                     <<"z,x,y"
                     <<"z,-x,-y"
                     <<"-z,-x,y"
                     <<"-z,x,-y"
                     <<"y,z,x"
                     <<"-y,z,-x"
                     <<"y,-z,-x"
                     <<"-y,-z,x";
    spaceGroups[194] = sp;
    //-----------------------
    sp.name = "";
    sp.number = 196;
    sp.symOperations.clear();
    sp.symOperations
                     <<"x,y,z"
                     <<"-x,-y,z"
                     <<"-x,y,-z"
                     <<"x,-y,-z"
                     <<"z,x,y"
                     <<"z,-x,-y"
                     <<"-z,-x,y"
                     <<"-z,x,-y"
                     <<"y,z,x"
                     <<"-y,z,-x"
                     <<"y,-z,-x"
                     <<"-y,-z,x"
                     <<"x,y+1/2,z+1/2"
                     <<"-x,-y+1/2,z+1/2"
                     <<"-x,y+1/2,-z+1/2"
                     <<"x,-y+1/2,-z+1/2"
                     <<"z,x+1/2,y+1/2"
                     <<"z,-x+1/2,-y+1/2"
                     <<"-z,-x+1/2,y+1/2"
                     <<"-z,x+1/2,-y+1/2"
                     <<"y,z+1/2,x+1/2"
                     <<"-y,z+1/2,-x+1/2"
                     <<"y,-z+1/2,-x+1/2"
                     <<"-y,-z+1/2,x+1/2"
                     <<"x+1/2,y,z+1/2"
                     <<"-x+1/2,-y,z+1/2"
                     <<"-x+1/2,y,-z+1/2"
                     <<"x+1/2,-y,-z+1/2"
                     <<"z+1/2,x,y+1/2"
                     <<"z+1/2,-x,-y+1/2"
                     <<"-z+1/2,-x,y+1/2"
                     <<"-z+1/2,x,-y+1/2"
                     <<"y+1/2,z,x+1/2"
                     <<"-y+1/2,z,-x+1/2"
                     <<"y+1/2,-z,-x+1/2"
                     <<"-y+1/2,-z,x+1/2"
                     <<"x+1/2,y+1/2,z"
                     <<"-x+1/2,-y+1/2,z"
                     <<"-x+1/2,y+1/2,-z"
                     <<"x+1/2,-y+1/2,-z"
                     <<"z+1/2,x+1/2,y"
                     <<"z+1/2,-x+1/2,-y"
                     <<"-z+1/2,-x+1/2,y"
                     <<"-z+1/2,x+1/2,-y"
                     <<"y+1/2,z+1/2,x"
                     <<"-y+1/2,z+1/2,-x"
                     <<"y+1/2,-z+1/2,-x"
                     <<"-y+1/2,-z+1/2,x";
    spaceGroups[195] = sp;
    //-----------------------
    sp.name = "";
    sp.number = 197;
    sp.symOperations.clear();
    sp.symOperations
                     <<"x,y,z"
                     <<"-x,-y,z"
                     <<"-x,y,-z"
                     <<"x,-y,-z"
                     <<"z,x,y"
                     <<"z,-x,-y"
                     <<"-z,-x,y"
                     <<"-z,x,-y"
                     <<"y,z,x"
                     <<"-y,z,-x"
                     <<"y,-z,-x"
                     <<"-y,-z,x"
                     <<"x+1/2,y+1/2,z+1/2"
                     <<"-x+1/2,-y+1/2,z+1/2"
                     <<"-x+1/2,y+1/2,-z+1/2"
                     <<"x+1/2,-y+1/2,-z+1/2"
                     <<"z+1/2,x+1/2,y+1/2"
                     <<"z+1/2,-x+1/2,-y+1/2"
                     <<"-z+1/2,-x+1/2,y+1/2"
                     <<"-z+1/2,x+1/2,-y+1/2"
                     <<"y+1/2,z+1/2,x+1/2"
                     <<"-y+1/2,z+1/2,-x+1/2"
                     <<"y+1/2,-z+1/2,-x+1/2"
                     <<"-y+1/2,-z+1/2,x+1/2";
    spaceGroups[196] = sp;
    //-----------------------
    sp.name = "";
    sp.number = 198;
    sp.symOperations.clear();
    sp.symOperations
                     <<"x,y,z"
                     <<"-x+1/2,-y,z+1/2"
                     <<"-x,y+1/2,-z+1/2"
                     <<"x+1/2,-y+1/2,-z"
                     <<"z,x,y"
                     <<"z+1/2,-x+1/2,-y"
                     <<"-z+1/2,-x,y+1/2"
                     <<"-z,x+1/2,-y+1/2"
                     <<"y,z,x"
                     <<"-y,z+1/2,-x+1/2"
                     <<"y+1/2,-z+1/2,-x"
                     <<"-y+1/2,-z,x+1/2";
    spaceGroups[197] = sp;
    //-----------------------
    sp.name = "";
    sp.number = 199;
    sp.symOperations.clear();
    sp.symOperations
                     <<"x,y,z"
                     <<"-x+1/2,-y,z+1/2"
                     <<"-x,y+1/2,-z+1/2"
                     <<"x+1/2,-y+1/2,-z"
                     <<"z,x,y"
                     <<"z+1/2,-x+1/2,-y"
                     <<"-z+1/2,-x,y+1/2"
                     <<"-z,x+1/2,-y+1/2"
                     <<"y,z,x"
                     <<"-y,z+1/2,-x+1/2"
                     <<"y+1/2,-z+1/2,-x"
                     <<"-y+1/2,-z,x+1/2"
                     <<"x+1/2,y+1/2,z+1/2"
                     <<"-x,-y+1/2,z"
                     <<"-x+1/2,y,-z"
                     <<"x,-y,-z+1/2"
                     <<"z+1/2,x+1/2,y+1/2"
                     <<"z,-x,-y+1/2"
                     <<"-z,-x+1/2,y"
                     <<"-z+1/2,x,-y"
                     <<"y+1/2,z+1/2,x+1/2"
                     <<"-y+1/2,z,-x"
                     <<"y,-z,-x+1/2"
                     <<"-y,-z+1/2,x";
    spaceGroups[198] = sp;
    //-----------------------
    sp.name = "";
    sp.number = 200;
    sp.symOperations.clear();
    sp.symOperations
                     <<"x,y,z"
                     <<"-x,-y,z"
                     <<"-x,y,-z"
                     <<"x,-y,-z"
                     <<"z,x,y"
                     <<"z,-x,-y"
                     <<"-z,-x,y"
                     <<"-z,x,-y"
                     <<"y,z,x"
                     <<"-y,z,-x"
                     <<"y,-z,-x"
                     <<"-y,-z,x"
                     <<"-x,-y,-z"
                     <<"x,y,-z"
                     <<"x,-y,z"
                     <<"-x,y,z"
                     <<"-z,-x,-y"
                     <<"-z,x,y"
                     <<"z,x,-y"
                     <<"z,-x,y"
                     <<"-y,-z,-x"
                     <<"y,-z,x"
                     <<"-y,z,x"
                     <<"y,z,-x";
    spaceGroups[199] = sp;
    //-----------------------
    sp.name = "";
    sp.number = 201;
    sp.symOperations.clear();
    sp.symOperations
                     <<"x,y,z"
                     <<"-x+1/2,-y+1/2,z"
                     <<"-x+1/2,y,-z+1/2"
                     <<"x,-y+1/2,-z+1/2"
                     <<"z,x,y"
                     <<"z,-x+1/2,-y+1/2"
                     <<"-z+1/2,-x+1/2,y"
                     <<"-z+1/2,x,-y+1/2"
                     <<"y,z,x"
                     <<"-y+1/2,z,-x+1/2"
                     <<"y,-z+1/2,-x+1/2"
                     <<"-y+1/2,-z+1/2,x"
                     <<"-x,-y,-z"
                     <<"x+1/2,y+1/2,-z"
                     <<"x+1/2,-y,z+1/2"
                     <<"-x,y+1/2,z+1/2"
                     <<"-z,-x,-y"
                     <<"-z,x+1/2,y+1/2"
                     <<"z+1/2,x+1/2,-y"
                     <<"z+1/2,-x,y+1/2"
                     <<"-y,-z,-x"
                     <<"y+1/2,-z,x+1/2"
                     <<"-y,z+1/2,x+1/2"
                     <<"y+1/2,z+1/2,-x";
    spaceGroups[200] = sp;
    //-----------------------
    sp.name = "";
    sp.number = 202;
    sp.symOperations.clear();
    sp.symOperations
                     <<"x,y,z"
                     <<"-x,-y,z"
                     <<"-x,y,-z"
                     <<"x,-y,-z"
                     <<"z,x,y"
                     <<"z,-x,-y"
                     <<"-z,-x,y"
                     <<"-z,x,-y"
                     <<"y,z,x"
                     <<"-y,z,-x"
                     <<"y,-z,-x"
                     <<"-y,-z,x"
                     <<"-x,-y,-z"
                     <<"x,y,-z"
                     <<"x,-y,z"
                     <<"-x,y,z"
                     <<"-z,-x,-y"
                     <<"-z,x,y"
                     <<"z,x,-y"
                     <<"z,-x,y"
                     <<"-y,-z,-x"
                     <<"y,-z,x"
                     <<"-y,z,x"
                     <<"y,z,-x"
                     <<"x,y+1/2,z+1/2"
                     <<"-x,-y+1/2,z+1/2"
                     <<"-x,y+1/2,-z+1/2"
                     <<"x,-y+1/2,-z+1/2"
                     <<"z,x+1/2,y+1/2"
                     <<"z,-x+1/2,-y+1/2"
                     <<"-z,-x+1/2,y+1/2"
                     <<"-z,x+1/2,-y+1/2"
                     <<"y,z+1/2,x+1/2"
                     <<"-y,z+1/2,-x+1/2"
                     <<"y,-z+1/2,-x+1/2"
                     <<"-y,-z+1/2,x+1/2"
                     <<"-x,-y+1/2,-z+1/2"
                     <<"x,y+1/2,-z+1/2"
                     <<"x,-y+1/2,z+1/2"
                     <<"-x,y+1/2,z+1/2"
                     <<"-z,-x+1/2,-y+1/2"
                     <<"-z,x+1/2,y+1/2"
                     <<"z,x+1/2,-y+1/2"
                     <<"z,-x+1/2,y+1/2"
                     <<"-y,-z+1/2,-x+1/2"
                     <<"y,-z+1/2,x+1/2"
                     <<"-y,z+1/2,x+1/2"
                     <<"y,z+1/2,-x+1/2"
                     <<"x+1/2,y,z+1/2"
                     <<"-x+1/2,-y,z+1/2"
                     <<"-x+1/2,y,-z+1/2"
                     <<"x+1/2,-y,-z+1/2"
                     <<"z+1/2,x,y+1/2"
                     <<"z+1/2,-x,-y+1/2"
                     <<"-z+1/2,-x,y+1/2"
                     <<"-z+1/2,x,-y+1/2"
                     <<"y+1/2,z,x+1/2"
                     <<"-y+1/2,z,-x+1/2"
                     <<"y+1/2,-z,-x+1/2"
                     <<"-y+1/2,-z,x+1/2"
                     <<"-x+1/2,-y,-z+1/2"
                     <<"x+1/2,y,-z+1/2"
                     <<"x+1/2,-y,z+1/2"
                     <<"-x+1/2,y,z+1/2"
                     <<"-z+1/2,-x,-y+1/2"
                     <<"-z+1/2,x,y+1/2"
                     <<"z+1/2,x,-y+1/2"
                     <<"z+1/2,-x,y+1/2"
                     <<"-y+1/2,-z,-x+1/2"
                     <<"y+1/2,-z,x+1/2"
                     <<"-y+1/2,z,x+1/2"
                     <<"y+1/2,z,-x+1/2"
                     <<"x+1/2,y+1/2,z"
                     <<"-x+1/2,-y+1/2,z"
                     <<"-x+1/2,y+1/2,-z"
                     <<"x+1/2,-y+1/2,-z"
                     <<"z+1/2,x+1/2,y"
                     <<"z+1/2,-x+1/2,-y"
                     <<"-z+1/2,-x+1/2,y"
                     <<"-z+1/2,x+1/2,-y"
                     <<"y+1/2,z+1/2,x"
                     <<"-y+1/2,z+1/2,-x"
                     <<"y+1/2,-z+1/2,-x"
                     <<"-y+1/2,-z+1/2,x"
                     <<"-x+1/2,-y+1/2,-z"
                     <<"x+1/2,y+1/2,-z"
                     <<"x+1/2,-y+1/2,z"
                     <<"-x+1/2,y+1/2,z"
                     <<"-z+1/2,-x+1/2,-y"
                     <<"-z+1/2,x+1/2,y"
                     <<"z+1/2,x+1/2,-y"
                     <<"z+1/2,-x+1/2,y"
                     <<"-y+1/2,-z+1/2,-x"
                     <<"y+1/2,-z+1/2,x"
                     <<"-y+1/2,z+1/2,x"
                     <<"y+1/2,z+1/2,-x";
    spaceGroups[201] = sp;
    //-----------------------
    sp.name = "";
    sp.number = 203;
    sp.symOperations.clear();
    sp.symOperations
                     <<"x,y,z"
                     <<"-x+3/4,-y+3/4,z"
                     <<"-x+3/4,y,-z+3/4"
                     <<"x,-y+3/4,-z+3/4"
                     <<"z,x,y"
                     <<"z,-x+3/4,-y+3/4"
                     <<"-z+3/4,-x+3/4,y"
                     <<"-z+3/4,x,-y+3/4"
                     <<"y,z,x"
                     <<"-y+3/4,z,-x+3/4"
                     <<"y,-z+3/4,-x+3/4"
                     <<"-y+3/4,-z+3/4,x"
                     <<"-x,-y,-z"
                     <<"x+1/4,y+1/4,-z"
                     <<"x+1/4,-y,z+1/4"
                     <<"-x,y+1/4,z+1/4"
                     <<"-z,-x,-y"
                     <<"-z,x+1/4,y+1/4"
                     <<"z+1/4,x+1/4,-y"
                     <<"z+1/4,-x,y+1/4"
                     <<"-y,-z,-x"
                     <<"y+1/4,-z,x+1/4"
                     <<"-y,z+1/4,x+1/4"
                     <<"y+1/4,z+1/4,-x"
                     <<"x,y+1/2,z+1/2"
                     <<"-x+3/4,-y+1/4,z+1/2"
                     <<"-x+3/4,y+1/2,-z+1/4"
                     <<"x,-y+1/4,-z+1/4"
                     <<"z,x+1/2,y+1/2"
                     <<"z,-x+1/4,-y+1/4"
                     <<"-z+3/4,-x+1/4,y+1/2"
                     <<"-z+3/4,x+1/2,-y+1/4"
                     <<"y,z+1/2,x+1/2"
                     <<"-y+3/4,z+1/2,-x+1/4"
                     <<"y,-z+1/4,-x+1/4"
                     <<"-y+3/4,-z+1/4,x+1/2"
                     <<"-x,-y+1/2,-z+1/2"
                     <<"x+1/4,y+3/4,-z+1/2"
                     <<"x+1/4,-y+1/2,z+3/4"
                     <<"-x,y+3/4,z+3/4"
                     <<"-z,-x+1/2,-y+1/2"
                     <<"-z,x+3/4,y+3/4"
                     <<"z+1/4,x+3/4,-y+1/2"
                     <<"z+1/4,-x+1/2,y+3/4"
                     <<"-y,-z+1/2,-x+1/2"
                     <<"y+1/4,-z+1/2,x+3/4"
                     <<"-y,z+3/4,x+3/4"
                     <<"y+1/4,z+3/4,-x+1/2"
                     <<"x+1/2,y,z+1/2"
                     <<"-x+1/4,-y+3/4,z+1/2"
                     <<"-x+1/4,y,-z+1/4"
                     <<"x+1/2,-y+3/4,-z+1/4"
                     <<"z+1/2,x,y+1/2"
                     <<"z+1/2,-x+3/4,-y+1/4"
                     <<"-z+1/4,-x+3/4,y+1/2"
                     <<"-z+1/4,x,-y+1/4"
                     <<"y+1/2,z,x+1/2"
                     <<"-y+1/4,z,-x+1/4"
                     <<"y+1/2,-z+3/4,-x+1/4"
                     <<"-y+1/4,-z+3/4,x+1/2"
                     <<"-x+1/2,-y,-z+1/2"
                     <<"x+3/4,y+1/4,-z+1/2"
                     <<"x+3/4,-y,z+3/4"
                     <<"-x+1/2,y+1/4,z+3/4"
                     <<"-z+1/2,-x,-y+1/2"
                     <<"-z+1/2,x+1/4,y+3/4"
                     <<"z+3/4,x+1/4,-y+1/2"
                     <<"z+3/4,-x,y+3/4"
                     <<"-y+1/2,-z,-x+1/2"
                     <<"y+3/4,-z,x+3/4"
                     <<"-y+1/2,z+1/4,x+3/4"
                     <<"y+3/4,z+1/4,-x+1/2"
                     <<"x+1/2,y+1/2,z"
                     <<"-x+1/4,-y+1/4,z"
                     <<"-x+1/4,y+1/2,-z+3/4"
                     <<"x+1/2,-y+1/4,-z+3/4"
                     <<"z+1/2,x+1/2,y"
                     <<"z+1/2,-x+1/4,-y+3/4"
                     <<"-z+1/4,-x+1/4,y"
                     <<"-z+1/4,x+1/2,-y+3/4"
                     <<"y+1/2,z+1/2,x"
                     <<"-y+1/4,z+1/2,-x+3/4"
                     <<"y+1/2,-z+1/4,-x+3/4"
                     <<"-y+1/4,-z+1/4,x"
                     <<"-x+1/2,-y+1/2,-z"
                     <<"x+3/4,y+3/4,-z"
                     <<"x+3/4,-y+1/2,z+1/4"
                     <<"-x+1/2,y+3/4,z+1/4"
                     <<"-z+1/2,-x+1/2,-y"
                     <<"-z+1/2,x+3/4,y+1/4"
                     <<"z+3/4,x+3/4,-y"
                     <<"z+3/4,-x+1/2,y+1/4"
                     <<"-y+1/2,-z+1/2,-x"
                     <<"y+3/4,-z+1/2,x+1/4"
                     <<"-y+1/2,z+3/4,x+1/4"
                     <<"y+3/4,z+3/4,-x";
    spaceGroups[202] = sp;
    //-----------------------
    sp.name = "";
    sp.number = 204;
    sp.symOperations.clear();
    sp.symOperations
                     <<"x,y,z"
                     <<"-x,-y,z"
                     <<"-x,y,-z"
                     <<"x,-y,-z"
                     <<"z,x,y"
                     <<"z,-x,-y"
                     <<"-z,-x,y"
                     <<"-z,x,-y"
                     <<"y,z,x"
                     <<"-y,z,-x"
                     <<"y,-z,-x"
                     <<"-y,-z,x"
                     <<"-x,-y,-z"
                     <<"x,y,-z"
                     <<"x,-y,z"
                     <<"-x,y,z"
                     <<"-z,-x,-y"
                     <<"-z,x,y"
                     <<"z,x,-y"
                     <<"z,-x,y"
                     <<"-y,-z,-x"
                     <<"y,-z,x"
                     <<"-y,z,x"
                     <<"y,z,-x"
                     <<"x+1/2,y+1/2,z+1/2"
                     <<"-x+1/2,-y+1/2,z+1/2"
                     <<"-x+1/2,y+1/2,-z+1/2"
                     <<"x+1/2,-y+1/2,-z+1/2"
                     <<"z+1/2,x+1/2,y+1/2"
                     <<"z+1/2,-x+1/2,-y+1/2"
                     <<"-z+1/2,-x+1/2,y+1/2"
                     <<"-z+1/2,x+1/2,-y+1/2"
                     <<"y+1/2,z+1/2,x+1/2"
                     <<"-y+1/2,z+1/2,-x+1/2"
                     <<"y+1/2,-z+1/2,-x+1/2"
                     <<"-y+1/2,-z+1/2,x+1/2"
                     <<"-x+1/2,-y+1/2,-z+1/2"
                     <<"x+1/2,y+1/2,-z+1/2"
                     <<"x+1/2,-y+1/2,z+1/2"
                     <<"-x+1/2,y+1/2,z+1/2"
                     <<"-z+1/2,-x+1/2,-y+1/2"
                     <<"-z+1/2,x+1/2,y+1/2"
                     <<"z+1/2,x+1/2,-y+1/2"
                     <<"z+1/2,-x+1/2,y+1/2"
                     <<"-y+1/2,-z+1/2,-x+1/2"
                     <<"y+1/2,-z+1/2,x+1/2"
                     <<"-y+1/2,z+1/2,x+1/2"
                     <<"y+1/2,z+1/2,-x+1/2";
    spaceGroups[203] = sp;
    //-----------------------
    sp.name = "";
    sp.number = 205;
    sp.symOperations.clear();
    sp.symOperations
                     <<"x,y,z"
                     <<"-x+1/2,-y,z+1/2"
                     <<"-x,y+1/2,-z+1/2"
                     <<"x+1/2,-y+1/2,-z"
                     <<"z,x,y"
                     <<"z+1/2,-x+1/2,-y"
                     <<"-z+1/2,-x,y+1/2"
                     <<"-z,x+1/2,-y+1/2"
                     <<"y,z,x"
                     <<"-y,z+1/2,-x+1/2"
                     <<"y+1/2,-z+1/2,-x"
                     <<"-y+1/2,-z,x+1/2"
                     <<"-x,-y,-z"
                     <<"x+1/2,y,-z+1/2"
                     <<"x,-y+1/2,z+1/2"
                     <<"-x+1/2,y+1/2,z"
                     <<"-z,-x,-y"
                     <<"-z+1/2,x+1/2,y"
                     <<"z+1/2,x,-y+1/2"
                     <<"z,-x+1/2,y+1/2"
                     <<"-y,-z,-x"
                     <<"y,-z+1/2,x+1/2"
                     <<"-y+1/2,z+1/2,x"
                     <<"y+1/2,z,-x+1/2";
    spaceGroups[204] = sp;
    //-----------------------
    sp.name = "";
    sp.number = 206;
    sp.symOperations.clear();
    sp.symOperations
                     <<"x,y,z"
                     <<"-x+1/2,-y,z+1/2"
                     <<"-x,y+1/2,-z+1/2"
                     <<"x+1/2,-y+1/2,-z"
                     <<"z,x,y"
                     <<"z+1/2,-x+1/2,-y"
                     <<"-z+1/2,-x,y+1/2"
                     <<"-z,x+1/2,-y+1/2"
                     <<"y,z,x"
                     <<"-y,z+1/2,-x+1/2"
                     <<"y+1/2,-z+1/2,-x"
                     <<"-y+1/2,-z,x+1/2"
                     <<"-x,-y,-z"
                     <<"x+1/2,y,-z+1/2"
                     <<"x,-y+1/2,z+1/2"
                     <<"-x+1/2,y+1/2,z"
                     <<"-z,-x,-y"
                     <<"-z+1/2,x+1/2,y"
                     <<"z+1/2,x,-y+1/2"
                     <<"z,-x+1/2,y+1/2"
                     <<"-y,-z,-x"
                     <<"y,-z+1/2,x+1/2"
                     <<"-y+1/2,z+1/2,x"
                     <<"y+1/2,z,-x+1/2"
                     <<"x+1/2,y+1/2,z+1/2"
                     <<"-x,-y+1/2,z"
                     <<"-x+1/2,y,-z"
                     <<"x,-y,-z+1/2"
                     <<"z+1/2,x+1/2,y+1/2"
                     <<"z,-x,-y+1/2"
                     <<"-z,-x+1/2,y"
                     <<"-z+1/2,x,-y"
                     <<"y+1/2,z+1/2,x+1/2"
                     <<"-y+1/2,z,-x"
                     <<"y,-z,-x+1/2"
                     <<"-y,-z+1/2,x"
                     <<"-x+1/2,-y+1/2,-z+1/2"
                     <<"x,y+1/2,-z"
                     <<"x+1/2,-y,z"
                     <<"-x,y,z+1/2"
                     <<"-z+1/2,-x+1/2,-y+1/2"
                     <<"-z,x,y+1/2"
                     <<"z,x+1/2,-y"
                     <<"z+1/2,-x,y"
                     <<"-y+1/2,-z+1/2,-x+1/2"
                     <<"y+1/2,-z,x"
                     <<"-y,z,x+1/2"
                     <<"y,z+1/2,-x";
    spaceGroups[205] = sp;
    //-----------------------
    sp.name = "";
    sp.number = 207;
    sp.symOperations.clear();
    sp.symOperations
                     <<"x,y,z"
                     <<"-x,-y,z"
                     <<"-x,y,-z"
                     <<"x,-y,-z"
                     <<"z,x,y"
                     <<"z,-x,-y"
                     <<"-z,-x,y"
                     <<"-z,x,-y"
                     <<"y,z,x"
                     <<"-y,z,-x"
                     <<"y,-z,-x"
                     <<"-y,-z,x"
                     <<"y,x,-z"
                     <<"-y,-x,-z"
                     <<"y,-x,z"
                     <<"-y,x,z"
                     <<"x,z,-y"
                     <<"-x,z,y"
                     <<"-x,-z,-y"
                     <<"x,-z,y"
                     <<"z,y,-x"
                     <<"z,-y,x"
                     <<"-z,y,x"
                     <<"-z,-y,-x";
    spaceGroups[206] = sp;
    //-----------------------
    sp.name = "";
    sp.number = 208;
    sp.symOperations.clear();
    sp.symOperations
                     <<"x,y,z"
                     <<"-x,-y,z"
                     <<"-x,y,-z"
                     <<"x,-y,-z"
                     <<"z,x,y"
                     <<"z,-x,-y"
                     <<"-z,-x,y"
                     <<"-z,x,-y"
                     <<"y,z,x"
                     <<"-y,z,-x"
                     <<"y,-z,-x"
                     <<"-y,-z,x"
                     <<"y+1/2,x+1/2,-z+1/2"
                     <<"-y+1/2,-x+1/2,-z+1/2"
                     <<"y+1/2,-x+1/2,z+1/2"
                     <<"-y+1/2,x+1/2,z+1/2"
                     <<"x+1/2,z+1/2,-y+1/2"
                     <<"-x+1/2,z+1/2,y+1/2"
                     <<"-x+1/2,-z+1/2,-y+1/2"
                     <<"x+1/2,-z+1/2,y+1/2"
                     <<"z+1/2,y+1/2,-x+1/2"
                     <<"z+1/2,-y+1/2,x+1/2"
                     <<"-z+1/2,y+1/2,x+1/2"
                     <<"-z+1/2,-y+1/2,-x+1/2";
    spaceGroups[207] = sp;
    //-----------------------
    sp.name = "";
    sp.number = 209;
    sp.symOperations.clear();
    sp.symOperations
                     <<"x,y,z"
                     <<"-x,-y,z"
                     <<"-x,y,-z"
                     <<"x,-y,-z"
                     <<"z,x,y"
                     <<"z,-x,-y"
                     <<"-z,-x,y"
                     <<"-z,x,-y"
                     <<"y,z,x"
                     <<"-y,z,-x"
                     <<"y,-z,-x"
                     <<"-y,-z,x"
                     <<"y,x,-z"
                     <<"-y,-x,-z"
                     <<"y,-x,z"
                     <<"-y,x,z"
                     <<"x,z,-y"
                     <<"-x,z,y"
                     <<"-x,-z,-y"
                     <<"x,-z,y"
                     <<"z,y,-x"
                     <<"z,-y,x"
                     <<"-z,y,x"
                     <<"-z,-y,-x"
                     <<"x,y+1/2,z+1/2"
                     <<"-x,-y+1/2,z+1/2"
                     <<"-x,y+1/2,-z+1/2"
                     <<"x,-y+1/2,-z+1/2"
                     <<"z,x+1/2,y+1/2"
                     <<"z,-x+1/2,-y+1/2"
                     <<"-z,-x+1/2,y+1/2"
                     <<"-z,x+1/2,-y+1/2"
                     <<"y,z+1/2,x+1/2"
                     <<"-y,z+1/2,-x+1/2"
                     <<"y,-z+1/2,-x+1/2"
                     <<"-y,-z+1/2,x+1/2"
                     <<"y,x+1/2,-z+1/2"
                     <<"-y,-x+1/2,-z+1/2"
                     <<"y,-x+1/2,z+1/2"
                     <<"-y,x+1/2,z+1/2"
                     <<"x,z+1/2,-y+1/2"
                     <<"-x,z+1/2,y+1/2"
                     <<"-x,-z+1/2,-y+1/2"
                     <<"x,-z+1/2,y+1/2"
                     <<"z,y+1/2,-x+1/2"
                     <<"z,-y+1/2,x+1/2"
                     <<"-z,y+1/2,x+1/2"
                     <<"-z,-y+1/2,-x+1/2"
                     <<"x+1/2,y,z+1/2"
                     <<"-x+1/2,-y,z+1/2"
                     <<"-x+1/2,y,-z+1/2"
                     <<"x+1/2,-y,-z+1/2"
                     <<"z+1/2,x,y+1/2"
                     <<"z+1/2,-x,-y+1/2"
                     <<"-z+1/2,-x,y+1/2"
                     <<"-z+1/2,x,-y+1/2"
                     <<"y+1/2,z,x+1/2"
                     <<"-y+1/2,z,-x+1/2"
                     <<"y+1/2,-z,-x+1/2"
                     <<"-y+1/2,-z,x+1/2"
                     <<"y+1/2,x,-z+1/2"
                     <<"-y+1/2,-x,-z+1/2"
                     <<"y+1/2,-x,z+1/2"
                     <<"-y+1/2,x,z+1/2"
                     <<"x+1/2,z,-y+1/2"
                     <<"-x+1/2,z,y+1/2"
                     <<"-x+1/2,-z,-y+1/2"
                     <<"x+1/2,-z,y+1/2"
                     <<"z+1/2,y,-x+1/2"
                     <<"z+1/2,-y,x+1/2"
                     <<"-z+1/2,y,x+1/2"
                     <<"-z+1/2,-y,-x+1/2"
                     <<"x+1/2,y+1/2,z"
                     <<"-x+1/2,-y+1/2,z"
                     <<"-x+1/2,y+1/2,-z"
                     <<"x+1/2,-y+1/2,-z"
                     <<"z+1/2,x+1/2,y"
                     <<"z+1/2,-x+1/2,-y"
                     <<"-z+1/2,-x+1/2,y"
                     <<"-z+1/2,x+1/2,-y"
                     <<"y+1/2,z+1/2,x"
                     <<"-y+1/2,z+1/2,-x"
                     <<"y+1/2,-z+1/2,-x"
                     <<"-y+1/2,-z+1/2,x"
                     <<"y+1/2,x+1/2,-z"
                     <<"-y+1/2,-x+1/2,-z"
                     <<"y+1/2,-x+1/2,z"
                     <<"-y+1/2,x+1/2,z"
                     <<"x+1/2,z+1/2,-y"
                     <<"-x+1/2,z+1/2,y"
                     <<"-x+1/2,-z+1/2,-y"
                     <<"x+1/2,-z+1/2,y"
                     <<"z+1/2,y+1/2,-x"
                     <<"z+1/2,-y+1/2,x"
                     <<"-z+1/2,y+1/2,x"
                     <<"-z+1/2,-y+1/2,-x";
    spaceGroups[208] = sp;
    //-----------------------
    sp.name = "";
    sp.number = 210;
    sp.symOperations.clear();
    sp.symOperations
                     <<"x,y,z"
                     <<"-x,-y+1/2,z+1/2"
                     <<"-x+1/2,y+1/2,-z"
                     <<"x+1/2,-y,-z+1/2"
                     <<"z,x,y"
                     <<"z+1/2,-x,-y+1/2"
                     <<"-z,-x+1/2,y+1/2"
                     <<"-z+1/2,x+1/2,-y"
                     <<"y,z,x"
                     <<"-y+1/2,z+1/2,-x"
                     <<"y+1/2,-z,-x+1/2"
                     <<"-y,-z+1/2,x+1/2"
                     <<"y+3/4,x+1/4,-z+3/4"
                     <<"-y+1/4,-x+1/4,-z+1/4"
                     <<"y+1/4,-x+3/4,z+3/4"
                     <<"-y+3/4,x+3/4,z+1/4"
                     <<"x+3/4,z+1/4,-y+3/4"
                     <<"-x+3/4,z+3/4,y+1/4"
                     <<"-x+1/4,-z+1/4,-y+1/4"
                     <<"x+1/4,-z+3/4,y+3/4"
                     <<"z+3/4,y+1/4,-x+3/4"
                     <<"z+1/4,-y+3/4,x+3/4"
                     <<"-z+3/4,y+3/4,x+1/4"
                     <<"-z+1/4,-y+1/4,-x+1/4"
                     <<"x,y+1/2,z+1/2"
                     <<"-x,-y,z"
                     <<"-x+1/2,y,-z+1/2"
                     <<"x+1/2,-y+1/2,-z"
                     <<"z,x+1/2,y+1/2"
                     <<"z+1/2,-x+1/2,-y"
                     <<"-z,-x,y"
                     <<"-z+1/2,x,-y+1/2"
                     <<"y,z+1/2,x+1/2"
                     <<"-y+1/2,z,-x+1/2"
                     <<"y+1/2,-z+1/2,-x"
                     <<"-y,-z,x"
                     <<"y+3/4,x+3/4,-z+1/4"
                     <<"-y+1/4,-x+3/4,-z+3/4"
                     <<"y+1/4,-x+1/4,z+1/4"
                     <<"-y+3/4,x+1/4,z+3/4"
                     <<"x+3/4,z+3/4,-y+1/4"
                     <<"-x+3/4,z+1/4,y+3/4"
                     <<"-x+1/4,-z+3/4,-y+3/4"
                     <<"x+1/4,-z+1/4,y+1/4"
                     <<"z+3/4,y+3/4,-x+1/4"
                     <<"z+1/4,-y+1/4,x+1/4"
                     <<"-z+3/4,y+1/4,x+3/4"
                     <<"-z+1/4,-y+3/4,-x+3/4"
                     <<"x+1/2,y,z+1/2"
                     <<"-x+1/2,-y+1/2,z"
                     <<"-x,y+1/2,-z+1/2"
                     <<"x,-y,-z"
                     <<"z+1/2,x,y+1/2"
                     <<"z,-x,-y"
                     <<"-z+1/2,-x+1/2,y"
                     <<"-z,x+1/2,-y+1/2"
                     <<"y+1/2,z,x+1/2"
                     <<"-y,z+1/2,-x+1/2"
                     <<"y,-z,-x"
                     <<"-y+1/2,-z+1/2,x"
                     <<"y+1/4,x+1/4,-z+1/4"
                     <<"-y+3/4,-x+1/4,-z+3/4"
                     <<"y+3/4,-x+3/4,z+1/4"
                     <<"-y+1/4,x+3/4,z+3/4"
                     <<"x+1/4,z+1/4,-y+1/4"
                     <<"-x+1/4,z+3/4,y+3/4"
                     <<"-x+3/4,-z+1/4,-y+3/4"
                     <<"x+3/4,-z+3/4,y+1/4"
                     <<"z+1/4,y+1/4,-x+1/4"
                     <<"z+3/4,-y+3/4,x+1/4"
                     <<"-z+1/4,y+3/4,x+3/4"
                     <<"-z+3/4,-y+1/4,-x+3/4"
                     <<"x+1/2,y+1/2,z"
                     <<"-x+1/2,-y,z+1/2"
                     <<"-x,y,-z"
                     <<"x,-y+1/2,-z+1/2"
                     <<"z+1/2,x+1/2,y"
                     <<"z,-x+1/2,-y+1/2"
                     <<"-z+1/2,-x,y+1/2"
                     <<"-z,x,-y"
                     <<"y+1/2,z+1/2,x"
                     <<"-y,z,-x"
                     <<"y,-z+1/2,-x+1/2"
                     <<"-y+1/2,-z,x+1/2"
                     <<"y+1/4,x+3/4,-z+3/4"
                     <<"-y+3/4,-x+3/4,-z+1/4"
                     <<"y+3/4,-x+1/4,z+3/4"
                     <<"-y+1/4,x+1/4,z+1/4"
                     <<"x+1/4,z+3/4,-y+3/4"
                     <<"-x+1/4,z+1/4,y+1/4"
                     <<"-x+3/4,-z+3/4,-y+1/4"
                     <<"x+3/4,-z+1/4,y+3/4"
                     <<"z+1/4,y+3/4,-x+3/4"
                     <<"z+3/4,-y+1/4,x+3/4"
                     <<"-z+1/4,y+1/4,x+1/4"
                     <<"-z+3/4,-y+3/4,-x+1/4";
    spaceGroups[209] = sp;
    //-----------------------
    sp.name = "";
    sp.number = 211;
    sp.symOperations.clear();
    sp.symOperations
                     <<"x,y,z"
                     <<"-x,-y,z"
                     <<"-x,y,-z"
                     <<"x,-y,-z"
                     <<"z,x,y"
                     <<"z,-x,-y"
                     <<"-z,-x,y"
                     <<"-z,x,-y"
                     <<"y,z,x"
                     <<"-y,z,-x"
                     <<"y,-z,-x"
                     <<"-y,-z,x"
                     <<"y,x,-z"
                     <<"-y,-x,-z"
                     <<"y,-x,z"
                     <<"-y,x,z"
                     <<"x,z,-y"
                     <<"-x,z,y"
                     <<"-x,-z,-y"
                     <<"x,-z,y"
                     <<"z,y,-x"
                     <<"z,-y,x"
                     <<"-z,y,x"
                     <<"-z,-y,-x"
                     <<"x+1/2,y+1/2,z+1/2"
                     <<"-x+1/2,-y+1/2,z+1/2"
                     <<"-x+1/2,y+1/2,-z+1/2"
                     <<"x+1/2,-y+1/2,-z+1/2"
                     <<"z+1/2,x+1/2,y+1/2"
                     <<"z+1/2,-x+1/2,-y+1/2"
                     <<"-z+1/2,-x+1/2,y+1/2"
                     <<"-z+1/2,x+1/2,-y+1/2"
                     <<"y+1/2,z+1/2,x+1/2"
                     <<"-y+1/2,z+1/2,-x+1/2"
                     <<"y+1/2,-z+1/2,-x+1/2"
                     <<"-y+1/2,-z+1/2,x+1/2"
                     <<"y+1/2,x+1/2,-z+1/2"
                     <<"-y+1/2,-x+1/2,-z+1/2"
                     <<"y+1/2,-x+1/2,z+1/2"
                     <<"-y+1/2,x+1/2,z+1/2"
                     <<"x+1/2,z+1/2,-y+1/2"
                     <<"-x+1/2,z+1/2,y+1/2"
                     <<"-x+1/2,-z+1/2,-y+1/2"
                     <<"x+1/2,-z+1/2,y+1/2"
                     <<"z+1/2,y+1/2,-x+1/2"
                     <<"z+1/2,-y+1/2,x+1/2"
                     <<"-z+1/2,y+1/2,x+1/2"
                     <<"-z+1/2,-y+1/2,-x+1/2";
    spaceGroups[210] = sp;
    //-----------------------
    sp.name = "";
    sp.number = 212;
    sp.symOperations.clear();
    sp.symOperations
                     <<"x,y,z"
                     <<"-x+1/2,-y,z+1/2"
                     <<"-x,y+1/2,-z+1/2"
                     <<"x+1/2,-y+1/2,-z"
                     <<"z,x,y"
                     <<"z+1/2,-x+1/2,-y"
                     <<"-z+1/2,-x,y+1/2"
                     <<"-z,x+1/2,-y+1/2"
                     <<"y,z,x"
                     <<"-y,z+1/2,-x+1/2"
                     <<"y+1/2,-z+1/2,-x"
                     <<"-y+1/2,-z,x+1/2"
                     <<"y+1/4,x+3/4,-z+3/4"
                     <<"-y+1/4,-x+1/4,-z+1/4"
                     <<"y+3/4,-x+3/4,z+1/4"
                     <<"-y+3/4,x+1/4,z+3/4"
                     <<"x+1/4,z+3/4,-y+3/4"
                     <<"-x+3/4,z+1/4,y+3/4"
                     <<"-x+1/4,-z+1/4,-y+1/4"
                     <<"x+3/4,-z+3/4,y+1/4"
                     <<"z+1/4,y+3/4,-x+3/4"
                     <<"z+3/4,-y+3/4,x+1/4"
                     <<"-z+3/4,y+1/4,x+3/4"
                     <<"-z+1/4,-y+1/4,-x+1/4";
    spaceGroups[211] = sp;
    //-----------------------
    sp.name = "";
    sp.number = 213;
    sp.symOperations.clear();
    sp.symOperations
                     <<"x,y,z"
                     <<"-x+1/2,-y,z+1/2"
                     <<"-x,y+1/2,-z+1/2"
                     <<"x+1/2,-y+1/2,-z"
                     <<"z,x,y"
                     <<"z+1/2,-x+1/2,-y"
                     <<"-z+1/2,-x,y+1/2"
                     <<"-z,x+1/2,-y+1/2"
                     <<"y,z,x"
                     <<"-y,z+1/2,-x+1/2"
                     <<"y+1/2,-z+1/2,-x"
                     <<"-y+1/2,-z,x+1/2"
                     <<"y+3/4,x+1/4,-z+1/4"
                     <<"-y+3/4,-x+3/4,-z+3/4"
                     <<"y+1/4,-x+1/4,z+3/4"
                     <<"-y+1/4,x+3/4,z+1/4"
                     <<"x+3/4,z+1/4,-y+1/4"
                     <<"-x+1/4,z+3/4,y+1/4"
                     <<"-x+3/4,-z+3/4,-y+3/4"
                     <<"x+1/4,-z+1/4,y+3/4"
                     <<"z+3/4,y+1/4,-x+1/4"
                     <<"z+1/4,-y+1/4,x+3/4"
                     <<"-z+1/4,y+3/4,x+1/4"
                     <<"-z+3/4,-y+3/4,-x+3/4";
    spaceGroups[212] = sp;
    //-----------------------
    sp.name = "";
    sp.number = 214;
    sp.symOperations.clear();
    sp.symOperations
                     <<"x,y,z"
                     <<"-x+1/2,-y,z+1/2"
                     <<"-x,y+1/2,-z+1/2"
                     <<"x+1/2,-y+1/2,-z"
                     <<"z,x,y"
                     <<"z+1/2,-x+1/2,-y"
                     <<"-z+1/2,-x,y+1/2"
                     <<"-z,x+1/2,-y+1/2"
                     <<"y,z,x"
                     <<"-y,z+1/2,-x+1/2"
                     <<"y+1/2,-z+1/2,-x"
                     <<"-y+1/2,-z,x+1/2"
                     <<"y+3/4,x+1/4,-z+1/4"
                     <<"-y+3/4,-x+3/4,-z+3/4"
                     <<"y+1/4,-x+1/4,z+3/4"
                     <<"-y+1/4,x+3/4,z+1/4"
                     <<"x+3/4,z+1/4,-y+1/4"
                     <<"-x+1/4,z+3/4,y+1/4"
                     <<"-x+3/4,-z+3/4,-y+3/4"
                     <<"x+1/4,-z+1/4,y+3/4"
                     <<"z+3/4,y+1/4,-x+1/4"
                     <<"z+1/4,-y+1/4,x+3/4"
                     <<"-z+1/4,y+3/4,x+1/4"
                     <<"-z+3/4,-y+3/4,-x+3/4"
                     <<"x+1/2,y+1/2,z+1/2"
                     <<"-x,-y+1/2,z"
                     <<"-x+1/2,y,-z"
                     <<"x,-y,-z+1/2"
                     <<"z+1/2,x+1/2,y+1/2"
                     <<"z,-x,-y+1/2"
                     <<"-z,-x+1/2,y"
                     <<"-z+1/2,x,-y"
                     <<"y+1/2,z+1/2,x+1/2"
                     <<"-y+1/2,z,-x"
                     <<"y,-z,-x+1/2"
                     <<"-y,-z+1/2,x"
                     <<"y+1/4,x+3/4,-z+3/4"
                     <<"-y+1/4,-x+1/4,-z+1/4"
                     <<"y+3/4,-x+3/4,z+1/4"
                     <<"-y+3/4,x+1/4,z+3/4"
                     <<"x+1/4,z+3/4,-y+3/4"
                     <<"-x+3/4,z+1/4,y+3/4"
                     <<"-x+1/4,-z+1/4,-y+1/4"
                     <<"x+3/4,-z+3/4,y+1/4"
                     <<"z+1/4,y+3/4,-x+3/4"
                     <<"z+3/4,-y+3/4,x+1/4"
                     <<"-z+3/4,y+1/4,x+3/4"
                     <<"-z+1/4,-y+1/4,-x+1/4";
    spaceGroups[213] = sp;
    //-----------------------
    sp.name = "";
    sp.number = 215;
    sp.symOperations.clear();
    sp.symOperations
                     <<"x,y,z"
                     <<"-x,-y,z"
                     <<"-x,y,-z"
                     <<"x,-y,-z"
                     <<"z,x,y"
                     <<"z,-x,-y"
                     <<"-z,-x,y"
                     <<"-z,x,-y"
                     <<"y,z,x"
                     <<"-y,z,-x"
                     <<"y,-z,-x"
                     <<"-y,-z,x"
                     <<"y,x,z"
                     <<"-y,-x,z"
                     <<"y,-x,-z"
                     <<"-y,x,-z"
                     <<"x,z,y"
                     <<"-x,z,-y"
                     <<"-x,-z,y"
                     <<"x,-z,-y"
                     <<"z,y,x"
                     <<"z,-y,-x"
                     <<"-z,y,-x"
                     <<"-z,-y,x";
    spaceGroups[214] = sp;
    //-----------------------
    sp.name = "";
    sp.number = 216;
    sp.symOperations.clear();
    sp.symOperations
                     <<"x,y,z"
                     <<"-x,-y,z"
                     <<"-x,y,-z"
                     <<"x,-y,-z"
                     <<"z,x,y"
                     <<"z,-x,-y"
                     <<"-z,-x,y"
                     <<"-z,x,-y"
                     <<"y,z,x"
                     <<"-y,z,-x"
                     <<"y,-z,-x"
                     <<"-y,-z,x"
                     <<"y,x,z"
                     <<"-y,-x,z"
                     <<"y,-x,-z"
                     <<"-y,x,-z"
                     <<"x,z,y"
                     <<"-x,z,-y"
                     <<"-x,-z,y"
                     <<"x,-z,-y"
                     <<"z,y,x"
                     <<"z,-y,-x"
                     <<"-z,y,-x"
                     <<"-z,-y,x"
                     <<"x,y+1/2,z+1/2"
                     <<"-x,-y+1/2,z+1/2"
                     <<"-x,y+1/2,-z+1/2"
                     <<"x,-y+1/2,-z+1/2"
                     <<"z,x+1/2,y+1/2"
                     <<"z,-x+1/2,-y+1/2"
                     <<"-z,-x+1/2,y+1/2"
                     <<"-z,x+1/2,-y+1/2"
                     <<"y,z+1/2,x+1/2"
                     <<"-y,z+1/2,-x+1/2"
                     <<"y,-z+1/2,-x+1/2"
                     <<"-y,-z+1/2,x+1/2"
                     <<"y,x+1/2,z+1/2"
                     <<"-y,-x+1/2,z+1/2"
                     <<"y,-x+1/2,-z+1/2"
                     <<"-y,x+1/2,-z+1/2"
                     <<"x,z+1/2,y+1/2"
                     <<"-x,z+1/2,-y+1/2"
                     <<"-x,-z+1/2,y+1/2"
                     <<"x,-z+1/2,-y+1/2"
                     <<"z,y+1/2,x+1/2"
                     <<"z,-y+1/2,-x+1/2"
                     <<"-z,y+1/2,-x+1/2"
                     <<"-z,-y+1/2,x+1/2"
                     <<"x+1/2,y,z+1/2"
                     <<"-x+1/2,-y,z+1/2"
                     <<"-x+1/2,y,-z+1/2"
                     <<"x+1/2,-y,-z+1/2"
                     <<"z+1/2,x,y+1/2"
                     <<"z+1/2,-x,-y+1/2"
                     <<"-z+1/2,-x,y+1/2"
                     <<"-z+1/2,x,-y+1/2"
                     <<"y+1/2,z,x+1/2"
                     <<"-y+1/2,z,-x+1/2"
                     <<"y+1/2,-z,-x+1/2"
                     <<"-y+1/2,-z,x+1/2"
                     <<"y+1/2,x,z+1/2"
                     <<"-y+1/2,-x,z+1/2"
                     <<"y+1/2,-x,-z+1/2"
                     <<"-y+1/2,x,-z+1/2"
                     <<"x+1/2,z,y+1/2"
                     <<"-x+1/2,z,-y+1/2"
                     <<"-x+1/2,-z,y+1/2"
                     <<"x+1/2,-z,-y+1/2"
                     <<"z+1/2,y,x+1/2"
                     <<"z+1/2,-y,-x+1/2"
                     <<"-z+1/2,y,-x+1/2"
                     <<"-z+1/2,-y,x+1/2"
                     <<"x+1/2,y+1/2,z"
                     <<"-x+1/2,-y+1/2,z"
                     <<"-x+1/2,y+1/2,-z"
                     <<"x+1/2,-y+1/2,-z"
                     <<"z+1/2,x+1/2,y"
                     <<"z+1/2,-x+1/2,-y"
                     <<"-z+1/2,-x+1/2,y"
                     <<"-z+1/2,x+1/2,-y"
                     <<"y+1/2,z+1/2,x"
                     <<"-y+1/2,z+1/2,-x"
                     <<"y+1/2,-z+1/2,-x"
                     <<"-y+1/2,-z+1/2,x"
                     <<"y+1/2,x+1/2,z"
                     <<"-y+1/2,-x+1/2,z"
                     <<"y+1/2,-x+1/2,-z"
                     <<"-y+1/2,x+1/2,-z"
                     <<"x+1/2,z+1/2,y"
                     <<"-x+1/2,z+1/2,-y"
                     <<"-x+1/2,-z+1/2,y"
                     <<"x+1/2,-z+1/2,-y"
                     <<"z+1/2,y+1/2,x"
                     <<"z+1/2,-y+1/2,-x"
                     <<"-z+1/2,y+1/2,-x"
                     <<"-z+1/2,-y+1/2,x";
    spaceGroups[215] = sp;
    //-----------------------
    sp.name = "";
    sp.number = 217;
    sp.symOperations.clear();
    sp.symOperations
                     <<"x,y,z"
                     <<"-x,-y,z"
                     <<"-x,y,-z"
                     <<"x,-y,-z"
                     <<"z,x,y"
                     <<"z,-x,-y"
                     <<"-z,-x,y"
                     <<"-z,x,-y"
                     <<"y,z,x"
                     <<"-y,z,-x"
                     <<"y,-z,-x"
                     <<"-y,-z,x"
                     <<"y,x,z"
                     <<"-y,-x,z"
                     <<"y,-x,-z"
                     <<"-y,x,-z"
                     <<"x,z,y"
                     <<"-x,z,-y"
                     <<"-x,-z,y"
                     <<"x,-z,-y"
                     <<"z,y,x"
                     <<"z,-y,-x"
                     <<"-z,y,-x"
                     <<"-z,-y,x"
                     <<"x+1/2,y+1/2,z+1/2"
                     <<"-x+1/2,-y+1/2,z+1/2"
                     <<"-x+1/2,y+1/2,-z+1/2"
                     <<"x+1/2,-y+1/2,-z+1/2"
                     <<"z+1/2,x+1/2,y+1/2"
                     <<"z+1/2,-x+1/2,-y+1/2"
                     <<"-z+1/2,-x+1/2,y+1/2"
                     <<"-z+1/2,x+1/2,-y+1/2"
                     <<"y+1/2,z+1/2,x+1/2"
                     <<"-y+1/2,z+1/2,-x+1/2"
                     <<"y+1/2,-z+1/2,-x+1/2"
                     <<"-y+1/2,-z+1/2,x+1/2"
                     <<"y+1/2,x+1/2,z+1/2"
                     <<"-y+1/2,-x+1/2,z+1/2"
                     <<"y+1/2,-x+1/2,-z+1/2"
                     <<"-y+1/2,x+1/2,-z+1/2"
                     <<"x+1/2,z+1/2,y+1/2"
                     <<"-x+1/2,z+1/2,-y+1/2"
                     <<"-x+1/2,-z+1/2,y+1/2"
                     <<"x+1/2,-z+1/2,-y+1/2"
                     <<"z+1/2,y+1/2,x+1/2"
                     <<"z+1/2,-y+1/2,-x+1/2"
                     <<"-z+1/2,y+1/2,-x+1/2"
                     <<"-z+1/2,-y+1/2,x+1/2";
    spaceGroups[216] = sp;
    //-----------------------
    sp.name = "";
    sp.number = 218;
    sp.symOperations.clear();
    sp.symOperations
                     <<"x,y,z"
                     <<"-x,-y,z"
                     <<"-x,y,-z"
                     <<"x,-y,-z"
                     <<"z,x,y"
                     <<"z,-x,-y"
                     <<"-z,-x,y"
                     <<"-z,x,-y"
                     <<"y,z,x"
                     <<"-y,z,-x"
                     <<"y,-z,-x"
                     <<"-y,-z,x"
                     <<"y+1/2,x+1/2,z+1/2"
                     <<"-y+1/2,-x+1/2,z+1/2"
                     <<"y+1/2,-x+1/2,-z+1/2"
                     <<"-y+1/2,x+1/2,-z+1/2"
                     <<"x+1/2,z+1/2,y+1/2"
                     <<"-x+1/2,z+1/2,-y+1/2"
                     <<"-x+1/2,-z+1/2,y+1/2"
                     <<"x+1/2,-z+1/2,-y+1/2"
                     <<"z+1/2,y+1/2,x+1/2"
                     <<"z+1/2,-y+1/2,-x+1/2"
                     <<"-z+1/2,y+1/2,-x+1/2"
                     <<"-z+1/2,-y+1/2,x+1/2";
    spaceGroups[217] = sp;
    //-----------------------
    sp.name = "";
    sp.number = 219;
    sp.symOperations.clear();
    sp.symOperations
                     <<"x,y,z"
                     <<"-x,-y,z"
                     <<"-x,y,-z"
                     <<"x,-y,-z"
                     <<"z,x,y"
                     <<"z,-x,-y"
                     <<"-z,-x,y"
                     <<"-z,x,-y"
                     <<"y,z,x"
                     <<"-y,z,-x"
                     <<"y,-z,-x"
                     <<"-y,-z,x"
                     <<"y+1/2,x+1/2,z+1/2"
                     <<"-y+1/2,-x+1/2,z+1/2"
                     <<"y+1/2,-x+1/2,-z+1/2"
                     <<"-y+1/2,x+1/2,-z+1/2"
                     <<"x+1/2,z+1/2,y+1/2"
                     <<"-x+1/2,z+1/2,-y+1/2"
                     <<"-x+1/2,-z+1/2,y+1/2"
                     <<"x+1/2,-z+1/2,-y+1/2"
                     <<"z+1/2,y+1/2,x+1/2"
                     <<"z+1/2,-y+1/2,-x+1/2"
                     <<"-z+1/2,y+1/2,-x+1/2"
                     <<"-z+1/2,-y+1/2,x+1/2"
                     <<"x,y+1/2,z+1/2"
                     <<"-x,-y+1/2,z+1/2"
                     <<"-x,y+1/2,-z+1/2"
                     <<"x,-y+1/2,-z+1/2"
                     <<"z,x+1/2,y+1/2"
                     <<"z,-x+1/2,-y+1/2"
                     <<"-z,-x+1/2,y+1/2"
                     <<"-z,x+1/2,-y+1/2"
                     <<"y,z+1/2,x+1/2"
                     <<"-y,z+1/2,-x+1/2"
                     <<"y,-z+1/2,-x+1/2"
                     <<"-y,-z+1/2,x+1/2"
                     <<"y+1/2,x,z"
                     <<"-y+1/2,-x,z"
                     <<"y+1/2,-x,-z"
                     <<"-y+1/2,x,-z"
                     <<"x+1/2,z,y"
                     <<"-x+1/2,z,-y"
                     <<"-x+1/2,-z,y"
                     <<"x+1/2,-z,-y"
                     <<"z+1/2,y,x"
                     <<"z+1/2,-y,-x"
                     <<"-z+1/2,y,-x"
                     <<"-z+1/2,-y,x"
                     <<"x+1/2,y,z+1/2"
                     <<"-x+1/2,-y,z+1/2"
                     <<"-x+1/2,y,-z+1/2"
                     <<"x+1/2,-y,-z+1/2"
                     <<"z+1/2,x,y+1/2"
                     <<"z+1/2,-x,-y+1/2"
                     <<"-z+1/2,-x,y+1/2"
                     <<"-z+1/2,x,-y+1/2"
                     <<"y+1/2,z,x+1/2"
                     <<"-y+1/2,z,-x+1/2"
                     <<"y+1/2,-z,-x+1/2"
                     <<"-y+1/2,-z,x+1/2"
                     <<"y,x+1/2,z"
                     <<"-y,-x+1/2,z"
                     <<"y,-x+1/2,-z"
                     <<"-y,x+1/2,-z"
                     <<"x,z+1/2,y"
                     <<"-x,z+1/2,-y"
                     <<"-x,-z+1/2,y"
                     <<"x,-z+1/2,-y"
                     <<"z,y+1/2,x"
                     <<"z,-y+1/2,-x"
                     <<"-z,y+1/2,-x"
                     <<"-z,-y+1/2,x"
                     <<"x+1/2,y+1/2,z"
                     <<"-x+1/2,-y+1/2,z"
                     <<"-x+1/2,y+1/2,-z"
                     <<"x+1/2,-y+1/2,-z"
                     <<"z+1/2,x+1/2,y"
                     <<"z+1/2,-x+1/2,-y"
                     <<"-z+1/2,-x+1/2,y"
                     <<"-z+1/2,x+1/2,-y"
                     <<"y+1/2,z+1/2,x"
                     <<"-y+1/2,z+1/2,-x"
                     <<"y+1/2,-z+1/2,-x"
                     <<"-y+1/2,-z+1/2,x"
                     <<"y,x,z+1/2"
                     <<"-y,-x,z+1/2"
                     <<"y,-x,-z+1/2"
                     <<"-y,x,-z+1/2"
                     <<"x,z,y+1/2"
                     <<"-x,z,-y+1/2"
                     <<"-x,-z,y+1/2"
                     <<"x,-z,-y+1/2"
                     <<"z,y,x+1/2"
                     <<"z,-y,-x+1/2"
                     <<"-z,y,-x+1/2"
                     <<"-z,-y,x+1/2";
    spaceGroups[218] = sp;
    //-----------------------
    sp.name = "";
    sp.number = 220;
    sp.symOperations.clear();
    sp.symOperations
                     <<"x,y,z"
                     <<"-x+1/2,-y,z+1/2"
                     <<"-x,y+1/2,-z+1/2"
                     <<"x+1/2,-y+1/2,-z"
                     <<"z,x,y"
                     <<"z+1/2,-x+1/2,-y"
                     <<"-z+1/2,-x,y+1/2"
                     <<"-z,x+1/2,-y+1/2"
                     <<"y,z,x"
                     <<"-y,z+1/2,-x+1/2"
                     <<"y+1/2,-z+1/2,-x"
                     <<"-y+1/2,-z,x+1/2"
                     <<"y+1/4,x+1/4,z+1/4"
                     <<"-y+1/4,-x+3/4,z+3/4"
                     <<"y+3/4,-x+1/4,-z+3/4"
                     <<"-y+3/4,x+3/4,-z+1/4"
                     <<"x+1/4,z+1/4,y+1/4"
                     <<"-x+3/4,z+3/4,-y+1/4"
                     <<"-x+1/4,-z+3/4,y+3/4"
                     <<"x+3/4,-z+1/4,-y+3/4"
                     <<"z+1/4,y+1/4,x+1/4"
                     <<"z+3/4,-y+1/4,-x+3/4"
                     <<"-z+3/4,y+3/4,-x+1/4"
                     <<"-z+1/4,-y+3/4,x+3/4"
                     <<"x+1/2,y+1/2,z+1/2"
                     <<"-x,-y+1/2,z"
                     <<"-x+1/2,y,-z"
                     <<"x,-y,-z+1/2"
                     <<"z+1/2,x+1/2,y+1/2"
                     <<"z,-x,-y+1/2"
                     <<"-z,-x+1/2,y"
                     <<"-z+1/2,x,-y"
                     <<"y+1/2,z+1/2,x+1/2"
                     <<"-y+1/2,z,-x"
                     <<"y,-z,-x+1/2"
                     <<"-y,-z+1/2,x"
                     <<"y+3/4,x+3/4,z+3/4"
                     <<"-y+3/4,-x+1/4,z+1/4"
                     <<"y+1/4,-x+3/4,-z+1/4"
                     <<"-y+1/4,x+1/4,-z+3/4"
                     <<"x+3/4,z+3/4,y+3/4"
                     <<"-x+1/4,z+1/4,-y+3/4"
                     <<"-x+3/4,-z+1/4,y+1/4"
                     <<"x+1/4,-z+3/4,-y+1/4"
                     <<"z+3/4,y+3/4,x+3/4"
                     <<"z+1/4,-y+3/4,-x+1/4"
                     <<"-z+1/4,y+1/4,-x+3/4"
                     <<"-z+3/4,-y+1/4,x+1/4";
    spaceGroups[219] = sp;
    //-----------------------
    sp.name = "";
    sp.number = 221;
    sp.symOperations.clear();
    sp.symOperations
                     <<"x,y,z"
                     <<"-x,-y,z"
                     <<"-x,y,-z"
                     <<"x,-y,-z"
                     <<"z,x,y"
                     <<"z,-x,-y"
                     <<"-z,-x,y"
                     <<"-z,x,-y"
                     <<"y,z,x"
                     <<"-y,z,-x"
                     <<"y,-z,-x"
                     <<"-y,-z,x"
                     <<"y,x,-z"
                     <<"-y,-x,-z"
                     <<"y,-x,z"
                     <<"-y,x,z"
                     <<"x,z,-y"
                     <<"-x,z,y"
                     <<"-x,-z,-y"
                     <<"x,-z,y"
                     <<"z,y,-x"
                     <<"z,-y,x"
                     <<"-z,y,x"
                     <<"-z,-y,-x"
                     <<"-x,-y,-z"
                     <<"x,y,-z"
                     <<"x,-y,z"
                     <<"-x,y,z"
                     <<"-z,-x,-y"
                     <<"-z,x,y"
                     <<"z,x,-y"
                     <<"z,-x,y"
                     <<"-y,-z,-x"
                     <<"y,-z,x"
                     <<"-y,z,x"
                     <<"y,z,-x"
                     <<"-y,-x,z"
                     <<"y,x,z"
                     <<"-y,x,-z"
                     <<"y,-x,-z"
                     <<"-x,-z,y"
                     <<"x,-z,-y"
                     <<"x,z,y"
                     <<"-x,z,-y"
                     <<"-z,-y,x"
                     <<"-z,y,-x"
                     <<"z,-y,-x"
                     <<"z,y,x";
    spaceGroups[220] = sp;
    //-----------------------
    sp.name = "";
    sp.number = 222;
    sp.symOperations.clear();
    sp.symOperations
                     <<"x,y,z"
                     <<"-x+1/2,-y+1/2,z"
                     <<"-x+1/2,y,-z+1/2"
                     <<"x,-y+1/2,-z+1/2"
                     <<"z,x,y"
                     <<"z,-x+1/2,-y+1/2"
                     <<"-z+1/2,-x+1/2,y"
                     <<"-z+1/2,x,-y+1/2"
                     <<"y,z,x"
                     <<"-y+1/2,z,-x+1/2"
                     <<"y,-z+1/2,-x+1/2"
                     <<"-y+1/2,-z+1/2,x"
                     <<"y,x,-z+1/2"
                     <<"-y+1/2,-x+1/2,-z+1/2"
                     <<"y,-x+1/2,z"
                     <<"-y+1/2,x,z"
                     <<"x,z,-y+1/2"
                     <<"-x+1/2,z,y"
                     <<"-x+1/2,-z+1/2,-y+1/2"
                     <<"x,-z+1/2,y"
                     <<"z,y,-x+1/2"
                     <<"z,-y+1/2,x"
                     <<"-z+1/2,y,x"
                     <<"-z+1/2,-y+1/2,-x+1/2"
                     <<"-x,-y,-z"
                     <<"x+1/2,y+1/2,-z"
                     <<"x+1/2,-y,z+1/2"
                     <<"-x,y+1/2,z+1/2"
                     <<"-z,-x,-y"
                     <<"-z,x+1/2,y+1/2"
                     <<"z+1/2,x+1/2,-y"
                     <<"z+1/2,-x,y+1/2"
                     <<"-y,-z,-x"
                     <<"y+1/2,-z,x+1/2"
                     <<"-y,z+1/2,x+1/2"
                     <<"y+1/2,z+1/2,-x"
                     <<"-y,-x,z+1/2"
                     <<"y+1/2,x+1/2,z+1/2"
                     <<"-y,x+1/2,-z"
                     <<"y+1/2,-x,-z"
                     <<"-x,-z,y+1/2"
                     <<"x+1/2,-z,-y"
                     <<"x+1/2,z+1/2,y+1/2"
                     <<"-x,z+1/2,-y"
                     <<"-z,-y,x+1/2"
                     <<"-z,y+1/2,-x"
                     <<"z+1/2,-y,-x"
                     <<"z+1/2,y+1/2,x+1/2";
    spaceGroups[221] = sp;
    //-----------------------
    sp.name = "";
    sp.number = 223;
    sp.symOperations.clear();
    sp.symOperations
                     <<"x,y,z"
                     <<"-x,-y,z"
                     <<"-x,y,-z"
                     <<"x,-y,-z"
                     <<"z,x,y"
                     <<"z,-x,-y"
                     <<"-z,-x,y"
                     <<"-z,x,-y"
                     <<"y,z,x"
                     <<"-y,z,-x"
                     <<"y,-z,-x"
                     <<"-y,-z,x"
                     <<"y+1/2,x+1/2,-z+1/2"
                     <<"-y+1/2,-x+1/2,-z+1/2"
                     <<"y+1/2,-x+1/2,z+1/2"
                     <<"-y+1/2,x+1/2,z+1/2"
                     <<"x+1/2,z+1/2,-y+1/2"
                     <<"-x+1/2,z+1/2,y+1/2"
                     <<"-x+1/2,-z+1/2,-y+1/2"
                     <<"x+1/2,-z+1/2,y+1/2"
                     <<"z+1/2,y+1/2,-x+1/2"
                     <<"z+1/2,-y+1/2,x+1/2"
                     <<"-z+1/2,y+1/2,x+1/2"
                     <<"-z+1/2,-y+1/2,-x+1/2"
                     <<"-x,-y,-z"
                     <<"x,y,-z"
                     <<"x,-y,z"
                     <<"-x,y,z"
                     <<"-z,-x,-y"
                     <<"-z,x,y"
                     <<"z,x,-y"
                     <<"z,-x,y"
                     <<"-y,-z,-x"
                     <<"y,-z,x"
                     <<"-y,z,x"
                     <<"y,z,-x"
                     <<"-y+1/2,-x+1/2,z+1/2"
                     <<"y+1/2,x+1/2,z+1/2"
                     <<"-y+1/2,x+1/2,-z+1/2"
                     <<"y+1/2,-x+1/2,-z+1/2"
                     <<"-x+1/2,-z+1/2,y+1/2"
                     <<"x+1/2,-z+1/2,-y+1/2"
                     <<"x+1/2,z+1/2,y+1/2"
                     <<"-x+1/2,z+1/2,-y+1/2"
                     <<"-z+1/2,-y+1/2,x+1/2"
                     <<"-z+1/2,y+1/2,-x+1/2"
                     <<"z+1/2,-y+1/2,-x+1/2"
                     <<"z+1/2,y+1/2,x+1/2";
    spaceGroups[222] = sp;
    //-----------------------
    sp.name = "";
    sp.number = 224;
    sp.symOperations.clear();
    sp.symOperations
                     <<"x,y,z"
                     <<"-x+1/2,-y+1/2,z"
                     <<"-x+1/2,y,-z+1/2"
                     <<"x,-y+1/2,-z+1/2"
                     <<"z,x,y"
                     <<"z,-x+1/2,-y+1/2"
                     <<"-z+1/2,-x+1/2,y"
                     <<"-z+1/2,x,-y+1/2"
                     <<"y,z,x"
                     <<"-y+1/2,z,-x+1/2"
                     <<"y,-z+1/2,-x+1/2"
                     <<"-y+1/2,-z+1/2,x"
                     <<"y+1/2,x+1/2,-z"
                     <<"-y,-x,-z"
                     <<"y+1/2,-x,z+1/2"
                     <<"-y,x+1/2,z+1/2"
                     <<"x+1/2,z+1/2,-y"
                     <<"-x,z+1/2,y+1/2"
                     <<"-x,-z,-y"
                     <<"x+1/2,-z,y+1/2"
                     <<"z+1/2,y+1/2,-x"
                     <<"z+1/2,-y,x+1/2"
                     <<"-z,y+1/2,x+1/2"
                     <<"-z,-y,-x"
                     <<"-x,-y,-z"
                     <<"x+1/2,y+1/2,-z"
                     <<"x+1/2,-y,z+1/2"
                     <<"-x,y+1/2,z+1/2"
                     <<"-z,-x,-y"
                     <<"-z,x+1/2,y+1/2"
                     <<"z+1/2,x+1/2,-y"
                     <<"z+1/2,-x,y+1/2"
                     <<"-y,-z,-x"
                     <<"y+1/2,-z,x+1/2"
                     <<"-y,z+1/2,x+1/2"
                     <<"y+1/2,z+1/2,-x"
                     <<"-y+1/2,-x+1/2,z"
                     <<"y,x,z"
                     <<"-y+1/2,x,-z+1/2"
                     <<"y,-x+1/2,-z+1/2"
                     <<"-x+1/2,-z+1/2,y"
                     <<"x,-z+1/2,-y+1/2"
                     <<"x,z,y"
                     <<"-x+1/2,z,-y+1/2"
                     <<"-z+1/2,-y+1/2,x"
                     <<"-z+1/2,y,-x+1/2"
                     <<"z,-y+1/2,-x+1/2"
                     <<"z,y,x";
    spaceGroups[223] = sp;
    //-----------------------
    sp.name = "";
    sp.number = 225;
    sp.symOperations.clear();
    sp.symOperations
                     <<"x,y,z"
                     <<"-x,-y,z"
                     <<"-x,y,-z"
                     <<"x,-y,-z"
                     <<"z,x,y"
                     <<"z,-x,-y"
                     <<"-z,-x,y"
                     <<"-z,x,-y"
                     <<"y,z,x"
                     <<"-y,z,-x"
                     <<"y,-z,-x"
                     <<"-y,-z,x"
                     <<"y,x,-z"
                     <<"-y,-x,-z"
                     <<"y,-x,z"
                     <<"-y,x,z"
                     <<"x,z,-y"
                     <<"-x,z,y"
                     <<"-x,-z,-y"
                     <<"x,-z,y"
                     <<"z,y,-x"
                     <<"z,-y,x"
                     <<"-z,y,x"
                     <<"-z,-y,-x"
                     <<"-x,-y,-z"
                     <<"x,y,-z"
                     <<"x,-y,z"
                     <<"-x,y,z"
                     <<"-z,-x,-y"
                     <<"-z,x,y"
                     <<"z,x,-y"
                     <<"z,-x,y"
                     <<"-y,-z,-x"
                     <<"y,-z,x"
                     <<"-y,z,x"
                     <<"y,z,-x"
                     <<"-y,-x,z"
                     <<"y,x,z"
                     <<"-y,x,-z"
                     <<"y,-x,-z"
                     <<"-x,-z,y"
                     <<"x,-z,-y"
                     <<"x,z,y"
                     <<"-x,z,-y"
                     <<"-z,-y,x"
                     <<"-z,y,-x"
                     <<"z,-y,-x"
                     <<"z,y,x"
                     <<"x,y+1/2,z+1/2"
                     <<"-x,-y+1/2,z+1/2"
                     <<"-x,y+1/2,-z+1/2"
                     <<"x,-y+1/2,-z+1/2"
                     <<"z,x+1/2,y+1/2"
                     <<"z,-x+1/2,-y+1/2"
                     <<"-z,-x+1/2,y+1/2"
                     <<"-z,x+1/2,-y+1/2"
                     <<"y,z+1/2,x+1/2"
                     <<"-y,z+1/2,-x+1/2"
                     <<"y,-z+1/2,-x+1/2"
                     <<"-y,-z+1/2,x+1/2"
                     <<"y,x+1/2,-z+1/2"
                     <<"-y,-x+1/2,-z+1/2"
                     <<"y,-x+1/2,z+1/2"
                     <<"-y,x+1/2,z+1/2"
                     <<"x,z+1/2,-y+1/2"
                     <<"-x,z+1/2,y+1/2"
                     <<"-x,-z+1/2,-y+1/2"
                     <<"x,-z+1/2,y+1/2"
                     <<"z,y+1/2,-x+1/2"
                     <<"z,-y+1/2,x+1/2"
                     <<"-z,y+1/2,x+1/2"
                     <<"-z,-y+1/2,-x+1/2"
                     <<"-x,-y+1/2,-z+1/2"
                     <<"x,y+1/2,-z+1/2"
                     <<"x,-y+1/2,z+1/2"
                     <<"-x,y+1/2,z+1/2"
                     <<"-z,-x+1/2,-y+1/2"
                     <<"-z,x+1/2,y+1/2"
                     <<"z,x+1/2,-y+1/2"
                     <<"z,-x+1/2,y+1/2"
                     <<"-y,-z+1/2,-x+1/2"
                     <<"y,-z+1/2,x+1/2"
                     <<"-y,z+1/2,x+1/2"
                     <<"y,z+1/2,-x+1/2"
                     <<"-y,-x+1/2,z+1/2"
                     <<"y,x+1/2,z+1/2"
                     <<"-y,x+1/2,-z+1/2"
                     <<"y,-x+1/2,-z+1/2"
                     <<"-x,-z+1/2,y+1/2"
                     <<"x,-z+1/2,-y+1/2"
                     <<"x,z+1/2,y+1/2"
                     <<"-x,z+1/2,-y+1/2"
                     <<"-z,-y+1/2,x+1/2"
                     <<"-z,y+1/2,-x+1/2"
                     <<"z,-y+1/2,-x+1/2"
                     <<"z,y+1/2,x+1/2"
                     <<"x+1/2,y,z+1/2"
                     <<"-x+1/2,-y,z+1/2"
                     <<"-x+1/2,y,-z+1/2"
                     <<"x+1/2,-y,-z+1/2"
                     <<"z+1/2,x,y+1/2"
                     <<"z+1/2,-x,-y+1/2"
                     <<"-z+1/2,-x,y+1/2"
                     <<"-z+1/2,x,-y+1/2"
                     <<"y+1/2,z,x+1/2"
                     <<"-y+1/2,z,-x+1/2"
                     <<"y+1/2,-z,-x+1/2"
                     <<"-y+1/2,-z,x+1/2"
                     <<"y+1/2,x,-z+1/2"
                     <<"-y+1/2,-x,-z+1/2"
                     <<"y+1/2,-x,z+1/2"
                     <<"-y+1/2,x,z+1/2"
                     <<"x+1/2,z,-y+1/2"
                     <<"-x+1/2,z,y+1/2"
                     <<"-x+1/2,-z,-y+1/2"
                     <<"x+1/2,-z,y+1/2"
                     <<"z+1/2,y,-x+1/2"
                     <<"z+1/2,-y,x+1/2"
                     <<"-z+1/2,y,x+1/2"
                     <<"-z+1/2,-y,-x+1/2"
                     <<"-x+1/2,-y,-z+1/2"
                     <<"x+1/2,y,-z+1/2"
                     <<"x+1/2,-y,z+1/2"
                     <<"-x+1/2,y,z+1/2"
                     <<"-z+1/2,-x,-y+1/2"
                     <<"-z+1/2,x,y+1/2"
                     <<"z+1/2,x,-y+1/2"
                     <<"z+1/2,-x,y+1/2"
                     <<"-y+1/2,-z,-x+1/2"
                     <<"y+1/2,-z,x+1/2"
                     <<"-y+1/2,z,x+1/2"
                     <<"y+1/2,z,-x+1/2"
                     <<"-y+1/2,-x,z+1/2"
                     <<"y+1/2,x,z+1/2"
                     <<"-y+1/2,x,-z+1/2"
                     <<"y+1/2,-x,-z+1/2"
                     <<"-x+1/2,-z,y+1/2"
                     <<"x+1/2,-z,-y+1/2"
                     <<"x+1/2,z,y+1/2"
                     <<"-x+1/2,z,-y+1/2"
                     <<"-z+1/2,-y,x+1/2"
                     <<"-z+1/2,y,-x+1/2"
                     <<"z+1/2,-y,-x+1/2"
                     <<"z+1/2,y,x+1/2"
                     <<"x+1/2,y+1/2,z"
                     <<"-x+1/2,-y+1/2,z"
                     <<"-x+1/2,y+1/2,-z"
                     <<"x+1/2,-y+1/2,-z"
                     <<"z+1/2,x+1/2,y"
                     <<"z+1/2,-x+1/2,-y"
                     <<"-z+1/2,-x+1/2,y"
                     <<"-z+1/2,x+1/2,-y"
                     <<"y+1/2,z+1/2,x"
                     <<"-y+1/2,z+1/2,-x"
                     <<"y+1/2,-z+1/2,-x"
                     <<"-y+1/2,-z+1/2,x"
                     <<"y+1/2,x+1/2,-z"
                     <<"-y+1/2,-x+1/2,-z"
                     <<"y+1/2,-x+1/2,z"
                     <<"-y+1/2,x+1/2,z"
                     <<"x+1/2,z+1/2,-y"
                     <<"-x+1/2,z+1/2,y"
                     <<"-x+1/2,-z+1/2,-y"
                     <<"x+1/2,-z+1/2,y"
                     <<"z+1/2,y+1/2,-x"
                     <<"z+1/2,-y+1/2,x"
                     <<"-z+1/2,y+1/2,x"
                     <<"-z+1/2,-y+1/2,-x"
                     <<"-x+1/2,-y+1/2,-z"
                     <<"x+1/2,y+1/2,-z"
                     <<"x+1/2,-y+1/2,z"
                     <<"-x+1/2,y+1/2,z"
                     <<"-z+1/2,-x+1/2,-y"
                     <<"-z+1/2,x+1/2,y"
                     <<"z+1/2,x+1/2,-y"
                     <<"z+1/2,-x+1/2,y"
                     <<"-y+1/2,-z+1/2,-x"
                     <<"y+1/2,-z+1/2,x"
                     <<"-y+1/2,z+1/2,x"
                     <<"y+1/2,z+1/2,-x"
                     <<"-y+1/2,-x+1/2,z"
                     <<"y+1/2,x+1/2,z"
                     <<"-y+1/2,x+1/2,-z"
                     <<"y+1/2,-x+1/2,-z"
                     <<"-x+1/2,-z+1/2,y"
                     <<"x+1/2,-z+1/2,-y"
                     <<"x+1/2,z+1/2,y"
                     <<"-x+1/2,z+1/2,-y"
                     <<"-z+1/2,-y+1/2,x"
                     <<"-z+1/2,y+1/2,-x"
                     <<"z+1/2,-y+1/2,-x"
                     <<"z+1/2,y+1/2,x";
    spaceGroups[224] = sp;
    //-----------------------
    sp.name = "";
    sp.number = 226;
    sp.symOperations.clear();
    sp.symOperations
                     <<"x,y,z"
                     <<"-x,-y,z"
                     <<"-x,y,-z"
                     <<"x,-y,-z"
                     <<"z,x,y"
                     <<"z,-x,-y"
                     <<"-z,-x,y"
                     <<"-z,x,-y"
                     <<"y,z,x"
                     <<"-y,z,-x"
                     <<"y,-z,-x"
                     <<"-y,-z,x"
                     <<"y+1/2,x+1/2,-z+1/2"
                     <<"-y+1/2,-x+1/2,-z+1/2"
                     <<"y+1/2,-x+1/2,z+1/2"
                     <<"-y+1/2,x+1/2,z+1/2"
                     <<"x+1/2,z+1/2,-y+1/2"
                     <<"-x+1/2,z+1/2,y+1/2"
                     <<"-x+1/2,-z+1/2,-y+1/2"
                     <<"x+1/2,-z+1/2,y+1/2"
                     <<"z+1/2,y+1/2,-x+1/2"
                     <<"z+1/2,-y+1/2,x+1/2"
                     <<"-z+1/2,y+1/2,x+1/2"
                     <<"-z+1/2,-y+1/2,-x+1/2"
                     <<"-x,-y,-z"
                     <<"x,y,-z"
                     <<"x,-y,z"
                     <<"-x,y,z"
                     <<"-z,-x,-y"
                     <<"-z,x,y"
                     <<"z,x,-y"
                     <<"z,-x,y"
                     <<"-y,-z,-x"
                     <<"y,-z,x"
                     <<"-y,z,x"
                     <<"y,z,-x"
                     <<"-y+1/2,-x+1/2,z+1/2"
                     <<"y+1/2,x+1/2,z+1/2"
                     <<"-y+1/2,x+1/2,-z+1/2"
                     <<"y+1/2,-x+1/2,-z+1/2"
                     <<"-x+1/2,-z+1/2,y+1/2"
                     <<"x+1/2,-z+1/2,-y+1/2"
                     <<"x+1/2,z+1/2,y+1/2"
                     <<"-x+1/2,z+1/2,-y+1/2"
                     <<"-z+1/2,-y+1/2,x+1/2"
                     <<"-z+1/2,y+1/2,-x+1/2"
                     <<"z+1/2,-y+1/2,-x+1/2"
                     <<"z+1/2,y+1/2,x+1/2"
                     <<"x,y+1/2,z+1/2"
                     <<"-x,-y+1/2,z+1/2"
                     <<"-x,y+1/2,-z+1/2"
                     <<"x,-y+1/2,-z+1/2"
                     <<"z,x+1/2,y+1/2"
                     <<"z,-x+1/2,-y+1/2"
                     <<"-z,-x+1/2,y+1/2"
                     <<"-z,x+1/2,-y+1/2"
                     <<"y,z+1/2,x+1/2"
                     <<"-y,z+1/2,-x+1/2"
                     <<"y,-z+1/2,-x+1/2"
                     <<"-y,-z+1/2,x+1/2"
                     <<"y+1/2,x,-z"
                     <<"-y+1/2,-x,-z"
                     <<"y+1/2,-x,z"
                     <<"-y+1/2,x,z"
                     <<"x+1/2,z,-y"
                     <<"-x+1/2,z,y"
                     <<"-x+1/2,-z,-y"
                     <<"x+1/2,-z,y"
                     <<"z+1/2,y,-x"
                     <<"z+1/2,-y,x"
                     <<"-z+1/2,y,x"
                     <<"-z+1/2,-y,-x"
                     <<"-x,-y+1/2,-z+1/2"
                     <<"x,y+1/2,-z+1/2"
                     <<"x,-y+1/2,z+1/2"
                     <<"-x,y+1/2,z+1/2"
                     <<"-z,-x+1/2,-y+1/2"
                     <<"-z,x+1/2,y+1/2"
                     <<"z,x+1/2,-y+1/2"
                     <<"z,-x+1/2,y+1/2"
                     <<"-y,-z+1/2,-x+1/2"
                     <<"y,-z+1/2,x+1/2"
                     <<"-y,z+1/2,x+1/2"
                     <<"y,z+1/2,-x+1/2"
                     <<"-y+1/2,-x,z"
                     <<"y+1/2,x,z"
                     <<"-y+1/2,x,-z"
                     <<"y+1/2,-x,-z"
                     <<"-x+1/2,-z,y"
                     <<"x+1/2,-z,-y"
                     <<"x+1/2,z,y"
                     <<"-x+1/2,z,-y"
                     <<"-z+1/2,-y,x"
                     <<"-z+1/2,y,-x"
                     <<"z+1/2,-y,-x"
                     <<"z+1/2,y,x"
                     <<"x+1/2,y,z+1/2"
                     <<"-x+1/2,-y,z+1/2"
                     <<"-x+1/2,y,-z+1/2"
                     <<"x+1/2,-y,-z+1/2"
                     <<"z+1/2,x,y+1/2"
                     <<"z+1/2,-x,-y+1/2"
                     <<"-z+1/2,-x,y+1/2"
                     <<"-z+1/2,x,-y+1/2"
                     <<"y+1/2,z,x+1/2"
                     <<"-y+1/2,z,-x+1/2"
                     <<"y+1/2,-z,-x+1/2"
                     <<"-y+1/2,-z,x+1/2"
                     <<"y,x+1/2,-z"
                     <<"-y,-x+1/2,-z"
                     <<"y,-x+1/2,z"
                     <<"-y,x+1/2,z"
                     <<"x,z+1/2,-y"
                     <<"-x,z+1/2,y"
                     <<"-x,-z+1/2,-y"
                     <<"x,-z+1/2,y"
                     <<"z,y+1/2,-x"
                     <<"z,-y+1/2,x"
                     <<"-z,y+1/2,x"
                     <<"-z,-y+1/2,-x"
                     <<"-x+1/2,-y,-z+1/2"
                     <<"x+1/2,y,-z+1/2"
                     <<"x+1/2,-y,z+1/2"
                     <<"-x+1/2,y,z+1/2"
                     <<"-z+1/2,-x,-y+1/2"
                     <<"-z+1/2,x,y+1/2"
                     <<"z+1/2,x,-y+1/2"
                     <<"z+1/2,-x,y+1/2"
                     <<"-y+1/2,-z,-x+1/2"
                     <<"y+1/2,-z,x+1/2"
                     <<"-y+1/2,z,x+1/2"
                     <<"y+1/2,z,-x+1/2"
                     <<"-y,-x+1/2,z"
                     <<"y,x+1/2,z"
                     <<"-y,x+1/2,-z"
                     <<"y,-x+1/2,-z"
                     <<"-x,-z+1/2,y"
                     <<"x,-z+1/2,-y"
                     <<"x,z+1/2,y"
                     <<"-x,z+1/2,-y"
                     <<"-z,-y+1/2,x"
                     <<"-z,y+1/2,-x"
                     <<"z,-y+1/2,-x"
                     <<"z,y+1/2,x"
                     <<"x+1/2,y+1/2,z"
                     <<"-x+1/2,-y+1/2,z"
                     <<"-x+1/2,y+1/2,-z"
                     <<"x+1/2,-y+1/2,-z"
                     <<"z+1/2,x+1/2,y"
                     <<"z+1/2,-x+1/2,-y"
                     <<"-z+1/2,-x+1/2,y"
                     <<"-z+1/2,x+1/2,-y"
                     <<"y+1/2,z+1/2,x"
                     <<"-y+1/2,z+1/2,-x"
                     <<"y+1/2,-z+1/2,-x"
                     <<"-y+1/2,-z+1/2,x"
                     <<"y,x,-z+1/2"
                     <<"-y,-x,-z+1/2"
                     <<"y,-x,z+1/2"
                     <<"-y,x,z+1/2"
                     <<"x,z,-y+1/2"
                     <<"-x,z,y+1/2"
                     <<"-x,-z,-y+1/2"
                     <<"x,-z,y+1/2"
                     <<"z,y,-x+1/2"
                     <<"z,-y,x+1/2"
                     <<"-z,y,x+1/2"
                     <<"-z,-y,-x+1/2"
                     <<"-x+1/2,-y+1/2,-z"
                     <<"x+1/2,y+1/2,-z"
                     <<"x+1/2,-y+1/2,z"
                     <<"-x+1/2,y+1/2,z"
                     <<"-z+1/2,-x+1/2,-y"
                     <<"-z+1/2,x+1/2,y"
                     <<"z+1/2,x+1/2,-y"
                     <<"z+1/2,-x+1/2,y"
                     <<"-y+1/2,-z+1/2,-x"
                     <<"y+1/2,-z+1/2,x"
                     <<"-y+1/2,z+1/2,x"
                     <<"y+1/2,z+1/2,-x"
                     <<"-y,-x,z+1/2"
                     <<"y,x,z+1/2"
                     <<"-y,x,-z+1/2"
                     <<"y,-x,-z+1/2"
                     <<"-x,-z,y+1/2"
                     <<"x,-z,-y+1/2"
                     <<"x,z,y+1/2"
                     <<"-x,z,-y+1/2"
                     <<"-z,-y,x+1/2"
                     <<"-z,y,-x+1/2"
                     <<"z,-y,-x+1/2"
                     <<"z,y,x+1/2";
    spaceGroups[225] = sp;
    //-----------------------
    sp.name = "";
    sp.number = 227;
    sp.symOperations.clear();
    sp.symOperations
                     <<"x,y,z"
                     <<"-x+3/4,-y+1/4,z+1/2"
                     <<"-x+1/4,y+1/2,-z+3/4"
                     <<"x+1/2,-y+3/4,-z+1/4"
                     <<"z,x,y"
                     <<"z+1/2,-x+3/4,-y+1/4"
                     <<"-z+3/4,-x+1/4,y+1/2"
                     <<"-z+1/4,x+1/2,-y+3/4"
                     <<"y,z,x"
                     <<"-y+1/4,z+1/2,-x+3/4"
                     <<"y+1/2,-z+3/4,-x+1/4"
                     <<"-y+3/4,-z+1/4,x+1/2"
                     <<"y+3/4,x+1/4,-z+1/2"
                     <<"-y,-x,-z"
                     <<"y+1/4,-x+1/2,z+3/4"
                     <<"-y+1/2,x+3/4,z+1/4"
                     <<"x+3/4,z+1/4,-y+1/2"
                     <<"-x+1/2,z+3/4,y+1/4"
                     <<"-x,-z,-y"
                     <<"x+1/4,-z+1/2,y+3/4"
                     <<"z+3/4,y+1/4,-x+1/2"
                     <<"z+1/4,-y+1/2,x+3/4"
                     <<"-z+1/2,y+3/4,x+1/4"
                     <<"-z,-y,-x"
                     <<"-x,-y,-z"
                     <<"x+1/4,y+3/4,-z+1/2"
                     <<"x+3/4,-y+1/2,z+1/4"
                     <<"-x+1/2,y+1/4,z+3/4"
                     <<"-z,-x,-y"
                     <<"-z+1/2,x+1/4,y+3/4"
                     <<"z+1/4,x+3/4,-y+1/2"
                     <<"z+3/4,-x+1/2,y+1/4"
                     <<"-y,-z,-x"
                     <<"y+3/4,-z+1/2,x+1/4"
                     <<"-y+1/2,z+1/4,x+3/4"
                     <<"y+1/4,z+3/4,-x+1/2"
                     <<"-y+1/4,-x+3/4,z+1/2"
                     <<"y,x,z"
                     <<"-y+3/4,x+1/2,-z+1/4"
                     <<"y+1/2,-x+1/4,-z+3/4"
                     <<"-x+1/4,-z+3/4,y+1/2"
                     <<"x+1/2,-z+1/4,-y+3/4"
                     <<"x,z,y"
                     <<"-x+3/4,z+1/2,-y+1/4"
                     <<"-z+1/4,-y+3/4,x+1/2"
                     <<"-z+3/4,y+1/2,-x+1/4"
                     <<"z+1/2,-y+1/4,-x+3/4"
                     <<"z,y,x"
                     <<"x,y+1/2,z+1/2"
                     <<"-x+3/4,-y+3/4,z"
                     <<"-x+1/4,y,-z+1/4"
                     <<"x+1/2,-y+1/4,-z+3/4"
                     <<"z,x+1/2,y+1/2"
                     <<"z+1/2,-x+1/4,-y+3/4"
                     <<"-z+3/4,-x+3/4,y"
                     <<"-z+1/4,x,-y+1/4"
                     <<"y,z+1/2,x+1/2"
                     <<"-y+1/4,z,-x+1/4"
                     <<"y+1/2,-z+1/4,-x+3/4"
                     <<"-y+3/4,-z+3/4,x"
                     <<"y+3/4,x+3/4,-z"
                     <<"-y,-x+1/2,-z+1/2"
                     <<"y+1/4,-x,z+1/4"
                     <<"-y+1/2,x+1/4,z+3/4"
                     <<"x+3/4,z+3/4,-y"
                     <<"-x+1/2,z+1/4,y+3/4"
                     <<"-x,-z+1/2,-y+1/2"
                     <<"x+1/4,-z,y+1/4"
                     <<"z+3/4,y+3/4,-x"
                     <<"z+1/4,-y,x+1/4"
                     <<"-z+1/2,y+1/4,x+3/4"
                     <<"-z,-y+1/2,-x+1/2"
                     <<"-x,-y+1/2,-z+1/2"
                     <<"x+1/4,y+1/4,-z"
                     <<"x+3/4,-y,z+3/4"
                     <<"-x+1/2,y+3/4,z+1/4"
                     <<"-z,-x+1/2,-y+1/2"
                     <<"-z+1/2,x+3/4,y+1/4"
                     <<"z+1/4,x+1/4,-y"
                     <<"z+3/4,-x,y+3/4"
                     <<"-y,-z+1/2,-x+1/2"
                     <<"y+3/4,-z,x+3/4"
                     <<"-y+1/2,z+3/4,x+1/4"
                     <<"y+1/4,z+1/4,-x"
                     <<"-y+1/4,-x+1/4,z"
                     <<"y,x+1/2,z+1/2"
                     <<"-y+3/4,x,-z+3/4"
                     <<"y+1/2,-x+3/4,-z+1/4"
                     <<"-x+1/4,-z+1/4,y"
                     <<"x+1/2,-z+3/4,-y+1/4"
                     <<"x,z+1/2,y+1/2"
                     <<"-x+3/4,z,-y+3/4"
                     <<"-z+1/4,-y+1/4,x"
                     <<"-z+3/4,y,-x+3/4"
                     <<"z+1/2,-y+3/4,-x+1/4"
                     <<"z,y+1/2,x+1/2"
                     <<"x+1/2,y,z+1/2"
                     <<"-x+1/4,-y+1/4,z"
                     <<"-x+3/4,y+1/2,-z+1/4"
                     <<"x,-y+3/4,-z+3/4"
                     <<"z+1/2,x,y+1/2"
                     <<"z,-x+3/4,-y+3/4"
                     <<"-z+1/4,-x+1/4,y"
                     <<"-z+3/4,x+1/2,-y+1/4"
                     <<"y+1/2,z,x+1/2"
                     <<"-y+3/4,z+1/2,-x+1/4"
                     <<"y,-z+3/4,-x+3/4"
                     <<"-y+1/4,-z+1/4,x"
                     <<"y+1/4,x+1/4,-z"
                     <<"-y+1/2,-x,-z+1/2"
                     <<"y+3/4,-x+1/2,z+1/4"
                     <<"-y,x+3/4,z+3/4"
                     <<"x+1/4,z+1/4,-y"
                     <<"-x,z+3/4,y+3/4"
                     <<"-x+1/2,-z,-y+1/2"
                     <<"x+3/4,-z+1/2,y+1/4"
                     <<"z+1/4,y+1/4,-x"
                     <<"z+3/4,-y+1/2,x+1/4"
                     <<"-z,y+3/4,x+3/4"
                     <<"-z+1/2,-y,-x+1/2"
                     <<"-x+1/2,-y,-z+1/2"
                     <<"x+3/4,y+3/4,-z"
                     <<"x+1/4,-y+1/2,z+3/4"
                     <<"-x,y+1/4,z+1/4"
                     <<"-z+1/2,-x,-y+1/2"
                     <<"-z,x+1/4,y+1/4"
                     <<"z+3/4,x+3/4,-y"
                     <<"z+1/4,-x+1/2,y+3/4"
                     <<"-y+1/2,-z,-x+1/2"
                     <<"y+1/4,-z+1/2,x+3/4"
                     <<"-y,z+1/4,x+1/4"
                     <<"y+3/4,z+3/4,-x"
                     <<"-y+3/4,-x+3/4,z"
                     <<"y+1/2,x,z+1/2"
                     <<"-y+1/4,x+1/2,-z+3/4"
                     <<"y,-x+1/4,-z+1/4"
                     <<"-x+3/4,-z+3/4,y"
                     <<"x,-z+1/4,-y+1/4"
                     <<"x+1/2,z,y+1/2"
                     <<"-x+1/4,z+1/2,-y+3/4"
                     <<"-z+3/4,-y+3/4,x"
                     <<"-z+1/4,y+1/2,-x+3/4"
                     <<"z,-y+1/4,-x+1/4"
                     <<"z+1/2,y,x+1/2"
                     <<"x+1/2,y+1/2,z"
                     <<"-x+1/4,-y+3/4,z+1/2"
                     <<"-x+3/4,y,-z+3/4"
                     <<"x,-y+1/4,-z+1/4"
                     <<"z+1/2,x+1/2,y"
                     <<"z,-x+1/4,-y+1/4"
                     <<"-z+1/4,-x+3/4,y+1/2"
                     <<"-z+3/4,x,-y+3/4"
                     <<"y+1/2,z+1/2,x"
                     <<"-y+3/4,z,-x+3/4"
                     <<"y,-z+1/4,-x+1/4"
                     <<"-y+1/4,-z+3/4,x+1/2"
                     <<"y+1/4,x+3/4,-z+1/2"
                     <<"-y+1/2,-x+1/2,-z"
                     <<"y+3/4,-x,z+3/4"
                     <<"-y,x+1/4,z+1/4"
                     <<"x+1/4,z+3/4,-y+1/2"
                     <<"-x,z+1/4,y+1/4"
                     <<"-x+1/2,-z+1/2,-y"
                     <<"x+3/4,-z,y+3/4"
                     <<"z+1/4,y+3/4,-x+1/2"
                     <<"z+3/4,-y,x+3/4"
                     <<"-z,y+1/4,x+1/4"
                     <<"-z+1/2,-y+1/2,-x"
                     <<"-x+1/2,-y+1/2,-z"
                     <<"x+3/4,y+1/4,-z+1/2"
                     <<"x+1/4,-y,z+1/4"
                     <<"-x,y+3/4,z+3/4"
                     <<"-z+1/2,-x+1/2,-y"
                     <<"-z,x+3/4,y+3/4"
                     <<"z+3/4,x+1/4,-y+1/2"
                     <<"z+1/4,-x,y+1/4"
                     <<"-y+1/2,-z+1/2,-x"
                     <<"y+1/4,-z,x+1/4"
                     <<"-y,z+3/4,x+3/4"
                     <<"y+3/4,z+1/4,-x+1/2"
                     <<"-y+3/4,-x+1/4,z+1/2"
                     <<"y+1/2,x+1/2,z"
                     <<"-y+1/4,x,-z+1/4"
                     <<"y,-x+3/4,-z+3/4"
                     <<"-x+3/4,-z+1/4,y+1/2"
                     <<"x,-z+3/4,-y+3/4"
                     <<"x+1/2,z+1/2,y"
                     <<"-x+1/4,z,-y+1/4"
                     <<"-z+3/4,-y+1/4,x+1/2"
                     <<"-z+1/4,y,-x+1/4"
                     <<"z,-y+3/4,-x+3/4"
                     <<"z+1/2,y+1/2,x";
    spaceGroups[226] = sp;
    //-----------------------
    sp.name = "";
    sp.number = 228;
    sp.symOperations.clear();
    sp.symOperations
                     <<"x,y,z"
                     <<"-x+1/4,-y+3/4,z+1/2"
                     <<"-x+3/4,y+1/2,-z+1/4"
                     <<"x+1/2,-y+1/4,-z+3/4"
                     <<"z,x,y"
                     <<"z+1/2,-x+1/4,-y+3/4"
                     <<"-z+1/4,-x+3/4,y+1/2"
                     <<"-z+3/4,x+1/2,-y+1/4"
                     <<"y,z,x"
                     <<"-y+3/4,z+1/2,-x+1/4"
                     <<"y+1/2,-z+1/4,-x+3/4"
                     <<"-y+1/4,-z+3/4,x+1/2"
                     <<"y+3/4,x+1/4,-z"
                     <<"-y+1/2,-x+1/2,-z+1/2"
                     <<"y+1/4,-x,z+3/4"
                     <<"-y,x+3/4,z+1/4"
                     <<"x+3/4,z+1/4,-y"
                     <<"-x,z+3/4,y+1/4"
                     <<"-x+1/2,-z+1/2,-y+1/2"
                     <<"x+1/4,-z,y+3/4"
                     <<"z+3/4,y+1/4,-x"
                     <<"z+1/4,-y,x+3/4"
                     <<"-z,y+3/4,x+1/4"
                     <<"-z+1/2,-y+1/2,-x+1/2"
                     <<"-x,-y,-z"
                     <<"x+3/4,y+1/4,-z+1/2"
                     <<"x+1/4,-y+1/2,z+3/4"
                     <<"-x+1/2,y+3/4,z+1/4"
                     <<"-z,-x,-y"
                     <<"-z+1/2,x+3/4,y+1/4"
                     <<"z+3/4,x+1/4,-y+1/2"
                     <<"z+1/4,-x+1/2,y+3/4"
                     <<"-y,-z,-x"
                     <<"y+1/4,-z+1/2,x+3/4"
                     <<"-y+1/2,z+3/4,x+1/4"
                     <<"y+3/4,z+1/4,-x+1/2"
                     <<"-y+1/4,-x+3/4,z"
                     <<"y+1/2,x+1/2,z+1/2"
                     <<"-y+3/4,x,-z+1/4"
                     <<"y,-x+1/4,-z+3/4"
                     <<"-x+1/4,-z+3/4,y"
                     <<"x,-z+1/4,-y+3/4"
                     <<"x+1/2,z+1/2,y+1/2"
                     <<"-x+3/4,z,-y+1/4"
                     <<"-z+1/4,-y+3/4,x"
                     <<"-z+3/4,y,-x+1/4"
                     <<"z,-y+1/4,-x+3/4"
                     <<"z+1/2,y+1/2,x+1/2"
                     <<"x,y+1/2,z+1/2"
                     <<"-x+1/4,-y+1/4,z"
                     <<"-x+3/4,y,-z+3/4"
                     <<"x+1/2,-y+3/4,-z+1/4"
                     <<"z,x+1/2,y+1/2"
                     <<"z+1/2,-x+3/4,-y+1/4"
                     <<"-z+1/4,-x+1/4,y"
                     <<"-z+3/4,x,-y+3/4"
                     <<"y,z+1/2,x+1/2"
                     <<"-y+3/4,z,-x+3/4"
                     <<"y+1/2,-z+3/4,-x+1/4"
                     <<"-y+1/4,-z+1/4,x"
                     <<"y+3/4,x+3/4,-z+1/2"
                     <<"-y+1/2,-x,-z"
                     <<"y+1/4,-x+1/2,z+1/4"
                     <<"-y,x+1/4,z+3/4"
                     <<"x+3/4,z+3/4,-y+1/2"
                     <<"-x,z+1/4,y+3/4"
                     <<"-x+1/2,-z,-y"
                     <<"x+1/4,-z+1/2,y+1/4"
                     <<"z+3/4,y+3/4,-x+1/2"
                     <<"z+1/4,-y+1/2,x+1/4"
                     <<"-z,y+1/4,x+3/4"
                     <<"-z+1/2,-y,-x"
                     <<"-x,-y+1/2,-z+1/2"
                     <<"x+3/4,y+3/4,-z"
                     <<"x+1/4,-y,z+1/4"
                     <<"-x+1/2,y+1/4,z+3/4"
                     <<"-z,-x+1/2,-y+1/2"
                     <<"-z+1/2,x+1/4,y+3/4"
                     <<"z+3/4,x+3/4,-y"
                     <<"z+1/4,-x,y+1/4"
                     <<"-y,-z+1/2,-x+1/2"
                     <<"y+1/4,-z,x+1/4"
                     <<"-y+1/2,z+1/4,x+3/4"
                     <<"y+3/4,z+3/4,-x"
                     <<"-y+1/4,-x+1/4,z+1/2"
                     <<"y+1/2,x,z"
                     <<"-y+3/4,x+1/2,-z+3/4"
                     <<"y,-x+3/4,-z+1/4"
                     <<"-x+1/4,-z+1/4,y+1/2"
                     <<"x,-z+3/4,-y+1/4"
                     <<"x+1/2,z,y"
                     <<"-x+3/4,z+1/2,-y+3/4"
                     <<"-z+1/4,-y+1/4,x+1/2"
                     <<"-z+3/4,y+1/2,-x+3/4"
                     <<"z,-y+3/4,-x+1/4"
                     <<"z+1/2,y,x"
                     <<"x+1/2,y,z+1/2"
                     <<"-x+3/4,-y+3/4,z"
                     <<"-x+1/4,y+1/2,-z+3/4"
                     <<"x,-y+1/4,-z+1/4"
                     <<"z+1/2,x,y+1/2"
                     <<"z,-x+1/4,-y+1/4"
                     <<"-z+3/4,-x+3/4,y"
                     <<"-z+1/4,x+1/2,-y+3/4"
                     <<"y+1/2,z,x+1/2"
                     <<"-y+1/4,z+1/2,-x+3/4"
                     <<"y,-z+1/4,-x+1/4"
                     <<"-y+3/4,-z+3/4,x"
                     <<"y+1/4,x+1/4,-z+1/2"
                     <<"-y,-x+1/2,-z"
                     <<"y+3/4,-x,z+1/4"
                     <<"-y+1/2,x+3/4,z+3/4"
                     <<"x+1/4,z+1/4,-y+1/2"
                     <<"-x+1/2,z+3/4,y+3/4"
                     <<"-x,-z+1/2,-y"
                     <<"x+3/4,-z,y+1/4"
                     <<"z+1/4,y+1/4,-x+1/2"
                     <<"z+3/4,-y,x+1/4"
                     <<"-z+1/2,y+3/4,x+3/4"
                     <<"-z,-y+1/2,-x"
                     <<"-x+1/2,-y,-z+1/2"
                     <<"x+1/4,y+1/4,-z"
                     <<"x+3/4,-y+1/2,z+1/4"
                     <<"-x,y+3/4,z+3/4"
                     <<"-z+1/2,-x,-y+1/2"
                     <<"-z,x+3/4,y+3/4"
                     <<"z+1/4,x+1/4,-y"
                     <<"z+3/4,-x+1/2,y+1/4"
                     <<"-y+1/2,-z,-x+1/2"
                     <<"y+3/4,-z+1/2,x+1/4"
                     <<"-y,z+3/4,x+3/4"
                     <<"y+1/4,z+1/4,-x"
                     <<"-y+3/4,-x+3/4,z+1/2"
                     <<"y,x+1/2,z"
                     <<"-y+1/4,x,-z+3/4"
                     <<"y+1/2,-x+1/4,-z+1/4"
                     <<"-x+3/4,-z+3/4,y+1/2"
                     <<"x+1/2,-z+1/4,-y+1/4"
                     <<"x,z+1/2,y"
                     <<"-x+1/4,z,-y+3/4"
                     <<"-z+3/4,-y+3/4,x+1/2"
                     <<"-z+1/4,y,-x+3/4"
                     <<"z+1/2,-y+1/4,-x+1/4"
                     <<"z,y+1/2,x"
                     <<"x+1/2,y+1/2,z"
                     <<"-x+3/4,-y+1/4,z+1/2"
                     <<"-x+1/4,y,-z+1/4"
                     <<"x,-y+3/4,-z+3/4"
                     <<"z+1/2,x+1/2,y"
                     <<"z,-x+3/4,-y+3/4"
                     <<"-z+3/4,-x+1/4,y+1/2"
                     <<"-z+1/4,x,-y+1/4"
                     <<"y+1/2,z+1/2,x"
                     <<"-y+1/4,z,-x+1/4"
                     <<"y,-z+3/4,-x+3/4"
                     <<"-y+3/4,-z+1/4,x+1/2"
                     <<"y+1/4,x+3/4,-z"
                     <<"-y,-x,-z+1/2"
                     <<"y+3/4,-x+1/2,z+3/4"
                     <<"-y+1/2,x+1/4,z+1/4"
                     <<"x+1/4,z+3/4,-y"
                     <<"-x+1/2,z+1/4,y+1/4"
                     <<"-x,-z,-y+1/2"
                     <<"x+3/4,-z+1/2,y+3/4"
                     <<"z+1/4,y+3/4,-x"
                     <<"z+3/4,-y+1/2,x+3/4"
                     <<"-z+1/2,y+1/4,x+1/4"
                     <<"-z,-y,-x+1/2"
                     <<"-x+1/2,-y+1/2,-z"
                     <<"x+1/4,y+3/4,-z+1/2"
                     <<"x+3/4,-y,z+3/4"
                     <<"-x,y+1/4,z+1/4"
                     <<"-z+1/2,-x+1/2,-y"
                     <<"-z,x+1/4,y+1/4"
                     <<"z+1/4,x+3/4,-y+1/2"
                     <<"z+3/4,-x,y+3/4"
                     <<"-y+1/2,-z+1/2,-x"
                     <<"y+3/4,-z,x+3/4"
                     <<"-y,z+1/4,x+1/4"
                     <<"y+1/4,z+3/4,-x+1/2"
                     <<"-y+3/4,-x+1/4,z"
                     <<"y,x,z+1/2"
                     <<"-y+1/4,x+1/2,-z+1/4"
                     <<"y+1/2,-x+3/4,-z+3/4"
                     <<"-x+3/4,-z+1/4,y"
                     <<"x+1/2,-z+3/4,-y+3/4"
                     <<"x,z,y+1/2"
                     <<"-x+1/4,z+1/2,-y+1/4"
                     <<"-z+3/4,-y+1/4,x"
                     <<"-z+1/4,y+1/2,-x+1/4"
                     <<"z+1/2,-y+3/4,-x+3/4"
                     <<"z,y,x+1/2";
    spaceGroups[227] = sp;
    //-----------------------
    sp.name = "";
    sp.number = 229;
    sp.symOperations.clear();
    sp.symOperations
                     <<"x,y,z"
                     <<"-x,-y,z"
                     <<"-x,y,-z"
                     <<"x,-y,-z"
                     <<"z,x,y"
                     <<"z,-x,-y"
                     <<"-z,-x,y"
                     <<"-z,x,-y"
                     <<"y,z,x"
                     <<"-y,z,-x"
                     <<"y,-z,-x"
                     <<"-y,-z,x"
                     <<"y,x,-z"
                     <<"-y,-x,-z"
                     <<"y,-x,z"
                     <<"-y,x,z"
                     <<"x,z,-y"
                     <<"-x,z,y"
                     <<"-x,-z,-y"
                     <<"x,-z,y"
                     <<"z,y,-x"
                     <<"z,-y,x"
                     <<"-z,y,x"
                     <<"-z,-y,-x"
                     <<"-x,-y,-z"
                     <<"x,y,-z"
                     <<"x,-y,z"
                     <<"-x,y,z"
                     <<"-z,-x,-y"
                     <<"-z,x,y"
                     <<"z,x,-y"
                     <<"z,-x,y"
                     <<"-y,-z,-x"
                     <<"y,-z,x"
                     <<"-y,z,x"
                     <<"y,z,-x"
                     <<"-y,-x,z"
                     <<"y,x,z"
                     <<"-y,x,-z"
                     <<"y,-x,-z"
                     <<"-x,-z,y"
                     <<"x,-z,-y"
                     <<"x,z,y"
                     <<"-x,z,-y"
                     <<"-z,-y,x"
                     <<"-z,y,-x"
                     <<"z,-y,-x"
                     <<"z,y,x"
                     <<"x+1/2,y+1/2,z+1/2"
                     <<"-x+1/2,-y+1/2,z+1/2"
                     <<"-x+1/2,y+1/2,-z+1/2"
                     <<"x+1/2,-y+1/2,-z+1/2"
                     <<"z+1/2,x+1/2,y+1/2"
                     <<"z+1/2,-x+1/2,-y+1/2"
                     <<"-z+1/2,-x+1/2,y+1/2"
                     <<"-z+1/2,x+1/2,-y+1/2"
                     <<"y+1/2,z+1/2,x+1/2"
                     <<"-y+1/2,z+1/2,-x+1/2"
                     <<"y+1/2,-z+1/2,-x+1/2"
                     <<"-y+1/2,-z+1/2,x+1/2"
                     <<"y+1/2,x+1/2,-z+1/2"
                     <<"-y+1/2,-x+1/2,-z+1/2"
                     <<"y+1/2,-x+1/2,z+1/2"
                     <<"-y+1/2,x+1/2,z+1/2"
                     <<"x+1/2,z+1/2,-y+1/2"
                     <<"-x+1/2,z+1/2,y+1/2"
                     <<"-x+1/2,-z+1/2,-y+1/2"
                     <<"x+1/2,-z+1/2,y+1/2"
                     <<"z+1/2,y+1/2,-x+1/2"
                     <<"z+1/2,-y+1/2,x+1/2"
                     <<"-z+1/2,y+1/2,x+1/2"
                     <<"-z+1/2,-y+1/2,-x+1/2"
                     <<"-x+1/2,-y+1/2,-z+1/2"
                     <<"x+1/2,y+1/2,-z+1/2"
                     <<"x+1/2,-y+1/2,z+1/2"
                     <<"-x+1/2,y+1/2,z+1/2"
                     <<"-z+1/2,-x+1/2,-y+1/2"
                     <<"-z+1/2,x+1/2,y+1/2"
                     <<"z+1/2,x+1/2,-y+1/2"
                     <<"z+1/2,-x+1/2,y+1/2"
                     <<"-y+1/2,-z+1/2,-x+1/2"
                     <<"y+1/2,-z+1/2,x+1/2"
                     <<"-y+1/2,z+1/2,x+1/2"
                     <<"y+1/2,z+1/2,-x+1/2"
                     <<"-y+1/2,-x+1/2,z+1/2"
                     <<"y+1/2,x+1/2,z+1/2"
                     <<"-y+1/2,x+1/2,-z+1/2"
                     <<"y+1/2,-x+1/2,-z+1/2"
                     <<"-x+1/2,-z+1/2,y+1/2"
                     <<"x+1/2,-z+1/2,-y+1/2"
                     <<"x+1/2,z+1/2,y+1/2"
                     <<"-x+1/2,z+1/2,-y+1/2"
                     <<"-z+1/2,-y+1/2,x+1/2"
                     <<"-z+1/2,y+1/2,-x+1/2"
                     <<"z+1/2,-y+1/2,-x+1/2"
                     <<"z+1/2,y+1/2,x+1/2";
    spaceGroups[228] = sp;
    //-----------------------
    sp.name = "";
    sp.number = 230;
    sp.symOperations.clear();
    sp.symOperations
                     <<"x,y,z"
                     <<"-x+1/2,-y,z+1/2"
                     <<"-x,y+1/2,-z+1/2"
                     <<"x+1/2,-y+1/2,-z"
                     <<"z,x,y"
                     <<"z+1/2,-x+1/2,-y"
                     <<"-z+1/2,-x,y+1/2"
                     <<"-z,x+1/2,-y+1/2"
                     <<"y,z,x"
                     <<"-y,z+1/2,-x+1/2"
                     <<"y+1/2,-z+1/2,-x"
                     <<"-y+1/2,-z,x+1/2"
                     <<"y+3/4,x+1/4,-z+1/4"
                     <<"-y+3/4,-x+3/4,-z+3/4"
                     <<"y+1/4,-x+1/4,z+3/4"
                     <<"-y+1/4,x+3/4,z+1/4"
                     <<"x+3/4,z+1/4,-y+1/4"
                     <<"-x+1/4,z+3/4,y+1/4"
                     <<"-x+3/4,-z+3/4,-y+3/4"
                     <<"x+1/4,-z+1/4,y+3/4"
                     <<"z+3/4,y+1/4,-x+1/4"
                     <<"z+1/4,-y+1/4,x+3/4"
                     <<"-z+1/4,y+3/4,x+1/4"
                     <<"-z+3/4,-y+3/4,-x+3/4"
                     <<"-x,-y,-z"
                     <<"x+1/2,y,-z+1/2"
                     <<"x,-y+1/2,z+1/2"
                     <<"-x+1/2,y+1/2,z"
                     <<"-z,-x,-y"
                     <<"-z+1/2,x+1/2,y"
                     <<"z+1/2,x,-y+1/2"
                     <<"z,-x+1/2,y+1/2"
                     <<"-y,-z,-x"
                     <<"y,-z+1/2,x+1/2"
                     <<"-y+1/2,z+1/2,x"
                     <<"y+1/2,z,-x+1/2"
                     <<"-y+1/4,-x+3/4,z+3/4"
                     <<"y+1/4,x+1/4,z+1/4"
                     <<"-y+3/4,x+3/4,-z+1/4"
                     <<"y+3/4,-x+1/4,-z+3/4"
                     <<"-x+1/4,-z+3/4,y+3/4"
                     <<"x+3/4,-z+1/4,-y+3/4"
                     <<"x+1/4,z+1/4,y+1/4"
                     <<"-x+3/4,z+3/4,-y+1/4"
                     <<"-z+1/4,-y+3/4,x+3/4"
                     <<"-z+3/4,y+3/4,-x+1/4"
                     <<"z+3/4,-y+1/4,-x+3/4"
                     <<"z+1/4,y+1/4,x+1/4"
                     <<"x+1/2,y+1/2,z+1/2"
                     <<"-x,-y+1/2,z"
                     <<"-x+1/2,y,-z"
                     <<"x,-y,-z+1/2"
                     <<"z+1/2,x+1/2,y+1/2"
                     <<"z,-x,-y+1/2"
                     <<"-z,-x+1/2,y"
                     <<"-z+1/2,x,-y"
                     <<"y+1/2,z+1/2,x+1/2"
                     <<"-y+1/2,z,-x"
                     <<"y,-z,-x+1/2"
                     <<"-y,-z+1/2,x"
                     <<"y+1/4,x+3/4,-z+3/4"
                     <<"-y+1/4,-x+1/4,-z+1/4"
                     <<"y+3/4,-x+3/4,z+1/4"
                     <<"-y+3/4,x+1/4,z+3/4"
                     <<"x+1/4,z+3/4,-y+3/4"
                     <<"-x+3/4,z+1/4,y+3/4"
                     <<"-x+1/4,-z+1/4,-y+1/4"
                     <<"x+3/4,-z+3/4,y+1/4"
                     <<"z+1/4,y+3/4,-x+3/4"
                     <<"z+3/4,-y+3/4,x+1/4"
                     <<"-z+3/4,y+1/4,x+3/4"
                     <<"-z+1/4,-y+1/4,-x+1/4"
                     <<"-x+1/2,-y+1/2,-z+1/2"
                     <<"x,y+1/2,-z"
                     <<"x+1/2,-y,z"
                     <<"-x,y,z+1/2"
                     <<"-z+1/2,-x+1/2,-y+1/2"
                     <<"-z,x,y+1/2"
                     <<"z,x+1/2,-y"
                     <<"z+1/2,-x,y"
                     <<"-y+1/2,-z+1/2,-x+1/2"
                     <<"y+1/2,-z,x"
                     <<"-y,z,x+1/2"
                     <<"y,z+1/2,-x"
                     <<"-y+3/4,-x+1/4,z+1/4"
                     <<"y+3/4,x+3/4,z+3/4"
                     <<"-y+1/4,x+1/4,-z+3/4"
                     <<"y+1/4,-x+3/4,-z+1/4"
                     <<"-x+3/4,-z+1/4,y+1/4"
                     <<"x+1/4,-z+3/4,-y+1/4"
                     <<"x+3/4,z+3/4,y+3/4"
                     <<"-x+1/4,z+1/4,-y+3/4"
                     <<"-z+3/4,-y+1/4,x+1/4"
                     <<"-z+1/4,y+1/4,-x+3/4"
                     <<"z+1/4,-y+3/4,-x+1/4"
                     <<"z+3/4,y+3/4,x+3/4";
    spaceGroups[229] = sp;
    //-----------------------


}

void set_init_properties(){

    setSpaceGroups();

    colors["H"]={0.6,0.6,0.6,1.0};
    colors["He"]={0.85,1.0,1.0,1.0};
    colors["Li"]={0.8,0.5,1.0,1.0};
    colors["Be"]={0.75,1.0,0.0,1.0};
    colors["B"]={1.0,1.0, 0.71,1.0};
    colors["C"]={0.4,0.4,0.4,1.0};
    colors["N"]={0.05,0.05,1.0,1.0};
    colors["O"]={1.0,0.05,0.05,1.0};
    colors["F"]={0.5,0.7,1.0,1.0};
    colors["Na"]={0.67, 0.36, 0.95,1.0};
    colors["Mg"]={0.54, 1.0, 0.0,1.0};
    colors["Al"]={0.75,0.65,0.65,1.0};
    colors["Si"]={0.5,0.6,0.6,1.0};
    colors["P"]={1.0,0.5,0.0,1.0};
    colors["S"]={0.7,0.7,0.0,1.0};
    colors["Cl"]={0.12,0.94,0.12,1.0};
    colors["K"]={0.56,0.25,0.83,1.0};
    colors["Ca"]={0.24,1.0,0.0,1.0};
    colors["Sc"]={0.9,0.9,0.9,1.0};
    colors["Ti"]={0.74,0.74,0.74,1.0};
    colors["V"]={0.65,0.65,0.65,1.0};
    colors["Cr"]={0.54,0.6,0.78,1.0};
    colors["Mn"]={0.61,0.48,0.78,1.0};
    colors["Fe"]={0.88,0.4,0.2,1.0};
    colors["Co"]={0.5,0.0,0.5,1.0};
    colors["Ni"]={0.14,0.45,0.19,1.0};
    colors["Cu"]={0.0,0.75,0.0,1.0};
    colors["Zn"]={0.48,0.5,0.69,1.0};
    colors["Ga"]={0.8,0.8,0.8,1.0};
    colors["Ge"]={0.8,0.8,0.8,1.0};
    colors["As"]={0.8,0.8,0.8,1.0};
    colors["Se"]={0.8,0.8,0.8,1.0};
    colors["Br"]={0.65,0.16,0.16,1.0};
    colors["Kr"]={0.8,0.8,0.8,1.0};
    colors["Rb"]={0.8,0.8,0.8,1.0};
    colors["Sr"]={0.8,0.8,0.8,1.0};
    colors["Y"]={0.8,0.8,0.8,1.0};
    colors["Zr"]={0.8,0.8,0.8,1.0};
    colors["Nb"]={0.8,0.8,0.8,1.0};
    colors["Mo"]={0.8,0.8,0.8,1.0};
    colors["Tc"]={0.8,0.8,0.8,1.0};
    colors["Ru"]={0.8,0.8,0.8,1.0};
    colors["Rh"]={0.8,0.8,0.8,1.0};
    colors["Pd"]={0.0,0.41,0.52,1.0};
    colors["Ag"]={1.0,1.0,1.0,1.0};
    colors["Cd"]={0.8,0.8,0.8,1.0};
    colors["In"]={0.8,0.8,0.8,1.0};
    colors["Sn"]={0.8,0.8,0.8,1.0};
    colors["Sb"]={0.8,0.8,0.8,1.0};
    colors["Te"]={0.8,0.8,0.8,1.0};
    colors["I"]={0.58,0.0,0.58,1.0};
    colors["Xe"]={0.8,0.8,0.8,1.0};
    colors["Cs"]={0.8,0.8,0.8,1.0};
    colors["Ba"]={0.8,0.8,0.8,1.0};
    colors["Lu"]={0.8,0.8,0.8,1.0};
    colors["Hf"]={0.8,0.8,0.8,1.0};
    colors["Ta"]={0.8,0.8,0.8,1.0};
    colors["W"]={0.8,0.8,0.8,1.0};
    colors["Re"]={0.8,0.8,0.8,1.0};
    colors["Os"]={0.8,0.8,0.8,1.0};
    colors["Ir"]={0.8,0.8,0.8,1.0};
    colors["Pt"]={0.96,0.93,0.82,1.0};
    colors["Au"]={0.93,0.94,0.117,1.0};
    colors["Hg"]={0.8,0.8,0.8,1.0};
    colors["Tl"]={0.8,0.8,0.8,1.0};
    colors["Pb"]={0.8,0.8,0.8,1.0};
    colors["Bi"]={0.8,0.8,0.8,1.0};
    colors["Po"]={0.8,0.8,0.8,1.0};
    colors["At"]={0.8,0.8,0.8,1.0};
    colors["Rn"]={0.8,0.8,0.8,1.0};
    colors["Fr"]={0.8,0.8,0.8,1.0};
    colors["Ra"]={0.8,0.8,0.8,1.0};
    colors["Lr"]={0.8,0.8,0.8,1.0};
    colors["Rf"]={0.8,0.8,0.8,1.0};
    colors["Db"]={0.8,0.8,0.8,1.0};
    colors["La"]={0.8,0.8,0.8,1.0};
    colors["Ce"]={0.8,0.8,0.8,1.0};
    colors["Pr"]={0.8,0.8,0.8,1.0};
    colors["Nd"]={0.8,0.8,0.8,1.0};
    colors["Pm"]={0.8,0.8,0.8,1.0};
    colors["Sm"]={0.8,0.8,0.8,1.0};
    colors["Eu"]={0.8,0.8,0.8,1.0};
    colors["Gd"]={0.8,0.8,0.8,1.0};
    colors["Tb"]={0.8,0.8,0.8,1.0};
    colors["Dy"]={0.8,0.8,0.8,1.0};
    colors["Ho"]={0.8,0.8,0.8,1.0};
    colors["Er"]={0.8,0.8,0.8,1.0};
    colors["Tm"]={0.8,0.8,0.8,1.0};
    colors["Yb"]={0.8,0.8,0.8,1.0};
    colors["Ac"]={0.8,0.8,0.8,1.0};
    colors["Th"]={0.8,0.8,0.8,1.0};
    colors["Pa"]={0.8,0.8,0.8,1.0};
    colors["U"]={0.8,0.8,0.8,1.0};
    colors["Np"]={0.8,0.8,0.8,1.0};
    colors["Pu"]={0.8,0.8,0.8,1.0};
    colors["Am"]={0.8,0.8,0.8,1.0};
    colors["Cm"]={0.8,0.8,0.8,1.0};
    colors["Bk"]={0.8,0.8,0.8,1.0};
    colors["Cf"]={0.8,0.8,0.8,1.0};
    colors["Es"]={0.8,0.8,0.8,1.0};
    colors["Fm"]={0.8,0.8,0.8,1.0};
    colors["Md"]={0.8,0.8,0.8,1.0};
    colors["No"]={0.8,0.8,0.8,1.0};
    colors["Sg"]={0.8,0.8,0.8,1.0};
    colors["Bh"]={0.8,0.8,0.8,1.0};
    colors["Hs"]={0.8,0.8,0.8,1.0};
    colors["Mt"]={0.8,0.8,0.8,1.0};
    colors["Ds"]={0.8,0.8,0.8,1.0};
    colors["Rg"]={0.8,0.8,0.8,1.0};
    colors["Cn"]={0.8,0.8,0.8,1.0};
    colors["Nh"]={0.8,0.8,0.8,1.0};
    colors["Fl"]={0.8,0.8,0.8,1.0};
    colors["Mc"]={0.8,0.8,0.8,1.0};
    colors["Lv"]={0.8,0.8,0.8,1.0};
    colors["Ts"]={0.8,0.8,0.8,1.0};
    colors["Og"]={0.8,0.8,0.8,1.0};



    radios["H"]=0.5;
    radios["He"]=0.5;
    radios["Li"]=1.0;
    radios["Be"]=1.0;
    radios["B"]=0.9;
    radios["C"]=0.9;
    radios["N"]=0.8;
    radios["O"]=0.8;
    radios["F"]=1.0;
    radios["Na"]=1.1;
    radios["Mg"]=1.1;
    radios["Al"]=1.3;
    radios["Si"]=0.9;
    radios["P"]=1.0;
    radios["S"]=1.09;
    radios["Cl"]=0.95;
    radios["K"]=1.2;
    radios["Ca"]=1.4;
    radios["Sc"]=1.3;
    radios["Ti"]=1.7;
    radios["V"]=1.3;
    radios["Cr"]=1.3;
    radios["Mn"]=1.3;
    radios["Fe"]=1.3;
    radios["Co"]=1.3;
    radios["Ni"]=1.3;
    radios["Cu"]=1.3;
    radios["Zn"]=1.3;
    radios["Ga"]=1.3;
    radios["Ge"]=1.3;
    radios["As"]=1.3;
    radios["Se"]=1.3;
    radios["Br"]=1.3;
    radios["Kr"]=1.3;
    radios["Rb"]=1.3;
    radios["Sr"]=1.3;
    radios["Y"]=1.3;
    radios["Zr"]=1.3;
    radios["Nb"]=1.3;
    radios["Mo"]=1.3;
    radios["Tc"]=1.3;
    radios["Ru"]=1.3;
    radios["Rh"]=1.3;
    radios["Pd"]=1.3;
    radios["Ag"]=1.3;
    radios["Cd"]=1.3;
    radios["In"]=1.3;
    radios["Sn"]=1.3;
    radios["Sb"]=1.3;
    radios["Te"]=1.3;
    radios["I"]=1.3;
    radios["Xe"]=1.3;
    radios["Cs"]=1.3;
    radios["Ba"]=1.3;
    radios["Lu"]=1.3;
    radios["Hf"]=1.3;
    radios["Ta"]=1.3;
    radios["W"]=1.3;
    radios["Re"]=1.3;
    radios["Os"]=1.3;
    radios["Ir"]=1.3;
    radios["Pt"]=1.3;
    radios["Au"]=1.3;
    radios["Hg"]=1.3;
    radios["Tl"]=1.3;
    radios["Pb"]=1.3;
    radios["Bi"]=1.3;
    radios["Po"]=1.3;
    radios["At"]=1.3;
    radios["Rn"]=1.3;
    radios["Fr"]=1.3;
    radios["Ra"]=1.3;
    radios["Lr"]=1.3;
    radios["Rf"]=1.3;
    radios["Db"]=1.3;
    radios["La"]=1.3;
    radios["Ce"]=1.3;
    radios["Pr"]=1.3;
    radios["Nd"]=1.3;
    radios["Pm"]=1.3;
    radios["Sm"]=1.3;
    radios["Eu"]=1.3;
    radios["Gd"]=1.3;
    radios["Tb"]=1.3;
    radios["Dy"]=1.3;
    radios["Ho"]=1.3;
    radios["Er"]=1.3;
    radios["Tm"]=1.3;
    radios["Yb"]=1.3;
    radios["Ac"]=1.3;
    radios["Th"]=1.3;
    radios["Pa"]=1.3;
    radios["U"]=1.3;
    radios["Np"]=1.3;
    radios["Pu"]=1.3;
    radios["Am"]=1.3;
    radios["Cm"]=1.3;
    radios["Bk"]=1.3;
    radios["Cf"]=1.3;
    radios["Es"]=1.3;
    radios["Fm"]=1.3;
    radios["Md"]=1.3;
    radios["No"]=1.3;
    radios["Sg"]=1.3;
    radios["Bh"]=1.3;
    radios["Hs"]=1.3;
    radios["Mt"]=1.3;
    radios["Ds"]=1.3;
    radios["Rg"]=1.3;
    radios["Cn"]=1.3;
    radios["Nh"]=1.3;
    radios["Fl"]=1.3;
    radios["Mc"]=1.3;
    radios["Lv"]=1.3;
    radios["Ts"]=1.3;
    radios["Og"]=1.3;

    cylinders["H"]=0.1;
    cylinders["He"]=0.2;
    cylinders["Li"]=0.2;
    cylinders["Be"]=0.2;
    cylinders["B"]=0.2;
    cylinders["C"]=0.2;
    cylinders["N"]=0.2;
    cylinders["O"]=0.2;
    cylinders["F"]=0.2;
    cylinders["Na"]=0.2;
    cylinders["Mg"]=0.2;
    cylinders["Al"]=0.2;
    cylinders["Si"]=0.2;
    cylinders["P"]=0.2;
    cylinders["S"]=0.2;
    cylinders["Cl"]=0.2;
    cylinders["K"]=0.2;
    cylinders["Ca"]=0.2;
    cylinders["Sc"]=0.2;
    cylinders["Ti"]=0.2;
    cylinders["V"]=0.2;
    cylinders["Cr"]=0.2;
    cylinders["Mn"]=0.2;
    cylinders["Fe"]=0.2;
    cylinders["Co"]=0.2;
    cylinders["Ni"]=0.2;
    cylinders["Cu"]=0.2;
    cylinders["Zn"]=0.2;
    cylinders["Ga"]=0.2;
    cylinders["Ge"]=0.2;
    cylinders["As"]=0.2;
    cylinders["Se"]=0.2;
    cylinders["Br"]=0.2;
    cylinders["Kr"]=0.2;
    cylinders["Rb"]=0.2;
    cylinders["Sr"]=0.2;
    cylinders["Y"]=0.2;
    cylinders["Zr"]=0.2;
    cylinders["Nb"]=0.2;
    cylinders["Mo"]=0.2;
    cylinders["Tc"]=0.2;
    cylinders["Ru"]=0.2;
    cylinders["Rh"]=0.2;
    cylinders["Pd"]=0.2;
    cylinders["Ag"]=0.2;
    cylinders["Cd"]=0.2;
    cylinders["In"]=0.2;
    cylinders["Sn"]=0.2;
    cylinders["Sb"]=0.2;
    cylinders["Te"]=0.2;
    cylinders["I"]=0.2;
    cylinders["Xe"]=0.2;
    cylinders["Cs"]=0.2;
    cylinders["Ba"]=0.2;
    cylinders["Lu"]=0.2;
    cylinders["Hf"]=0.2;
    cylinders["Ta"]=0.2;
    cylinders["W"]=0.2;
    cylinders["Re"]=0.2;
    cylinders["Os"]=0.2;
    cylinders["Ir"]=0.2;
    cylinders["Pt"]=0.2;
    cylinders["Au"]=0.2;
    cylinders["Hg"]=0.2;
    cylinders["Tl"]=0.2;
    cylinders["Pb"]=0.2;
    cylinders["Bi"]=0.2;
    cylinders["Po"]=0.2;
    cylinders["At"]=0.2;
    cylinders["Rn"]=0.2;
    cylinders["Fr"]=0.2;
    cylinders["Ra"]=0.2;
    cylinders["Lr"]=0.2;
    cylinders["Rf"]=0.2;
    cylinders["Db"]=0.2;
    cylinders["La"]=0.2;
    cylinders["Ce"]=0.2;
    cylinders["Pr"]=0.2;
    cylinders["Nd"]=0.2;
    cylinders["Pm"]=0.2;
    cylinders["Sm"]=0.2;
    cylinders["Eu"]=0.2;
    cylinders["Gd"]=0.2;
    cylinders["Tb"]=0.2;
    cylinders["Dy"]=0.2;
    cylinders["Ho"]=0.2;
    cylinders["Er"]=0.2;
    cylinders["Tm"]=0.2;
    cylinders["Yb"]=0.2;
    cylinders["Ac"]=0.2;
    cylinders["Th"]=0.2;
    cylinders["Pa"]=0.2;
    cylinders["U"]=0.2;
    cylinders["Np"]=0.2;
    cylinders["Pu"]=0.2;
    cylinders["Am"]=0.2;
    cylinders["Cm"]=0.2;
    cylinders["Bk"]=0.2;
    cylinders["Cf"]=0.2;
    cylinders["Es"]=0.2;
    cylinders["Fm"]=0.2;
    cylinders["Md"]=0.2;
    cylinders["No"]=0.2;
    cylinders["Sg"]=0.2;
    cylinders["Bh"]=0.2;
    cylinders["Hs"]=0.2;
    cylinders["Mt"]=0.2;
    cylinders["Ds"]=0.2;
    cylinders["Rg"]=0.2;
    cylinders["Cn"]=0.2;
    cylinders["Nh"]=0.2;
    cylinders["Fl"]=0.2;
    cylinders["Mc"]=0.2;
    cylinders["Lv"]=0.2;
    cylinders["Ts"]=0.2;
    cylinders["Og"]=0.2;

    spheres["H"]=0.25;
    spheres["He"]=1.0;
    spheres["Li"]=1.0;
    spheres["Be"]=1.0;
    spheres["B"]=1.0;
    spheres["C"]=0.4;
    spheres["N"]=0.4;
    spheres["O"]=0.36;
    spheres["F"]=0.4;
    spheres["Na"]=0.6;
    spheres["Mg"]=0.6;
    spheres["Al"]=0.6;
    spheres["Si"]=0.3;
    spheres["P"]=0.5;
    spheres["S"]=0.5;
    spheres["Cl"]=0.5;
    spheres["K"]=0.6;
    spheres["Ca"]=0.6;
    spheres["Sc"]=1.0;
    spheres["Ti"]=1.0;
    spheres["V"]=1.0;
    spheres["Cr"]=1.0;
    spheres["Mn"]=1.0;
    spheres["Fe"]=1.0;
    spheres["Co"]=0.65;
    spheres["Ni"]=0.65;
    spheres["Cu"]=0.6;
    spheres["Zn"]=0.6;
    spheres["Ga"]=0.6;
    spheres["Ge"]=0.6;
    spheres["As"]=0.6;
    spheres["Se"]=0.6;
    spheres["Br"]=0.6;
    spheres["Kr"]=0.6;
    spheres["Rb"]=0.6;
    spheres["Sr"]=0.6;
    spheres["Y"]=0.6;
    spheres["Zr"]=1.0;
    spheres["Nb"]=1.0;
    spheres["Mo"]=1.0;
    spheres["Tc"]=1.0;
    spheres["Ru"]=1.0;
    spheres["Rh"]=1.0;
    spheres["Pd"]=1.0;
    spheres["Ag"]=1.0;
    spheres["Cd"]=1.0;
    spheres["In"]=1.0;
    spheres["Sn"]=1.0;
    spheres["Sb"]=1.0;
    spheres["Te"]=1.0;
    spheres["I"]=1.0;
    spheres["Xe"]=1.0;
    spheres["Cs"]=1.0;
    spheres["Ba"]=1.0;
    spheres["Lu"]=1.0;
    spheres["Hf"]=1.0;
    spheres["Ta"]=1.0;
    spheres["W"]=1.0;
    spheres["Re"]=1.0;
    spheres["Os"]=1.0;
    spheres["Ir"]=1.0;
    spheres["Pt"]=1.0;
    spheres["Au"]=1.0;
    spheres["Hg"]=0.65;
    spheres["Tl"]=1.0;
    spheres["Pb"]=1.0;
    spheres["Bi"]=1.0;
    spheres["Po"]=1.0;
    spheres["At"]=1.0;
    spheres["Rn"]=1.0;
    spheres["Fr"]=1.0;
    spheres["Ra"]=1.0;
    spheres["Lr"]=1.0;
    spheres["Rf"]=1.0;
    spheres["Db"]=1.0;
    spheres["La"]=1.0;
    spheres["Ce"]=1.0;
    spheres["Pr"]=1.0;
    spheres["Nd"]=1.0;
    spheres["Pm"]=1.0;
    spheres["Sm"]=1.0;
    spheres["Eu"]=1.0;
    spheres["Gd"]=1.0;
    spheres["Tb"]=1.0;
    spheres["Dy"]=1.0;
    spheres["Ho"]=1.0;
    spheres["Er"]=1.0;
    spheres["Tm"]=1.0;
    spheres["Yb"]=1.0;
    spheres["Ac"]=1.0;
    spheres["Th"]=1.0;
    spheres["Pa"]=1.0;
    spheres["U"]=1.0;
    spheres["Np"]=1.0;
    spheres["Pu"]=1.0;
    spheres["Am"]=1.0;
    spheres["Cm"]=1.0;
    spheres["Bk"]=1.0;
    spheres["Cf"]=1.0;
    spheres["Es"]=1.0;
    spheres["Fm"]=1.0;
    spheres["Md"]=1.0;
    spheres["No"]=1.0;
    spheres["Sg"]=1.0;
    spheres["Bh"]=1.0;
    spheres["Hs"]=1.0;
    spheres["Mt"]=1.0;
    spheres["Ds"]=1.0;
    spheres["Rg"]=1.0;
    spheres["Cn"]=1.0;
    spheres["Nh"]=1.0;
    spheres["Fl"]=1.0;
    spheres["Mc"]=1.0;
    spheres["Lv"]=1.0;
    spheres["Ts"]=1.0;
    spheres["Og"]=1.0;



    masas["H"]=1.008;
    masas["He"]=4.0026;
    masas["Li"]=6.94;
    masas["Be"]=9.0122;
    masas["B"]=10.81;
    masas["C"]=12.01;
    masas["N"]=14.007;
    masas["O"]=15.999;
    masas["F"]=18.9984;
    masas["Si"]=28.0585;
    masas["Ni"]=58.6934;
    masas["Al"]=26.981539;
    masas["Cu"]=63.546;
    masas["Co"]=58.933195;
    masas["P"]=32.;
    masas["Cl"]=35.5;
    masas["Hg"]=200.59;
    masas["Sr"]=87.62;
}




float dist(array<float,3> point1, array<float,3> point2){
    float distancia;
    distancia = sqrt(pow((point1[0]-point2[0]), 2) + pow((point1[1]-point2[1]), 2) + pow((point1[2]-point2[2]), 2));
    return distancia;
}


float dist(float x1, float y1, float z1, float x2, float y2, float z2){
    float distancia;
    distancia = sqrt(pow((x1-x2), 2) + pow((y1-y2), 2) + pow((z1-z2), 2));
    return distancia;
}


vector<string> split(const string &s, char delim) {
    stringstream ss(s);
    string item;
    vector<string> tokens;
    while (getline(ss, item, delim)) {
        if(item.compare("") != 0){
                tokens.push_back(item);
        }
    }
    return tokens;
}


float angle(array<float, 3> p1, array<float, 3> p2, array<float, 3> p3){
    float v1[3],v2[3];
    float n1,n2, angle;

    v1[0] = p2[0] - p1[0];
    v1[1] = p2[1] - p1[1];
    v1[2] = p2[2] - p1[2];

    v2[0] = p2[0] - p3[0];
    v2[1] = p2[1] - p3[1];
    v2[2] = p2[2] - p3[2];

    n1 = sqrt(pow(v1[0],2) + pow(v1[1],2) + pow(v1[2],2));
    n2 = sqrt(pow(v2[0],2) + pow(v2[1],2) + pow(v2[2],2));

    angle = (v1[0]*v2[0] + v1[1]*v2[1] + v1[2]*v2[2])/(n1*n2);
    angle = acos(angle);
    angle = angle*180/PI;

    return angle;

}


float cos_angle(array<float, 3> v1, array<float, 3> v2){
    float n1,n2, cosangle;

    n1 = sqrt(pow(v1[0],2) + pow(v1[1],2) + pow(v1[2],2));
    n2 = sqrt(pow(v2[0],2) + pow(v2[1],2) + pow(v2[2],2));

    cosangle = (v1[0]*v2[0] + v1[1]*v2[1] + v1[2]*v2[2])/(n1*n2);
//    angle = acos(angle);
//    angle = angle*180/PI;

    return cosangle;

}

float average(vector<float> numbers){
    if(numbers.size()==0) return (float)0.0;

    float suma=0.0, ave;
    for(auto &n:numbers){
        suma += n;
    }
    ave = suma/numbers.size();
    return ave;
}

float dihedral(array<float, 3> p1, array<float, 3> p2, array<float, 3> p3, array<float, 3> p4){
    float b_a[3], b_c[3], c_d[3];
    float n1[3];
    float n2[3];
    float m[3];
    float x, y, ang;

    //b1
    b_a[0] = -(p1[0] - p2[0]);
    b_a[1] = -(p1[1] - p2[1]);
    b_a[2] = -(p1[2] - p2[2]);

    //b2
    b_c[0] = p2[0] - p3[0];
    b_c[1] = p2[1] - p3[1];
    b_c[2] = p2[2] - p3[2];

    //b3
    c_d[0] = p4[0] - p3[0];
    c_d[1] = p4[1] - p3[1];
    c_d[2] = p4[2] - p3[2];

    //normalize vectors ba, bc, cd
//    b_c =
    VectorNormalisation(b_c);
    VectorNormalisation(b_a);
    VectorNormalisation(c_d);

    CrossProduct(b_a, b_c, n1);
    CrossProduct(b_c, c_d, n2);
    CrossProduct(n1, b_c, m);

    x = DotProduct(n1, n2);
    y = DotProduct(m, n2);

    ang = 180.0 / PI * atan2(y, x);
    return ang;
}

void VectorNormalisation(float v[3]){
    float norm = sqrt(pow(v[0],2) + pow(v[1],2) + pow(v[2],2));
    v[0] /= norm; v[1] /= norm; v[2] /= norm;
}

void CrossProduct(float w[3], float v[3], float cross[3]){
    cross[0] = w[1] * v[2] - w[2] * v[1];
    cross[1] = w[2] * v[0] - w[0] * v[2];
    cross[2] = w[0] * v[1] - w[1] * v[0];
}

float DotProduct(float v[3], float w[3]) {
    return (v[0] * w[0] + v[1] * w[1] + v[2] * w[2]);
}

bool check_equal_coors(array<float, 3> &a1, array<float, 3> &a2, float umb){
    for(unsigned int i=0;i<3;i++){
        if(abs(a1[i] - a2[i]) > umb){
            return false;
        }
    }
    return true;
}

bool check_equal_replicated(array<float, 3> &a1, array<float, 3> &a2, float umb){
    for(unsigned int i=0;i<3;i++){
        if(abs(a1[i] - a2[i]) > 1-umb && abs(a1[i] - a2[i]) < 1+umb){
            return true;
        }
    }
    return false;
}

QStringList parse_symbols(QStringList &symbols){
    QStringList labels;
    bool found=false;
    for(auto &sym:symbols){
        found = false;
        //ojo esto es nuevo verificar si ahy algun error al leer cif
        sym = sym.replace(" ", "");
        //ojo esto es nuevo verificar si ahy algun error al leer cif
        for(auto l:element_symbols){
            QString element = QString::fromStdString(l);
            if(element.isEmpty()) continue;
            if(element.compare(sym)==0){
                labels.append(element);
                found = true;
                break;
            }
            //if they sym and element are not equal
            //the method replace should not return an empty string
            if(sym.startsWith(element) && element.size()==2){      // puedo tener problemas con elementos como Os y O
                labels.append(element);
                found = true;
                break;
            }
            if(sym.startsWith(element) && !sym.at(1).isLetter()){
                labels.append(element);
                found = true;
                break;
            }
        }
        if(!found){
            labels.append(sym);
        }
    }
    return labels;
}

QStringList special_split(QString line){
    /*
This is a function used to split strings with single quote
character. That is, words enclosed in single quotes will be
considered as a single word
*/
    QStringList final_list;
    QString tempstring = "";
    QChar blank = QChar(' ');
    QChar comilla = QChar('\'');
    bool comillamode = false;

    // check whether there is an even or odd number of quotes
    int numberComilla = line.count(comilla);
    if(numberComilla % 2 != 0){
        line = line.replace(comilla, "");
        final_list = line.split(" ", QString::SkipEmptyParts);
        return final_list;
    }

    for(unsigned int i=0;i<line.size();i++){
        if(line.at(i) == comilla){
            if(!comillamode){
                comillamode = true;
            }
            else{
                 comillamode = false;
                 final_list.append(tempstring);
                 tempstring = QString("");
            }
        }
        else if(comillamode){
            tempstring.append(line.at(i));
        }
        else if(line.at(i) == blank){
            if(!tempstring.isEmpty()){
                final_list.append(tempstring);
                tempstring = QString("");
            }
        }else{
           tempstring.append(line.at(i));
        }

        if(i==line.size()-1 && !tempstring.isEmpty()){
            final_list.append(tempstring);
        }
    }
    return final_list;
}


void updateMemoryUsage()
{
    long pages = sysconf(_SC_PHYS_PAGES);
    long page_size = sysconf(_SC_PAGE_SIZE);
    totalMemory =  (pages * page_size)/1024;

   int tSize = 0, resident = 0, share = 0;
   ifstream buffer("/proc/self/statm");
   buffer >> tSize >> resident >> share;
   buffer.close();

   long page_size_kb = sysconf(_SC_PAGE_SIZE) / 1024; // in case x86-64 is configured to use 2MB pages
   memUsage = resident * page_size_kb;
   availableMemory = totalMemory - memUsage;
}

//*************************GLOBAL FUNCTIONS*******************************************************

