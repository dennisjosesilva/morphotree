#pragma

#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <pybind11/functional.h>
#include <pybind11/numpy.h>

#include "morphotree/attributes/attributeComputer.hpp"
#include "morphotree/attributes/boundingboxComputer.hpp"

#include "core/opaque_types.hpp"

namespace py = pybind11;
namespace mt = morphotree;

template<typename ValueType>
void bindBoundingBoxComputer(py::module &m, const std::string &valueType);

void bindFoundamentalTypesBoundingBoxComputer(py::module &m);

// ======================= [ IMPLEMENTATION ] ======================================================
template<typename ValueType>
void bindBoundingBoxComputer(py::module &m, const std::string &valueType)
{
  std::string className = valueType + "BoundingBoxComputer";
  py::class_<mt::BoundingBoxComputer<ValueType>,
    mt::AttributeComputer<mt::Box, ValueType>>(m, className.c_str())
  .def(py::init<mt::Box>(), py::arg("domain"))
  .def("initAttributes", &mt::BoundingBoxComputer<ValueType>::initAttributes)
  .def("computeInitialValue", &mt::BoundingBoxComputer<ValueType>::computeInitialValue)
  .def("mergeToParent", &mt::BoundingBoxComputer<ValueType>::mergeToParent);
}