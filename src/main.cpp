#include "morphotree/core/convKernel.hpp"
#include "morphotree/core/io.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

int main(int argc, char *argv[])
{
  using ConvKernel = morphotree::ConvKernel;
  using Box = morphotree::Box;
  using uint8 = morphotree::uint8;
  using UI32Point = morphotree::UI32Point;

  ConvKernel k = ConvKernel::Gaussian(2.0f);

  std::vector<float> kernel = k.weight();  
  
  int width, height, nchannels;
  uint8 *data = stbi_load(argv[1], &width, &height, &nchannels, 1);

  Box domain = Box::fromSize(UI32Point{width, height});
  std::vector<uint8> f{data, data + (domain.numberOfPoints())}; 

  std::vector<uint8> f_filtered = k.convolve(domain, f);
  
  stbi_write_png(argv[2], domain.width(), domain.height(), 1, (void*)f_filtered.data(), 0);
  stbi_image_free(data);

  return 0;
}