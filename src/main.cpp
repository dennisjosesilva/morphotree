#include "morphotree/core/alias.hpp"
#include "morphotree/core/point.hpp"
#include "morphotree/core/box.hpp"

#include "morphotree/adjacency/adjacency4c.hpp"
#include "morphotree/adjacency/adjacency8c.hpp"

#include "morphotree/tree/ct_builder.hpp"
#include "morphotree/tree/mtree.hpp"

#include "morphotree/core/io.hpp"

#include "morphotree/tree/treeOfShapes/kgrid.hpp"
#include "morphotree/core/hqueue.hpp"

#include "morphotree/tree/treeOfShapes/order_image.hpp"
#include "morphotree/tree/treeOfShapes/tos.hpp"

#include "morphotree/attributes/smoothnessContourComputer.hpp"


#include <iostream>
#include <memory>

#include <algorithm>
#include <numeric>
#include <iomanip>
#include <functional>

#include "morphotree/tree/treeOfShapes/order_image.hpp"
#include "morphotree/attributes/attributeComputer.hpp"
#include "morphotree/attributes/areaComputer.hpp"
#include "morphotree/attributes/perimeterComputer.hpp"
#include "morphotree/attributes/bitquads/quadCountComputer.hpp"
#include "morphotree/attributes/bitquads/quadCountTreeOfShapesComputer.hpp"
#include "morphotree/attributes/volumeComputer.hpp"


#include "morphotree/filtering/globalOptimiser/GlobalOptimisationFilter.hpp"
#include "morphotree/filtering/globalOptimiser/MinCPerimeterWithMSE.hpp"

#include <map>

using namespace morphotree;


// #define STB_IMAGE_IMPLEMENTATION
// #include "stb_image.h"

// #define STB_IMAGE_WRITE_IMPLEMENTATION
// #include "stb_image_write.h"


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


// int main(int argc, char *argv[]) 
// {
//   std::vector<uint8> f = {
//     4, 4, 4, 4, 4, 4,
//     4, 7, 7, 0, 0, 4,
//     4, 7, 4, 4, 0, 4,
//     4, 7, 4, 4, 0, 4,
//     4, 7, 7, 0, 0, 4,
//     4, 4, 4, 4, 4, 4
//   };

//   //   std::vector<uint8> f = {
//   //     4, 4, 4, 4, 4, 4,
//   //     4, 4, 4, 4, 4, 4,
//   //     4, 4, 0, 7, 4, 4,
//   //     4, 4, 7, 0, 4, 4,
//   //     4, 4, 4, 4, 4, 4,
//   //     4, 4, 4, 4, 4, 4
//   // };

//   Box domain = Box::fromSize(UI32Point{6, 6});

//   using KGridType = KGrid<uint8>; 

//   KGridType grid{domain, f};

//   // Box gdomain = grid.immerseDomain();
  
//   //std::cout << grid;

//   // std::map<int32, char> q;
//   // std::map<int32, char>::iterator lower, upper;

//   // q[0] = 'a';
//   // q[1] = 'b';
//   // q[2] = 'c';

//   // q[10] = 'd';
//   // q[12] = 'f'; 
  
//   // lower = q.lower_bound(8);
//   // upper = q.upper_bound(8);

//   // std::cout << "lower = (" << lower->first << ", " << lower->second << ")" << std::endl;
//   // std::cout << "lower = (" << upper->first << ", " << upper->second << ")" << std::endl;


//   // HQueue<uint8, int32> hqueue;

//   // hqueue.insert(5, 0);
//   // hqueue.insert(5, 1);
//   // hqueue.insert(5, 2);
//   // hqueue.insert(5, 3);

//   // hqueue.insert(2, 3);
//   // hqueue.insert(2, 5);

//   // hqueue.insert(9, 1);
//   // hqueue.insert(9, 2);

//   // std::cout << hqueue.pop(5) << " " << hqueue.pop(3) <<  " " << hqueue.pop(2) << " " << hqueue.pop(2) << std::endl;

//   OrderImageResult<uint8> result = computeOrderImage(domain, f, grid); 

