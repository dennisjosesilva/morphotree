#include "morphotree/core/box.hpp"

namespace morphotree
{
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
  Box::Box(I32Point topleft, I32Point bottomright)
  { 
    if (topleft.x() < bottomright.x() && topleft.y() < bottomright.y()) {
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
      return I32Point{0, 0};
    
    return p;
  }

  std::ostream& operator<<(std::ostream &out, const Box &b)
  {
    return out << "(tl=" <<  b.topleft() << ", br=" << b.bottomright() << ")\n";
  }
}