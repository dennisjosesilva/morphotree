#include "attributes/boundingBoxComputerpy.hpp"

void bindFoundamentalTypesBoundingBoxComputer(py::module &m)
{
  bindBoundingBoxComputer<mt::uint8>(m, "UI8");
  bindBoundingBoxComputer<mt::int8>(m, "I8");
  bindBoundingBoxComputer<mt::uint32>(m, "UI32");
  bindBoundingBoxComputer<mt::int32>(m, "I32");
}