#pragma once

#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <pybind11/functional.h>
#include <pybind11/numpy.h>

#include "morphotree/filtering/lexographicalFilter.hpp"

#include "core/opaque_types.hpp"

namespace py = pybind11;
namespace mt = morphotree;

template<typename ValueType>
void bindIFilterTreeLexographically(py::module &m, const std::string &valueType);

template<typename ValueType>
void bindFilterTreeLexographically(py::module &m, const std::string &valueType);

void bindFoundamentalTypeFilterTreeLexographically(py::module &m);

// ==================== [ IMPLEMENTATION ] ============================================
template<typename ValueType>
void bindIFilterTreeLexographically(py::module &m, const std::string &valueType)
{
  std::string functionName = valueType + "filterTreeLexographically";
  m.def(functionName.c_str(), &mt::filterTreeLexographically<ValueType>);
}

template<typename ValueType>
void bindFilterTreeLexographically(py::module &m, const std::string &valueType)
{
  std::string functionName = valueType + "ifilterTreeLexographically";
  m.def(functionName.c_str(), &mt::ifilterTreeLexographically<ValueType>);
}