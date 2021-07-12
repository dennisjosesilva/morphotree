#pragma once

#include "morphotree/tree/mtree.hpp"
#include "morphotree/tree/treeOfShapes/tos.hpp"
#include "morphotree/attributes/bitquads/quadCountTreeOfShapesComputer.hpp"
#include "morphotree/attributes/bitquads/quads.hpp"

#include <cmath>
#include <queue>
#include <stack>

namespace morphotree
{
  template<typename ValueType>
  class MinCPerimeterWithAbsErrorToS
  {
  protected:
    struct AbsErrorAccPerimeterPair
    {
      double sumPerimeter;
      double absError;
    };

  public:
    using MTree = MorphologicalTree<ValueType>;
    using NodePtr = typename MTree::NodePtr;

    MinCPerimeterWithAbsErrorToS(const Box &domain, const std::vector<ValueType> &f,
      double approxThrehold);

    inline double sumPerimeter() const { return sumPerimeter_; }

    inline double absError() const { return currAbsError_; }
    inline double fsumPerimeter() const { return currSumPerimeter_; }
    inline uint32 numberOfNodes() const { return tree_.numberOfNodes(); }

    inline uint32 sumFilteredNodes() const { return numFilteredNodes_; }
    inline double normAbsError() const { return currAbsError_ / maxError_; }

    std::vector<ValueType> filter(double absErrorThreshold);
    std::vector<ValueType> filterWithNormalisedError(double normAbsErrorThreshold);

  protected:
    void buildTreeOfShapesAndComputeCPerimeter();

    double computeAccPerimeter(const std::vector<double> &accPerimeter, NodePtr node);
    double computeFilteredChildrenAccPerimeter(const std::vector<double> &accPerimeter, NodePtr node);
    double computeFinalAccPerimeter(const std::vector<double> &accPerimeter, const MTree &tree);

    double computeAbsError(const std::vector<double> &absError, NodePtr node);
    double computeFilteredChildrenAbsError(const std::vector<double> &absError, NodePtr node);
    double computeFinalAbsError(const std::vector<double> &absError, const MTree &tree);

    void filterNodesFromMorphoTree(MTree &tree);
    void performFilter(MTree &tree, double absErrorThreshold);
    AbsErrorAccPerimeterPair bottomUpAnalysis(MTree &tree, double lambda);

    double computeMaxError() const;

    void resetKeepMapping();
    bool approx(double v0, double v1) const;

  protected:
    double approxThreshold_;
    MTree tree_;
    double currAbsError_;
    double currSumPerimeter_;
    double sumPerimeter_;
    uint32 numFilteredNodes_;
    std::vector<bool> keep_;

    std::vector<ValueType> f_;
    Box domain_;
    std::vector<double> cperimeter_;
    double maxError_;    
  };


  // ======================= [ IMPLEMENTATION ] ==============================================================
  template<typename ValueType>
  MinCPerimeterWithAbsErrorToS<ValueType>::MinCPerimeterWithAbsErrorToS(const Box &domain,
    const std::vector<ValueType> &f, double approxThreshold)
    :approxThreshold_{approxThreshold},     
     currAbsError_{0.0},
     currSumPerimeter_{0.0},
     numFilteredNodes_{0},
     f_{f},
     tree_{MorphoTreeType::TreeOfShapes},
     domain_{domain}
  {    
    buildTreeOfShapesAndComputeCPerimeter();
    maxError_ = computeMaxError();
    keep_.resize(tree_.numberOfNodes(), true);
  }

  template<typename ValueType>
  void MinCPerimeterWithAbsErrorToS<ValueType>::buildTreeOfShapesAndComputeCPerimeter()
  {
    KGrid<ValueType> F{domain_, f_};
    OrderImageResult<ValueType> r = computeOrderImage(domain_, f_, F);

    MTree etos = buildEnlargedTreeOfShapes(r, F);
    std::unique_ptr<AttributeComputer<Quads, uint8>> quadsComp = 
      std::make_unique<TreeOfShapesQuadCountsComputer<uint8>>(F, r.orderImg);
    
    std::vector<Quads> quads = quadsComp->computeAttribute(etos);
    cperimeter_.resize(etos.numberOfNodes(), 0.0f);

    sumPerimeter_ = 0.0;
    etos.tranverse([&quads,this](NodePtr node){
      cperimeter_[node->id()] = quads[node->id()].continuousPerimeter();
      sumPerimeter_ += quads[node->id()].continuousPerimeter();
    });

    tree_ = emergeTreeOfShapes(F, etos);
  }

  template<typename ValueType>
  void MinCPerimeterWithAbsErrorToS<ValueType>::filterNodesFromMorphoTree(MTree &tree)
  {
    tree.idirectFilter([this](NodePtr node) { return keep_[node->id()]; });
  }

  template<typename ValueType>
  void MinCPerimeterWithAbsErrorToS<ValueType>::performFilter(MTree &tree, double absErrorThreshold)
  {
    double C0 = absErrorThreshold;

    double llambda = 0.0f;  /* Compute continuous perimeter and absolute error for a very low lambda.  */
    AbsErrorAccPerimeterPair CD = bottomUpAnalysis(tree, llambda);
    double Cl = CD.absError;
    double Dl = CD.sumPerimeter;

    if (Cl < C0)
      return;

    double hlambda = 10e10; /* Compute continuous perimeter and absolute error for a very high lambda. */
    CD = bottomUpAnalysis(tree, hlambda);
    double Ch = CD.absError;
    double Dh = CD.sumPerimeter;

    if (Ch > C0)
      return;

    do {  /* Find the optimum lambda value */
      double lambda = (Dl - Dh) / (Ch - Cl);
      CD = bottomUpAnalysis(tree, lambda);

      if (CD.absError < C0) {
        Ch = CD.absError;
        Dh = CD.sumPerimeter;
      }
      else {
        Cl = CD.absError;
        Dl = CD.sumPerimeter;
      } 
    } while (!approx(CD.absError, C0));

    filterNodesFromMorphoTree(tree);
    currSumPerimeter_ = CD.sumPerimeter;
    currAbsError_ = CD.absError;    
  }

