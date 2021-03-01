#include "attributes/bitquads/quadCountTreeOfShapesComputerpy.hpp"

void bindWindowMaxTreeNode(py::module &m)
{
  py::class_<mt::WindowMaxTreeNode>(m, "WindowMaxTreeNode")
    .def(py::init<mt::uint8, mt::uint8, mt::uint8, mt::uint32>())
    .def_property("id", 
      py::overload_cast<>(&mt::WindowMaxTreeNode::id), 
      py::overload_cast<mt::uint8>(&mt::WindowMaxTreeNode::id))
    .def_property("parent", 
      py::overload_cast<>(&mt::WindowMaxTreeNode::parent), 
      py::overload_cast<mt::uint8>(&mt::WindowMaxTreeNode::parent))
    .def_property("size", 
      py::overload_cast<>(&mt::WindowMaxTreeNode::size), 
      py::overload_cast<mt::uint8>(&mt::WindowMaxTreeNode::size))
    .def_property("representative", 
      py::overload_cast<>(&mt::WindowMaxTreeNode::representative),
      py::overload_cast<mt::uint32>(&mt::WindowMaxTreeNode::representative));
}

void bindWindowMaxTree(py::module &m)
{
  py::class_<mt::WindowMaxTree>(m, "WindowMaxTree")
    .def(py::init<const std::vector<mt::uint32>&, std::shared_ptr<mt::Adjacency>>())
    .def_readonly_static("UndefinedParent", &mt::WindowMaxTree::UndefinedParent)
    .def_readonly_static("UndeginedNodeId", &mt::WindowMaxTree::UndefinedNodeId)
    .def("build", &mt::WindowMaxTree::build)
    .def("tranverse", &mt::WindowMaxTree::tranverse)
    .def("node", &mt::WindowMaxTree::node);
}

void bindFoundamentalTypesTreeOfShapesQuadCountsComputer(py::module &m)
{
  bindTreeOfShapesQuadCountsComputer<mt::uint8>(m, "UI8");
}