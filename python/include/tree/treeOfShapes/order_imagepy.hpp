#pragma once

#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

#include "morphotree/tree/treeOfShapes/order_image.hpp"
#include "core/opaque_types.hpp"

#include <string>
#include <sstream>

namespace py = pybind11;
namespace mt = morphotree;

template<typename T>
void bindOrderImageResult(py::module &m, const std::string &type);

template<typename T>
void bindComputeOrderImage(py::module &m, const std::string &type);

void bindFoundamentalTypeOrderImageResult(py::module &m);
void bindFoundamentalTypeComputeOrderImage(py::module &m);

// =================== [ IMPLEMENTATION ] ====================================================
template<typename T>
void bindOrderImageResult(py::module &m, const std::string &type)
{
  std::string className = type + "OrderImageResult";
  py::class_<mt::OrderImageResult<T>>(m, className.c_str())
    .def(py::init<>())
    .def(py::init<std::vector<mt::uint32>, std::vector<T>, std::vector<mt::uint32>, mt::Box>(),
      py::arg("orderImg"), py::arg("flattern"), py::arg("R"), py::arg("domain"))
    .def_property("orderImg", 
      [](mt::OrderImageResult<T> &res){ return res.orderImg; },
      [](mt::OrderImageResult<T> &res, std::vector<mt::uint32> v) { res.orderImg = v; })
    .def_property("R",
      [](mt::OrderImageResult<T> &res) { return res.R; },
      [](mt::OrderImageResult<T> &res, std::vector<mt::uint32> R) { res.R = R; })
    .def_property("flattern",
      [](mt::OrderImageResult<T> &res) { return res.flattern; },
      [](mt::OrderImageResult<T> &res, std::vector<T> flattern) { res.flattern = flattern; })
    .def_property("domain",
      [](mt::OrderImageResult<T> &res) { return res.domain; },
      [](mt::OrderImageResult<T> &res, mt::Box domain) { res.domain = domain; });
}

template<typename T>
void bindComputeOrderImage(py::module &m, const std::string &type)
{
  m.def("computeOrderImage", &mt::computeOrderImage<T>,
    py::arg("domain"), py::arg("f"), py::arg("F"), py::arg("pInfinity") = mt::I32Point{0,0});
}