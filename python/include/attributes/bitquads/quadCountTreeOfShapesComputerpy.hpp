#pragma once 

#include "morphotree/attributes/bitquads/quads.hpp"
#include "morphotree/attributes/bitquads/quadCountTreeOfShapesComputer.hpp"

#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <pybind11/stl_bind.h>

#include "core/opaque_types.hpp"

namespace py = pybind11;
namespace mt = morphotree;

void bindWindowMaxTreeNode(py::module &m);
void bindWindowMaxTree(py::module &m);

template<class ValueType>
void bindTreeOfShapesQuadCountsComputer(py::module &m, const std::string &type);

void bindFoundamentalTypesTreeOfShapesQuadCountsComputer(py::module &m);

// ======================== [ IMPLEMENTATION ] ===========================================
template<class ValueType>
void bindTreeOfShapesQuadCountsComputer(py::module &m, const std::string &type)
{
  std::string className = type + "TreeOfShapesQuadCountsComputer";
  py::class_<mt::TreeOfShapesQuadCountsComputer<ValueType>, mt::AttributeComputer<mt::Quads, ValueType>>(m, className.c_str())
    .def(py::init<const mt::KGrid<ValueType>&, const std::vector<mt::uint32>&>())
    .def("initAttributes", &mt::TreeOfShapesQuadCountsComputer<ValueType>::initAttributes)
    .def("computeInitialValue", &mt::TreeOfShapesQuadCountsComputer<ValueType>::computeInitialValue)
    .def("mergeToParent", &mt::TreeOfShapesQuadCountsComputer<ValueType>::mergeToParent);
}