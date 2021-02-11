#pragma once

#include "morphotree/core/alias.hpp"

namespace morphotree 
{
  class Quads
  {
    public:
      Quads();

      uint32  q1() const { return q1_; }
      uint32& q1() { return q1_; }
      void q1(uint32 val) { q1_ = val; }
      void incQ1() { q1_++; }
      void decQ1() { q1_--; }

      uint32  q2() const { return q2_; }
      uint32& q2() { return q2_; }
      void q2(uint32 val) { q2_ = val; }
      void incQ2() { q2_++; }
      void decQ2() { q2_--; }

      uint32  qd() const { return qd_; }
      uint32& qd() { return qd_; }
      void qd(uint32 val) { qd_ = val; }
      void incQD() { qd_++; }
      void decQD() { qd_--; }

      uint32  q3() const { return q3_; }
      uint32& q3() { return q3_; }
      void q3(uint32 val) { q3_ = val; }
      void incQ3() { q3_++; }
      void decQ3() { q3_--; }

      uint32  q4() const { return q4_; }
      uint32& q4() { return q4_; }
      void q4(uint32 val) { q4_ = val; }
      void incQ4() { q4_++; }
      void decQ4() { q4_--; }

      uint32 area() const;
      uint32 perimeter() const;
      
      float continuousArea() const;
      float continuousPerimeter() const;

      uint32 eulerNumber4C() const;
      uint32 eulerNumber8C() const;
      uint32 numberOfHoles4C() const;
      uint32 numberOfHoles8C() const;
      
    private:
      const static float ONE_OVER_SQRT_TWO = 0.70710678f;

      uint32 q1_;
      uint32 q2_;
      uint32 qd_;
      uint32 q3_;
      uint32 q4_;
  };
}