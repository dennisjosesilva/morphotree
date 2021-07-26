
#include <iostream>
#include <vector>
// #include "morphotree/filtering/globalOptimiser/MinCPerimeterWithAbsError.hpp"

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

  std::vector<mt::uint32> area = mt::AreaComputer<mt::uint8>().computeAttribute(tree);

  ExtinctionValueLeavesComputerType extictionValueComp;

  std::unordered_map<mt::uint32, mt::uint32> extValues = 
    extictionValueComp.compute(tree, area);

  std::cout << std::endl;
  for (std::pair<mt::uint32, mt::uint32> keyValue : extValues) {
    std::cout << "Ex[" << keyValue.first << "] = " << keyValue.second << "\n";
  }

  using MapType = decltype(extValues);

  std::vector<mt::uint32> extValuesAttr = 
    ExtinctionValueComputerType(extValues).computeAttribute(tree);

  tree.traverseByLevel([&area, &domain, &extValuesAttr](NodePtr node) {
    std::cout << "node.id= " << node->id() << "\n";
    std::cout << "area[" << node->id() << "]= " << area[node->id()] << "\n";
    
    std::cout << "extinction[" << node->id() << "]= " << 
      extValuesAttr[node->id()] << "\n";
    
    mt::printImageIntoConsoleWithCast<mt::int32>(node->reconstruct(domain), domain);
    std::cout << std::endl;
  });

  return 0;
}