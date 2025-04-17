#pragma once

#include "morphotree/core/alias.hpp"
#include "morphotree/core/box.hpp"
#include <memory>
#include <list>
#include <vector>
#include <limits>

#include "morphotree/tree/ct_builder.hpp"
#include "morphotree/adjacency/adjacency.hpp"
#include "morphotree/core/sort.hpp"

#include <queue>
#include <stack>

namespace morphotree
{
  enum class MorphoTreeType
  {
    MaxTree,
    MinTree,
    TreeOfShapes
  };

  template<class WeightType>
  class MTNode
  {
  public:
    using ValueType = WeightType; 
    using NodePtr = std::shared_ptr<MTNode<WeightType>>;    

    MTNode(uint32 id=0);

    inline uint32 id() const { return id_; }
    inline uint32& id() { return id_; }
    inline void id(uint32 newid) { id_ = newid; } 

    inline uint32 representative() const { return representative_; }
    inline uint32& representative() { return representative_; } 
    inline void representative(uint32 newrep) { representative_ = newrep; }

    inline WeightType& level() { return level_; }
    inline WeightType  level() const { return level_; }
    inline void level(WeightType v) { level_ = v; }

    inline const std::vector<uint32>& cnps() const { return cnps_; }
    inline void appendCNP(uint32 cnp) { cnps_.push_back(cnp); }
    inline void includeCNPS(const std::vector<uint32> &cnps);

    inline NodePtr parent() { return parent_; }
    inline const NodePtr parent() const  { return parent_; }
    inline void parent(NodePtr parent) { parent_ = parent;}

    inline void appendChild(std::shared_ptr<MTNode> child) { children_.push_back(child); }
    inline void includeChildren(const std::vector<NodePtr> &children);
    inline void removeChild(NodePtr c) { children_.remove(c); }
    inline const std::list<NodePtr>& children() const { return children_; }
    
    std::vector<uint32> reconstruct() const;
    std::vector<bool> reconstruct(const Box &domain) const;

    std::vector<WeightType> reconstructGrey(const Box &domain, 
      WeightType backgroundValue=0) const;

    NodePtr copy() const;

  private:
    void reconstruct(std::vector<uint32> &pixels, const NodePtr node) const; 
    void reconstructGrey(NodePtr node, const Box &domain, 
      std::vector<WeightType> &f) const;

  private:
    uint32 id_;
    uint32 representative_;

    WeightType level_;
    std::vector<uint32> cnps_;
    NodePtr parent_;
    std::list<NodePtr> children_;
  };



  template<class WeightType>
  class MorphologicalTree
  {
  public:

    // ====================================================
    // Traversal iterators
    // ====================================================
    class Traversal
    {
    public:
      class Iterator 
      {
      public:
        using MTree = MorphologicalTree<WeightType>;
        using NodePtr = typename MTNode<WeightType>::NodePtr;

        Iterator(const MorphologicalTree<WeightType> &tree, uint32 index); 

        inline const NodePtr operator*() const;
        Iterator& operator++();
        bool operator==(const Iterator &it) const;
        bool operator!=(const Iterator &it) const { return !(*this == it); }

      private:
        uint32 index_;
        const MorphologicalTree<WeightType> &tree_;
      };
      
    public:
      Traversal(const MorphologicalTree<WeightType> &tree);
      Iterator begin() const;
      Iterator end() const;

    private:
      const MorphologicalTree<WeightType> &tree_;      
    };


    class TraversalByLevel
    {
    public:
      class Iterator
      {
      public:
        using MTree = MorphologicalTree<WeightType>;
        using NodePtr = typename MTNode<WeightType>::NodePtr;

        Iterator(std::queue<NodePtr> *queue);

        inline const NodePtr operator*() const;
        Iterator &operator++();
        bool operator==(const Iterator &it) const;
        bool operator!=(const Iterator &it) const { return !(*this == it); }

      private:
        std::queue<NodePtr> *queue_;
      };

      TraversalByLevel(const MorphologicalTree<WeightType> &tree);
      Iterator begin();
      Iterator end();
    
    private:
      const MorphologicalTree<WeightType>& tree_;
      std::queue<typename Iterator::NodePtr> queue_;
      std::queue<typename Iterator::NodePtr> emptyQueue_;
    };


    using NodePtr = typename MTNode<WeightType>::NodePtr; 
    using NodeType = MTNode<WeightType>;
    using TreeWeightType = WeightType;