//   std::vector<uint32> ord = result.orderImg;
//   Box Fdomain = result.domain;
  
//   for (uint32 y = Fdomain.top(); y <= Fdomain.bottom(); y++) {
//     for (uint32 x = Fdomain.left(); x <= Fdomain.right(); x++) {
//       std::cout << ord[Fdomain.pointToIndex(x, y)] << " ";
//     }
//     std::cout << "\n";
//   }

//   std::cout << "\nemerged set\n" ;

//   std::vector<I32Point> pts {
//     I32Point{0,0}, I32Point{0,1}, I32Point{0, 2}, I32Point{0, 3},
//     I32Point{1,2}, I32Point{1,3}, I32Point{1, 4}, I32Point{2, 4}
//   };

//   std::vector<I32Point> emergedPts = grid.emergeSet(pts);

//   for (const I32Point &p : emergedPts) {
//     std::cout << p << std::endl;
//   }


//   using MorphoTree = MorphologicalTree<uint8>;
//   MorphoTree etos = buildEnlargedTreeOfShapes(domain, f);

//   etos.tranverse([&grid](MorphoTree::NodePtr node) {
//     printImageIntoConsole(node->reconstruct(grid.immerseDomain()), grid.immerseDomain());
//     std::cout << std::endl;
//   });

//   printImageIntoConsoleWithCast<int32>(etos.reconstructImage(), grid.immerseDomain());
//   std::cout << std::endl;
//   std::cout << "emerged Tree of Shapes." << std::endl;

//   // tree of shapes
//   MorphoTree tos = buildTreeOfShapes(domain, f);
  
//   tos.tranverse([&domain](MorphoTree::NodePtr node) {
//     printImageIntoConsole(node->reconstruct(domain), domain);
//     std::cout << std::endl;
//   });

//   printImageIntoConsoleWithCast<int32>(tos.reconstructImage(), domain);


//   return 0;
// }



// int main(int argc, char *argv[])
// {
//   using MorphoTree = MorphologicalTree<uint8>;
//   using Grid = KGrid<uint8>;

//   int width, height, nchannels;
//   uint8 *data = stbi_load("/mnt/HDD-Ubuntu/documents/phd/code/simple-morph-tree/image/Zuckerberg.pgm", &width, &height, &nchannels, 1);

//   std::vector<uint8> f(data, data + (width*height));
//   Box domain = Box::fromSize(UI32Point{width, height});

  
//   Grid F{domain, f};
//   Box Fdomain = F.immerseDomain();

//   OrderImageResult<uint8> res = computeOrderImage(domain, f, F);

//   MorphoTree tree = buildEnlargedTreeOfShapes(domain, f, F);

//   tree.tranverse([&F,&Fdomain](MorphoTree::NodePtr node){ 
//     bool hasCNP = false;
//     for (uint32 cnp : node->cnps()) {
//       if (F.isZeroFace(Fdomain.indexToPoint(cnp))) {
//         hasCNP = true;
//         break;
//       }
//     }
//     if (!hasCNP) {
//       std::cout << "node #" << node->id() << std::endl;
//       for (uint32 p : node->cnps()) {
//         std::cout << "\t" << Fdomain.indexToPoint(p) << "\n";
//       }
//     }
//   });

//   // std::vector<uint8> filtered_image = tree.reconstructImage();
//   // stbi_write_png("../image/output.png", domain.width(), domain.height(), 1, (void*)filtered_image.data(), 0);
//   //stbi_write_png("../image/output.png", Fdomain.width(), Fdomain.height(), 1, (void*)res.orderImg.data(), 0);
//   // stbi_image_free(data);

//   std::cout << "\nimage window: \n";


//   std::vector<uint32> orderWindow;
//   Box window = Box::fromCorners(I32Point(492, 468), I32Point(494, 470));
//   for (int y = window.top(); y <= window.bottom(); y++) {
//     for (int x = window.left(); x <= window.right(); x++) {
//       orderWindow.push_back(res.orderImg[Fdomain.pointToIndex(x, y)]);
//     }
//   }

