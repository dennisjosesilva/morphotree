#pragma once

#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <pybind11/functional.h>
#include <pybind11/numpy.h>
#include "core/opaque_types.hpp"

#include "morphotree/tree/mtree.hpp"

namespace py = pybind11;
namespace mt = morphotree;

template<typename T>
void bindMTNode(py::module &m, const std::string& type);

template<typename T>
void bindMorphologicalTree(py::module &m, const std::string& type);

template<typename T>
py::array MTNodeReconstructNumpy(mt::MTNode<T> *node);

template<typename T>
py::array MTNodeReconstructAsImageNumpy(mt::MTNode<T> *node, const mt::Box &domain);

template<typename T>
py::array MorphologicalTreeReconstructNode(mt::MorphologicalTree<T> *tree, mt::uint32 nodeId);

template<typename T>
py::array MorphologicalTreeReconstructNodeAsImage(mt::MorphologicalTree<T> *tree, 
  mt::uint32 nodeId, const mt::Box &domain);

template<typename T>
py::array MorphologicalTreeReconstructImage(mt::MorphologicalTree<T> *tree, 
  const mt::Box &domain);

void bindMorphoTreeType(py::module &m);

void bindFoundamentalTypeMTNode(py::module &m);

void bindFoundamentalTypeMorphologicalTree(py::module &m);

// =========================== [ MY IMPLEMENTATION ] ================================
template<typename T>
py::array MTNodeReconstructNumpy(mt::MTNode<T> *node)
{
  std::vector<mt::uint32> rec = node->reconstruct();
  return py::cast(rec);
}

template<typename T>
py::array MTNodeReconstructAsImageNumpy(mt::MTNode<T> *node, const mt::Box &domain)
{
  std::vector<bool> rec = node->reconstruct(domain);
  py::array recnp = py::cast(rec);
  recnp.resize({domain.height(), domain.width()});
  return recnp;
}

template<typename T>
py::array MorphologicalTreeReconstructNode(mt::MorphologicalTree<T> *tree, mt::uint32 nodeId)
{
  std::vector<mt::uint32> v = tree->reconstructNode(nodeId);
  return py::cast(v);
}

template<typename T>
py::array MorphologicalTreeReconstructNodeAsImage(mt::MorphologicalTree<T> *tree, 
  mt::uint32 nodeId, const mt::Box &domain)
{
  std::vector<bool> rec = tree->reconstructNode(nodeId, domain);
  py::array recnp = py::cast(rec);
  recnp.resize({domain.height(), domain.width()});
  return recnp;
}

template<typename T>
py::array MorphologicalTreeReconstructImage(mt::MorphologicalTree<T> *tree, 
  const mt::Box &domain)
{
  std::vector<T> rec = tree->reconstructImage();
  py::array recnp = py::cast(rec);
  recnp.resize({domain.height(), domain.width()});
  return recnp;
}

template<typename T>
void bindMTNode(py::module &m, const std::string &type)
{
  std::string className = type + "MTNode";
  py::class_<mt::MTNode<T>, std::shared_ptr<mt::MTNode<T>>>(m, className.c_str())
    .def(py::init<mt::uint32>(), py::arg("id") = 0)
    .def_property("id", py::overload_cast<>(&mt::MTNode<T>::id), py::overload_cast<mt::uint32>(&mt::MTNode<T>::id))
    .def_property("level", py::overload_cast<>(&mt::MTNode<T>::level), py::overload_cast<T>(&mt::MTNode<T>::level))
    .def_property_readonly("cnps", &mt::MTNode<T>::cnps)
    .def("appendCNP", &mt::MTNode<T>::appendCNP)
    .def("includeCNPS", &mt::MTNode<T>::includeCNPS)
    .def_property("parent", py::overload_cast<>(&mt::MTNode<T>::parent), 
      py::overload_cast<typename mt::MTNode<T>::NodePtr>(&mt::MTNode<T>::parent))
    .def("appendChild", &mt::MTNode<T>::appendChild)
    .def("includeChildren", &mt::MTNode<T>::includeChildren)
    .def("removeChild", &mt::MTNode<T>::removeChild)
    .def_property_readonly("children", &mt::MTNode<T>::children)
    .def("reconstruct", py::overload_cast<>(&mt::MTNode<T>::reconstruct, py::const_))
    .def("reconstruct", py::overload_cast<const mt::Box&>(&mt::MTNode<T>::reconstruct, py::const_))
    .def("reconstructNumpy", &MTNodeReconstructNumpy<T>)
    .def("reconstructNumpy", &MTNodeReconstructAsImageNumpy<T>)
    .def("copy", &mt::MTNode<T>::copy);
}

template<typename T>
void bindMorphologicalTree(py::module &m, const std::string& type)
{
  std::string className = type + "MorphologicalTree";
  py::class_<mt::MorphologicalTree<T>>(m, className.c_str())
    .def(py::init<mt::MorphoTreeType>())
    .def(py::init<mt::MorphoTreeType, const std::vector<T>&, const mt::CTBuilderResult&>())
    .def("node", py::overload_cast<mt::uint32>(&mt::MorphologicalTree<T>::node))
    .def_property_readonly("root", py::overload_cast<>(&mt::MorphologicalTree<T>::root, py::const_))
    .def("reconstructNode", py::overload_cast<mt::uint32>(&mt::MorphologicalTree<T>::reconstructNode, py::const_))
    .def("reconstructNode", py::overload_cast<mt::uint32, const mt::Box&>(&mt::MorphologicalTree<T>::reconstructNode, py::const_))
    .def("reconstructNodeNumpy", &MorphologicalTreeReconstructNode<T>)
    .def("reconstructNodeNumpy", &MorphologicalTreeReconstructNodeAsImage<T>)
    .def_property_readonly("numberOfNodes", &mt::MorphologicalTree<T>::numberOfNodes)
    .def("tranverse", &mt::MorphologicalTree<T>::tranverse)
    .def("reconstructImage", &mt::MorphologicalTree<T>::reconstructImage)
    .def("reconstructImageNumpy", &MorphologicalTreeReconstructImage<T>)
    .def("idirectFilter", &mt::MorphologicalTree<T>::idirectFilter)
    .def("directFilter", &mt::MorphologicalTree<T>::directFilter)
    .def("tranverseByLevel", py::overload_cast<std::function<void(typename mt::MorphologicalTree<T>::NodePtr)>>(&mt::MorphologicalTree<T>::traverseByLevel))
    .def("smallComponent", &mt::MorphologicalTree<T>::smallComponent)
    .def("copy", &mt::MorphologicalTree<T>::copy)
    .def_property_readonly("type", &mt::MorphologicalTree<T>::type);

  std::string maxtreeName = type + "buildMaxTree";
  m.def(maxtreeName.c_str(), &mt::buildMaxTree<T>);

  std::string mintreeName = type + "buildMinTree";
  m.def(mintreeName.c_str(), &mt::buildMinTree<T>);
}