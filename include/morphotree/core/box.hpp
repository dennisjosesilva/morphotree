#pragma once

#include "morphotree/core/point.hpp"
#include  "morphotree/core/alias.hpp"

#include <limits>
#include <ostream>

namespace morphotree
{
  class Box;

  class ForwardBoxScan
  {
  public:
    ForwardBoxScan(Box *box);

    I32Point current() const;
    I32Point next();
    inline bool hasFinished() { return curr_ == end_; }

  private:
    Box *box_;
    uint32 curr_;
    uint32 end_;
  };

  class BackwardBoxScan
  {
  public:
    BackwardBoxScan(Box *box);

    I32Point current() const;
    I32Point next();
    bool hasFinished() { return curr_ == end_; };

  private:
    Box *box_;
    uint32 curr_;
    uint32 end_;
  };


  class Box
  {
  public: 
    class SubBox
    {

    private:
      SubBox(const Box& superDomain, I32Point topleft, I32Point bottomright);
      static SubBox fromCorners(const Box& superDomain, I32Point topleft, 
        I32Point bottomright);
      static SubBox fromSize(const Box& superDomain, I32Point topleft, 
        UI32Point size);
      static SubBox fromSize(const Box& superDomain, UI32Point size);

    public: 
      static const uint32 UndefinedIndex;

      inline I32Point topleft() const { return topleft_; }
      inline I32Point bottomright() const { return bottomright_; }

      inline int32 top() const { return topleft_.y(); }
      inline int32 left() const { return topleft_.x(); }

      inline int32 bottom() const { return bottomright_.y(); }
      inline int32 right() const { return bottomright_.x(); }

      bool contains(const I32Point &p) const;

      uint32 pointToIndex(const I32Point &p) const;
      inline uint32 pointToIndex(int32 x, int32 y) const { return pointToIndex(I32Point{x, y}); }
      I32Point indexToPoint(uint32 idx) const;

      uint32 pointToLocalIndex(const I32Point &p) const;
      inline uint32 pointToLocalIndex(int32 x, int32 y) const { return pointToLocalIndex(I32Point{x, y}); }
      I32Point localIndexToPoint(uint32 idx) const;
      uint32 localIndexToIndex(uint lidx) const;
      uint32 indexToLocalIndex(uint idx) const;

      I32Point at(uint32 idx) const { return localIndexToPoint(idx); }
      I32Point operator[](uint32 idx) const { return localIndexToPoint(idx); }

      UI32Point size() const { return size_; }
      uint32 width() const { return size_.width(); }
      uint32 height() const { return size_.height(); }
      uint32 numberOfPoints() const { return size_.width() * size_.height(); }

      friend class Box;

    private:
      const Box& superDomain_; 
      I32Point topleft_;
      I32Point bottomright_;
      UI32Point size_;
    };


  public:
    Box();
    Box(I32Point topleft, I32Point bottomright);

    static Box fromCorners(I32Point topleft, I32Point bottomright);
    static Box fromSize(I32Point topleft, UI32Point size);
    static Box fromSize(UI32Point size);

    static const uint32 UndefinedIndex = std::numeric_limits<uint32>::max(); 

    int32 top() const { return topleft_.y(); }
    int32 left() const { return topleft_.x(); }
    
    int32 bottom() const { return bottomright_.y(); }
    int32 right() const { return bottomright_.x(); }

    bool contains(const I32Point &p) const;

    I32Point topleft() const { return topleft_; }
    I32Point bottomright() const { return bottomright_; } 

    uint32 pointToIndex(const I32Point& p) const;
    inline uint32 pointToIndex(int32 x, int32 y) const { return pointToIndex(I32Point{x, y}); }
    I32Point indexToPoint(uint32 idx) const;

    I32Point at(uint32 idx) const { return indexToPoint(idx); }
    I32Point operator[](uint32 idx) const { return indexToPoint(idx); }

    SubBox subBoxFromCorners(const I32Point &topleft, const I32Point &bottomright) const;
    SubBox subBoxFromSize(const I32Point &topleft, const UI32Point &size) const;

    UI32Point size() const { return size_; } 
    uint32 width() const { return size_.width(); }
    uint32 height() const { return size_.height(); }
    uint32 numberOfPoints() const { return size_.width() * size_.height(); }

    ForwardBoxScan forwardBoxScan() { return ForwardBoxScan{this}; }
    BackwardBoxScan backwardBoxScan() { return BackwardBoxScan{this}; } 
  
  private:
    I32Point topleft_;
    I32Point bottomright_;
    UI32Point size_;
  };

  std::ostream& operator<<(std::ostream &out, const Box &b);
}