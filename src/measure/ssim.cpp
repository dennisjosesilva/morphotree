#include "morphotree/measure/ssim.hpp"

namespace morphotree
{
  SSIM::SSIM(float C1, float C2, float C3,
    float alpha, float beta, float gamma, const ConvKernel &w)
    :C1_{C1}, C2_{C2}, C3_{C3}, alpha_{alpha}, beta_{beta}, gamma_{gamma}, 
     w_{w}
  {}

  float SSIM::l(float fmean, float gmean) const
  {
    float val = (2.f * fmean * gmean + C1_) /
      ((fmean*fmean) + (gmean*gmean) + C1_);
    return val;
  }

  float SSIM::c(float fstd, float gstd) const
  {
    float val = (2.f * fstd * gstd + C2_) / 
      ( (fstd*fstd) + (gstd*gstd) + C2_);
    return val;
  }

  float SSIM::s(float fgstd, float fstd, float gstd) const 
  {
    float val = (fgstd + C3_) / (fstd * gstd + C3_);
    return val;
  }
}