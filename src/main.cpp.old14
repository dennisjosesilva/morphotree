#include "morphotree/core/box.hpp"
#include "morphotree/tree/treeOfShapes/order_image.hpp"
#include <iostream>
#include <vector>
#include "morphotree/core/io.hpp"

int main(int argc, char *argv[])
{
  using morphotree::Box;
  using morphotree::I32Point;
  using morphotree::UI32Point;
  using morphotree::uint8;
  using morphotree::OrderImageResult;
  using morphotree::computeOrderImage;
  using morphotree::KGrid;
  using morphotree::printImageIntoConsole;

  // Box box = Box::fromCorners(I32Point{-1,-1}, I32Point{1,1});

  Box domain = Box::fromSize(UI32Point{4,4});
  std::vector<uint8> f = {
    3, 3, 3, 3,
    3, 0, 8, 3,
    3, 9, 4, 3,
    3, 3, 3, 3
  };

  KGrid<uint8> F{domain, f};
  OrderImageResult<uint8> r = 
    computeOrderImage(domain, f, F);

  printImageIntoConsole(r.orderImg, r.domain);

  // std::cout << " ============= FORWARD SCAN ===============\n";
  // for (const I32Point &p : box.forwardBoxScan())
  // {
  //   std::cout << p <<std::endl;
  // }

  // std::cout << " ============= BACKWARD SCAN ===============\n";
  // for (const I32Point &p : box.backwardBoxScan())
  // {
  //   std::cout << p <<std::endl;
  // }

  return 0;
}