#include <iostream>
#include <vector>

#include "morphotree/tree/mtree.hpp"
#include "morphotree/adjacency/adjacency8c.hpp"

#include "morphotree/core/io.hpp"
#include "morphotree/core/alias.hpp"
#include "morphotree/core/box.hpp"

using namespace morphotree;


int main(int argc, char *argv[]) 
{
  using MTree = MorphologicalTree<uint8>;
  using NodePtr = typename MTree::NodePtr;

  std::vector<uint8> f = {
    0, 0, 0, 0, 0, 0, 0,
    0, 4, 4, 4, 7, 7, 7,
    0, 7, 7, 4, 7, 4, 7,
    0, 7, 4, 4, 7, 4, 7,
    0, 4, 4, 4, 7, 4, 7,
    0, 7, 7, 4, 7, 7, 7,
    0, 0, 0, 0, 0, 0, 0};
  
  Box domain = Box::fromSize(UI32Point{7, 7});

  MTree tree = buildMaxTree(f, std::make_shared<Adjacency8C>(domain));

  tree.tranverse([&domain](NodePtr node) {
    std::cout << "id= " << node->id() << "\n";
    printImageIntoConsoleWithCast<int32>(node->reconstruct(domain), domain);
  });

  std::vector<uint8> frec = tree.reconstructImage();
  std::vector<uint8> frecWithMask = tree.reconstructImage([](NodePtr node){
    return !(node->id() == 1 || node->id() == 3);
  });

  std::cout << "\nRECONSTRUCTED IMAGE\n";
  printImageIntoConsoleWithCast<int>(frec, domain);

  std::cout << "\nRECONSTRUCTED FILTERED IMAGE\n";
  printImageIntoConsoleWithCast<int>(frecWithMask, domain);

  return 0;
}