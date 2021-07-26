#include "morphotree/core/box.hpp"

namespace morphotree
{
  const uint32 Box::SubBox::UndefinedIndex = std::numeric_limits<uint32>::max();

  // Forward Box Scan
  ForwardBoxScan::ForwardBoxScan(Box *box)
    :box_{box}, curr_{0}, end_{box_->numberOfPoints()}
  { }

  I32Point ForwardBoxScan::current() const 
  {
    return box_->indexToPoint(curr_);
  }

  I32Point ForwardBoxScan::next()
  {
    if (!hasFinished()) {
      I32Point p = box_->indexToPoint(curr_);
      curr_++;
      return p;
    }
    return I32Point{0, 0};
  } 

  // Backward Box Scan
  BackwardBoxScan::BackwardBoxScan(Box *box)
    :box_{box}, curr_{0}, end_{box_->numberOfPoints()}
  {}

  I32Point BackwardBoxScan::current() const
  {
    return box_->indexToPoint(curr_);
  }

  I32Point BackwardBoxScan::next()
  {
    if (!hasFinished()) {
      I32Point p = box_->indexToPoint(end_ - curr_ - 1);
      curr_++;
      return p;
    }
    return I32Point{0,0};
  }


  // Box
  Box::Box()
    :topleft_{I32Point{0,0}}, bottomright_{I32Point{0,0}} 
  {}

  Box::Box(I32Point topleft, I32Point bottomright)
  { 
    if (topleft.x() <= bottomright.x() && topleft.y() <= bottomright.y()) {
      topleft_ = topleft;
      bottomright_ = bottomright;
      size_.x(bottomright.x() - topleft.x() + 1);
      size_.y(bottomright.y() - topleft.y() + 1);
    }
  }

  Box Box::fromCorners(I32Point topleft, I32Point bottomright)
  {
    return Box{topleft, bottomright};
  }

  Box Box::fromSize(I32Point topleft, UI32Point size)
  {
    return Box{topleft, I32Point{topleft.x() + (int32)size.x() - 1, 
        topleft.y() + (int32)size.y() - 1}};
  }

  Box Box::fromSize(UI32Point size)
  {
    return Box::fromSize(I32Point{0,0}, size);
  }

  bool Box::contains(const I32Point &p) const 
  {
    return topleft_.x() <= p.x() && topleft_.y() <= p.y() 
      && p.x() <= bottomright_.x() && p.y() <= bottomright_.y();
  }

  uint32 Box::pointToIndex(const I32Point &p) const
  {
    if (!contains(p))
      return UndefinedIndex;

    I32Point q = p - topleft_;
    return q.y() *size_.width() + q.x();
  }

  I32Point Box::indexToPoint(uint32 idx) const 
  {
    I32Point p{ (int32)idx % (int32)size_.width() + topleft_.x(), 
      (int32)idx / (int32)size_.width() + topleft_.y() };
    if (!contains(p))
      return I32Point::UndefinedPoint;
    
    return p;
  }

  Box::SubBox Box::subBoxFromCorners(const I32Point &topleft, 
    const I32Point &bottomright) const
  {
    return Box::SubBox::fromCorners(*this, topleft, bottomright);
  }

  Box::SubBox Box::subBoxFromSize(const I32Point &topleft, 
    const UI32Point &size) const
  {
    return Box::SubBox::fromSize(*this, topleft, size);
  }

  std::ostream& operator<<(std::ostream &out, const Box &b)
  {
    return out << "(tl=" <<  b.topleft() << ", br=" << b.bottomright() << ")\n";
  }

  // ====================== SubBox =================================================
  Box::SubBox::SubBox(const Box &superDomain, I32Point topleft, 
    I32Point bottomright)
    :superDomain_{superDomain}
  {
    if (topleft.x() < bottomright.x() && topleft.y() < bottomright.y()) {
      topleft_ = topleft;
      bottomright_ = bottomright;
      size_.width(bottomright.x() - topleft.x() + 1);
      size_.height(bottomright.y() - topleft.y() + 1);
    }
  }

  Box::SubBox Box::SubBox::fromCorners(const Box &superDomain, I32Point topleft,
    I32Point bottomright)
  {
    return Box::SubBox(superDomain, topleft, bottomright);
  }

  Box::SubBox Box::SubBox::fromSize(const Box& superDomain, I32Point topleft, 
    UI32Point size)
  {
    return Box::SubBox{superDomain, topleft, 
      I32Point{topleft.x() + (int32)size.x() - 1, topleft.y() + (int32)size.y() - 1}};
  }

  Box::SubBox Box::SubBox::fromSize(const Box& superDomain, UI32Point size)
  {
    return SubBox::fromSize(superDomain, I32Point{0,0}, size);
  }

  bool Box::SubBox::contains(const I32Point &p) const 
  {
    return topleft_.x() <= p.x() && topleft_.y() <= p.y() 
      && p.x() <= bottomright_.x() && p.y() <= bottomright_.y();
  }

  uint32 Box::SubBox::pointToIndex(const I32Point &p) const
  {
    if (!contains(p))
      return UndefinedIndex;
    
    return superDomain_.pointToIndex(p);
  }

  I32Point Box::SubBox::indexToPoint(uint32 idx) const 
  {
    I32Point p{ (int32)idx % (int32)superDomain_.width() + superDomain_.left(),
      (int32)idx / (int32)superDomain_.width() + superDomain_.top() };
    
    if (!contains(p))
      return I32Point::UndefinedPoint;

    return p;
  }

  uint32 Box::SubBox::pointToLocalIndex(const I32Point &p) const
  {
      if (!contains(p))
        return UndefinedIndex;

      I32Point q = p - topleft_;
      return q.y() * size_.width() + q.x();
  }

  I32Point Box::SubBox::localIndexToPoint(uint32 idx) const 
  {
    I32Point p{ (int32)idx % (int32)size_.width() + topleft_.x(),
      (int32)idx / (int32)size_.width() + topleft_.y() };
    
    if (!contains(p))
      return I32Point::UndefinedPoint;

    return p;
  }

  uint32 Box::SubBox::localIndexToIndex(uint32 lidx) const
  {
    if (lidx >= numberOfPoints())
      return UndefinedIndex;
      
    return superDomain_.pointToIndex(localIndexToPoint(lidx));
  }

  uint32 Box::SubBox::indexToLocalIndex(uint idx) const
  {
    if (!contains(indexToPoint(idx)))
      return UndefinedIndex;

    I32Point p = superDomain_.indexToPoint(idx) - topleft_;
    return (width() * p.y()) + p.x();
  }
}