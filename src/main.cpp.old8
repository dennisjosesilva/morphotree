#include "morphotree/core/convKernel.hpp"
#include "morphotree/core/io.hpp"
#include "morphotree/measure/ssim.hpp"

#include <morphotree/tree/ct_builder.hpp>
#include <morphotree/tree/mtree.hpp>
#include <morphotree/attributes/areaComputer.hpp>
#include <morphotree/adjacency/adjacency8c.hpp>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

int main(int argc, char *argv[])
{
  using ConvKernel = morphotree::ConvKernel;
  using Box = morphotree::Box;
  using uint8 = morphotree::uint8;
  using uint32 = morphotree::uint32;
  using UI32Point = morphotree::UI32Point;
  using SSIM = morphotree::SSIM;
  using Adjacency8C = morphotree::Adjacency8C;
  using MTree = morphotree::MorphologicalTree<uint8>;
  using AreaComputer = morphotree::AreaComputer<uint8>;
  using NodePtr = typename MTree::NodePtr;
  
  using morphotree::buildMaxTree;

  // ConvKernel k = ConvKernel::Gaussian(2.0f);
  // ConvKernel k = ConvKernel::SobelY();

  // std::vector<float> kernel = k.weight();  
  
  int width, height, nchannels;
  uint8 *data = stbi_load(argv[1], &width, &height, &nchannels, 1);

  Box domain = Box::fromSize(UI32Point{static_cast<uint32>(width), static_cast<uint32>(height)});
  std::vector<uint8> f{data, data + (domain.numberOfPoints())}; 

  // // area opening
  // MTree tree = buildMaxTree(f, std::make_shared<Adjacency8C>(domain));
  // std::vector<uint32> area = std::make_unique<AreaComputer>()->computeAttribute(tree);
  // std::vector<uint8> f_filtered = tree.directFilter([&area](NodePtr node){ 
  //   return area[node->id()] >= 100;
  // }).reconstructImage();

  SSIM ssim;    
  std::vector<uint8> f_filtered = ConvKernel::Gaussian(1.0f).convolve(domain, f);  
  
  SSIM::Result r =  ssim.computeWithMap(domain, f, f_filtered);  
  std::cout << r.fullResult << std::endl;

  std::vector<uint8> ssimMap(r.map.size());
  for (uint32 i = 0; i < ssimMap.size(); ++i) 
    ssimMap[i] = static_cast<uint8>((1.0f-r.map[i])*255);

  stbi_write_png(argv[2], domain.width(), domain.height(), 1, 
    (void*)f_filtered.data(), 0);
  
  stbi_write_png(argv[3], domain.width(), domain.height(), 1, 
    (void*)ssimMap.data(), 0);
  stbi_image_free(data);

  return 0;
}