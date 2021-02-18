#pragma once

#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <pybind11/stl_bind.h>

#include <vector>
#include "morphotree/core/alias.hpp"
#include "morphotree/attributes/bitquads/quads.hpp"

namespace py = pybind11;
namespace mt = morphotree;

PYBIND11_MAKE_OPAQUE(std::vector<mt::uint8>);
PYBIND11_MAKE_OPAQUE(std::vector<mt::int8>);
PYBIND11_MAKE_OPAQUE(std::vector<mt::uint32>);
PYBIND11_MAKE_OPAQUE(std::vector<mt::int32>);
PYBIND11_MAKE_OPAQUE(std::vector<mt::Quads>);