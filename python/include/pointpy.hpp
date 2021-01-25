#pragma once

#include <pybind11/pybind11.h>
#include <pybind11/operators.h>
#include <morphotree/core/point.hpp>
#include <string>
#include <sstream>

namespace py = pybind11;
namespace mt = morphotree;

template<typename T>
void bindPoint(py::module &m, const std::string &type);

void bindFoundamentalTypePoints(py::module &m);

// ======================[ IMPLEMENTATION ] =================================
template<typename T>
void bindPoint(py::module &m, const std::string &type)
{
  std::string className = type + "Point";
  py::class_<mt::Point<T>>(m, className.c_str())
    .def(py::init<T, T>(), py::arg("x") = T(), py::arg("y") = T())
    .def_property("x", py::overload_cast<>(&mt::Point<T>::x), py::overload_cast<T>(&mt::Point<T>::x))
    .def_property("col", py::overload_cast<>(&mt::Point<T>::col), py::overload_cast<T>(&mt::Point<T>::col))
    .def_property("width", py::overload_cast<>(&mt::Point<T>::width), py::overload_cast<T>(&mt::Point<T>::width))
    .def_property("y", py::overload_cast<>(&mt::Point<T>::y), py::overload_cast<T>(&mt::Point<T>::y))
    .def_property("row", py::overload_cast<>(&mt::Point<T>::row), py::overload_cast<T>(&mt::Point<T>::row))
    .def_property("height", py::overload_cast<>(&mt::Point<T>::height), py::overload_cast<T>(&mt::Point<T>::height))
    .def("__getitem__", py::overload_cast<mt::uint32>(&mt::Point<T>::operator[]))
    .def("__setitem__", [](mt::Point<T> &p, mt::uint32 i, T& value){ p[i] = value;})
    .def("add", &mt::Point<T>::add)
    .def("sub", &mt::Point<T>::sub)
    .def("mult", py::overload_cast<float>(&mt::Point<T>::mult, py::const_))
    .def("mult", py::overload_cast<const mt::Point<T>&>(&mt::Point<T>::mult, py::const_))
    .def("div", py::overload_cast<float>(&mt::Point<T>::div, py::const_))
    .def("div", py::overload_cast<const mt::Point<T>&>(&mt::Point<T>::div, py::const_))
    .def("iadd", &mt::Point<T>::iadd)
    .def("isub", &mt::Point<T>::isub)
    .def("imult", py::overload_cast<float>(&mt::Point<T>::imult))
    .def("imult", py::overload_cast<const mt::Point<T>&>(&mt::Point<T>::imult))
    .def("idiv", py::overload_cast<float>(&mt::Point<T>::idiv))
    .def("idiv", py::overload_cast<const mt::Point<T>&>(&mt::Point<T>::idiv))
    .def("equals", &mt::Point<T>::equals)
    .def(py::self += py::self)
    .def(py::self -= py::self)
    .def(py::self *= float())
    .def(py::self *= py::self)
    .def(py::self /= float())
    .def(py::self /= py::self)
    .def(py::self + py::self)
    .def(py::self - py::self)
    .def(py::self * float())
    .def(py::self * py::self)
    .def(py::self / float())
    .def(py::self / py::self)
    .def(py::self == py::self)
    .def(py::self != py::self)
    .def("__repr__", [](const mt::Point<T> &p) { 
      std::stringstream ss;
      ss << p;
      return ss.str();
    });
}

