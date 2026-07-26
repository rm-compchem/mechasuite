cmake_minimum_required(VERSION 3.15)
project(example LANGUAGES CXX)

find_package(pybind11 REQUIRED)

pybind11_add_module(mk
		mechasuite/kmc_bindings.cpp 
		mechasuite/kmc_common.cpp 
		mechasuite/kmc_ode.cpp
		mechasuite/kmc_system.cpp
	)

install(TARGETS mk LIBRARY DESTINATION mechasuite)

