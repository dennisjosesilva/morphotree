#pragma once

#include "morphotree/tree/mtree.hpp"
#include "morphotree/attributes/attributeComputer.hpp"
#include "morphotree/attributes/extinctionValues/ExtinctionValueLeavesComputer.hpp"

#include <limits>
#include <unordered_map>
#include <vector>

namespace morphotree
{
  template<class ValueType, class AttrType>
  class ExtinctionValueComputer : public AttributeComputer<AttrType, ValueType>
  {
  public:    
    using TreeType = MorphologicalTree<ValueType>;
    using NodePtr = typename TreeType::NodePtr;
    using MapType = std::unordered_map<uint32, AttrType>;

    const static AttrType SmallestAttrValue;

    ExtinctionValueComputer(const MapType &extValues);
    
    std::vector<AttrType> initAttributes(const TreeType &tree) override;
    void computeInitialValue(std::vector<AttrType> &attr, NodePtr node) override;
    void mergeToParent(std::vector<AttrType> &attr, NodePtr node, NodePtr parent) override;
   
    virtual ~ExtinctionValueComputer() {}

  private:
    inline bool isLeaf(NodePtr node) const { return node->children().size() == 0; }

  private:
    const MapType &extValues_;
  };


  // =================== [ IMPLEMENTATION ] ===========================================================
  template<class ValueType, class AttrType>
  const AttrType ExtinctionValueComputer<ValueType, AttrType>::SmallestAttrValue = 
    std::numeric_limits<AttrType>::min();

  template<class ValueType, class AttrType>
  ExtinctionValueComputer<ValueType, AttrType>::ExtinctionValueComputer(const MapType &extValues)
    :extValues_{extValues}
  {}

  template<class ValueType, class AttrType>
  std::vector<AttrType> ExtinctionValueComputer<ValueType, AttrType>::initAttributes(
    const TreeType &tree) 
  {
    return std::vector<AttrType>(tree.numberOfNodes(), SmallestAttrValue);
  }

  template<class ValueType, class AttrType>
  void ExtinctionValueComputer<ValueType, AttrType>::computeInitialValue(std::vector<AttrType> &attr,
    NodePtr node)
  {
    if (isLeaf(node))
      attr[node->id()] = extValues_.at(node->id());   
  }

  template<class ValueType, class AttrType>
  void ExtinctionValueComputer<ValueType, AttrType>::mergeToParent(std::vector<AttrType> &attr, 
    NodePtr node, NodePtr parent)
  {    
    if (attr[parent->id()] < attr[node->id()]) 
      attr[parent->id()] = attr[node->id()];          
  }
}