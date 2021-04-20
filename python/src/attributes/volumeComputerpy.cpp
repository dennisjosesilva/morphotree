#include "attributes/volumeComputerpy.hpp"
     
void bindFoundamentalTypesMaxTreeVolumeComputer(py::module &m)
{
  bindMaxTreeVolumeComputer<mt::uint8>(m, "UI8");
  bindMaxTreeVolumeComputer<mt::int8>(m, "I8");
  bindMaxTreeVolumeComputer<mt::uint32>(m, "UI32");
  bindMaxTreeVolumeComputer<mt::int32>(m, "I32");
}