#include "attributes/topologicalHeightComputerpy.hpp"

void bindFoundamentalTypesTopologicalHeightComputer(py::module &m)
{
  bindTopologicalHeightComputer<mt::uint8>(m, "UI8");
  bindTopologicalHeightComputer<mt::int8>(m, "I8");
  bindTopologicalHeightComputer<mt::uint32>(m, "UI32");
  bindTopologicalHeightComputer<mt::int32>(m, "I32");
}