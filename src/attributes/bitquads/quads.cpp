#include "morphotree/attributes/bitquads/quads.hpp"

namespace morphotree
{
  const  uint32 Quads::P1 = 0;
  const  uint32 Quads::P2 = 1;
  const  uint32 Quads::P3 = 2;
  const  uint32 Quads::P4 = 3;
  const  uint32 Quads::PD = 4;
  const  uint32 Quads::P1T = 5;
  const  uint32 Quads::P2T = 6;
  const  uint32 Quads::P3T = 7;
  const  uint32 Quads::PDT = 8;

  Quads::Quads()
    :quads_{0,0,0,0,0}
  {}

  uint32 Quads::area() const 
  {
    return (q1() + 2*q2() + 2*qd() + 3*q3() + 4*q4())/4;
  }

  uint32 Quads::perimeter() const
  {
    return q1() + q2() + q3() + 2*qd();
  }

  float Quads::continuousArea() const 
  {
    return (float(q1())/8.0f) + (float(q2())/2.0f) + (float(qd()) * 3.0f/4.0f) 
      + (float(q3()) * 7.0f/8.0f)  + float(q4());
  }

  float Quads::continuousPerimeter() const 
  {
    return float(q2()) + (float(q1()) + float(q3()) + 2.0f*float(qd())) * ONE_OVER_SQRT_TWO;
  }

  uint32 Quads::eulerNumber4C() const 
  {
    return (q1() - q3() + 2*qd()) / 4; 
  }

  uint32 Quads::eulerNumber8C() const
  {
    return (q1() - q3() - 2*qd()) / 4; 
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