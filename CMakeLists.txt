cmake_minimum_required(VERSION 3.15)
project(example LANGUAGES CXX)

find_package(pybind11 REQUIRED)

pybind11_add_module(kmc mechasuite/kmc.cpp)

install(TARGETS kmc LIBRARY DESTINATION mechasuite)

