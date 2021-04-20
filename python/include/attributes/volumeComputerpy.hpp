#pragma once 

#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <pybind11/functional.h>
#include <pybind11/numpy.h>

#include "morphotree/attributes/attributeComputer.hpp"
#include "morphotree/attributes/volumeComputer.hpp"

#include "core/opaque_types.hpp"

namespace py = pybind11;
namespace mt = morphotree;

template<typename ValueType>
void bindMaxTreeVolumeComputer(py::module &m, const std::string &valueType);

void bindFoundamentalTypesMaxTreeVolumeComputer(py::module &m);

// ======================= [ IMPLEMENTATION ] ===================================================
template<typename ValueType>
void bindMaxTreeVolumeComputer(py::module &m, const std::string &valueType)
{
  std::string className = valueType + "MaxTreeVolumeComputer";
  py::class_<mt::MaxTreeVolumeComputer<ValueType>, 
    mt::AttributeComputer<float, ValueType>>(m, className.c_str())
  .def(py::init<>())
  .def("initAttributes", &mt::MaxTreeVolumeComputer<ValueType>::initAttributes)
  .def("computeInitialValue", &mt::MaxTreeVolumeComputer<ValueType>::computeInitialValue)
  .def("mergeToParent", &mt::MaxTreeVolumeComputer<ValueType>::mergeToParent)
  .def("finaliseComputation", &mt::MaxTreeVolumeComputer<ValueType>::finaliseComputation);
}