    MorphologicalTree(MorphoTreeType type, const std::vector<WeightType> &f, const CTBuilderResult &res);
    MorphologicalTree(MorphoTreeType type, std::vector<uint32> &&cmap, std::vector<NodePtr> &&nodes);
    MorphologicalTree(MorphoTreeType type);

    const NodePtr node(uint32 id) const { return nodes_[id]; }
    NodePtr node(uint32 id) { return nodes_[id]; }
    
    const NodePtr root() const { return root_; }
    NodePtr root() { return root_; }

    inline std::vector<uint32> reconstructNode(uint32 nodeId) const { return nodes_[nodeId]->reconstruct(); }
    std::vector<bool> reconstructNode(uint32 nodeId, const Box &domain) const { return nodes_[nodeId]->reconstruct(domain); };

    std::vector<uint32> reconstructNodes(std::function<bool(NodePtr)> keep) const;
    std::vector<bool> reconstructNodes(std::function<bool(NodePtr)> keep, const Box &domain) const;

    uint32 numberOfNodes() const { return nodes_.size(); }

    uint32 numberOfCNPs() const { return cmap_.size(); }

    // Iterators
    Traversal nodes() const { return Traversal{*this}; }
    TraversalByLevel nodesByLevel() const { return TraversalByLevel{*this}; }

    std::vector<WeightType> reconstructImage() const;
    std::vector<WeightType> reconstructImage(std::function<bool(const NodePtr)> keep) const;

    void idirectFilter(std::function<bool(const NodePtr)> keep);

    MorphologicalTree<WeightType> directFilter(std::function<bool(const NodePtr)> keep) const;    

    inline NodePtr smallComponent(uint32 idx) { return nodes_[cmap_[idx]]; }    
    inline const NodePtr smallComponent(uint32 idx) const { return nodes_[cmap_[idx]]; }
    
    NodePtr smallComponent(uint32 idx, const std::vector<bool> &mask);
    const NodePtr smallComponent(uint32 idx, const std::vector<bool> &mask) const;

    MorphologicalTree<WeightType> copy() const;

    inline MorphoTreeType type() const { return type_; }

    static const uint32 UndefinedIndex;

  private:
    void performDirectFilter(MorphologicalTree<WeightType> &tree, std::function<bool(const NodePtr)> keep) const;
    
  private:
    std::vector<NodePtr> nodes_;
    std::vector<uint32> cmap_;
    NodePtr root_;
    MorphoTreeType type_;
  };

  template<class WeightType> 
  MorphologicalTree<WeightType> buildMaxTree(const std::vector<WeightType> &f,
    std::shared_ptr<Adjacency> adj);

  template<class WeightType>
  MorphologicalTree<WeightType> buildMinTree(const std::vector<WeightType> &f,
    std::shared_ptr<Adjacency> adj);


  // ======================[ IMPLEMENTATION ] ===================================================================

  // ===================[ TRAVERSAL ITERATORS ] =================================================================
  template<typename WeightType>
  MorphologicalTree<WeightType>::Traversal::Traversal(const MorphologicalTree<WeightType> &tree)
    :tree_{tree}
  {}

  template<typename WeightType>
  typename MorphologicalTree<WeightType>::Traversal::Iterator 
    MorphologicalTree<WeightType>::Traversal::begin() const 
  { 
    return Iterator{tree_, 0}; 
  }

  template<typename WeightType>
  typename MorphologicalTree<WeightType>::Traversal::Iterator 
    MorphologicalTree<WeightType>::Traversal::end() const 
  { 
    return Iterator{tree_, tree_.numberOfNodes()}; 
  }

  template<typename WeightType>
  MorphologicalTree<WeightType>::Traversal::Iterator::Iterator(
    const MorphologicalTree<WeightType> &tree, uint32 index)
    : tree_{tree}, index_{index}
  {} 

  template<typename WeightType>
  const typename MorphologicalTree<WeightType>::NodePtr 
    MorphologicalTree<WeightType>::Traversal::Iterator::operator*() const 
  { 
    return tree_.nodes_[tree_.numberOfNodes() - index_ - 1];
  }

  template<typename WeightType>
  typename MorphologicalTree<WeightType>::Traversal::Iterator& 
  MorphologicalTree<WeightType>::Traversal::Iterator::operator++() {
    ++index_;
    return *this;
  }
        
  template<typename WeightType>
  bool MorphologicalTree<WeightType>::Traversal::Iterator::operator==(
    const Iterator &it) const 
  {
    return index_ == it.index_;
  }

  // Traversal per level
  template<typename WeightType>
  MorphologicalTree<WeightType>::TraversalByLevel::TraversalByLevel(
    const MorphologicalTree<WeightType> &tree) : tree_{tree}
  {}

