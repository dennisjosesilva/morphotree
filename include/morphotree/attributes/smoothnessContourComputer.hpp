#pragma 

#include "morphotree/core/alias.hpp"
#include "morphotree/attributes/attributeComputer.hpp"
#include "morphotree/core/box.hpp"
#include "morphotree/core/point.hpp"

// Implementation based on 
// Connected Attribute Filtering Based on Contour Smoothness
// G. K. Ouzounis, E. R. Urbach, M. H. F. Wilkison

namespace morphotree 
{
  // ================= MAX-TREE ==============================================
  template<class ValueType>
  class MaxTreeSmoothnessContourComputer : public AttributeComputer<float, ValueType>
  {
  public:
    using AttrType = float;
    using TreeType = typename AttributeComputer<float, ValueType>::TreeType;
    using NodePtr = typename TreeType::NodePtr;

    MaxTreeSmoothnessContourComputer(Box domain, const std::vector<ValueType> &image);

    std::vector<float> initAttributes(const TreeType &tree);
    void computeInitialValue(std::vector<float> &attr, NodePtr node);
    void mergeToParent(std::vector<float> &attr, NodePtr node, NodePtr parent);

    void finaliseComputation(std::vector<float> &attr, NodePtr node);

  private:
    Box domain_;
    const std::vector<ValueType> &image_; 

    std::vector<uint32> area_;
    std::vector<uint32> perimeter_;
    std::vector<int32> sumX_;
    std::vector<int32> sumY_;
    std::vector<int32> sumXAndYSquared_;

    constexpr static float PiSquared = 9.86960440109f;

    const std::array<I32Point, 4> offsets_ = {
      I32Point{-1,0}, I32Point{0,-1}, I32Point{1, 0}, I32Point{0,1}};
  };

  // ======================[ MIN-TREE ]================================================
  template<class ValueType>
  class MinTreeSmoothnessContourComputer : public AttributeComputer<float, ValueType>
  {
  public:
    using AttrType = float;
    using TreeType = typename AttributeComputer<float, ValueType>::TreeType;
    using NodePtr = typename TreeType::NodePtr;

    MinTreeSmoothnessContourComputer(Box domain, const std::vector<ValueType> &image);

    std::vector<float> initAttributes(const TreeType &tree);
    void computeInitialValue(std::vector<float> &attr, NodePtr node);
    void mergeToParent(std::vector<float> &attr, NodePtr node, NodePtr parent);

    void finaliseComputation(std::vector<float> &attr, NodePtr node);

  private:
    Box domain_;
    const std::vector<ValueType> &image_;

    std::vector<uint32> area_;
    std::vector<uint32> perimeter_;
    std::vector<int32> sumX_;
    std::vector<int32> sumY_;
    std::vector<int32> sumXAndYSquared_;

    constexpr static float PiSquared = 9.86960440109f;

    const std::array<I32Point, 4> offsets_ = {
      I32Point{-1,0}, I32Point{0,-1}, I32Point{1, 0}, I32Point{0,1}};
  };

  // ======================[ TREE OF SHAPES ] ================================================
  template<class ValueType>
  class TreeOfShapesSmoothnessContourComputer : public AttributeComputer<float, ValueType>
  {  
  public:
    using AttrType = float;
    using TreeType = typename AttributeComputer<float, ValueType>::TreeType;
    using NodePtr = typename TreeType::NodePtr;

    TreeOfShapesSmoothnessContourComputer(Box domain, const std::vector<ValueType> &image);

    std::vector<float> initAttributes(const TreeType &tree);
    void computeInitialValue(std::vector<float> &attr, NodePtr node);
    void mergeToParent(std::vector<float> &attr, NodePtr node, NodePtr parent);

    void finaliseComputation(std::vector<float> &attr, NodePtr node);
  private:
    enum class NodeType { MaxTreeNodeType, MinTreeNodeType, Unknown };
    NodeType nodeType(const NodePtr node);

    void computeInitialPerimeterMaxTree(const NodePtr node);
    void computeInitialPerimeterMinTree(const NodePtr node);
    void computePerimeterRootNode(const NodePtr node);

  private:
    Box domain_;
    std::vector<ValueType> image_;

    std::vector<uint32> area_;
    std::vector<uint32> perimeter_;
    std::vector<int32> sumX_;
    std::vector<int32> sumY_;
    std::vector<int32> sumXAndYSquared_;

