#include "attributes/smoothnessContourComputerpy.hpp"


void bindFoundamentalTypeMaxTreeSmoothnessContourComputer(py::module &m)
{
  bindMaxTreeSmoothnessContourComputer<mt::uint8>(m, "UI8");
  bindMaxTreeSmoothnessContourComputer<mt::int8>(m, "I8");
  bindMaxTreeSmoothnessContourComputer<mt::uint32>(m, "UI32");
  bindMaxTreeSmoothnessContourComputer<mt::int32>(m, "I32");
}

void bindFoundamentalTypeMinTreeSmoothnessContourComputer(py::module &m)
{
  bindMinTreeSmoothnessContourComputer<mt::uint8>(m, "UI8");
  bindMinTreeSmoothnessContourComputer<mt::int8>(m, "I8");
  bindMinTreeSmoothnessContourComputer<mt::uint32>(m, "UI32");
  bindMinTreeSmoothnessContourComputer<mt::int32>(m, "I32");
}

void bindFoundamentalTypeTreeOfShapesSmoothnessContourComputer(py::module &m)
{
  bindTreeOfShapesSmoothnessContourComputer<mt::uint8>(m, "UI8");
}