  template<typename WeightType>
  typename MorphologicalTree<WeightType>::TraversalByLevel::Iterator 
    MorphologicalTree<WeightType>::TraversalByLevel::begin()
  {
    queue_ = std::queue<NodePtr>();
    queue_.push(tree_.root());
    return Iterator{&queue_};
  }
  
  template<typename WeightType>
  typename MorphologicalTree<WeightType>::TraversalByLevel::Iterator
    MorphologicalTree<WeightType>::TraversalByLevel::end()
  {        
    return Iterator{&emptyQueue_};
  }

  template<typename WeightType>
  MorphologicalTree<WeightType>::TraversalByLevel::Iterator::Iterator(
    std::queue<NodePtr> *queue) : queue_{queue}
  {}
  
  template<typename WeightType>
  const typename MorphologicalTree<WeightType>::TraversalByLevel::Iterator::NodePtr 
    MorphologicalTree<WeightType>::TraversalByLevel::Iterator::operator*() const
  {
    return queue_->front();
  }


  template<typename WeightType>
  typename MorphologicalTree<WeightType>::TraversalByLevel::Iterator& 
    MorphologicalTree<WeightType>::TraversalByLevel::Iterator::operator++()
  {
    NodePtr node = queue_->front();
    for (NodePtr c : node->children()) {
      queue_->push(c);
    }
    queue_->pop();
    return *this;
  }

  template<typename WeightType>  
  bool MorphologicalTree<WeightType>::TraversalByLevel::Iterator::operator==(
    const Iterator &it) const
  {
    uint32 myId = queue_->empty() ? MTree::UndefinedIndex : queue_->front()->id();
    uint32 otherId = it.queue_->empty() ? MTree::UndefinedIndex : it.queue_->front()->id();

    return myId == otherId;
  }

  // ================= [ TREE ] ================================================================================
  template<typename WeightType>
  const uint32 MorphologicalTree<WeightType>::UndefinedIndex = std::numeric_limits<uint32>::max();

  template<class WeightType>
  MTNode<WeightType>::MTNode(uint32 id)
    :id_{id}, level_{0}, parent_{nullptr}
  {}

  template<class WeightType>
  std::vector<uint32> MTNode<WeightType>::reconstruct() const
  {
    std::vector<uint32> pixels{cnps_};
    for (NodePtr child : children_) {
      reconstruct(pixels, child);
    }
    return pixels;
  }

  template<class WeightType>
  void MTNode<WeightType>::reconstruct(std::vector<uint32> &pixels, const NodePtr node) const
  {
    pixels.insert(pixels.end(), node->cnps().begin(), node->cnps().end());
    for (NodePtr child: node->children()) {
      reconstruct(pixels, child);
    }
  }

  template<class WeightType>
  std::vector<bool> MTNode<WeightType>::reconstruct(const Box &domain) const
  {
    std::vector<uint32> indices = reconstruct();
    std::vector<bool> img(domain.numberOfPoints(), false);
    for (uint32 i : indices) {
      img[i] = true;
    }
    return img;
  }

  template<class WeightType>
  std::vector<WeightType>  MTNode<WeightType>::reconstructGrey(const Box &domain,
    WeightType backgroundValue) const
  {
    std::vector<WeightType> f(domain.numberOfPoints(), backgroundValue);
    
    for (uint32 p : cnps()) {
      f[p] = level();
    }
    for (NodePtr child :children()) {
      reconstructGrey(child, domain, f);
    }
    
    return f;
  }

  template<class WeightType>
  void MTNode<WeightType>::reconstructGrey(NodePtr node, const Box &domain, 
    std::vector<WeightType> &f) const
  {
    for (uint32 p : node->cnps()) {
      f[p] = node->level();
    }
    for (NodePtr c : node->children()) {
      reconstructGrey(c, domain, f);
    }
  }

  template<class WeightType>
  void MTNode<WeightType>::includeCNPS(const std::vector<uint32>& cnps)
  {
    cnps_.insert(cnps_.end(), cnps.begin(), cnps.end());
  }

  template<class WeightType>
  void MTNode<WeightType>::includeChildren(const std::vector<NodePtr> &children)
  {
    children_.insert(children_.end(), children.begin(), children.end());
  }

  template<class WeightType>
  typename MTNode<WeightType>::NodePtr MTNode<WeightType>::copy() const
  {
    NodePtr cnode = std::make_shared<MTNode<WeightType>>(id_);
    cnode->level(level_);  
    cnode->cnps_ = cnps_;
    cnode->representative_ = representative_;
    return cnode;
  }

