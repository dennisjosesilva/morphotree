#pragma once 

#include "morphotree/core/alias.hpp"
#include "morphotree/attributes/attributeComputer.hpp"
#include "morphotree/attributes/bitquads/quads.hpp"
#include "morphotree/tree/treeOfShapes/kgrid.hpp"
#include "morphotree/tree/treeOfShapes/order_image.hpp"

#include <array>
#include <limits>
#include <functional>
#include <unordered_map>


namespace morphotree
{

  class WindowMaxTreeNode
  {
  public:
    WindowMaxTreeNode(
      uint8 id = std::numeric_limits<uint8>::max(),
      uint8 parent=std::numeric_limits<uint8>::max(), 
      uint8 size=std::numeric_limits<uint8>::max(),
      uint32 representative=std::numeric_limits<uint32>::max());

    inline uint8& id() { return id_; }
    inline uint8  id() const { return id_;}
    inline void id(uint8 i) { id_ = i; }

    inline uint8& parent() { return parent_; }
    inline uint8  parent() const { return parent_; }
    inline void   parent(uint8 p) { parent_ = p; }

    inline uint8& size() { return size_; }
    inline uint8  size() const { return size_; }
    inline void   size(uint8 s) { size_ = s; }

    inline uint32& representative() { return representative_; }
    inline uint32  representative() const { return representative_; }
    inline void    representative(uint32 r) { representative_ = r; }  

  private:
    uint8 id_;
    uint8 parent_;
    uint8 size_;
    uint32 representative_;
  };

  class WindowMaxTree
  {
  public:
    WindowMaxTree(const std::vector<uint32> &orderImage, 
      std::shared_ptr<Adjacency> adj);

    void build(const Box::SubBox &wDomain);
    void tranverse(std::function<void(const WindowMaxTreeNode&)> visit);
    
    inline const WindowMaxTreeNode& node(uint8 idx) const { return nodes_[idx]; }

  public:
    static const uint32 UndefinedParent;
    static const uint8 UndefinedNodeId;

  private:
    void sort(const Box::SubBox &wDomain);
    void cleanAttributes();
    void resetSortedArray(const Box::SubBox &wDomain);
    uint32 findRoot(uint32 p);
    void canonise(const Box::SubBox &wDomain);
    void computeSize(const Box::SubBox &wDomain);

  private:
    const std::vector<uint32> &orderImage_;
    std::shared_ptr<Adjacency> adj_;

    std::array<WindowMaxTreeNode, 9> nodes_;
    std::array<uint32, 9>  S_;
    std::array<uint32, 9> parent_;
    std::array<uint32, 9> zpar_; 
    std::array<uint8, 9> tsize_;
    std::array<uint8, 9> size_;
    std::unordered_map<uint32, uint8> cmap_;
    std::array<uint32, 9> sortedLvRoot_;

    uint8 nLevelRoots_;
  };

  template<class ValueType>
  class TreeOfShapesQuadCountsComputer : public AttributeComputer<Quads, ValueType>
  {
  public:
    using AttrType = Quads;
    using TreeType = typename AttributeComputer<Quads, ValueType>::TreeType;
    using NodePtr = typename TreeType::NodePtr;

    TreeOfShapesQuadCountsComputer(const KGrid<ValueType> &kgrid, 
      const std::vector<uint32> &orderImage);

    std::vector<Quads> initAttributes(const TreeType &tree);
    void computeInitialValue(std::vector<Quads> &attr, NodePtr node);
    void mergeToParent(std::vector<Quads> &attr, NodePtr node, NodePtr parent);

  private:
    void extractWindow(std::vector<uint32> &window, const I32Point &topleft);
    void countQuadW(std::vector<Quads> &quads, uint8 quadIdx,
      const NodePtr Nu, const WindowMaxTreeNode &Nw, 
      const WindowMaxTree &wtree, const TreeType &tos);
    bool isQD(const Box::SubBox &wDomain, uint32 greylevel) const;

    void countQuadsOnBorder(const Box &domain, const TreeType tree, std::vector<Quads> &quads);
    void countQuads3nPixels(const Box &domain, const I32Point &p1, const I32Point &p2,
      const I32Point &p3, const TreeType &tree, std::vector<Quads> &quads);
    
  private:
    const KGrid<ValueType> &kgrid_;
    const std::vector<uint32> &orderImage_;    
  };

  // ================================ [ IMPLEMENTATION ] =================================================
  template<class ValueType>
  TreeOfShapesQuadCountsComputer<ValueType>::TreeOfShapesQuadCountsComputer(
    const KGrid<ValueType> &kgrid, const std::vector<uint32> &orderImage)
    :kgrid_{kgrid}, orderImage_{orderImage}
  {}

