#include "morphotree/core/convKernel.hpp"

#include <exception>
#include <cmath>


#include <iostream>

namespace morphotree 
{
  ConvKernel::ConvKernel(uint32 width, uint32 height,
    const std::vector<float> &weight)
    :weight_{weight}
  {
    if (width % 2 == 0) 
      throw std::runtime_error("ConvKernel with even width");

    if (height % 2 == 0)
      throw std::runtime_error("ConvKernel with even height");

    I32Point tl{ - static_cast<int>(width)/2, -static_cast<int>(height)/2 };
    domain_ = Box::fromSize(tl, UI32Point{width, height});
    
    if (weight.size() != domain_.numberOfPoints()) 
      throw std::runtime_error("missmatch size of width*height with weight size");
  }
  
  ConvKernel ConvKernel::PrewittX()
  {
    std::vector<float> k = {
      -1, 0, 1,
      -1, 0, 1,
      -1, 0, 1
    };
    return ConvKernel{3, 3, k};
  }

  ConvKernel ConvKernel::PrewittY()
  {
    std::vector<float> k = { 
      -1, -1, -1,
       0,  0,  0,
       1,  1,  1
    };
    return ConvKernel{3, 3, k};
  }
  
  ConvKernel ConvKernel::SobelX()
  {
    std::vector<float> k = {
      -1, 0, 1,
      -2, 0, 2,
      -1, 0, 1
    };
    return ConvKernel{3, 3, k};
  }

  ConvKernel ConvKernel::SobelY()
  {
    std::vector<float> k = { 
      -1, -2, -1,
       0,  0,  0,
       1,  2,  1
    };
    return ConvKernel{3, 3, k};
  }

  ConvKernel ConvKernel::Laplancian()
  {
    std::vector<float> k = {
      0,  1, 0,
      0, -4, 1,
      0,  1, 0
    };
    return ConvKernel{3, 3, k};
  }

  ConvKernel ConvKernel::Gaussian(unsigned int size, float sigma)
  {
    if (size % 2 == 0)
      size += 1; // make it an odd number.
    
    int sizeOverTwo = size / 2;
    std::vector<float> k(size*size, 0.f);
    float sigmaTwo = sigma*sigma;
    // const float coef = 1.f / (2.0f * M_PI * (sigmaTwo));
    float sum = 0.f;

    unsigned int idx=0;
    for (int u = -sizeOverTwo; u <= sizeOverTwo; u++) {
      for (int v = -sizeOverTwo; v <= sizeOverTwo; v++) {
        k[idx] = expf(-(u*u + v*v)/(2.f*sigmaTwo));
        sum += k[idx];
        idx++;
      }
    }    

    for (idx = 0; idx < k.size(); idx++)
      k[idx] = k[idx] / sum;

    return ConvKernel{size, size, k};
  }

  ConvKernel ConvKernel::Gaussian(float sigma)
  {
    return Gaussian(static_cast<unsigned int>(6*sigma), sigma);
  }
}

