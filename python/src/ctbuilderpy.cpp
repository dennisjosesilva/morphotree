#include "ctbuilderpy.hpp"
#include "morphotree/core/alias.hpp"

void bindFoundamentalTypeCTBuilder(py::module &m)
{
  bindCTBuilder<mt::uint32>(m, "UI32");
  bindCTBuilder<mt::uint8>(m, "UI8");
  bindCTBuilder<mt::int32>(m, "I32");
  bindCTBuilder<mt::int8>(m, "I8");
}