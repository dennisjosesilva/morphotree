#pragma once 

#include <vector>

#include "morphotree/core/box.hpp"
#include "morphotree/core/alias.hpp"
#include "morphotree/core/hqueue.hpp"
#include "morphotree/tree/treeOfShapes/kgrid.hpp"

namespace morphotree {
  template<class Value>
  struct OrderImageResult
  {
    using ValueType = Value;

    OrderImageResult();
    OrderImageResult(std::vector<uint32> porderImg, std::vector<ValueType> pflattern, std::vector<uint32> pR, Box pDomain);

    std::vector<uint32> orderImg;
    std::vector<uint32> R;
    std::vector<ValueType> flattern;
    Box domain; 
  };


  template<class ValueType>
  OrderImageResult<ValueType> computeOrderImage(
    const Box &domain,
    const std::vector<ValueType> &f,
    const KGrid<ValueType> &F,
    const I32Point &pInfinity = I32Point{0,0});



  // ====================================== [ IMPLEMENTATION ] ============================================
  template<class ValueType>
  OrderImageResult<ValueType>::OrderImageResult()
  {}

  template<class ValueType>
  OrderImageResult<ValueType>::OrderImageResult(std::vector<uint32> porderImg, std::vector<ValueType> pflattern,
    std::vector<uint32> pR, Box pDomain)
    :orderImg{porderImg}, flattern{pflattern}, R{pR}, domain{pDomain}
  {}

  template<class ValueType>
  OrderImageResult<ValueType> computeOrderImage(
    const Box &domain,
    const std::vector<ValueType> &f,
    const KGrid<ValueType> &F,
    const I32Point &pInfinity)
  {
    const uint32 UNPROCESSED = std::numeric_limits<uint32>::max();
    const uint32 PROCESSED = std::numeric_limits<uint>::max()-1;

    using GridType = decltype(F);
    using QueueType = HQueue<ValueType, uint32>;
    using QueueKeyValueType = typename HQueue<ValueType, uint32>::KeyValueType;
    using QueueKeyType = typename HQueue<ValueType, uint32>::KeyType;
    using QueueValueType = typename HQueue<ValueType, uint32>::ValueType;
    using GridIntervalType = typename KGrid<ValueType>::IntervalType;

    unsigned int d = 0;
    Box Fdomain = F.immerseDomain();

    QueueType Q;
    std::vector<ValueType> flattern = std::vector<ValueType>(Fdomain.numberOfPoints());
    std::vector<uint32> R = std::vector<uint32>(Fdomain.numberOfPoints(), UNPROCESSED);
    std::vector<uint32> ord = std::vector<uint32>(Fdomain.numberOfPoints(), UNPROCESSED);
    const std::shared_ptr<Adjacency> adj = F.adj();

    

    ValueType lambdaOld = f[domain.pointToIndex(pInfinity)];
    Q.insert(lambdaOld, Fdomain.pointToIndex(F.immersePoint(pInfinity)));   

    uint32 i = R.size()-1;

    while (!Q.isEmpty()) {
      QueueKeyValueType keyvalue = Q.pop(lambdaOld);
      
      QueueKeyType lambda = keyvalue.key;
      QueueValueType pIndex = keyvalue.value;

      if (lambdaOld != keyvalue.key) {
        d++;
      }

      ord[pIndex] = d;
      flattern[pIndex] = lambda;
      R[i--] = pIndex;

      for (uint32 n : adj->neighbours(pIndex)) {
        if (ord[n] == UNPROCESSED) {
          GridIntervalType ab = F.interval(n);
          if (lambda < ab.min()) {
            Q.insert(ab.min(), n);
          }
          else if (lambda > ab.max()) {
            Q.insert(ab.max(), n);
          }
          else {
            Q.insert(lambda, n);
          }
          ord[n] = PROCESSED;
        }
      }
      lambdaOld = lambda;
    }

    return OrderImageResult<ValueType>{ord, flattern, R, Fdomain};
  }  
}
