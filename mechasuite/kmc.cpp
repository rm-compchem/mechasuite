#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <string>
#include <random>
#include <cmath>

//g++ kmc.cpp  $(python3 -m pybind11 --includes) -shared -fPIC  -o kmc$(python3-config --extension-suffix)


//#include "json.hpp"
//using json = nlohmann::json;
using namespace std;

constexpr double Rgas = 8.314462618; // J/mol/K

// ---------------- Reaction types ----------------
enum class ReactionType 
{
    Surface,      // Surface reaction: R* -> I*
    Adsorption,   // Gas -> Surface
    Desorption    // Surface -> Gas (leaves reactor)
};

// ---------------- Reaction ----------------
struct Reaction 
{
    ReactionType type;
    string species;      // surface species
    string product;      // only for surface reactions
    double k;
    double Ea;
};

// ---------------- Arrhenius ----------------
double arrhenius(double k298, double T, double Ea) 
{
    return k298 * exp(-Ea / Rgas * (1.0 / T - 1.0 / 298.0));
}

// ---------------- KMC Engine ----------------
class KMC 
{
public:
    map<string, int> surface_species;    // tracked surface species
    map<string, std::vector<int>> species_evol;    // tracked surface species
    map<string, double> out_gas;         // counts desorbed molecules
    vector<Reaction> reactions;
    vector<double> time_evol = {0.0};

    int N_sites = 0;    // total surface sites
    int n_empty = 0;    // empty sites
    double temperature;
    double pressure = 1.0;  // gas inlet pressure (constant feed)

    KMC() : rng(12345), uni(0.0,1.0), time(0.0) {}

    void addReaction(Reaction r)
    {
      reactions.push_back(r);
    }

    void addSurfaceSpecies(string name, int n)
    {
      surface_species[name] = n;
      species_evol[name] = {n};
      n_empty -= n;
    }

    void run(double t_end) {
        while (time < t_end) {
            vector<double> rates;
            double Rtot = 0.0;

            // Compute propensities
            for (auto &r : reactions) {
                //double k = arrhenius(r.k, temperature, r.Ea);
                double k = r.k;
                double a = 0.0;

                if (r.type == ReactionType::Surface) {
                    a = k * surface_species[r.species];
                }
                else if (r.type == ReactionType::Adsorption) {
                    a = k * pressure * n_empty; // only empty sites limit adsorption
                }
                else if (r.type == ReactionType::Desorption) {
                    a = k * surface_species[r.species];
                }

                rates.push_back(a);
                Rtot += a;
            }

            if (Rtot <= 0.0) break;

            // Gillespie time step
            double dt = -log(uni(rng)) / Rtot;
            time += dt;

            // Select reaction
            double rrand = uni(rng) * Rtot;
            double cum = 0.0;
            size_t chosen = 0;
            for (size_t i=0;i<rates.size();++i){
                cum += rates[i];
                if (rrand < cum) {
                    chosen = i;
                    break;
                }
            }

            // Fire reaction
            Reaction &rxn = reactions[chosen];

            if (rxn.type == ReactionType::Surface) {
                if (surface_species[rxn.species] > 0) {
                    surface_species[rxn.species]--;
                    surface_species[rxn.product]++;
                }
            }
            else if (rxn.type == ReactionType::Adsorption) {
                if (n_empty > 0) {
                    n_empty--;
                    surface_species[rxn.species]++;
                    // gas reservoir is constant
                }
            }
            else if (rxn.type == ReactionType::Desorption) {
                if (surface_species[rxn.species] > 0) {
                    surface_species[rxn.species]--;
                    n_empty++;
                    out_gas[rxn.species] += 1.0; // track molecules leaving reactor
                }
            }

	    if(time > t_end) break;

            // Log
            cout << "t= " << time;
	    time_evol.push_back(time);
            for (auto &sp : surface_species){
                cout << " " << sp.first << "= " << sp.second;
		species_evol[sp.first].push_back(sp.second);
	    }
            cout << " empty= " << n_empty;

            // optional: print desorbed gas counts
            for (auto &g : out_gas)
                cout << " " << g.first << "_out= " << g.second;
            cout << endl;
        }
    }

private:
    mt19937_64 rng;
    uniform_real_distribution<double> uni;
    double time;
};


namespace py = pybind11;

PYBIND11_MODULE(kmc, m)
{
  py::class_<KMC>(m, "kMC")
	  .def(py::init<>())
	  .def("run", &KMC::run)
	  .def("add_reaction", &KMC::addReaction)
	  .def("add_surface_species", &KMC::addSurfaceSpecies)
	  //.def("surface_species", &KMC::surface_species)
	  //.def("out_gas", &KMC::out_gas)
	  .def_readwrite("N_sites", &KMC::N_sites)
	  .def_readwrite("n_empty", &KMC::n_empty)
	  .def_readwrite("temperature", &KMC::temperature)
	  .def_readwrite("pressure", &KMC::pressure)
	  .def_readwrite("surface_species", &KMC::surface_species)
	  .def_readwrite("species_evol", &KMC::species_evol)
	  .def_readonly("time_evol", &KMC::time_evol)

	  ;

    py::enum_<ReactionType>(m, "ReactionType")
	  .value("Surface", ReactionType::Surface)
	  .value("Adsorption", ReactionType::Adsorption)
	  .value("Desorption", ReactionType::Desorption)
	  .export_values()
	  ;

  py::class_<Reaction>(m, "Reaction")
          .def(py::init<>())
          .def(py::init<ReactionType,string, string, double, double >())
	  .def_readwrite("type", &Reaction::type)
	  .def_readwrite("species", &Reaction::species)
	  .def_readwrite("product", &Reaction::product)
	  .def_readwrite("k", &Reaction::k)
	  .def_readwrite("Ea", &Reaction::Ea)
	  ;

}


