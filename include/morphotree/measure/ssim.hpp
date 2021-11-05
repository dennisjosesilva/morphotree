#pragma once 

#include "morphotree/core/point.hpp"
#include "morphotree/core/box.hpp"

#include <vector>

namespace morphotree
{
  class SSIM
  {
  public:
    SSIM(const std::vector<float> &weight, float padValue=0);

    // inline float getWeight(const I32Point &p) const { return getWeight(p.x(), p.y()); };
    // float getWeight(int x, int y) const;

  private:    
    std::vector<float> weight_;
    float padValue_;
  };

}