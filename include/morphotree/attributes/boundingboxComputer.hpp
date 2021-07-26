#pragma once 

#include "morphotree/core/alias.hpp"
#include "morphotree/core/point.hpp"
#include "morphotree/core/box.hpp"
#include "morphotree/attributes/attributeComputer.hpp"

#include <limits>


namespace morphotree
{
  template<class ValueType>
  class BoundingBoxComputer : public AttributeComputer<Box, ValueType>
  {
  public:
    using AttrType = Box;
    using TreeType = typename AttributeComputer<Box, ValueType>::TreeType;
    using NodePtr = typename TreeType::NodePtr;

    BoundingBoxComputer(Box domain);

    std::vector<AttrType> initAttributes(const TreeType &tree);
    void computeInitialValue(std::vector<AttrType> &attr, NodePtr node);
    void mergeToParent(std::vector<AttrType> &attr, NodePtr node, NodePtr parent);

  private:
    Box domain_;
  };

  // ======================= [ IMPLEMENTATION ] ================================================
  template<class ValueType>
  BoundingBoxComputer<ValueType>::BoundingBoxComputer(Box domain)
    :domain_{domain}
  {}

  template<class ValueType>
  std::vector<typename BoundingBoxComputer<ValueType>::AttrType>
    BoundingBoxComputer<ValueType>::initAttributes(const TreeType &tree)
  {
    std::vector<AttrType> attr(tree.numberOfNodes(), Box());

    tree.tranverse([&attr, this](NodePtr node) {
      int32 xmin = std::numeric_limits<int32>::max();
      int32 ymin = std::numeric_limits<int32>::max();

      int32 xmax = std::numeric_limits<int32>::min();    
      int32 ymax = std::numeric_limits<int32>::min();

      for (uint32 pidx : node->cnps()) {
        I32Point p = domain_.indexToPoint(pidx);

        if (xmax < p.x()) xmax = p.x();
        if (xmin > p.x()) xmin = p.x();
        if (ymax < p.y()) ymax = p.y();
        if (ymin > p.y()) ymin = p.y();
      }

      attr[node->id()] = Box::fromCorners(I32Point{xmin, ymin}, I32Point{xmax, ymax});
    });

    return attr;
  }

  template<class ValueType>
  void BoundingBoxComputer<ValueType>::computeInitialValue(std::vector<AttrType> &attr, 
    NodePtr node)
  { }

  template<class ValueType>
  void BoundingBoxComputer<ValueType>::mergeToParent(std::vector<AttrType> &attr, 
    NodePtr node, NodePtr parent)
  {
    int32 left = attr[parent->id()].left();
    int32 top = attr[parent->id()].top();
    int32 right = attr[parent->id()].right();
    int32 bottom = attr[parent->id()].bottom();

    Box bb = attr[node->id()];

    if (bb.left() < left) left = bb.left();
    if (bb.right() > right) right = bb.right();
    if (bb.top() < top) top = bb.top();
    if (bb.bottom() > bottom) bottom = bb.bottom();

    attr[parent->id()] = Box::fromCorners(I32Point{left, top}, I32Point{right, bottom});
  }
}