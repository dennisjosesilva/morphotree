#pragma once 

#include "morphotree/core/alias.hpp"
#include "morphotree/attributes/attributeComputer.hpp"

namespace morphotree
{
  template<class ValueType>
  class NumberOfDescendantsComputer : public AttributeComputer<uint32, ValueType>
  {
  public:
    using AttrType = uint32;
    using TreeType = typename AttributeComputer<AttrType, ValueType>::TreeType;
    using NodePtr = typename TreeType::NodePtr;

    std::vector<AttrType> initAttributes(const TreeType &tree);
    void computeInitialValue(std::vector<AttrType> &attr, NodePtr node);
    void mergeToParent(std::vector<AttrType> &attr, NodePtr node, NodePtr parent);
  };

  // ========================= [ IMPLEMENTATION ] =========================================
  template<class ValueType>
  std::vector<typename NumberOfDescendantsComputer<ValueType>::AttrType> 
    NumberOfDescendantsComputer<ValueType>::initAttributes(const TreeType &tree)
  {
    return std::vector<AttrType>(tree.numberOfNodes(), 0);
  }

  template<class ValueType>
  void NumberOfDescendantsComputer<ValueType>::computeInitialValue(std::vector<AttrType> &attr,
    NodePtr node)
  {
    attr[node->id()] += node->children().size();
  }

  template<class ValueType>
  void NumberOfDescendantsComputer<ValueType>::mergeToParent(std::vector<AttrType> &attr,
    NodePtr node, NodePtr parent)
  {
    attr[parent->id()] += attr[node->id()];
  }
}