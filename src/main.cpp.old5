
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

#include "morphotree/attributes/differenceAttributeComputer.hpp"

#include "morphotree/filtering/extinctionFilter.hpp"


namespace mt = morphotree;

int main(int argc, char *argv[]) 
{
  using ValueType = mt::uint8;
  using AreaType = typename mt::AreaComputer<mt::uint8>::AttrType;
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


  std::vector<AreaType> area = 
    std::make_unique<mt::AreaComputer<mt::uint8>>()->computeAttribute(tree);

  std::vector<AreaType> areaDifference = 
    std::make_unique<mt::DifferenceAttributeComputer<AreaType, ValueType>>(area)->computeAttribute(tree);

  MTree tree_filtered = mt::extinctionFilter(tree, area, 1);
  

  std::cout << "========================= TREE =======================================\n";
  tree.traverseByLevel([&domain, &area, &areaDifference](NodePtr node) {
    std::cout << "node.id= " << node->id() << std::endl;
    std::cout << "area[" << node->id() << "]= " << area[node->id()] << "\n";
    std::cout << "areaDifference[" << node->id() << "]= " << areaDifference[node->id()] << "\n";
    mt::printImageIntoConsoleWithCast<mt::int32>(node->reconstruct(domain), domain);
    std::cout << std::endl;
  });

  std::cout << "========================= TREE FILTERED ================================\n";
  tree_filtered.traverseByLevel([&domain, &tree](NodePtr node) {
    std::cout << "node.id= " << node->id() << std::endl;
    std::cout << "original node=" << tree.smallComponent(node->representative())->id() << "\n"; 
    mt::printImageIntoConsoleWithCast<mt::int32>(node->reconstruct(domain), domain);
    std::cout << std::endl;
  });

  
  std::vector<ValueType> f_filtered = tree_filtered.reconstructImage();

  std::cout << "\n";
  std::cout << "========================= IMAGE FILTERED ================================\n";
  mt::printImageIntoConsoleWithCast<mt::int32>(f_filtered, domain);

  std::cout << "\n";
  std::cout << "========================= IMAGE NODE 2 ================================\n";
  NodePtr node = tree.smallComponent(tree_filtered.node(2)->representative());
  
  std::vector<ValueType> recNode = node->reconstructGrey(domain);
  mt::printImageIntoConsoleWithCast<mt::int32>(recNode, domain);

  return 0;
}