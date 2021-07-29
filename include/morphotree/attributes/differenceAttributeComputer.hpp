#pragma once 

#include "morphotree/tree/mtree.hpp"
#include "morphotree/attributes/attributeComputer.hpp"

#include <vector>

namespace morphotree
{
  template<class AttrType, class ValueType>
  class DifferenceAttributeComputer : public AttributeComputer<AttrType, ValueType>
  {
  public:
    using TreeType = typename AttributeComputer<AttrType, ValueType>::TreeType;
    using NodePtr = typename TreeType::NodePtr;
    using AttributeType = AttrType;

    DifferenceAttributeComputer(const std::vector<AttrType> &underlyingAttr);

    std::vector<AttrType> initAttributes(const TreeType &tree) override;
    void computeInitialValue(std::vector<AttrType> &attr, NodePtr node) override;
    void mergeToParent(std::vector<AttrType> &attr, NodePtr node, NodePtr parent) override;

    virtual ~DifferenceAttributeComputer() { }

  private:
    const std::vector<AttrType> &underlyingAttr_;
  };

  // ======================== [ IMPLEMENTATION ] ===================================================
  template<class AttrType, class ValueType>
  DifferenceAttributeComputer<AttrType, ValueType>::DifferenceAttributeComputer(
    const std::vector<AttrType> &underlyingAttr)
  :underlyingAttr_{underlyingAttr}
  {}

  template<class AttrType, class ValueType>
  std::vector<AttrType> DifferenceAttributeComputer<AttrType, ValueType>::initAttributes(
    const TreeType &tree)
  {
    return std::vector<AttrType>(tree.numberOfNodes(), 0);
  }

  template<class AttrType, class ValueType>
  void DifferenceAttributeComputer<AttrType, ValueType>::computeInitialValue(
    std::vector<AttrType> &attr, NodePtr node)
  {}

  template<class AttrType, class ValueType>
  void DifferenceAttributeComputer<AttrType, ValueType>::mergeToParent(
    std::vector<AttrType> &attr, NodePtr node, NodePtr parent)
  {
    if (parent != nullptr)
      attr[node->id()] = underlyingAttr_[parent->id()] - underlyingAttr_[node->id()];
  }
}