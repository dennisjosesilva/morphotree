#pragma once 

#include "morphotree/tree/mtree.hpp"

#include <functional>
#include <cmath>

namespace morphotree 
{
  template<typename ValueType>
  class GlobalOptimisationFilter
  {
  protected:
    struct CriterionConstraintPair
    {
      float criterion;
      float constraint;
    };
    
  public:
    using MTree = MorphologicalTree<ValueType>;
    using NodePtr = typename MTree::NodePtr;

    GlobalOptimisationFilter(const MTree &tree, float approxThreshold=0.001f);

    MTree filter(float constraintValue);

    inline float constraint() const { return currConstraint_; }
    inline float criterion() const { return currCriterion_; }
    inline uint32 numFilteredNodes() const { return numFilteredNodes_; }

  protected:
    virtual float computeCriterion(const std::vector<float> &vcriterion, 
      NodePtr node)  = 0;
    virtual float computeFilteredChildrenCriterion(const std::vector<float> &vcriterion, 
      NodePtr node) = 0;
    virtual float computeConstraint(const std::vector<float> &vconstraint, 
      NodePtr node) = 0;
    virtual float computeFilteredChildrenConstraint(const std::vector<float> &vconstraint, 
      NodePtr node) = 0;

    virtual void filterNodesFromTheMorphoTree(MTree &tree) = 0;

    void performFilter(MTree &tree, float constraintValue);
    CriterionConstraintPair bottomUpAnalysis(MTree &tree, float lambda);

    void resetKeepMapping();

    bool approx(float v0, float v1) const;

  protected:
    float approxThreshold_;    
    const MTree &tree_;
    float currConstraint_;
    float currCriterion_;
    uint32 numFilteredNodes_;
    std::vector<bool> keep_;
  };


  // ================================[ IMPLEMENTATION ] =============================================
  template<typename ValueType>
  GlobalOptimisationFilter<ValueType>::GlobalOptimisationFilter(const MTree &tree,
    float approxThreshold)
    :tree_{tree}, approxThreshold_{approxThreshold}, numFilteredNodes_{0}
  {
    keep_.resize(tree.numberOfNodes(), true);
  }

  template<typename ValueType>
  typename GlobalOptimisationFilter<ValueType>::MTree 
    GlobalOptimisationFilter<ValueType>::filter(float constraintValue) 
  {
    MTree tree = tree_.copy();
    performFilter(tree, constraintValue);    
    return tree;
  }

  template<typename ValueType>
  void GlobalOptimisationFilter<ValueType>::performFilter(MTree &tree, float constraintValue)
  {
    using CriterionConstraintPair = GlobalOptimisationFilter<ValueType>::CriterionConstraintPair;

    float C0 = constraintValue;
    std::vector<bool> keep(tree.numberOfNodes(), true);
        
    float llambda = 0;   /*  Compute criterion and constraint for a very low lambda.  */
    CriterionConstraintPair CD = bottomUpAnalysis(tree, llambda);
    float Cl = CD.constraint;
    float Dl = CD.criterion;

    if (CD.constraint < C0)
      return;

    float hlambda = 10e10f;  /* Compute criterion and constraint for a very high lambda. */
    CD = bottomUpAnalysis(tree, hlambda);
    float Ch = CD.constraint;
    float Dh = CD.criterion;

    if (CD.constraint > C0)
      return; 

    do {   /* Find the optimum lambda value */
      float lambda = (Dl - Dh) / (Ch - Cl);
      CD = bottomUpAnalysis(tree, lambda);

      if (CD.constraint < C0) {
        Ch = CD.constraint;
        Dh = CD.criterion;
      }
      else {
        Cl = CD.constraint;
        Dl = CD.criterion;
      }
        
      
    } while (!approx(CD.constraint, C0));

    filterNodesFromTheMorphoTree(tree);
    currConstraint_ = CD.constraint;
    currCriterion_ = CD.criterion;    
  }

  template<typename ValueType>
  typename GlobalOptimisationFilter<ValueType>::CriterionConstraintPair 
  GlobalOptimisationFilter<ValueType>::bottomUpAnalysis(MTree &tree, float lambda)
  {
    std::vector<float> vcriterion(tree.numberOfNodes(), 0.0f);
    std::vector<float> vconstraint(tree.numberOfNodes(), 0.0f);

    resetKeepMapping();
    numFilteredNodes_ = 0;

    tree.tranverse([this,&vcriterion,&vconstraint,lambda](NodePtr node){
      
      if (node->children().empty()) {
        vcriterion[node->id()] = computeCriterion(vcriterion, node);
        vconstraint[node->id()] = computeConstraint(vconstraint, node);
      }
      else {
        float CC = computeFilteredChildrenConstraint(vconstraint, node);
        float CD = computeFilteredChildrenCriterion(vcriterion, node);

        float C = computeConstraint(vconstraint, node);
        float D = computeCriterion(vcriterion, node);
        
        if ((CD + (lambda*CC)) <= (D + (lambda * C))) {
          // remove children
          vcriterion[node->id()] = CD;
          vconstraint[node->id()] = CC;

          for (NodePtr c : node->children()) {
            this->keep_[c->id()] = false;
            this->numFilteredNodes_++;
          }

        }
        else {
          // keep children
          vcriterion[node->id()] = D;
          vconstraint[node->id()] = C;
        }
      }
    });
    
    return CriterionConstraintPair{ vcriterion[0], vconstraint[0] };
  }
  
  template<typename ValueType>
  void GlobalOptimisationFilter<ValueType>::resetKeepMapping()
  {
    std::fill(keep_.begin(), keep_.end(), true);
  }

  template<typename ValueType>
  bool GlobalOptimisationFilter<ValueType>::approx(float v0, float v1) const
  {
    return (fabsf32( v1 - v0 ) <= approxThreshold_);
  }
}

