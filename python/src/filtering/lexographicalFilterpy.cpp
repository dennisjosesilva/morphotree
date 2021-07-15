#include "filtering/lexographicalFilterpy.hpp"

void bindFoundamentalTypeFilterTreeLexographically(py::module &m)
{
  bindFilterTreeLexographically<mt::uint8>(m, "UI8");
  bindFilterTreeLexographically<mt::uint32>(m, "UI32");
  bindFilterTreeLexographically<mt::int8>(m, "I8");
  bindFilterTreeLexographically<mt::int32>(m, "I32");

  bindIFilterTreeLexographically<mt::uint8>(m, "UI8");
  bindIFilterTreeLexographically<mt::uint32>(m, "UI32");
  bindIFilterTreeLexographically<mt::int8>(m, "I8");
  bindIFilterTreeLexographically<mt::int32>(m, "I32");
}