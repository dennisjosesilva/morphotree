#pragma once 

#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <pybind11/functional.h>
#include <pybind11/numpy.h>

#include "morphotree/attributes/attributeComputer.hpp"
#include "morphotree/attributes/smoothnessContourComputer.hpp"

#include "core/opaque_types.hpp"

namespace py = pybind11;
namespace mt = morphotree;

template<typename ValueType>
void bindMaxTreeSmoothnessContourComputer(py::module &m, const std::string &valueType);

template<typename ValueType>
void bindMinTreeSmoothnessContourComputer(py::module &m, const std::string &valueType);

template<typename ValueType>
void bindTreeOfShapesSmoothnessContourComputer(py::module &m, const std::string &valueType);

void bindFoundamentalTypeMaxTreeSmoothnessContourComputer(py::module &m);
void bindFoundamentalTypeMinTreeSmoothnessContourComputer(py::module &m);
void bindFoundamentalTypeTreeOfShapesSmoothnessContourComputer(py::module &m);

// ========================== [IMPLEMENTATION] ===================================================
template<typename ValueType>
void bindMaxTreeSmoothnessContourComputer(py::module &m, const std::string &valueType)
{
  std::string className =  valueType + "MaxTreeSmoothnessContourComputer";
  py::class_<mt::MaxTreeSmoothnessContourComputer<ValueType>, 
    mt::AttributeComputer<float, ValueType>>(m, className.c_str())
    .def(py::init<mt::Box, const std::vector<ValueType>&>())
    .def("initAttributes", &mt::MaxTreeSmoothnessContourComputer<ValueType>::initAttributes)
    .def("computeInitialValue", &mt::MaxTreeSmoothnessContourComputer<ValueType>::computeInitialValue)
    .def("mergeToParent", &mt::MaxTreeSmoothnessContourComputer<ValueType>::mergeToParent)
    .def("finaliseComputation", &mt::MaxTreeSmoothnessContourComputer<ValueType>::finaliseComputation);
}

template<typename ValueType>
void bindMinTreeSmoothnessContourComputer(py::module &m, const std::string &valueType)
{
  std::string className = valueType + "MinTreeSmoothnessContourComputer";
  py::class_<mt::MinTreeSmoothnessContourComputer<ValueType>, 
    mt::AttributeComputer<float, ValueType>>(m, className.c_str())
    .def(py::init<mt::Box, const std::vector<ValueType>&>())
    .def("initAttributes", &mt::MinTreeSmoothnessContourComputer<ValueType>::initAttributes)
    .def("computeInitialValue", &mt::MinTreeSmoothnessContourComputer<ValueType>::computeInitialValue)
    .def("mergeToParent", &mt::MinTreeSmoothnessContourComputer<ValueType>::mergeToParent)
    .def("finaliseComputation", &mt::MinTreeSmoothnessContourComputer<ValueType>::finaliseComputation);
}

template<typename ValueType>
void bindTreeOfShapesSmoothnessContourComputer(py::module &m, const std::string &valueType)
{
  std::string className = valueType + "TreeOfShapesSmoothnessContourComputer";
  py::class_<mt::TreeOfShapesSmoothnessContourComputer<ValueType>, 
    mt::AttributeComputer<float, ValueType>>(m, className.c_str())
    .def(py::init<mt::Box, const std::vector<ValueType>&>())
    .def("initAttributes", &mt::TreeOfShapesSmoothnessContourComputer<ValueType>::initAttributes)
    .def("computeInitialValue", &mt::TreeOfShapesSmoothnessContourComputer<ValueType>::computeInitialValue)
    .def("mergeToParent", &mt::TreeOfShapesSmoothnessContourComputer<ValueType>::mergeToParent)
    .def("finaliseComputation", &mt::TreeOfShapesSmoothnessContourComputer<ValueType>::finaliseComputation);
}