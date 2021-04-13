#pragma once

#include "morphotree/core/alias.hpp"
#include "morphotree/attributes/attributeComputer.hpp"

namespace morphotree
{
  template<class ValueType>
  class MaxTreeVolumeComputer : public AttributeComputer<float, ValueType>
  {
  public:
    using AttrType = float;
    using TreeType = typename AttributeComputer<float, ValueType>::TreeType;
    using NodePtr = typename TreeType::NodePtr;

    std::vector<float> initAttributes(const TreeType &tree);
    void computeInitialValue(std::vector<float> &attr, NodePtr node);
    void mergeToParent(std::vector<float> &attr, NodePtr node, NodePtr parent);

  private: 
    std::vector<uint32> area_;
  };

  // ================ [Implementation] ======================================================
  template<class ValueType>
  std::vector<float> MaxTreeVolumeComputer<ValueType>::initAttributes(const TreeType &tree)
  {
    area_.resize(tree.numberOfNodes(), 0);
    return std::vector<float>(tree.numberOfNodes(), 0.f);   
  }

  template<class ValueType>
  void MaxTreeVolumeComputer<ValueType>::computeInitialValue(std::vector<float> &attr, NodePtr node)
  {
    area_[node->id()] += node->cnps().size();
    attr[node->id()] += node->cnps().size();
  }

  template<class ValueType>
  void MaxTreeVolumeComputer<ValueType>::mergeToParent(std::vector<float> &attr, NodePtr node, 
    NodePtr parent)
  {
    area_[parent->id()] += area_[node->id()];
    attr[parent->id()] += area_[node->id()] +  attr[node->id()] +  
      (area_[node->id()] *  (node->level() - parent->level()));
  }
}