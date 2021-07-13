#pragma once 

#include "morphotree/core/alias.hpp"
#include "morphotree/tree/mtree.hpp"
#include <vector>

namespace morphotree
{
  template<class ValueType>
  MorphologicalTree<ValueType> filterTreeLexographically(const MorphologicalTree<ValueType> &tree, 
    const std::vector<uint32> &order, uint32 numberOfNodesToBeKept);  
  
  template<class ValueType>
  void ifilterTreeLexographically(MorphologicalTree<ValueType> &tree, 
    const std::vector<uint32> &order, uint32 numberOfNodesToBeKept);

  // ========================== [ IMPLEMENTATION ] ================================================
  template<class ValueType>
  void ifilterTreeLexographically(const MorphologicalTree<ValueType> &tree, 
    const std::vector<uint32> &order, uint32 numberOfNodesToBeKept)
  {
    using NodePtr = typename MorphologicalTree<ValueType>::NodePtr;

    tree.idirectFilter([&order, numberOfNodesToBeKept](NodePtr node) {
      return order[node->id()] < numberOfNodesToBeKept;
    });
  }

  template<class ValueType>
  MorphologicalTree<ValueType> filterTreeLexographically(const MorphologicalTree<ValueType> &tree,
    const std::vector<uint32> &order, uint32 numberOfNodesToBeKept)
  {
    MorphologicalTree<ValueType> ctree = tree.copy();
    ifilterTreeLexographical(ctree, order, numberOfNodesToBeKept);
    return ctree;
  }
}