#include <iostream>
#include <vector>

#include "morphotree/tree/mtree.hpp"
#include "morphotree/adjacency/adjacency8c.hpp"

#include "morphotree/core/io.hpp"
#include "morphotree/core/alias.hpp"
#include "morphotree/core/box.hpp"

#include "morphotree/attributes/volumeComputer.hpp"

using namespace morphotree;

int main(int argc, char *argv[])
{
  using MTree = MorphologicalTree<uint8>;
  using NodePtr = typename MTree::NodePtr;

  using MyAttributeComputer = AttributeComputer<float, uint8>;
  using MyVolumeComputer = VolumeComputer<uint8>;


  std::vector<uint8> f = {
    0, 0, 0, 0, 0, 0, 0,
    0, 4, 4, 4, 7, 7, 7,
    0, 7, 7, 4, 7, 4, 7,
    0, 7, 4, 4, 7, 4, 7,
    0, 4, 4, 4, 7, 4, 7,
    0, 7, 7, 4, 7, 7, 7,
    0, 0, 0, 0, 0, 0, 0};

  Box domain = Box::fromSize(UI32Point{7, 7});

  MTree tree = buildMinTree(f, std::make_shared<Adjacency8C>(domain));

  std::unique_ptr<MyAttributeComputer> attrComp = 
    std::make_unique<MyVolumeComputer>();

  std::vector<float> vol = 
    attrComp->computeAttribute(tree);
  
  tree.traverseByLevel([=](NodePtr node) {
    std::cout << "vol(" << node->id() << ") = " << vol[node->id()] << "\n";
    printImageIntoConsoleWithCast<int>(node->reconstruct(domain), domain);
    std::cout << "\n";
  });
}