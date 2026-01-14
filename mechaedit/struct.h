#ifndef STRUCT_H
#define STRUCT_H

#include <QFile>
#include <QTextStream>
#include <QXmlStreamReader>
#include <QMessageBox>
#include <QDir>
//#include <openbabel/mol.h>
//#include <openbabel/obconversion.h>
#include "atom.h"
#include "animation.h"
#include "plane.h"
#include "arrow.h"


using namespace std;

struct CV
{
    /*
     * types are:
     * bond, angle, dihedral
     */
    QString name;
    QString type;
    vector<int> indices;
};

struct CTRLZ{
    //no uso punteros como selat por al borrar y poner estoy haciendo copiass
    // y los punteros perderan su referencia
    QString action;
    vector<Atom> atoms;
    QStringList labels;
    vector<int> indices;
    vector<float> spheres;
    vector<float> radios;
    vector<float> cyls;
    vector<array<float,4>> colors;
    vector<array<float,3>> coors;
    array<int, 3> replication;
};

struct GROUP
{
  QString name;
  vector<int> indices;
};

class Struct : public QObject
{
     Q_OBJECT
public:
     Struct();
     //fatoms = final atoms, the ones to be saved
     vector<Atom> atoms,fatoms;
     vector <Atom*> selat;
     vector<Animation> animation;
     vector<Plane> planes;
     vector<Arrow> arrows;
     //mcoors = main coors, the one fo the optimized structure in case it is
     vector <array<float, 3>> mcoors, reversed_rot_vec;
     vector <vector <array<float, 3>>> peli, disp_peli;
     vector <CTRLZ> afterdata, beforedata;
     vector<Freq> freqs;
     vector<float> en_peli;
     vector<CV> col_var;  //collective variables

     array<array<float,3>,3> cell;
     array<array<float,3>,24> cell_lines;
     array<float, 12> cell_lines_widths;
     array<array<float, 4>, 12> cell_lines_colors;
     array<int, 3> replication;
     float Xcm, Ycm, Zcm, MM;
     float va, vb, vc, valpha, vbeta, vgamma;
     float cos_valpha, cos_vbeta, cos_vgamma;
     float sin_valpha, sin_vbeta, sin_vgamma;
     unsigned int pelisize = 0;
     float trotmat[3][3] = {{1.0,0.0,0.0},{0.0, 1.0, 0.0},{0.0, 0.0, 1.0}};
     float totinvmat[3][3] = {{1.0,0.0,0.0},{0.0, 1.0, 0.0},{0.0, 0.0, 1.0}};
     unsigned int peli_pos = 0, freq_pos = 5, cfreq=0, freqSteps=10;   //cfreq is current frequencie
     // fractions to displace the atoms to mimic vibration of frequency cfreq
     array<float, 11> freq_disp_fracs;
     bool freq_forward = true;
     bool showcell;
     bool replicated = false;
     QString current_file;
     VASP vasp;
     PWscf pwscf;
     Gaussian gaussian;
     vector<GROUP> groups;