    constexpr static float PiSquared = 9.86960440109f;

    const std::array<I32Point, 4> offsets_ = {
      I32Point{-1,0}, I32Point{0,-1}, I32Point{1, 0}, I32Point{0,1}}; 
  };

  // ======================[ Implementation ] ================================================
  template<class ValueType>
  MaxTreeSmoothnessContourComputer<ValueType>::MaxTreeSmoothnessContourComputer(Box domain, 
    const std::vector<ValueType> &image)
    :domain_{domain}, image_{image}
  {}

  template<class ValueType>
  std::vector<float> MaxTreeSmoothnessContourComputer<ValueType>::initAttributes(
    const TreeType &tree)
  {
    area_.clear();
    perimeter_.clear();
    sumX_.clear();
    sumY_.clear();
    sumXAndYSquared_.clear();

    area_.resize(tree.numberOfNodes(), 0);
    perimeter_.resize(tree.numberOfNodes(), 0);
    sumX_.resize(tree.numberOfNodes(), 0);
    sumY_.resize(tree.numberOfNodes(), 0);
    sumXAndYSquared_.resize(tree.numberOfNodes(), 0);

    return std::vector<float>(tree.numberOfNodes(), 0.0f);
  }

  template<class ValueType>
  void MaxTreeSmoothnessContourComputer<ValueType>::computeInitialValue(std::vector<float> &attr,
    NodePtr node)
  {
    area_[node->id()] += node->cnps().size();
    for (uint32 pidx : node->cnps()) {
      int32 H = 0, L = 0;
      I32Point p = domain_.indexToPoint(pidx);
      for (const I32Point &offset : offsets_) {
        I32Point q = p + offset;
        if (!domain_.contains(q) || image_[domain_.pointToIndex(q)] < node->level()) {
          L++;
        }
        else if (image_[domain_.pointToIndex(q)] > node->level()) {
          H++;
        }
      }
      perimeter_[node->id()] += L - H;
      sumX_[node->id()] += p.x();
      sumY_[node->id()] += p.y();
      sumXAndYSquared_[node->id()] += (p.x() * p.x()) + (p.y() * p.y());
    }
  }

  template<class ValueType>
  void MaxTreeSmoothnessContourComputer<ValueType>::mergeToParent(std::vector<float> &attr, 
    NodePtr node, NodePtr parent)
  {     
    area_[parent->id()] += area_[node->id()];
    perimeter_[parent->id()] += perimeter_[node->id()];
    sumX_[parent->id()] += sumX_[node->id()];
    sumY_[parent->id()] += sumY_[node->id()];
    sumXAndYSquared_[parent->id()] += sumXAndYSquared_[node->id()];
  }

  template<class ValueType>
  void MaxTreeSmoothnessContourComputer<ValueType>::finaliseComputation(std::vector<float> &attr, 
    NodePtr node)
  {
    float I = float(sumXAndYSquared_[node->id()])  
      - (float(sumX_[node->id()] * sumX_[node->id()]) / float(area_[node->id()]))
      - (float(sumY_[node->id()] * sumY_[node->id()]) / float(area_[node->id()]))
      + (float(area_[node->id()]) / 6.0f);

    attr[node->id()] = 
      (float(area_[node->id()]) * float(perimeter_[node->id()] * perimeter_[node->id()])) /
       (8.0f * I * PiSquared); 
  }


  // =====================[ IMPLEMENTATION MIN-TREE ] ================================================
  template<class ValueType>
  MinTreeSmoothnessContourComputer<ValueType>::MinTreeSmoothnessContourComputer(Box domain, 
    const std::vector<ValueType> &image)
    :domain_{domain}, image_{image}
  {}

  template<class ValueType>
  std::vector<float> MinTreeSmoothnessContourComputer<ValueType>::initAttributes(
    const TreeType &tree)
  {
    area_.clear();
    perimeter_.clear();
    sumX_.clear();
    sumY_.clear();
    sumXAndYSquared_.clear();

    area_.resize(tree.numberOfNodes(), 0);
    perimeter_.resize(tree.numberOfNodes(), 0);
    sumX_.resize(tree.numberOfNodes(), 0);
    sumY_.resize(tree.numberOfNodes(), 0);
    sumXAndYSquared_.resize(tree.numberOfNodes(), 0);

    return std::vector<float>(tree.numberOfNodes(), 0.0f);
  }

