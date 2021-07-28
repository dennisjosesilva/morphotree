#pragma once 

#include "morphotree/tree/mtree.hpp"

#include <unordered_map>
#include <vector>
#include <limits>

namespace morphotree
{
  template<class ValueType, class AttrType>
  class ExtinctionValueLeavesComputer
  {
  public:
    using MTree = MorphologicalTree<ValueType>;
    using NodePtr = typename MTree::NodePtr;
    using MapType = std::unordered_map<uint32, AttrType>;

    std::unordered_map<uint32, AttrType> compute(const MTree &tree, 
      const std::vector<AttrType> &attr) const;


    const static AttrType INF;

  private:  
    std::vector<NodePtr> extractLeaves(const MTree &tree) const;
    
  };


  // ===================== [ IMPLEMENTATION ] =============================================
  template<class ValueType, class AttrType>
  const AttrType ExtinctionValueLeavesComputer<ValueType, AttrType>::INF = 
    std::numeric_limits<AttrType>::max();


  template<class ValueType, class AttrType>
  std::vector<typename ExtinctionValueLeavesComputer<ValueType, AttrType>::NodePtr> 
    ExtinctionValueLeavesComputer<ValueType, AttrType>::extractLeaves(const MTree &tree) const
  {
    std::vector<NodePtr> leaves;
    tree.tranverse([&leaves](NodePtr node) {
      if (node->children().size() <= 0) { // is it a leaf? 
        leaves.push_back(node);
      }
    });

    return leaves;
  }

  template<class ValueType, class AttrType>
  std::unordered_map<uint32, AttrType> 
    ExtinctionValueLeavesComputer<ValueType, AttrType>::compute(const MTree &tree,
      const std::vector<AttrType> &attr) const
  {
    using ChildItr = typename std::list<NodePtr>::iterator;

    std::vector<NodePtr> leaves = extractLeaves(tree);
    std::vector<bool> visited(tree.numberOfNodes(), false);
    std::unordered_map<uint32, AttrType> extinctionValues;
    bool shouldContinue = true;

    for (NodePtr Nl : leaves) {
      AttrType extinction = INF;
      NodePtr Na = Nl;
      NodePtr Np = Na->parent();

      while (shouldContinue && Np != nullptr) {
        if (Np->children().size() > 1) {  
          // Np has a bifurcation.
          
          // verify whether we have a tie or the extinction value 
          // can already be computed
          std::list<NodePtr> children = Np->children();
          for (ChildItr citr = children.begin(); citr != children.end() && shouldContinue; citr++) {
            NodePtr Nc = *citr;
            if ((visited[Nc->id()] && Nc->id() != Na->id() && attr[Nc->id()] == attr[Na->id()]) || 
              (Nc->id() != Na->id() && attr[Nc->id()] > attr[Na->id()])) {
                shouldContinue = false;
              }              
            visited[Nc->id()] = true;
          }
        }
        // Move to parent.
        if (shouldContinue) {
          Na = Np;
          Np = Na->parent();
        }
      }
      shouldContinue = true;
      if (Np != nullptr)
        extinction = attr[Na->id()];
      
      extinctionValues[Nl->id()] = extinction;
    }
    return extinctionValues;
  }
}