    void add_to_before(QString action, vector<int> indices = {});
    void add_to_before(QString action, vector<array<float,3>> coors, vector<int> indices={});
    float get_traj_ave_distance(int i, int j);
    float get_traj_ave_angle(int i, int j, int k);
    float get_traj_ave_dihedral(int i, int j, int k, int l);
    void undo();
    void redo();
    void round_finalcoors();
    void drag(float dx, float dy, int atom = -1);
    bool add_rectangular_selection(int selectx, int selecty, int mouseX, int mouseY);
    vector<int> get_bonded_atoms(int atomo);
    vector<int> reversed_bond_search(int bonds, vector<int>atomslist);
//    vector<int> bondedRing(int index, vector<int>atomslist);
    void add_atom(float x, float y, QString element);
    bool add_to_selection(float x, float y);
    bool add_to_selection(vector<int> index);
    void alignToBond(QString dStr);
    void changeElements(QString element);
    int createGroup(QString name);
    int exportPlumedFileCV(QString filename);
    void frac_cart(vector <Atom> &coors, bool tocart = true, bool peli=false);
    void frac_cart(vector <Atom*> &coors, bool tocart = true, bool peli=false);
    void reverse_rotation(int which=0);
    void rotate_coors(float ang, float x, float y, float z = 0.0);
    void glob_rotate(vector <array<float,3>> &refcoors, float rotmat[3][3]);
    void glob_rotate(vector <Atom> &refatoms, float rotmat[3][3]);
    void glob_rotate(vector <Atom*> &refatoms, float rotmat[3][3]);
    void glob_rotate(float refcoors[3][3], float rotmat[3][3]);
    void glob_rotate(float refcoors[3], float rotmat[3][3]);
    void glob_rotate(array<float,3> &refcoors, float rotmat[3][3]);
    void glob_rotate(array<array<float,3>,24> &refcoors, float rotmat[3][3]);
    void update_coor(array<float,3> &refcoors, int index);
//    void rotate_cell_lines();
    void rotate_selection(float ang, float x, float y, float z = 0.0);
    void translate_selection(float x,float y,float z);
    void translateSelectionTo(float coor[3]);
    void set_cell_lines();
    void shift_to_mass_center(bool recalcXYZ=false);
    void load_xyz(QString inputfile);
    void load_poscar(QString inputfile);
    void load_outcar(QString inputfile);
    void load_pwo(QString inputfile);
    void load_pwi(QString inputfile);
    void load_gaussian_input(QString inputfile);
    void load_gaussian_output(QString inputfile);
    void load_cif(QString inputfile);
    void loadVaspXml(QString inputfile);
    void loadCml(QString inputfile);
    void loadCP2Kinput(QString inputfile);
    void loadPDB(QString inputfile);
    void appendFragmentCml(QString inputfile);
    void dragInsideCell(float x, float y);
    void make_supercell();
    array<float, 3> calculate_cif_symmetry_positions(QString line, array<float,3> &coor);
    void readfile(QString structfile);
    void genbonds();
    void paste_atoms(vector <Atom> &ats);
    void delete_selection();
    void delete_by_index(const vector<int> &indices);
    void readsettings();
    void save_xyz(QString filename);
    void save_xyz_trajectory(QString filename);
    void save_poscar(QString filename);
    void save_pwi(QString filename);
    void save_gaussian_input(QString filename);
    void update_atom_by_label(int index, QString label);
    void load_data(QString file);
    void update_color(float red, float green, float blue, float sat);
    void update_radios(float rad, bool ctrlz = false);
    void update_spheres(float rad, bool ctrlz = false);
    void update_cylinders(float rad);
    void update_cylinders(Atom &at, float rad);
    void select_inside_sphere(float rad);
    void select_inside_sphere_bonded(float rad);
    void select_by_elements(QStringList elements);
    void select_fixed();
    void invert_selection();
    void invertSelectionAtomsType();
    void reset_mat(float mat[3][3]);
    Atom clone_atoms(Atom &at);
    void set_stick_view(float radius = 0.15);
    void set_wire_view();
    void set_default_view();
    void set_default_color();
    void replicate(array<int, 3> xyz, bool selection=false);
    void replicate_structure(int x, int y, int z);
    void replicate_selection(int x, int y, int z);
    void select_all();
    void selectByIndex(QString list);
    void selectBySphere(float ss);
    void selectBySphere(float sphereRange[2]);
    void selectByNMR(float nmr);
    void select_replicated_copy();
    void select_original_copy();
    void selectAtomsWithNBonds(vector<int> nbonds);
    void set_inv_mat();
    void shift_coors(vector<Atom> &atoms);
    void shift_coors(vector<Atom*> &atoms);
    void shift_coors(vector<array<float, 3>> &coor);
    bool shift_peli(QString direction);
    bool shift_peli(int newpelipos);
    bool shift_freq_desp();

    void unselect_all();
    void unshift_coors(vector<Atom> &atoms);
    void unshift_coors(vector<Atom*> &atoms);
    void unshift_coors(vector<array<float, 3>> &coors);
    void unshift_coors(array<float,3> &coor);
    void connectivity();
    void update_selection_coors(array<float,3> newcoor, int index); //update the coordinates of the selected atoms
    void bring_inside_cell(bool sel);
    void set_unit_cell(float lat[3][3]);
    void set_unit_cell(float a, float b, float c, float alpha, float beta, float gamma);
    void recenter();
    void prueba();
    void unReplicate();
    void hide_selected();
    void showall();
    void export_collective_variable(QString filename);
    void average_peli(int interval);
    int newCollectiveVariable(QString name, QString type);
    void del_peli_by_index(vector<int> &indices);
    void check_covered();    //check whether there atoms over z axis so that it is not necesarry to render them
    bool valid_cell();
    void remove_duplicates(bool cart);
    void remove_duplicates_pbc_frac();

   protected:

   signals:
    void data_changed();

};

#endif // STRUCT_H
