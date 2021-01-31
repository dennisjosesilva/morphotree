#pragma once

#include "morphotree/adjacency/adjacency.hpp"
#include "morphotree/core/box.hpp"
#include "morphotree/core/point.hpp"

namespace morphotree 
{

  enum class DiagonalConnection : int32
  {
    None = 1,
    SW = 2,
    NE = 4,
    SE = 8,
    NW = 16
  };

  constexpr DiagonalConnection operator|(DiagonalConnection dconn1, DiagonalConnection dconn2) 
  {
    return static_cast<DiagonalConnection>(static_cast<int32>(dconn1) | static_cast<int32>(dconn2));
  }

  constexpr bool operator&(DiagonalConnection dconn1, DiagonalConnection dconn2)
  {
    return static_cast<bool>(static_cast<int32>(dconn1) & static_cast<int32>(dconn2));
  }

  class AdjacencyUC : public Adjacency
  {
  public:
    AdjacencyUC(Box imgdomain);

    std::vector<uint32> neighbours(uint32 v) const;

    inline DiagonalConnection& dconn(uint32 v) { return dconn_[v]; }
    inline DiagonalConnection  dconn(uint32 v) const { return dconn_[v]; }
    inline void dconn(uint32 v, DiagonalConnection dconn) { dconn_[v] = dconn_[v] | dconn; }

  protected:
    Box domain_;
    std::array<I32Point, 4> offset_;
    std::vector<DiagonalConnection> dconn_;
  };
}