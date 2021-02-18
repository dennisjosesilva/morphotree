#include "attributes/bitquads/quadCountComputerpy.hpp"

void bindQuads(py::module &m) 
{
  py::class_<mt::Quads>(m, "Quads")
    .def(py::init<>())
    .def_property("q1", py::overload_cast<>(&mt::Quads::q1), py::overload_cast<mt::uint32>(&mt::Quads::q1))
    .def_property("q2", py::overload_cast<>(&mt::Quads::q2), py::overload_cast<mt::uint32>(&mt::Quads::q2))
    .def_property("q3", py::overload_cast<>(&mt::Quads::q3), py::overload_cast<mt::uint32>(&mt::Quads::q3))
    .def_property("qd", py::overload_cast<>(&mt::Quads::qd), py::overload_cast<mt::uint32>(&mt::Quads::qd))
    .def_property("q4", py::overload_cast<>(&mt::Quads::q4), py::overload_cast<mt::uint32>(&mt::Quads::q4))
    .def("incQ1", &mt::Quads::incQ1)
    .def("incQ2", &mt::Quads::incQ2)
    .def("incQ3", &mt::Quads::incQ3)
    .def("incQD", &mt::Quads::incQD)
    .def("incQ4", &mt::Quads::incQ4)
    .def("decQ1", &mt::Quads::decQ1)
    .def("decQ2", &mt::Quads::decQ2)
    .def("decQ3", &mt::Quads::decQ3)
    .def("decQD", &mt::Quads::decQD)
    .def("decQ4", &mt::Quads::decQ4)
    .def("area", &mt::Quads::area)
    .def("perimeter", &mt::Quads::perimeter)
    .def("continuousArea", &mt::Quads::continuousArea)
    .def("continuousPerimeter", &mt::Quads::continuousPerimeter)
    .def("eulerNumber4C", &mt::Quads::eulerNumber4C)
    .def("eulerNumber8C", &mt::Quads::eulerNumber8C)
    .def("numberOfHoles4C", &mt::Quads::numberOfHoles4C)
    .def("numberOfHoles8C", &mt::Quads::numberOfHoles8C);
}

void bindQuadsVector(py::module &m)
{
  py::bind_vector<std::vector<mt::Quads>>(m, "QuadsVector");
}

void bindFoundamentalTypesCTreeQuadCountsComputer(py::module &m)
{
  bindCTreeQuadCountsComputer<mt::uint8>(m, "UI8");
  bindCTreeQuadCountsComputer<mt::uint32>(m, "UI32");
  bindCTreeQuadCountsComputer<mt::int8>(m, "I8");
  bindCTreeQuadCountsComputer<mt::int32>(m, "I32");
}