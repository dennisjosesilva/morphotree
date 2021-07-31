#include "filtering/progressiveDifferenceAttributeFilterpy.hpp"

void bindFoundamentalTypesProgressiveDifferenceFilter(py::module &m)
{
  // progressive difference filter
  bindProgressiveDifferenceFilter<mt::uint8, mt::uint8>(m, "UI8", "UI8");
  bindProgressiveDifferenceFilter<mt::uint8, mt::uint32>(m, "UI8", "UI32");
  bindProgressiveDifferenceFilter<mt::uint8, mt::int8>(m, "UI8", "I8");
  bindProgressiveDifferenceFilter<mt::uint8, mt::int32>(m, "UI8", "I32");
  bindProgressiveDifferenceFilter<mt::uint8, float>(m, "UI8", "F");
  bindProgressiveDifferenceFilter<mt::uint8, double>(m, "UI8", "D");

  bindProgressiveDifferenceFilter<mt::uint32, mt::uint8>(m, "UI32", "UI8");
  bindProgressiveDifferenceFilter<mt::uint32, mt::uint32>(m, "UI32", "UI32");
  bindProgressiveDifferenceFilter<mt::uint32, mt::int8>(m, "UI32", "I8");
  bindProgressiveDifferenceFilter<mt::uint32, mt::int32>(m, "UI32", "I32");
  bindProgressiveDifferenceFilter<mt::uint32, float>(m, "UI32", "F");
  bindProgressiveDifferenceFilter<mt::uint32, double>(m, "UI32", "D");

  bindProgressiveDifferenceFilter<mt::int8, mt::uint8>(m, "I8", "UI8");
  bindProgressiveDifferenceFilter<mt::int8, mt::uint32>(m, "I8", "UI32");
  bindProgressiveDifferenceFilter<mt::int8, mt::int8>(m, "I8", "I8");
  bindProgressiveDifferenceFilter<mt::int8, mt::int32>(m, "I8", "I32");
  bindProgressiveDifferenceFilter<mt::int8, float>(m, "I8", "F");
  bindProgressiveDifferenceFilter<mt::int8, double>(m, "I8", "D");

  bindProgressiveDifferenceFilter<mt::int32, mt::uint8>(m, "I32", "UI8");
  bindProgressiveDifferenceFilter<mt::int32, mt::uint32>(m, "I32", "UI32");
  bindProgressiveDifferenceFilter<mt::int32, mt::int8>(m, "I32", "I8");
  bindProgressiveDifferenceFilter<mt::int32, mt::int32>(m, "I32", "I32");
  bindProgressiveDifferenceFilter<mt::int32, float>(m, "I32", "F");
  bindProgressiveDifferenceFilter<mt::int32, double>(m, "I32", "D");
  

  // inplace - progressive difference filter
  bindIProgressiveDifferenceFilter<mt::uint8, mt::uint8>(m, "UI8", "UI8");
  bindIProgressiveDifferenceFilter<mt::uint8, mt::uint32>(m, "UI8", "UI32");
  bindIProgressiveDifferenceFilter<mt::uint8, mt::int8>(m, "UI8", "I8");
  bindIProgressiveDifferenceFilter<mt::uint8, mt::int32>(m, "UI8", "I32");
  bindIProgressiveDifferenceFilter<mt::uint8, float>(m, "UI8", "F");
  bindIProgressiveDifferenceFilter<mt::uint8, double>(m, "UI8", "D");

  bindIProgressiveDifferenceFilter<mt::uint32, mt::uint8>(m, "UI32", "UI8");
  bindIProgressiveDifferenceFilter<mt::uint32, mt::uint32>(m, "UI32", "UI32");
  bindIProgressiveDifferenceFilter<mt::uint32, mt::int8>(m, "UI32", "I8");
  bindIProgressiveDifferenceFilter<mt::uint32, mt::int32>(m, "UI32", "I32");
  bindIProgressiveDifferenceFilter<mt::uint32, float>(m, "UI32", "F");
  bindIProgressiveDifferenceFilter<mt::uint32, double>(m, "UI32", "D");

  bindIProgressiveDifferenceFilter<mt::int8, mt::uint8>(m, "I8", "UI8");
  bindIProgressiveDifferenceFilter<mt::int8, mt::uint32>(m, "I8", "UI32");
  bindIProgressiveDifferenceFilter<mt::int8, mt::int8>(m, "I8", "I8");
  bindIProgressiveDifferenceFilter<mt::int8, mt::int32>(m, "I8", "I32");
  bindIProgressiveDifferenceFilter<mt::int8, float>(m, "I8", "F");
  bindIProgressiveDifferenceFilter<mt::int8, double>(m, "I8", "D");

  bindIProgressiveDifferenceFilter<mt::int32, mt::uint8>(m, "I32", "UI8");
  bindIProgressiveDifferenceFilter<mt::int32, mt::uint32>(m, "I32", "UI32");
  bindIProgressiveDifferenceFilter<mt::int32, mt::int8>(m, "I32", "I8");
  bindIProgressiveDifferenceFilter<mt::int32, mt::int32>(m, "I32", "I32");
  bindIProgressiveDifferenceFilter<mt::int32, float>(m, "I32", "F");
  bindIProgressiveDifferenceFilter<mt::int32, double>(m, "I32", "D");
}