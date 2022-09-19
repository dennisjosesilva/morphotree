#include "morphotree/attributes/countorExtraction.hpp"
#include "morphotree/adjacency/adjacency4c.hpp"
#include "morphotree/tree/ct_builder.hpp"

#include "morphotree/core/io.hpp"


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
  using mt::buildMaxTree;
  using mt::printImageIntoConsoleWithCast;

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

  return 0;
}