  // ========================== [TREEE] =========================================================================
  template<class WeightType>
  MorphologicalTree<WeightType>::MorphologicalTree(MorphoTreeType type)
    :type_{type}
  { }

  template<class WeightType>
  MorphologicalTree<WeightType>::MorphologicalTree(MorphoTreeType type, 
    std::vector<uint32> &&cmap, std::vector<NodePtr> &&nodes)
    :cmap_{cmap}, nodes_{nodes}, type_{type}
  {
    root_ = nodes_[0];
  }

  template<class WeightType>
  MorphologicalTree<WeightType>::MorphologicalTree(MorphoTreeType type, 
    const std::vector<WeightType> &f,  const CTBuilderResult &res)
    :type_{type}
  {
    const uint32 UNDEF = std::numeric_limits<uint32>::max();
    std::vector<uint32> sortedLevelRoots;
    cmap_.resize(f.size(), UNDEF);

    for (uint32 i = 0; i < res.R.size(); i++) {
      uint32 p = res.R[i];
      if (f[res.parent[p]] != f[p] || res.parent[p] == p)
        sortedLevelRoots.push_back(p);
    }

    nodes_.resize(sortedLevelRoots.size(), nullptr);

    uint32 p = sortedLevelRoots[sortedLevelRoots.size()-1];
    NodePtr root = std::make_shared<NodeType>(0);
    root->level(f[p]);
    root->appendCNP(p);
    root->representative(p);
    root->parent(nullptr);
    cmap_[p] = root->id();
    nodes_[0] = root;
    root_ = root;

    
    for (uint32 i = 2; i <= sortedLevelRoots.size(); i++) {
      uint32 p = sortedLevelRoots[sortedLevelRoots.size()-i];
      cmap_[p] = i-1; 
      
      NodePtr node = std::make_shared<NodeType>(i-1);
      NodePtr parentNode = nodes_[cmap_[res.parent[p]]]; 
      node->id(i-1);
      node->parent(parentNode);
      node->level(f[p]);
      node->appendCNP(p);
      node->representative(p);
      parentNode->appendChild(node);

      nodes_[i-1] = node; 
      
    }

    for (uint32 i = 0; i < f.size(); i++) {
      if (cmap_[i] == UNDEF) {
        cmap_[i] = cmap_[res.parent[i]];
        nodes_[cmap_[i]]->appendCNP(i);
      }
    }
  }

  template<typename WeightType>
  std::vector<uint32> 
  MorphologicalTree<WeightType>::reconstructNodes(std::function<bool(NodePtr)> keep) const
  {
    std::vector<uint32> rec;
    
    std::stack<NodePtr> s;
    s.push(root_);

    while (!s.empty()) {
      NodePtr n = s.top();
      s.pop();

      if ((n->id() != root_->id()) && keep(n)) {
        std::vector<uint32> recn = n->reconstruct();
        rec.insert(rec.end(), recn.begin(), recn.end());
      }
      else {
        for (NodePtr c : n->children()) 
          s.push(c);
      }
    }

    return rec;
  }


  template<typename WeightType>  
  std::vector<bool> MorphologicalTree<WeightType>::reconstructNodes(std::function<
    bool(NodePtr)> keep, const Box &domain) const
  {
    std::vector<bool> bin(domain.numberOfPoints(), false);
    std::vector<uint32> pixels = reconstructNodes(keep);

    for (const uint32 pidx : pixels) {
      bin[pidx] = true;
    }

    return bin;
  }
  
  template<class WeightType> 
  MorphologicalTree<WeightType> buildMaxTree(const std::vector<WeightType> &f,
    std::shared_ptr<Adjacency> adj)
  {
    CTBuilder<WeightType> builder;
    std::vector<uint32> R = sortIncreasing(f);
    return MorphologicalTree<WeightType>(MorphoTreeType::MaxTree, f, builder.build(f, adj, R));
  }

  template<class WeightType>
  MorphologicalTree<WeightType> buildMinTree(const std::vector<WeightType> &f,
    std::shared_ptr<Adjacency> adj)
  {
    CTBuilder<WeightType> builder;
    std::vector<uint32> R = sortDecreasing(f);
    return MorphologicalTree<WeightType>(MorphoTreeType::MinTree, f, builder.build(f, adj, R));
  }

