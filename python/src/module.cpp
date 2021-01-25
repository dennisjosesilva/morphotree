#include <pybind11/pybind11.h>
#include <pybind11/operators.h>
#include <pybind11/stl.h>
#include <pybind11/functional.h>

#include <sstream>

#include "pointpy.hpp"
#include "ctbuilderpy.hpp"
#include "mtreepy.hpp"

#include "morphotree/core/point.hpp"
#include "morphotree/core/box.hpp"
#include "morphotree/core/sort.hpp"
#include "morphotree/adjacency/adjacency.hpp"
#include "morphotree/adjacency/adjacency4c.hpp"
#include "morphotree/adjacency/adjacency8c.hpp"
#include "morphotree/tree/ct_builder.hpp"
#include "morphotree/tree/mtree.hpp"

#include "core/hqueuepy.hpp"
#include "tree/treeOfShapes/kgridpy.hpp"

namespace py = pybind11;
namespace mt = morphotree;

PYBIND11_MODULE(morphotreepy, m) 
{
  m.doc() = "Simple prototyping morphological tree library.";

  bindFoundamentalTypePoints(m);

  py::class_<mt::ForwardBoxScan>(m, "ForwardBoxScan")
    .def(py::init<mt::Box*>())
    .def_property_readonly("current", &mt::ForwardBoxScan::next)
    .def("next", &mt::ForwardBoxScan::next)
    .def_property_readonly("hasFinished", &mt::ForwardBoxScan::hasFinished);

  py::class_<mt::BackwardBoxScan>(m, "BackwardBoxScan")
    .def(py::init<mt::Box*>())
    .def_property_readonly("current", &mt::BackwardBoxScan::next)
    .def("next", &mt::BackwardBoxScan::next)
    .def_property_readonly("hasFinished", &mt::BackwardBoxScan::hasFinished);
  
  py::class_<mt::Box>(m, "Box")
    .def(py::init<mt::I32Point, mt::I32Point>(), py::arg("topleft"), py::arg("bottomright"))
    .def_static("fromCorners", &mt::Box::fromCorners, py::arg("topleft"), py::arg("bottomright"))
    .def_static("fromSize", py::overload_cast<mt::I32Point, mt::UI32Point>(&mt::Box::fromSize), py::arg("topleft"), py::arg("size"))
    .def_static("fromSize", py::overload_cast<mt::UI32Point>(&mt::Box::fromSize), py::arg("size"))
    .def_property_readonly("top", &mt::Box::top)
    .def_property_readonly("left", &mt::Box::left)
    .def_property_readonly("bottom", &mt::Box::bottom)
    .def_property_readonly("right", &mt::Box::right)
    .def_property_readonly("topleft", &mt::Box::topleft)
    .def_property_readonly("bottomright", &mt::Box::bottomright)
    .def_property_readonly("size", &mt::Box::size)
    .def_property_readonly("width", &mt::Box::width)
    .def_property_readonly("height", &mt::Box::height)
    .def_property_readonly("numberOfPoints", &mt::Box::numberOfPoints)
    .def("contains", &mt::Box::contains)
    .def("pointToIndex", py::overload_cast<const mt::I32Point&>(&mt::Box::pointToIndex, py::const_))
    .def("pointToIndex", py::overload_cast<mt::int32, mt::int32>(&mt::Box::pointToIndex, py::const_))
    .def("indexToPoint", &mt::Box::indexToPoint)
    .def("at", &mt::Box::at)
    .def("__getitem__", py::overload_cast<mt::uint32>(&mt::Box::operator[], py::const_))
    .def("forwardBoxScan", &mt::Box::forwardBoxScan)
    .def("backwardBoxScan", &mt::Box::backwardBoxScan)
    .def("__repr__", [](const mt::Box &b) { 
      std::stringstream ss;
      ss << b;
      return ss.str();
     });

  m.def("UI32STLsortIndex", &mt::STLsortIndex<mt::uint32>)
    .def("UI8STLsortIndex", &mt::STLsortIndex<mt::uint8>)
    .def("I32STLsortIndex", &mt::STLsortIndex<mt::int32>)
    .def("I8STLsortIndex", &mt::STLsortIndex<mt::int8>);

  m.def("UI32countingSortIncreasing", &mt::countingSortIncreasing<mt::uint32>)
    .def("UI8countingSortIncreasing", &mt::countingSortIncreasing<mt::uint8>)
    .def("I32countingSortIncreasing", &mt::countingSortIncreasing<mt::int32>)
    .def("I8countingSortIncreasing", &mt::countingSortIncreasing<mt::int8>);

  m.def("UI32countingSortDecreasing", &mt::countingSortDecreasing<mt::uint32>)
    .def("UI8countingSortDecreasing", &mt::countingSortDecreasing<mt::uint8>)
    .def("I32countingSortDecreasing", &mt::countingSortDecreasing<mt::int32>)
    .def("I8countingSortDecreasing", &mt::countingSortDecreasing<mt::int8>); 

  m.def("UI32sortIncreasing", &mt::sortIncreasing<mt::uint32>)
    .def("UI8sortIncreasing", &mt::sortIncreasing<mt::uint8>)
    .def("I32sortIncreasing", &mt::sortIncreasing<mt::int32>)
    .def("I8sortIncreasing", &mt::sortIncreasing<mt::int8>); 
 
  m.def("UI32sortDecreasing", &mt::sortDecreasing<mt::uint32>)
    .def("UI8sortDecreasing", &mt::sortDecreasing<mt::uint8>)
    .def("I32sortDecreasing", &mt::sortDecreasing<mt::int32>)
    .def("I8sortDecreasing", &mt::sortDecreasing<mt::int8>); 

  py::class_<mt::Adjacency, std::shared_ptr<mt::Adjacency>>(m, "Adjacency")
    .def("neighbours", &mt::Adjacency::neighbours);

  py::class_<mt::Adjacency4C, mt::Adjacency, std::shared_ptr<mt::Adjacency4C>>(m, "Adjacency4C")
    .def(py::init<mt::Box>())
    .def("neighbours", &mt::Adjacency4C::neighbours);

  py::class_<mt::Adjacency8C, mt::Adjacency, std::shared_ptr<mt::Adjacency8C>>(m, "Adjacency8C")
    .def(py::init<mt::Box>())
    .def("neighbours", &mt::Adjacency8C::neighbours);

  py::enum_<mt::DiagonalConnection>(m, "DiagonalConnection", py::arithmetic())
    .value("None", mt::DiagonalConnection::None)
    .value("SW", mt::DiagonalConnection::SW)
    .value("NE", mt::DiagonalConnection::NE)
    .value("SE", mt::DiagonalConnection::SE)
    .value("NW", mt::DiagonalConnection::NW);

  py::class_<mt::AdjacencyUC, mt::Adjacency, std::shared_ptr<mt::AdjacencyUC>>(m, "AdjacencyUC")
    .def(py::init<mt::Box>())
    .def("neighbours", &mt::AdjacencyUC::neighbours)
    .def("dconn", py::overload_cast<mt::uint32>(&mt::AdjacencyUC::dconn))
    .def("dconn", py::overload_cast<mt::uint32, mt::DiagonalConnection>(&mt::AdjacencyUC::dconn));

  py::class_<mt::CTBuilderResult>(m, "CTBuilderResult")
    .def(py::init<const std::vector<mt::uint32>, std::vector<mt::uint32>>())
    .def_readwrite("parent", &mt::CTBuilderResult::parent)
    .def_readwrite("R", &mt::CTBuilderResult::R);

  bindFoundamentalTypeCTBuilder(m);
  bindFoundamentalTypeMTNode(m);
  bindFoundamentalTypeMorphologicalTree(m);

  // tree of shapes
  bindFoundamentalTypeKeyValue(m);
  bindFoundamentalTypeHQueue(m);

  bindFoundamentalTypeInterval(m);
  bindFoundamentalTypeKGrid(m);
}