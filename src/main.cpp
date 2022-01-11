#include "morphotree/core/io.hpp"
#include "morphotree/measure/ssim.hpp"

#include "morphotree/tree/ct_builder.hpp"
#include "morphotree/tree/mtree.hpp"
#include "morphotree/adjacency/adjacency8c.hpp"

#include "morphotree/filtering/globalOptimiser/MinCPerimeterWithSSIM.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#define STB_IMAGE_WRITE_IMEPLEMENTATION
#include "stb_image_write.h"

int main(int argc, char *argv[])
{
  using morphotree::Box;
  using morphotree::uint8;
  using morphotree::uint32;
  using morphotree::UI32Point;
  using morphotree::Adjacency8C;
  using morphotree::buildMaxTree;
  using MTree = morphotree::MorphologicalTree<uint8>;
  using NodePtr = typename MTree::NodePtr;
  using Optimiser = morphotree::MinCPerimeterWithSSIM<uint8>;

  // Read input image
  // ----------------
  int width, height, nchannels;
  uint8 *data = stbi_load(argv[1], &width, &height, &nchannels, 1);  
  Box domain = Box::fromSize(UI32Point{ static_cast<uint32>(width), static_cast<uint32>(height)} );
  std::vector<uint8> f{data, data + domain.numberOfPoints()};

  // build max-tree
  MTree tree = buildMaxTree(f, std::make_shared<Adjacency8C>(domain));

  // filtering
  Optimiser opt{ domain, f, "../resource/dt-max-tree-8c.dat", tree, 0.05 };  
  std::vector<uint8> f_filtered = opt.filter(0.7);

  std::cout << "DONE";

  return 0;
}