  template<class ValueType>
  void MinTreeSmoothnessContourComputer<ValueType>::computeInitialValue(std::vector<float> &attr,
    NodePtr node)
  {
    area_[node->id()] += node->cnps().size();
    for (uint32 pidx : node->cnps()) {
      int32 H = 0, L = 0;
      I32Point p = domain_.indexToPoint(pidx);
      for (const I32Point &offset : offsets_) {
        I32Point q = p + offset;
        if (!domain_.contains(q) || image_[domain_.pointToIndex(q)] > node->level()) {
          H++;
        }
        else if (image_[domain_.pointToIndex(q)] < node->level()) {
          L++;
        }
      }
      perimeter_[node->id()] += H - L;
      sumX_[node->id()] += p.x();
      sumY_[node->id()] += p.y();
      sumXAndYSquared_[node->id()] += ((p.x() * p.x()) + (p.y() * p.y()));
    }
  }

  template<class ValueType>
  void MinTreeSmoothnessContourComputer<ValueType>::mergeToParent(std::vector<float> &attr,
    NodePtr node, NodePtr parent)
  {
    area_[parent->id()] += area_[node->id()];
    perimeter_[parent->id()] += perimeter_[node->id()];
    sumX_[parent->id()] += sumX_[node->id()];
    sumY_[parent->id()] += sumY_[node->id()];
    sumXAndYSquared_[parent->id()] += sumXAndYSquared_[node->id()];
  }

  template<class ValueType>
  void MinTreeSmoothnessContourComputer<ValueType>::finaliseComputation(std::vector<float> &attr,
    NodePtr node)
  {
    float I = float(sumXAndYSquared_[node->id()])  
      - (float(sumX_[node->id()] * sumX_[node->id()]) / float(area_[node->id()]))
      - (float(sumY_[node->id()] * sumY_[node->id()]) / float(area_[node->id()]))
      + (float(area_[node->id()]) / 6.0f);

    attr[node->id()] = 
      (float(area_[node->id()]) * float(perimeter_[node->id()] * perimeter_[node->id()])) /
       (8.0f * I * PiSquared); 
  }

  // ==================== [ IMPLEMENTATION - TREE OF SHAPES ] =============================================
  template<class ValueType>
  TreeOfShapesSmoothnessContourComputer<ValueType>::TreeOfShapesSmoothnessContourComputer(Box domain, 
    const std::vector<ValueType> &image)
    :domain_{domain}, image_{image}
  {}

  template<class ValueType>
  std::vector<float> TreeOfShapesSmoothnessContourComputer<ValueType>::initAttributes(
    const TreeType &tree)
  {
    area_.clear();
    perimeter_.clear();
    sumX_.clear();
    sumY_.clear();
    sumXAndYSquared_.clear();

    area_.resize(tree.numberOfNodes(), 0);
    perimeter_.resize(tree.numberOfNodes(), 0);
    sumX_.resize(tree.numberOfNodes(), 0);
    sumY_.resize(tree.numberOfNodes(), 0);
    sumXAndYSquared_.resize(tree.numberOfNodes(), 0);

    return std::vector<float>(tree.numberOfNodes(), 0.0f);
  }

  template<class ValueType>
  void TreeOfShapesSmoothnessContourComputer<ValueType>::computeInitialValue(std::vector<float> &attr,
    NodePtr node)
  {
    area_[node->id()] += node->cnps().size();
    if (nodeType(node) == NodeType::MaxTreeNodeType)
      computeInitialPerimeterMaxTree(node);
    else if (nodeType(node) == NodeType::MinTreeNodeType)
      computeInitialPerimeterMinTree(node);
    else {
      for (uint32 pidx: node->cnps()) {
        I32Point p = domain_.indexToPoint(pidx);
        sumX_[node->id()] += p.x();
        sumY_[node->id()] += p.y();
        sumXAndYSquared_[node->id()] += ((p.x() * p.x()) + (p.y() * p.y()));
      }
    }
  }

  template<class ValueType>
  typename TreeOfShapesSmoothnessContourComputer<ValueType>::NodeType 
  TreeOfShapesSmoothnessContourComputer<ValueType>::nodeType(const NodePtr node)
  {
    if (node->parent() == nullptr)
      return NodeType::Unknown;
    else if (node->parent()->level() < node->level())
      return NodeType::MaxTreeNodeType;
    else if (node->parent()->level() > node->level())
      return NodeType::MinTreeNodeType;

    return NodeType::Unknown;
  }