  template<typename WeightType>
  std::vector<WeightType> MorphologicalTree<WeightType>::reconstructImage() const
  {
    std::vector<WeightType> f(cmap_.size());
    for (NodePtr node : nodes_) {
      for (uint idx : node->cnps()) {
        f[idx] = node->level();
      }
    }
    return f;
  }
  template<typename WeightType>
  std::vector<WeightType> MorphologicalTree<WeightType>::reconstructImage(
    std::function<bool(const NodePtr)> keep) const
  {
    using namespace std;
    vector<vector<uint32>> up(numberOfNodes(), vector<uint32>());
    vector<WeightType> f(cmap_.size(), 0);

    for (NodePtr node : nodes()) {
      if (keep(node)) {
        for (uint32 pidx : nodes->cnp())
          f[pidx] = node->level();
        
        for (uint32 pdix : up[node->id()])
          f[pdix] = node->level();  
      }
      else {
        const vector<uint32> &cnps = node->cnps();
        const vector<uint32> &upCNPs = up[node->id()];
        vector<uint32> &upParent = up[node->parent()->id()];

        upParent.insert(upParent.end(), cnps.begin(), cnps.end());
        upParent.insert(upParent.end(), upCNPs.begin(), upCNPs.end());
      }
    }

    return f;
  }

  template<class WeightType>
  void MorphologicalTree<WeightType>::idirectFilter(std::function<bool(const NodePtr)> keep)
  {
    performDirectFilter(*this, keep);
  }

  template<class WeightType>
  MorphologicalTree<WeightType> MorphologicalTree<WeightType>::directFilter(
    std::function<bool(const NodePtr)> keep) const
  {
    MorphologicalTree<WeightType> ctree = copy();
    performDirectFilter(ctree, keep);
    return ctree;
  }

  template<typename WeightType>
  void MorphologicalTree<WeightType>::performDirectFilter(MorphologicalTree<WeightType> &tree, 
    std::function<bool(const NodePtr)> keep) const 
  {
    // remove node from data structure.
    uint32 numRemovedNodes = 0;
      
    std::queue<NodePtr> queue;
    queue.push(tree.root());

    while (!queue.empty())
    {
      NodePtr node = queue.front();
      queue.pop();
      
      for (NodePtr c : node->children()) {
        queue.push(c);
      }

      if (!keep(node) && node->parent() != nullptr) {
        numRemovedNodes++;  
        NodePtr parentNode = node->parent();
        parentNode->includeCNPS(node->cnps());
        parentNode->removeChild(node);
        for (NodePtr c : node->children()) {
          parentNode->appendChild(c);
          c->parent(parentNode);
        }
      }
    }

    // fix cmap and nodes arrays
    uint32 prevNumOfNodes = tree.numberOfNodes();
    tree.nodes_.clear();
    tree.nodes_.resize(prevNumOfNodes - numRemovedNodes);
    uint32 newId = 0;

    for (NodePtr n : nodesByLevel()) {
      n->id(newId);
      tree.nodes_[newId] = n;
      for (uint32 idx : n->cnps()) {
        tree.cmap_[idx] = newId;
      }
      newId++;
    }
  }

  template<class WeightType>
  typename MorphologicalTree<WeightType>::NodePtr 
  MorphologicalTree<WeightType>::smallComponent(uint32 idx, const std::vector<bool> &mask)
  {
    NodePtr node = nodes_[cmap_[idx]];
    while (!mask[node->id()] && node->id() != 0) 
      node = node->parent();
    
    return node;
  }

  template<class WeightType>
  const typename MorphologicalTree<WeightType>::NodePtr 
  MorphologicalTree<WeightType>::smallComponent(uint32 idx, const std::vector<bool> &mask) const
  {
    NodePtr node = nodes_[cmap_[idx]];
    while (!mask[node->id()] && node->id() != 0) 
      node = node->parent();
    
    return node;
  }

  template<class WeightType>
  MorphologicalTree<WeightType> MorphologicalTree<WeightType>::copy() const
  {
    MorphologicalTree ctree{type_};
    ctree.nodes_.reserve(numberOfNodes());
    ctree.cmap_ = cmap_;

    for (NodePtr node : nodes_) {
      ctree.nodes_.push_back(node->copy());
    }

    for (NodePtr node : nodesByLevel()) {
      NodePtr cnode = ctree.nodes_[node->id()];
      
      if (node->parent() != nullptr) {
        NodePtr cparent = ctree.nodes_[node->parent()->id()];
        cnode->parent(cparent);
        cparent->appendChild(cnode); 
      }
    }

    ctree.root_ = ctree.nodes_[0];
    return ctree;
  }
} 