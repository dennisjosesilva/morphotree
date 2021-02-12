#include "mtreepy.hpp"

void bindMorphoTreeType(py::module &m)
{
  py::enum_<mt::MorphoTreeType>(m, "MorphoTreeType")
    .value("MaxTree", mt::MorphoTreeType::MaxTree)
    .value("MinTree", mt::MorphoTreeType::MinTree)
    .value("TreeOfShapes", mt::MorphoTreeType::TreeOfShapes);
}


void bindFoundamentalTypeMTNode(py::module &m)
{
  bindMTNode<mt::uint8>(m, "UI8");
  bindMTNode<mt::uint32>(m, "UI32");
  bindMTNode<mt::int8>(m, "I8");
  bindMTNode<mt::int32>(m, "I32");
}

void bindFoundamentalTypeMorphologicalTree(py::module &m)
{
  bindMorphologicalTree<mt::uint8>(m, "UI8");
  bindMorphologicalTree<mt::uint32>(m, "UI32");
  bindMorphologicalTree<mt::int8>(m, "I8");
  bindMorphologicalTree<mt::int32>(m, "I32");
}