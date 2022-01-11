#pragma once 

#include "morphotree/core/point.hpp"
#include "morphotree/core/box.hpp"
#include "morphotree/core/convKernel.hpp"

#include <vector>
#include <cmath>

#include <omp.h>

namespace morphotree
{
  class SSIM
  {
  public:
    struct Result
    {
      std::vector<float> map;
      float fullResult;
    };
    
    SSIM(float C1=1.0f, float C2=1.0f, float C3=1.0f,
      float alpha=1.f, float beta=1.f, float gamma=1.f,      
      const ConvKernel &w = ConvKernel::Gaussian(11, 1.0f));

    template<typename ValueType>
    float compute(const Box &domain, const std::vector<ValueType> &f,
      const std::vector<ValueType> &g) const;

    template<typename ValueType>
    Result computeWithMap(const Box &domain, const std::vector<ValueType> &f,
      const std::vector<ValueType> &g) const;

  private:
    float l(float fmean, float gmean) const;
    float c(float fstd, float gstd) const;
    float s(float fgstd, float fstd, float gstd) const;

  private:    
    ConvKernel w_;
    float alpha_, beta_, gamma_;
    float C1_, C2_, C3_;
  };

  // ===============================[ IMPLEMENTATION ] ==================================
  template<typename ValueType>
  float SSIM::compute(const Box &domain, const std::vector<ValueType> &f, 
    const std::vector<ValueType> &g) const
  {
    Result r = computeWithMap(domain, f, g);
    return r.fullResult;
  }

  template<typename ValueType>
  SSIM::Result SSIM::computeWithMap(const Box &domain, const std::vector<ValueType> &f, 
    const std::vector<ValueType> &g) const 
  {
    std::vector<float> ssim(f.size(), 0.f);
    const Box &wdomain = w_.domain();
    I32Point p;

    #pragma omp parallel for
    for (int y = domain.top(); y <= domain.bottom(); y++) {
      for (int x = domain.left(); x <= domain.right(); x++) {
        I32Point p{x, y};
        I32Point o;
        float fmean = 0.f, gmean = 0.f;
        float fstd = 0.f, gstd = 0.f;
        float fgstd = 0.f;

        // compute mean
        for (o.y() = wdomain.top(); o.y() <= wdomain.bottom(); o.y()++) {
          for (o.x() = wdomain.left(); o.x() <= wdomain.right(); o.x()++) {
            I32Point q = p + o;
            if (domain.contains(q)) {
              uint32 qidx = domain.pointToIndex(q), oidx = wdomain.pointToIndex(o);
              fmean += w_[oidx] * f[qidx];
              gmean += w_[oidx] * g[qidx];              
            }
          }
        }

        for (o.y() = wdomain.top(); o.y() <= wdomain.bottom(); o.y()++) {
          for (o.x() = wdomain.left(); o.x() <= wdomain.right(); o.x()++) {
            I32Point q = p + o;
            if (domain.contains(q)) {
              uint32 qidx = domain.pointToIndex(q), oidx = wdomain.pointToIndex(o);
              
              float fminusMean = f[qidx] - fmean;
              float gminusMean = g[qidx] - gmean;

              float fminusMeanSquared = fminusMean * fminusMean;
              float gminusMeanSquared = gminusMean * gminusMean;

              fstd += w_[oidx] * fminusMeanSquared;
              gstd += w_[oidx] * gminusMeanSquared;
              fgstd += w_[oidx] * fminusMean * gminusMean;
            }
          }
        }

        fstd = sqrtf(fstd);
        gstd = sqrtf(gstd);
        
        ssim[domain.pointToIndex(p)] = 
          powf(l(fmean, gmean), alpha_) *
          powf(c(fstd, gstd), beta_) *
          powf(s(fgstd, fstd, gstd), gamma_);
      }        
    }

    float sumSSIM = 0.0f;
    for (float s : ssim) 
      sumSSIM += s;
    
    return Result{ ssim, sumSSIM / ssim.size() };
  }
}