#pragma once

#include "morphotree/tree/ct_builder.hpp"
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include "core/opaque_types.hpp"

namespace py = pybind11;
namespace mt = morphotree;

template<typename T>
void bindCTBuilder(py::module &m, const std::string type);

void bindFoundamentalTypeCTBuilder(py::module &m);

// ==============[ MY IMPLEMENTATION ] ===========================
template<typename T>
void bindCTBuilder(py::module &m, const std::string type)
{
  std::string className = type + "CTBuilder";
  py::class_<mt::CTBuilder<T>>(m, className.c_str())
    .def(py::init<>())
    .def("build", &mt::CTBuilder<T>::build);
}
