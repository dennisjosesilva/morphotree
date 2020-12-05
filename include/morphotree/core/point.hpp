#pragma once

#include "morphotree/core/alias.hpp"
#include <ostream>
#include <array>

namespace morphotree
{
  template<class T>
  class Point
  {
  public:
    Point()
      :data_{0,0}
    {}

    Point(T c1, T c2)
      :data_{c1, c2}
    {}

    // cols
    inline T  x() const { return data_[0]; }
    inline T& x() { return data_[0]; }
    inline void x(T val) { data_[0] = val; }

    inline T  col() const { return data_[0]; }
    inline T& col() { return data_[0]; }
    inline void col(T val) { data_[0] = val; } 

    inline T  width() const { return data_[0]; }
    inline T& width() { return data_[0]; }
    inline void width(T val) { data_[0] = val; }

    // rows
    inline T  y() const { return data_[1]; }
    inline T& y()  { return data_[1]; }
    inline void y(T val) { data_[1] = val; }

    inline T  row() const { return data_[1]; }
    inline T& row() { return data_[1]; }
    inline void row(T val) { data_[1] = val; }

    inline T  height() const { return data_[1]; }
    inline T& height() { return data_[1]; }
    inline void height(T val) { data_[1] = val; } 

    // index operators
    T  operator[](uint32 idx) const { return data_[idx]; }
    T& operator[](uint32 idx) { return data_[idx]; }

    inline Point<T> add(const Point<T> &p) const { return Point<T>{x() + p.x(), y() + p.y()}; }
    inline Point<T> sub(const Point<T> &p) const { return Point<T>{x() - p.x(), y() - p.y()}; }
    inline Point<T> mult(float val) const { return Point<T>{x() * (T)val, y() * (T)val}; }
    inline Point<T> mult(const Point<T> &p) const { return Point<T>{x() * p.x(), y() * p.y()}; }
    inline Point<T> div(float val) const { return Point<T>{x() / (T)val, y() / (T)val}; }
    inline Point<T> div(const Point<T> &p) const { return Point<T>{x() / p.x(), y() / p.y()}; }

    inline void iadd(const Point<T> &p) { x() += p.x(); y() += p.y(); }
    inline void isub(const Point<T> &p) { x() -= p.x(); y() -= p.y(); }
    inline void imult(float val) { x() *= (T)val; y() *= (T)val; }
    inline void imult(const Point<T> &p) { x() *= p.x(); y() *= p.y(); }
    inline void idiv(float val) { x() *= (T)val; y() *= (T)val; }
    inline void idiv(const Point<T> &p) { x() /= p.x(); y() /= p.y(); }

    inline bool equals(const Point<T> &p) const { return x() == p.x() && y() == p.y(); }

    // arithmetic operator
    void operator+=(const Point<T> &p) { iadd(p); }
    void operator-=(const Point<T> &p) { isub(p); }
    void operator*=(float val) { imult(val); }
    void operator*=(const Point<T> &p) { imult(p); }
    void operator/=(float val) { idiv(val); }
    void operator/=(const Point<T> &p) { idiv(p); }

  private:
    std::array<T, 2> data_;
  };

  template<class T>
  Point<T> operator+(const Point<T> &p, const Point<T> &q) { return p.add(q); }

  template<class T>
  Point<T> operator-(const Point<T> &p, const Point<T> &q) { return p.sub(q); }

  template<class T>
  Point<T> operator*(const Point<T> &p, float val) { return p.mult(val); }

  template<class T>
  Point<T> operator*(const Point<T> &p, const Point<T> &q) { return p.mult(q); }

  template<class T>
  Point<T> operator/(const Point<T> &p, float val) { return p.div(val); }

  template<class T>
  Point<T> operator/(const Point<T> &p, const Point<T> &q) { return p.div(q); }

  template<class T>
  bool operator==(const Point<T> &p, const Point<T> &q) { return p.equals(q); }

  template<class T> 
  bool operator!=(const Point<T> &p, const Point<T> &q) { return !p.equals(q); }   

  template<class T>
  std::ostream& operator<<(std::ostream &out, const Point<T> &p) { 
    return out << "(" << p.x() << ", " << p.y() << ")";
  }

  using UI32Point = Point<uint32>;
  using UI8Point = Point<uint8>;
  using I32Point = Point<int32>;
  using I8Point = Point<int8>;
}