#pragma once 

#include "morphotree/core/alias.hpp"
#include "morphotree/attributes/attributeComputer.hpp"

namespace morphotree
{
  template<class ValueType>
  class TopologicalHeightComputer : public AttributeComputer<uint32, ValueType>
  {
  public:
    using AttrType = uint32;
    using TreeType = typename AttributeComputer<uint32, ValueType>::TreeType;
    using NodePtr = typename TreeType::NodePtr;

    std::vector<AttrType> initAttributes(const TreeType &tree);
    void computeInitialValue(std::vector<AttrType> &attr, NodePtr node);
    void mergeToParent(std::vector<AttrType> &attr, NodePtr node, NodePtr parent);
  };

  // ========================== [ IMPLEMENTATION ] ==============================================
  template<class ValueType>
  std::vector<typename TopologicalHeightComputer<ValueType>::AttrType> 
    TopologicalHeightComputer<ValueType>::initAttributes(const TreeType &tree)
  {
    return std::vector<AttrType>(tree.numberOfNodes(), 0);
  }

  template<class ValueType>
  void TopologicalHeightComputer<ValueType>::computeInitialValue(std::vector<AttrType> &attr,
    NodePtr node)
  {}

  template<class ValueType>
  void TopologicalHeightComputer<ValueType>::mergeToParent(std::vector<AttrType> &attr, 
    NodePtr node, NodePtr parent)
  {        
    if (attr[parent->id()] > 0) {
      AttrType pattr = attr[parent->id()] - 1;
      
      if (pattr < attr[node->id()])
        attr[parent->id()] = attr[node->id()] + 1;
    }
    else {
      attr[parent->id()] = attr[node->id()] + 1;
    }
  }
}