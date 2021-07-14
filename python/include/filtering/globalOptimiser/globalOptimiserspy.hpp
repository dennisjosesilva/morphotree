#pragma once

#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <pybind11/functional.h>
#include <pybind11/numpy.h>

#include "morphotree/filtering/globalOptimiser/MinCPerimeterWithAbsError.hpp"
#include "morphotree/filtering/globalOptimiser/MinCPerimeterWithAbsErrorToS.hpp"
#include "morphotree/filtering/globalOptimiser/MinCPerimeterWithSquaredErrorToS.hpp"

#include "core/opaque_types.hpp"

namespace py = pybind11;
namespace mt = morphotree;

template<typename ValueType>
void bindMinCPerimeterWithAbsError(py::module &m, const std::string &valueType);

template<typename ValueType>
void bindMinCPerimeterWithAbsErrorToS(py::module &m, const std::string &valueType);

template<typename ValueType>
void bindMinCPerimeterWithSquaredErrorToS(py::module &m, const std::string &valueType);

void bindFoundamentalTypeMinCPerimeterWithAbsError(py::module &m);
void bindFoundamentalTypeMinCPerimeterWithAbsErrorToS(py::module &m);
void bindFoundamentalTypeMinCPerimeterWithSquaredErrorToS(py::module &m);

// ======================== [ IMPLEMENTATION ] ==================================================
template<typename ValueType>
void bindMinCPerimeterWithAbsError(py::module &m, const std::string &valueType)
{
  using MTree = mt::MorphologicalTree<ValueType>;

  std::string className = valueType + "MinCPerimeterWithAbsError";
  py::class_<mt::MinCPerimeterWithAbsError<ValueType>>(m, className.c_str())
    .def(py::init<const mt::Box&, const std::vector<ValueType>&, std::string, const MTree&, float>(),
         py::arg("domain"), py::arg("f"), py::arg("dtFilename"), py::arg("tree"), 
         py::arg("approxThreshold"))
    .def_property_readonly("absError", &mt::MinCPerimeterWithAbsError<ValueType>::absError)
    .def_property_readonly("sumPerimeter", &mt::MinCPerimeterWithAbsError<ValueType>::sumPerimeter)
    .def_property_readonly("numFilteredNodes", &mt::MinCPerimeterWithAbsError<ValueType>::numFilteredNodes)
    .def_property_readonly("normAbsError", &mt::MinCPerimeterWithAbsError<ValueType>::normAbsError)
    .def("filter", &mt::MinCPerimeterWithAbsError<ValueType>::filter)
    .def("filterWithNormalisedError", &mt::MinCPerimeterWithAbsError<ValueType>::filterWithNormalisedError);
}

template<typename ValueType>
void bindMinCPerimeterWithAbsErrorToS(py::module &m, const std::string &valueType)
{
  using MTree = mt::MorphologicalTree<ValueType>;

  std::string className = valueType + "MinCPerimeterWithAbsErrorToS";
  py::class_<mt::MinCPerimeterWithAbsErrorToS<ValueType>>(m, className.c_str())
    .def(py::init<const mt::Box&, const std::vector<ValueType>&, double>(), 
         py::arg("domain"), py::arg("f"), py::arg("approxThreshold"))
    .def_property_readonly("sumPerimeter", &mt::MinCPerimeterWithAbsErrorToS<ValueType>::sumPerimeter)
    .def_property_readonly("absError", &mt::MinCPerimeterWithAbsErrorToS<ValueType>::absError)
    .def_property_readonly("fsumPerimeter", &mt::MinCPerimeterWithAbsErrorToS<ValueType>::fsumPerimeter)
    .def_property_readonly("numberOfNodes", &mt::MinCPerimeterWithAbsErrorToS<ValueType>::numberOfNodes)
    .def_property_readonly("numPrunnedNodes", &mt::MinCPerimeterWithAbsErrorToS<ValueType>::numPrunnedNodes)
    .def_property_readonly("normAbsError", &mt::MinCPerimeterWithAbsErrorToS<ValueType>::normAbsError)
    .def("filter", &mt::MinCPerimeterWithAbsErrorToS<ValueType>::filter)
    .def("filterWithNormalisedError", &mt::MinCPerimeterWithAbsErrorToS<ValueType>::filterWithNormalisedError);
}

template<typename ValueType>
void bindMinCPerimeterWithSquaredErrorToS(py::module &m, const std::string &valueType)
{
  using MTree = mt::MorphologicalTree<ValueType>;

  std::string className = valueType + "MinCPerimeterWithSquaredErrorToS";
  py::class_<mt::MinCPerimeterWithSquaredErrorToS<ValueType>>(m, className.c_str())
    .def(py::init<const mt::Box&, const std::vector<ValueType>&, double>(), 
         py::arg("domain"), py::arg("f"), py::arg("approxThreshold"))
    .def_property_readonly("sunPerimeter", &mt::MinCPerimeterWithSquaredErrorToS<ValueType>::sumPerimeter)
    .def_property_readonly("squaredError", &mt::MinCPerimeterWithSquaredErrorToS<ValueType>::squaredError)
    .def_property_readonly("fsumPerimeter", &mt::MinCPerimeterWithSquaredErrorToS<ValueType>::fsumPerimeter)
    .def_property_readonly("numberOfNodes", &mt::MinCPerimeterWithSquaredErrorToS<ValueType>::numberOfNodes)
    .def_property_readonly("numPrunnedNodes", &mt::MinCPerimeterWithSquaredErrorToS<ValueType>::numPrunnedNodes)
    .def_property_readonly("normSquaredError", &mt::MinCPerimeterWithSquaredErrorToS<ValueType>::normSquaredError)
    .def("filter", &mt::MinCPerimeterWithSquaredErrorToS<ValueType>::filter)
    .def("filterWithNormalisedError", &mt::MinCPerimeterWithSquaredErrorToS<ValueType>::filterWithNormalisedError);
}