#include "tree/treeOfShapes/kgridpy.hpp"

void bindFoundamentalTypeInterval(py::module &m)
{
  //bindInterval<mt::uint32>(m, "UI32");
  bindInterval<mt::uint8>(m, "IU8");
  // bindInterval<mt::int32>(m, "I32");
  // bindInterval<mt::int8>(m, "I8");
}

void bindFoundamentalTypeKGrid(py::module &m)
{
  //bindKGrid<mt::uint32>(m, "UI32");
  bindKGrid<mt::uint8>(m, "UI8");
  // bindKGrid<mt::int32>(m, "I32");
  // bindKGrid<mt::int8>(m, "I8");
}