  template<class ValueType>
  std::vector<Quads> TreeOfShapesQuadCountsComputer<ValueType>::initAttributes(const TreeType &tree)
  {
    std::vector<Quads> quads(tree.numberOfNodes(), Quads{});
    Box edomain = kgrid_.emergeDomain();
    Box idomain = kgrid_.immerseDomain();
    WindowMaxTree wtree{orderImage_, kgrid_.adj()};
    
    for (int32 y = edomain.top(); y < edomain.bottom(); y++) {
      for (int32 x = edomain.left(); x < edomain.right(); x++) {
        I32Point p_prime = kgrid_.immersePoint(I32Point{x, y}); 
        Box::SubBox wDomain = idomain.subBoxFromSize(p_prime, UI32Point{3,3});
        wtree.build(wDomain);
      
        wtree.tranverse([this, &quads, &wtree, &tree, &wDomain](const WindowMaxTreeNode &Nw) {
          uint8 quadIdx = Nw.size()-1;
          const NodePtr Nu = tree.smallComponent(Nw.representative());
          uint32 greylevel = orderImage_[Nu->cnps()[0]];
          if (quadIdx == Quads::P2 &&  isQD(wDomain, greylevel)) {
            countQuadW(quads, Quads::PD, Nu, Nw, wtree, tree);
          }
          else {
            countQuadW(quads, quadIdx, Nu, Nw, wtree, tree);
          }
        });
      }
    }

    countQuadsOnBorder(idomain, tree, quads);
    return quads;
  }
  
  template<class ValueType>
  bool TreeOfShapesQuadCountsComputer<ValueType>::isQD(const Box::SubBox &wDomain, 
    uint32 greylevel) const
  {
    // top left.
    I32Point tl = wDomain.topleft();
    if (orderImage_[wDomain.pointToIndex(tl)] >= greylevel) {
      bool h = (orderImage_[wDomain.pointToIndex(tl + I32Point{1,0})] >= greylevel) &&
        (orderImage_[wDomain.pointToIndex(tl + I32Point{2,0})] >= greylevel);
      bool v = (orderImage_[wDomain.pointToIndex(tl + I32Point{0,1})] >= greylevel) &&
        (orderImage_[wDomain.pointToIndex(tl + I32Point{0,2})] >= greylevel);
      
      return !(h || v);
    }

    // bottom right
    I32Point br = wDomain.bottomright();
    if (orderImage_[wDomain.pointToIndex(br)] >= greylevel) {
      bool h = (orderImage_[wDomain.pointToIndex(br + I32Point{-1, 0})] >= greylevel) &&
        (orderImage_[wDomain.pointToIndex(br + I32Point{-2, 0})] >= greylevel);
      bool v = (orderImage_[wDomain.pointToIndex(br + I32Point{0,-1})] >= greylevel) &&
        (orderImage_[wDomain.pointToIndex(br + I32Point{0, -2})] >= greylevel);
      return !(h || v);
    }

    return true;
  }

  template<class ValueType>
  void TreeOfShapesQuadCountsComputer<ValueType>::countQuadW(std::vector<Quads> &quads, 
    uint8 quadIdx, const NodePtr Nu, const WindowMaxTreeNode &Nw, 
    const WindowMaxTree &wtree, const TreeType &tos)
  {
    quads[Nu->id()][quadIdx]++;
    if (Nw.parent() != WindowMaxTree::UndefinedNodeId) {
      const WindowMaxTreeNode &Pw = wtree.node(Nw.parent());
      uint32 r = Pw.representative();
      const NodePtr Pu = tos.smallComponent(r);
      quads[Pu->id()][quadIdx]--;
    }
  }

  template<class ValueType>
  void TreeOfShapesQuadCountsComputer<ValueType>::countQuadsOnBorder(const Box &domain, 
    const TreeType tree, std::vector<Quads> &quads)
  {
    // count Q1 quads on the four corners of the image domain
    I32Point tl{ domain.left(), domain.top() }, 
             tr{ domain.right(), domain.top() },
             bl{ domain.left(), domain.bottom() },
             br{ domain.right(), domain.bottom() };
    
    quads[tree.smallComponent(domain.pointToIndex(tl))->id()].incQ1();
    quads[tree.smallComponent(domain.pointToIndex(tr))->id()].incQ1();
    quads[tree.smallComponent(domain.pointToIndex(bl))->id()].incQ1();
    quads[tree.smallComponent(domain.pointToIndex(br))->id()].incQ1();

    // count quads on the first and last row of the image
    for (int32 i = domain.left(); i < (domain.right()-1); i += 2) {
      I32Point p1{ i, domain.top() }, p2{ i+1, domain.top() }, p3{ i+2, domain.top() };
      I32Point q1{ i, domain.bottom() }, q2{ i+1, domain.bottom() }, q3{ i+2, domain.bottom() };

      countQuads3nPixels(domain, p1, p2, p3, tree, quads);
      countQuads3nPixels(domain, q1, q2, q3, tree, quads);
    }

    // count quads on the first and last column of the image
    for (int32 i = domain.top(); i < (domain.bottom()-1); i += 2) {
      I32Point p1{ domain.left(), i }, p2 { domain.left(), i+1 }, p3{ domain.left(), i+2 };
      I32Point q1{ domain.right(), i }, q2 { domain.right(), i+1 }, q3 { domain.right(), i+2 };

      countQuads3nPixels(domain, p1, p2, p3, tree, quads);
      countQuads3nPixels(domain, q1, q2, q3, tree, quads);
    }
  }

