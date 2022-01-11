#include "morphotree/core/io.hpp"

#include "morphotree/tree/ct_builder.hpp"
#include "morphotree/tree/mtree.hpp"
#include "morphotree/adjacency/adjacency8c.hpp"

int main(int argc, char *argv[])
{
  using morphotree::Box;
  using morphotree::uint8;
  using morphotree::uint32;
  using morphotree::UI32Point;
  using morphotree::Adjacency8C;
  using morphotree::buildMaxTree;
  using MTree = morphotree::MorphologicalTree<uint8>;
  using NodePtr = typename MTree::NodePtr;
  using morphotree::printImageIntoConsoleWithCast;

  using Image = std::vector<uint8>;
  using BinImage = std::vector<bool>;

  // define image
  // ------------
  Image f = {
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 4, 4, 4, 0, 4, 4, 4, 4, 0,
    0, 4, 4, 4, 0, 4, 7, 9, 4, 0,
    0, 4, 4, 4, 0, 4, 4, 4, 4, 0,
    0, 4, 4, 4, 0, 4, 7, 7, 4, 0,
    0, 4, 4, 4, 0, 4, 7, 7, 4, 0,
    0, 4, 4, 4, 0, 4, 4, 4, 4, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0 
  };
  Box domain = Box::fromSize(UI32Point{10, 8});

  // build max-tree
  // --------------
  MTree tree = buildMaxTree(f, std::make_shared<Adjacency8C>(domain));

  tree.traverseByLevel([&domain](NodePtr node){
    BinImage bimg =  node->reconstruct(domain);
    std::cout << "node.id= " << node->id() << "\n";
    printImageIntoConsoleWithCast<int>(bimg, domain);
  });
  std::cout << "\n";

  // remove nodes
  // ------------ 
  //                        0,     1,      2,    3,     4,     5 
  std::vector<bool> keep = {false, false, true, true, false, true};

  BinImage bimg = tree.reconstructNodes([&keep](NodePtr node){ return keep[node->id()]; }, 
    domain);

  std::cout << "\n\n----------------- BINARY IMAGE ------------------------\n";
  printImageIntoConsoleWithCast<int>(bimg, domain);

  return 0;
}