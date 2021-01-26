#include "tree/treeOfShapes/tospy.hpp"

void bindFoundamentalTypeBuildOrderImageMaxTree(py::module &m)
{
  bindBuildOrderImageMaxTree<mt::uint8>(m, "UI8");
}

void bindFoundamentalTypeBuildEnlargedTreeOfShapes(py::module &m)
{
  bindBuildEnlargedTreeOfShapes<mt::uint8>(m, "UI8");
}

void bindFoundamentalTypeBuildTreeOfShapes(py::module &m)
{
  bindBuildTreeOfShapes<mt::uint8>(m, "UI8");
}

void bindFoundamentalTypeEmergeTreeOfShapes(py::module &m)
{
  bindEmergeTreeOfShapes<mt::uint8>(m, "UI8");
}