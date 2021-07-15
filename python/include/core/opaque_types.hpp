#pragma once

#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <pybind11/stl_bind.h>

#include <vector>
#include <unordered_map>
#include "morphotree/core/alias.hpp"
#include "morphotree/attributes/bitquads/quads.hpp"

namespace py = pybind11;
namespace mt = morphotree;

PYBIND11_MAKE_OPAQUE(std::vector<mt::uint8>);
PYBIND11_MAKE_OPAQUE(std::vector<mt::int8>);
PYBIND11_MAKE_OPAQUE(std::vector<mt::uint32>);
PYBIND11_MAKE_OPAQUE(std::vector<mt::int32>);
PYBIND11_MAKE_OPAQUE(std::vector<mt::Quads>);

using unordered_map_uint32_uint8 = std::unordered_map<mt::uint32, mt::uint8>;
using unordered_map_uint32_uint32 = std::unordered_map<mt::uint32, mt::uint32>;
using unordered_map_uint32_int8 = std::unordered_map<mt::uint32, mt::int8>;
using unordered_map_uint32_int32 = std::unordered_map<mt::uint32, mt::int32>;
using unordered_map_uint32_float = std::unordered_map<mt::uint32, float>;
using unordered_map_uint32_double = std::unordered_map<mt::uint32, double>;

PYBIND11_MAKE_OPAQUE(unordered_map_uint32_uint8);
PYBIND11_MAKE_OPAQUE(unordered_map_uint32_uint32);
PYBIND11_MAKE_OPAQUE(unordered_map_uint32_int8);
PYBIND11_MAKE_OPAQUE(unordered_map_uint32_int32);
PYBIND11_MAKE_OPAQUE(unordered_map_uint32_float);
PYBIND11_MAKE_OPAQUE(unordered_map_uint32_double);