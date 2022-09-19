#include "morphotree/attributes/countorExtraction.hpp"
#include "morphotree/adjacency/adjacency4c.hpp"
#include "morphotree/tree/ct_builder.hpp"

#include "morphotree/core/io.hpp"


#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

namespace mt = morphotree;

int main(int argc, char *argv[])
{
  using mt::uint8;
  using mt::uint32;
  using MTree = mt::MorphologicalTree<uint8>;
  using NodePtr = typename MTree::NodePtr;
  using mt::InfAdjacency4C;
  using mt::Adjacency4C;
  using mt::Adjacency;
  using mt::Box;
  using mt::extractCountors;
  using mt::reconstructContourImage;
  using mt::UI32Point;
  using mt::I32Point;
  using mt::buildMaxTree;
  using mt::printImageIntoConsoleWithCast;

  if (argc < 2) {
    std::vector<uint8> f = {
      0, 0, 0, 0, 0, 0, 0,
      0, 4, 4, 4, 7, 7, 7,
      0, 7, 7, 4, 7, 4, 7,
      0, 7, 4, 4, 7, 4, 7,
      0, 4, 4, 4, 7, 4, 7,
      0, 7, 7, 4, 7, 7, 7,
      0, 0, 0, 0, 0, 0, 0,
    };

    Box domain = Box::fromSize(UI32Point{7,7});
    std::shared_ptr<Adjacency> adj 
      = std::make_shared<Adjacency4C>(domain);
    MTree tree = buildMaxTree(f, adj);

    std::vector<std::set<uint32>> countors = 
      extractCountors(
        domain, 
        f, 
        std::make_shared<InfAdjacency4C>(domain), 
        tree);

    tree.traverseByLevel([&domain, &countors](NodePtr node) {
      std::vector<bool> bimg = node->reconstruct(domain);
      std::cout << "\nbimage:\n";
      printImageIntoConsoleWithCast<int>(bimg, domain);
      std::cout << "\ncontours:\n";
      std::vector<bool> bcontour = 
        reconstructContourImage(countors[node->id()], domain);
      printImageIntoConsoleWithCast<int>(bcontour, domain);
      std::cout << "\n";
    });
  }
  else {
    int nx, ny, nc;
    uint8 *data = stbi_load(argv[1], &nx, &ny, &nc, 0);

    std::vector<uint8> f(data, data + (nx*ny));
    Box domain = Box::fromSize(UI32Point{nx, ny});

    std::shared_ptr<Adjacency> adj 
      = std::make_shared<Adjacency4C>(domain);
    MTree tree = buildMaxTree(f, adj);

    std::vector<std::set<uint32>> countors = 
      extractCountors(
        domain, 
        f, 
        std::make_shared<InfAdjacency4C>(domain), 
        tree);

    NodePtr node = tree.smallComponent(domain.pointToIndex(
      I32Point{nx/2, ny/2}));
    const std::set<uint32> &contour = countors[node->id()];
    std::vector<uint8> output(domain.numberOfPoints(), 255);
    for (uint32 pidx : node->reconstruct())
      output[pidx] = 0;
    for (uint32 pidx : contour)
      output[pidx] = 128;

    stbi_write_png("out.png", nx, ny, 1, output.data(), 0);
  } 
  
  return 0;
}