#include "struct.h"
#include <mutex>
#include<omp.h>

#include<chrono>

Struct::Struct()
{
    MM = 0.0;
    showcell = true;
    Xcm = 0.0, Ycm = 0.0, Zcm = 0.0;
    replication = {1, 1, 1};
    freq_disp_fracs = {-1.0, -0.8, -0.6, -0.4, -0.2, 0.0, 0.2, 0.4, 0.6, 0.8, 1.0};
    peli_pos = 0;
    fatoms.clear();
    //initialize cell
    for(unsigned int i=0;i<3;i++){
        for(unsigned int j=0;j<3;j++){
            cell[i][j] = 0.0;
        }
    }
    // set cell lines parameters
    cell_lines_widths = clw;
    cell_lines_colors = clc;

    pwscf.control ["calculation"] = "'scf'";
    pwscf.control ["outdir"] = "'scratch'";
    pwscf.control ["prefix"] = "'scf'";
    pwscf.control ["pseudo_dir"] = "'/soft/quantum-espresso/ps-gipaw-pbe'";
    pwscf.control ["restart_mode"] = "'from_scratch'";
    pwscf.control ["title"] = "'scf'";
    pwscf.control ["tprnfor"] = ".true.";
    pwscf.control ["tstress"] = ".true.";

    pwscf.system["ecutwfc"] = "80";
    pwscf.system["ibrav"] = "0";
    pwscf.system["nosym"] = ".true.";
    pwscf.system["spline_ps"] = ".true.";

    pwscf.electrons["conv_thr"] = "1e-10";
    pwscf.electrons["diago_thr_init"] = "1e-4";
    pwscf.electrons["diagonalization"] = "'cg'";
    pwscf.electrons["electron_maxstep"] = "200";
    pwscf.electrons["mixing_beta"] = "0.7";
    pwscf.electrons["mixing_mode"] = "'plain'";
    pwscf.electrons["scf_must_converge"] = ".true.";
    pwscf.ions["ion_dynamics"] = "'bfgs'";
}


void Struct::add_to_before(QString action, vector<int> indices){
    CTRLZ bef;

    // action = del and indices empty incompatible
    // indices must be ordered for del

    if(action.compare("del") == 0){
        bef.action = "del";
        set_inv_mat();
        for(auto i:indices){
            bef.atoms.push_back(atoms[i]);
        }

        glob_rotate(bef.atoms, totinvmat);
        unshift_coors(bef.atoms);
        beforedata.push_back(bef);

    }else if(action.compare("add")==0){
            bef.action = "add";
            set_inv_mat();
            for(auto i:indices){
                bef.atoms.push_back(atoms[i]);
            }
            glob_rotate(bef.atoms, totinvmat);
            unshift_coors(bef.atoms);
            beforedata.push_back(bef);
    }
    else if(action.compare("replication_all")==0){
        bef.action = "replication_all";
        bef.replication = replication;
        beforedata.push_back(bef);
    }
//    else if(action.compare("change_color")==0){ //lo estoy haciendo en la misma funcion update_color
//        bef.action = "change_color";
//        for(auto &sel:selat){
//            bef.indices.push_back(sel->index);
//            bef.colors.push_back(sel->color);
//        }
//        beforedata.push_back(bef);
//    }
}

void Struct::add_to_before(QString action, vector<array<float, 3>> coors, vector<int> indices){
     CTRLZ bef;
     // coordinates have to be restored to original values reference, in case struct is rotated
     if(action.compare("dragged") == 0){
        set_inv_mat();
        //store the indices of the atoms dragged
        // ohh I am really storing the whole atom, I should change this
        // to store only the coors and indices, well I have to rotate them so that's why
        // I rotate the whole atoms and I dont newcoor and oldcoor in CTRL struct
        if(indices.size()>0){
            for(auto &in:indices){
                bef.atoms.push_back(atoms[in]);
            }
        }else{
            for(auto &sel:selat){
                bef.atoms.push_back(*sel);
            }
        }
        bef.coors = coors;
        bef.action = "dragged";
        //inverse rotation to the original coordinates systems
        glob_rotate(bef.coors, totinvmat);
        glob_rotate(bef.atoms, totinvmat);
        unshift_coors(bef.coors);
        unshift_coors(bef.atoms);
        beforedata.push_back(bef);
     }
}


void Struct:: undo(){
    if(beforedata.size()==0){
        return;
    }

    CTRLZ bef = beforedata[beforedata.size()-1];
//    afterdata.push_back(bef);
    if(bef.action.compare("del")==0){
        afterdata.push_back(bef);
        /*they have to be shifted and rotated because in
        add_to_before they are restore to the original reference*/
        shift_coors(bef.atoms);
        glob_rotate(bef.atoms, trotmat);

        //-------------------------------------------------
        for(auto &at:bef.atoms){
            Atom atom = clone_atoms(at);
            atom.index = at.index;
            vector <Atom>::iterator it;
            it = atoms.begin();
            atoms.insert(it+atom.index, atom);
        }
        //update indices, there must be a more efficient way
        for(unsigned int i=0;i<atoms.size();i++){
            atoms[i].index = i;
        }
       genbonds();
       emit data_changed();
    }
    else if(bef.action.compare("add")==0){
        afterdata.push_back(bef);
        int dec=0;
        for(auto at:bef.atoms){
            atoms.erase(atoms.begin()+at.index-dec);
            dec += 1;
        }
       genbonds();
       emit data_changed();
    }
    else if(bef.action.compare("dragged")==0){
        afterdata.push_back(bef);
        shift_coors(bef.coors);
        glob_rotate(bef.coors, trotmat);
        for(unsigned int i=0;i<bef.atoms.size();i++){
            int in = bef.atoms[i].index;
            atoms[in].coor = bef.coors[i];
        }
       genbonds();
       emit data_changed();
    }
    else if(bef.action.compare("replication_all")==0){
        array<int, 3> current_replication;
        current_replication = replication;
        if(replication[0] !=1 || replication[1]!=1 || replication[2]!=1){
            unReplicate();
        }
        replication = bef.replication;
        set_inv_mat();
        glob_rotate(atoms, totinvmat);
        replicate(replication);
        glob_rotate(atoms, trotmat);
        genbonds();
        bef.replication = current_replication;
        afterdata.push_back(bef);
    }
    else if(bef.action == "change_color"){
        vector<array<float, 4>> tempColors;
        for(unsigned int i=0;i<bef.indices.size();i++){
            unsigned int in = bef.indices[i]; //grab the index of the atom that had color i
            tempColors.push_back(atoms[in].color);
            atoms[in].color = bef.colors[i];
        }
        bef.colors = tempColors;
        afterdata.push_back(bef);
    }
    else if(bef.action == "change_sphere"){
        vector<float> tempSpheres;
        for(unsigned int i=0;i<bef.indices.size();i++){
            unsigned int in = bef.indices[i];
            tempSpheres.push_back(atoms[in].sphere);
            atoms[in].sphere = bef.spheres[i];
        }
        bef.spheres = tempSpheres;
        afterdata.push_back(bef);
    }
    else if(bef.action == "change_radius"){
        vector<float> tempRadius;
        for(unsigned int i=0;i<bef.indices.size();i++){
            unsigned int in = bef.indices[i];
            tempRadius.push_back(atoms[in].radio);
            atoms[in].radio = bef.radios[i];
        }
        bef.spheres = tempRadius;
        afterdata.push_back(bef);
        genbonds();
    }
    else if(bef.action=="change_element"){
         vector<float> tempRadii;
         vector<float> tempSpheres;
         vector<float> tempCyls;
         QStringList tempLabels;
         vector<array<float,4>> tempColors;
         for(unsigned int i=0;i<bef.indices.size();i++){
             unsigned int in = bef.indices[i];
             tempRadii.push_back(atoms[in].radio);
             tempSpheres.push_back(atoms[in].sphere);
             tempCyls.push_back(atoms[in].cyl);
             tempColors.push_back(atoms[in].color);
             tempLabels.append(atoms[in].label);

             atoms[in].radio = bef.radios[i];
             atoms[in].sphere = bef.spheres[i];
             atoms[in].cyl = bef.cyls[i];
             atoms[in].color = bef.colors[i];
             atoms[in].label = bef.labels[i];

         }

         bef.radios = tempRadii;
         bef.spheres = tempSpheres;
         bef.cyls = tempCyls;
         bef.colors = tempColors;
         bef.labels = tempLabels;
         afterdata.push_back(bef);
         genbonds();
         emit data_changed();
    }
    beforedata.pop_back();
}

void Struct::redo(){
    if(afterdata.size()==0){
        return;
    }

    CTRLZ af = afterdata[afterdata.size()-1];
//    beforedata.push_back(af);
    if(af.action.compare("del")==0){
        beforedata.push_back(af);
        int dec = 0;
        //erase the atoms with index i from atoms vector
        for(auto &at:af.atoms){
            atoms.erase(atoms.begin()+at.index-dec);
            dec += 1;
        }

        //fix indices of the atoms not deleted
        for(unsigned int i=0;i<atoms.size();i++){
            atoms[i].index = i;
        }
        genbonds();
    }
    else if(af.action.compare("dragged")==0){
       beforedata.push_back(af);
        shift_coors(af.atoms);
        glob_rotate(af.atoms, trotmat);
        for(auto at:af.atoms){
            int in = at.index;
            atoms[in].coor = at.coor;
        }
        genbonds();
        emit data_changed();
    }
    else if(af.action.compare("add")==0){
        beforedata.push_back(af);
        shift_coors(af.coors);
        glob_rotate(af.coors, trotmat);
        for(auto at:af.atoms){
            atoms.push_back(at);
        }
    }
    else if(af.action.compare("replication_all")==0){
        array<int,3> current_replication;
        current_replication[0] = replication[0];
        current_replication[1] = replication[1];
        current_replication[2] = replication[2];
        if(replication[0] !=1 || replication[1]!=1 || replication[2]!=1){
            unReplicate();
        }
        replication = af.replication;
        set_inv_mat();
        glob_rotate(atoms, totinvmat);
        replicate(replication);
        glob_rotate(atoms, trotmat);
        genbonds();
        af.replication[0] = current_replication[0];
        af.replication[1] = current_replication[1];
        af.replication[2] = current_replication[2];
        beforedata.push_back(af);
    }
    else if(af.action=="change_color"){
        vector<array<float, 4>> tempColors;
        for(unsigned int i=0;i<af.indices.size();i++){
            unsigned int in = af.indices[i];
            tempColors.push_back(atoms[in].color);
            atoms[in].color = af.colors[i];
        }
        af.colors = tempColors;
        beforedata.push_back(af);
    }
    else if(af.action=="change_sphere"){
        vector<float> tempSpheres;
        for(unsigned int i=0;i<af.indices.size();i++){
            unsigned int in = af.indices[i];
            tempSpheres.push_back(atoms[in].sphere);
            atoms[in].sphere = af.spheres[i];
        }
        af.spheres = tempSpheres;
        beforedata.push_back(af);
    }
    else if(af.action=="change_radius"){
        vector<float> tempRadius;
        for(unsigned int i=0;i<af.indices.size();i++){
            unsigned int in = af.indices[i];
            tempRadius.push_back(atoms[in].radio);
            atoms[in].radio = af.radios[i];
        }
        af.spheres = tempRadius;
        beforedata.push_back(af);
        genbonds();
    }
    else if(af.action=="change_element"){
         vector<float> tempRadii;
         vector<float> tempSpheres;
         vector<float> tempCyls;
         vector<array<float,4>> tempColors;
         QStringList tempLabels;
         for(unsigned int i=0;i<af.indices.size();i++){
             unsigned int in = af.indices[i];
             tempRadii.push_back(atoms[in].radio);
             tempSpheres.push_back(atoms[in].sphere);
             tempCyls.push_back(atoms[in].cyl);
             tempColors.push_back(atoms[in].color);
             tempLabels.append(atoms[in].label);

             atoms[in].radio = af.radios[i];
             atoms[in].sphere = af.spheres[i];
             atoms[in].cyl = af.cyls[i];
             atoms[in].color = af.colors[i];
             atoms[in].label = af.labels[i];
         }

         af.radios = tempRadii;
         af.spheres = tempSpheres;
         af.cyls = tempCyls;
         af.colors = tempColors;
         af.labels = tempLabels;
         beforedata.push_back(af);
         genbonds();
         emit data_changed();
    }
    afterdata.pop_back();
}

void Struct::round_finalcoors(){
    /* this function is supposed to round
   the direct coordinates that will be used to write file*/
    for(auto &i:fatoms){
        for(unsigned int j=0;j<3;j++){
            if(abs(i.coor[j]) < 1.0e-6){
                i.coor[j] = 0.0;
            }
//            if(i.coor[j]>1.0 && i.coor[j]<2.0){
//                if(abs(i.coor[j]-1) < 1.0e-6){
//                    i.coor[j] = 0.0;
//                }
//            }
        }
    }
}

void Struct::drag(float dx, float dy, int atom){
    if(atom != -1){
        if(!atoms[atom].visible){
            return;
        }
        atoms[atom].coor[0] += dx;
        atoms[atom].coor[1] += dy;
    }else{
        for(auto s:selat){
            if(!s->visible){continue;}
            s->coor[0] += dx;
            s->coor[1] += dy;
        }
    }
}

bool Struct::add_rectangular_selection(int selrectx, int selrecty, int mouseX, int mouseY){
    bool found = false;
    if(selrectx < mouseX && selrecty > mouseY){
        for(auto &at:atoms){
            if(!at.visible){
                continue;
            }
            if(at.coor[0] > selrectx && at.coor[0] < mouseX && at.coor[1] < selrecty && at.coor[1] > mouseY){
                auto it = find(selat.begin(), selat.end(), &at);
                if(it == selat.end()){
                    found = true;
                    selat.push_back(&at);
                    at.sel = true;
                }
            }
        }
    }
    else if(selrectx > mouseX && selrecty > mouseY){
        for(auto &at:atoms){
            if(!at.visible){
                continue;
            }
            if(at.coor[0] < selrectx && at.coor[0] > mouseX && at.coor[1] < selrecty && at.coor[1] > mouseY){
                auto it = find(selat.begin(), selat.end(), &at);
                if(it == selat.end()){
                    found = true;
                    selat.push_back(&at);
                    at.sel = true;
                }
            }
        }
    }
    else if(selrectx > mouseX && selrecty < mouseY){
        for(auto &at:atoms){
            if(!at.visible){
                continue;
            }
            if(at.coor[0] < selrectx && at.coor[0] > mouseX && at.coor[1] > selrecty && at.coor[1] < mouseY){
                auto it = find(selat.begin(), selat.end(), &at);
                if(it == selat.end()){
                    found = true;
                    selat.push_back(&at);
                    at.sel = true;
                }
            }
        }
    }
    else if(selrectx < mouseX && selrecty < mouseY){
        for(auto &at:atoms){
            if(!at.visible){
                continue;
            }
            if(at.coor[0] > selrectx && at.coor[0] < mouseX && at.coor[1] > selrecty && at.coor[1] < mouseY){
                auto it = find(selat.begin(), selat.end(), &at);
                if(it == selat.end()){
                    found = true;
                    selat.push_back(&at);
                    at.sel = true;
                }
            }
        }
    }
    return found;
}



vector<int> Struct::get_bonded_atoms(int at){
    vector<int> bonded;
    for(auto b:atoms[at].bonds){
        bonded.push_back(b.neighbor);
    }

    for(auto &a:atoms){
        for(auto &b:a.bonds){
            if(b.neighbor== at){
                bonded.push_back(a.index);
            }
        }
    }
    return bonded;
}

vector<int> Struct::reversed_bond_search(int bonds, vector<int>atomslist){
    vector<int> partiallist;
    vector<int> totallist;

     if(bonds > 0){
        bonds--;
        for(unsigned int a=0; a<atomslist.size();a++){
            partiallist = get_bonded_atoms(atomslist[a]);
            for(unsigned int p=0;p<partiallist.size();p++){
                //check whether it has already been add to totallist
                auto it=find(totallist.begin(), totallist.end(), partiallist[p]);
                if(it == totallist.end()){
                    totallist.push_back(partiallist[p]);
                }
                //check whether it has already been add to totallist
            }
            partiallist = reversed_bond_search(bonds, partiallist);
            for(unsigned int p=0;p<partiallist.size();p++){
                //check whether it has already been add to totallist
                auto it=find(totallist.begin(), totallist.end(), partiallist[p]);
                if(it == totallist.end()){
                    totallist.push_back(partiallist[p]);
                }
            }

        }
    }

    return totallist;
}

//vector<int> Struct::bondedRing(int bonds, vector<int>atomslist){
//    vector<vector<int>> totalPartialList;
//    vector<int> partialList;
//    vector<int> totalList;

//     if(bonds > 0){
//        bonds--;
//        for(unsigned int a=0; a<atomslist.size();a++){
//            partialList = get_bonded_atoms(atomslist[a]);
//            for(unsigned int p=0;p<partialList.size();p++){
//                //check whether it has already been add to totallist ----------------------
//                auto it=find(totalList.begin(), totalList.end(), partialList[p]);
//                if(it == totalList.end()){
//                    totalList.push_back(partialList[p]);
//                }
//                else{
//                    //se cerro el ciclo, el problema es si es del tamaño que yo quiero
//                }
//                //check whether it has already been add to totallist ----------------------
//            }

////            totalPartialList.emplace_back(partialList);
//        }
//    }
//    return totallist;
//}

void Struct::add_atom(float x, float y, QString element){
    array<float, 3> tempcoor = {x, y, 0.0};
    int in = atoms.size();  //index that the new atoms will have
    Atom atom(tempcoor, element, in);
    atoms.push_back(atom);
    vector<int> indices = {in};
    add_to_before("add", indices);
    genbonds();
}


bool Struct::add_to_selection(float x, float y){
    float distx, disty, maxz=-10000;
    Atom *fs;
    bool found = false;
    for(auto &i:atoms){
        if(!i.visible){
            continue;
        }
        distx = abs(x - i.coor[0]);
        disty = abs(y - i.coor[1]);
        if(distx < i.sphere && disty < i.sphere && i.coor[2] >= maxz){
            fs = &i;
            maxz = i.coor[2];
            found = true;
        }
    }

    if (!found) return false;
    auto it = find(selat.begin(), selat.end(), fs);
    if(it != selat.end()){
        fs->sel = false;
        selat.erase(it);
        return false;
    }else{
        fs->sel = true;
        selat.push_back(fs);
        return true;
    }
}

bool Struct::add_to_selection(vector<int> index){
    if(index.size()==0) return false;
    for(auto &sel:selat){
        sel->sel = false;
    }

    selat.clear();
    for(auto i:index){
        atoms[i].sel = true;
        selat.push_back(&atoms[i]);
    }
    return true;
}

