#include "filtering/globalOptimiser/globalOptimiserspy.hpp"

void bindFoundamentalTypeMinCPerimeterWithAbsError(py::module &m)
{
  bindMinCPerimeterWithAbsError<mt::uint8>(m, "U8");
  bindMinCPerimeterWithAbsError<mt::int8>(m, "I8");
  bindMinCPerimeterWithAbsError<mt::uint32>(m, "UI32");
  bindMinCPerimeterWithAbsError<mt::int32>(m, "I32");
}

void bindFoundamentalTypeMinCPerimeterWithAbsErrorToS(py::module &m)
{
  bindMinCPerimeterWithAbsErrorToS<mt::uint8>(m, "U8");
}

void bindFoundamentalTypeMinCPerimeterWithSquaredErrorToS(py::module &m)
{
  bindMinCPerimeterWithSquaredErrorToS<mt::uint8>(m, "U8");
}