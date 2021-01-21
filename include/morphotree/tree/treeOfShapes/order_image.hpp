#pragma once 

#include <vector>

#include "morphotree/core/box.hpp"
#include "morphotree/core/alias.hpp"
#include "morphotree/core/hqueue.hpp"
#include "morphotree/tree/treeOfShapes/kgrid.hpp"

namespace morphotree {
  struct OrderImageResult
  {
    std::vector<uint32> orderImg;
    std::vector<uint32> R;
    Box domain; 
  };


  template<class ValueType>
  OrderImageResult computeOrderImage(
    const Box &domain,
    const std::vector<ValueType> &f,
    const KGrid<ValueType> &F,
    const I32Point &pInfinity = I32Point{0,0});



  // ====================================== [ IMPLEMENTATION ] ============================================
  template<class ValueType>
  OrderImageResult computeOrderImage(
    const Box &domain,
    const std::vector<ValueType> &f,
    const KGrid<ValueType> &F,
    const I32Point &pInfinity)
  {
    const uint32 UNPROCESSED = std::numeric_limits<uint32>::max();
    const uint32 PROCESSED = std::numeric_limits<uint>::max()-1;

    HQueue<ValueType, uint32> Q;
    std::vector<uint32> R;
    std::vector<uint32> ord;

    


  }  

}