void Struct::select_all(){
    selat.clear();
    for(auto &at:atoms){
        if(!at.visible){
            continue;
        }
        at.sel = true;
        selat.push_back(&at);
    }
}

void Struct::frac_cart(vector <Atom> &coors, bool tocart, bool peli){
    array<array<float,3>,3> matrix;
    float a,b,c, v, sin_gamma;
    float cos_alpha, cos_beta, cos_gamma, suma[3];
//    vector<Atom>:: iterator it;

    a = sqrt(pow(cell[0][0],2) + pow(cell[0][1],2) + pow(cell[0][2],2));
    b = sqrt(pow(cell[1][0],2) + pow(cell[1][1],2) + pow(cell[1][2],2));
    c = sqrt(pow(cell[2][0],2) + pow(cell[2][1],2) + pow(cell[2][2],2));

    cos_alpha = (cell[1][0]*cell[2][0] + cell[1][1]*cell[2][1]+cell[1][2]*cell[2][2])/(b*c);
    cos_beta = (cell[0][0]*cell[2][0] + cell[0][1]*cell[2][1]+cell[0][2]*cell[2][2])/(a*c);
    cos_gamma = (cell[0][0]*cell[1][0] + cell[0][1]*cell[1][1]+cell[0][2]*cell[1][2])/(a*b);
    sin_gamma = sqrt(1-pow(cos_gamma,2));
    v = sqrt( 1 - pow(cos_alpha,2) - pow(cos_beta,2) -  pow(cos_gamma,2) + 2 * cos_alpha * cos_beta * cos_gamma );
// wiki pedia ------------------------------------------------
    if(tocart){
        matrix[0] = {a , b * cos_gamma, c * cos_beta };
        matrix[1] = {0, b * sin_gamma, c * (cos_alpha - cos_beta * cos_gamma)/(sin_gamma)};
        matrix[2] = {0,0, c * v/sin_gamma};
    }else{
        matrix[0] = {1/a, -cos_gamma/(a*sin_gamma), (cos_alpha * cos_gamma - cos_beta)/(a*v*sin_gamma)};
        matrix[1] = { 0, 1/(b * sin_gamma), (cos_beta * cos_gamma - cos_alpha)/(b*v*sin_gamma)};
        matrix[2] = {0, 0, sin_gamma/(c*v)};
    }


   //     multiplico las coordenadas ****************************************
   for(auto it=coors.begin();it!=coors.end();it++){
           suma[0] = 0.0;
           suma[1] = 0.0;
           suma[2] = 0.0;
       for(unsigned int row=0;row<3;row++){
           for(unsigned int rowcol=0;rowcol<3;rowcol++){
              suma[row] += it->coor[rowcol]*matrix[row][rowcol];
           }
       }
       it->coor[0] = suma[0];
       it->coor[1] = suma[1];
       it->coor[2] = suma[2];

       if(peli){                             //bool in case the peli needs to be transformed
           for(auto &p:it->peli){
               suma[0] = 0.0;
               suma[1] = 0.0;
               suma[2] = 0.0;
               for(unsigned int row=0;row<3;row++){
                   for(unsigned int rowcol=0;rowcol<3;rowcol++){
                      suma[row] += p[rowcol]*matrix[row][rowcol];
                   }
               }
               p[0] = suma[0];
               p[1] = suma[1];
               p[2] = suma[2];
           }
       }
    }
 // multiplico las coordenadas ****************************************
}

void Struct::frac_cart(vector <Atom*> &coors, bool tocart, bool peli){
    array<array<float,3>,3> matrix;
    float a, b, c, v, sin_gamma;
    float cos_alpha, cos_beta, cos_gamma, suma[3];
//    vector<Atom>:: iterator it;


    a = sqrt(pow(cell[0][0],2) + pow(cell[0][1],2) + pow(cell[0][2],2));
    b = sqrt(pow(cell[1][0],2) + pow(cell[1][1],2) + pow(cell[1][2],2));
    c = sqrt(pow(cell[2][0],2) + pow(cell[2][1],2) + pow(cell[2][2],2));

    cos_alpha = (cell[1][0]*cell[2][0] + cell[1][1]*cell[2][1]+cell[1][2]*cell[2][2])/(b*c);
    cos_beta = (cell[0][0]*cell[2][0] + cell[0][1]*cell[2][1]+cell[0][2]*cell[2][2])/(a*c);
    cos_gamma = (cell[0][0]*cell[1][0] + cell[0][1]*cell[1][1]+cell[0][2]*cell[1][2])/(a*b);
    sin_gamma = sqrt(1-pow(cos_gamma,2));

    v = sqrt( 1 - pow(cos_alpha,2) - pow(cos_beta,2) - pow(cos_gamma,2) + 2 * cos_alpha * cos_beta * cos_gamma );

    if(tocart){
        matrix[0] = {a , b * cos_gamma, c * cos_beta };
        matrix[1] = {0, b * sin_gamma, c * (cos_alpha - cos_beta * cos_gamma)/(sin_gamma)};
        matrix[2] = {0,0, (c*v)/sin_gamma};
    }else{
        matrix[0] = {1/a, -cos_gamma/(a*sin_gamma), (cos_alpha * cos_gamma - cos_beta)/(a*v*sin_gamma)};
        matrix[1] = { 0, 1/(b * sin_gamma), (cos_beta * cos_gamma - cos_alpha)/(b*v*sin_gamma)};
        matrix[2] = {0, 0, sin_gamma/(c*v)};
    }

    // multiplico las coordenadas ****************************************
   for(auto &it:coors){
           suma[0] = 0.0;
           suma[1] = 0.0;
           suma[2] = 0.0;
       for(unsigned int row=0;row<3;row++){
           for(unsigned int rowcol=0;rowcol<3;rowcol++){
              suma[row] += it->coor[rowcol]*matrix[row][rowcol];
           }
       }
       it->coor[0] = suma[0];
       it->coor[1] = suma[1];
       it->coor[2] = suma[2];
       if(peli){                             //bool in case the peli needs to be transformed
           for(auto &p:it->peli){
               suma[0] = 0.0;
               suma[1] = 0.0;
               suma[2] = 0.0;
               for(unsigned int row=0;row<3;row++){
                   for(unsigned int rowcol=0;rowcol<3;rowcol++){
                      suma[row] += p[rowcol]*matrix[row][rowcol];
                   }
               }
               p[0] = suma[0];
               p[1] = suma[1];
               p[2] = suma[2];
           }
       }
    }
 // multiplico las coordenadas ****************************************
}


void Struct::glob_rotate(vector<Atom*> &refatoms, float rotmat[3][3]){
    float suma[3] = {0.0,0.0,0.0};
    // multiplico las coordenadas ****************************************
   for(auto i:refatoms){
           suma[0] = 0.0;
           suma[1] = 0.0;
           suma[2] = 0.0;
       for(unsigned int row=0;row<3;row++){
           for(unsigned int rowcol=0;rowcol<3;rowcol++){
              suma[row] += i->coor[rowcol]*rotmat[row][rowcol];
           }
       }
       i->coor[0] = suma[0];
       i->coor[1] = suma[1];
       i->coor[2] = suma[2];
    }

 // multiplico las coordenadas ****************************************
}


void Struct::glob_rotate(vector<Atom> &refatoms, float rotmat[3][3]){
    float suma[3] = {0.0,0.0,0.0};
    // multiplico las coordenadas ****************************************
//    #pragma omp parallel private(suma)
//    {
//        printf("%i\n", omp_get_thread_num());
//    #pragma omp for
       for(auto i=refatoms.begin();i!=refatoms.end();i++){
               suma[0] = 0.0;
               suma[1] = 0.0;
               suma[2] = 0.0;
           for(unsigned int row=0;row<3;row++){
               for(unsigned int rowcol=0;rowcol<3;rowcol++){
                  suma[row] += i->coor[rowcol]*rotmat[row][rowcol];
               }
           }
           i->coor[0] = suma[0];
           i->coor[1] = suma[1];
           i->coor[2] = suma[2];
        }
//    }

 // multiplico las coordenadas ****************************************
}

void Struct::glob_rotate(vector<array<float,3>> &refcoors, float rotmat[3][3]){
    float suma[3] = {0.0,0.0,0.0};
    // multiplico las coordenadas ****************************************
   for(unsigned int i=0;i<refcoors.size();i++){
           suma[0] = 0.0;
           suma[1] = 0.0;
           suma[2] = 0.0;
       for(unsigned int row=0;row<3;row++){
           for(unsigned int rowcol=0;rowcol<3;rowcol++){
              suma[row] += refcoors[i][rowcol]*rotmat[row][rowcol];
           }
       }
       refcoors[i][0] = suma[0];
       refcoors[i][1] = suma[1];
       refcoors[i][2] = suma[2];
    }

 // multiplico las coordenadas ****************************************
}

void Struct::glob_rotate(float refcoors[3], float rotmat[3][3]){
    float suma[3] = {0.0,0.0,0.0};
    for(unsigned int row=0;row<3;row++){
       for(unsigned int rowcol=0;rowcol<3;rowcol++){
          suma[row] += refcoors[rowcol]*rotmat[row][rowcol];
       }
    }
    for(unsigned int i=0;i<3;i++){
        refcoors[i] = suma[i];
    }
}


void Struct::glob_rotate(array<float,3> &refcoors, float rotmat[3][3]){
    /*function que multiplica una matrix por un vector
    el vector es un vector fila.  El ciclo interno itera sobre las columnas del vector
*/
    float suma[3] = {0.0,0.0,0.0};
    for(unsigned int row=0;row<3;row++){
       for(unsigned int col=0;col<3;col++){
          suma[row] += refcoors[col]*rotmat[row][col];
       }
    }
    for(unsigned int i=0;i<3;i++){
        refcoors[i] = suma[i];
    }
}


void Struct::glob_rotate(float refcoors[3][3], float rotmat[3][3]){
    /* multiplico dos matrices 3x3, este es diferente a las otras implementaciones
   porque en las otras multiplico un vector para hacerlo como las otras tendria que
transponer los vectores filas por columnas*/

    float tempmatmul[3][3] ={{0.0,0.0,0.0},{0.0,0.0,0.0},{0.0,0.0,0.0}};
    for(int row=0;row<3;row++){
            for(int col=0;col<3;col++){
                for(int rowcol=0;rowcol<3;rowcol++){
                    tempmatmul[row][col] += rotmat[row][rowcol]* refcoors[rowcol][col];
                }
            }
        }
        for(int i=0;i<3;i++){
            for(int j=0;j<3;j++){
                refcoors[i][j] = tempmatmul[i][j];
            }
        }
}

void Struct::glob_rotate(array<array<float,3>,24> &refcoors, float rotmat[3][3]){
    float suma[3] = {0.0,0.0,0.0};
    // multiplico las coordenadas ****************************************
   for(unsigned int i=0;i<24;i++){
           suma[0] = 0.0;
           suma[1] = 0.0;
           suma[2] = 0.0;
       for(unsigned int row=0;row<3;row++){
           for(unsigned int rowcol=0;rowcol<3;rowcol++){
              suma[row] += refcoors[i][rowcol]*rotmat[row][rowcol];
           }
       }
       refcoors[i][0] = suma[0];
       refcoors[i][1] = suma[1];
       refcoors[i][2] = suma[2];
    }

 // multiplico las coordenadas ****************************************
}

void Struct::update_selection_coors(array<float,3> newcoor, int index){
    /* updates the coordinates of the selected atoms
    newcoor is the new coordinate, index is the index of the atom coordinate
    will be updated. if index is 3 then all three indices of the atom coordinate
    are updated, but it does not make sense when there are more than one atoms selected.
    ALl of them will have the same coordinates, useless chemical structure
*/
    set_inv_mat();
    if(index>2 || index < 0){
        for(auto &s:selat){
            update_coor(newcoor, s->index);
        }
    }else{
        for(auto &s:selat){
            glob_rotate(s->coor, totinvmat);
            unshift_coors(s->coor);
            s->coor[index] = newcoor[index];
            update_coor(s->coor, s->index);
        }
    }
}

void Struct::update_coor(array<float,3> &refcoors, int index){
    vector<array<float, 3>> tempcoors;  // vector to store the oldcoors to send it to add_to_before
    vector<int> tempindices;  // vector to store the indices to add_to_before
    refcoors[0] -= Xcm;
    refcoors[1] -= Ycm;
    refcoors[2] -= Zcm;

    glob_rotate(refcoors, trotmat);
    tempcoors.push_back(atoms[index].coor);
    tempindices.push_back(index);
    atoms[index].coor[0] = refcoors[0];
    atoms[index].coor[1] = refcoors[1];
    atoms[index].coor[2] = refcoors[2];
    add_to_before("dragged", tempcoors, tempindices);
}

void Struct::translate_selection(float x, float y, float z){
    set_inv_mat();

    glob_rotate(selat, totinvmat);
    for(auto sel:selat){
        sel->coor[0] += x;
        sel->coor[1] += y;
        sel->coor[2] += z;
    }
    glob_rotate(selat, trotmat);
}

void Struct::translateSelectionTo(float coor[3]){
    coor[0] -= Xcm;
    coor[1] -= Ycm;
    coor[2] -= Zcm;

    for(auto sel:selat){
        sel->coor[0] = coor[0];
        sel->coor[1] = coor[1];
        sel->coor[2] = coor[2];
    }
    glob_rotate(selat, trotmat);
}

void Struct::reset_mat(float mat[3][3]){
    mat[0][0] = 1.0;
    mat[0][1] = 0.0;
    mat[0][2] = 0.0;
    mat[1][0] = 0.0;
    mat[1][1] = 1.0;
    mat[1][2] = 0.0;
    mat[2][0] = 0.0;
    mat[2][1] = 0.0;
    mat[2][2] = 1.0;
}

void Struct::reverse_rotation(int which){
    float suma[3];
    Atom atom;

    set_inv_mat();

    if(which == 0){
        fatoms.clear();
        // multiplico las coordenadas ****************************************
       for(auto &i:atoms){
           suma[0] = 0.0;
           suma[1] = 0.0;
           suma[2] = 0.0;
           for(unsigned int row=0;row<3;row++){
               for(unsigned int rowcol=0;rowcol<3;rowcol++){
                  suma[row] += i.coor[rowcol]*totinvmat[row][rowcol];
               }
           }
           atom = clone_atoms(i);
           atom.coor[0] = suma[0]+Xcm;
           atom.coor[1] = suma[1]+Ycm;
           atom.coor[2] = suma[2]+Zcm;
           atom.index = i.index;

           fatoms.push_back(atom);
        }
         // multiplico las coordenadas ****************************************
    }
    else{
        glob_rotate(atoms, totinvmat);
        glob_rotate(cell_lines, totinvmat);
    }
}


void Struct::rotate_coors(float ang, float x, float y, float z){
//        #include <chrono>
//        auto start = std::chrono::system_clock::now();

//    mutex m;

    float matrix[3][3], mod;
    mod = sqrt(pow(x,2)+pow(y,2)+pow(z,2));

    if(mod == 0.0) return;
    ang *= mod;

    x /= mod;
    y /= mod;
    z /= mod;

    matrix[0][0] = cos(ang) + pow(x,2)*(1-cos(ang));
    matrix[0][1] = x*y*(1-cos(ang))-z*sin(ang);
    matrix[0][2] = x*z*(1-cos(ang)) + y*sin(ang);
    matrix[1][0] = x*y*(1-cos(ang)) + z*sin(ang);
    matrix[1][1] = cos(ang)+pow(y,2)*(1-cos(ang));
    matrix[1][2] = y*z*(1-cos(ang))-x*sin(ang);
    matrix[2][0] = z*x*(1-cos(ang))-y*sin(ang);
    matrix[2][1] = z*y*(1-cos(ang))+x*sin(ang);
    matrix[2][2] = cos(ang)+pow(z,2)*(1-cos(ang));


    //multiplico la  mtriz**************************************************************
        glob_rotate(trotmat, matrix);
    //multiplico la  mtriz**************************************************************

    // multiplico las coordenadas ****************************************
    glob_rotate(atoms, matrix);
     // multiplico las coordenadas ****************************************

     // multiplico las celda unidad ****************************************
    glob_rotate(cell_lines, matrix);
     // multiplico las celda ****************************************

    //try to find atoms that are not necessary to be rendered by checking if other atoms are over them on z axis
    //check_covered();
    //try to find atoms that are not necessary to be rendered by checking if other atoms are over them on z axis

//      multiplico peli si es mayor que cero
//     if(peli.size() > 0){
//        for(unsigned int p=0;p<peli.size();p++){
//           for(unsigned int i=0;i<peli[p].size();i++){
//               suma[0] = 0.0;
//               suma[1] = 0.0;
//               suma[2] = 0.0;
//               for(unsigned int row=0;row<3;row++){
//                   for(unsigned int rowcol=0;rowcol<3;rowcol++){
//                      suma[row] += peli[p][i][rowcol]*matrix[row][rowcol];
//                   }
//               }
//               m.lock();
//               peli[p][i][0] = suma[0];
//               peli[p][i][1] = suma[1];
//               peli[p][i][2] = suma[2];
//               m.unlock();
//            }
//        }
//     }

     //falta multiplicar la pelicelda

//        auto end = std::chrono::system_clock::now();
//        std::chrono::duration<double> elapsed_seconds = end-start;
//        std::cout << "elapsed time: " << elapsed_seconds.count() << "s\n";

}


void Struct::rotate_selection(float ang, float x, float y, float z){
    float matrix[3][3], suma[3] = {0.0,0.0,0.0}, mod;
    float lxcm=0.0, lycm=0.0, lzcm=0.0;
    int nat;

    mod = sqrt(pow(x,2)+pow(y,2)+pow(z,2));
    if(mod == 0.0) return;
    ang *= mod;
    x /= mod;
    y /= mod;
    z /= mod;


    matrix[0][0] = cos(ang) + pow(x,2)*(1-cos(ang));
    matrix[0][1] = x*y*(1-cos(ang))-z*sin(ang);
    matrix[0][2] = x*z*(1-cos(ang)) + y*sin(ang);
    matrix[1][0] = x*y*(1-cos(ang)) + z*sin(ang);
    matrix[1][1] = cos(ang)+pow(y,2)*(1-cos(ang));
    matrix[1][2] = y*z*(1-cos(ang))-x*sin(ang);
    matrix[2][0] = z*x*(1-cos(ang))-y*sin(ang);
    matrix[2][1] = z*y*(1-cos(ang))+x*sin(ang);
    matrix[2][2] = cos(ang)+pow(z,2)*(1-cos(ang));

    nat = selat.size();
    //TRASLADO LAS COORDENADAS A CENTRO DE MASA
    for(auto sel:selat){
        lxcm += sel->coor[0];
        lycm += sel->coor[1];
        lzcm += sel->coor[2];
    }

    lxcm /= nat;
    lycm /= nat;
    lzcm /= nat;

    //TRASLADO LAS COORDENADAS A CENTRO DE MASA


    // multiplico las coordenadas ****************************************
       for(auto sel:selat){
           sel->coor[0] -= lxcm;
           sel->coor[1] -= lycm;
           sel->coor[2] -= lzcm;
           suma[0] = 0.0;
           suma[1] = 0.0;
           suma[2] = 0.0;
           for(unsigned int row=0;row<3;row++){
               for(unsigned int rowcol=0;rowcol<3;rowcol++){
                  suma[row] += sel->coor[rowcol]*matrix[row][rowcol];
               }
           }
           sel->coor[0] = suma[0]+lxcm;
           sel->coor[1] = suma[1]+lycm;
           sel->coor[2] = suma[2]+lzcm;
        }

     // multiplico las coordenadas ****************************************
}

