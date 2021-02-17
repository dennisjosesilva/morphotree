#pragma once

#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

#include "morphotree/tree/treeOfShapes/kgrid.hpp"

#include "core/opaque_types.hpp"

#include <string>
#include <sstream>

namespace py = pybind11;
namespace mt = morphotree;

template<typename T>
void bindInterval(py::module &m, const std::string &type);

template<typename T>
void bindKGrid(py::module &m, const std::string &type);

void bindFoundamentalTypeInterval(py::module &m);
void bindFoundamentalTypeKGrid(py::module &m);


// ========================= [ IMPLEMENTATION ] ==========================================
template<typename T>
void bindInterval(py::module &m, const std::string &type)
{
  std::string className = type + "Interval";
  py::class_<mt::Interval<T>>(m, className.c_str())
    .def(py::init<T, T>(), py::arg("min") = T(), py::arg("max") = T())
    .def_static("fromMinMax", py::overload_cast<T, T>(&mt::Interval<T>::fromMinMax))
    .def_property("min", py::overload_cast<>(&mt::Interval<T>::min), py::overload_cast<T>(&mt::Interval<T>::min))
    .def_property("max", py::overload_cast<>(&mt::Interval<T>::max), py::overload_cast<T>(&mt::Interval<T>::max))
    .def("__repr__", [](const mt::Interval<T> &i) {
      std::stringstream ss;
      ss << i;
      return ss.str();
    });
}

template<typename T>
void bindKGrid(py::module &m, const std::string &type)
{
  using IntervalType = typename mt::KGrid<T>::IntervalType;

  std::string className = type + "KGrid";
  py::class_<mt::KGrid<T>>(m, className.c_str())
    .def(py::init<>())
    .def(py::init<const mt::Box&, const std::vector<T>&>())
    .def("interval", py::overload_cast<mt::uint32>(&mt::KGrid<T>::interval))
    .def("interval", py::overload_cast<mt::uint32, IntervalType>(&mt::KGrid<T>::interval))
    .def("interval", py::overload_cast<mt::int32, mt::int32>(&mt::KGrid<T>::interval))
    .def("interval", py::overload_cast<mt::int32, mt::int32, IntervalType>(&mt::KGrid<T>::interval))
    .def("interval", py::overload_cast<const mt::I32Point&>(&mt::KGrid<T>::interval))
    .def("interval", py::overload_cast<const mt::I32Point&, IntervalType>(&mt::KGrid<T>::interval))
    .def("emergePoint", py::overload_cast<const mt::I32Point&>(&mt::KGrid<T>::emergePoint, py::const_))
    .def("emergePoint", py::overload_cast<mt::uint32>(&mt::KGrid<T>::emergePoint, py::const_))
    .def("emergeSet", py::overload_cast<const std::vector<mt::I32Point>&>(&mt::KGrid<T>::emergeSet, py::const_))
    .def("emergeSet", py::overload_cast<const std::vector<mt::uint32>&>(&mt::KGrid<T>::emergeSet, py::const_))
    .def_property_readonly("emergeDomain", &mt::KGrid<T>::emergeDomain)
    .def("immersePoint", py::overload_cast<const mt::I32Point&>(&mt::KGrid<T>::immersePoint, py::const_))
    .def("immersePoint", py::overload_cast<mt::uint32>(&mt::KGrid<T>::immersePoint, py::const_))
    .def("immerseSet", py::overload_cast<const std::vector<mt::I32Point>&>(&mt::KGrid<T>::immerseSet, py::const_))
    .def("immerseSet", py::overload_cast<const std::vector<mt::uint32>&>(&mt::KGrid<T>::immerseSet, py::const_))
    .def_property_readonly("immerseDomain", &mt::KGrid<T>::immerseDomain)
    .def("isZeroFace", &mt::KGrid<T>::isZeroFace)
    .def("isOneFace", &mt::KGrid<T>::isOneFace)
    .def("isTwoFace", &mt::KGrid<T>::isTwoFace)
    .def("adj", py::overload_cast<>(&mt::KGrid<T>::adj))
    .def("__repr__", [](const mt::KGrid<T> &F) {
      std::stringstream ss;
      ss << F;
      return ss.str();
    });
}