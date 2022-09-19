#pragma once 

#include "morphotree/adjacency/adjacency.hpp"
#include "morphotree/core/box.hpp"
#include "morphotree/tree/mtree.hpp"

#include <vector>
#include <set>

namespace morphotree
{
  template<class ValueType>
  std::vector<std::set<uint32>> extractCountors(
    const Box &domain,
    const std::vector<ValueType> &f,
    std::shared_ptr<Adjacency> adj,
    const MorphologicalTree<ValueType> &tree);

  std::vector<bool> reconstructContourImage(const std::set<uint32>& contour, 
    const Box &domain);

  // =================== [IMPLEMENTATION] ==========================
  template<class ValueType>
  std::vector<std::set<uint32>> extractCountors(
    const Box &domain, 
    const std::vector<ValueType> &f,
    std::shared_ptr<Adjacency> adj,
    const MorphologicalTree<ValueType> &tree)
  {
    using MTree = MorphologicalTree<ValueType>;
    using NodePtr = typename MTree::NodePtr;

    std::vector<std::set<uint32>> contours(tree.numberOfNodes());
    std::vector<uint8> ncount(domain.numberOfPoints());

    tree.tranverse([&f, &contours, &ncount, adj](NodePtr node){
      
      // Initialise contours of node "N"
      std::set<uint32> &Ncontour = contours[node->id()];
      for (NodePtr c : node->children()) {
        for (uint32 pidx : contours[c->id()])
          Ncontour.insert(pidx);
      }

      for (uint32 pidx : node->cnps()) {
        for (uint32 qidx : adj->neighbours(pidx)) {
          if (qidx == Box::UndefinedIndex || f[pidx] > f[qidx]) 
            ncount[pidx]++;
          else if (f[pidx] < f[qidx]) {
            ncount[qidx]--;

            if (ncount[qidx] == 0) 
              Ncontour.erase(qidx);
          }
        }

        if (ncount[pidx] > 0)
          Ncontour.insert(pidx);
      }
    });
    
    return contours;
  }


  std::vector<bool> reconstructContourImage(const std::set<uint32>& contour, 
      const Box &domain)
  {
    std::vector<bool> bimg(domain.numberOfPoints(), false);
    for (uint32 pidx : contour) {
      bimg[pidx] = true;
    }

    return bimg;
  }
}
