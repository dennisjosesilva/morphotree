#include "morphotree/adjacency/adjacencyuc.hpp"

namespace morphotree
{
  AdjacencyUC::AdjacencyUC(Box imgdomain)
    :domain_{imgdomain},
     offset_{I32Point{-1,0}, I32Point{0,-1}, I32Point{1,0}, I32Point{}}
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

    if (dconn_[v] & DiagonalConnection::NE) {
      I32Point r = p + I32Point{1, -1};
      if (domain_.contains(r)) 
        n.push_back(domain_.pointToIndex(r));
    }
    if (dconn_[v] & DiagonalConnection::NW) {
      I32Point r = p + I32Point{-1, -1};
      if (domain_.contains(r))
        n.push_back(domain_.pointToIndex(r));
    }
    if (dconn_[v] & DiagonalConnection::SW) {
      I32Point r = p + I32Point{-1, +1};
      if (domain_.contains(r))
        n.push_back(domain_.pointToIndex(r));
    }
    if (dconn_[v] & DiagonalConnection::SE) {
      I32Point r = p + I32Point{ +1, +1};
      if (domain_.contains(r)) 
        n.push_back(domain_.pointToIndex(r));
    }

    return n;
  }
}