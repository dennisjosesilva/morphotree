#pragma once 

#include "morphotree/core/alias.hpp"
#include "morphotree/attributes/attributeComputer.hpp"
#include "morphotree/attributes/bitquads/quads.hpp"

#include <fstream>
#include <array>

namespace morphotree 
{
  template<class ValueType>
  class CTreeQuadCountsComputer: public AttributeComputer<Quads, ValueType>
  {
  public:
    using AttrType = Quads;
    using TreeType = typename AttributeComputer<Quads, ValueType>::TreeType;
    using NodePtr = typename TreeType::NodePtr;

    CTreeQuadCountsComputer(Box domain, const std::vector<ValueType> &image,
      const std::string &dtFilename);

    std::vector<Quads> initAttributes(const TreeType &tree);
    void computeInitialValue(std::vector<Quads> &attr, NodePtr node);
    void mergeToParent(std::vector<Quads> &attr, NodePtr node, NodePtr parent);

  private:
    void readDecisionTree(const std::string &dtFilename);
    const std::array<int8, 9>& getCountsFromDT(const I32Point &p);

    bool isLower(const I32Point &p, const I32Point &q) const;
    bool isGreater(const I32Point &p, const I32Point &q) const;

  private:
    const static uint32 NUM_DT_LEAVES = 6561;
    const static std::array<I32Point, 8> offsets;

    MorphoTreeType treeType_;
    std::array<std::array<int8, 9>, NUM_DT_LEAVES> dt_;
    const std::vector<ValueType> &image_;
    Box domain_;
  };

  // ============== [implementation ] ====================================================
  template<class ValueType>
  const std::array<I32Point, 8> CTreeQuadCountsComputer<ValueType>::offsets = 
  std::array<I32Point, 8>{
      I32Point{-1,-1}, I32Point{ 0,-1}, I32Point{ 1,-1}, I32Point{-1, 0},
      I32Point{ 1, 0}, I32Point{-1, 1}, I32Point{ 0, 1}, I32Point{ 1, 1}
  }; 

  template<class ValueType>
  CTreeQuadCountsComputer<ValueType>::CTreeQuadCountsComputer(Box domain, 
    const std::vector<ValueType> &image, const std::string &dtFilename)
    :domain_{domain}, image_{image}
  {
    readDecisionTree(dtFilename);    
  } 

  template<class ValueType>
  void CTreeQuadCountsComputer<ValueType>::readDecisionTree(const std::string &dtFilename)
  {
    std::ifstream in{dtFilename, std::ios::binary};
    std::array<int8, 9> buffer;

    for (uint32 i = 0; i < NUM_DT_LEAVES; ++i) {
      in.read(buffer.data(), buffer.size());
      dt_[i] = buffer;
    }
  }

  template<class ValueType>
  std::vector<Quads> CTreeQuadCountsComputer<ValueType>::initAttributes(const TreeType &tree)
  {
    treeType_ = tree.type();
    return std::vector<Quads>(tree.numberOfNodes(), Quads{});
  }

  template<class ValueType>
  void CTreeQuadCountsComputer<ValueType>::computeInitialValue(std::vector<Quads> &attr, NodePtr node)
  {
    for (uint32 pidx : node->cnps()) {
      const std::array<int8, 9> &c = getCountsFromDT(domain_.indexToPoint(pidx));
      attr[node->id()].q1() += c[Quads::P1] - c[Quads::P1T]; 
      attr[node->id()].q2() += c[Quads::P2] - c[Quads::P2T];
      attr[node->id()].q3() += c[Quads::P3] - c[Quads::P3T];
      attr[node->id()].qd() += c[Quads::PD] - c[Quads::PDT];
      attr[node->id()].q4() += c[Quads::P4];
    }
  }

  template<class ValueType>
  void CTreeQuadCountsComputer<ValueType>::mergeToParent(std::vector<Quads> &attr,
    NodePtr node, NodePtr parent)
  {
    attr[parent->id()].q1() += attr[node->id()].q1();
    attr[parent->id()].q2() += attr[node->id()].q2();
    attr[parent->id()].q3() += attr[node->id()].q3();
    attr[parent->id()].qd() += attr[node->id()].qd();
    attr[parent->id()].q4() += attr[node->id()].q4();
  }

  template<class ValueType>
  const std::array<int8, 9>& CTreeQuadCountsComputer<ValueType>::getCountsFromDT(const I32Point &p)
  {                          
    std::string coding = "11111111";
    for (uint8 i = 0; i < offsets.size(); i++) {
      const I32Point &n = offsets[i];
      I32Point q = p + n;
      if (isLower(p, q)) 
        coding[i] = '0';
      else if (isGreater(p, q)) 
        coding[i] = '2';
    }

    int32 idt = std::stoi(coding, 0, 3);
    return dt_[idt];
  }

  template<class ValueType>
  bool CTreeQuadCountsComputer<ValueType>::isLower(const I32Point &p, const I32Point &q) const
  {
    if (!domain_.contains(q)) 
      return treeType_ == MorphoTreeType::MaxTree;
    return image_[domain_.pointToIndex(q)] < image_[domain_.pointToIndex(p)];
  }
  
  template<class ValueType>
  bool CTreeQuadCountsComputer<ValueType>::isGreater(const I32Point &p, const I32Point &q) const
  {
    if (!domain_.contains(q))
      return treeType_ == MorphoTreeType::MinTree;
    return image_[domain_.pointToIndex(q)] > image_[domain_.pointToIndex(p)];
  }
}