void Struct::set_cell_lines(){
           // de cero a a
        cell_lines[0] = {0.0,0.0,0.0};
        cell_lines[1] = {cell[0][0], cell[0][1], cell[0][2]};
        // de cero a b
        cell_lines[2] = {0.0,0.0,0.0};
        cell_lines[3] = {cell[1][0], cell[1][1], cell[1][2]};
        // de cero a c
        cell_lines[4] = {0.0,0.0,0.0};
        cell_lines[5] = {cell[2][0], cell[2][1], cell[2][2]};
        // de a a a+b
        cell_lines[6] = {cell[0][0], cell[0][1], cell[0][2]};
        cell_lines[7] = {cell[0][0]+cell[1][0], cell[0][1]+cell[1][1], cell[0][2]+cell[1][2]};
        // de  b a a+b
        cell_lines[8] = {cell[1][0], cell[1][1], cell[1][2]};
        cell_lines[9] = {cell[0][0]+cell[1][0], cell[0][1]+cell[1][1], cell[0][2]+cell[1][2]};
        // de a a a+c
        cell_lines[10] = {cell[0][0], cell[0][1], cell[0][2]};
        cell_lines[11] = {cell[0][0]+cell[2][0], cell[0][1]+cell[2][1], cell[0][2]+cell[2][2]};
        // de c a a+c
        cell_lines[12] = {cell[2][0], cell[2][1], cell[2][2]};
        cell_lines[13] = {cell[0][0]+cell[2][0], cell[0][1]+cell[2][1], cell[0][2]+cell[2][2]};
        // de  b a b+c
        cell_lines[14] = {cell[1][0], cell[1][1], cell[1][2]};
        cell_lines[15] = {cell[1][0]+cell[2][0], cell[1][1]+cell[2][1], cell[1][2]+cell[2][2]};
        // de  c a b+c
        cell_lines[16] = {cell[2][0], cell[2][1], cell[2][2]};
        cell_lines[17] = {cell[1][0]+cell[2][0], cell[1][1]+cell[2][1], cell[1][2]+cell[2][2]};
        //de b+c a a+b+c
        cell_lines[18] = {cell[1][0]+cell[2][0], cell[1][1]+cell[2][1], cell[1][2]+cell[2][2]};
        cell_lines[19] = {cell[0][0]+cell[1][0]+cell[2][0], cell[0][1]+cell[1][1]+cell[2][1],cell[0][2]+cell[1][2]+cell[2][2]};
        //de a+c a a+b+c
        cell_lines[20] = {cell[0][0]+cell[2][0], cell[0][1]+cell[2][1], cell[0][2]+cell[2][2]};
        cell_lines[21] = {cell[0][0]+cell[1][0]+cell[2][0], cell[0][1]+cell[1][1]+cell[2][1],cell[0][2]+cell[1][2]+cell[2][2]};
        //de a+b a a+b+c
        cell_lines[22] = {cell[0][0]+cell[1][0], cell[0][1]+cell[1][1], cell[0][2]+cell[1][2]};
        cell_lines[23] = {cell[0][0]+cell[1][0]+cell[2][0], cell[0][1]+cell[1][1]+cell[2][1],cell[0][2]+cell[1][2]+cell[2][2]};

}
//void Struct::set_cell_lines(){
//    float a, b, c, gamma, cos_alpha, cos_beta, cos_gamma, sin_gamma;
//    float tcell[3][3]; //temporary cell
//    a = sqrt(pow(cell[0][0],2) + pow(cell[0][1],2) + pow(cell[0][2],2));
//    b = sqrt(pow(cell[1][0],2) + pow(cell[1][1],2) + pow(cell[1][2],2));
//    c = sqrt(pow(cell[2][0],2) + pow(cell[2][1],2) + pow(cell[2][2],2));

//    cos_alpha = (cell[1][0]*cell[2][0] + cell[1][1]*cell[2][1]+cell[1][2]*cell[2][2])/(b*c);
//    cos_beta = (cell[0][0]*cell[2][0] + cell[0][1]*cell[2][1]+cell[0][2]*cell[2][2])/(a*c);
//    cos_gamma = (cell[0][0]*cell[1][0] + cell[0][1]*cell[1][1]+cell[0][2]*cell[1][2])/(a*b);
//    gamma = acos(cos_gamma);
//    sin_gamma = sin(gamma);
//    //set up temp cell ---------------------------
//    tcell[0][0] = a; //vector a
//    tcell[0][1] = 0.0;
//    tcell[0][2] = 0.0;

//    tcell[1][0] = b*cos_gamma;
//    tcell[1][1] = b*sin_gamma;
//    tcell[1][2] = 0.0;

//    //z vector components
//    tcell[2][0] = (cos_beta*b); //cos_beta*a*b/a
//    tcell[2][1] = (cos_alpha*b*c - tcell[1][0]*tcell[2][0])/tcell[1][1];
//    tcell[2][2] = sqrt(c*c - tcell[2][0]*tcell[2][0] - tcell[2][1]*tcell[2][1]);
//    //set up temp cell --------------------------------

//    // de cero a a
//    cell_lines[0] = {0.0,0.0,0.0};
//    cell_lines[1] = {tcell[0][0], tcell[0][1], tcell[0][2]};
//    // de cero a b
//    cell_lines[2] = {0.0,0.0,0.0};
//    cell_lines[3] = {tcell[1][0], tcell[1][1], tcell[1][2]};
//    // de cero a c
//    cell_lines[4] = {0.0,0.0,0.0};
//    cell_lines[5] = {tcell[2][0], tcell[2][1], tcell[2][2]};
//    // de a a a+b
//    cell_lines[6] = {tcell[0][0], tcell[0][1], tcell[0][2]};
//    cell_lines[7] = {tcell[0][0]+tcell[1][0], tcell[0][1]+tcell[1][1], tcell[0][2]+tcell[1][2]};
//    // de  b a a+b
//    cell_lines[8] = {tcell[1][0], tcell[1][1], tcell[1][2]};
//    cell_lines[9] = {tcell[0][0]+tcell[1][0], tcell[0][1]+tcell[1][1], tcell[0][2]+tcell[1][2]};
//    // de a a a+c
//    cell_lines[10] = {tcell[0][0], tcell[0][1], tcell[0][2]};
//    cell_lines[11] = {tcell[0][0]+tcell[2][0], tcell[0][1]+tcell[2][1], tcell[0][2]+tcell[2][2]};
//    // de c a a+c
//    cell_lines[12] = {tcell[2][0], tcell[2][1], tcell[2][2]};
//    cell_lines[13] = {tcell[0][0]+tcell[2][0], tcell[0][1]+tcell[2][1], tcell[0][2]+tcell[2][2]};
//    // de  b a b+c
//    cell_lines[14] = {tcell[1][0], tcell[1][1], tcell[1][2]};
//    cell_lines[15] = {tcell[1][0]+tcell[2][0], tcell[1][1]+tcell[2][1], tcell[1][2]+tcell[2][2]};
//    // de  c a b+c
//    cell_lines[16] = {tcell[2][0], tcell[2][1], tcell[2][2]};
//    cell_lines[17] = {tcell[1][0]+tcell[2][0], tcell[1][1]+tcell[2][1], tcell[1][2]+tcell[2][2]};
//    //de b+c a a+b+c
//    cell_lines[18] = {tcell[1][0]+tcell[2][0], tcell[1][1]+tcell[2][1], tcell[1][2]+tcell[2][2]};
//    cell_lines[19] = {tcell[0][0]+tcell[1][0]+tcell[2][0], tcell[0][1]+tcell[1][1]+tcell[2][1],tcell[0][2]+tcell[1][2]+tcell[2][2]};
//    //de a+c a a+b+c
//    cell_lines[20] = {tcell[0][0]+tcell[2][0], tcell[0][1]+tcell[2][1], tcell[0][2]+tcell[2][2]};
//    cell_lines[21] = {tcell[0][0]+tcell[1][0]+tcell[2][0], tcell[0][1]+tcell[1][1]+tcell[2][1],tcell[0][2]+tcell[1][2]+tcell[2][2]};
//    //de a+b a a+b+c
//    cell_lines[22] = {tcell[0][0]+tcell[1][0], tcell[0][1]+tcell[1][1], tcell[0][2]+tcell[1][2]};
//    cell_lines[23] = {tcell[0][0]+tcell[1][0]+tcell[2][0], tcell[0][1]+tcell[1][1]+tcell[2][1],tcell[0][2]+tcell[1][2]+tcell[2][2]};

//}

void Struct::shift_coors(vector<Atom> &atoms){
    for(auto i=atoms.begin();i!=atoms.end();i++){
        i->coor[0] -= Xcm;
        i->coor[1] -= Ycm;
        i->coor[2] -= Zcm;
    }
}

void Struct::shift_coors(vector<Atom*> &atoms){
    for(auto &a:atoms){
        a->coor[0] -= Xcm;
        a->coor[1] -= Ycm;
        a->coor[2] -= Zcm;
    }
}


void Struct::shift_coors(vector<array<float, 3>> &coors){
    for(auto &c:coors){
        c[0] -= Xcm;
        c[1] -= Ycm;
        c[2] -= Zcm;
    }
}

void Struct::unshift_coors(vector<Atom> &atoms){
    for(auto i=atoms.begin();i!=atoms.end();i++){
        i->coor[0] += Xcm;
        i->coor[1] += Ycm;
        i->coor[2] += Zcm;
    }
}


void Struct::unshift_coors(vector<Atom*> &atoms){
    for(auto &a:atoms){
        a->coor[0] += Xcm;
        a->coor[1] += Ycm;
        a->coor[2] += Zcm;
    }
}

void Struct::unshift_coors(vector<array<float, 3>> &coors){
    for(auto &c:coors){
        c[0] += Xcm;
        c[1] += Ycm;
        c[2] += Zcm;
    }
}


void Struct::unshift_coors(array<float,3> &coor){
    coor[0] += Xcm;
    coor[1] += Ycm;
    coor[2] += Zcm;
}


void Struct::shift_to_mass_center(bool recalcXYZ){
    //set Xcm, Ycm and Zcm to the first point in the peli
    if(!recalcXYZ){
        Xcm = animation[0].com[0];
        Ycm = animation[0].com[1];
        Zcm = animation[0].com[2];
        for(auto i=atoms.begin();i!=atoms.end();i++){
            i->coor[0] -= Xcm;
            i->coor[1] -= Ycm;
            i->coor[2] -= Zcm;
            for(unsigned j=0;j<pelisize;j++){
                i->peli[j][0] -= animation[j].com[0];
                i->peli[j][1] -= animation[j].com[1];
                i->peli[j][2] -= animation[j].com[2];
            }
        }

        // shift cell_lines
        for(unsigned int i=0;i<24;i++){
            cell_lines[i][0] -= Xcm;
            cell_lines[i][1] -= Ycm;
            cell_lines[i][2] -= Zcm;
        }
        //tengo que revisar que el peli size este bien
//        for(unsigned int i=0;i<pelisize;i++){
//            for(unsigned int j=0;j<3;j++){
//                animation[i].cell[j][0] -= animation[i].com[0];
//                animation[i].cell[j][1] -= animation[i].com[1];
//                animation[i].cell[j][2] -= animation[i].com[2];
//            }
//        }
    }
    else{
        float oldXcm = Xcm;
        float oldYcm = Ycm;
        float oldZcm = Zcm;

        Xcm = 0.0; Ycm = 0.0; Zcm = 0.0;
        // sum up the three components of COM
        for(auto &at:atoms){
            Xcm += at.coor[0];
            Ycm += at.coor[1];
            Zcm += at.coor[2];
        }
        //normalize the three components of COM
        Xcm /= atoms.size();
        Ycm /= atoms.size();
        Zcm /= atoms.size();

        for(auto &at:atoms){
            at.coor[0] -= Xcm;
            at.coor[1] -= Ycm;
            at.coor[2] -= Zcm;
        }


        // shift cell_lines
        for(unsigned int i=0;i<24;i++){
            cell_lines[i][0] -= Xcm;
            cell_lines[i][1] -= Ycm;
            cell_lines[i][2] -= Zcm;
        }

        Xcm += oldXcm;
        Ycm += oldYcm;
        Zcm += oldZcm;

//        for(unsigned int i=0;i<animation.size();i++){
//            animation[i].com[0] = Xcm;
//            animation[i].com[1] = Ycm;
//            animation[i].com[2] = Zcm;

//            // translate coordinates to new COM
//            for(auto &at:atoms){
//                for(unsigned int j=0;j<3;j++){
//                     at.peli[i][j] -= animation[i].com[j];
//                }
//                if(i==0){
//                    for(unsigned int j=0;j<3;j++){
//                         at.coor[j] -= animation[i].com[j];
//                    }
//                }
//            }
//            for(unsigned int j=0;j<3;j++){
//                animation[i].cell[j][0] -= animation[i].com[0];
//                animation[i].cell[j][1] -= animation[i].com[1];
//                animation[i].cell[j][2] -= animation[i].com[2];
//            }
//            //Correct new COM with old ones to keep track of original coordinates
//            animation[i].com[0] -= oldXcm;
//            animation[i].com[1] -= oldYcm;
//            animation[i].com[2] -= oldZcm;
//        }
    }


    //stablish mcoors as the initial and original coors
    for(auto at:atoms){
        mcoors.push_back(at.coor);
    }
    return;
}

void Struct::load_gaussian_input(QString inputfile){
    QFile file1(inputfile);
    if(!file1.open(QIODevice::ReadOnly)){
        QMessageBox::information(0, "error", file1.errorString());
    }
    QTextStream in(&file1);
    array<float, 3> tempcoor;
    QStringList lsplitted;
    QStringList filelines;
    int coors_init_line, cont = 0;
    Atom atom;
    bool fixed = false;

    while(!in.atEnd()){
        filelines.append(in.readLine());
    }
    for(int i=0;i<filelines.size();i++){
        gaussian.heading.append(filelines[i]);
        if(filelines[i].startsWith("#")){
            gaussian.heading.append(filelines[i+1]);
            gaussian.heading.append(filelines[i+2]);
            gaussian.heading.append(filelines[i+3]);
            gaussian.heading.append(filelines[i+4]);
            coors_init_line = i+5;
            break;
        }
    }

    for(int i=coors_init_line;i<filelines.size();i++){
        lsplitted = filelines[i].split(" ", QString::SkipEmptyParts);
        if(lsplitted.size()==0){
            break;
        }
        MM += masas[lsplitted[0]];
        if (lsplitted.count() == 5){
            tempcoor[0] = lsplitted[2].toFloat();
            tempcoor[1] = lsplitted[3].toFloat();
            tempcoor[2] = lsplitted[4].toFloat();
            if(lsplitted[1].compare("-1") == 0){
                fixed = true;
            }
            else if(lsplitted[1].compare("0") == 0){
                fixed = false;
            }
        }else{
            tempcoor[0] = lsplitted[1].toFloat();
            tempcoor[1] = lsplitted[2].toFloat();
            tempcoor[2] = lsplitted[3].toFloat();
            }
        atom = Atom(tempcoor, lsplitted[0], cont);
        if(fixed){
            atom.fixed = {true, true, true};
        }
        atoms.push_back(atom);
        cont++;
    }
    file1.close();
    showcell = false;
}


void Struct::load_gaussian_output(QString inputfile){
    QFile file1(inputfile);
    if(!file1.open(QIODevice::ReadOnly)){
        QMessageBox::information(0, "error", file1.errorString());
    }
    QTextStream in(&file1);
    array<float, 3> tempcoor, freqdisp, tempdispcoor;
    array<float, 3> tempCenterOfMass = {0,0,0};
    QStringList lsplitted;
    QString line;
    QString symbol;
    bool readcoors = false;
    int cont = 0;
    int pelicount= 0;
    int freqcont = 0;   // frequency counter in groups of three
    int atomic_number; //atomic number
    int nat; //number of atoms
    Atom atom;
    Animation temp_animation;
    bool fullatoms = false;
    Freq freq;

    //-------------------- reset attributes -----------------------------------
    pelisize = 0;
    //-------------------- reset attributes -----------------------------------

    while(!in.atEnd()){
//        filelines.append(in.readLine());
        line = in.readLine();
        if(line.contains("Input orientation:") || line.contains("Standard orientation")){
            readcoors = true;
            for(int i=0;i<4;i++){
                in.readLine();
            }
            continue;
        }

        if(readcoors){
            if(line.contains("-------")){
                //I have to do it at the beginning of this loop because
                // I don't have a way to tell when I got to the end of the
                // atomic coordinates line unless I find this line
                readcoors = false;
                nat = cont;
                cont = 0;
                pelicount ++;
                fullatoms = true;
                // ----------- store center of mass for last peli position--------
                for(int i=0;i<3;i++){
                    tempCenterOfMass[i] /= nat;
                }
                temp_animation.com = tempCenterOfMass;
                // -----------store center of mass for last peli position---------

                // the step in an geo opt will be the equal to pelicount
                temp_animation.step = pelicount;

                //add animation to animation vector and instantiate temp_animation
                // I have to read the energy before appending animation
                animation.push_back(temp_animation);
                temp_animation = Animation();
                continue;
           }

            // start reading coordinates if it not a ------  line
            lsplitted = line.split(" ", QString::SkipEmptyParts);
            tempcoor[0] = lsplitted[3].toFloat();
            tempcoor[1] = lsplitted[4].toFloat();
            tempcoor[2] = lsplitted[5].toFloat();
            if(!fullatoms){
                atomic_number = lsplitted[1].toInt();
                symbol = QString::fromStdString(element_symbols[atomic_number-1]);
    //            MM += masas[lsplitted[0]];
                atom = Atom(tempcoor, symbol, cont);
                atom.fixed = {false, false, false};
                atom.peli.push_back(tempcoor);
                atom.pelidisp.push_back({0.0,0.0,0.0});
                atoms.push_back(atom);
            }
            else{
                for(int j=0;j<3;j++){
                   tempdispcoor[j] = tempcoor[j] - atoms[cont].peli[pelicount-1][j];
                }
                atoms[cont].pelidisp.push_back(tempdispcoor);
            }
            //substract to store displacement to play optimization

            // store peli in atom object
            atoms[cont].peli.push_back(tempcoor);
            //update temporal center of mass---------------------------
            for(int i=0;i<3;i++){
                tempCenterOfMass[i] += tempcoor[i];
            }
            //update temporal center of mass---------------------------
            cont ++;
            continue;
        }

        if(line.contains("Frequencies")){
            lsplitted = line.split(" ", QString::SkipEmptyParts);
            int columns = lsplitted.size()-2; //detect how many columns are displaying frequencies
            for(int i=0;i<columns;i++){
                freq.vib = lsplitted[i+2].toFloat();
                freqs.push_back(freq);
            }
            // paso de leer lo que no me interesa
            for(int i=0;i<4;i++){
                in.readLine();
            }
            // paso de leer lo que no me interesa
            for(unsigned int i=0;i<atoms.size();i++){     //iterate over each displacement corresonding to each atom, disp i belong to atom i
                line = in.readLine();
                lsplitted = line.split(" ", QString::SkipEmptyParts);
                for(int j=0;j<columns;j++){                // iterate over the three column of freqs
                    for(int k=0;k<3;k++){                 // iterate over the x y z of collumn j of freqs
                        freqdisp[k] = lsplitted[j*3+k+2].toFloat();  //assign x y z of displacement of freq j
                    }
                    atoms[i].freqdisp.push_back(freqdisp);     // is in the loop of j column, will add the three displacements j to atom i
//                    freqs[freqcont*3+j].disps.push_back(freqdisp);
                }
            }
            freqcont++;   // esto esta de mas
        }
    }
    file1.close();
    pelisize = pelicount;
    showcell = false;
}

