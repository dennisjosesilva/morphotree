#pragma once 

#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <pybind11/functional.h>
#include <pybind11/numpy.h>

#include "morphotree/filtering/extinctionFilter.hpp"

#include "core/opaque_types.hpp"

namespace py = pybind11;
namespace mt = morphotree;

template<typename ValueType, typename AttrType>
void bindIExtinctionFilter(py::module &m, const std::string &valueType, 
  const std::string &attrType);

template<typename ValueType, typename AttrType>
void bindExtinctionFilter(py::module &m, const std::string &valueType,
  const std::string &attrType);

void bindFoundamentalTypesExtinctionFilter(py::module &m);

// =============================== [ IMPLEMENTATION ] ========================================
template<typename ValueType, typename AttrType>
void bindIExtinctionFilter(py::module &m, const std::string &valueType, 
  const std::string &attrType)
{
  std::string functioName = "A" + attrType + "V" + valueType + "extinctionFilter";
  m.def(functioName.c_str(), &mt::extinctionFilter<ValueType, AttrType>);
}

template<typename ValueType, typename AttrType>
void bindExtinctionFilter(py::module &m, const std::string &valueType, 
  const std::string &attrType)
{
  std::string functionName = "A" + attrType + "V" + valueType + "iextinctionFilter";
  m.def(functionName.c_str(), &mt::iextinctionFilter<ValueType, AttrType>);
}