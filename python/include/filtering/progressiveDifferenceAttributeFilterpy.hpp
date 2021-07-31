#pragma once 

#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <pybind11/functional.h>

#include "morphotree/filtering/progressiveDifferenceAttributeFilter.hpp"

namespace py = pybind11;
namespace mt = morphotree;

template<typename ValueType, typename AttrType>
void bindProgessiveDifferenceFilter(py::module &m, const std::string &valueType, 
  const std::string &attrType);

template<typename ValueType, typename AttrType>
void bindIProgressiveDifferenceFilter(py::module &m, const std::string &valueType, 
  const std::string &attrType);

void bindFoundamentalTypesProgressiveDifferenceFilter(py::module &m);

// =====================[ IMPLEMENTATION ] ====================================================
template<typename ValueType, typename AttrType>
void bindProgressiveDifferenceFilter(py::module &m, const std::string &valueType, 
  const std::string &attrType)
{
  std::string functionName = "A" + attrType + "V" + valueType + "progressiveDifferenceFilter";
  m.def(functionName.c_str(), &mt::progressiveDifferenceFilter<ValueType, AttrType>);
}

template<typename ValueType, typename AttrType>
void bindIProgressiveDifferenceFilter(py::module &m, const std::string &valueType, 
  const std::string &attrType)
{
  std::string functionName = "A" + attrType + "V" + valueType + "iprogressiveDifferenceFilter";
  m.def(functionName.c_str(), &mt::iprogressiveDifferenceFilter<ValueType, AttrType>);
}