void Struct::loadPDB(QString inputfile){
//        array<float,3> tempcoor;
//        Atom atom;
//        QString label;
//
//        OpenBabel::OBConversion obconversion;
//        if (!obconversion.SetInFormat("PDB")){
//            cout << "PDB format not available"<<endl;
//            return;
//        };
//
//        OpenBabel::OBMol mol;
//        OpenBabel::OBAtom a;
//        bool notatend = obconversion.ReadFile(&mol, inputfile.toStdString());
//
////        cout <<mol.GetMolWt() <<endl;
////        cout <<notatend<<endl;
//
//        FOR_ATOMS_OF_MOL(a, mol){
//            tempcoor[0] = a->GetX();
//            tempcoor[1] = a->GetY();
//            tempcoor[2] = a->GetZ();
//
//            label = QString::fromStdString(element_symbols[a->GetAtomicNum()-1]);
//            atom = Atom(tempcoor, label, atoms.size());
//            atom.peli.push_back(tempcoor);
//            atom.pelidisp.push_back({0.0,0.0,0.0});
//            atoms.push_back(atom);
//        }
//
//        // setup animation ------------------------------
//        Animation anim;
//        //sum up coors to COM
//        //I have to do it here because if the coors
//        // are direct I would calculate the wrong COM
//        for(auto &at:atoms){
//            for(unsigned int i=0;i<3;i++){
//                anim.com[i] += at.coor[i];
//            }
//        }
//        //Normalizo COM
//        for(unsigned int i=0;i<3;i++){
//            anim.com[i] /= atoms.size();
//        }
//        animation.push_back(anim);
//        // setup animation ------------------------------
//        cout<<"done reading pdb\n";
//
        return;
}

void Struct::load_cif(QString inputfile){

    QFile file1(inputfile);
    if(!file1.open(QIODevice::ReadOnly)) {
        QMessageBox::information(0, "error", file1.errorString());
    }
    QTextStream in(&file1);

    QString line;
    QStringList lsplitted;
    array<float,6> cell_param;
    bool reading_loop = false;
    bool reading_loop_headers = false;
    QStringList loop_headers;
    int loop_header_x = -1, loop_header_y = -1, loop_header_z = -1, symetric_pos = -1;
    int atom_sym_mult = -1, atom_site_type_symbol = -1, atom_site_label = -1;
    int tableNumber = 0;
    vector<array<float,3>> coors;
    vector<int> multiplicities;
    QStringList sym_equivalent_sites;
    QStringList labels, atom_site_labels;
    QStringList atom_site_type_symbols;
    Atom atom;
    array<float, 3> tempcoor;

    while(!in.atEnd()){
        line = in.readLine();
        line = line.trimmed();
        if(line.startsWith("#")){
            continue;
        }
        if(line.isEmpty()){
            reading_loop = false;
            continue;
        }
        if(line.contains("'")){
            lsplitted = special_split(line);
        }else{
            lsplitted = line.split(" ", QString::SkipEmptyParts);
        }

        if(lsplitted.size()==2 && lsplitted[1].endsWith(")") && lsplitted[1].contains("(")){
//            int bracket_index = lsplitted[1].indexOf("(");
            lsplitted[1] = lsplitted[1].split("(")[0];
        }
// detect space group number --------------------------------------------------------
        if(line.contains("_symmetry_Int_Tables_number")){
            tableNumber = lsplitted[1].toInt();
            sym_equivalent_sites = spaceGroups[tableNumber-1].symOperations;
            continue;
        }
        if(line.contains("_space_group.IT_number")){
            tableNumber = lsplitted[1].toInt();
            sym_equivalent_sites = spaceGroups[tableNumber-1].symOperations;
            continue;
        }
        if(line.contains("_space_group_IT_number")){
            tableNumber = lsplitted[1].toInt();
            sym_equivalent_sites = spaceGroups[tableNumber-1].symOperations;
            continue;
        }
// detect space group number --------------------------------------------------------
        if(line.contains("_cell_angle_alpha")){
            cell_param[3] = lsplitted[1].toFloat();
            continue;
        }
        if(line.contains("_cell_angle_beta")){
            cell_param[4] = lsplitted[1].toFloat();
            continue;
        }
        if(line.contains("_cell_angle_gamma")){
            cell_param[5] = lsplitted[1].toFloat();
            continue;
        }
        if(line.contains("_cell_length_a")){
            cell_param[0] = lsplitted[1].toFloat();
            continue;
        }
        if(line.contains("_cell_length_b")){
            cell_param[1] = lsplitted[1].toFloat();
            continue;
        }
        if(line.contains("_cell_length_c")){
            cell_param[2] = lsplitted[1].toFloat();
            continue;
        }
        //check if I am reading some other header outside the loop
        if(reading_loop && !reading_loop_headers && line.startsWith("_")){
            reading_loop = false;
            //I shoudl read something here
            continue;
        }
        if(line.startsWith("loop_")){
            cout<<"reading loop\n";
            reading_loop = true;
            reading_loop_headers = true;
            loop_headers.clear();
            loop_header_x = -1;
            loop_header_y = -1;
            loop_header_z = -1;
            symetric_pos = -1;
            atom_sym_mult = -1;
            atom_site_type_symbol = -1;
            atom_site_label = -1;
            continue;
        }
        if(reading_loop){
            QStringList templist = line.split(" ", QString::SkipEmptyParts);
            if(templist.size()==0){
                continue;
            }
            if(templist[0].startsWith("_")){
                reading_loop_headers = true;
                loop_headers.append(lsplitted[0]);
            }
            else{
                if(reading_loop_headers){
                    reading_loop_headers = false;
                    //find out the indices of interest in headers list ---------------------
                    for(unsigned int i=0;i<loop_headers.size();i++){
                        if(loop_headers[i].compare("_symmetry_equiv_pos_as_xyz")==0 && tableNumber == 0){
                            symetric_pos = i;
                        }
                        else if(loop_headers[i].compare("_space_group_symop_operation_xyz")==0 && tableNumber == 0){
                            // this is just another keyword in the cif file for the same thing
                            symetric_pos = i;
                        }
                        else if(loop_headers[i].contains("_atom_site_fract_x")){
                            loop_header_x = i;
                        }
                        else if(loop_headers[i].contains("_atom_site_fract_y")){
                            loop_header_y = i;
                        }
                        else if(loop_headers[i].contains("_atom_site_fract_z")){
                            loop_header_z = i;
                        }
                        else if(loop_headers[i].contains("_atom_site_symmetry_multiplicity")){
                            atom_sym_mult = i;
                        }
                        else if(loop_headers[i].contains("_atom_site_type_symbol")){
                            atom_site_type_symbol = i;
                        }
                        else if(loop_headers[i].contains("_atom_site_label")){
                            atom_site_label = i;
                        }
                    }
                    //find out the indices of interest ------------------------------------

                    if(loop_header_x >-1){
                        tempcoor[0] = lsplitted[loop_header_x].toFloat();
                        tempcoor[1] = lsplitted[loop_header_y].toFloat();
                        tempcoor[2] = lsplitted[loop_header_z].toFloat();
                        coors.push_back(tempcoor);
                    }
                    if(atom_site_type_symbol > -1){
                        atom_site_type_symbols.append(lsplitted[atom_site_type_symbol]);
                    }
                    if(atom_sym_mult >-1){
                        multiplicities.push_back(lsplitted[atom_sym_mult].toInt());
                    }
                    if(symetric_pos > -1){
                        sym_equivalent_sites.append(lsplitted[symetric_pos]);
                    }
                    if(atom_site_label > -1){
                        atom_site_labels.append(lsplitted[atom_site_label]);
                    }
                    //find out the indices of interest ------------------------------------
                }
                else{
                    //indices must be already set, just use it
                    if(loop_header_x >-1){
                        tempcoor[0] = lsplitted[loop_header_x].toFloat();
                        tempcoor[1] = lsplitted[loop_header_y].toFloat();
                        tempcoor[2] = lsplitted[loop_header_z].toFloat();
                        coors.push_back(tempcoor);
                    }
                    if(atom_site_type_symbol > -1){
                        atom_site_type_symbols.append(lsplitted[atom_site_type_symbol]);
                    }
                    if(atom_sym_mult >-1){
                        multiplicities.push_back(lsplitted[atom_sym_mult].toInt());
                    }
                    if(symetric_pos > -1){
                        sym_equivalent_sites.append(lsplitted[symetric_pos]);
                    }
                    if(atom_site_label > -1){
                        atom_site_labels.append(lsplitted[atom_site_label]);
                    }
                    //find out the indices of interest ------------------------------------
                }
            }
        }
    }

    //parge labels
    if(atom_site_type_symbols.size()>0){
        labels = parse_symbols(atom_site_type_symbols);
//        printf("%i %i %i type simbols\n");
    }
    else if(atom_site_labels.size()>0){
        labels = parse_symbols(atom_site_labels);
//        printf("%i %i %i site labels\n");
    }
    else{
        cout<<"No labels could be read\n";
    }

    int index_cont = 0;
    cout<<sym_equivalent_sites.size()<<" table number:"<<tableNumber <<endl;
    for(unsigned int i=0;i<coors.size();i++){
        for(auto &s:sym_equivalent_sites){
            tempcoor = calculate_cif_symmetry_positions(s, coors[i]);
            atom = Atom(tempcoor, labels[i], index_cont);
            if(atom_site_labels.size() == coors.size()){
                atom.atom_site_label = atom_site_labels[i];
            }
            atom.peli.push_back(tempcoor);
            atom.pelidisp.push_back({0.0,0.0,0.0});
            atoms.push_back(atom);
            index_cont ++;
        }
    }

    // setup unit cell --------------    
    set_unit_cell(cell_param[0],cell_param[1],cell_param[2],
            cell_param[3],cell_param[4],cell_param[5]);
    // setup unit cell --------------

    //bring all atoms that are outside inside cell
    bring_inside_cell(false);
    remove_duplicates(false);
    frac_cart(atoms);   //este linea siempre debe ir despues que la celda este definida
    // setup animation ------------------------------
    Animation anim;
    //sum up coors to COM
    //I have to do it here because if the coors
    // are direct I would calculate the wrong COM
    for(auto &at:atoms){
        for(unsigned int i=0;i<3;i++){
            anim.com[i] += at.coor[i];
        }
    }
    //Normalizo COM
    for(unsigned int i=0;i<3;i++){
        anim.com[i] /= atoms.size();
    }
    animation.push_back(anim);
    // setup animation ------------------------------

    showcell = true;
    file1.close();
}

array<float, 3> Struct::calculate_cif_symmetry_positions(QString line, array<float,3> &coor){
    array<float, 3> newcoor = {0.0,0.0,0.0} ;
    line = line.replace("'", "");
    line = line.replace(" ", "");
    QStringList eq_pos = line.split(",");  //split string line such like this  +x,-y,1/2-z
    if (eq_pos.size()!=3) return newcoor;

    const QChar plus = QChar('+');
    QChar minus = QChar('-');
    QChar x = QChar('x');
    QChar y = QChar('y');
    QChar z = QChar('z');
    int index;
    float rmat[3][3];
    float tvec[3];

    //initialize matrix with zeros
    for(int i=0;i<3;i++){
        for(int j=0;j<3;j++){
            rmat[i][j] = 0.0;
        }
    }

    for(unsigned int i=0;i<3;i++){
//        cout<<eq_pos[i].toStdString()<<endl;
        //parsing x---------------------------------
        index = eq_pos[i].indexOf(x);
        if(index == -1){
            rmat[i][0] = 0.0;
        }
        else if(index==0){
//            printf("index is 0 so m[i][0] is 1.0 \n");
            rmat[i][0]=1.0;
        }
        else{
            if(eq_pos[i][index-1]==plus){
//                printf("+ before so m[i][0] is 1.0 \n");
                rmat[i][0] = 1.0;
            }
            else if(eq_pos[i][index-1]==minus){
//                printf("- before so m[i][0] is -1.0 \n");
                rmat[i][0]= -1.0;
            }else{
                 rmat[i][1]= (float)0.0;
                std::cout<<"this file might be corrupted, weird character found before x\n";  //devolver una matriz en perticular para mostrar luego un mesaje de error
            }
        }
        //parsing y---------------------------------
        index = eq_pos[i].indexOf(y);
        if(index == -1){
            rmat[i][1] = 0.0;
        }
        else if(index==0){
            rmat[i][1]=1.0;
        }
        else{
            if(eq_pos[i][index-1]==plus){
                rmat[i][1] = 1.0;
            }
            else if(eq_pos[i][index-1]==minus){
                rmat[i][1]= -1.0;
            }else{
                rmat[i][1]= (float)0.0;
                std::cout<<"this file might be corrupted, weird character found before y\n";  //devolver una matriz en perticular para mostrar luego un mesaje de error
            }
        }
        //parsing z---------------------------------
        index = eq_pos[i].indexOf(z);
        if(index == -1){
            rmat[i][2] = 0.0;
        }
        else if(index==0){
            rmat[i][2]=1.0;
        }
        else{
            if(eq_pos[i][index-1]==plus){
                rmat[i][2] = 1.0;
            }
            else if(eq_pos[i][index-1]==minus){
                rmat[i][2]= -1.0;
            }else{
                 rmat[i][1]= (float)0.0;
                std::cout<<"this file might be corrupted, weird character found before z\n";  //devolver una matriz en perticular para mostrar luego un mesaje de error
            }
        }

        // parse translation
        index = eq_pos[i].indexOf(QChar('/'));
        if(index == -1){
//            printf("vector index is -1 so 0 will be\n");
            tvec[i] = 0.0;
        }else if(index == 0){
//            printf("vector index is 0 so 0 will be\n");
            std::cout<<"this file might be corrupted, / character found in 0 position of string\n";  //devolver una matriz en perticular para mostrar luego un mesaje de error
        }else if(index == 1){    //check if / is the second character, then 1/2 would be the starting of the string,  ojo: might get an error
//            printf("vector index is 1 so 0 will be\n");
//            cout<<"trans "<<i<<" :"<<QString(eq_pos[i].at(index-2)).toStdString()<<QString(eq_pos[i].at(index-1)).toStdString()<<QString(eq_pos[i].at(index)).toStdString()<<QString(eq_pos[i].at(index+1)).toStdString()<<endl;
            tvec[i] = (float)eq_pos[i].at(index-1).digitValue()/(float)eq_pos[i].at(index+1).digitValue();

        }else{
//            cout<<"trans "<<i<<" :"<<QString(eq_pos[i].at(index-2)).toStdString()<<QString(eq_pos[i].at(index-1)).toStdString()<<QString(eq_pos[i].at(index)).toStdString()<<QString(eq_pos[i].at(index+1)).toStdString()<<endl;
            if(eq_pos[i].at(index-2)==plus){
                tvec[i] = (float)eq_pos[i].at(index-1).digitValue()/(float)eq_pos[i].at(index+1).digitValue();
            }else if(eq_pos[i].at(index-2)==minus){
                tvec[i] = -(float)eq_pos[i].at(index-1).digitValue()/(float)eq_pos[i].at(index+1).digitValue();
            }else{
                tvec[i] = (float)eq_pos[i].at(index-1).digitValue()/(float)eq_pos[i].at(index+1).digitValue();
            }
        }
    }

        //multiply matrix per vector
        /*function que multiplica una matrix por un vector
        el vector es un vector fila.  El ciclo interno itera sobre las columnas del vector
    */
        float suma[3] = {0.0,0.0,0.0};
        for(unsigned int row=0;row<3;row++){
           for(unsigned int col=0;col<3;col++){
              suma[row] += coor[col]*rmat[row][col];
           }
        }
        for(unsigned int i=0;i<3;i++){
            newcoor[i] = suma[i]+tvec[i];
        }
    return newcoor;
}


