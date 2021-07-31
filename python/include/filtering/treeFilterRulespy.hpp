#pragma once

#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <pybind11/functional.h>

#include "morphotree/filtering/treeFilterRules.hpp"

namespace py = pybind11;
namespace mt = morphotree;

template<typename ValueType>
void bindMaxRuleFilter(py::module &m, const std::string &valueType);

template<typename ValueType>
void bindIMaxRuleFilter(py::module &m, const std::string &valueType);

void bindFoundamentalTypesMaxRuleFilter(py::module &m);

// ======================[ IMPLEMENTATION ] =================================================
template<typename ValueType>
void bindMaxRuleFilter(py::module &m, const std::string &valueType)
{
  std::string functionName = valueType + "maxRuleFilter";
  m.def(functionName.c_str(), &mt::maxRuleFilter<ValueType>);
}

template<typename ValueType>
void bindIMaxRuleFilter(py::module &m, const std::string &valueType)
{
  std::string functionName = valueType + "imaxRuleFilter";
  m.def(functionName.c_str(), &mt::imaxRuleFilter<ValueType>);
}