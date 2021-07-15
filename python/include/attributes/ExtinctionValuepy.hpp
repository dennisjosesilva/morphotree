#pragma once

#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <pybind11/functional.h>
#include <pybind11/numpy.h>

#include "morphotree/attributes/attributeComputer.hpp"
#include "morphotree/attributes/extinctionValues/ExtinctionValueComputer.hpp"

#include "core/opaque_types.hpp"

namespace py = pybind11;
namespace mt = morphotree;

template<typename AttrType, typename ValueType>
void bindExtinctionValueLeavesComputer(py::module &m, const std::string &attrType, 
  const std::string &valueType);

template<typename AttrType, typename ValueType>
void bindExtinctionValueComputer(py::module &m, const std::string &attrType,
  const std::string &valueType);

void bindFoundamentalTypeExtinctionValueLeavesComputer(py::module &m);
void bindFoundamentalTypeExtinctionValueComputer(py::module &m);

// ============================ [ IMPLEMENTATION ] ================================================
template<typename AttrType, typename ValueType>
void bindExtinctionValueLeavesComputer(py::module &m, const std::string &attrType, 
  const std::string &valueType)
{
  std::string className = "A" + attrType + "V" + valueType + "ExtinctionValueLeavesComputer";
  py::class_<mt::ExtinctionValueLeavesComputer<ValueType, AttrType>>(m, className.c_str())
    .def(py::init<>())
    .def("compute", &mt::ExtinctionValueLeavesComputer<ValueType, AttrType>::compute)
    .def_readonly_static("INF", &mt::ExtinctionValueLeavesComputer<ValueType, AttrType>::INF);
}

template<typename AttrType, typename ValueType>
void bindExtinctionValueComputer(py::module &m, const std::string &attrType, 
  const std::string &valueType)
{
  using MapType = std::unordered_map<mt::uint32, AttrType>;

  std::string className = "A" + attrType + "V" + valueType + "ExtinctionValueComputer";
  py::class_<mt::ExtinctionValueComputer<ValueType, AttrType>,
      mt::AttributeComputer<AttrType, ValueType>>(m, className.c_str())
    .def(py::init<const MapType&>(), py::arg("extValues"))
    .def("initAttributes", &mt::ExtinctionValueComputer<ValueType, AttrType>::initAttributes)
    .def("computeInitialValue", &mt::ExtinctionValueComputer<ValueType, AttrType>::computeInitialValue)
    .def("mergeToParent", &mt::ExtinctionValueComputer<ValueType, AttrType>::mergeToParent);
}