void Struct::loadVaspXml(QString inputfile){
    QFile file(inputfile);
    QXmlStreamAttributes attr;
    QStringList labels;
    array<array<float,3>,3> tempCell;
    array<float, 3> tempCoor, tempDispCoor, tempCOM;
    Animation anim;
    unsigned int nat = 0;
    bool firstPeliStep = true;
    unsigned int atomIndex = 0, peliIndex=0;

    Atom at;

    if(!file.open(QIODevice::ReadOnly)) {
        QMessageBox::information(0, "error", file.errorString());
    }
    QXmlStreamReader inxml(&file);
    cout<<"Document info: "<<inxml.documentVersion().toString().toStdString()<<endl;
    while(!inxml.atEnd()){
        if(inxml.readNextStartElement()){
            //reading labels
            if(inxml.name()=="array" && inxml.attributes().value("name")=="atoms"){
                bool endLabels = false;
                while(!endLabels){
                    inxml.readNext();
                    if(inxml.isStartElement() && inxml.name()=="rc"){
                        inxml.readNextStartElement();
                        labels.append(inxml.readElementText().replace(" ",""));
                    }
                    if(inxml.isEndElement() && inxml.name()=="array"){
                        endLabels = true;
                    }
                }
                //labels already read
                nat = labels.size();
            }
            //reading struct
            else if(inxml.name()=="structure"){
                bool endStruct = false;
                while(!endStruct){
                    inxml.readNext();
                    //read cell, can read volume also
                    if(inxml.isStartElement() && inxml.name()=="varray" && inxml.attributes().value("name")=="basis"){
                        for(unsigned int i=0;i<3;i++){
                            inxml.readNextStartElement();
                            QString vec = inxml.readElementText();
                            QStringList vecxyz = vec.split(" ",QString::SkipEmptyParts);
                            for(unsigned int j=0;j<3;j++){
                                tempCell[i][j] = vecxyz[j].toFloat();
                            }
                        }
                        anim.cell = tempCell;
                    }

                    //read coors
                    if(inxml.isStartElement() && inxml.name()=="varray" && inxml.attributes().value("name")=="positions"){
                        bool endPositions = false;
                        atomIndex = 0;
                        while(!endPositions){
                            inxml.readNext();
                            if(inxml.isStartElement()){
                                QString coor_i = inxml.readElementText();
                                QStringList coor_is = coor_i.split(" ", QString::SkipEmptyParts);
                                for(unsigned int i = 0;i<3;i++){
                                    tempCoor[i] = coor_is[i].toFloat();
                                }
                                if(firstPeliStep){
                                     at = Atom(tempCoor, labels[atomIndex], atomIndex);
                                     at.pelidisp.push_back({0,0,0});
                                     atoms.push_back(at);
                                }else{
                                    for(int j=0;j<3;j++){
                                       tempDispCoor[j] = tempCoor[j] - atoms[atomIndex].peli[peliIndex-1][j];
                                    }
                                    atoms[atomIndex].pelidisp.push_back(tempDispCoor);
                                }
                                //substract to store displacement to play optimization

                                // store peli in atom object
                                atoms[atomIndex].peli.push_back(tempCoor);

                                //update temporal center of mass---------------------------
                                for(int i=0;i<3;i++){
                                    tempCOM[i] += tempCoor[i];
                                }
                                //update temporal center of mass---------------------------
                                atomIndex ++;
                            }
                            if(inxml.isEndElement() && inxml.name()=="varray"){
                                endPositions = true;
                            }
                            //just in case
                            if(atomIndex == nat){
                                endPositions = true;
                            }
                        }
                        //process to add to animation
                    }
                    if(inxml.isEndElement()&&inxml.name()=="structure"){
                        endStruct = true;
                    }
                }

                //structure i is read, process to add to animation
                firstPeliStep = false;
                // ----------- store center of mass for last peli position--------
                for(int i=0;i<3;i++){
                    tempCOM[i] /= nat;
                }
                anim.com = tempCOM;
                // reset tempCOM--------------------------------------------
                for(int i=0;i<3;i++){
                    tempCOM[i] = 0.0;
                }
                // -----------store center of mass for last peli position---------

                //add animation to animation vector and instantiate temp_animation
                animation.push_back(anim);
                anim = Animation();
                peliIndex ++;
            }
        }
    }

    pelisize = animation.size();
    cell = animation[0].cell;
    showcell = true;
    set_cell_lines();
    frac_cart(atoms, true, true);
    return;
}

void Struct::loadCml(QString inputfile){
    QFile file(inputfile);
    if(!file.open(QIODevice::ReadOnly)) {
        QMessageBox::information(0, "error", file.errorString());
    }
    QXmlStreamReader inxml(&file);
    array<float, 3> tempcoor;
    QString label;
    unsigned int counter = 0;
    while(!inxml.atEnd()){
        if(inxml.readNextStartElement()){
            if(inxml.name().toString()=="atom"){
                QXmlStreamAttributes attr = inxml.attributes();
                label = attr.value("elementType").toString();
                tempcoor[0] = attr.value("x3").toFloat();
                tempcoor[1] = attr.value("y3").toFloat();
                tempcoor[2] = attr.value("z3").toFloat();
                Atom at = Atom(tempcoor, label, counter);
                atoms.push_back(at);
                counter ++;
                inxml.readNextStartElement();
            }
         }
    }
    return;
}

void Struct::appendFragmentCml(QString inputfile){
    QFile file(inputfile);
    if(!file.open(QIODevice::ReadOnly)) {
        QMessageBox::information(0, "error", file.errorString());
    }
    QXmlStreamReader inxml(&file);
    array<float, 3> tempcoor;
    QString label;
    unsigned int counter = atoms.size(); // initial value will be the size of atmos
    vector<int> indices;
    selat.clear();
    while(!inxml.atEnd()){
        if(inxml.readNextStartElement()){
            if(inxml.name().toString()=="atom"){
                QXmlStreamAttributes attr = inxml.attributes();
                label = attr.value("elementType").toString();
                tempcoor[0] = attr.value("x3").toFloat();
                tempcoor[1] = attr.value("y3").toFloat();
                tempcoor[2] = attr.value("z3").toFloat();
                Atom at = Atom(tempcoor, label, counter);
                atoms.push_back(at);
                indices.push_back(counter);
                inxml.readNextStartElement();
                counter ++;
            }
        }
    }
    genbonds();
    GROUP group;
    group.name = inputfile;
    for(unsigned int i=0;i<indices.size();i++){
        selat.push_back(&atoms[indices[i]]);
        atoms[indices[i]].sel = true;
        group.indices.push_back(indices[i]);
    }
    groups.push_back(group);
    add_to_before("add", indices);
    emit data_changed();
    return;
}

void Struct::load_xyz(QString inputfile){
    QFile file1(inputfile);
    if(!file1.open(QIODevice::ReadOnly)) {
        QMessageBox::information(0, "error", file1.errorString());
    }
    QTextStream in(&file1);
    array<float, 3> tempcoor, temp_center_of_mass = {0.0,0.0,0.0};
    QStringList lsplitted;
    bool readcoors = false, firstcoors = true;
    QString line;
    Atom atom;
    Animation temp_animation;
    int nat;
    int cont = 0;
    int pelicount = 0;
    // how to read the information in the second line
    // stands for cp2k trajectory output
    int format = 0;

    while(!in.atEnd()){
        if(readcoors){
//                auto start = chrono::high_resolution_clock::now();
            line = in.readLine();
            lsplitted = line.split(" ", QString::SkipEmptyParts);
            if (lsplitted.count() == 4){
                tempcoor[0] = lsplitted[1].toFloat();
                tempcoor[1] = lsplitted[2].toFloat();
                tempcoor[2] = lsplitted[3].toFloat();
                if(firstcoors){
                    atom = Atom(tempcoor, lsplitted[0], cont);
                    atoms.push_back(atom);
                }
                //update temporal center of mass
                for(int i=0;i<3;i++){
                    temp_center_of_mass[i] += tempcoor[i];
                }
                //update atoms peli
                atoms[cont].peli.push_back(tempcoor);
                cont ++;
                if(cont == nat){
                    // ----------- store center of mass for last peli position--------
                    for(int i=0;i<3;i++){
                        temp_center_of_mass[i] /= nat;
                    }
                    temp_animation.com = temp_center_of_mass;
                    animation.push_back(temp_animation);
                    // -----------store center of mass for last peli position---------
                    firstcoors = false;
                    readcoors = false;
                    cont = 0;
                    nat = 0;
                    pelicount ++;
                }
            }
//            auto stop = chrono::high_resolution_clock::now();
//            auto duration = chrono::duration_cast<chrono::microseconds> (stop-start);
//            cout<<duration.count()<<endl;
        }
        else{
            line = in.readLine();
            lsplitted = line.split(" ", QString::SkipEmptyParts);
            if (lsplitted.count() == 1){
                try{
                    //getting the number of atoms that will be read
                    nat = lsplitted[0].toInt();
                    readcoors = true;

                    //try to read the energy, time and step in the second line
                    //check the format
                    line = in.readLine();
                    if(format == 0){
                        if(line.contains("i =") && line.contains("time =") && line.contains("E =")){
                            format = 1;
                            line.replace(",", "");
                            lsplitted = line.split(" ", QString::SkipEmptyParts);
                            temp_animation = Animation();
                            temp_animation.step = lsplitted[2].toInt();
                            temp_animation.time = lsplitted[5].toFloat();
                            temp_animation.energy = lsplitted[8].toFloat();
                        }
                    }
                    else if(format == 1){
                        line.replace(",", "");
                        lsplitted = line.split(" ", QString::SkipEmptyParts);
                        temp_animation = Animation();
                        temp_animation.step = lsplitted[2].toInt();
                        temp_animation.time = lsplitted[5].toFloat();
                        temp_animation.energy = lsplitted[8].toFloat();
                    }
                }
                catch(...){
                    readcoors = false;
                    cont = 0;
                    nat = 0;
                }
            }
        }
    }
       pelisize = pelicount;
       file1.close();
       showcell = false;
}

void Struct::load_poscar(QString inputfile){
    array<float, 3> tempcoor;
    array<bool, 3> fixeddimensions;
    QStringList lines, lsplitted;
    int initcoorpos, cont = 0;
    Atom atom;
    Animation anim;
    float tempcell[3][3];

    QFile file1(inputfile);
    if(!file1.open(QIODevice::ReadOnly)) {
        QMessageBox::information(0, "error", file1.errorString());
    }

    QTextStream in(&file1);

     while(!in.atEnd()){
         //appending string to qstringlist
         lines << in.readLine();
    }
    file1.close();
    vasp.comment = lines[0];
    lsplitted = lines[1].split(" ", QString::SkipEmptyParts);
    vasp.cell_scale = lsplitted[0].toFloat();

    for(unsigned int i=2;i<5;i++){
        lsplitted = lines[i].split(" ", QString::SkipEmptyParts);
        tempcell[i-2][0] = lsplitted[0].toFloat();
        tempcell[i-2][1] = lsplitted[1].toFloat();
        tempcell[i-2][2] = lsplitted[2].toFloat();
    }

    lsplitted = lines[5].split(" ", QString::SkipEmptyParts);
    if(lsplitted[0].toInt() == 0){
        vasp.old = false;
        for(int i=0;i<lsplitted.size();i++){
            vasp.labels << lsplitted.at(i);
        }
    }else{
        vasp.old = true;
        for(int i=0;i<lsplitted.size();i++){
            vasp.atoms.push_back(lsplitted.at(i).toInt());
        }
        lsplitted = vasp.comment.split(" ", QString::SkipEmptyParts);
        vasp.labels.clear();
        for(int i=0;i<lsplitted.size();i++){
            vasp.labels << lsplitted.at(i);
        }
    }

    if(!vasp.old){
        lsplitted = lines[6].split(" ",QString::SkipEmptyParts);
        for(int i=0;i<lsplitted.size();i++){
            vasp.atoms.push_back(lsplitted.at(i).toInt());
        }
         lsplitted = lines[7].split(" ", QString::SkipEmptyParts);
        if(lsplitted[0].startsWith("s") || lsplitted[0].startsWith("S")){
            //printf("este es sel dynamics");
            vasp.seldynamics = true;
            lsplitted = lines[8].split(" ",QString::SkipEmptyParts);
            initcoorpos = 9;
        }else{
            vasp.seldynamics = false;
            initcoorpos = 8;
        }
        if(lsplitted[0].startsWith("d") || lsplitted[0].startsWith("D")){
            vasp.cartesian = false;
        }
        else{
            vasp.cartesian = true;
        }
    }else{
         lsplitted = lines[6].split(" ", QString::SkipEmptyParts);
        if(lsplitted[0].startsWith("s") || lsplitted[0].startsWith("S")){
            vasp.seldynamics = true;
             lsplitted = lines[7].split(" ", QString::SkipEmptyParts);
            initcoorpos = 8;
        }else{
            vasp.seldynamics = false;
            initcoorpos = 7;
        }
        if(lsplitted[0].startsWith("d") || lsplitted[0].startsWith("D")){
            vasp.cartesian = false;
        }
        else{
            vasp.cartesian = true;
        }
    }

    if(vasp.labels.size() != vasp.atoms.size()){
        printf("Atoms and labels on line 5 and 6 of POSCAR are not the same\n");
        exit(EXIT_FAILURE);
    }

    for(unsigned int i=0;i<vasp.atoms.size();i++){
        for(unsigned int j=0;j<vasp.atoms[i];j++){
            MM += masas[vasp.labels[i]];
            lsplitted = lines[initcoorpos].split(" ", QString::SkipEmptyParts);
            initcoorpos++;
            tempcoor[0] = lsplitted[0].toFloat() ;
            tempcoor[1] = lsplitted[1].toFloat() ;
            tempcoor[2] = lsplitted[2].toFloat() ;
            atom = Atom(tempcoor, vasp.labels[i], cont);
            if (lsplitted.size() == 6){
                for(unsigned int k=3;k<6;k++){
                    if(lsplitted[k].compare("F") == 0){
                        fixeddimensions[k-3] = true;
                    }else{
                        fixeddimensions[k-3] = false;
                    }
                }
                atom.fixed = fixeddimensions;
            }
            else{
                atom.fixed = {false, false, false};
            }
            //initialize peli info
            atom.peli.push_back(tempcoor);
            atom.pelidisp.push_back({0.0,0.0,0.0});
            atoms.push_back(atom);
            cont ++;
        }
    }


    //scale the cell vectors
    for(unsigned int i=0;i<3;i++){
        for(unsigned int j=0;j<3;j++){
            tempcell[i][j] *= vasp.cell_scale;
        }
    }
    set_unit_cell(tempcell);

    if(!vasp.cartesian){
        frac_cart(atoms);
    }
    //sum up coors to COM
    //I have to do it here because if the coors
    // are direct I would calculate the wrong COM
    for(auto &at:atoms){
        for(unsigned int i=0;i<3;i++){
            anim.com[i] += at.coor[i];
        }
    }
    //Normalizo COM
    for(unsigned int i=0;i<3;i++){
        anim.com[i] /= atoms.size();
    }
    animation.push_back(anim);

    showcell = true;
    //set_cell_lines();
    vasp.setup = true;
    return;
}

void Struct::loadCP2Kinput(QString inputfile){
    array<float, 3> tempcoor, center_of_mass;
    QString line;
    QStringList lsplitted;
    unsigned int cont = 0;
    Atom atom;
    bool readCoors = false, readCell=false;

    QFile file1(inputfile);
    if(!file1.open(QIODevice::ReadOnly)) {
        QMessageBox::information(0, "error", file1.errorString());
    }

    QTextStream in(&file1);

     while(!in.atEnd()){
         line = in.readLine();
         lsplitted = line.split(" ", QString::SkipEmptyParts);
         if(line.contains("&COORD")){
             readCoors = true;
             continue;
         }

         if(line.contains("&END COORD")){
             readCoors = false;
             continue;
         }

         if(line.contains("&CELL")){
             readCell = true;
             continue;
         }
         if(line.contains("&END CELL")){
             readCell = false;
             continue;
         }


         //--------reading coors ----------------------
         if (readCoors){
             if(lsplitted.size() != 4){
                 readCoors = false;
                 continue;
             }

             for(unsigned int i=0;i<3;++i){
                 tempcoor[i] = lsplitted[i+1].toFloat();
             }

             atom = Atom(tempcoor, lsplitted[0], cont);
             atoms.push_back(atom);

             //update temporal center of mass
             for(int i=0;i<3;i++){
                 center_of_mass[i] += tempcoor[i];
             }

             atoms[cont].peli.push_back(tempcoor);
             cont ++;
         }

         //---------- read cell-----------------------------
         if(readCell){
             if (lsplitted[0].compare("A") == 0){
                 for(unsigned int i=0; i<3; ++i){
                     cell[0][i] = lsplitted[i+1].toFloat();
                 }

                 line = in.readLine();
                 lsplitted = line.split(" ", QString::SkipEmptyParts);
                 for(unsigned int i=0; i<3; ++i){
                     cell[1][i] = lsplitted[i+1].toFloat();
                 }

                 line = in.readLine();
                 lsplitted = line.split(" ", QString::SkipEmptyParts);
                 for(unsigned int i=0; i<3; ++i){
                     cell[2][i] = lsplitted[i+1].toFloat();
                 }
             }


             readCell = false;
         }
         //---------- read cell-----------------------------

     }

     Animation anim;
     //sum up coors to COM
     //I have to do it here because if the coors
     // are direct I would calculate the wrong COM
     for(unsigned int i=0;i<3;++i){
             anim.com[i] += center_of_mass[i]/atoms.size();
     }
     animation.push_back(anim);

     showcell = true;
     set_cell_lines();

    file1.close();
}

