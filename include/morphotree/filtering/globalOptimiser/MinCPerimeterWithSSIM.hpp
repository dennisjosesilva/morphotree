#pragma once 

#include "morphotree/tree/mtree.hpp"
#include "morphotree/attributes/bitquads/quadCountComputer.hpp"

namespace morphotree
{
  template<typename ValueType>
  class MinCPerimeterWithSSIM
  {
  protected:
    struct SSIMAccPerimeterPair
    {
      float sumPerimeter;
      float ssim;
    };

  public:
    using MTree = MorphologicalTree<ValueType>;
    using NodePtr = typename MTree::NodePtr;

    MinCPerimeterWithSSIM(const Box &domain, const std::vector<ValueType> &f,
      std::string dtFilename, const MTree &tree, float approxThreshold);

    inline float ssim() const { return curSSIM_; }
    inline float sumPerimeter() const { return curSumPerimeter_; }
    inline uint32 numFilteredNodes() const { return numFilteredNodes_; }

    MTree filter(float ssimThreshold);

    float computeAccPerimeter(std::vector<float> &accPerimeter, 
      NodePtr node);
    float computeFiltetedChildrenAccPerimter(
      const std::vector<float> &accPerimeter, NodePtr node);
    float computeFinalAccPerimeter(const std::vector<float> &accPerimeter, 
      const MTree &tree);

    float computeSSIM(const std::vector<float> &ssim, NodePtr node);
    float computeFilteredChildrenSSIM(const std::vector<float> &ssim, 
      NodePtr node);
    float computeFinalSSIM(const std::vector<float> &ssim, const MTree &tree);

  protected:
    void computeCPerimeter(const MTree &tree);

    void filterNodesFromMorphoTree(MTree &tree);
    void performFilter(MTree &tree, float ssimError);
    SSIMAccPerimeterPair bottomUpAnalysis(MTree &tree, float lambda);

    float computeMaxError() const;

    void resetKeepMapping();
    bool approx(float v0, float v1);

  private:
    float approxThrehold_;
    const MTree &tree_;
    float curSSIM_;
    float curSumPerimeter_;
    uint32 numFilteredNodes_;
    std::vector<bool> keep_;

    std::vector<ValueType> f_;
    Box domain_;
    std::vector<float> cperimeter_;
    std::string dtFilename_;
    float maxSSIM_;

    std::vector<uint32> errorArea_;    
  };    

  // ====================== [ IMPLEMENTATION ] ===================================
  template<typename ValueType>
  MinCPerimeterWithSSIM<ValueType>::MinCPerimeterWithSSIM(const Box &domain, 
    const std::vector<ValueType> &f, std::string dtFilename, const MTree &tree,
    float approxThrehold)
    : approxThrehold_{approxThrehold},
      tree_{tree},
      curSSIM_{0.0f},
      curSumPerimeter_{0.0f},
      numFilteredNodes_{0},
      f_{f},
      domain_{domain},
      dtFilename_{dtFilename}
  {
    keep_.resize(tree.numberOfNodes(), true);
    computeCPerimeter(tree);
  }

  template<typename ValueType>
  void MinCPerimeterWithSSIM<ValueType>::computeCPerimeter(const MTree &tree)
  {
    using AttrComputer = AttributeComputer<Quad, ValueType>;
    using QuadCounter = CTreeQuadCountsComputer<ValueType>;

    std::unique_ptr<AttrComputer> quadComputer = 
      std::make_unique<QuadCounter>(domain_, f_, dtFilename_);
    
    cperimeter_.resize(tree.numberOfNodes(), 0.0f);

    std::vector<Quads> quads = quadComputer->initAttributes(tree);
    tree.tranverse([this, &quadComputer, &quads](NodePtr node){
      quadComputer->computeInitialValue(quads, node);

      if (node->parent() != nullptr) {
        quadComputer->mergeToParent(quads, node, node->parent());
      }

      quadComputer->finaliseComputation(quads, node);
      this->cperimeter_[node->id()] = quads[node->id()].continuousPerimeter();
    });
  }

  template<typename ValueType>
  void MinCPerimeterWithSSIM<ValueType>::filterNodesFromMorphoTree(MTree &tree)
  {
    tree.idirectFilter([this](NodePtr node) { return keep_[node->id()]; });
  }

  // TODO : Finish imeplementation
  template<typename ValueType>
  void MinCPerimeterWithSSIM<ValueType>::performFilter(MTree &tree, float ssimThreshold)
  {    
    float C0 = ssimThreshold;

    float llambda = 0.0f; // Compute continuous perimeter and absolute error for a very low lambda.
    SSIMAccPerimeterPair CD = bottomUpAnalysis(tree, llambda);
    float Cl = CD.ssim;
    float Dl = CD.sumPerimeter;

    if (Cl < C0)
      return;

    float hlambda = 10e10; // Compute continuous perimeter and absolute error for a very high lambda
    CD = bottomUpAnalysis(tree, hlambda);
    float Ch = CD.ssim;
    float Dh = CD.sumPerimeter;

    if (Ch > C0)
      return;

    do {  // Find the optimum lambda value 
      float lambda = (Dl - Dh) / (Ch - Cl);
      CD = bottomUpAnalysis(tree, lambda);

      if (CD.ssim < C0) {
        Ch = CD.ssim;
        Dh = CD.sumPerimeter;
      }
      else {
        Cl = CD.ssim;
        Dl = CD.sumPerimeter;
      }
    } while (!approx(CD.ssim, C0));

    filterNodesFromMorphoTree(tree);
    curSumPerimeter_ = CD.sumPerimeter;
    curSSIM_ = CD.ssim;
  }

  template<typename ValueType>
  typename MinCPerimeterWithSSIM<ValueType>::SSIMAccPerimeterPair
  MinCPerimeterWithSSIM<ValueType>::bottomUpAnalysis(MTree &tree, float lambda)
  {
    std::vector<float> vsumPerimeter(tree.numberOfNodes(), 0.0f);
    std::vector<float> vssim(tree.numberOfNodes(), 0.0f);

    resetKeepMapping();
    numFilteredNodes_ = 0;

    tree.tranverse([this, &vssim, &vsumPerimeter, lambda](NodePtr node){
      if (node->children().empty()) {
        vsumPerimeter[node->id()] = computeAccPerimeter(vsumPerimeter, node);
        vssim[node->id()] = computeSSIM(vssim, node);
      }
      else {
        float CC = computeFilteredChildrenSSIM(vssim, node);
        float CD = computeFiltetedChildrenAccPerimter(vsumPerimeter, node);

        float C = computeSSIM(vssim, node);
        float D = computeAccPerimeter(vsumPerimeter, node);
      }
      if ( (CD + (lambda * CC)) <= (D + (lambda * C)) ) {
        // remove children
        vsumPerimeter[node->id()] = CD;
        vssim[node->id()] = CC;

        for (NodePtr c : node->children()) {
          keep_[c->id()] = false;
          numFilteredNodes_++;
        }
      }
      else {
        // keep children
        vsumPerimeter[node->id()]
      }
    });

  }
}