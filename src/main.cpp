#include "morphotree/core/alias.hpp"
#include "morphotree/core/point.hpp"
#include "morphotree/core/box.hpp"

#include "morphotree/adjacency/adjacency4c.hpp"
#include "morphotree/adjacency/adjacency8c.hpp"

#include "morphotree/tree/ct_builder.hpp"
#include "morphotree/tree/mtree.hpp"

#include "morphotree/core/io.hpp"

#include "morphotree/tree/treeOfShapes/KGrid.hpp"

#include <iostream>
#include <memory>

#include <algorithm>
#include <numeric>
#include <iomanip>

using namespace morphotree;


#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"


using tree_t = MorphologicalTree<uint8>;

// int main(int argc, char *argv[])
// {
//   // read image 
//   int width, height, nchannels;
//   uint8 *data = stbi_load("../image/noisy-apple.png", &width, &height, &nchannels, 1);

//   std::vector<uint8> f(data, data + (width*height));
//   Box domain = Box::fromSize(UI32Point{width, height});
//   std::shared_ptr<Adjacency> adj = std::make_shared<Adjacency8C>(domain);

//   tree_t tree = buildMinTree(f, adj);

//   std::vector<uint32> area(tree.numberOfNodes(), 0);

//   tree.tranverse([&area](tree_t::NodePtr node){
//     area[node->id()] += node->cnps().size();
//     if (node->parent() != nullptr) {
//       area[node->parent()->id()] += area[node->id()];
//     }
//   });

  
//   tree_t filtered_tree = tree.directFilter([&area](tree_t::NodePtr node){
//     return area[node->id()] > 50;
//   });

//   std::vector<uint8> filtered_image = filtered_tree.reconstructImage();
//   stbi_write_png("../image/output.png", domain.width(), domain.height(), 1, (void*)filtered_image.data(), 0);
//   stbi_image_free(data);

//   std::cout << "=========== DONE ================\n";

//   return 0;
// }

// int main(int argc, char *argv[])
// {
//   Box bx = Box::fromSize(I32Point{-3, -5}, UI32Point{7, 11});

//   std::cout << bx << "\n";

//   std::cout << "first element: " << bx.indexToPoint(0) << "\n";
//   std::cout << "middle element: " << bx.indexToPoint(bx.numberOfPoints()/2) << "\n";
//   std::cout << "last element: " << bx[bx.numberOfPoints()-1] << "\n\n";
 
//   std::cout << "================ SCAN ===============================\n\n";
  
//   for (ForwardBoxScan scan = bx.forwardBoxScan(); !scan.hasFinished(); scan.next()) {
//     I32Point p = scan.current();
//     std::cout << p << "\n";
//   }

//   std::cout << "================ 4C ADJACENCY ==========================\n";
//   std::unique_ptr<Adjacency> adj = std::make_unique<Adjacency4C>(bx);

//   std::cout << "4 neighbours of " << bx.indexToPoint(0) << ":\n";
//   for(uint32 n : adj->neighbours(0)) {
//     std::cout << bx.indexToPoint(n) << " ";
//   }
//   std::cout << "\n\n";
  
//   std::cout << "4 neighbours of " << bx.indexToPoint(bx.numberOfPoints()/2) << ":\n";
//   for(uint32 n : adj->neighbours(bx.numberOfPoints()/2)) {
//     std::cout << bx.indexToPoint(n) << " ";
//   }
//   std::cout << "\n\n";
  
//   std::cout << "4 neighbours of " << bx.indexToPoint(bx.numberOfPoints()-1) << ":\n";
//   for(uint32 n : adj->neighbours(bx.numberOfPoints()-1)) {
//     std::cout << bx.indexToPoint(n) << " ";
//   }
//   std::cout << "\n\n";

//   std::cout << "================ 8C ADJACENCY ==========================\n";
//   adj = std::make_unique<Adjacency8C>(bx);

//   std::cout << "4 neighbours of " << bx.indexToPoint(0) << ":\n";
//   for(uint32 n : adj->neighbours(0)) {
//     std::cout << bx.indexToPoint(n) << " ";
//   }
//   std::cout << "\n\n";
  
//   std::cout << "4 neighbours of " << bx.indexToPoint(bx.numberOfPoints()/2) << ":\n";
//   for(uint32 n : adj->neighbours(bx.numberOfPoints()/2)) {
//     std::cout << bx.indexToPoint(n) << " ";
//   }
//   std::cout << "\n\n";
  
