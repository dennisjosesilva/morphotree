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
  class MinCPerimeterWithSquaredErrorToS
  {
  protected:
    struct SquaredErrorAccPerimeterPair
    {
      double sumPerimeter;
      double squaredError;
    };

  public:
    using MTree = MorphologicalTree<ValueType>;
    using NodePtr = typename MTree::NodePtr;

    MinCPerimeterWithSquaredErrorToS(const Box &domain, const std::vector<ValueType> &f, 
      double approxThreshold);

    inline double sumPerimeter() const { return sumPerimeter_; }
    inline double squaredError() const { return currSquaredError_; }
    inline double fsumPerimeter() const { return curSumPerimeter_; }
    inline uint32 numberOfNodes() const { return tree_.numberOfNodes(); }

    inline uint32 numPrunnedNodes() const { return numPrunnedNodes_; }
    inline double normSquaredError() const { return currSquaredError_ / maxError_; }
    
    std::vector<ValueType> filter(double squaredErrorThreshold);
    std::vector<ValueType> filterWithNormalisedError(double squaredErrorThreshold);

  protected:
    void buildTreeOfShapesAndComputeCPerimeter();

    double computeAccPerimeter(const std::vector<double> &accPerimeter, NodePtr node);
    double computePrunnedChildrenAccPerimeter(const std::vector<double> &accPerimeter, NodePtr node);
    double computeFinalAccPerimeter(const std::vector<double> &accPerimeter, const MTree &tree);

    double computeSquaredError(const std::vector<double> &squaredError, NodePtr node);
    double computePrunnedChildrenSquaredError(const std::vector<double> &squaredError, NodePtr node);
    double computeFinalSquaredError(const std::vector<double> &squaredError, const MTree &tree);

    void filterNodesFromMorphoTree(MTree &tree);
    void performFilter(MTree &tree, double squaredErrorThreshold);
    SquaredErrorAccPerimeterPair bottomUpAnalysis(MTree &tree, double lambda);

    double computeMaxError() const; 

    void resetKeepMapping();
    void transformKeepToPrunning(const MTree &tree);
    bool approx(double v0, double v1) const;

  private:
    double approxThreshold_;
    MTree tree_;
    double currSquaredError_;
    double curSumPerimeter_;
    double sumPerimeter_;
    uint32 numPrunnedNodes_;
    std::vector<bool> keep_;

    std::vector<ValueType> f_;
    Box domain_;
    std::vector<double> cperimeter_;
    double maxError_;    
  };

  // =================== [ IMPLEMENTATION ] ==========================================================================
  template<typename ValueType>
  MinCPerimeterWithSquaredErrorToS<ValueType>::MinCPerimeterWithSquaredErrorToS(const Box &domain, 
    const std::vector<ValueType> &f, double approxThreshold)
    :approxThreshold_{approxThreshold},
     currSquaredError_{0.0},
     curSumPerimeter_{0.0},
     numPrunnedNodes_{0},
     f_{f},
     tree_{MorphoTreeType::TreeOfShapes},
     domain_{domain}
  {
    buildTreeOfShapesAndComputeCPerimeter();
    maxError_ = computeMaxError();
    keep_.resize(tree_.numberOfNodes(), true);
  }

  template<typename ValueType>
  void MinCPerimeterWithSquaredErrorToS<ValueType>::buildTreeOfShapesAndComputeCPerimeter()
  {
    KGrid<ValueType> F{domain_, f_};
    OrderImageResult<ValueType> r = computeOrderImage(domain_, f_, F);

    MTree etos = buildEnlargedTreeOfShapes(r, F);
    std::unique_ptr<AttributeComputer<Quads, uint8>> quadsComp = 
      std::make_unique<TreeOfShapesQuadCountsComputer<uint8>>(F, r.orderImg);

    std::vector<Quads> quads = quadsComp->computeAttribute(etos);
    cperimeter_.resize(etos.numberOfNodes(), 0.0f);
    
    sumPerimeter_ = 0.0;
    etos.tranverse([&quads, this](NodePtr node) {
      cperimeter_[node->id()] = quads[node->id()].continuousPerimeter();
      sumPerimeter_ += quads[node->id()].continuousPerimeter();
    });

    tree_ = emergeTreeOfShapes(F, etos);
  }

  template<typename ValueType>
  void MinCPerimeterWithSquaredErrorToS<ValueType>::filterNodesFromMorphoTree(MTree &tree)
  {
    transformKeepToPrunning(tree);
    tree.idirectFilter([this](NodePtr node) { return keep_[node->id()]; });
  }

  template<typename ValueType>
  void MinCPerimeterWithSquaredErrorToS<ValueType>::performFilter(MTree &tree, 
    double squaredErrorThreshold)
  {
    double C0 = squaredErrorThreshold;

    double llambda = 0.0f; /* Compute continuous perimeter and squared error for a very low lambda. */
    SquaredErrorAccPerimeterPair CD = bottomUpAnalysis(tree, llambda);
    double Cl = CD.squaredError;
    double Dl = CD.sumPerimeter;

    if (Cl < C0)
      return; 

    if (approx(C0, CD.squaredError)) {
      filterNodesFromMorphoTree(tree);
      curSumPerimeter_ = CD.sumPerimeter;
      currSquaredError_ = CD.squaredError;
      return;
    }

    double hlambda = 10e10; /* compute continuous perimeter and squared error for a very high lambda. */ 
    CD = bottomUpAnalysis(tree, hlambda);
    double Ch = CD.squaredError;
    double Dh = CD.sumPerimeter;

    if (Ch > C0)
      return;

    if (approx(C0, CD.squaredError)) {
      filterNodesFromMorphoTree(tree);
      curSumPerimeter_ = CD.sumPerimeter;
      currSquaredError_ = CD.squaredError;
      return;
    }

    do { /* Find the optimum lambda value  */
      double lambda = (Dl - Dh) / (Ch - Cl);
      CD = bottomUpAnalysis(tree, lambda);

      if (CD.squaredError < C0) {
        Ch = CD.squaredError;
        Dh = CD.sumPerimeter;
      }
      else {
        Cl = CD.squaredError;
        Dl = CD.sumPerimeter;
      }       
    } while (!approx(CD.squaredError, C0));

    filterNodesFromMorphoTree(tree);
    curSumPerimeter_ = CD.sumPerimeter;
    currSquaredError_ = CD.squaredError;
  }

  template<typename ValueType>
  typename MinCPerimeterWithSquaredErrorToS<ValueType>::SquaredErrorAccPerimeterPair
  MinCPerimeterWithSquaredErrorToS<ValueType>::bottomUpAnalysis(MTree &tree, double lambda)
  {
    std::vector<double> vsumPerimeter(tree.numberOfNodes(), 0.0);
    std::vector<double> vsquaredError(tree.numberOfNodes(), 0.0);

    resetKeepMapping();
    numPrunnedNodes_ = 0;
    tree.tranverse([this, &vsquaredError, &vsumPerimeter, lambda](NodePtr node) {
      if (node->children().empty()) {
        vsumPerimeter[node->id()] = computeAccPerimeter(vsumPerimeter, node);
        vsquaredError[node->id()] = computeSquaredError(vsquaredError, node);
      }
      else {
        double CC = computePrunnedChildrenSquaredError(vsquaredError, node);
        double CD = computePrunnedChildrenAccPerimeter(vsumPerimeter, node);

        double C = computeSquaredError(vsquaredError, node);
        double D = computeAccPerimeter(vsumPerimeter, node);

        if ( (CD + (lambda * CC)) < (D + (lambda * C)) ) {
          // prune chidren
          vsumPerimeter[node->id()] = CD;
          vsquaredError[node->id()] = CC;

          for (NodePtr c : node->children()) {
            keep_[c->id()] = false;
            numPrunnedNodes_++;
          }
        }
        else {
          // keep children
          vsumPerimeter[node->id()] = D;
          vsquaredError[node->id()] = C;
        }
      }
    });

    return SquaredErrorAccPerimeterPair{
      computeFinalAccPerimeter(vsumPerimeter, tree),
      computeFinalSquaredError(vsquaredError, tree)};
  }

  template<typename ValueType>
  void MinCPerimeterWithSquaredErrorToS<ValueType>::resetKeepMapping()
  {
    std::fill(keep_.begin(), keep_.end(), true);
  }

  template<typename ValueType>
  bool MinCPerimeterWithSquaredErrorToS<ValueType>::approx(double v0, double v1) const 
  {
    return fabs(v0 - v1) <= approxThreshold_;
  }
  
  template<typename ValueType>
  double MinCPerimeterWithSquaredErrorToS<ValueType>::computeMaxError() const 
  {
    double error = 0.0;
    ValueType rootValue = tree_.root()->level();
    for (uint32 p = 0; p < f_.size(); p++)
      error += pow(f_[p] - rootValue, 2);

    return error;
  }

  template<typename ValueType>
  std::vector<ValueType>
    MinCPerimeterWithSquaredErrorToS<ValueType>::filterWithNormalisedError(double normSquaredErrorThreshold)
  {
    return filter(normSquaredErrorThreshold * maxError_);
  }

  template<typename ValueType>
  std::vector<ValueType>
    MinCPerimeterWithSquaredErrorToS<ValueType>::filter(double squaredErrorThreshold)
  {
    MTree tree = tree_.copy();
    performFilter(tree, squaredErrorThreshold);
    return tree.reconstructImage();
  }

  template<typename ValueType>
  double MinCPerimeterWithSquaredErrorToS<ValueType>::computeAccPerimeter(
    const std::vector<double> &accPerimeter, NodePtr node) 
  {
    double nodeAccPerimeter = cperimeter_[node->id()];
    for (NodePtr child : node->children())
      nodeAccPerimeter += accPerimeter[child->id()];
    
    return nodeAccPerimeter;
  }

  template<typename ValueType>
  double MinCPerimeterWithSquaredErrorToS<ValueType>::computePrunnedChildrenAccPerimeter(
    const std::vector<double> &accPerimeter, NodePtr node)
  {
    return cperimeter_[node->id()];
  }

  template<typename ValueType>
  double MinCPerimeterWithSquaredErrorToS<ValueType>::computeFinalAccPerimeter(
    const std::vector<double> &accPerimeter, const MTree &tree)
  {
    return accPerimeter[tree.root()->id()];
  }

  template<typename ValueType>
  double MinCPerimeterWithSquaredErrorToS<ValueType>::computeSquaredError(
    const std::vector<double> &squaredError, NodePtr node)
  {
    double error = 0.0;
    for (NodePtr child : node->children()) {
      error += squaredError[child->id()];
    }

    return error;
  }

  template<typename ValueType>
  double MinCPerimeterWithSquaredErrorToS<ValueType>::computePrunnedChildrenSquaredError(
    const std::vector<double> &squaredError, NodePtr node)
  {
    double error = 0.0;
    std::stack<NodePtr> sNodes;

    for (NodePtr child : node->children())
      sNodes.push(child);

    while (!sNodes.empty()) {
      NodePtr rn = sNodes.top();
      sNodes.pop();
      error += pow(rn->level() - node->level(), 2) * rn->cnps().size();

      for (NodePtr rchild : rn->children()) {
        sNodes.push(rchild);
      }
    }

    return error;
  }

  template<typename ValueType>
  void MinCPerimeterWithSquaredErrorToS<ValueType>::transformKeepToPrunning(
    const MTree &tree)
  {
    tree.traverseByLevel([this](NodePtr node) {
      if (node->parent() != nullptr) {
        if (!keep_[node->parent()->id()])
          keep_[node->id()] = false;
      }
    });
  }

  template<typename ValueType>
  double MinCPerimeterWithSquaredErrorToS<ValueType>::computeFinalSquaredError(
    const std::vector<double> &squaredError, const MTree &tree)
  {
    return squaredError[tree.root()->id()];
  }
}