#include "attributes/differenceAttributeComputerpy.hpp"

void bindFoundamentalTypesDifferenceAttributeComputer(py::module &m)
{
  bindDifferenceAttributeComputer<mt::uint32, mt::uint8>(m, "UI32", "UI8");
  bindDifferenceAttributeComputer<mt::uint32, mt::int8>(m, "UI32", "I8");
  bindDifferenceAttributeComputer<mt::uint32, mt::uint32>(m, "UI32", "UI32");
  bindDifferenceAttributeComputer<mt::uint32, mt::int32>(m, "UI32", "I32");
  
  bindDifferenceAttributeComputer<mt::int32, mt::uint8>(m, "I32", "UI8");
  bindDifferenceAttributeComputer<mt::int32, mt::int8>(m, "I32", "I8");
  bindDifferenceAttributeComputer<mt::int32, mt::uint32>(m, "I32", "UI32");
  bindDifferenceAttributeComputer<mt::int32, mt::int32>(m, "I32", "I32");
  
  bindDifferenceAttributeComputer<float, mt::uint8>(m, "F", "UI8");
  bindDifferenceAttributeComputer<float, mt::int8>(m, "F", "I8");
  bindDifferenceAttributeComputer<float, mt::uint32>(m, "F", "UI32");
  bindDifferenceAttributeComputer<float, mt::int32>(m, "F", "I32");
}