void Struct::load_outcar(QString inputfile){
    Atom atom;
    Animation temp_animation;
    QFile file1(inputfile);
    if(!file1.open(QIODevice::ReadOnly)) {
        QMessageBox::information(0, "error", file1.errorString());
    }
    QString line;
    int ibrion;
    QStringList lsplitted;
    bool readcell= false;
    bool readcoors = false;
    bool readfreqs = false;
    bool readdisp = false;
    bool readnmr = false;
    unsigned int readcell_count = 0;
    unsigned int readcoors_count = 0;
    unsigned int pelicount=0;
    unsigned int fdispcount=0;
    unsigned int nmrcount = 0;
    bool firstcoors_asigned = false;
    array<float, 3> tempcoor = {0.0,0.0,0.0}, temp_center_of_mass = {0.0,0.0,0.0}, tempdispcoor;
    array<array<float,3>,3> tempcellpeli;
    unsigned int nat=0, counter=0;
    Freq freq;

    QTextStream in(&file1);
    while(!in.atEnd()){
         line = in.readLine();

         if(line.contains("POTCAR:")){
             lsplitted = line.split(" ", QString::SkipEmptyParts);
             // check if the label has already been included in the vector vasp.labels
             auto it = std::find(vasp.labels.begin(), vasp.labels.end(), lsplitted[2]);
             if(it == vasp.labels.end()){
                 vasp.labels.push_back(lsplitted[2]);
    //             vasp.labels.push_back(lsplitted[1].remove(":").remove("="));
             }
         }
         else if(line.contains("IBRION")){
              lsplitted = line.split(" ", QString::SkipEmptyParts);
              ibrion = lsplitted[2].toInt();
         }
         else if(line.contains("ions per type =")){
             lsplitted = line.split(" ", QString::SkipEmptyParts);
             for(int j=4;j<lsplitted.size();j++){
                 vasp.atoms.push_back(lsplitted[j].toInt());
                 nat += lsplitted[j].toInt();
             }
            //create the vector of atoms objects without coordinates
             for(unsigned int i=0;i<vasp.atoms.size();i++){
                 for(int j=0;j<vasp.atoms[i];j++){
                     atom = Atom({0.0,0.0,0.0}, vasp.labels[i], counter);
                     atoms.push_back(atom);
                     counter ++;
                 }
             }
             //create the vector of atoms objects without coordinates
         }
         else if(line.contains("direct lattice vectors") && readcell_count == 0){
            readcell = true;
         }
         else if(readcell){
             lsplitted = line.split(" ", QString::SkipEmptyParts);
             for(int j=0;j<3;j++){
                 tempcellpeli[readcell_count][j] = lsplitted[j].toFloat();
             }
             readcell_count ++;
             if(readcell_count == 3){
                 readcell = false;
                 readcell_count = 0;
                 temp_animation.cell = tempcellpeli;
             }
         }
         else if(line.contains("POSITION") && readcoors_count == 0){
            readcoors = true;
         }
         else if(readcoors){
             if(line.contains("---------")) continue;
             lsplitted = line.split(" ", QString::SkipEmptyParts);
             for(int j=0;j<3;j++){
                tempcoor[j] = lsplitted[j].toFloat();
             }
             //substract to store displacement to play optimization
             if(!firstcoors_asigned){
                 atoms[readcoors_count].coor = tempcoor;
                 atoms[readcoors_count].pelidisp.push_back({0.0,0.0,0.0});
             }
             else{
                 for(int j=0;j<3;j++){
                    tempdispcoor[j] = tempcoor[j] - atoms[readcoors_count].peli[pelicount-1][j];
                 }     
                 atoms[readcoors_count].pelidisp.push_back(tempdispcoor);
             }
             //substract to store displacement to play optimization

             // store peli in atom object
             atoms[readcoors_count].peli.push_back(tempcoor);

             //update temporal center of mass---------------------------
             for(int i=0;i<3;i++){
                 temp_center_of_mass[i] += tempcoor[i];
             }
             //update temporal center of mass---------------------------

             readcoors_count ++;
             if(readcoors_count == nat){
                 readcoors = false;
                 readcoors_count = 0;
                 pelicount ++;
                 firstcoors_asigned = true;

                 // ----------- store center of mass for last peli position--------
                 for(int i=0;i<3;i++){
                     temp_center_of_mass[i] /= nat;
                 }
                 temp_animation.com = temp_center_of_mass;
                 // -----------store center of mass for last peli position---------

                 //add animation to animation vector and instantiate temp_animation
                 animation.push_back(temp_animation);
                 temp_animation = Animation();
            }
         }
         else if(line.contains("Eigenvectors and eigenvalues of the dynamical matrix")){
             readfreqs = true;
         }
         else if(readfreqs){
            if(line.contains("THz") && line.contains("cm-1")){
                lsplitted = line.split(" ", QString::SkipEmptyParts);
                if(line.contains("f  =")){
                    freq.vib = lsplitted[7].toFloat();
                }
                else if(line.contains("f/i=")){
                    freq.vib = -lsplitted[6].toFloat();
                }
                else{
                    continue;
                }
                readdisp = true;
            }
            else if(readdisp){
                if(line.contains("dx          dy          dz")){
                    continue;
                }
                lsplitted = line.split(" ", QString::SkipEmptyParts);
                for(unsigned i=0;i<3;i++){
                    tempcoor[i] = lsplitted[i+3].toFloat();
                }
                atoms[fdispcount].freqdisp.push_back(tempcoor);
                fdispcount ++;
                if (fdispcount==nat){
                    fdispcount = 0;
                    freqs.push_back(freq);
                    freq.disps.clear();
                    readdisp = false;
                }
            }
//            if(freqs.size() == 3*nat){  esta mal lo tengo que arreglar, leeral freqs eternamente
//                readfreqs = false;
//            }
         }
         else if(line.contains("  (absolute, valence and core)")){
             readnmr = true;
         }
         else if(readnmr){
             lsplitted = line.split(" ", QString::SkipEmptyParts);
             atoms[nmrcount].chemical_shift = lsplitted[4].toFloat();
             nmrcount ++;
             if(nmrcount==nat){
                 readnmr = false;
             }
         }
         else if(line.contains("energy  without entropy=")){
            lsplitted = line.split(" ", QString::SkipEmptyParts);
            float energy = lsplitted[6].toFloat();
            animation[animation.size()-1].energy = energy;
         }
    }
    pelisize = pelicount;
    cell = animation[0].cell;
    showcell = true;
    set_cell_lines();
}

void Struct::load_pwo(QString inputfile){
//    ifstream file1; // Open for reading
//    array<float, 3> tempcoor;
//    vector <string> lsplitted;
//    vector <float> temp;
//    string linea;
//    char delim = ' ';
//    float num;
//    bool readatomicpos = false, readcell = false;
//    int nat = 0, natcounter = 0, cellcounter = 0;

//	file1.open(inputfile, ios::in);
//
//    while(getline(file1, linea)) {
//        lsplitted = split(linea, delim);
//        if(linea.find("number of atoms/cell") != string::npos){
//            nat = atoi(lsplitted[4].c_str());
//            for(unsigned int i=0;i<nat;i++){
//                labels.push_back("");
//                mis.push_back(0.0);
//                coors.push_back({0,0,0});
//                coor_spheres.push_back(0.0);
//                coor_radii.push_back(0.0);
//                coor_cylinders.push_back(0.0);
//                coor_colors.push_back({0,0,0,0});
//            }
//            cout << "nat  " << nat << endl;
//            continue;
//        }
//        if (readatomicpos){
//            labels[natcounter] = lsplitted[0];
//            mis[natcounter] = masas[lsplitted[0]];
//            MM += masas[lsplitted[0]];
//            tempcoor[0] = atof(lsplitted[1].c_str()) ;
//            tempcoor[1] = atof(lsplitted[2].c_str()) ;
//            tempcoor[2] = atof(lsplitted[3].c_str()) ;
//            //this->coors.push_back(tempcoor);
//            coors[natcounter] = tempcoor;
//            coor_spheres[natcounter] = spheres[lsplitted[0]];
//            coor_radii[natcounter] = radios[lsplitted[0]];
//            coor_cylinders[natcounter] = cylinders[lsplitted[0]];
//            coor_colors[natcounter] = colors[lsplitted[0]];
//            natcounter ++;
//            if(natcounter == nat){
//                readatomicpos = false;
//            }
//            continue;
//        }
//        if(readcell){
//            cell[cellcounter][0] = atof(lsplitted[0].c_str());
//            cell[cellcounter][1] = atof(lsplitted[1].c_str());
//            cell[cellcounter][2] = atof(lsplitted[2].c_str());
//            cellcounter ++;
//            if(cellcounter == 3){
//                readcell = false;
//            }
//            continue;
//        }
//    }
//
//        file1.close();
//        showcell = true;
//        //rot_ang += coors.size()*3.57e-5;
//        set_cell_lines();
    return;
}


void Struct::load_pwi(QString inputfile){
//    ifstream file1; // Open for reading
//    array<float, 3> tempcoor;
//    vector <string> lsplitted;
//    vector <string> lines;
//    vector <float> temp;
//    string linea;
//    char delim = ' ';
//    float num;
//    array<bool, 3> fixeddimensions;
//    bool readcell = false, readcontrol = false, readsystem = false;
//    bool readelectrons = false, readions = false;
//    int cont;

//    file1.open(inputfile, ios::in);

//    while(getline(file1, linea)) {
//        lines.push_back(linea);
//    }
//    file1.close();

//    for(unsigned int i=0;i<lines.size();i++){
//        linea = lines[i];
//        lsplitted = split(lines[i], delim);

//        if(lsplitted.size() == 1 && lsplitted[0].compare("/") == 0){
//                readcell = false;
//                readcontrol = false;
//                readsystem = false;
//                readelectrons = false;
//                readions = false;
//                continue;
//            }
//        if(readcontrol){
//            pwscf.control[lsplitted[0]] = lsplitted[2];
//        }
//        else if(readsystem){
//            pwscf.system[lsplitted[0]] = lsplitted[2];
//        }
//        else if(readelectrons){
//            pwscf.electrons[lsplitted[0]] = lsplitted[2];
//        }
//        else if(readions){
//            pwscf.ions[lsplitted[0]] = lsplitted[2];
//        }
//        else if(readcell){
//            pwscf.cell[lsplitted[0]] = lsplitted[2];
//        }

//        if(linea.find("&CONTROL") != string::npos || linea.find("&control") != string::npos){
//            readcontrol = true;
//        }
//        else if(linea.find("&SYSTEM") != string::npos || linea.find("&system") != string::npos){
//            readsystem = true;
//        }
//        else if(linea.find("&ELECTRONS") != string::npos || linea.find("&electrons") != string::npos){
//            readelectrons = true;
//        }
//        else if(linea.find("&IONS") != string::npos || linea.find("&ions") != string::npos){
//            readions = true;
//        }
//        else if(linea.find("&CELL") != string::npos || linea.find("&cell") != string::npos){
//            readcell = true;
//        }
//        else if(linea.find("ATOMIC_SPECIES") != string::npos){
//            int ntyp = atoi(pwscf.system["ntyp"].c_str());
//            for(unsigned int j=i+1;j<i+ntyp+1;j++){
//                 lsplitted = split(lines[j], delim);
//                 pwscf.atomic_species.push_back({lsplitted[0], lsplitted[1], lsplitted[2]});
//                 cont = j;
//            }
//            i = cont;
//        }
//        else if(linea.find("ATOMIC_POSITIONS") != string::npos){
//            int nat = atoi(pwscf.system["nat"].c_str());
////            cout << "nattttt:" << nat << endl;
//            for(unsigned int j=i+1;j<nat+i+1;j++){
//                lsplitted = split(lines[j], delim);
//                labels.push_back(QString::fromStdString(lsplitted[0]));
//                mis.push_back(masas[lsplitted[0]]);
//                MM += masas[lsplitted[0]];
//                tempcoor[0] = atof(lsplitted[1].c_str()) ;
//                tempcoor[1] = atof(lsplitted[2].c_str()) ;
//                tempcoor[2] = atof(lsplitted[3].c_str()) ;
//                //this->coors.push_back(tempcoor);
//                coors.push_back(tempcoor);
//                coor_spheres.push_back(spheres[lsplitted[0]]);
//                coor_radii.push_back(radios[lsplitted[0]]);
//                coor_cylinders.push_back(cylinders[lsplitted[0]]);
//                coor_colors.push_back(colors[lsplitted[0]]);
//                if (lsplitted.size() == 7){
//                    for(unsigned int k=4;k<7;k++){
//                        if(lsplitted[k].compare("0") == 0){
//                            fixeddimensions[k-4] = false;
//                        }else{
//                            fixeddimensions[k-4] = true;
//                        }
//                    }
//                    atoms_are_movable.push_back(fixeddimensions);
//                }
//                cont = j;
//            }
//            i = cont;
//        }
//        else if(linea.find("CELL_PARAMETERS") != string::npos){
//            for(unsigned int j=0;j<3;j++){
//                lsplitted = split(lines[j+i+1], delim);
//                cell[j][0] = atof(lsplitted[0].c_str());
//                cell[j][1] = atof(lsplitted[1].c_str());
//                cell[j][2] = atof(lsplitted[2].c_str());
//                cont = j;
//            }
//            i=cont+i+1;
//        }
//        else if(linea.find("K_POINTS") != string::npos){
////            cout <<"sii es kpoints\n";
//            if(lsplitted[1].compare("automatic") == 0){
//                pwscf.kpoints.push_back("automatic");
//                pwscf.kpoints.push_back(lines[i+1]);
//            }
//        }
//    }
//        showcell = true;

//        set_cell_lines();
//        pwscf.setup = true;
    return;
}

void Struct::readfile(QString structfile){
    atoms.clear();
    fatoms.clear();
    reversed_rot_vec.clear();
    peli.clear();

    for(unsigned int c=0; c<3; c++){
        cell[c][0] = 0.0;
        cell[c][1] = 0.0;
        cell[c][2] = 0.0;
    }

    vasp.setup = false;
    vasp.comment = "";
    vasp.labels.clear();
    vasp.atoms.clear();
    vasp.seldynamics = false;
    vasp.cartesian = false;
    vasp.old = false;
    gaussian.heading.clear();

    MM = 0.0;
    Xcm = 0.0; Ycm = 0.0;Zcm = 0.0;

    current_file = structfile;

    if (structfile.contains(".xyz")){
        load_xyz(structfile);
    }
    else if(structfile.contains(".com")){
        load_gaussian_input(structfile);
    }
    else if(structfile.endsWith(".cif")){
        load_cif(structfile);
    }
    else if(structfile.endsWith(".xml") && structfile.contains("vasprun")){
        loadVaspXml(structfile);
    }
    else if(structfile.endsWith(".cml")){
        loadCml(structfile);
    }
    else if(structfile.endsWith(".pdb")){
        loadPDB(structfile);
    }
    else if(structfile.contains("POSCAR")){
         load_poscar(structfile);
    }
    else if(structfile.contains("CONTCAR")){
         load_poscar(structfile);
    }
    else if(structfile.contains("OUTCAR")){
         load_outcar(structfile);
    }
    else{
        string linea;
        ifstream file1;
        file1.open(structfile.toStdString(), ios::in);
        for(unsigned int i=0;i<20;i++){
            getline(file1, linea);
            if(linea.find("Program PWSCF") != string::npos){
                file1.close();
                load_pwo(structfile);
                return;
            }
            if(linea.find("This is part of the Gaussian(R) 09 program") !=string::npos){
                file1.close();
                load_gaussian_output(structfile);
                return;
            }
            if(linea.find("&GLOBAL")){
                loadCP2Kinput(structfile);
                return;
            }
        }
        load_pwi(structfile);
    }
}

void Struct::genbonds(){
    float sradio, d;
    BOND tempbond;
    for(unsigned int i=0;i<atoms.size();i++){
        atoms[i].bonds.clear();
        for(unsigned int j=0;j<i;j++){
            d = dist(atoms[i].coor, atoms[j].coor);
            sradio = atoms[i].radio + atoms[j].radio;
            if(d <= sradio && d != 0.0){
                tempbond.neighbor = j;
                if(atoms[i].cyl <= atoms[j].cyl){
                    tempbond.radio = atoms[i].cyl;
                }else{
                    tempbond.radio = atoms[j].cyl;
                }
                atoms[i].bonds.push_back(tempbond);
            }
        }
    }
}

void Struct::update_radios(float rad, bool ctrl){
    if(ctrl){
        CTRLZ bef;
        bef.action = "change_radius";
        for(auto &sel:selat){
            bef.indices.push_back(sel->index);
            bef.radios.push_back(sel->radio);
            sel->radio = rad;
        }
        genbonds();
        beforedata.push_back(bef);
    }
    else{
        for(auto &sel:selat){
            sel->radio = rad;
        }
        genbonds();
    }
}


void Struct::paste_atoms(vector<Atom> &ats){
    selat.clear();
    vector<unsigned int> indices; //temporal
    for(auto &at:ats){
        indices.push_back(atoms.size());
        at.index = atoms.size();
        at.oind = -1;
        at.rep_xyz = {1,1,1};
        atoms.push_back(at);
    }
    for(auto &i:indices){
        selat.push_back(&atoms[i]);
    }
    genbonds();
}


void Struct::delete_selection(){

    //vector to store and order the indices to be deleted
    vector <int> indices;

    //get the indices of the selected atoms
    for(auto sel:selat){
        indices.push_back(sel->index);
    }

    //sort the indices to properly take them out of atoms vector in order
    sort(indices.begin(), indices.end());

    int dec = 0;

    // keep what going to be deleted to add them afterwards if ctrl z
    add_to_before("del", indices);

    //erase the atoms with index i from atoms vector
    for(auto i:indices){
        atoms.erase(atoms.begin()+i-dec);
        dec += 1;
    }

    //update indices of the atoms not deleted
    for(unsigned int i=0;i<atoms.size();i++){
        atoms[i].index = i;
    }
    selat.clear();
    genbonds();
}



void Struct::delete_by_index(vector<int> &indices){

    //sort the indices to properly take them out of atoms vector in order
    sort(indices.begin(), indices.end());
    int dec = 0;

    // keep what going to be deleted to add them afterwards if ctrl z
//    add_to_before("del", indices);
    //I actually have the indices, no need to add_to_before now, can be done before calling
    // this function

    //erase the atoms with index i from atoms vector
    for(auto i:indices){
        atoms.erase(atoms.begin()+i-dec);
        dec += 1;
    }
    //update indices of the atoms not deleted
    for(unsigned int i=0;i<atoms.size();i++){
        atoms[i].index = i;
    }
}




void Struct::save_gaussian_input(QString filename){
    QFile outfile(filename);

    if(!outfile.open(QIODevice::WriteOnly)) {
        QMessageBox::information(0, "error", outfile.errorString());
    }
    QTextStream out(&outfile);
    for(auto i:gaussian.heading){
        out << i <<" \n";
    }
    for(auto at:fatoms){
        out << at.label << "     ";
        if(gaussian.fixed){
            if(at.fixed[0] == true){
                out << " -1   ";
            }else{
                out << " 0    ";
            }
        }
        for(unsigned int j=0;j<3;j++){
            out << "  " <<QString::number(at.coor[j], 'f', 16) << "     ";
        }
        out << "    \n";
    }
    out<<"  \n";
    outfile.close();
}

void Struct::save_pwi(QString filename){

}

void Struct::save_xyz(QString filename){
    QFile outfile(filename);
    unsigned int nat;

    nat = fatoms.size();
    if(!outfile.open(QIODevice::WriteOnly)) {
        QMessageBox::information(0, "error", outfile.errorString());
    }
    QTextStream out(&outfile);
    out << nat << " \n\n";

    for(auto at:fatoms){
        out << at.label << "     ";
        for(unsigned int j=0;j<3;j++){
            out << "  " <<QString::number(at.coor[j], 'f', 16) << "     ";
        }
        out << "    \n";
    }
    outfile.close();

}


void Struct::save_xyz_trajectory(QString filename){
    QFile outfile(filename);
    unsigned int nat = atoms.size();
    float tempcoor;

    if(!outfile.open(QIODevice::WriteOnly)) {
        QMessageBox::information(0, "error", outfile.errorString());
    }
    QTextStream out(&outfile);

    for(unsigned int i=0;i<pelisize;i++){
        out <<"    " << nat << " \n";
        if (animation[i].step == 0){
            out<<"i = "<<i;
        }else{
            out<<"i = "<<animation[i].step;
        }
        out <<", time = "<<animation[i].time<<", E = "<<animation[i].energy<<endl;
        for(auto &at:atoms){
            out << at.label << "     ";
            for(unsigned int j=0;j<3;j++){
                tempcoor = at.peli[i][j] + animation[i].com[j];
                out << "  " <<QString::number(tempcoor, 'f', 5) << "     ";
            }
            out << "    \n";
        }
    }
    outfile.close();
}


