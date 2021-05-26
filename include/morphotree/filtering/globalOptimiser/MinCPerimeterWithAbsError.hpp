#pragma once

#include "morphotree/tree/mtree.hpp"

#include <cmath>

namespace morphotree
{
  template<typename ValueType>
  class MinCPerimeterWithAbsError
  {
  protected:
    struct AbsErrorAccPerimeterPair 
    {
      float sumPerimeter;
      float absError;
    };

  public:
    using MTree = MorphologicalTree<ValueType>;
    using NodePtr = typename MTree::NodePtr;

    MinCPerimeterWithAbsError(const Box &domain, const std::vector<ValueType> &f, 
      std::string dtFilename, const MTree &tree, float approxThreshold);

    inline float absError() const { return currAbsError_; }
    inline float sumPerimeter() const { return currSumPerimeter_; }
    inline uint32 numFilteredNodes() const { return numFilteredNodes_; }
    inline float normAbsError() const { return currAbsError_ / maxError_; }


    MTree filter(float absErrorThreshold);
    MTree filterWithNormalisedError(float normAbsErrorThreshold);

    float computeAccPerimeter(const std::vector<float> &accPerimeter, NodePtr node);
    float computeFilteredChildrenAccPerimeter(const std::vector<float> &accPerimteter, NodePtr node);
    float computeFinalAccPerimeter(const std::vector<float> &accPerimeter, const MTree &tree);

    float computeAbsError(const std::vector<float> &absError, NodePtr node);
    float computeFilteredChildrenAbsError(const std::vector<float> &absError, NodePtr node);
    float computeFinalAbsError(const std::vector<float> &absError, const MTree &tree);

  protected:
    float computeAbsErrorPixelSet(const std::vector<uint32> &points, ValueType level) const;
    void computeCPerimeter(const MTree &tree);

    void filterNodesFromMorphoTree(MTree &tree);
    void performFilter(MTree &tree, float absErrorThreshold);
    AbsErrorAccPerimeterPair bottomUpAnalysis(MTree &tree, float lambda);

    float computeMaxError() const;

    void resetKeepMapping();
    void resetAreaError();
    bool approx(float v0, float v1) const;

  protected:
    float approxThreshold_;
    const MTree &tree_;
    float currAbsError_;
    float currSumPerimeter_;
    uint32 numFilteredNodes_;
    std::vector<bool> keep_;

    std::vector<ValueType> f_;    
    Box domain_;
    std::vector<float> cperimeter_;
    std::string dtFilename_;
    float maxError_;

    std::vector<uint32> errorArea_;
  }; 

// ========================= [ IMPLEMENTATION ] ==========================================================
  template<typename ValueType>
  MinCPerimeterWithAbsError<ValueType>::MinCPerimeterWithAbsError(const Box &domain, 
    const std::vector<ValueType> &f, std::string dtFilename, const MTree &tree, 
    float approxThreshold)
    :approxThreshold_{approxThreshold},
     tree_{tree},
     currAbsError_{0.0f},
     currSumPerimeter_{0.0f},
     numFilteredNodes_{0},
     f_{f},
     domain_{domain},
     dtFilename_{dtFilename}  
  {
    keep_.resize(tree.numberOfNodes(), true);
    errorArea_.resize(tree.numberOfNodes(), 0);
    maxError_ = computeMaxError();
    computeCPerimeter(tree);    
  }

