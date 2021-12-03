#include "morphotree/core/box.hpp"
#include "morphotree/tree/mtree.hpp"
#include "morphotree/adjacency/adjacency8c.hpp"
#include "morphotree/core/io.hpp"

#include <vector>

namespace mt = morphotree;

int main(int argc, char *argv[]) 
{
  using MTree = mt::MorphologicalTree<mt::uint8>;
  
  mt::Box domain = mt::Box::fromSize(mt::UI32Point{7, 7});
  std::vector<mt::uint8> f = {
    0, 0, 0, 0, 0, 0, 0,
    0, 4, 4, 4, 7, 7, 7,
    0, 7, 7, 4, 7, 4, 7,
    0, 7, 4, 4, 7, 4, 7,
    0, 4, 4, 4, 7, 4, 7,
    0, 7, 7, 4, 7, 7, 7,
    0, 0, 0, 0, 0, 0, 0 
  };

  MTree tree = mt::buildMaxTree(f, std::make_shared<mt::Adjacency8C>(domain));

  std::vector<bool> bimg = 
    tree.smallComponent(domain.pointToIndex(4, 1))->reconstruct(domain);

  mt::printImageIntoConsoleWithCast<int>(bimg, domain);

  std::vector<bool> keep(tree.numberOfNodes(), true);
  keep[tree.smallComponent(domain.pointToIndex(4, 1))->id()] = false;

  for (int i = 0; i < keep.size(); i++)
    std::cout << "i: " << keep[i] << std::endl;

  bimg = 
    tree.smallComponent(domain.pointToIndex(4, 1), keep)->reconstruct(domain);

  std::cout << "------------- Removing node at (4, 1) --------------------\n";
  mt::printImageIntoConsoleWithCast<int>(bimg, domain);

  return 0;
}