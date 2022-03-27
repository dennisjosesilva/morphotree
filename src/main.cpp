#include "morphotree/core/box.hpp"
#include <iostream>

int main(int argc, char *argv[])
{
  using morphotree::Box;
  using morphotree::I32Point;
  using morphotree::UI32Point;

  Box box = Box::fromCorners(I32Point{-1,-1}, I32Point{1,1});

  
  std::cout << " ============= FORWARD SCAN ===============\n";
  for (const I32Point &p : box.forwardBoxScan())
  {
    std::cout << p <<std::endl;
  }

  std::cout << " ============= BACKWARD SCAN ===============\n";
  for (const I32Point &p : box.backwardBoxScan())
  {
    std::cout << p <<std::endl;
  }

  return 0;
}