  template<typename ValueType>
  void MinCPerimeterWithAbsError<ValueType>::computeCPerimeter(const MTree &tree)
  {
    std::unique_ptr<AttributeComputer<Quads, ValueType>> quadComputer = 
      std::make_unique<CTreeQuadCountsComputer<ValueType>>(domain_, f_, dtFilename_);

    cperimeter_.resize(tree.numberOfNodes(), 0.0f);

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

  template<typename ValueType>
  float MinCPerimeterWithAbsError<ValueType>::computeAbsErrorPixelSet
    (const std::vector<uint32> &points, ValueType level) const
  {
    float error = 0.f;
    for (uint32 p : points) 
      error += fabsf32( level - f_[p] );
    
    return error;
  }

  template<typename ValueType>
  void MinCPerimeterWithAbsError<ValueType>::filterNodesFromMorphoTree(MTree &tree)
  {
    tree.idirectFilter([this](NodePtr node) { return keep_[node->id()]; });
  }

  template<typename ValueType>
  void MinCPerimeterWithAbsError<ValueType>::performFilter(MTree &tree, float absErrorThreshold)
  {
    float C0 = absErrorThreshold;
    
    float llambda = 0.0f;   /* Compute continuous perimeter and absolute error for a very low lambda. */
    AbsErrorAccPerimeterPair CD = bottomUpAnalysis(tree, llambda);
    float Cl = CD.absError;
    float Dl = CD.sumPerimeter;

    if (Cl < C0)
      return;

    float hlambda = 10e10f; /* Compute continuous perimeter and absolute error for a very high lambda. */
    CD = bottomUpAnalysis(tree, hlambda);
    float Ch = CD.absError;
    float Dh = CD.sumPerimeter;

    if (Ch > C0)
      return;

    do {   /* Find the optimum lambda value */ 
      float lambda = (Dl - Dh) / (Ch - Cl);
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
  typename MinCPerimeterWithAbsError<ValueType>::AbsErrorAccPerimeterPair 
  MinCPerimeterWithAbsError<ValueType>::bottomUpAnalysis(MTree &tree, 
    float lambda)
  {
    std::vector<float> vsumPerimeter(tree.numberOfNodes(), 0.0f);
    std::vector<float> vabsError(tree.numberOfNodes(), 0.0f);

    resetKeepMapping();
    resetAreaError();
    numFilteredNodes_ = 0;

    tree.tranverse([this, &vabsError, &vsumPerimeter,lambda](NodePtr node) {
      if (node->children().empty()) {
        vsumPerimeter[node->id()] = computeAccPerimeter(vsumPerimeter, node);
        vabsError[node->id()] = computeAbsError(vabsError, node);
      }
      else {
        float CC = computeFilteredChildrenAbsError(vabsError, node);
        float CD = computeFilteredChildrenAccPerimeter(vsumPerimeter, node);

        float C = computeAbsError(vabsError, node);
        float D = computeAccPerimeter(vsumPerimeter, node);

        if ( (CD + (lambda * CC)) <= (D + (lambda * C)) ) {
          // remove children 
          vsumPerimeter[node->id()] = CD;
          vabsError[node->id()] = CC;

          for (NodePtr c : node->children()) {
            keep_[c->id()] = false;
            errorArea_[c->id()] += c->cnps().size();
            errorArea_[node->id()] += errorArea_[c->id()];
            numFilteredNodes_++;
          }           
        }
        else {
          // keep children
          vsumPerimeter[node->id()] = D;
          vabsError[node->id()] = C;

          for (NodePtr c : node->children()) {
            errorArea_[node->id()] += errorArea_[c->id()];            
          }
        }        
      }
    });

    return AbsErrorAccPerimeterPair{ 
      computeFinalAccPerimeter(vsumPerimeter, tree), 
      computeFinalAbsError(vabsError, tree)};
  }

  template<typename ValueType>
  void MinCPerimeterWithAbsError<ValueType>::resetKeepMapping()
  {
    std::fill(keep_.begin(), keep_.end(), true);
  }

  template<typename ValueType>
  void MinCPerimeterWithAbsError<ValueType>::resetAreaError()
  {
    std::fill(errorArea_.begin(), errorArea_.end(), 0);
  }

  template<typename ValueType>
  bool MinCPerimeterWithAbsError<ValueType>::approx(float v0, float v1) const
  {
    return fabs(v0 - v1) <= approxThreshold_;
  }

  template<typename ValueType>
  float MinCPerimeterWithAbsError<ValueType>::computeMaxError() const 
  {
    float error = 0.f;
    ValueType minValue = tree_.root()->level();
    for (uint32 p = 0; p < f_.size(); p++)
      error += fabsf32(f_[p] - minValue);
    
    return error;
  }

  template<typename ValueType>
  typename MinCPerimeterWithAbsError<ValueType>::MTree
    MinCPerimeterWithAbsError<ValueType>::filterWithNormalisedError(float normAbsErrorThreshold)
  {
    return filter(normAbsErrorThreshold * maxError_);
  }

  template<typename ValueType>
  typename MinCPerimeterWithAbsError<ValueType>::MTree 
    MinCPerimeterWithAbsError<ValueType>::filter(float absErrorThreshold) 
  {
    MTree tree = tree_.copy();
    performFilter(tree, absErrorThreshold);
    return tree;
  }

  template<typename ValueType>
  float MinCPerimeterWithAbsError<ValueType>::computeAccPerimeter(
    const std::vector<float> &accPerimeter, NodePtr node)
  {
    float nodeAccPerimeter = cperimeter_[node->id()];
    for (NodePtr child : node->children()) 
      nodeAccPerimeter += accPerimeter[child->id()];
    return nodeAccPerimeter;
  }

  template<typename ValueType>
  float MinCPerimeterWithAbsError<ValueType>::computeFilteredChildrenAccPerimeter(
    const std::vector<float> &accPerimeter, NodePtr node)
  {
    float nodeAccPerimeter = cperimeter_[node->id()];
    for (NodePtr child : node->children()) {
      nodeAccPerimeter += (accPerimeter[child->id()] - cperimeter_[child->id()]);
    }

    return nodeAccPerimeter;
  }

  template<typename ValueType>
  float MinCPerimeterWithAbsError<ValueType>::computeFinalAccPerimeter(
    const std::vector<float> &accPerimeter, const MTree &tree)
  {
    return accPerimeter[tree.root()->id()];
  }

  template<typename ValueType>
  float MinCPerimeterWithAbsError<ValueType>::computeAbsError(
    const std::vector<float> &absError, NodePtr node)
  {
    float error = 0.0f;
    for (NodePtr child : node->children()) {
      error +=  absError[child->id()] ;
    }

    return error;
  }

  template<typename ValueType>
  float MinCPerimeterWithAbsError<ValueType>::computeFilteredChildrenAbsError(
    const std::vector<float> &absError, NodePtr node)
  {
    float error = 0.0f;
    for (NodePtr child : node->children()) {
      uint32 childNodeErrorArea = errorArea_[child->id()] + child->cnps().size();
      error +=  (childNodeErrorArea * fabsf32(node->level() - child->level())) + absError[child->id()];
    }

    return error;
  }

  template<typename ValueType>
  float MinCPerimeterWithAbsError<ValueType>::computeFinalAbsError(
    const std::vector<float> &absError, const MTree &tree)
  {
    return absError[tree.root()->id()];
  }
}
