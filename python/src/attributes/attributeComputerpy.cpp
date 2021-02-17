#include "attributes/attributeComputerpy.hpp"
#include "morphotree/attributes/bitquads/quads.hpp"


void bindFoundamentalTypeAttributeComputer(py::module &m)
{
  bindAttributeComputer<mt::uint32, mt::uint8>(m, "UI32", "UI8");
  bindAttributeComputer<mt::uint32, mt::int8>(m, "UI32", "I8");
  bindAttributeComputer<mt::uint32, mt::uint32>(m, "UI32", "UI32");
  bindAttributeComputer<mt::uint32, mt::int32>(m, "UI32", "I32");

  bindAttributeComputer<mt::int32, mt::uint8>(m, "I32", "UI8");
  bindAttributeComputer<mt::int32, mt::int8>(m, "I32", "I8");
  bindAttributeComputer<mt::int32, mt::uint32>(m, "I32", "UI32");
  bindAttributeComputer<mt::int32, mt::int32>(m, "I32", "I32");

  bindAttributeComputer<float, mt::uint8>(m, "F", "UI8");
  bindAttributeComputer<float, mt::int8>(m, "F", "I8");
  bindAttributeComputer<float, mt::uint32>(m, "F", "UI32");
  bindAttributeComputer<float, mt::int32>(m, "F", "I32");

  bindAttributeComputer<mt::Quads, mt::uint8>(m, "Quads", "UI8");
  bindAttributeComputer<mt::Quads, mt::int8>(m, "Quads", "I8");
  bindAttributeComputer<mt::Quads, mt::uint32>(m, "Quads", "UI32");
  bindAttributeComputer<mt::Quads, mt::int32>(m, "Quads", "I32");
}

void bindFoundamentalTypeAreaComputer(py::module &m)
{
  bindAreaComputer<mt::uint8>(m, "UI8");
  bindAreaComputer<mt::int8>(m, "I8");
  bindAreaComputer<mt::uint32>(m, "UI32");
  bindAreaComputer<mt::int32>(m, "I32");
}

void bindFoundamentalTypeMaxTreePerimeterComputer(py::module &m)
{
  bindMaxTreePerimeterComputer<mt::uint8>(m, "UI8");  
  bindMaxTreePerimeterComputer<mt::int8>(m, "I8");
  bindMaxTreePerimeterComputer<mt::uint32>(m, "UI32");
  bindMaxTreePerimeterComputer<mt::int32>(m, "I32");
}

void bindFoundamentalTypeMinTreePerimeterComputer(py::module &m)
{
  bindMinTreePerimeterComputer<mt::uint8>(m, "UI8");
  bindMinTreePerimeterComputer<mt::int8>(m, "I8");
  bindMinTreePerimeterComputer<mt::uint32>(m, "UI32");
  bindMinTreePerimeterComputer<mt::int32>(m, "I32");
}