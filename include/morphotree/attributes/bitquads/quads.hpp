#pragma once

#include "morphotree/core/alias.hpp"
#include <array>

namespace morphotree 
{
  class Quads
  {
    public:
      Quads();

      int32  q1() const { return quads_[P1]; }
      int32& q1() { return quads_[P1]; }
      void q1(int32 val) { quads_[P1] = val; }
      void incQ1() { quads_[P1]++; }
      void decQ1() { quads_[P1]--; }

      int32  q2() const { return quads_[P2]; }
      int32& q2() { return quads_[P2]; }
      void q2(int32 val) { quads_[P2]= val; }
      void incQ2() { quads_[P2]++; }
      void decQ2() { quads_[P2]--; }

      int32  qd() const { return quads_[PD]; }
      int32& qd() { return quads_[PD]; }
      void qd(int32 val) { quads_[PD] = val; }
      void incQD() { quads_[PD]++; }
      void decQD() { quads_[PD]--; }

      int32  q3() const { return quads_[P3]; }
      int32& q3() { return quads_[P3]; }
      void q3(int32 val) { quads_[P3] = val; }
      void incQ3() { quads_[P3]++; }
      void decQ3() { quads_[P3]--; }

      int32  q4() const { return quads_[P4]; }
      int32& q4() { return quads_[P4]; }
      void q4(int32 val) { quads_[P4] = val; }
      void incQ4() { quads_[P4]++; }
      void decQ4() { quads_[P4]--; }

      inline int32  operator[](uint8 idx) const { return quads_[idx]; }
      inline int32& operator[](uint8 idx) { return quads_[idx];}

      uint32 area() const;
      uint32 perimeter() const;
      
      float continuousArea() const;
      float continuousPerimeter() const;

      uint32 eulerNumber4C() const;
      uint32 eulerNumber8C() const;
      uint32 numberOfHoles4C() const;
      uint32 numberOfHoles8C() const;
      
      const static uint32 P1;
      const static uint32 P2;
      const static uint32 P3;
      const static uint32 P4;
      const static uint32 PD;

      const static uint32 P1T;
      const static uint32 P2T;
      const static uint32 P3T;
      const static uint32 PDT;

    private:
      constexpr static float ONE_OVER_SQRT_TWO = 0.70710678f;

      std::array<int32, 5> quads_;

      // uint32 q1_;
      // uint32 q2_;
      // uint32 qd_;
      // uint32 q3_;
      // uint32 q4_;
  };
}