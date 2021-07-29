#include "filtering/extinctionFilterpy.hpp"

void bindFoundamentalTypesExtinctionFilter(py::module &m)
{
  // extinction filter
  bindExtinctionFilter<mt::uint8, mt::uint8>(m, "UI8", "UI8");
  bindExtinctionFilter<mt::uint8, mt::uint32>(m, "UI8", "UI32");
  bindExtinctionFilter<mt::uint8, mt::int8>(m, "UI8", "I8");
  bindExtinctionFilter<mt::uint8, mt::int32>(m, "UI8", "I32");
  bindExtinctionFilter<mt::uint8, float>(m, "UI8", "F");
  bindExtinctionFilter<mt::uint8, double>(m, "UI8", "D");

  bindExtinctionFilter<mt::uint32, mt::uint8>(m, "UI32", "UI8");
  bindExtinctionFilter<mt::uint32, mt::uint32>(m, "UI32", "UI32");
  bindExtinctionFilter<mt::uint32, mt::int8>(m, "UI32", "I8");
  bindExtinctionFilter<mt::uint32, mt::int32>(m, "UI32", "I32");
  bindExtinctionFilter<mt::uint32, float>(m, "UI32", "F");
  bindExtinctionFilter<mt::uint32, double>(m, "UI32", "D");

  bindExtinctionFilter<mt::int8, mt::uint8>(m, "I8", "UI8");
  bindExtinctionFilter<mt::int8, mt::uint32>(m, "I8", "UI32");
  bindExtinctionFilter<mt::int8, mt::int8>(m, "I8", "I8");
  bindExtinctionFilter<mt::int8, mt::int32>(m, "I8", "I32");
  bindExtinctionFilter<mt::int8, float>(m, "I8", "F");
  bindExtinctionFilter<mt::int8, double>(m, "I8", "D");

  bindExtinctionFilter<mt::int32, mt::uint8>(m, "I32", "UI8");
  bindExtinctionFilter<mt::int32, mt::uint32>(m, "I32", "UI32");
  bindExtinctionFilter<mt::int32, mt::int8>(m, "I32", "I8");
  bindExtinctionFilter<mt::int32, mt::int32>(m, "I32", "I32");
  bindExtinctionFilter<mt::int32, float>(m, "I32", "F");
  bindExtinctionFilter<mt::int32, double>(m, "I32", "D");

  // inplace extinction filter
  bindIExtinctionFilter<mt::uint8, mt::uint8>(m, "UI8", "UI8");
  bindIExtinctionFilter<mt::uint8, mt::uint32>(m, "UI8", "UI32");
  bindIExtinctionFilter<mt::uint8, mt::int8>(m, "UI8", "I8");
  bindIExtinctionFilter<mt::uint8, mt::int32>(m, "UI8", "I32");
  bindIExtinctionFilter<mt::uint8, float>(m, "UI8", "F");
  bindIExtinctionFilter<mt::uint8, double>(m, "UI8", "D");

  bindIExtinctionFilter<mt::uint32, mt::uint8>(m, "UI32", "UI8");
  bindIExtinctionFilter<mt::uint32, mt::uint32>(m, "UI32", "UI32");
  bindIExtinctionFilter<mt::uint32, mt::int8>(m, "UI32", "I8");
  bindIExtinctionFilter<mt::uint32, mt::int32>(m, "UI32", "I32");
  bindIExtinctionFilter<mt::uint32, float>(m, "UI32", "F");
  bindIExtinctionFilter<mt::uint32, double>(m, "UI32", "D");

  bindIExtinctionFilter<mt::int8, mt::uint8>(m, "I8", "UI8");
  bindIExtinctionFilter<mt::int8, mt::uint32>(m, "I8", "UI32");
  bindIExtinctionFilter<mt::int8, mt::int8>(m, "I8", "I8");
  bindIExtinctionFilter<mt::int8, mt::int32>(m, "I8", "I32");
  bindIExtinctionFilter<mt::int8, float>(m, "I8", "F");
  bindIExtinctionFilter<mt::int8, double>(m, "I8", "D");

  bindIExtinctionFilter<mt::int32, mt::uint8>(m, "I32", "UI8");
  bindIExtinctionFilter<mt::int32, mt::uint32>(m, "I32", "UI32");
  bindIExtinctionFilter<mt::int32, mt::int8>(m, "I32", "I8");
  bindIExtinctionFilter<mt::int32, mt::int32>(m, "I32", "I32");
  bindIExtinctionFilter<mt::int32, float>(m, "I32", "F");
  bindIExtinctionFilter<mt::int32, double>(m, "I32", "D");
}