#include "morphotree/adjacency/adjacency8c.hpp"

namespace morphotree
{
  Adjacency8C::Adjacency8C(Box imgdomain)
    :domain_{imgdomain},
     offset_{ I32Point{-1,-1}, I32Point{0,-1}, I32Point{1,-1},
              I32Point{-1,0},                  I32Point{1,0},
              I32Point{-1,1},  I32Point{0,1},  I32Point{1,1}}
  {}

  std::vector<uint32> Adjacency8C::neighbours(uint32 v) const 
  {
    std::vector<uint32> neighbours;
    I32Point p = domain_.indexToPoint(v);
    for (const I32Point &q : offset_) {
      I32Point n = p + q;
      if (domain_.contains(n)) {
        neighbours.push_back(domain_.pointToIndex(n));
      }
    }
    return neighbours;
  }

  InfAdjacency8C::InfAdjacency8C(Box imgdomain)
    :domain_{imgdomain},
     offset_{ I32Point{-1,-1}, I32Point{0,-1}, I32Point{1,-1},
              I32Point{-1,0},                  I32Point{1,0},
              I32Point{-1,1},  I32Point{0,1},  I32Point{1,1}}
  {}

  std::vector<uint32> InfAdjacency8C::neighbours(uint32 v) const 
  {
    std::vector<uint32> neighbours;
    I32Point p = domain_.indexToPoint(v);
    for (const I32Point &q : offset_) {
      I32Point n = p + q;
      neighbours.push_back(domain_.pointToIndex(n));
    }
    return neighbours;
  }
}