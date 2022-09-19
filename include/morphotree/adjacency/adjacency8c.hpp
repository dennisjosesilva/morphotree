#pragma once

#include "morphotree/core/alias.hpp"
#include "morphotree/core/box.hpp"
#include "morphotree/adjacency/adjacency.hpp"

#include <vector>
#include <array>

namespace morphotree
{
  class Adjacency8C : public Adjacency
  {
  public:
    Adjacency8C(Box imgdomain);

    std::vector<uint32> neighbours(uint32 v) const;

  protected:
    Box domain_;  
    std::array<I32Point, 8> offset_;
  };

  class InfAdjacency8C : public Adjacency
  {
  public:
    InfAdjacency8C(Box imgdomain);
    std::vector<uint32> neighbours(uint32 v) const;
  
  protected:
    Box domain_;
    std::array<I32Point, 8> offset_;
  };
}