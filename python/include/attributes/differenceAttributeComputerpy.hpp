#pragma once 

#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <pybind11/functional.h>
#include <pybind11/numpy.h>

#include "morphotree/attributes/attributeComputer.hpp"
#include "morphotree/attributes/differenceAttributeComputer.hpp"

#include "core/opaque_types.hpp"

namespace py = pybind11;
namespace mt = morphotree;

template<typename AttrType, typename ValueType>
void bindDifferenceAttributeComputer(py::module &m, const std::string &attrType,
  const std::string &valueType);

void bindFoundamentalTypesDifferenceAttributeComputer(py::module &m);

// ========================= [ IMPLEMENTATION ] ================================================
template<typename AttrType, typename ValueType>
void bindDifferenceAttributeComputer(py::module &m, const std::string &attrType, 
  const std::string &valueType)
{
  std::string className = "A" + attrType + "V" + valueType + "DifferenceAttributeComputer";
  py::class_<mt::DifferenceAttributeComputer<AttrType, ValueType>,
    mt::AttributeComputer<AttrType, ValueType>>(m, className.c_str())
  .def(py::init<std::vector<AttrType>&>(), py::arg("underlyingAttr"))
  .def("initAttributes", &mt::DifferenceAttributeComputer<AttrType, ValueType>::initAttributes)
  .def("computeInitialValue", &mt::DifferenceAttributeComputer<AttrType, ValueType>::computeInitialValue)
  .def("mergeToParent", &mt::DifferenceAttributeComputer<AttrType, ValueType>::mergeToParent);
}