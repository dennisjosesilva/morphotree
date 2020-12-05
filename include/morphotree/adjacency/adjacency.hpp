#pragma once

#include <morphotree/core/alias.hpp>
#include <vector>

namespace morphotree
{
  class Adjacency
  {
  public:
    virtual std::vector<uint32> neighbours(uint32 v) const = 0;
  };
}