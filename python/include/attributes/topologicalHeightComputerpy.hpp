#pragma

#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <pybind11/functional.h>
#include <pybind11/numpy.h>

#include "morphotree/attributes/attributeComputer.hpp"
#include "morphotree/attributes/topologicalHeightComputer.hpp"

namespace py = pybind11;
namespace mt = morphotree;

template<typename ValueType>
void bindTopologicalHeightComputer(py::module &m, const std::string &valueType);

void bindFoundamentalTypesTopologicalHeightComputer(py::module &m);

// ================== [ IMPLEMENTATION ] ===============================================================
template<typename ValueType>
void bindTopologicalHeightComputer(py::module &m, const std::string &valueType)
{
  std::string className = valueType + "TopologicalHeightComputer";
  py::class_<mt::TopologicalHeightComputer<ValueType>, 
    mt::AttributeComputer<mt::uint32, ValueType>>(m, className.c_str())
  .def(py::init<>())
  .def("initAttributes", &mt::TopologicalHeightComputer<ValueType>::initAttributes)
  .def("computeInitialValue", &mt::TopologicalHeightComputer<ValueType>::computeInitialValue)
  .def("mergeToParent", &mt::TopologicalHeightComputer<ValueType>::mergeToParent);
}