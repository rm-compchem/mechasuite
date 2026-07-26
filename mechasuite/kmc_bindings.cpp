#include "kmc_common.h"
#include "kmc_system.h"
#include "kmc_ode.h"

#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

namespace py = pybind11;

PYBIND11_MODULE(mk, m) {

    py::class_<Reaction>(m, "Reaction")
        .def(py::init<>())
        .def_readwrite("reactants", &Reaction::reactants)
        .def_readwrite("products", &Reaction::products)
        .def_readwrite("rate", &Reaction::rate)
        .def_readwrite("adsorption", &Reaction::adsorption)
        .def_readwrite("desorption", &Reaction::desorption)
    ;

    py::class_<Reactor>(m, "Reactor")
        .def(py::init<>())
        .def_readwrite("closed_system", &Reactor::closed_system)
        .def_readwrite("residence_time", &Reactor::residence_time)
        .def_readwrite("gas_species", &Reactor::gas_species)
        .def_readwrite("reservoir", &Reactor::reservoir)
        .def_readwrite("partial_pressure", &Reactor::partial_pressure)
        .def("isGas", &Reactor::isGas)
        .def_readwrite("volume", &Reactor::volume)
        .def_readwrite("temperature", &Reactor::temperature)
        .def_readwrite("scaleup", &Reactor::scaleup)
    ;

    // ---------------- stochastic engine ----------------
    py::class_<System>(m, "System")
        .def(py::init<>())
        .def_readwrite("species", &System::species)
        .def_readwrite("reactions", &System::reactions)
        .def_readwrite("reactor", &System::reactor)
        .def_readwrite("history", &System::history)
        .def_readwrite("times", &System::times)
        .def_readwrite("reaction_extent", &System::reaction_extent)
        .def_readwrite("extent_history", &System::extent_history)
        .def_readwrite("save_freq", &System::saveFreq)
        .def_readwrite("step", &System::step)
        .def_readwrite("logfile", &System::logfile)
        .def_readonly("names", &System::names)
        .def("addSpecies", &System::addSpecies)
        .def("getSpeciesIndex", &System::getSpeciesIndex)
        .def("addReaction", &System::addReaction)
        .def("print_reactions", &System::printReactions)
        .def("computeAllPropensities", &System::computeAllPropensities)
        .def("updatePropensities", &System::updatePropensities)
        .def("stepSSA", &System::stepSSA)
        .def("stepTau", &System::stepTau)
        .def_static("binomial", &System::binomial)
    ;

    py::class_<KMC>(m, "KMC")
        .def(py::init<System&, unsigned>(), py::arg("system"), py::arg("seed")=42)
        .def_property_readonly("system", [](KMC &self) -> System& { return self.system; })
        .def_property("steady_window",
            [](KMC& k){ return k.steady.window; },
            [](KMC& k, double v){ k.steady.window = v; })
        .def_property("steady_check_freq",
            [](KMC& k){ return k.steady.checkFreq; },
            [](KMC& k, size_t v){ k.steady.checkFreq = v; })
        .def_property("steady_tol",
            [](KMC& k){ return k.steady.tol; },
            [](KMC& k, double v){ k.steady.tol = v; })
        .def_property_readonly("steady_onset", [](KMC& k){ return k.steady.onset; })
        .def("runSSA", &KMC::runSSA)
        .def("runTau", &KMC::runTau)
        .def("runAdaptive", &KMC::runAdaptive,
            py::arg("t_end"), py::arg("max_steps"), py::arg("eps")=0.03,
            py::arg("nc")=10, py::arg("nSSAFallback")=100)
    ;

    // ---------------- deterministic mean-field engine ----------------
    py::class_<ODESystem>(m, "ODESystem")
        .def(py::init<const System&>(), py::arg("system"))
        .def_readwrite("species", &ODESystem::species)
        .def_readwrite("reactions", &ODESystem::reactions)
        .def_readwrite("reactor", &ODESystem::reactor)
        .def_readwrite("history", &ODESystem::history)
        .def_readwrite("times", &ODESystem::times)
        .def_readwrite("reaction_extent", &ODESystem::reaction_extent)
        .def_readwrite("extent_history", &ODESystem::extent_history)
        .def_readwrite("save_freq", &ODESystem::saveFreq)
        .def_readwrite("step", &ODESystem::step)
        .def_readwrite("logfile", &ODESystem::logfile)
        .def_readonly("names", &ODESystem::names)
        .def("getSpeciesIndex", &ODESystem::getSpeciesIndex)
        .def("gasLogFilename", &ODESystem::gasLogFilename)
    ;

    py::class_<MeanFieldODE>(m, "MeanFieldODE")
        .def(py::init<ODESystem&>(), py::arg("system"))
        .def_property_readonly("system", [](MeanFieldODE &self) -> ODESystem& { return self.system; })
        .def_property("steady_window",
            [](MeanFieldODE& k){ return k.steady.window; },
            [](MeanFieldODE& k, double v){ k.steady.window = v; })
        .def_property("steady_check_freq",
            [](MeanFieldODE& k){ return k.steady.checkFreq; },
            [](MeanFieldODE& k, size_t v){ k.steady.checkFreq = v; })
        .def_property("steady_tol",
            [](MeanFieldODE& k){ return k.steady.tol; },
            [](MeanFieldODE& k, double v){ k.steady.tol = v; })
        .def_property_readonly("steady_onset", [](MeanFieldODE& k){ return k.steady.onset; })
        .def("run", &MeanFieldODE::run,
            py::arg("t_end"), py::arg("rtol")=1e-6, py::arg("atol")=1e-6,
            py::arg("h_init")=1e-8, py::arg("h_min")=1e-14, py::arg("h_max")=-1.0,
            py::arg("max_steps")=2000000)
    ;
}