  template<class ValueType>
  void TreeOfShapesQuadCountsComputer<ValueType>::countQuads3nPixels(const Box &domain, const I32Point &p1, 
    const I32Point &p2, const I32Point &p3, const TreeType &tree, std::vector<Quads> &quads)
  {
    if(orderImage_[domain.pointToIndex(p1)] > orderImage_[domain.pointToIndex(p2)]) {
      quads[tree.smallComponent(domain.pointToIndex(p1))->id()].incQ1();
      quads[tree.smallComponent(domain.pointToIndex(p2))->id()].decQ1();

      if (orderImage_[domain.pointToIndex(p3)] > orderImage_[domain.pointToIndex(p2)]) {
        quads[tree.smallComponent(domain.pointToIndex(p3))->id()].incQ1();
        quads[tree.smallComponent(domain.pointToIndex(p2))->id()].decQ1();
        quads[tree.smallComponent(domain.pointToIndex(p2))->id()].incQ2();
      }
      else { 
        quads[tree.smallComponent(domain.pointToIndex(p3))->id()].incQ2();
      }
    }
    else { // Order[p1] <= Order[p2]
      if (orderImage_[domain.pointToIndex(p1)] < orderImage_[domain.pointToIndex(p3)]) {
        quads[tree.smallComponent(domain.pointToIndex(p3))->id()].incQ1();
        quads[tree.smallComponent(domain.pointToIndex(p1))->id()].decQ1();
        quads[tree.smallComponent(domain.pointToIndex(p1))->id()].incQ2();
      }
      else if (orderImage_[domain.pointToIndex(p1)] > orderImage_[domain.pointToIndex(p3)]) {
        quads[tree.smallComponent(domain.pointToIndex(p1))->id()].incQ1();
        quads[tree.smallComponent(domain.pointToIndex(p3))->id()].decQ1();
        quads[tree.smallComponent(domain.pointToIndex(p3))->id()].incQ2();
      }
      else {
        quads[tree.smallComponent(domain.pointToIndex(p1))->id()].incQ2();
      }
    }
  }
    

  template<class ValueType>
  void TreeOfShapesQuadCountsComputer<ValueType>::extractWindow(std::vector<uint32> &window, 
    const I32Point &topleft)
  {
    const Box& domain = kgrid_.immerseDomain();
    window[0] = orderImage_[domain.pointToIndex(topleft)]; 
    window[1] = orderImage_[domain.pointToIndex(topleft + I32Point{1,0})];
    window[2] = orderImage_[domain.pointToIndex(topleft + I32Point{3,0})];
    window[3] = orderImage_[domain.pointToIndex(topleft + I32Point{0,1})];
    window[4] = orderImage_[domain.pointToIndex(topleft + I32Point{1,1})];
    window[5] = orderImage_[domain.pointToIndex(topleft + I32Point{2,1})];
    window[6] = orderImage_[domain.pointToIndex(topleft + I32Point{0,2})];
    window[7] = orderImage_[domain.pointToIndex(topleft + I32Point{1,2})];
    window[8] = orderImage_[domain.pointToIndex(topleft + I32Point{2,2})];
  }


  template<class ValueType>
  void TreeOfShapesQuadCountsComputer<ValueType>::computeInitialValue(std::vector<Quads> &attr, NodePtr node)
  {  
     // this method is intentionally left in blank.
  }



  template<class ValueType>
  void TreeOfShapesQuadCountsComputer<ValueType>::mergeToParent(std::vector<Quads> &attr, NodePtr node,
    NodePtr parent)
  {
    attr[parent->id()].q1() += attr[node->id()].q1();
    attr[parent->id()].q2() += attr[node->id()].q2();
    attr[parent->id()].q3() += attr[node->id()].q3();
    attr[parent->id()].qd() += attr[node->id()].qd();
    attr[parent->id()].q4() += attr[node->id()].q4();
  }
}