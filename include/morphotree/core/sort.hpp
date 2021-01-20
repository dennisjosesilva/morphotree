#pragma once

#include "morphotree/core/alias.hpp"
#include "morphotree/core/types.hpp"

#include <vector>
#include <algorithm>
#include <numeric>
#include <functional>

namespace morphotree
{
  template<typename T>
  std::vector<uint32> STLsortIndex(const std::vector<T> &v, 
    std::function<bool(const T&, const T&)> cmp);

  template<typename T>
  std::vector<uint32> countingSortIncreasing(const std::vector<T> &v);

  template <typename T>
  std::vector<uint32> countingSortDecreasing(const std::vector<T> &v);
    
  template<typename T>
  std::vector<uint32> sortIncreasing(const std::vector<T> &v);

  template<typename T>
  std::vector<uint32> sortDecreasing(const std::vector<T> &v);


  // ===========[ IMPLEMENTATION ] ============================================
  template<typename T>
  std::vector<uint32> STLsortIndex(const std::vector<T> &v, 
    std::function<bool(const T&, const T&)> cmp)
  {
    std::vector<uint32> idx(v.size());
    std::iota(idx.begin(), idx.end(), 0);
    std::sort(idx.begin(), idx.end(), [&v, cmp](uint i1, uint i2) {
      return cmp(v[i1], v[i2]);
    });
    return idx;
  }

  template<typename T>
  std::vector<uint32> countingSortIncreasing(const std::vector<T> &v)
  {
    T maxvalue = std::numeric_limits<T>::max();
    std::vector<uint32> counter(maxvalue + 1, 0);
    std::vector<uint32> idx(v.size());

    for (uint32 i = 0; i < v.size(); i++)
      counter[maxvalue - v[i]]++;

    for (T i = 1; i < maxvalue; i++) 
      counter[i] += counter[i - 1];
    counter[maxvalue] += counter[maxvalue-1];


    for (int i = v.size() - 1; i >= 0; --i)
      idx[--counter[maxvalue - v[i]]] = i;

    return idx;    
  }

  template<typename T>            
  std::vector<uint32> countingSortDecreasing(const std::vector<T> &v)
  {
    T maxvalue = std::numeric_limits<T>::max();
    std::vector<uint32> counter(maxvalue+1, 0);
    std::vector<uint32> idx(v.size());

    for (uint32 i = 0; i < v.size(); i++)
      counter[v[i]]++;

    for (T i = 1; i < maxvalue; i++) 
      counter[i] += counter[i-1];
    counter[maxvalue] += counter[maxvalue-1];

    for (int i = v.size()-1; i >= 0; i--)
      idx[--counter[v[i]]] = i;

    return idx;
  }

  template<typename T>
  std::vector<uint> sortIncreasing(const std::vector<T> &v)
  {
    if (isLowSizeType<T>())
      return countingSortIncreasing(v);
    return STLsortIndex<T>(v, [](const T& v1, const T& v2){ return v1 > v2; });
  }

  template<typename T>
  std::vector<uint32> sortDecreasing(const std::vector<T> &v)
  {
    if (isLowSizeType<T>())
      return countingSortDecreasing(v);
    return STLsortIndex<T>(v, [](const T& v1, const T& v2) { return v1 < v2; });
  }
}