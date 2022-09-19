#pragma once 

#include "morphotree/core/alias.hpp"
#include "morphotree/core/box.hpp"
#include "morphotree/adjacency/adjacency.hpp"

#include <vector>
#include <array>

namespace morphotree
{
  class Adjacency4C : public Adjacency
  {
  public:
    Adjacency4C(Box imgdomain);

    std::vector<uint32> neighbours(uint32 v) const;

  protected:
    Box domain_;
    std::array<I32Point, 4> offset_;
  };

  class InfAdjacency4C : public Adjacency
  {
  public:
    InfAdjacency4C(Box imgdomain);
    std::vector<uint32> neighbours(uint32 v) const;

  protected:
    Box domain_;
    std::array<I32Point, 4> offset_;
  };
}