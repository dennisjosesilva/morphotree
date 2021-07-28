#pragma

#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <pybind11/functional.h>
#include <pybind11/numpy.h>

#include "morphotree/attributes/attributeComputer.hpp"
#include "morphotree/attributes/numberOfDescendantsComputer.hpp"

namespace py = pybind11;
namespace mt = morphotree;

template<typename ValueType>
void bindNumberOfDescendantsComputer(py::module &m, const std::string &valueType);

void bindFoundamentalTypesNumberOfDescendantsComputer(py::module &m);

// ======================== [ IMPLEMENTATION ] ======================================================
template<typename ValueType>
void bindNumberOfDescendantsComputer(py::module &m, const std::string &valueType)
{
  std::string className = valueType + "NumberOfDescendantsComputer";
  py::class_<mt::NumberOfDescendantsComputer<ValueType>,
    mt::AttributeComputer<mt::uint32, ValueType>>(m, className.c_str())
  .def(py::init<>())
  .def("initAttributes", &mt::NumberOfDescendantsComputer<ValueType>::initAttributes)
  .def("computeInitialValue", &mt::NumberOfDescendantsComputer<ValueType>::computeInitialValue)
  .def("mergeToParent", &mt::NumberOfDescendantsComputer<ValueType>::mergeToParent);
}