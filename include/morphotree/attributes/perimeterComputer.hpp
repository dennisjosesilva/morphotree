#pragma once 

#include "morphotree/core/alias.hpp"
#include "morphotree/attributes/attributeComputer.hpp"
#include "morphotree/core/box.hpp"
#include "morphotree/core/point.hpp"
#include "morphotree/adjacency/adjacency8c.hpp"

#include <array>

// Implementation based on
// Morphological hat-transform scale spaces and their use inpattern classication
// Andrei C. Jalba, Michael H.F. Wilkinson, Jos B.T.M. Roerdink

namespace morphotree 
{
  template<class ValueType>
  class MaxTreePerimeterComputer : public AttributeComputer<uint32, ValueType>
  {
  public:
    using AttrType = uint32;
    using TreeType = typename AttributeComputer<uint32, ValueType>::TreeType;
    using NodePtr = typename TreeType::NodePtr;

    MaxTreePerimeterComputer(Box domain, const std::vector<ValueType> &image);

    std::vector<uint32> initAttributes(const TreeType &tree);
    void computeInitialValue(std::vector<uint32> &attr, NodePtr node);
    void mergeToParent(std::vector<uint32> &attr, NodePtr node, NodePtr parent); 

  private:
    const std::array<I32Point, 4> offsets_ = {
      I32Point{-1,0}, I32Point{0, -1}, I32Point{1, 0}, I32Point{0, 1} 
    };

  private:
    const std::vector<ValueType> &image_;
    Box domain_;
    
  };

  template<class ValueType>
  class MinTreePerimeterComputer: public AttributeComputer<uint32, ValueType>
  {
  public:
    using AttrType = uint32;
    using TreeType = typename AttributeComputer<uint32, ValueType>::TreeType;
    using NodePtr = typename TreeType::NodePtr;

    MinTreePerimeterComputer(Box domain, const std::vector<ValueType> &image);

    std::vector<uint32> initAttributes(const TreeType &tree);
    void computeInitialValue(std::vector<uint32> &attr, NodePtr node);
    void mergeToParent(std::vector<uint32> &attr, NodePtr node, NodePtr parent);    

  private:
    const std::array<I32Point, 4> offsets_ = {
      I32Point{-1,0}, I32Point{0, -1}, I32Point{1, 0}, I32Point{0, 1} 
    };

  private:
    const std::vector<ValueType> &image_;
    Box domain_;
  };


  // ============================= [ IMPLEMENTATION ] =========================================
  // ============================= [ MAX-TREE ] ===============================================
  template<class ValueType>
  MaxTreePerimeterComputer<ValueType>::MaxTreePerimeterComputer(Box domain,
    const std::vector<ValueType> &image)
    :domain_{domain}, image_{image}
  {}

  template<class ValueType>
  std::vector<uint32> MaxTreePerimeterComputer<ValueType>::initAttributes(
    const MaxTreePerimeterComputer<ValueType>::TreeType &tree)
  {
    return std::vector<uint32>(tree.numberOfNodes(), 0);
  }

  template<class ValueType>
  void MaxTreePerimeterComputer<ValueType>::computeInitialValue(std::vector<uint32> &attr,
    MaxTreePerimeterComputer<ValueType>::NodePtr node)
  {    
    for (uint32 pidx : node->cnps()) {
      int32 H = 0, L = 0;
      I32Point p = domain_.indexToPoint(pidx);
      for (const I32Point& offset : offsets_) {
        I32Point q = p + offset;
        if (!domain_.contains(q) || image_[domain_.pointToIndex(q)] < node->level()) {
          L++;
        }
        else if (image_[domain_.pointToIndex(q)] > node->level()) {
          H++;
        }
      }
      attr[node->id()] += L - H;
    }
  }

  template<class ValueType>
  void MaxTreePerimeterComputer<ValueType>::mergeToParent(std::vector<uint32> &attr,
    MaxTreePerimeterComputer<ValueType>::NodePtr node, 
    MaxTreePerimeterComputer<ValueType>::NodePtr parent)
  {
    attr[parent->id()] += attr[node->id()];
  }  

  // ============================= [ MIN-TREE ] ===============================================
  template<class ValueType>
  MinTreePerimeterComputer<ValueType>::MinTreePerimeterComputer(Box domain,
    const std::vector<ValueType> &image)
    :domain_{domain}, image_{image}
  {}

  template<class ValueType>
  std::vector<uint32> MinTreePerimeterComputer<ValueType>::initAttributes(
    const MinTreePerimeterComputer<ValueType>::TreeType &tree)
  {
    return std::vector<uint32>(tree.numberOfNodes(), 0);
  }

  template<class ValueType>
  void MinTreePerimeterComputer<ValueType>::computeInitialValue(std::vector<uint32> &attr,
    MinTreePerimeterComputer<ValueType>::NodePtr node)
  {
    for (uint32 pidx : node->cnps()) {
      int32 H = 0, L = 0;
      I32Point p = domain_.indexToPoint(pidx);
      for (const I32Point offset: offsets_) {
        I32Point q = p + offset;
        if (!domain_.contains(q) || image_[domain_.pointToIndex(q)] > node->level()) {
          H++;
        }
        else if (image_[domain_.pointToIndex(q)] < node->level()) {
          L++;
        }
      }
      attr[node->id()] += H - L;
    }
  }

  template<class ValueType>
  void MinTreePerimeterComputer<ValueType>::mergeToParent(std::vector<uint32> &attr, 
    MinTreePerimeterComputer<ValueType>::NodePtr node,
    MinTreePerimeterComputer<ValueType>::NodePtr parent)
  {
    attr[parent->id()] += attr[node->id()];
  }
}