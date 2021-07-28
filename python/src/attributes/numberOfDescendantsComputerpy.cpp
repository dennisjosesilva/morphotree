#include "attributes/numberOfDescendantsComputerpy.hpp"

void bindFoundamentalTypesNumberOfDescendantsComputer(py::module &m)
{
  bindNumberOfDescendantsComputer<mt::uint8>(m, "UI8");
  bindNumberOfDescendantsComputer<mt::int8>(m, "I8");
  bindNumberOfDescendantsComputer<mt::uint32>(m, "UI32");
  bindNumberOfDescendantsComputer<mt::int32>(m, "I32");
}