  template<class ValueType>
  void TreeOfShapesSmoothnessContourComputer<ValueType>::computeInitialPerimeterMaxTree(
    NodePtr node)
  {
    for (uint32 pidx : node->cnps()) {
      int32 H = 0, L = 0;
      I32Point p = domain_.indexToPoint(pidx);
      for (const I32Point &offset : offsets_) {
        I32Point q = p + offset;
        if (!domain_.contains(q) || image_[domain_.pointToIndex(q)] < node->level()) {
          L++;
        }
        else if (image_[domain_.pointToIndex(q)] > node->level()) {
          H++;
        } 
      }
      perimeter_[node->id()] += L - H;
      sumX_[node->id()] += p.x();
      sumY_[node->id()] += p.y();
      sumXAndYSquared_[node->id()] += (p.x() * p.x()) + (p.y() * p.y());
    }
  }

  template<class ValueType>
  void TreeOfShapesSmoothnessContourComputer<ValueType>::computeInitialPerimeterMinTree(
    NodePtr node)
  {
    for (uint32 pidx : node->cnps()) {
      int32 H = 0, L = 0;
      I32Point p = domain_.indexToPoint(pidx);
      for (const I32Point &offset : offsets_) {
        I32Point q = p + offset;
        if (!domain_.contains(q) || image_[domain_.pointToIndex(q)] > node->level()) {
          H++;
        }
        else if (image_[domain_.pointToIndex(q)] < node->level()) {
          L++;
        }
      }
      perimeter_[node->id()] += H - L;
      sumX_[node->id()] += p.x();
      sumY_[node->id()] += p.y();
      sumXAndYSquared_[node->id()] += ((p.x() * p.x()) + (p.y() * p.y()));
    }
  }

  template<class ValueType>
  void TreeOfShapesSmoothnessContourComputer<ValueType>::computePerimeterRootNode(
    const NodePtr node) 
  {
    perimeter_[node->id()] = (2*domain_.width()) + (2*domain_.height());
  }

  template<class ValueType>
  void TreeOfShapesSmoothnessContourComputer<ValueType>::mergeToParent(std::vector<float> &attr,
    NodePtr node, NodePtr parent)
  {
    area_[parent->id()] += area_[node->id()];
    
    if (nodeType(node) == nodeType(parent))
      perimeter_[parent->id()] += perimeter_[node->id()];
    else
      perimeter_[parent->id()] -= perimeter_[node->id()];
    
    sumX_[parent->id()] += sumX_[node->id()];
    sumY_[parent->id()] += sumY_[node->id()];
    sumXAndYSquared_[parent->id()] += sumXAndYSquared_[node->id()];
  }

  template<class ValueType>
  void TreeOfShapesSmoothnessContourComputer<ValueType>::finaliseComputation(std::vector<float> &attr,
    NodePtr node)
  {
    if (node->parent() == nullptr)    // if node is the root node.
      computePerimeterRootNode(node);

    float I = float(sumXAndYSquared_[node->id()])  
      - (float(sumX_[node->id()] * sumX_[node->id()]) / float(area_[node->id()]))
      - (float(sumY_[node->id()] * sumY_[node->id()]) / float(area_[node->id()]))
      + (float(area_[node->id()]) / 6.0f);

    // std::cout << "node id: " << node->id() << "; I: " << I 
    //   << "; area: " << area_[node->id()] << "; perimeter: " << perimeter_[node->id()] << std::endl;
    // std::cout << "sum x and y squared: " << sumXAndYSquared_[node->id()] 
    //   << "; sumX: " << sumX_[node->id()] << "; sumY: " << sumY_[node->id()] << std::endl; 

    // std::cout << "sumX^2: " << (float(sumX_[node->id()] * sumX_[node->id()]) / float(area_[node->id()])) << "; sumY^2: "
    //   << (float(sumY_[node->id()] * sumY_[node->id()]) / float(area_[node->id()])) << "; A/6: "
    //   << (float(area_[node->id()]) / 6.0f) << std::endl;
      

    // std::cout << (float(area_[node->id()]) * float(perimeter_[node->id()] * perimeter_[node->id()])) << " / "
    //   << (8.0f * I * PiSquared) << "\n\n"; 

    attr[node->id()] = 
      (float(area_[node->id()]) * float(perimeter_[node->id()] * perimeter_[node->id()])) /
       (8.0f * I * PiSquared);
  }
}