void Struct::save_poscar(QString filename){
    QFile outfile(filename);

    if(!outfile.open(QIODevice::WriteOnly)) {
        QMessageBox::information(0, "error", outfile.errorString());
    }

    QTextStream out(&outfile);

    int lpos, index;
    bool found = false;
    vector <vector<int>> index_group;
    vector<int> tempvector;

    if(!vasp.cartesian){
        frac_cart(fatoms, false);
        round_finalcoors();
    }

    //reviso el numero de atoms y labels
    vasp.labels.clear();
    vasp.atoms.clear();
    for(auto at:fatoms){
        found = false;
        tempvector.clear();
        for(int j=0;j<vasp.labels.size();j++){
            if(at.label.compare(vasp.labels[j]) == 0){
                found = true;
                lpos = j;
                break;
            }
        }
        if(!found){
            vasp.labels << at.label;
            vasp.atoms.push_back(1);
            tempvector.push_back(at.index);
            index_group.push_back(tempvector);
        }else{
            vasp.atoms[lpos]++;
            index_group[lpos].push_back(at.index);
        }
    }

    /*
     * lo quite porque cuando cambio los atoms me deja siempre los atoms viejos y tengo problemas
     * con el POTCAR luego
    if(vasp.old){
        for(int i=0;i<vasp.labels.size();i++){
            out << QString( "   %1  ").arg(vasp.labels[i]);
        }
            out << "\n  " << QString::number(vasp.cell_scale, 'f', 5) <<" \n";
    }else{
        out << QString(" %1 \n").arg(vasp.comment) <<"  " << QString::number(vasp.cell_scale, 'f', 5)<<" \n";
    }
*/

    for(int i=0;i<vasp.labels.size();i++){
        out << QString( "   %1  ").arg(vasp.labels[i]);
    }
        out << "\n  " << QString::number(vasp.cell_scale, 'f', 5) <<" \n";

    for(unsigned int i=0;i<3;i++){
        for(unsigned int j=0;j<3;j++){
//            fprintf(outfile, "  % 20.16f    ", cell[i][j]);
            out << "    " << QString::number(cell[i][j]/vasp.cell_scale, 'f', 16) << "   ";
        }
        out << " \n";
    }
    if(!vasp.old){
        for(int i=0;i<vasp.labels.size();i++){
            out << QString("   %1  ").arg(vasp.labels[i]);
        }
        out << " \n";
    }
    for(unsigned int i=0;i<vasp.atoms.size();i++){
        out << QString("   %1  ").arg(vasp.atoms[i]);
    }
    out << " \n";
    if(vasp.seldynamics){
         out << "Selective dynamics \n";
    }
    if(vasp.cartesian){
         out << "Cartesian \n";
    }else{
         out << "Direct \n";
    }

    for(unsigned int i=0;i<index_group.size();i++){
        for(unsigned int k=0; k<index_group[i].size();k++){
            index = index_group[i][k];
            for(unsigned int j=0;j<3;j++){
                out << "    " << QString::number(fatoms[index].coor[j], 'f', 16);
            }
            if(vasp.seldynamics){
                for(unsigned int j=0;j<3;j++){
                    if(fatoms[index].fixed[j]){
                        out << "   F   ";
                    }else{
                        out << "   T   ";
                    }
                }
            }
            out << " \n";
        }
    }
    outfile.close();
}


void Struct::prueba(){
    for(auto sel:selat){
        for (int j =0;j<4;j++){
            cout<<sel->color[j]<<"  ";
        }
        cout<<endl;
    }
}

void Struct::unReplicate(){
    //AQUI NUNCA PUEDO PONER ADD_TO_BEFORE PORQUE SE USA EN REPLICATE
    // LO QUE AÑADIR A BEFORE FUERA
//    atoms.erase(atoms.begin()+mcoors.size(), atoms.end());
    vector<Atom> newatoms;
    for(auto at:atoms){
        if (at.oind ==-1){
            newatoms.push_back(at);
        }
    }
    atoms = newatoms;
    replication = {1, 1, 1};
    replicated = false;
//    genbonds();
//    glob_rotate(atoms, trotmat);
    return;
}

bool Struct::shift_peli(QString direction){
    if(direction.compare("f")==0){
        if(peli_pos == pelisize-1){
            return false;
        }
        peli_pos ++;
    }
    else{
        if(peli_pos == 0){
            return false;
        }
        peli_pos --;
    }

    for(int i=0;i<atoms.size();i++){
        atoms[i].shift_peli(peli_pos);
    }
    glob_rotate(atoms, trotmat);
    genbonds();
    return true;
}

bool Struct::shift_peli(int newpelipos){
    peli_pos = newpelipos;
    for(int i=0;i<atoms.size();i++){
        atoms[i].shift_peli(peli_pos);
    }
    glob_rotate(atoms, trotmat);
    genbonds();
    return true;
}

bool Struct::shift_freq_desp(){
    //There is not return false???? ohhhh
    if(freq_forward){
        freq_pos ++;
        if(freq_pos == freqSteps){
            freq_forward = false;
        }
    }
    else{
        freq_pos --;
        if(freq_pos == 0){
            freq_forward = true;
        }
    }
    unsigned int nat = atoms.size();
    for(unsigned int i=0;i<nat;i++){
        atoms[i].coor = atoms[i].peli[0];
        for(unsigned int j=0;j<3;j++){
            if(atoms[i].freqdisp[cfreq][j] != 0.0){
                atoms[i].coor[j] += freq_disp_fracs[freq_pos]*atoms[i].freqdisp[cfreq][j];
            }
        }
    }
    glob_rotate(atoms, trotmat);
//    genbonds();

    return true;
}

void Struct::update_atom_by_label(int index, QString label){
    CTRLZ bef;
    bef.action = "change_element";
    bef.indices.push_back(atoms[index].index);
    bef.labels.push_back(atoms[index].label);
    bef.radios.push_back(atoms[index].radio);
    bef.spheres.push_back(atoms[index].sphere);
    bef.cyls.push_back(atoms[index].cyl);
    bef.colors.push_back(atoms[index].color);

    atoms[index].update_by_label(label);
    genbonds();
    beforedata.push_back(bef);
}

void Struct::load_data(QString file){
    try{
        readfile(file);   //me ecargo de detectar el formato, y definir las coordenadas y la celda
    }
    catch(...){
        cout<<"there was a problem reading file. Probably segmentation fault\n";
    }
    try{
        shift_to_mass_center();  // desplazo tanto las coordenadas como la celda
    }
    catch(...){
        cout<<"there was a problem reading file. Probably segmentation fault\n";
    }

   genbonds();
}

void Struct::update_color(float red, float green, float blue, float sat){
    //seria mas rapido ir añadiendo aqui en este ciclo los colores a un objeto CTRLZ
    // antes de cambiar los colores???? evitaria hacer el ciclo dos veces
    CTRLZ bef;
    bef.action = "change_color";
//    add_to_before("change_color");
    for(auto &sel:selat){
        bef.indices.push_back(sel->index);
        bef.colors.push_back(sel->color);
        sel->color = {red, green, blue, sat};
    }
    beforedata.push_back(bef);
    bef = beforedata[beforedata.size()-1];
}

void Struct::update_spheres(float rad, bool ctrlz){
    if(ctrlz){
        CTRLZ bef;
        bef.action = "change_sphere";
        for(auto &sel:selat){
            bef.indices.push_back(sel->index);
            bef.spheres.push_back(sel->sphere);
            sel->sphere = rad;
        }
        beforedata.push_back(bef);
    }
    else{
        for(auto &sel:selat){
            sel->sphere = rad;
        }
    }

}

void Struct::select_inside_sphere(float rad){
    float d;
    for(auto &at:atoms){
        if(at.sel){
            continue;
        }
        d = dist(at.coor, selat[0]->coor);
        if(d <= rad){
            at.sel = true;
            selat.push_back(&at);
        }
    }
}

void Struct::select_inside_sphere_bonded(float rad){
    float d;
    for(auto &at:atoms){
        if(at.sel){
            continue;
        }
        d = dist(at.coor, selat[0]->coor);
        if(d <= rad){
            for(auto &s:selat){
                if(s->bonded(at)){
                    at.sel = true;
                    selat.push_back(&at);
                    continue;
                }
            }

        }
    }
}

void Struct::invert_selection(){
    selat.clear();
    for(auto &at:atoms){
        if (!at.visible){
            continue;
        }
       if(at.sel){
            at.sel = false;
       }
       else{
            at.sel = true;
            selat.push_back(&at);
       }
    }
}

void Struct::invertSelectionAtomsType(){
    QStringList atomsType;
    for(auto &sel:selat){
        if(!atomsType.contains(sel->label)){
            atomsType.append(sel->label);
        }
    }

    selat.clear();
    for(auto &at:atoms){
        if (!at.visible){
            continue;
        }

        if(!atomsType.contains(at.label)){
            at.sel = false;
            continue;
        }
        if(at.sel){
            at.sel = false;
        }
        else{
            at.sel = true;
            selat.push_back(&at);
        }
    }
}

void Struct::selectByIndex(QString list){
    QStringList indices = list.split(" ", QString::SkipEmptyParts);
    for(auto str:indices){
        int index = str.toInt();

        if(!atoms[index].visible){
            continue;
        }

        if(atoms[index].sel){
            continue;
        }
        if(index >= atoms.size()){
            continue;
        }

        atoms[index].sel = true;
        selat.push_back(&atoms[index]);
    }
}

void Struct::select_by_elements(QStringList elements){
    selat.clear();
    for(auto l:elements){
        for(auto &at:atoms){
            if(l.compare(at.label) == 0){
                at.sel = true;
                selat.push_back(&at);
            }
        }
    }
}

void Struct::select_fixed(){
    selat.clear();
    for(auto &at:atoms){
        if(at.fixed[0] || at.fixed[1] || at.fixed[2]){
            at.sel = true;
            selat.push_back(&at);
        }
    }
}

void Struct::update_cylinders(float rad){
    for(auto &sel:selat){
        sel->cyl = rad;
        for(auto &bd:sel->bonds){
            if(sel->cyl <= atoms[bd.neighbor].cyl){
               bd.radio = sel->cyl;
            }else{
                bd.radio = atoms[bd.neighbor].cyl;
            }
        }
    }
}


void Struct::update_cylinders(Atom &at, float rad){
        at.cyl = rad;
        for(auto &bd:at.bonds){
            if(at.cyl <= atoms[bd.neighbor].cyl){
               bd.radio = at.cyl;
            }else{
                bd.radio = atoms[bd.neighbor].cyl;
            }
        }
    }

Atom Struct::clone_atoms(Atom &at){
    //tengo que tener cuiado porque igual quiero
    // copiar un atomo que no necesariamente sera una replica
    //trasladada
    Atom atcopy(at.coor, at.label, -1);
    atcopy.color = at.color;
    atcopy.sphere = at.sphere;
    atcopy.fixed = at.fixed;
    atcopy.radio = at.radio;
    atcopy.cyl = at.cyl;
    atcopy.peli = at.peli;
    atcopy.chemical_shift = at.chemical_shift;
    atcopy.freqdisp = at.freqdisp;
    atcopy.atom_site_label = at.atom_site_label;
    atcopy.visible = at.visible;
    if(at.oind == -1){
        atcopy.oind = at.index;
    }else{
        atcopy.oind = at.oind;
    }
    return atcopy;
}

void Struct::set_stick_view(float radius){
    for(auto sel:selat){
        sel->sphere = radius;
    }
    update_cylinders(radius);
    genbonds(); //no funciona no se porque se queda con los cylindros como estaban
}

void Struct::set_wire_view(){
    for(auto sel:selat){
        sel->sphere = 0.02;
    }
    update_cylinders(0.02);
    genbonds(); //no funciona no se porque se queda con los cylindros como estaban
}

void Struct::set_default_view(){
    for(auto sel:selat){
        sel->color = colors[sel->label];
        sel->sphere = spheres[sel->label];
        sel->radio = radios[sel->label];
        sel->cyl = cylinders[sel->label];
    }
    genbonds();
}

void Struct::set_default_color(){
    for(auto sel:selat){
        sel->color = colors[sel->label];
    }
}

void Struct::replicate_structure(int x, int y, int z){
    if(x == replication[0] && y == replication[1] && z == replication[2]){
        return;
    }
    //add to before so that it can be unreplicated with ctrl + z
    add_to_before("replication_all");
    if(replication[0] !=1 || replication[1]!=1 || replication[2]!=1){
        unReplicate();
    }
    replicated = true;
    replication = {x, y, z};
    set_inv_mat();
    glob_rotate(atoms, totinvmat);
    replicate(replication);
    glob_rotate(atoms, trotmat);
    genbonds();
}

void Struct::replicate(array<int, 3> xyz , bool selection){
    frac_cart(atoms, false, true);
    unsigned int nat;
    for(unsigned int j=0; j<3; j++){
        // j is the dimension x or y or z in which it is been replicated
        // j = 1 means it is replicated in the x axis and so on
        nat = atoms.size();
        for(unsigned int at=0;at<nat;at++){
            if(selection && !atoms[at].sel){
                continue;
            }
            for(int i=1; i < xyz[j]; i++){
                //i is the step of the of the replication in the j dimension
                Atom atom = clone_atoms(atoms[at]);
                atom.coor[j] += i;
                if(pelisize>0){
                    for(unsigned int k=0;k<pelisize;k++){
                        atom.peli[k][j] += i;
                    }
                }
                atom.index = atoms.size();
                atom.rep_xyz[j] = i+1;
                atoms.push_back(atom);
            }
        }
    }
    frac_cart(atoms, true, true);
}

void Struct::replicate_selection(int x, int y, int z){
    if(x==0) x=1;
    if(y==0) y=1;
    if(z==0) z=1;

//    replication = {x, y, z};
//    replicated = true;
    set_inv_mat();
    glob_rotate(atoms, totinvmat);
    replicate(array<int, 3> {x, y, z}, true);
    glob_rotate(atoms, trotmat);
    genbonds();
}

void Struct::set_inv_mat(){
    float det=0;
    //finding determinant
    for(unsigned int i = 0; i < 3; i++){
        det = det + (trotmat[0][i] * (trotmat[1][(i+1)%3] * trotmat[2][(i+2)%3] - trotmat[1][(i+2)%3] * trotmat[2][(i+1)%3]));
    }

    for(unsigned int i = 0; i < 3; i++){
        for(unsigned int j = 0; j < 3; j++){
            totinvmat[i][j] = ((trotmat[(j+1)%3][(i+1)%3] * trotmat[(j+2)%3][(i+2)%3]) - (trotmat[(j+1)%3][(i+2)%3] * trotmat[(j+2)%3][(i+1)%3]))/ det;
        }
    }
}

void Struct::connectivity(){
    float v1[3],v2[3];
    float n1,n2, angle;
    vector<array<int,2>> pairs;
    vector<array<float,4>> angles;
    int u,v,w;

    for(auto &at:atoms){
        for(unsigned int i=0;i<at.bonds.size();i++){
            pairs.push_back({at.index, at.bonds[i].neighbor});
        }
    }

    for(unsigned int i=0;i<pairs.size();i++){
        for(unsigned int j=0;j<i;j++){
            if (pairs[i][0]==pairs[j][0]){
                v = pairs[i][0];   // angle vertex, the one repeated
                u = pairs[i][1];
                w = pairs[j][1];
            }
            else if(pairs[i][0]==pairs[j][1]){
                v = pairs[i][0];    // angle vertex, the one repeated
                u = pairs[i][1];
                w = pairs[j][0];
            }
            else if(pairs[i][1]==pairs[j][0]){
                v = pairs[i][1];    // angle vertex, the one repeated
                u = pairs[i][0];
                w = pairs[j][1];
            }
            else if(pairs[i][1]==pairs[j][1]){
                v = pairs[i][1];    // angle vertex, the one repeated
                u = pairs[i][0];
                w = pairs[j][0];
            }else{
                continue;
            }

            v1[0] = atoms[v].coor[0] - atoms[u].coor[0];
            v1[1] = atoms[v].coor[1] - atoms[u].coor[1];
            v1[2] = atoms[v].coor[2] - atoms[u].coor[2];

            v2[0] = atoms[v].coor[0] - atoms[w].coor[0];
            v2[1] = atoms[v].coor[1] - atoms[w].coor[1];
            v2[2] = atoms[v].coor[2] - atoms[w].coor[2];

            n1 = sqrt(pow(v1[0],2) + pow(v1[1],2) + pow(v1[2],2));
            n2 = sqrt(pow(v2[0],2) + pow(v2[1],2) + pow(v2[2],2));

            angle = (v1[0]*v2[0] + v1[1]*v2[1] + v1[2]*v2[2])/(n1*n2);
            angle = acos(angle);
            angle = angle*180/PI;

            if(25.0<angle && angle<35.0){
                angle = 30.0;
            }
            else if(55.0<angle && angle<65.0){
                angle = 60.0;
            }
            else if(85.0<angle && angle<95.0){
                angle = 90.0;
            }
            else if(115.0<angle && angle <125.0){
                angle = 120.0;
            }
            else if(170.0<angle && angle <181.0){
                angle = 180.0;
            }
            else{
                continue;
            }

            if(v1[0]*v2[1] - v1[1]*v2[0]<=0){
                angles.push_back({u,v,w, angle});
            }else{
                angles.push_back({u, v, w, -angle});
            }
        }
    }

    for(auto &p:pairs){
        cout <<p[0]+1<<"-"<<p[1]+1<<"   ";
    }
    cout<<endl;
    cout<<endl;

    for(auto &a:angles){
        cout<<a[0]+1<<"-"<<a[1]+1<<"-"<<a[2]+1<<":"<<QString::number(a[3], 'f', 1).toStdString()<<"  ";
    }
    cout <<endl;
    cout <<endl;
    float top=0.8,hcp=0.6,fcc=0.4;
    for(auto &at:atoms){
        if(at.sphere == top) {
            cout << "top  ";
        }
        else if(at.sphere == hcp) {
            cout << "hcp  ";
        }
        else if(at.sphere == fcc) {
            cout << "fcc  ";
        }
        else {
            cout <<"NA  ";
        }
    }
    cout <<endl;
    cout <<endl;

    vector<QColor> qcolors;
    vector <int> lastdent;
    unsigned int pos = 0;
    QColor blank(1.0,1.0,1.0);

    for(auto &at:atoms){
        QColor color = QColor(at.color[0], at.color[1],at.color[2]);
        bool found = false;
        for(unsigned int i=0;i<qcolors.size();i++){
            if(qcolors[i]!=blank && qcolors[i] == color){
                found=true;
                pos = i;
                lastdent[i] += 1;
                break;
            }
        }
        if(found){
            cout <<"    "<<to_string(pos+1)<<" " <<"spec"<<to_string(pos+1)<<"  "<<to_string(lastdent[pos])<<endl;
        }else{
            qcolors.push_back(color);
            lastdent.push_back(1);

            if(color == blank){
                cout <<"    " <<to_string(lastdent.size())<<"  "<< "*" <<"  1 "<<endl;
            }else{
                cout <<"    " <<to_string(lastdent.size())<<"  "<<"spec"<< to_string(lastdent.size()) <<"  1 "<<endl;
            }
        }
    }
    cout <<endl;
}


