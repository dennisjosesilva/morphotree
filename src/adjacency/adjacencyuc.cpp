#include "morphotree/adjacency/adjacencyuc.hpp"

namespace morphotree
{
  AdjacencyUC::AdjacencyUC(Box imgdomain)
    :domain_{imgdomain},
     offset_{I32Point{-1,0}, I32Point{0,-1}, I32Point{1,0}, I32Point{0, 1}}
  {
    dconn_.resize(domain_.numberOfPoints(), DiagonalConnection::None);
  }

  std::vector<uint32> AdjacencyUC::neighbours(uint32 v) const
  {
    std::vector<uint32> n;
    n.reserve(4);

    I32Point p = domain_.indexToPoint(v);
    for (const I32Point &q : offset_) {
      I32Point r = p + q;
      if (domain_.contains(r))
        n.push_back(domain_.pointToIndex(p + q));
    }

    DiagonalConnection curDconn = dconn_[v];
    if ((curDconn & DiagonalConnection::NE)) {
      I32Point r = p + I32Point{1, -1};
      if (domain_.contains(r)) 
        n.push_back(domain_.pointToIndex(r));
    }
    if ((curDconn & DiagonalConnection::NW)) {
      I32Point r = p + I32Point{-1, -1};
      if (domain_.contains(r))
        n.push_back(domain_.pointToIndex(r));
    }
    if ((curDconn & DiagonalConnection::SW)) {
      I32Point r = p + I32Point{-1, +1};
      if (domain_.contains(r))
        n.push_back(domain_.pointToIndex(r));
    }
    if ((curDconn & DiagonalConnection::SE)) {
      I32Point r = p + I32Point{ +1, +1};
      if (domain_.contains(r)) 
        n.push_back(domain_.pointToIndex(r));
    }

    return n;
  }
}