//   printImageIntoConsoleWithCast<uint32>(orderWindow, window);
  
//   I32Point p{493, 469};
//   Box fwindowBox = Box::fromCorners(F.emergePoint(I32Point(492, 468)), F.emergePoint(I32Point(494, 470)));
//   std::vector<uint8> fwindow = {
//     f[domain.pointToIndex(F.emergePoint(p +  I32Point{-1,-1}))], f[domain.pointToIndex( F.emergePoint(p +  I32Point{ 1,-1}))],
//     f[domain.pointToIndex(F.emergePoint(p +  I32Point{-1, 1}))], f[domain.pointToIndex( F.emergePoint(p +  I32Point{ 1, 1}))]
//   };
  
//   printImageIntoConsoleWithCast<int>(fwindow, fwindowBox);

//   std::cout << std::endl << std::endl;
//   printImageIntoConsoleWithCast<int>(f, Box::fromCorners(F.emergePoint(I32Point(44, 512)), F.emergePoint(I32Point(46, 514))));


//   for (uint32 n : F.adj()->neighbours(Fdomain.pointToIndex(I32Point(492, 470)))) {
//     std::cout << Fdomain.indexToPoint(n);
//   }

//   std::cout << "=========== DONE ================\n";

//   return 0;
// }


int main(int argc, char *argv[]) 
{
  std::vector<uint8> f = {
    0, 0, 0, 0, 0, 0, 0,
    0, 4, 4, 4, 7, 7, 7, 
    0, 7, 7, 4, 7, 4, 7,
    0, 7, 4, 4, 7, 4, 7,
    0, 4, 4, 4, 7, 4, 7,
    0, 7, 7, 4, 7, 7, 7,
    0, 0, 0, 0, 0, 0, 0 
  };

  // std::vector<uint8> f = {
  //     3, 3, 3, 3, 3, 3, 3,
  //     3, 4, 4, 4, 4, 4, 3, 
  //     3, 4, 2, 2, 2, 4, 3,
  //     3, 4, 2, 4, 2, 4, 3,
  //     3, 4, 4, 2, 2, 4, 3,
  //     3, 4, 4, 4, 4, 4, 3,
  //     3, 3, 3, 3, 3, 3, 3 
  // };

  // std::vector<uint8> f = {
  //     3, 3, 3, 3, 3, 3, 3,
  //     3, 4, 4, 4, 4, 4, 3, 
  //     3, 4, 7, 4, 7, 4, 3,
  //     3, 4, 7, 4, 7, 4, 3,
  //     3, 4, 4, 7, 7, 4, 3,
  //     3, 4, 4, 4, 4, 4, 3,
  //     3, 3, 3, 3, 3, 3, 3 
  // };



  Box domain = Box::fromSize(I32Point{0,0}, UI32Point{7,7});

  //Box::SubBox subDomain = domain.subBoxFromSize(I32Point{6,2}, UI32Point{3,3});

  // printImageIntoConsoleWithCast<int32>(f, domain);
  // std::cout << std::endl;
  // printImageIntoConsoleWithCast<int32>(f, subDomain);


  // KGrid<uint8> F{domain, f};
  // OrderImageResult<uint8> r = computeOrderImage(domain, f, F);   
  
  // Box idomain = F.immerseDomain();

  // Box::SubBox subDomain = idomain.subBoxFromSize(I32Point{6,2}, UI32Point{3,3});

  // printImageIntoConsoleWithCast<int32>(r.orderImg, idomain);
  // std::cout << std::endl;
  // printImageIntoConsoleWithCast<int32>(r.orderImg, subDomain);

  // WindowMaxTree wtree{r.orderImg, F.adj()};
  // wtree.build(subDomain);

  // std::cout << "\n";
  // wtree.tranverse([](const WindowMaxTreeNode &node) {
  //   std::cout << "id: " << (int)node.id() << ", size: " << (int)node.size() << "\n";
  // });

  // MorphologicalTree<uint8> etos = buildEnlargedTreeOfShapes(r, F);
  // std::unique_ptr<AttributeComputer<Quads, uint8>> quadsComp = 
  //   std::make_unique<TreeOfShapesQuadCountsComputer<uint8>>(F, r.orderImg);

  // std::vector<Quads> quads = quadsComp->computeAttribute(etos);

  // MorphologicalTree<uint8> tos = emergeTreeOfShapes(F, etos);

  // using TreeType = decltype(tos);

  // tos.tranverse([&quads, &domain](TreeType::NodePtr node) {
  //   std::cout << "id: " << node->id() << std::endl;
  //   std::cout << "Q1: " << quads[node->id()].q1() << std::endl;
  //   std::cout << "Q2: " << quads[node->id()].q2() << std::endl;
  //   std::cout << "Q3: " << quads[node->id()].q3() << std::endl;
  //   std::cout << "QD: " << quads[node->id()].qd() << std::endl;
  //   std::cout << "Q4: " << quads[node->id()].q4() << std::endl;
    
  //   printImageIntoConsoleWithCast<int32>(node->reconstruct(domain), domain);
  //   std::cout << std::endl;
  // });

  // std::cout << "Finished." << std::endl;

  // Box::SubBox wDomain = Fdomain.subBoxFromSize(I32Point{8,2}, UI32Point{3,3});
  // std::cout << "\n1st: 3 x 3 window" << std::endl;
  // printImageIntoConsole(r.orderImg, wDomain);

  // WindowMaxTree wtree{r.orderImg, F.adj()};

  // wtree.build(wDomain);
  // wtree.tranverse([&wDomain](const WindowMaxTreeNode &node){ 
  //    std::cout << "area[" << wDomain.indexToPoint(node.representative()) << "] = " << (int)node.size() << "\n"; 
  // });
  
  // Box::SubBox wDomain2 = Fdomain.subBoxFromSize(I32Point{2,4}, UI32Point{3,3});
  // std::cout << "\n2nd: 3 x 3 window" << std::endl;
  // printImageIntoConsole(r.orderImg, wDomain2);
  // wtree.build(wDomain2);
  // wtree.tranverse([&wDomain2](const WindowMaxTreeNode &node){ 
  //    std::cout << "area[" << wDomain2.indexToPoint(node.representative()) << "] = " << (int)node.size() << "\n"; 
  // });
  
  // std::unique_ptr<Adjacency> adj = std::make_unique<Adjacency8C>(domain);
  // std::unique_ptr<AttributeComputer<uint32, uint8>> areaComputer = std::make_unique<AreaComputer<uint8>>();
  // // std::unique_ptr<AttributeComputer<uint32, uintnode8>> perimeterComputer 
  // //   = std::make_unique<MaxTreePerimeterComputer<uint8>>(domain, f);
  // std::unique_ptr<AttributeComputer<uint32, uint8>> perimeterComputer
  //   = std::make_unique<MinTreePerimeterComputer<uint8>>(domain, f);
  // std::unique_ptr<AttributeComputer<Quads, uint8>> quadsComputer = 
  //   std::make_unique<CTreeQuadCountsComputer<uint8>>(domain, f, "../resource/quads/dt-min-tree-8c.dat");
  

  // using TreeType = MorphologicalTree<uint8>;
  // using NodePtr = typename TreeType::NodePtr;

  // MorphologicalTree<uint8> tree = buildMinTree<uint8>(f, std::move(adj));
  //MorphologicalTree<uint8> tree = buildMinTree<uint8>(f, std::move(adj));

  // std::vector<Quads> quads = quadsComputer->computeAttribute(tree);

  // std::vector<uint32> area = areaComputer->initAttributes(tree);
  // std::vector<uint32> perimeter = perimeterComputer->initAttributes(tree);
  // tree.tranverse([&area, &perimeter, &perimeterComputer, &areaComputer](NodePtr node){ 
  //   areaComputer->computeInitialValue(area, node);
  //   perimeterComputer->computeInitialValue(perimeter, node);

  //   if (node->parent() != nullptr){
  //     areaComputer->mergeToParent(area, node, node->parent());
  //     perimeterComputer->mergeToParent(perimeter, node, node->parent());
  //   }
  // });
  
  // tree.traverseByLevel([&area, &perimeter, &domain](NodePtr node) {
  //    std::cout << "area[" << node->id() << "] = " << area[node->id()] << "\n";
  //    std::cout << "perimeter[" << node->id() << "] = " << perimeter[node->id()] << "\n";

  //    printImageIntoConsoleWithCast<int32>(node->reconstruct(domain), domain);
  //    std::cout << "\n";
  //  });

  //  tree.traverseByLevel([&quads, &domain](NodePtr node) {
  //    std::cout << "area[" << node->id() << "] = " << quads[node->id()].area() << "\n";
  //    std::cout << "perimeter[" << node->id() << "] = " << quads[node->id()].perimeter() << "\n";

  //   // std::cout << "q1: " << quads[node->id()].q1() << ", q2: " << quads[node->id()].q2() << ", q3: " << quads[node->id()].q3()
  //   //   << ", q4: " << quads[node->id()].q4() << ", qd: " << quads[node->id()].qd() << std::endl;

  //    printImageIntoConsoleWithCast<int32>(node->reconstruct(domain), domain);
  //    std::cout << "\n";
  //  });
  
  std::unique_ptr<Adjacency> adj = std::make_unique<Adjacency8C>(domain);
  MorphologicalTree<uint8> tree = buildMaxTree(f, std::move(adj));
  // //MorphologicalTree<uint8> tree = buildMinTree(f, std::move(adj));
  // MorphologicalTree<uint8> tree = buildTreeOfShapes(domain, f);

  // // std::unique_ptr<AttributeComputer<float, uint8>> scComputer = 
  // //   std::make_unique<MinTreeSmoothnessContourComputer<uint8>>(domain, f);
  // std::unique_ptr<AttributeComputer<float, uint8>> scComputer = 
  //   std::make_unique<TreeOfShapesSmoothnessContourComputer<uint8>>(domain, f);

  // std::vector<float> sc = scComputer->computeAttribute(tree);

  using MTree = decltype(tree);
  using NodePtr = typename MTree::NodePtr;

  // tree.tranverse([&sc, &f, &domain](const NodePtr node) {
  //   std::cout << "node id: " << node->id() << std::endl;
  //   std::cout << "smoothness: " << sc[node->id()] << std::endl;
  //   printImageIntoConsoleWithCast<int32>(node->reconstruct(domain) , domain);
  // });

  // std::vector<float> volume = std::make_unique<MaxTreeVolumeComputer<uint8>>()->computeAttribute(tree);

  // tree.traverseByLevel([&volume, &domain](NodePtr node) {
  //   std::cout << "V(" << node->id() << ") = " << volume[node->id()] << "\n";
  //   printImageIntoConsoleWithCast<int32>(node->reconstruct(domain), domain);
  //   std::cout << "\n";
  // });

  MinCPerimeterWithMSE<MTree::TreeWeightType> filter{domain, f, 
    "../resource/quads/dt-max-tree-8c.dat", tree, 50.f};

  std::vector<Quads> quads = CTreeQuadCountsComputer<uint8>(domain, f, 
    "../resource/quads/dt-max-tree-8c.dat").computeAttribute(tree);

  // tree.traverseByLevel([&f, &domain, &quads](NodePtr node){
  //   std::cout << "node.id = " << node->id() << std::endl;
  //   std::cout << "cperimeter = " << quads[node->id()].continuousPerimeter() << "\n";
  //   printImageIntoConsoleWithCast<int32>(node->reconstruct(domain), domain);
  //   std::cout << std::endl;
  // });

  MTree ftree = filter.filter(155.f);

  std::cout << "perimeter: " << filter.criterion() << std::endl;
  std::cout << "squared error: " << filter.constraint() << std::endl; 

  ftree.traverseByLevel([&f, &domain, &quads](NodePtr node){
    std::cout << "node.id = " << node->id() << std::endl;
    std::cout << "cperimeter = " << quads[node->id()].continuousPerimeter() << "\n";
    printImageIntoConsoleWithCast<int32>(node->reconstruct(domain), domain);
    std::cout << std::endl;
  });
}