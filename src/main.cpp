
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
#include "morphotree/filtering/progressiveDifferenceAttributeFilter.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

namespace mt = morphotree;

int main(int argc, char *argv[]) 
{
  using ValueType = mt::uint8;
  using AreaType = typename mt::AreaComputer<mt::uint8>::AttrType;
  using ExtinctionValueLeavesComputerType = mt::ExtinctionValueLeavesComputer<ValueType, mt::uint32>;
  using ExtinctionValueComputerType = mt::ExtinctionValueComputer<ValueType, mt::uint32>;
  using MTree = mt::MorphologicalTree<mt::uint8>;
  using NodePtr = MTree::NodePtr;


  int width, height, nchannels;
  ValueType *data = stbi_load("imgExample.png", &width, &height, &nchannels, 1);

  std::vector<ValueType> f(data, data + (width*height));
  mt::Box domain = mt::Box::fromSize(mt::UI32Point(width, height));
    
  MTree tree = 
    mt::buildMaxTree(f, std::make_shared<mt::Adjacency8C>(domain));

  std::vector<AreaType> area = 
    std::make_unique<mt::AreaComputer<ValueType>>()->computeAttribute(tree);

  tree.traverseByLevel([&domain, &area](NodePtr node) {
    std::cout << "node.id= " << node->id() << std::endl;    
    std::cout << "area[" << node->id() << "]= " << area[node->id()] << "\n";
    //mt::printImageIntoConsoleWithCast<mt::int32>(node->reconstruct(domain), domain);   
    std::cout << "\n";
  });

  MTree tree_filtered = mt::progressiveDifferenceFilter(tree, area, static_cast<AreaType>(35));
  

  std::cout << "================= FILTETED RESULT =========================================";
  tree_filtered.traverseByLevel([&domain, &area, &tree](NodePtr node) {
    NodePtr onode = tree.smallComponent(node->representative());
    std::cout << "node.id= " << node->id() << std::endl;  
    std::cout << "onode.id= " << onode->id() << "\n";    
    std::cout << "area[" << onode->id() << "]= " << area[onode->id()] << "\n";
    //mt::printImageIntoConsoleWithCast<mt::int32>(node->reconstruct(domain), domain);   
    std::cout << "\n";
  });

  return 0;
}