  template<typename ValueType>
  typename MinCPerimeterWithAbsErrorToS<ValueType>::AbsErrorAccPerimeterPair
  MinCPerimeterWithAbsErrorToS<ValueType>::bottomUpAnalysis(MTree &tree, double lambda)
  {
    std::vector<double> vsumPerimeter(tree.numberOfNodes(), 0.0f);
    std::vector<double> vabsError(tree.numberOfNodes(), 0.0f);

    resetKeepMapping();
    numFilteredNodes_ = 0;
    tree.tranverse([this, &vabsError, &vsumPerimeter, lambda](NodePtr node) {
      if (node->children().empty()) {
        vsumPerimeter[node->id()] = computeAccPerimeter(vsumPerimeter, node);
        vabsError[node->id()] = computeAbsError(vabsError, node);
      }
      else {
        double CC = computeFilteredChildrenAbsError(vabsError, node);
        double CD = computeFilteredChildrenAccPerimeter(vsumPerimeter, node);

        double C = computeAbsError(vabsError, node);
        double D = computeAccPerimeter(vsumPerimeter, node);

        if ( (CD + (lambda * CC)) < (D + (lambda * C)) ) {
          // remove children
          vsumPerimeter[node->id()] = CD;
          vabsError[node->id()] = CC;

          for (NodePtr c : node->children()) {
            keep_[c->id()] = false;
            numFilteredNodes_++;
          }
        }
        else {
          // keep children
          vsumPerimeter[node->id()] = D;
          vabsError[node->id()] = C;          
        }
      }
    });

    return AbsErrorAccPerimeterPair{
      computeFinalAccPerimeter(vsumPerimeter, tree),
      computeFinalAbsError(vabsError, tree)};
  }

  template<typename ValueType>
  void MinCPerimeterWithAbsErrorToS<ValueType>::resetKeepMapping()
  {
    std::fill(keep_.begin(), keep_.end(), true);
  }

  template<typename ValueType>
  bool MinCPerimeterWithAbsErrorToS<ValueType>::approx(double v0, double v1) const 
  {
    return fabs(v0 - v1) <= approxThreshold_;
  }

  template<typename ValueType>
  double MinCPerimeterWithAbsErrorToS<ValueType>::computeMaxError() const 
  {
    double error = 0.0f;
    ValueType rootValue = tree_.root()->level();
    for (uint32 p = 0; p < f_.size(); p++)
      error += fabsf32(f_[p] - rootValue);

    return error;
  }

  template<typename ValueType>
  std::vector<ValueType>
    MinCPerimeterWithAbsErrorToS<ValueType>::filterWithNormalisedError(double normAbsErrorThreshold)
  {
    return filter(normAbsErrorThreshold * maxError_);
  }

  template<typename ValueType>
  std::vector<ValueType>
    MinCPerimeterWithAbsErrorToS<ValueType>::filter(double absErrorThreshold)
  {
    MTree tree = tree_.copy();
    performFilter(tree, absErrorThreshold);
    return tree.reconstructImage();
  }

  template<typename ValueType>
  double MinCPerimeterWithAbsErrorToS<ValueType>::computeAccPerimeter(
    const std::vector<double> &accPerimeter, NodePtr node)
  {
    double nodeAccPerimeter = cperimeter_[node->id()];
    for (NodePtr child : node->children())
      nodeAccPerimeter += accPerimeter[child->id()];
    
    return nodeAccPerimeter;
  }

  template<typename ValueType>
  double MinCPerimeterWithAbsErrorToS<ValueType>::computeFilteredChildrenAccPerimeter(
    const std::vector<double> &accPerimeter, NodePtr node)
  {
    double nodeAccPerimeter = cperimeter_[node->id()];
    for (NodePtr child : node->children()) {
      nodeAccPerimeter += (accPerimeter[child->id()] - cperimeter_[child->id()]);
    }

    return nodeAccPerimeter;
  }

  template<typename ValueType>
  double MinCPerimeterWithAbsErrorToS<ValueType>::computeFinalAccPerimeter(
    const std::vector<double> &accPerimeter, const MTree &tree)
  {
    return accPerimeter[tree.root()->id()];
  }

  template<typename ValueType>
  double MinCPerimeterWithAbsErrorToS<ValueType>::computeAbsError(
    const std::vector<double> &absError, NodePtr node)
  {
    double error = 0.0f;
    for (NodePtr child : node->children()) {
      error += absError[child->id()];
    }

    return error;
  }

  template<typename ValueType>
  double MinCPerimeterWithAbsErrorToS<ValueType>::computeFilteredChildrenAbsError(
    const std::vector<double> &absError, NodePtr node)
  {
    float error = 0.0f;
    std::stack<NodePtr> sNodes;

    for (NodePtr child : node->children()) {
      sNodes.push(child);
    }

    while (!sNodes.empty()) {
      NodePtr rn = sNodes.top();
      sNodes.pop();
      error += fabsf32(rn->level() - node->level()) * rn->cnps().size();

      for (NodePtr rchild : rn->children()) {
        if (!keep_[rchild->id()])
          sNodes.push(rchild);
      }
    }

    return error;
  }

  template<typename ValueType>
  double MinCPerimeterWithAbsErrorToS<ValueType>::computeFinalAbsError(
    const std::vector<double> &absError, const MTree &tree)
  {
    return absError[tree.root()->id()];
  }
}