void Struct::unselect_all(){
    for(auto &sel:selat){
        sel->sel = false;
    }
    selat.clear();

    for(auto &p:planes){
        p.selected = false;
    }
}

void Struct::bring_inside_cell(bool sel){
    //tengo que verificar si hay celda o no
    //hacer un warning cuando alguno es mayor de 2
    float zero = 0.0;
    float one = 1.0;
    if(sel){
        for(auto &sel:selat){
            for(int j=0;j<3;j++){
                while(sel->coor[j]>one){
                    sel->coor[j] -= one;
                 }
                while(sel->coor[j] < zero){
                    sel->coor[j] += one;
                 }
            }
        }
    }
    else{
        for(auto &at:atoms){
            for(int j=0;j<3;j++){
                while(at.coor[j]>one){
                    at.coor[j] -= one;
                 }
                while(at.coor[j] < zero){
                    at.coor[j] += one;
                 }
            }
        }
    }
}

void Struct::set_unit_cell(float lat[3][3]){

    // change unit cell

//        for(unsigned int i=0;i<3;i++){
//            for(unsigned int j=0;j<3;j++){
//                cell[i][j] = valores[3*i+j];
//            }
//        }
    this->va = sqrt(pow(lat[0][0],2) + pow(lat[0][1],2) + pow(lat[0][2],2));
    this->vb = sqrt(pow(lat[1][0],2) + pow(lat[1][1],2) + pow(lat[1][2],2));
    this->vc = sqrt(pow(lat[2][0],2) + pow(lat[2][1],2) + pow(lat[2][2],2));
    this->cos_valpha = (lat[1][0]*lat[2][0] + lat[1][1]*lat[2][1]+lat[1][2]*lat[2][2])/(this->vb*this->vc);
    this->cos_vbeta = (lat[0][0]*lat[2][0] + lat[0][1]*lat[2][1]+lat[0][2]*lat[2][2])/(this->va*this->vc);
    this->cos_vgamma = (lat[0][0]*lat[1][0] + lat[0][1]*lat[1][1]+lat[0][2]*lat[1][2])/(this->va*this->vb);
    this->sin_vgamma = sqrt(1-pow(this->cos_vgamma,2));
    this->sin_vbeta = sqrt(1-pow(this->cos_vbeta,2));
    this->sin_valpha = sqrt(1-pow(this->cos_valpha,2));

    cell[0][0] = this->va;
    cell[0][1] = 0.0;
    cell[0][2] = 0.0;

    cell[1][0] = this->vb*this->cos_vgamma;
    cell[1][1] = this->vb*this->sin_vgamma;
    cell[1][2] = 0.0;

    //z vector components
    cell[2][0] = (this->cos_vbeta*this->vc);
    cell[2][1] = (this->cos_valpha*this->vb*this->vc - cell[1][0]*cell[2][0])/cell[1][1];
    cell[2][2] = sqrt(this->vc*this->vc - cell[2][0]*cell[2][0] - cell[2][1]*cell[2][1]);
    set_cell_lines();
    return;
}

void Struct::set_unit_cell(float a, float b, float c, float alpha, float beta, float gamma){
    this->va = a;
    this->vb = b;
    this->vc = c;
    this->valpha = PI/180*alpha;
    this->vbeta = PI/180*beta;
    this->vgamma = PI/180*gamma;
    this->cos_valpha = cos(this->valpha);
    this->cos_vbeta = cos(this->vbeta);
    this->cos_vgamma = cos(this->vgamma);
    this->sin_vgamma = sin(this->vgamma);
    this->sin_vbeta = sin(this->vbeta);
    this->sin_valpha = sin(this->valpha);

    cell[0][0] = this->va;
    cell[0][1] = 0.0;
    cell[0][2] = 0.0;

    cell[1][0] = this->vb*this->cos_vgamma;
    cell[1][1] = this->vb*this->sin_vgamma;
    cell[1][2] = 0.0;

    //z vector components
    cell[2][0] = (this->cos_vbeta*this->vc);
    cell[2][1] = (this->cos_valpha*this->vb*this->vc - cell[1][0]*cell[2][0])/cell[1][1];
    cell[2][2] = sqrt(this->vc*this->vc - cell[2][0]*cell[2][0] - cell[2][1]*cell[2][1]);
    set_cell_lines();
    return ;
}

void Struct::recenter(){
//    float oldxcm, oldycm, oldzcm;
//    oldxcm = Xcm;
//    oldycm = Ycm;
//    oldzcm = Zcm;
    set_inv_mat();
    glob_rotate(atoms, totinvmat);
    glob_rotate(cell_lines, totinvmat);
    shift_to_mass_center(true);
//    Xcm += oldxcm;
//    Ycm += oldycm;
//    Zcm += oldzcm;
    glob_rotate(atoms, trotmat);
    glob_rotate(cell_lines, trotmat);
}

void Struct::hide_selected(){
    for(auto sel:selat){
        sel->visible = false;
    }
    selat.clear();
}

void Struct::showall(){
    for(auto &at:atoms){
        at.visible = true;
    }
}

void Struct::export_collective_variable(QString filename){
    QFile outfile(filename);
    vector<float> tempvalue;
    vector<vector<float>> allvc;


    if(!outfile.open(QIODevice::WriteOnly)) {
        QMessageBox::information(0, "error", outfile.errorString());
    }
    QTextStream out(&outfile);

    for(auto cv:col_var){
        if(cv.type.compare("bond") == 0){
            unsigned int in1, in2;
            in1 = cv.indices[0];
            in2 = cv.indices[1];

            for(unsigned int i=0; i<pelisize;i++){
                float d = dist(atoms[in1].peli[i], atoms[in2].peli[i]);
                tempvalue.push_back(d);
            }

            allvc.push_back(tempvalue);
            tempvalue.clear();
        }
        else if(cv.type.compare("dihedral") == 0){
            unsigned int in1, in2, in3, in4;
            in1 = cv.indices[0];
            in2 = cv.indices[1];
            in3 = cv.indices[2];
            in4 = cv.indices[3];
            for(unsigned int i=0; i<pelisize;i++){
                float d = dihedral(atoms[in1].peli[i], atoms[in2].peli[i], atoms[in3].peli[i], atoms[in4].peli[i]);
                tempvalue.push_back(d);
            }
            allvc.push_back(tempvalue);
            tempvalue.clear();
        }
        else if(cv.type.compare("angle") == 0){
            unsigned int in1, in2, in3;
            in1 = cv.indices[0];
            in2 = cv.indices[1];
            in3 = cv.indices[2];
            for(unsigned int i=0; i<pelisize;i++){
                float d = angle(atoms[in1].peli[i], atoms[in2].peli[i], atoms[in3].peli[i]);
                tempvalue.push_back(d);
            }
            allvc.push_back(tempvalue);
            tempvalue.clear();
        }
        else if(cv.type.compare("y") == 0){
            unsigned int in1;
            in1 = cv.indices[0];
            for(unsigned int i=0; i<pelisize;i++){
                tempvalue.push_back(atoms[in1].peli[i][1]);
            }
            allvc.push_back(tempvalue);
            tempvalue.clear();
        }
    }
    // write headers
    for(int k=0;k<col_var.size();k++){
//        out<<"Collective Variable: \""<<col_var[k].name<< "\" , Average : "<<QString::number(averages[k]);
        out<<col_var[k].name<< "     ";
    }
    out<<endl;

    for(int i=0;i<pelisize;i++){
        for(int j=0;j<col_var.size();j++){
//            if(j==0){
//                out<<QString::number(i)<<"    ";
//            }
            out<<QString::number(allvc[j][i]) << "     ";
        }
        out<<endl;
    }
    outfile.close();
}

void Struct::average_peli(int interval){
    for(auto &at:atoms){
        at.average_peli(interval);
    }
    pelisize = atoms[0].peli.size();
}

int Struct::newCollectiveVariable(QString name, QString type){
    CV new_var;

    //remove white spaces at begining and the end
    name = name.trimmed();
    //search if name exits
    for(auto cv:col_var){
        if(name == cv.name){
            return 1;
        }
    }

    new_var.name = name;
    new_var.type = type;
    for(auto sel:selat){
        new_var.indices.push_back(sel->index);
    }
    col_var.push_back(new_var);

    return 0;
}


void Struct::del_peli_by_index(vector<int> &indices){
    //sort the indices to properly take them out of atoms vector in order
    sort(indices.rbegin(), indices.rend());

//   vector<int> ::reverse_iterator rit; ya lo no necesito porque ordene el vector de atras para alante

    for(unsigned int at=0;at<atoms.size();at++){
        for(auto i:indices){
           atoms[at].peli.erase(atoms[at].peli.begin() + i);
        }
    }
    //ojo sino cambio pelisize, al hacer la peli no da error, parece que no se borra atom de la memoria y
    //y si puede acceder a el, esto puede ser problematico
    pelisize = atoms[0].peli.size();
    peli_pos = -1;
}

void Struct::select_replicated_copy(){
    for(auto &at:atoms){
        for(auto sel:selat){
            if(at.oind == sel->index){
                selat.push_back(&at);
            }
        }
    }
}


void Struct::select_original_copy(){
    for(auto &at:atoms){
        for(auto sel:selat){
            if(at.index == sel->oind){
                selat.push_back(&at);
            }
        }
    }
}

void Struct::check_covered(){
#pragma omp parallel
  {
//    omp_set_num_threads(4);
    //cout<<omp_get_num_threads()<<endl;
#pragma omp for
    for(size_t i=0;i<atoms.size();i++){
        atoms[i].covered = false;
        for(size_t j=0;j<atoms.size();j++){
            if(i==j){
                continue;
            }
            if(atoms[i].coor[2]>atoms[j].coor[2]){
                continue;
            }

            float mod = dist(atoms[i].coor[0], atoms[i].coor[1], 0.0, atoms[j].coor[0], atoms[j].coor[1], 0.0);
            float diff_sphere = atoms[j].sphere - atoms[i].sphere;
            if(diff_sphere < 0.0){
                continue;
            }

            if(mod <= diff_sphere){
                atoms[i].covered = true;
                break;
            }
        }
    }
  }
}

bool Struct::valid_cell(){
    for(unsigned int i=0;i<3;i++){
        if(cell[i][0]==0.0 && cell[i][1]==0.0 && cell[i][2] == 0.0){
            showcell = false;
            return false;
        }
    }
    return true;
}

void Struct::remove_duplicates(bool cart){
/*removes atoms that are two close to each other
    and have the same typename */
    vector<int> indices;

    for(unsigned int i=0;i<atoms.size();i++){
        for(unsigned j=i+1;j<atoms.size();j++){

            if(atoms[i].label.compare(atoms[j].label)!=0){
                continue;
            }
            if(atoms[i].overlapped || atoms[j].overlapped){
                continue;
            }
            /* I can also calculate the distance and check whether it is less
            that a normal bond distance but I think this is faster. I only one
            coordinate greater than a value it is not overlapped.
            I have to check which are the appropiate values for the thresholds,
            they should be a bit diffrent*/
            if(check_equal_coors(atoms[i].coor, atoms[j].coor, 0.001)){
                atoms[j].overlapped = true;
                indices.push_back(j);
                continue;
            }
            if(!cart && check_equal_replicated(atoms[i].coor, atoms[j].coor, 0.0001)){
                atoms[j].overlapped = true;
                indices.push_back(j);
            }
        }
    }
    delete_by_index(indices);
}


void Struct::selectBySphere(float ss){
    for(auto &at:atoms){
        if(at.sphere == ss){
            at.sel = true;
            selat.push_back(&at);
        }
    }
}

void Struct::selectBySphere(float sphereRange[]){
    for(auto &at:atoms){
        if(sphereRange[0] <= at.sphere && at.sphere <= sphereRange[1]){
            at.sel = true;
            selat.push_back(&at);
        }
    }
}


void Struct::selectByNMR(float nmr){
    for(auto &at:atoms){
        if(at.chemical_shift == nmr){
            at.sel = true;
            selat.push_back(&at);
        }
    }
}

void Struct::changeElements(QString element){
    CTRLZ bef;
    bef.action = "change_element";

    for(auto &sel:selat){
        bef.indices.push_back(sel->index);
        bef.labels.push_back(sel->label);
        bef.radios.push_back(sel->radio);
        bef.spheres.push_back(sel->sphere);
        bef.cyls.push_back(sel->cyl);
        bef.colors.push_back(sel->color);

//        sel->label = element;
//        sel->radio = radios[element];
//        sel->sphere = spheres[element];
//        sel->cyl = cylinders[element];
//        sel->color = colors[element];
        sel->update_by_label(element);
    }
    genbonds();
    beforedata.push_back(bef);
}

int Struct::createGroup(QString name){
    // this function returns an int as code on whether or not
    // the group was created and why
    // 1 means the group was not create because there was already
    // a group with the specified name

    for(auto g:groups){
        if(name == g.name){
            return 1;
        }
    }

    GROUP group;
    group.name = name;
    for(auto &sel:selat){
        group.indices.push_back(sel->index);
        sel->groups.append(name);
    }
    groups.push_back(group);

    return 0;
}

void Struct::alignToBond(QString dStr){
    float d = dStr.toFloat();
    float vec[3] = {selat[1]->coor[0]-selat[0]->coor[0],
                    selat[1]->coor[1]-selat[0]->coor[1],
                    selat[1]->coor[2]-selat[0]->coor[2]};
    VectorNormalisation(vec);

    float displacement;
    for(unsigned int i=0;i<3;i++){
        displacement = vec[i]*d;
        selat[2]->coor[i] = selat[1]->coor[i]+displacement;
    }
    genbonds();
}


float Struct::get_traj_ave_distance(int i, int j){
    float sum = 0.0;
    for(unsigned int k=0; k<pelisize;k++){
        sum += dist(atoms[i].peli[k], atoms[j].peli[k]);
    }
    sum /= pelisize;
    return sum;
}

float Struct::get_traj_ave_angle(int i, int j, int k){
    float sum = 0.0;
    for(unsigned int l=0; l<pelisize;l++){
        sum += angle(atoms[i].peli[l], atoms[j].peli[l], atoms[k].peli[l]);
    }
    sum /= pelisize;
    return sum;
}

float Struct::get_traj_ave_dihedral(int i, int j, int k, int l){
    float sum = 0.0;
    for(unsigned int m=0; m<pelisize;m++){
        sum += dihedral(atoms[i].peli[m], atoms[j].peli[m], atoms[k].peli[m], atoms[l].peli[m]);
    }
    sum /= pelisize;
    return sum;
}

void Struct::selectAtomsWithNBonds(vector<int> nbonds){
    int bonds[atoms.size()] = {0};
    for(auto at:atoms){
        bonds[at.index] += at.bonds.size();
        for(auto b:at.bonds){
            bonds[b.neighbor] += 1;
        }
    }

    //check which atoms has n bonds
    for(unsigned int i=0; i<atoms.size();i++){
        for(auto n:nbonds){
            if(bonds[i] == n){
                if(atoms[i].sel){
                    continue;
                }
                selat.emplace_back(&atoms[i]);
                atoms[i].sel = true;
            }
        }
    }
}

int Struct::exportPlumedFileCV(QString filename){
    //error code 1 means could not create/write in file

    QFile f(filename);

    if(!f.open(QIODevice::WriteOnly | QIODevice::Text)){
        cout<<"Could write file: "<<filename.toStdString()<<endl;
        return 1;
    }

    QTextStream out(&f);
    out<<"UNITS LENGTH=A TIME=fs\n\n\n";
    for(auto cv:col_var){
        if(cv.type == "bond"){
            out<<cv.name<<": DISTANCE ATOMS="<<cv.indices[0]+1<<","<<cv.indices[1]+1<<"  NOPBC\n";
        }
        else if(cv.type == "angle"){
            out<<cv.name<<": ANGLE ATOMS=";
            QString indices;
            for(auto i:cv.indices){
                indices += QString::number(i+1) + ",";
            }
            indices.chop(1);
            out<<indices<<"  NOPBC \n";

        }
        else if(cv.type == "dihedral"){
            out<<cv.name<<": TORSION ATOMS=";
            QString indices;
            for(auto i:cv.indices){
                indices += QString::number(i+1) + ",";
            }
            indices.chop(1);
            out<<indices<<"  NOPBC \n\n\n";
        }
    }

    //write the names for printing
    out<<"PRINT STRIDE=1  ARG=";
    QString names;
    for(auto cv:col_var){
        names += cv.name + ",";
    }
    names.chop(1);
    out<<names<<" FILE=colvar\n";

    f.close();
}

void Struct::dragInsideCell(float x, float y){
    //with this function , all atoms are dragged with the mouse
    // and are alway inside the cell, if they fall out, they are
    //moved inside the cell

    for(auto &at:atoms){
        at.coor[0] += x;
        at.coor[1] += y;
    }
    if(replicated){
       return;
    }

    //convert to fractional coordinates
    //to verify that they are inside the cell
    set_inv_mat();
    glob_rotate(atoms,  totinvmat);
    unshift_coors(atoms);
    frac_cart(atoms, false);
    bring_inside_cell(false);
    frac_cart(atoms, true);
    shift_coors(atoms);
    glob_rotate(atoms,  trotmat);
    genbonds();
}


void Struct::make_supercell(){
    if (!replicated){
        return;
    }

    for(auto &at:atoms){
        at.oind = -1;
        at.rep_xyz = {1, 1, 1};
    }

    for(unsigned int i=0;i<3;i++){
        for(unsigned int j =0; j<3;j++){
            cell[i][j] *= replication[i];
        }
    }

    set_cell_lines();
    // shift cell_lines
    for(unsigned int i=0;i<24;i++){
        cell_lines[i][0] -= Xcm;
        cell_lines[i][1] -= Ycm;
        cell_lines[i][2] -= Zcm;
    }
    glob_rotate(cell_lines, trotmat);

    replicated = false;
    replication = {1, 1, 1};
}
