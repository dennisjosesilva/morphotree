#pragma once

#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <pybind11/functional.h>
#include <pybind11/numpy.h>

#include "morphotree/attributes/attributeComputer.hpp"
#include "morphotree/attributes/areaComputer.hpp"
#include "morphotree/attributes/perimeterComputer.hpp"

#include "core/opaque_types.hpp"

namespace py = pybind11;
namespace mt = morphotree;

template<typename AttrType, typename ValueType>
void bindAttributeComputer(py::module &m, const std::string &attrType, const std::string &valueType);

template<typename ValueType>
void bindAreaComputer(py::module &m, const std::string &type);

template<typename ValueType>
void bindMaxTreePerimeterComputer(py::module &m, const std::string &type);

template<typename ValueType>
void bindMinTreePerimeterComputer(py::module &m, const std::string &type);

template<class AttrType, typename ValueType>
class AttributeComputerPy : public mt::AttributeComputer<AttrType, ValueType>
{
  using TreeType = mt::MorphologicalTree<ValueType>;
  using NodePtr = typename TreeType::NodePtr;
  using AttributeComputerType = mt::AttributeComputer<AttrType, ValueType>;

  using mt::AttributeComputer<AttrType, ValueType>::AttributeComputer;

  std::vector<AttrType> initAttributes(const TreeType &tree) override {
    PYBIND11_OVERLOAD_PURE(
      std::vector<AttrType>, /* Return Type */
      AttributeComputerType, /* Parent Type */
      initAttributes, /* Name of function in C++ (must match Python name) */
      tree /*Argument (s) */
    );
  }

  void computeInitialValue(std::vector<AttrType> &attr, NodePtr node) override {
    py::object dummy = py::cast(&attr);
    PYBIND11_OVERLOAD_PURE(
      void, /* Return Type*/
      AttributeComputerType, /* Parent Type */ 
      computeInitialValue, /* Name of function in C++ (must match Python name) */
      attr, node  /* Arguments */
    );
  }

  void mergeToParent(std::vector<AttrType> &attr, NodePtr node, NodePtr parent) override {
    py::object dummy = py::cast(&attr);
    PYBIND11_OVERLOAD_PURE(
      void, /* Return Type  */
      AttributeComputerType, /* Parent Type */
      mergeToParent,
      attr, node, parent /* Arguments */
    );
  }
};

void bindFoundamentalTypeAttributeComputer(py::module &m);
void bindFoundamentalTypeAreaComputer(py::module &m);
void bindFoundamentalTypeMaxTreePerimeterComputer(py::module &m);
void bindFoundamentalTypeMinTreePerimeterComputer(py::module &m);

// ===================== [IMPLEMENTATION ] ================================================================
template<typename AttrType, typename ValueType>
void bindAttributeComputer(py::module &m, const std::string &attrType, const std::string &valueType)
{
  std::string className = "A" + attrType + "V" + valueType + "AttributeComputer";
  py::class_<mt::AttributeComputer<AttrType, ValueType>, AttributeComputerPy<AttrType, ValueType>>(m, className.c_str())
    .def(py::init<>())
    .def("computeAttribute", &mt::AttributeComputer<AttrType, ValueType>::computeAttribute)
    .def("initAttributes", &mt::AttributeComputer<AttrType, ValueType>::initAttributes)
    .def("computeInitialValue", &mt::AttributeComputer<AttrType, ValueType>::computeInitialValue)
    .def("mergeToParent", &mt::AttributeComputer<AttrType, ValueType>::mergeToParent);
}


template<typename ValueType>
void bindAreaComputer(py::module &m, const std::string &type)
{
  std::string className = type + "AreaComputer";
  py::class_<mt::AreaComputer<ValueType>, mt::AttributeComputer<mt::uint32, ValueType>>(m, className.c_str())
    .def(py::init<>())
    .def("initAttributes", &mt::AreaComputer<ValueType>::initAttributes)
    .def("computeInitialValue", &mt::AreaComputer<ValueType>::computeInitialValue)
    .def("mergeToParent", &mt::AreaComputer<ValueType>::mergeToParent);
}

template<typename ValueType>
void bindMaxTreePerimeterComputer(py::module &m, const std::string &type)
{
  std::string className = type + "MaxTreePerimeterComputer";
  py::class_<mt::MaxTreePerimeterComputer<ValueType>, mt::AttributeComputer<mt::uint32, ValueType>>(m, className.c_str())
    .def(py::init<mt::Box, const std::vector<ValueType>&>())
    .def("initAttributes", &mt::MaxTreePerimeterComputer<ValueType>::initAttributes)
    .def("computeInitialValue", &mt::MaxTreePerimeterComputer<ValueType>::computeInitialValue)
    .def("mergeToParent", &mt::MaxTreePerimeterComputer<ValueType>::mergeToParent);
}

template<typename ValueType>
void bindMinTreePerimeterComputer(py::module &m, const std::string &type)
{
  std::string className = type + "MinTreePerimeterComputer";
  py::class_<mt::MinTreePerimeterComputer<ValueType>, mt::AttributeComputer<mt::uint32, ValueType>>(m, className.c_str())
    .def(py::init<mt::Box, const std::vector<ValueType>&>())
    .def("initAttributes", &mt::MinTreePerimeterComputer<ValueType>::initAttributes)
    .def("computeInitialValue", &mt::MinTreePerimeterComputer<ValueType>::computeInitialValue)
    .def("mergeToParent", &mt::MinTreePerimeterComputer<ValueType>::mergeToParent);
}