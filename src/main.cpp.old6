
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

#include "morphotree/filtering/treeFilterRules.hpp"


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
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 2, 2, 2, 2, 2, 2, 2, 2, 2 ,2, 2, 0,
    0, 2, 4, 4, 4, 2, 4, 4, 4, 4, 4, 2, 0,
    0, 2, 4, 7, 4, 2, 4, 7, 4, 7, 4, 2, 0,
    0, 2, 4, 4, 4, 2, 4, 4, 4, 4, 4, 2, 0,
    0, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
  };

  mt::Box domain = mt::Box::fromSize(mt::UI32Point(13, 7));
    
  MTree tree = 
    mt::buildMaxTree(f, std::make_shared<mt::Adjacency8C>(domain));

  std::vector<bool> keep = {true, false, false, true, true, false, false};

  MTree tree_filtered = mt::maxRuleFilter(tree, [&keep](NodePtr node) { return keep[node->id()]; });

  tree_filtered.traverseByLevel([&domain, &tree](NodePtr node) {
    std::cout << "node.id= " << node->id() << std::endl;
    std::cout << "id= " << tree.smallComponent(node->representative())->id() << std::endl;
    mt::printImageIntoConsoleWithCast<mt::int32>(node->reconstruct(domain), domain);   
    std::cout << std::endl;
  });

  std::cout << std::endl << std::endl;
  std::vector<ValueType> f_filtered = tree_filtered.reconstructImage();
  mt::printImageIntoConsoleWithCast<mt::int32>(f_filtered, domain);

  return 0;
}