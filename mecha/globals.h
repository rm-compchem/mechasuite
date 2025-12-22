#ifndef GLOBALS_H
#define GLOBALS_H
#define PI 3.14159265
#include <stdlib.h>
#include <stdio.h>
#include <map>
#include <string>
#include <iostream>
#include <vector>
#include <array>
#include <fstream>
#include <sstream>
#include <math.h>
#include <unistd.h>
#include <algorithm>
#include <thread>
#include <QMap>
#include <locale.h>
#include<QString>
#include<QStringList>
#include<omp.h>
#ifdef Q_OS_UNIX
#endif




using namespace std;

struct SPACEGROUP{
  QString name = "";
  QStringList symOperations = {};
  int number = 0;
};

struct BOND{
    float radio = 0.0;
    int neighbor = -1;
};

struct Freq{
    float vib;
    vector<array<float, 3>> disps; //displacement for every atom
};

struct bondedCycle
{
    int parent;
    vector<int> childs;
};

struct VASP{
    bool setup = false;
    float cell_scale = 1.0;
    QString comment;
    float cell[3][3];
    QStringList labels;
    vector<int> atoms;
    bool seldynamics = false;
    bool cartesian = false;
    bool old = false;
};

struct PWscf{
    bool setup = false;
    bool allfree = true;
    map <string, string> control;
    map <string, string> system;
    map <string, string> electrons;
    map <string, string> ions;
    map <string, string> cell;
    vector<array<string,3>> atomic_species;
    map <string, string> atomic_positions;
    QStringList kpoints;
    map <string, string> cell_parameters;
};

struct Gaussian{
    QStringList heading;
    bool fixed = false;
};

extern QMap<QString, float> radios;
extern QMap<QString, float> spheres;
extern QMap<QString, float> cylinders;
extern QMap<QString, float> masas;
extern QMap<int, QString> atomic_symbols;
extern QMap<QString, array<float,4>> colors;
extern array<string,73> element_symbols;
extern array<float, 12> clw;
extern array<array<float, 4>, 12> clc;
extern array<SPACEGROUP, 230> spaceGroups;
extern QString settings_folder, applicationFolder;
extern QString fragmentsFolder;
extern double memUsage;
extern double totalMemory;
extern double availableMemory;

void set_init_properties();
float dist(array<float,3> point1, array<float,3> point2);
float dist(float x1, float y1, float z1, float x2, float y2, float z2);
float average(vector<float> list);
//float average(vector<int> list);
float angle(array<float,3>p1, array<float,3>p2, array<float,3>p3);
float cos_angle(array<float,3>v1, array<float,3>v2);
vector<string> split(const string &s, char delim);
float dihedral(array<float,3>p1, array<float,3>p2, array<float,3>p3, array<float,3>p4);
void CrossProduct(float w[3], float v[3], float cross[3]);
float DotProduct(float v[3], float w[3]);
void VectorNormalisation(float v[3]);
bool check_equal_coors(array<float, 3> &a1, array<float, 3>&a2, float umb);
bool check_equal_replicated(array<float, 3> &a1, array<float, 3>&a2, float umb);
void updateMemoryUsage();
QStringList parse_symbols(QStringList &symbols);
QStringList special_split(QString line);

#endif // GLOBALS_H

