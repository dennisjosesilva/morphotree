#pragma 

#include "morphotree/filtering/globalOptimiser/GlobalOptimisationFilter.hpp"
#include "morphotree/attributes/bitquads/quadCountComputer.hpp"

#include <vector>

namespace morphotree 
{
  template<typename ValueType>
  class MinCPerimeterWithMSE: public GlobalOptimisationFilter<ValueType>
  {
  public:
    using MTree = typename GlobalOptimisationFilter<ValueType>::MTree;
    using NodePtr = typename MTree::NodePtr;
    
    MinCPerimeterWithMSE(const Box &domain, const std::vector<ValueType> &f,
      std::string dtFilename, const MTree &tree, 
      float approxThrehold=0.001f);

  protected:
    virtual float computeCriterion(const std::vector<float>& vcriterion, NodePtr node) override;
    virtual float computeFilteredChildrenCriterion(const std::vector<float>& vcriterion, NodePtr node) override;

    virtual float computeConstraint(const std::vector<float> &vconstraint, NodePtr node) override;
    virtual float computeFilteredChildrenConstraint(const std::vector<float> &vconstraint, NodePtr node) override;

    virtual void filterNodesFromTheMorphoTree(MTree &tree) override;

  protected:
    float squaredError(const std::vector<uint32>& points, ValueType level) const;
    void computeCPerimeter(const MTree &tree);

  protected:
    std::vector<float> cperimeter_;
    std::vector<ValueType> f_;
    Box domain_;
    std::string dtFilename_;
  };


// =============================== [ IMPLEMENTATION ] =======================================================

  template<typename ValueType>
  MinCPerimeterWithMSE<ValueType>::MinCPerimeterWithMSE(const Box &domain, const std::vector<ValueType> &f,
    std::string dtFilename, const MTree &tree, float approxThreshold)
    :GlobalOptimisationFilter<ValueType>{tree, approxThreshold}, 
     f_{f}, domain_{domain}, dtFilename_{dtFilename}
  {
    computeCPerimeter(tree);
  }

  template<typename ValueType>
  float MinCPerimeterWithMSE<ValueType>::squaredError(const std::vector<uint32> &points,
    ValueType level) const
  {
    float se = 0.f;
    for (uint32 p : points) {
      float error = f_[p] - level;
      se += (error*error); 
    }
    return se;
  }

  template<typename ValueType>
  float MinCPerimeterWithMSE<ValueType>::computeCriterion(
    const std::vector<float> &vcriterion, NodePtr node) 
  {
    float D = cperimeter_[node->id()];
    for (NodePtr child : node->children()) 
      D += vcriterion[child->id()];
    return D;
  }

  template<typename ValueType>
  float MinCPerimeterWithMSE<ValueType>::computeFilteredChildrenCriterion(
    const std::vector<float> &vcriterion, NodePtr node)
  {
    float D = cperimeter_[node->id()];
    for (NodePtr child : node->children()) {
      D += (vcriterion[child->id()] - cperimeter_[child->id()]);      
    }
    return D;
  }

  template<typename ValueType>
  float MinCPerimeterWithMSE<ValueType>::computeFilteredChildrenConstraint(
    const std::vector<float> &vconstraint, NodePtr node)
  {
    float C = 0;

    for (NodePtr child : node->children()) {
      C += (vconstraint[child->id()] + squaredError(child->cnps(), node->level()));      
    }
    return C;
  }

  template<typename ValueType>
  float MinCPerimeterWithMSE<ValueType>::computeConstraint(
    const std::vector<float> &vconstraint, NodePtr node)
  {
    float C = 0;
    for (NodePtr child : node->children()) {
      C += vconstraint[child->id()];
    }
    return C;
  }

  template<typename ValueType>
  void MinCPerimeterWithMSE<ValueType>::filterNodesFromTheMorphoTree(MTree &tree)
  {    
    tree.idirectFilter([this](NodePtr node) { return this->keep_[node->id()]; });    
  }

  template<typename ValueType>
  void MinCPerimeterWithMSE<ValueType>::computeCPerimeter(const MTree &tree)
  {
    std::unique_ptr<AttributeComputer<Quads, ValueType>> quadComputer = 
      std::make_unique<CTreeQuadCountsComputer<ValueType>>(domain_, f_, dtFilename_);

    cperimeter_.resize(tree.numberOfNodes(), 0.f);

    std::vector<Quads> quads = quadComputer->initAttributes(tree);
    tree.tranverse([this, &quadComputer, &quads](NodePtr node) {
      quadComputer->computeInitialValue(quads, node);

      if (node->parent() != nullptr) {
        quadComputer->mergeToParent(quads, node, node->parent());
      }

      quadComputer->finaliseComputation(quads, node);
      this->cperimeter_[node->id()] = quads[node->id()].continuousPerimeter();
    });
  }
}