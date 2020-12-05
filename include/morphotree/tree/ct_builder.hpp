#pragma once

#include "morphotree/adjacency/adjacency.hpp"
#include <functional>
#include <vector>
#include <limits>
#include <memory>

namespace morphotree
{
  struct CTBuilderResult
  {
    CTBuilderResult(const std::vector<uint32> par, std::vector<uint32> r)
      :parent{par}, R{r}
    {}

    std::vector<uint32> parent;
    std::vector<uint32> R;
  };

  template<class WeightType>
  class CTBuilder
  {
  public:
    CTBuilderResult build(const std::vector<WeightType> &f, 
                          std::shared_ptr<Adjacency> adj,
                          const std::vector<uint32> &R);

  private:
    static const uint32 UNDEF;  
    void initZPar(uint32 numberOfElements);
    uint32 findRoot(uint32 x);
    void canoniseTree(std::vector<uint32> &r, const std::vector<uint32> &R,
      const std::vector<WeightType> &f);

  private:
    std::vector<uint32> zpar_;
  };


  template<class WeightType> 
  const uint32 CTBuilder<WeightType>::UNDEF = std::numeric_limits<uint32>::max();

  // =============== [IMPLEMENTATION] ==================================================
  template<class WeightType>
  CTBuilderResult CTBuilder<WeightType>::build(const std::vector<WeightType> &f, 
          std::shared_ptr<Adjacency> adj,
          const std::vector<uint32> &R)
  {
    initZPar(f.size());
    std::vector<uint32> parent(f.size()); 

    for (uint32 p : R) {    
      parent[p] = p;
      zpar_[p] = p;
      for (uint32 n : adj->neighbours(p)) {        
        if (zpar_[n] != UNDEF) {
          uint32 r = findRoot(n);       
          if (r != p) {
            parent[r] = p;  
            zpar_[r] = p;
          }
        }
      }
    }
    canoniseTree(parent, R, f);
    return CTBuilderResult{parent, R};
  }

  template<class WeightType>
  void CTBuilder<WeightType>::initZPar(uint32 numberOfElements)
  {
    zpar_.resize(numberOfElements);        
    std::fill(zpar_.begin(), zpar_.end(), UNDEF);
  }

  template<class WeightType>
  uint32 CTBuilder<WeightType>::findRoot(uint32 x)
  { 
    if (zpar_[x] == x){
      return x;
    }
    else{
      zpar_[x] = findRoot(zpar_[x]);
      return zpar_[x];
    }
  }

  template<class WeightType>
  void CTBuilder<WeightType>::canoniseTree(std::vector<uint32> &parent, 
    const std::vector<uint32> &R, const std::vector<WeightType> &f)
  { 
    using RItr = std::vector<uint32>::const_reverse_iterator;
    for (RItr rit = R.rbegin(); rit != R.rend(); rit++)
    {
      uint32 q = parent[*rit];
      if (f[parent[q]] == f[q]) {
        parent[*rit] = parent[q];
      }
    }
  }
}