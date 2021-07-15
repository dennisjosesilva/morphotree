#include "attributes/ExtinctionValuepy.hpp"

void bindFoundamentalTypeExtinctionValueLeavesComputer(py::module &m)
{
  bindExtinctionValueLeavesComputer<mt::uint32, mt::uint8>(m, "UI32", "UI8");
  bindExtinctionValueLeavesComputer<mt::uint32, mt::int8>(m, "UI32", "I8");
  bindExtinctionValueLeavesComputer<mt::uint32, mt::uint32>(m, "UI32", "UI32");
  bindExtinctionValueLeavesComputer<mt::uint32, mt::int32>(m, "UI32", "I32");

  bindExtinctionValueLeavesComputer<mt::int32, mt::uint8>(m, "I32", "UI8");
  bindExtinctionValueLeavesComputer<mt::int32, mt::int8>(m, "I32", "I8");
  bindExtinctionValueLeavesComputer<mt::int32, mt::uint32>(m, "I32", "UI32");
  bindExtinctionValueLeavesComputer<mt::int32, mt::int32>(m, "I32", "I32");

  bindExtinctionValueLeavesComputer<float, mt::uint8>(m, "F", "UI8");
  bindExtinctionValueLeavesComputer<float, mt::int8>(m, "F", "I8");
  bindExtinctionValueLeavesComputer<float, mt::uint32>(m, "F", "UI32");
  bindExtinctionValueLeavesComputer<float, mt::int32>(m, "F", "I32");  
}

void bindFoundamentalTypeExtinctionValueComputer(py::module &m)
{
  bindExtinctionValueComputer<mt::uint32, mt::uint8>(m, "UI32", "UI8");
  bindExtinctionValueComputer<mt::uint32, mt::int8>(m, "UI32", "I8");
  bindExtinctionValueComputer<mt::uint32, mt::uint32>(m, "UI32", "UI32");
  bindExtinctionValueComputer<mt::uint32, mt::int32>(m, "UI32", "I32");

  bindExtinctionValueComputer<mt::int32, mt::uint8>(m, "I32", "UI8");
  bindExtinctionValueComputer<mt::int32, mt::int8>(m, "I32", "I8");
  bindExtinctionValueComputer<mt::int32, mt::uint32>(m, "I32", "UI32");
  bindExtinctionValueComputer<mt::int32, mt::int32>(m, "I32", "I32");

  bindExtinctionValueComputer<float, mt::uint8>(m, "F", "UI8");
  bindExtinctionValueComputer<float, mt::int8>(m, "F", "I8");
  bindExtinctionValueComputer<float, mt::uint32>(m, "F", "UI32");
  bindExtinctionValueComputer<float, mt::int32>(m, "F", "I32");
}