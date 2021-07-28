#pragma once 

#include "morphotree/core/alias.hpp"
#include "morphotree/attributes/extinctionValues/ExtinctionValueLeavesComputer.hpp"
#include "morphotree/tree/mtree.hpp"

#include <algorithm>

namespace morphotree
{
  template<class ValueType, class AttrType>
  MorphologicalTree<ValueType> extinctionFilter(const MorphologicalTree<ValueType> &tree,
    const std::vector<AttrType> &attr, uint32 numberOfLeavesToKeep);

  template<class ValueType, class AttrType>
  void iextinctionFilter(MorphologicalTree<ValueType> &tree, const std::vector<AttrType> &attr,
    uint32 numberOfLeavesToKeep);


  // ============================ [ IMPLEMENTATION ] ===============================================
  template<class ValueType, class AttrType>
  void iextinctionFilter(MorphologicalTree<ValueType> &tree, const std::vector<AttrType> &attr, 
    uint32 numberOfLeavesToKeep)
  {
    using NodePtr = typename MorphologicalTree<ValueType>::NodePtr;
    using MapType = typename ExtinctionValueLeavesComputer<ValueType, AttrType>::MapType;    
    using KeyValuePair = std::pair<uint32, AttrType>;

    MapType extLeaves = ExtinctionValueLeavesComputer<ValueType, AttrType>().compute(tree, attr);

    if (numberOfLeavesToKeep < extLeaves.size()) {
      std::vector<KeyValuePair> items(extLeaves.begin(), extLeaves.end());
      std::sort(items.begin(), items.end(), [](KeyValuePair kv1, KeyValuePair kv2){
        return kv1.second > kv2.second;
      });

      std::vector<bool> keep(tree.numberOfNodes(), false);

      for (uint32 i = 0; i < numberOfLeavesToKeep; ++i) {
        keep[items[i].first] = true;
      }

      tree.tranverse([&keep](NodePtr node) {
        NodePtr parent = node->parent();
        if (parent != nullptr && !keep[parent->id()]) {
          keep[parent->id()] = keep[node->id()];
        }
      });

      keep[tree.root()->id()] = true;      
      tree.idirectFilter([&keep](NodePtr node) { return keep[node->id()]; });
    }    
  }

  template<class ValueType, class AttrType>
  MorphologicalTree<ValueType> extinctionFilter(const MorphologicalTree<ValueType> &tree,
    const std::vector<AttrType> &attr, uint32 numberOfLeavesToKeep)
  {
    MorphologicalTree<ValueType> ctree = tree.copy();
    iextinctionFilter(ctree, attr, numberOfLeavesToKeep);
    return ctree;
  }
}