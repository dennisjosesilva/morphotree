
#include <iostream>
#include <vector>
// #include "morphotree/filtering/globalOptimiser/MinCPerimeterWithAbsError.hpp"

#include "morphotree/attributes/topologicalHeightComputer.hpp"
#include "morphotree/attributes/numberOfDescendantsComputer.hpp"
#include "morphotree/attributes/boundingboxComputer.hpp"

#include "morphotree/attributes/extinctionValues/ExtinctionValueLeavesComputer.hpp"
#include "morphotree/attributes/extinctionValues/ExtinctionValueComputer.hpp"
#include "morphotree/adjacency/adjacency8c.hpp"
#include "morphotree/attributes/areaComputer.hpp"
#include "morphotree/tree/mtree.hpp"
#include "morphotree/core/io.hpp"


namespace mt = morphotree;

int main(int argc, char *argv[]) 
{
  using ValueType = mt::uint8;
  using ExtinctionValueLeavesComputerType = mt::ExtinctionValueLeavesComputer<ValueType, mt::uint32>;
  using ExtinctionValueComputerType = mt::ExtinctionValueComputer<ValueType, mt::uint32>;
  using MTree = mt::MorphologicalTree<mt::uint8>;
  using NodePtr = MTree::NodePtr;

  std::vector<ValueType> f = {
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 0,
    0, 2, 4, 4, 4, 2, 4, 4, 4, 4, 2, 0,
    0, 2, 4, 7, 4, 2, 4, 7, 7, 4, 2, 0,
    0, 2, 4, 4, 4, 2, 4, 4, 4, 4, 2, 0,
    0, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 
  };

  mt::Box domain = mt::Box::fromSize(mt::UI32Point(12, 7));
    
  MTree tree = 
    mt::buildMaxTree(f, std::make_shared<mt::Adjacency8C>(domain));

  std::vector<mt::Box> bbs = std::make_unique<mt::BoundingBoxComputer<mt::uint8>>(
    domain)->computeAttribute(tree);

  std::vector<mt::uint32> numberOfDescendants = std::make_unique<
    mt::NumberOfDescendantsComputer<mt::uint8>>()->computeAttribute(tree);

  std::vector<mt::uint32> topHeight = 
    std::make_unique<mt::TopologicalHeightComputer<mt::uint8>>()->computeAttribute(tree);

  tree.traverseByLevel([&domain, &bbs, &numberOfDescendants, &topHeight](NodePtr node) {
    std::cout << "node.id= " << node->id() << "\n";
    mt::Box bb = bbs[node->id()];
    
    std::cout << "bb[" << node->id() << "]= " << bb;
    std::cout << "bb.width[" << node->id() << "]= " << bb.width() << "\n"; 
    std::cout << "bb.height[" << node->id() << "]= " << bb.height() << "\n"; 
    std::cout << "numberOfDescendants[" << node->id() << "]= " << numberOfDescendants[node->id()] << "\n";
    std::cout << "topHeight[" << node->id() << "]= " << topHeight[node->id()] << "\n";

    mt::printImageIntoConsoleWithCast<mt::int32>(node->reconstruct(domain), domain);
    std::cout << std::endl;
  });

  return 0;
}