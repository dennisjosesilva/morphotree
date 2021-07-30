#pragma once 

#include "morphotree/core/alias.hpp"
#include "morphotree/tree/mtree.hpp"

#include <algorithm>
#include <vector>
#include <stack>
#include <unordered_map>

namespace morphotree 
{
  template<class ValueType, class AttrType>
  MorphologicalTree<ValueType> progressiveDifferenceFilter(const MorphologicalTree<ValueType>& tree, 
    const std::vector<AttrType> &attr, AttrType threshold);
  
  template<class ValueType, class AttrType>
  void iprogressiveDifferenceFilter(MorphologicalTree<ValueType> &tree, 
    const std::vector<AttrType> &attr, AttrType threshold);

  // =====================[ IMPLEMENTATION ] ===============================================================
  template<class ValueType, class AttrType>
  MorphologicalTree<ValueType> progressiveDifferenceFilter(const MorphologicalTree<ValueType> &tree, 
    const std::vector<AttrType> &attr, AttrType threshold)
  {
    MorphologicalTree<ValueType> ctree = tree.copy();
    iprogressiveDifferenceFilter(ctree, attr, threshold);
    return ctree;
  }

  template<class ValueType, class AttrType>
  void iprogressiveDifferenceFilter(MorphologicalTree<ValueType> &tree, const std::vector<AttrType> &attr,
    AttrType threshold)
  {
    using MTree = MorphologicalTree<ValueType>;    
    using NodePtr = typename MTree::NodePtr;
    using Stack = std::stack<NodePtr>;

    std::vector<NodePtr> leaves;
    std::vector<bool> visited(tree.numberOfNodes(), false);
    std::vector<bool> keep(tree.numberOfNodes(), false);

    tree.tranverse([&leaves](NodePtr node) {
      if (node->children().size() <= 0)
        leaves.push_back(node);
    });

    for (uint32 i = 0; i < leaves.size(); ++i) {
      Stack stack;
      
      for (NodePtr node = leaves[i]; node != nullptr; node = node->parent()) {
        stack.push(node);
      }

      // setup root node
      uint32 lastAncenstorId = 0;
      
      while (!stack.empty()) {
        NodePtr curNode = stack.top();
        stack.pop();

        if (visited[curNode->id()]) {
          if (keep[curNode->id()])
            lastAncenstorId = curNode->id();
        }           
        else {
          visited[curNode->id()] = true;
          if (attr[lastAncenstorId] - attr[curNode->id()] > threshold) {
            keep[curNode->id()] = true;
            lastAncenstorId = curNode->id();
          }
        }
      }
    }

    tree.idirectFilter([&keep](NodePtr node) { return keep[node->id()]; });    
  }

}