//   std::cout << "4 neighbours of " << bx.indexToPoint(bx.numberOfPoints()-1) << ":\n";
//   for(uint32 n : adj->neighbours(bx.numberOfPoints()-1)) {
//     std::cout << bx.indexToPoint(n) << " ";
//   }
//   std::cout << "\n\n";


//   std::vector<uint8> f = {
//     0, 0, 0, 0, 0, 0, 0,
//     0, 4, 4, 4, 7, 7, 7, 
//     0, 7, 7, 4, 7, 4, 7,
//     0, 7, 4, 4, 7, 4, 7,
//     0, 4, 4, 4, 7, 4, 7,
//     0, 7, 7, 4, 7, 7, 7,
//     0, 0, 0, 0, 0, 0, 0 
//   };

//   Box domain = Box::fromSize(I32Point{0,0}, UI32Point{7,7});
//   adj = std::make_unique<Adjacency8C>(domain);

//   using TreeType = MorphologicalTree<uint8>;
//   using NodePtr = typename TreeType::NodePtr;

//   MorphologicalTree<uint8> tree = buildMinTree<uint8>(f, std::move(adj));

//   std::vector<uint32> area(tree.numberOfNodes(), 0);

//   tree.tranverse([&area](tree_t::NodePtr node){
//     area[node->id()] += node->cnps().size();
//     if (node->parent() != nullptr) {
//       area[node->parent()->id()] += area[node->id()];
//     }
//   });

//   tree.traverseByLevel([&domain, &area](NodePtr node) { 
//     std::cout << "area(" << node->id() << ") = " << area[node->id()] << "\n";
//     printImageIntoConsole(node->reconstruct(domain), domain);
//     std::cout << std::endl;
//   });

//   TreeType filteredTree = tree.directFilter([](NodePtr n){
//     return n->id() != 1 && n->id() != 2;
//   });

//   // tree.idirectFilter([](NodePtr n){
//   //   return n->id() != 1;
//   // });

//   // std::cout << "tree:" << std::endl;

//   // tree.traverseByLevel([&domain](NodePtr node) { 
//   //   printImageIntoConsole(node->reconstruct(domain), domain);
//   //   std::cout << std::endl;
//   // });

//   // std::cout << "reconstruct image:" << std::endl;

//   // printImageIntoConsoleWithCast<uint32>(tree.reconstructImage(), domain);

//   // std::cout << "filtered tree: " << std::endl; 

//   filteredTree.traverseByLevel([&domain](NodePtr node) { 
//     printImageIntoConsole(node->reconstruct(domain), domain);
//     std::cout << std::endl;
//   });

//   std::cout << "reconstruct image:" << std::endl;

//   printImageIntoConsoleWithCast<uint32>(filteredTree.reconstructImage(), domain);

//   return 0;
// }


int main(int argc, char *argv[]) 
{
  // std::vector<uint8> f = {
  //   4, 4, 4, 4, 4, 4,
  //   4, 7, 7, 0, 0, 4,
  //   4, 7, 4, 4, 0, 4,
  //   4, 7, 4, 4, 0, 4,
  //   4, 7, 7, 0, 0, 4,
  //   4, 4, 4, 4, 4, 4
  // };

    std::vector<uint8> f = {
      4, 4, 4, 4, 4, 4,
      4, 4, 4, 4, 4, 4,
      4, 4, 0, 7, 4, 4,
      4, 4, 7, 0, 4, 4,
      4, 4, 4, 4, 4, 4,
      4, 4, 4, 4, 4, 4
  };

  Box domain = Box::fromSize(UI32Point{6, 6});

  using KGridType = KGrid<uint8>; 

  KGridType grid{domain, f};

  Box gdomain = grid.immerseDomain();
  
  std::cout << grid;

  // for (uint32 y = gdomain.top(); y <= gdomain.bottom(); y++) {
  //   for (uint32 x = gdomain.left(); x <= gdomain.right(); x++) {
  //     KGridType::IntervalType i = grid.interval(x, y);
  //     std::cout << "[" << (int)i.min() << ", " << (int)i.max() << "], ";
  //   }
  //   std::cout << "\n";
  // }
}