#include "morphotree/attributes/bitquads/quadCountTreeOfShapesComputer.hpp"

namespace morphotree
{
  const uint32 WindowMaxTree::UndefinedParent = std::numeric_limits<uint32>::max();
  const uint8 WindowMaxTree::UndefinedNodeId = std::numeric_limits<uint8>::max();

  WindowMaxTreeNode::WindowMaxTreeNode(uint8 id, uint8 parent, uint8 size, uint32 representative)
    :id_{id}, parent_{parent}, size_{size}, representative_{representative}
  {}
  
  WindowMaxTree::WindowMaxTree(const std::vector<uint32> &orderImage,
    std::shared_ptr<Adjacency> adj)
    : orderImage_{orderImage}, adj_{adj}
  {}

  void WindowMaxTree::sort(const Box::SubBox &wDomain)
  {
    uint8 minIndex;
    uint8 temp;

    uint32 N = wDomain.numberOfPoints();
    resetSortedArray(wDomain);

    for (uint32 j = 0; j < N; j++) {
      minIndex = j;
      for (uint32 i = j+1; i < N; i++) {
        uint32 li = S_[i];
        if (orderImage_[wDomain.localIndexToIndex(li)] < orderImage_[wDomain.localIndexToIndex(S_[minIndex])]) 
          minIndex = i;
      }  
      temp = S_[j];
      S_[j] = S_[minIndex];
      S_[minIndex] = temp;
    } 
  }

  void WindowMaxTree::resetSortedArray(const Box::SubBox &wDomain)
  {
    const uint8 N = wDomain.numberOfPoints();
    for (uint32 i = 0; i < N; ++i) 
      S_[i] = i;
    
  }

  void WindowMaxTree::build(const Box::SubBox &wDomain)
  {
    cleanAttributes();
    sort(wDomain);
    const uint32 N = wDomain.numberOfPoints();
    
    for (uint32 ni = 1; ni <= N; ni++) {
      uint32 i = N - ni;
      parent_[S_[i]] = S_[i];
      zpar_[S_[i]] = S_[i];
      for (uint32 n : adj_->neighbours(wDomain.localIndexToIndex(S_[i]))) {
        uint32 ln = wDomain.indexToLocalIndex(n);
    
        if (ln != Box::SubBox::UndefinedIndex && wDomain.contains(wDomain.indexToPoint(n))) {
          if (parent_[ln] != UndefinedParent) {
            uint8 r = findRoot(ln);
            if (r != S_[i]) {
              zpar_[r] = S_[i];
              parent_[r] = S_[i];
            }
          }
        }
      }
    }
    
    canonise(wDomain);
    computeSize(wDomain);
  }

  uint32 WindowMaxTree::findRoot(uint32 p) 
  {
    if (zpar_[p] != p)
      zpar_[p] = findRoot(zpar_[p]);
    return zpar_[p];
  }

  void WindowMaxTree::canonise(const Box::SubBox &wDomain)
  {
    for (uint32 p : S_) {
      uint32 q = parent_[p];
      if (orderImage_[wDomain.localIndexToIndex(q)] == orderImage_[wDomain.localIndexToIndex(parent_[q])]) {
        parent_[p] = parent_[q];
      }
    }
  }

  void WindowMaxTree::cleanAttributes()
  {
    std::fill(std::begin(parent_), std::end(parent_), UndefinedParent);
    std::fill(std::begin(parent_), std::end(zpar_), UndefinedParent);
    std::fill(std::begin(sortedLvRoot_), std::end(sortedLvRoot_), UndefinedParent);
    std::fill(std::begin(size_), std::end(size_), 1);

    cmap_.clear();
    cmap_.reserve(9);

    nLevelRoots_ = 0;
    tsize_ = { 1,0,1,   0,0,0,   1,0,1 };
  }

  void WindowMaxTree::computeSize(const Box::SubBox &wDomain)
  {
    const uint32 N = wDomain.numberOfPoints();
    for (int8 i = N-1; i >= 0; --i) {
      uint32 p = S_[i];
      tsize_[parent_[p]] = tsize_[parent_[p]] + tsize_[p];
      if ((orderImage_[wDomain.localIndexToIndex(parent_[p])] != orderImage_[wDomain.localIndexToIndex(p)]) 
        || (parent_[p] == p)) {
        sortedLvRoot_[nLevelRoots_] = p;
        ++nLevelRoots_;
      }
    }
  
    uint8 id = 0;
    for (int8 i = 1; i <= nLevelRoots_; ++i) {
      uint32 p = sortedLvRoot_[nLevelRoots_ - i];
      WindowMaxTreeNode &node = nodes_[id];
      node.id(id);
      cmap_[p] = id;
      node.representative(wDomain.localIndexToIndex(p));
      
      if (parent_[p] != p)
        node.parent( nodes_[cmap_[parent_[p]]].id() );
      
      node.size(tsize_[p]);
      ++id;      
    }

    nodes_[0].size(nodes_[0].size() / 2);
  }

  void  WindowMaxTree::tranverse(std::function<void(const WindowMaxTreeNode&)> visit)
  {
    for (int8 id = nLevelRoots_-1; id >= 0; id--) {
      visit(nodes_[id]);
    }
  }
}