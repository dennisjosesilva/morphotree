#include "tree/treeOfShapes/order_imagepy.hpp"
#include "morphotree/core/alias.hpp"

namespace mt = morphotree;

void bindFoundamentalTypeOrderImageResult(py::module &m)
{
  //bindOrderImageResult<mt::uint32>(m, "UI32");
  bindOrderImageResult<mt::uint8>(m, "UI8");
  // bindOrderImageResult<mt::int32>(m, "I32");
  // bindOrderImageResult<mt::int8>(m, "I8");
}

void bindFoundamentalTypeComputeOrderImage(py::module &m)
{
  //bindComputeOrderImage<mt::uint32>(m, "UI32");
  bindComputeOrderImage<mt::uint8>(m, "UI8");
  // bindComputeOrderImage<mt::int32>(m, "I32");
  // bindComputeOrderImage<mt::int8>(m, "I8");
}