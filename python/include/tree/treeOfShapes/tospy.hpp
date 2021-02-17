#pragma once

#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

#include "morphotree/tree/treeOfShapes/tos.hpp"
#include "core/opaque_types.hpp"

namespace py = pybind11;
namespace mt = morphotree;

template<typename T>
void bindBuildOrderImageMaxTree(py::module &m, const std::string &type);

template<typename T>
void bindBuildEnlargedTreeOfShapes(py::module &m, const std::string &type);

template<typename T>
void bindBuildTreeOfShapes(py::module &m, const std::string &type);

template<typename T>
void bindEmergeTreeOfShapes(py::module &m, const std::string &type);

void bindFoundamentalTypeBuildOrderImageMaxTree(py::module &m);
void bindFoundamentalTypeBuildEnlargedTreeOfShapes(py::module &m);
void bindFoundamentalTypeBuildTreeOfShapes(py::module &m);
void bindFoundamentalTypeEmergeTreeOfShapes(py::module &m);

// =========================== [ IMPLEMENTATION ] =========================================
template<typename T>
void bindBuildOrderImageMaxTree(py::module &m, const std::string &type)
{
  std::string functionName = type + "BuildImageMaxTree";
  m.def(functionName.c_str(), 
    py::overload_cast<const mt::Box&, const std::vector<T>&, const mt::KGrid<T>&, mt::I32Point>(&mt::buildOrderImageMaxtree<T>), 
    py::arg("domain"), py::arg("f"), py::arg("kgrid"), py::arg("pInfty") = mt::I32Point{0,0});
  // m.def(functionName.c_str(),                                                   buildOrderImageMaxtree
  //   py::overload_cast<const mt::Box&, const std::vector<T>&, mt::I32Point>(&mt::buildOrderImageMaxtree<T>),
  //   py::arg("domain"), py::arg("f"), py::arg("pInfty") = mt::I32Point{0,0});
}

template<typename T>
void bindBuildEnlargedTreeOfShapes(py::module &m, const std::string &type)
{
  std::string functionName = type + "BuildEnlargedTreeOfShapes";
  m.def(functionName.c_str(), 
    py::overload_cast<const mt::Box&, const std::vector<T>&, const mt::KGrid<T>&, mt::I32Point>(&mt::buildEnlargedTreeOfShapes<T>),
    py::arg("domain"), py::arg("f"), py::arg("kgrid"), py::arg("pInfty") = mt::I32Point{0,0});
  m.def(functionName.c_str(), 
    py::overload_cast<const mt::Box&, const std::vector<T>&, mt::I32Point>(&mt::buildEnlargedTreeOfShapes<T>),
    py::arg("domain"), py::arg("f"), py::arg("pInfty") = mt::I32Point{0,0});
}

template<typename T>
void bindBuildTreeOfShapes(py::module &m, const std::string &type)
{
  std::string functionName = type + "BuildTreeOfShapes";
  m.def(functionName.c_str(),
    py::overload_cast<const mt::Box&, const std::vector<T>&, const mt::KGrid<T>&, mt::I32Point>(&mt::buildTreeOfShapes<T>),
    py::arg("domain"), py::arg("f"), py::arg("kgrid"), py::arg("pInfty") = mt::I32Point{0,0});
  m.def(functionName.c_str(),
    py::overload_cast<const mt::Box&, const std::vector<T>&, mt::I32Point>(&mt::buildTreeOfShapes<T>),
    py::arg("domain"), py::arg("f"), py::arg("pInfty") = mt::I32Point{0,0});
}


template<typename T>
void bindEmergeTreeOfShapes(py::module &m, const std::string &type)
{
  std::string functionName = type + "EmergeTreeOfShapes";
  m.def(functionName.c_str(), &mt::emergeTreeOfShapes<T>);
}