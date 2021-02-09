#pragma once 

#include "morphotree/core/alias.hpp"
#include "morphotree/attributes/attributeComputer.hpp"


namespace morphotree
{
  template<class ValueType>
  class AreaComputer : public AttributeComputer<uint32, ValueType>
  {
  public:
    using AttrType = uint32;
    using TreeType = typename AttributeComputer<uint32, ValueType>::TreeType;
    using NodePtr = typename TreeType::NodePtr;
    

    std::vector<uint32> initAttributes(const TreeType &tree);
    void computeInitialValue(std::vector<uint32> &attr, NodePtr node);
    void mergeToParent(std::vector<uint32> &attr, NodePtr node, NodePtr parent);
  };


  // ========================== [Implementation] ===============================================
  template<class ValueType>
  std::vector<uint32> AreaComputer<ValueType>::initAttributes(
    const AreaComputer<ValueType>::TreeType &tree)
  {
    return std::vector<uint32>(tree.numberOfNodes(), 0);
  }

  template<class ValueType>
  void AreaComputer<ValueType>::computeInitialValue(std::vector<uint32> &attr, 
    AreaComputer<ValueType>::NodePtr node)
  {
    attr[node->id()] += node->cnps().size(); 
  }

  template<class ValueType>
  void AreaComputer<ValueType>::mergeToParent(std::vector<uint32> &attr,
    AreaComputer<ValueType>::NodePtr node, AreaComputer<ValueType>::NodePtr parent)
  {
    attr[parent->id()] += attr[node->id()];
  }
}