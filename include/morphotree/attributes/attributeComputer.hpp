#pragma once 

#include "morphotree/tree/mtree.hpp"
#include <vector>

namespace morphotree 
{
  template<class AttrType, class ValueType>
  class AttributeComputer
  {
  public:
    using NodePtr = typename MorphologicalTree<ValueType>::NodePtr;
    using TreeType = MorphologicalTree<ValueType>;

    std::vector<AttrType> computeAttribute(const TreeType &tree);

    virtual std::vector<AttrType> initAttributes(const TreeType &tree) = 0;
    virtual void computeInitialValue(std::vector<AttrType> &attr, NodePtr node) = 0;
    virtual void mergeToParent(std::vector<AttrType> &attr, NodePtr node, NodePtr parent) = 0;

    virtual ~AttributeComputer() {}
  };

  // ========================= [Implementation] ===============================================================
  template<class AttrType, class ValueType>
  std::vector<AttrType> AttributeComputer<AttrType, ValueType>::computeAttribute(
    const MorphologicalTree<ValueType> &tree)
  {
    using TreeType = MorphologicalTree<ValueType>; 
    using NodePtr = typename TreeType::NodePtr;

    std::vector<AttrType> attr = initAttributes(tree);

    tree.tranverse([this, &attr](NodePtr node){
      this->computeInitialValue(attr, node);
      if (node->parent() != nullptr)
        this->mergeToParent(attr, node, node->parent());
    });

    return attr;
  }
}

