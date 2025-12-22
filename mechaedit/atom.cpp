#include "atom.h"

Atom::Atom(array<float,3> acoor, QString alabel, int aindex)
{
    coor = acoor;
    label = alabel;
    sel = false;
    overlapped = false;
    color = colors[label];
    sphere = spheres[label];
    radio = radios[label];
    cyl = cylinders[label];
    fixed = {false, false, false};
    bonds.clear();
    index = aindex;
    oind = -1;
    rep_xyz = {1, 1, 1};
}

//Atom::Atom(array<float,3> acoor, int anumber, int aindex)
//{
//    coor = acoor;
//    label = atomic_symbols[anumber];
//    sel = false;
//    color = colors[label];
//    sphere = spheres[label];
//    radio = radios[label];
//    cyl = cylinders[label];
//    fixed = {false, false, false};
//    bonds.clear();
//    index = aindex;
//    oind = -1;
//    rep_xyz = {1, 1, 1};
//}

bool Atom::bonded(Atom &at){
    for(auto b:bonds){
        if(b.neighbor == at.index){
            return true;
        }
    }
    for(auto b:at.bonds){
        if(b.neighbor == index){
            return true;
        }
    }
    return false;
}

void Atom::update_by_label(QString lab){
    label = lab;
    color = colors[label];
    cyl = cylinders[label];
    radio = radios[label];
    sphere = spheres[label];
}

bool Atom::shift_peli(int p){
    try{
        coor = peli[p];
    }
    catch(...){
        return false;
    }
    return true;
}

void Atom::average_peli(int interval){
    vector<array<float, 3>>temppeli;
    array<float, 3> avetemp = {0.0, 0.0, 0.0};
    int cont=0;

    for(auto p:peli){
        if(cont==interval){
            // divide by interval to take average
            for(int i =0; i<3;i++){
                avetemp[i] /= interval;
            }
            // append to temppeli which stores the averaged steps
            temppeli.push_back(avetemp);
            // reset avetemp to zero
            avetemp = {0.0, 0.0, 0.0};
            // reset cont to zero
            cont = 0;
        }
        for(int i =0; i<3;i++){
            avetemp[i] +=p[i];
        }
        cont ++;
    }
    peli = temppeli;
}

QString  Atom::getInfo(){
    QString info;
    info.append("Index:          " + QString::number(index)+ "\n");
    info.append("Label:          " + label + "\n");
    info.append("Radio:          " + QString::number(radio) + "\n");
    info.append("Sphere:         " + QString::number(sphere) + "\n");
    info.append("Sigma:          " + QString::number(chemical_shift) + "\n");
    info.append("Atom Site Type: " + atom_site_label + "\n");
    info.append("Groups: ");
    for(auto g:groups){
        info.append(g + ", ");
    }
    info.append("\n");
    return info;
}
