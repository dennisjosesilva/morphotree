#pragma once

#include <pybind11/pybind11.h>
#include <pybind11/operators.h>
#include <morphotree/core/hqueue.hpp>
#include <string>
#include <sstream>

namespace py = pybind11;
namespace mt = morphotree;

template<typename K, typename V>
void bindKeyValue(py::module &m, const std::string &typeKey, const std::string &typeValue); 

template<typename K, typename V>
void bindHQueue(py::module &m, const std::string &typeKey, const std::string &typeValue);

void bindFoundamentalTypeHQueue(py::module &m);
void bindFoundamentalTypeKeyValue(py::module &m); 

// ============================== [ IMPLEMENTATION ] ==========================================
template<typename K, typename V>
void bindKeyValue(py::module &m, const std::string &typeKey, const std::string &typeValue)
{
  std::string className = "K" + typeKey + "V" + typeValue;
  py::class_<mt::KeyValue<K, V>>(m, className.c_str())
    .def(py::init<K, V>(), py::arg("key") = K(), py::arg("value") = V())
    .def_property("key", 
      [](mt::KeyValue<K,V> &kv) { return kv.key; }, 
      [](mt::KeyValue<K, V> &kv, K k) { kv.key = k; })
    .def_property("value", 
      [](mt::KeyValue<K,V> &kv){ return kv.value; }, 
      [](mt::KeyValue<K,V> &kv, V v){ kv.value = v; })
    .def("__repr__", [](const mt::KeyValue<K, V> &kv){
      std::stringstream ss;
      ss << kv;
      return ss.str();
    });
}

template<typename K, typename V>
void bindHQueue(py::module &m, const std::string &typeKey, const std::string &typeValue)
{
  std::string className = "K" + typeKey  + "V" + typeValue + "HQueue";
  py::class_<mt::HQueue<K, V>>(m, className.c_str())
    .def(py::init())
    .def("insert", &mt::HQueue<K, V>::insert)
    .def("pop", &mt::HQueue<K, V>::pop)
    .def("front", &mt::HQueue<K, V>::front)
    .def("isEmpty", &mt::HQueue<K, V>::isEmpty);
}