#include <morphotree/core/alias.hpp>
#include "pointpy.hpp"

namespace mt = morphotree;

void bindFoundamentalTypePoints(py::module &m)
{
  bindPoint<mt::uint32>(m, "UI32");
  bindPoint<mt::uint8>(m, "UI8");
  bindPoint<mt::int32>(m, "I32");
  bindPoint<mt::int8>(m, "I8");
}