#include "core/hqueuepy.hpp"

void bindFoundamentalTypeKeyValue(py::module &m)
{
  // bindKeyValue<mt::uint32, mt::uint32>(m, "UI32", "UI32");
  // bindKeyValue<mt::uint32, mt::uint8>(m, "UI32", "UI8");
  // bindKeyValue<mt::uint32, mt::int32>(m, "UI32", "I32");
  // bindKeyValue<mt::uint32, mt::int8>(m, "UI32", "I8");

  bindKeyValue<mt::uint8, mt::uint32>(m, "UI8", "UI32");
  // bindKeyValue<mt::uint8, mt::uint8>(m, "UI8", "UI8");
  // bindKeyValue<mt::uint8, mt::int32>(m, "UI8", "I32");
  // bindKeyValue<mt::uint8, mt::int8>(m, "UI8", "I8");

  // bindKeyValue<mt::int32, mt::uint32>(m, "I32", "UI32");
  // bindKeyValue<mt::int32, mt::uint8>(m, "I32", "UI8");
  // bindKeyValue<mt::int32, mt::int32>(m, "I32", "I32");
  // bindKeyValue<mt::int32, mt::int8>(m, "I32", "I8");

  // bindKeyValue<mt::int8, mt::uint32>(m, "I8", "UI32");
  // bindKeyValue<mt::int8, mt::uint8>(m, "I8", "UI8");
  // bindKeyValue<mt::int8, mt::int32>(m, "I8", "I32");
  // bindKeyValue<mt::int8, mt::int8>(m, "I8", "I8");
}

void bindFoundamentalTypeHQueue(py::module &m)
{
  // bindHQueue<mt::uint32, mt::uint32>(m, "UI32", "UI32");
  // bindHQueue<mt::uint32, mt::uint8>(m, "UI32", "UI8");
  // bindHQueue<mt::uint32, mt::int32>(m, "UI32", "I32");
  // bindHQueue<mt::uint32, mt::int8>(m, "UI32", "I8");

  bindHQueue<mt::uint8, mt::uint32>(m, "UI8", "UI32");
  // bindHQueue<mt::uint8, mt::uint8>(m, "UI8", "UI8");
  // bindHQueue<mt::uint8, mt::int32>(m, "UI8", "I32");
  // bindHQueue<mt::uint8, mt::int8>(m, "UI8", "I8");

  // bindHQueue<mt::int32, mt::uint32>(m, "I32", "UI32");
  // bindHQueue<mt::int32, mt::uint8>(m, "I32", "UI8");
  // bindHQueue<mt::int32, mt::int32>(m, "I32", "I32");
  // bindHQueue<mt::int32, mt::int8>(m, "I32", "I8");

  // bindHQueue<mt::int8, mt::uint32>(m, "I8", "UI32");
  // bindHQueue<mt::int8, mt::uint8>(m, "I8", "UI8");
  // bindHQueue<mt::int8, mt::int32>(m, "I8", "I32");
  // bindHQueue<mt::int8, mt::int8>(m, "I8", "I8");
}