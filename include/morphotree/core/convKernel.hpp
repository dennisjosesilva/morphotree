#pragma once 

#include "morphotree/core/point.hpp"
#include "morphotree/core/box.hpp"

#include <iostream>
#include <vector>
#include <functional>

#include <omp.h>

namespace morphotree 
{
  class ConvKernel
  {
  public:
    ConvKernel(uint32 width, uint32 height, 
      const std::vector<float> &weight);

    static ConvKernel SobelX();
    static ConvKernel SobelY();
    static ConvKernel PrewittX();
    static ConvKernel PrewittY();
    static ConvKernel Laplancian();
    static ConvKernel Gaussian(unsigned int size, float sigma);
    static ConvKernel Gaussian(float sigma=1.0f);

    inline const Box& domain() const { return domain_; }

    inline const std::vector<float> weight() const { return weight_; }
    inline float weight(uint32 idx) const { return weight_[idx]; }
    inline float weight(const I32Point& p) const { return weight(domain_.pointToIndex(p)); }
    inline float weight(int x, int y) const { return weight(I32Point{x, y}); }

    float operator[](uint32 idx) const { return weight_[idx]; }    

    template<typename ValueType>
    std::vector<ValueType> convolve(const Box &fDomain,
      const std::vector<ValueType> &f,
      std::function<ValueType(const I32Point &, const I32Point &, const ConvKernel&)> outDomainValue=
      [](const I32Point &, const I32Point &, const ConvKernel&) { return 0; }) const;

  private:
    Box domain_;
    std::vector<float> weight_;    
  };

  // =============== [IMPLEMENTATION] ========================================================
  template<typename ValueType>
  std::vector<ValueType> ConvKernel::convolve(const Box &fDomain,
    const std::vector<ValueType> &f,
    std::function<ValueType(const I32Point &, const I32Point &, const ConvKernel&)> outDomainValue) const
  {
    std::vector<ValueType> fFiltered(fDomain.numberOfPoints(), 0);

    // I32Point p;    

    #pragma omp parallel for   
    for (int y = fDomain.top(); y <= fDomain.bottom(); y++) {
      for (int x = fDomain.left(); x <= fDomain.right(); x++) {        
        I32Point p{x, y};
        I32Point k;
        float acc = 0;
        for (k.y() = domain_.top(); k.y() <= domain_.bottom(); k.y()++) {
          for (k.x() = domain_.left(); k.x() <= domain_.right(); k.x()++) {
            I32Point q = p + k;
            if (!fDomain.contains(q))
              acc += outDomainValue(q, k, *this);
            else
              acc += weight(k) * f[fDomain.pointToIndex(q)];
          }          
        }
        fFiltered[fDomain.pointToIndex(p)] = static_cast<ValueType>(acc);        
      }
    }
    return fFiltered;
  }
}