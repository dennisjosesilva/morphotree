#include "morphotree/attributes/bitquads/quads.hpp"

namespace morphotree
{
  Quads::Quads()
    :q1_{0}, q2_{0}, qd_{0}, q3_{0}, q4_{0}
  {}

  uint32 Quads::area() const 
  {
    return (q1_ + 2*q2_ + 2*qd_ + 3*q3_ + 4*q4_)/4;
  }

  uint32 Quads::perimeter() const
  {
    return q1_ + q2_ + q3_ + 2*qd_;
  }

  float Quads::continuousArea() const 
  {
    return (float(q1_)/8.0f) + (float(q2_)/2.0f) + (float(qd_) * 3.0f/4.0f) 
      + (float(q3_) * 7.0f/8.0f)  + float(q4_);
  }

  float Quads::continuousPerimeter() const 
  {
    return float(q2_) + (float(q1_) + float(q3_) + 2.0f*float(qd_)) * ONE_OVER_SQRT_TWO;
  }

  uint32 Quads::eulerNumber4C() const 
  {
    return (q1_ - q3_ + 2*qd_) / 4; 
  }

  uint32 Quads::eulerNumber8C() const
  {
    return (q1_ - q3_ - 2*qd_) / 4; 
  }
 
 uint32 Quads::numberOfHoles4C() const
 {
   return 1 - eulerNumber4C();
 }

 uint32 Quads::numberOfHoles8C() const 
 {
   return 1 - eulerNumber8C();
 }
}