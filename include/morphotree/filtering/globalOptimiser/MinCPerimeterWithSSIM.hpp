#pragma once 

#include "morphotree/tree/mtree.hpp"
#include "morphotree/attributes/bitquads/quadCountComputer.hpp"
#include "morphotree/measure/ssim.hpp"

#include <cmath>
#include <stack>


// DEBUG HEADERS
#include <iostream>
#include <chrono>

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

    std::vector<ValueType> filter(float ssimThreshold);

    float computeAccPerimeter();
    float computeFilteredChildrenAccPerimeter(const std::vector<bool> &keepWithPrunning);
          
    float computeSSIM();
    float computeFilteredChildrenSSIM(const std::vector<bool> &keepWithPrunning);

  protected:
    void computeCPerimeter(const MTree &tree);

    std::vector<bool> prune(NodePtr node);
    void pruneAtKeepAttribute(NodePtr node);

    void filterNodesFromMorphoTree(MTree &tree);
    void performFilter(MTree &tree, float ssimError);
    SSIMAccPerimeterPair bottomUpAnalysis(MTree &tree, float lambda);

    float computeMaxError() const;

    void resetKeepMapping();
    bool approx(float v0, float v1);

  private:
    float approxThreshold_;
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
    SSIM ssim_; 
  };    

  // ====================== [ IMPLEMENTATION ] ===================================
  template<typename ValueType>
  MinCPerimeterWithSSIM<ValueType>::MinCPerimeterWithSSIM(const Box &domain, 
    const std::vector<ValueType> &f, std::string dtFilename, const MTree &tree,
    float approxThrehold)
    : approxThreshold_{approxThrehold},
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
  std::vector<ValueType> 
    MinCPerimeterWithSSIM<ValueType>::filter(float ssimThreshold)
  {
    MTree tree = tree_.copy();
    performFilter(tree, ssimThreshold);
    return tree.reconstructImage();
  }

  template<typename ValueType>
  void MinCPerimeterWithSSIM<ValueType>::computeCPerimeter(const MTree &tree)
  {
    using AttrComputer = AttributeComputer<Quads, ValueType>;
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

      std::cout << "lambda: " << lambda << " ssim: " << CD.ssim << "\n";

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
    using std::chrono::duration_cast;
    using std::chrono::high_resolution_clock;
    using std::chrono::milliseconds;

    resetKeepMapping();
    numFilteredNodes_ = 0;

    tree.tranverse([this, lambda](NodePtr node){       
      if (!node->children().empty()) {
        std::vector<bool> keepWithPrunning = prune(node);

        auto start = high_resolution_clock::now();        
        float CC = computeFilteredChildrenSSIM(keepWithPrunning);
        auto end = high_resolution_clock::now();
        auto duration = duration_cast<milliseconds>(end - start);
        std::cout << "computeFilteredChildrenSSIM: " << duration.count() << " ms\n";
        
        start = high_resolution_clock::now();                
        float CD = computeFilteredChildrenAccPerimeter(keepWithPrunning);
        end = high_resolution_clock::now();
        duration = duration_cast<milliseconds>(end - start);
        std::cout << "computeFilteredChildrenAccPerimeter: " << duration.count() << " ms\n";

        start = high_resolution_clock::now();
        float C = computeSSIM();
        end = high_resolution_clock::now();
        duration = duration_cast<milliseconds>(end - start);
        std::cout << "computeSSIM: " << duration.count() << " ms\n";

        start = high_resolution_clock::now();
        float D = computeAccPerimeter();      
        end = high_resolution_clock::now();
        duration = duration_cast<milliseconds>(end - start);
        std::cout << "computeAccPerimeter: " << duration.count() << " ms\n";

        if ( (CD + (lambda * CC)) <= (D + (lambda * C)) ) {
          // remove children        
          pruneAtKeepAttribute(node);
          numFilteredNodes_++;          
        }
      // else (keep children), nothing to-do
      }
    });

    return SSIMAccPerimeterPair{
      computeAccPerimeter(),
      computeSSIM()};
  }

  template<typename ValueType>
  float MinCPerimeterWithSSIM<ValueType>::computeAccPerimeter()
  {
    float accPerimeter = 0.0f;
    tree_.tranverse([&accPerimeter, this](NodePtr node) {
      if (keep_[node->id()])
        accPerimeter += cperimeter_[node->id()];
    });  

    return accPerimeter;
  }

  template<typename ValueType>
  float MinCPerimeterWithSSIM<ValueType>::computeFilteredChildrenAccPerimeter(
    const std::vector<bool> &keepWithPrunning)
  {
    float accPerimeter = 0.0f;
    tree_.tranverse([&accPerimeter, &keepWithPrunning, this](NodePtr node) {
      if (keepWithPrunning[node->id()])
        accPerimeter += cperimeter_[node->id()];
    });

    return accPerimeter;
  }

  template<typename ValueType>
  float MinCPerimeterWithSSIM<ValueType>::computeSSIM()
  {
    std::vector<ValueType> frec = 
      tree_.reconstructImage([this](NodePtr node) { return keep_[node->id()]; });
    return ssim_.compute(domain_, f_, frec);
  }

  template<typename ValueType>
  float MinCPerimeterWithSSIM<ValueType>::computeFilteredChildrenSSIM(
    const std::vector<bool> &keepWithPrunnig)
  {
    std::vector<ValueType> frec = 
      tree_.reconstructImage([this, &keepWithPrunnig](NodePtr node){ 
        return keepWithPrunnig[node->id()];
      });
    
    return ssim_.compute(domain_, f_, frec);
  }

  template<typename ValueType>
  std::vector<bool> MinCPerimeterWithSSIM<ValueType>::prune(NodePtr node)
  {
    std::vector<bool> keepAfterPrunning(keep_.size(), true);

    std::stack<NodePtr> s;

    // push every child node of node to a stack.
    for (NodePtr c : node->children()) {
      s.push(c);
    }

    // traverse the subtree rooted at node 
    // by level (breadth-first traverse)
    while (!s.empty()) {
      // pop a node from the stack
      NodePtr n = s.top();
      s.pop();

      // remove it from the reconstruction
      keepAfterPrunning[n->id()] = false;

      // add its children to be removed next.
      for (NodePtr c : n->children()) {
        s.push(c);
      }
    }

    return keepAfterPrunning;
  }

  template<typename ValueType> 
  void MinCPerimeterWithSSIM<ValueType>::pruneAtKeepAttribute(NodePtr node)
  {
    // auxiliary stack to traverse the subtree rooted at node
    std::stack<NodePtr> s;

    // push all children to the stack.
    for (NodePtr c : node->children()) {
      if (keep_[c->id()]) 
        s.push(c);
    }

    // tranverse the tree.
    while (!s.empty()) {
      // pop the top of the stack
      NodePtr n = s.top();
      s.pop();

      // remove the node from the reconstruction
      keep_[n->id()] = false;
      
      // include its children to be processed next (if necessary)
      for (NodePtr c : n->children()) {
        if (keep_[c->id()])
          s.push(c);
      }
    }
  }

  template<typename ValueType>
  void MinCPerimeterWithSSIM<ValueType>::resetKeepMapping()
  {
    std::fill(keep_.begin(), keep_.end(), true);
  }

  template<typename ValueType>
  bool MinCPerimeterWithSSIM<ValueType>::approx(float v0, float v1)
  {
    return fabs(v0 - v1) <= approxThreshold_;
  }

  template<typename ValueType>
  float MinCPerimeterWithSSIM<ValueType>::computeMaxError() const 
  {
    return 0.0f;
  }
}