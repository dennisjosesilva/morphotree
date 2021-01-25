#pragma once 

#include "morphotree/tree/ct_builder.hpp"
#include "morphotree/tree/mtree.hpp"
#include "morphotree/tree/treeOfShapes/kgrid.hpp"
#include "morphotree/tree/treeOfShapes/order_image.hpp"

namespace morphotree 
{
  
  // build max-tree of the order image
  template<class WeightType>
  MorphologicalTree<uint32> buildOrderImageMaxtree(
    const Box &domain, const std::vector<WeightType> &f,
    const KGrid<WeightType> &kgrid, I32Point pInfty = I32Point{0,0});

  template<class WeightType>
  MorphologicalTree<uint32> buildOrderImageMaxtree(
    const Box &domain, const std::vector<WeightType> &f,
    I32Point pInfty= I32Point{0,0});

  // build enlarged tree of shapes (using flattern image)
  template<class WeightType>
  MorphologicalTree<WeightType> buildEnlargedTreeOfShapes(
    const Box &domain, const std::vector<WeightType> &f,
    const KGrid<WeightType> &kgrid, I32Point pInfty = I32Point{0,0});

  template<class WeightType>
  MorphologicalTree<WeightType> buildEnlargedTreeOfShapes(
    const Box &domain, const std::vector<WeightType> &f,
    I32Point pInfty = I32Point{0,0});

  // build tree of shapes by emerging all nodes.
  template<class WeightType>
  MorphologicalTree<WeightType> buildTreeOfShapes(
    const Box &domain, const std::vector<WeightType> &f,
    const KGrid<WeightType> &kgrid, I32Point pInfty = I32Point{0,0});

  template<class WeightType>
  MorphologicalTree<WeightType> buildTreeOfShapes(
    const Box &domain, const std::vector<WeightType> &f,
    I32Point pInfty = I32Point{0,0});

  // emerge tree of shapes or max-tree of order image.
  template<class WeightType>
  MorphologicalTree<WeightType> emergeTreeOfShapes(
    const KGrid<WeightType> &grid,
    const MorphologicalTree<WeightType> &treeOfShapes);

  // ========================= [ Implementation ] ===============================================
  // Build max-tree of the order image
  template<class WeightType>
  MorphologicalTree<uint32> buildOrderImageMaxtree(const Box &domain, 
    const std::vector<WeightType> &f, const KGrid<WeightType> &kgrid, I32Point pInfty)
  {
    OrderImageResult<WeightType> orderRes = computeOrderImage(domain, f, kgrid, pInfty);
    CTBuilder<uint32> builder;
    return MorphologicalTree<uint32>{orderRes.orderImg, builder.build(orderRes.orderImg, kgrid.adj(), orderRes.R)};
  }

  template<class WeightType>
  MorphologicalTree<WeightType> buildOrderImageMaxtree(const Box &domain, 
    const std::vector<WeightType> &f, I32Point pInfty)
  {
    KGrid<WeightType> kgrid{domain, f};
    return buildOrderImageMaxtree(domain, f, kgrid, pInfty);    
  }
  
  // Build enlarged tree of shapes
  template<class WeightType>
  MorphologicalTree<WeightType> buildEnlargedTreeOfShapes(const Box &domain, 
    const std::vector<WeightType> &f, const KGrid<WeightType> &kgrid,
    I32Point pInfty)
  {
    OrderImageResult<WeightType> orderRes = computeOrderImage(domain, f, kgrid, pInfty);
    CTBuilder<uint32> builder;
    return MorphologicalTree<WeightType>{orderRes.flattern, builder.build(orderRes.orderImg, kgrid.adj(), orderRes.R)};
  }

  template<class WeightType>
  MorphologicalTree<WeightType> buildEnlargedTreeOfShapes(const Box &domain, 
    const std::vector<WeightType> &f, I32Point pInfty)
  {
    KGrid<WeightType> kgrid{domain, f};
    return buildEnlargedTreeOfShapes(domain, f, kgrid, pInfty);
  } 

  // build tree of shapes by emerging all nodes.  
  template<class WeightType>
  MorphologicalTree<WeightType> buildTreeOfShapes(const Box &domain, 
    const std::vector<WeightType> &f, const KGrid<WeightType> &kgrid, 
    I32Point pInfty)
  {
    OrderImageResult<WeightType> orderRes = computeOrderImage(domain, f, kgrid, pInfty);
    CTBuilder<uint32> builder;
    return emergeTreeOfShapes(kgrid, MorphologicalTree<WeightType>{orderRes.flattern, 
      builder.build(orderRes.orderImg, kgrid.adj(), orderRes.R)}); 
  }

  template<class WeightType>
  MorphologicalTree<WeightType> buildTreeOfShapes(const Box &domain, 
    const std::vector<WeightType> &f, I32Point pInfty)
  {
    KGrid<WeightType> kgrid{domain, f};
    return buildTreeOfShapes(domain, f, kgrid, pInfty);
  }

  template<class WeightType>
  MorphologicalTree<WeightType> emergeTreeOfShapes(
    const KGrid<WeightType> &grid,
    const MorphologicalTree<WeightType> &treeOfShapes)
  {
    using MTree = MorphologicalTree<WeightType>;
    using NodePtr = typename MorphologicalTree<WeightType>::NodePtr; 
    using NodeType = typename MorphologicalTree<WeightType>::NodeType; 

    std::vector<NodePtr> nodes = std::vector<NodePtr>(treeOfShapes.numberOfNodes());
    std::vector<uint32> cmap  = std::vector<uint32>(grid.emergeDomain().numberOfPoints());

    treeOfShapes.traverseByLevel([&nodes, &grid, &cmap](const NodePtr oldNode) {
      NodePtr newNode =   std::make_shared<NodeType>(oldNode->id());
      newNode->level(oldNode->level());
      newNode->includeCNPS(grid.emergeSet(oldNode->cnps()));
      nodes[oldNode->id()] = newNode;

      for (uint32 cnp : newNode->cnps()) {
        cmap[cnp] = newNode->id();
      }      
    });


    treeOfShapes.traverseByLevel([&nodes](const NodePtr oldNode){
      NodePtr newNode = nodes[oldNode->id()];
      if (oldNode->parent() != nullptr) {
        newNode->parent(nodes[oldNode->parent()->id()]);
      }

      for (NodePtr child : oldNode->children()) {
        newNode->appendChild(nodes[child->id()]);
      }
    }); 

    return MTree{std::move(cmap), std::move(nodes)};
  }
}