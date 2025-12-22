#ifndef ATOM_H
#define ATOM_H

#include "globals.h"

class Atom
{
public:
    Atom(array<float,3> acoor = {0.0,0.0,0.0}, QString alabel="", int aindex=-1);
//    Atom(array<float,3> acoor = {0.0,0.0,0.0}, int anumber = -1, int aindex=-1);
//    Atom(Atom &at);
    array<float,4> color;
    float sphere;
    float radio;
    float cyl;
    float chemical_shift=0.0;
    QString label;
    bool sel = false;                   //true if atom is selected, false otherwise
    bool visible = true;
    bool covered = false;
    bool overlapped = false;
    QStringList groups;
    QString atom_site_label = "None";
    array <float, 3> coor;
    array <int, 3> rep_xyz;
    array<bool, 3> fixed;
    vector<BOND> bonds;
    vector<array<float,3>> freqdisp, pelidisp, peli;
    int index, oind = -1;  //original atom index

    bool shift_peli(int p);
    void update_by_label(QString label);
    void average_peli(int interval);
    bool operator==(const Atom &atom2)const{
        if(this->index==atom2.index){
            return true;
        }else{
            return false;
        }
    }
    bool operator==(const Atom* &atom2)const{
        if(this->index==atom2->index){
            return true;
        }else{
            return false;
        }
    }
    bool bonded(Atom &at);
    QString getInfo();
};

#endif // ATOM_H
