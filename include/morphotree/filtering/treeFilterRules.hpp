#pragma once


#include "morphotree/tree/mtree.hpp"

#include <functional>

namespace morphotree
{
  template<class ValueType>
  MorphologicalTree<ValueType> maxRuleFilter(const MorphologicalTree<ValueType> &tree,
    std::function<bool(typename MorphologicalTree<ValueType>::NodePtr)> keep);

  template<class ValueType>
  void imaxRuleFilter(MorphologicalTree<ValueType> &tree, 
    std::function<bool(typename MorphologicalTree<ValueType>::NodePtr)> keep);

  // ======================[ IMPLEMENTATION ] ===================================================
  template<class ValueType>
  MorphologicalTree<ValueType> maxRuleFilter(const MorphologicalTree<ValueType> &tree,
    std::function<bool(typename MorphologicalTree<ValueType>::NodePtr)> keep)
  {
    MorphologicalTree<ValueType> ctree = tree.copy();
    imaxRuleFilter(ctree, keep);
    return ctree;
  }

  template<class ValueType>
  void imaxRuleFilter(MorphologicalTree<ValueType> &tree, 
    std::function<bool(typename MorphologicalTree<ValueType>::NodePtr)> keep)
  {
    using MTree = MorphologicalTree<ValueType>;
    using NodePtr = typename MTree::NodePtr;

    std::vector<bool> shouldKeep(tree.numberOfNodes(), false);

    tree.tranverse([&shouldKeep, keep](NodePtr node) { shouldKeep[node->id()] = keep(node); });
    tree.tranverse([&shouldKeep](NodePtr node){
      if (node->parent() != nullptr && shouldKeep[node->id()]) {
        shouldKeep[node->parent()->id()] = true;
      }
    });

    tree.idirectFilter([&shouldKeep](NodePtr node) { return shouldKeep[node->id()]; });
  }
}