#pragma once 

#include "morphotree/attributes/bitquads/quads.hpp"
#include "morphotree/attributes/bitquads/quadCountComputer.hpp"

#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <pybind11/stl_bind.h>


#include "core/opaque_types.hpp"

namespace py = pybind11;
namespace mt = morphotree;


template<typename ValueType>
void bindCTreeQuadCountsComputer(py::module &m, const std::string &type);

void bindQuads(py::module &m);
void bindQuadsVector(py::module &m);
void bindFoundamentalTypesCTreeQuadCountsComputer(py::module &m);

// ===================== [ IMPLEMENTATION ] =======================================
template<typename ValueType>
void bindCTreeQuadCountsComputer(py::module &m, const std::string &type)
{
  std::string className = type + "CTreeQuadCountsComputer";
  py::class_<mt::CTreeQuadCountsComputer<ValueType>, mt::AttributeComputer<mt::Quads, ValueType>>(m, className.c_str())
    .def(py::init<mt::Box, const std::vector<ValueType> &, const std::string &>())
    .def("initAttributes", &mt::CTreeQuadCountsComputer<ValueType>::initAttributes)
    .def("computeInitialValue", &mt::CTreeQuadCountsComputer<ValueType>::computeInitialValue)
    .def("mergeToParent", &mt::CTreeQuadCountsComputer<ValueType>::mergeToParent);
}