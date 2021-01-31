#pragma once 

#include "morphotree/core/alias.hpp"
#include "morphotree/core/box.hpp"
#include "morphotree/adjacency/adjacencyuc.hpp"

#include <ostream>
#include <memory>
#include <vector>
#include <array>

namespace morphotree
{
  template<class ValueType>
  class Interval
  {
  public:
    using Type = ValueType; 

    Interval(ValueType min, ValueType max);

    static Interval<ValueType> fromMinMax(ValueType v0, ValueType v1);
    static Interval<ValueType> fromMinMax(std::initializer_list<ValueType> values);

    inline ValueType  min() const { return min_; }
    inline ValueType& min() { return min_; }
    inline void min(ValueType val) { min_ = val; }

    inline ValueType  max() const { return max_; }
    inline ValueType& max() { return max_; }
    inline void max(ValueType val) { max_ = val; }

  private:
    ValueType min_;
    ValueType max_;
  };

  template<class ValueType>
  std::ostream& operator<<(std::ostream &out, const Interval<ValueType> &intvl);

  template<class ValueType>
  class KGrid
  {
  public:
    using IntervalType = Interval<ValueType>;
    using Type =  ValueType;

    KGrid();
    KGrid(const Box &imgDomain, const std::vector<ValueType> &data);
  
    IntervalType& interval(uint32 idx) { return data_[idx]; }
    IntervalType  interval(uint32 idx) const { return data_[idx]; }
    void interval(uint32 idx, IntervalType intvl) { data_[idx] = intvl; }

    IntervalType& interval(int32 x, int32 y) { return data_[domain_.pointToIndex(x, y)]; }
    IntervalType  interval(int32 x, int32 y) const { return data_[domain_.pointToIndex(x, y)]; }
    void interval(int32 x, int32 y, IntervalType intvl) { data_[domain_.pointToIndex(x, y)] = intvl; }

    IntervalType& interval(const I32Point &p) { return data_[domain_.pointToIndex(p)]; }
    IntervalType  interval(const I32Point &p) const { return data_[domain_.pointToIndex(p)]; }
    void interval(const I32Point &p, IntervalType intvl) { data_[domain_.pointToIndex(p)]; }

    I32Point emergePoint(const I32Point &p) const;
    uint32 emergePoint(uint32 p) const;
    std::vector<I32Point> emergeSet(const std::vector<I32Point> &set) const;
    std::vector<uint32> emergeSet(const std::vector<uint32> &set) const;
    Box emergeDomain() const;

    I32Point immersePoint(const I32Point &p) const;
    uint32 immersePoint(uint32 p) const;
    std::vector<I32Point> immerseSet(const std::vector<I32Point> &set) const;
    std::vector<uint32> immerseSet(const std::vector<uint32> &set) const;
    Box immerseDomain() const;

    bool isZeroFace(const I32Point &p) const;
    bool isOneFace(const I32Point &p) const;
    bool isTwoFace(const I32Point &p) const;

    inline std::shared_ptr<AdjacencyUC> adj() { return adjU_; }
    inline const std::shared_ptr<AdjacencyUC> adj() const { return adjU_; }
  private: 
    void computeGrid(const Box &imgDomain, const std::vector<ValueType> &data);
    IntervalType computeZeroFace(const I32Point &p, Type v0, Type v1, Type V2, Type v3) const;
    
  private:
    Box domain_;
    Box imgDomain_;
    std::vector<Interval<ValueType>> data_;
    std::shared_ptr<AdjacencyUC> adjU_; 
  };

  template<class ValueType>
  std::ostream& operator<<(std::ostream& out, const KGrid<ValueType> &grid);

  // ============== IMPLEMENTATION ===============================================================================
  // ============== INTERVAL =====================================================================================
  template<class ValueType>
  Interval<ValueType>::Interval(ValueType min, ValueType max)
    :min_{min}, max_{max}
  {}

  template<class ValueType>
  Interval<ValueType> Interval<ValueType>::fromMinMax(ValueType v0, ValueType v1)
  {
    if (v0 <= v1) 
      return Interval<ValueType>{v0, v1};
    return Interval<ValueType>{v1, v0};
  }

  template<class ValueType>
  Interval<ValueType> fromMinMax(std::initializer_list<ValueType> values)
  {
    ValueType min = values[0];
    ValueType max = values[0];

    for (const ValueType &v : values) {
      if (v <= min) min = v;
      if (v >= max) max = v;
    }

    return Interval<ValueType>{min, max};
  }

  template<class ValueType>
  std::ostream& operator<<(std::ostream &out, const Interval<ValueType> &intvl)
  {
    out << "[" << (int)intvl.min() << ", " << (int)intvl.max() << "]";
    return out;
  }

  // ============= KGrid =========================================================================================
  template<class ValueType>
  KGrid<ValueType>::KGrid()
    :domain_{I32Point{0,0}, I32Point{0,0}}
  {}

  template<class ValueType>
  KGrid<ValueType>::KGrid(const Box &imgDomain, const std::vector<ValueType> &data)
    :imgDomain_{imgDomain}
  {
    computeGrid(imgDomain, data);
  }

  template<class ValueType>
  void KGrid<ValueType>::computeGrid(const Box &imgDomain, const std::vector<ValueType> &data)
  {
    domain_ = Box::fromSize(imgDomain.topleft(), 
      UI32Point{2*imgDomain.width()-1, 2*imgDomain.height()-1});
    data_.resize(domain_.numberOfPoints(), IntervalType{0,0});

    adjU_ = std::make_shared<AdjacencyUC>(domain_);

    // Compute interval from 2-faces.
    I32Point p = domain_.topleft();
    for (; p.y() <= domain_.bottom(); p.y() += 2)
    {
      for (p.x(domain_.left()) ; p.x() <= domain_.right(); p.x() += 2) {
        I32Point q = emergePoint(p);
        uint32 idx = imgDomain.pointToIndex(q); 
        data_[domain_.pointToIndex(p)] = IntervalType{data[idx], data[idx]};
      }
    }

    // Compute interval horizontally for 1-faces
    p = domain_.topleft();
    for (; p.y() <= domain_.bottom(); p.y() += 2) {
      for (p.x(domain_.left()+1); p.x() <= domain_.right(); p.x() += 2) {
        I32Point q1 = emergePoint(p + I32Point{-1, 0});
        I32Point q2 = emergePoint(p + I32Point{ 1, 0});
        uint32 idx1 = imgDomain.pointToIndex(q1);
        uint32 idx2 = imgDomain.pointToIndex(q2);
        data_[domain_.pointToIndex(p)] = IntervalType::fromMinMax(data[idx1], data[idx2]);
      }
    }

    // Compute interval vertically for 1-faces.
    p = domain_.topleft();
    p.y()++;
    for (; p.y() <= domain_.bottom(); p.y() += 2) {
      for (p.x(domain_.left()); p.x() <= domain_.right(); p.x() += 2) {
        I32Point q1 = emergePoint(p + I32Point{ 0,-1});
        I32Point q2 = emergePoint(p + I32Point{ 0, 1});
        uint32 idx1 = imgDomain.pointToIndex(q1);
        uint32 idx2 = imgDomain.pointToIndex(q2);
        data_[domain_.pointToIndex(p)] = IntervalType::fromMinMax(data[idx1], data[idx2]);
      }
    }

    // Compute interval for 0-faces
    p = domain_.topleft();
    p.y()++;
    for (; p.y() <= domain_.bottom(); p.y() += 2) {
      for (p.x(domain_.left()+1); p.x() <= domain_.right(); p.x() += 2) {
        
        if (p.x() == 45 && p.y() == 513)
        {
          std::cout << "pixel" << std::endl;
        }

        I32Point q1 = emergePoint(p + I32Point{-1,-1});
        I32Point q2 = emergePoint(p + I32Point{ 1,-1});
        I32Point q3 = emergePoint(p + I32Point{-1, 1});
        I32Point q4 = emergePoint(p + I32Point{ 1, 1});
        uint32 idx1 = imgDomain.pointToIndex(q1);
        uint32 idx2 = imgDomain.pointToIndex(q2);
        uint32 idx3 = imgDomain.pointToIndex(q3);
        uint32 idx4 = imgDomain.pointToIndex(q4);
        data_[domain_.pointToIndex(p)] = computeZeroFace(p, data[idx1],
          data[idx2], data[idx3], data[idx4]);
      }
    }
  }

  template<class ValueType>
  typename KGrid<ValueType>::IntervalType 
    KGrid<ValueType>::computeZeroFace(const I32Point &p, Type v0, Type v1, Type v2, Type v3) const
  {
    // block:
    // v0  |  v1
    //  -  p  -
    // v2  |  v3



    IntervalType v0v3 = IntervalType::fromMinMax(v0, v3);
    IntervalType v1v2 = IntervalType::fromMinMax(v1, v2);

    if (v0v3.min() > v1v2.max()) {
      // One critical configuration.
      adjU_->dconn(domain_.pointToIndex(p + I32Point{-1, 0}), DiagonalConnection::SE);
      adjU_->dconn(domain_.pointToIndex(p + I32Point{ 0, 1}), DiagonalConnection::NW);

      adjU_->dconn(domain_.pointToIndex(p + I32Point{-1,-1}), DiagonalConnection::SE);
      adjU_->dconn(domain_.pointToIndex(p), DiagonalConnection::SE | DiagonalConnection::NW);
      adjU_->dconn(domain_.pointToIndex(p + I32Point{+1,+1}), DiagonalConnection::NW);

      adjU_->dconn(domain_.pointToIndex(p + I32Point{ 0,-1}), DiagonalConnection::SE);
      adjU_->dconn(domain_.pointToIndex(p + I32Point{+1, 0}), DiagonalConnection::NW);
      return v0v3;
    }
    else if (v1v2.min() > v0v3.max()) {
      // TODO: Other critical Configuration.
      adjU_->dconn(domain_.pointToIndex(p + I32Point{-1, 0}), DiagonalConnection::NE);
      adjU_->dconn(domain_.pointToIndex(p + I32Point{ 0,-1}), DiagonalConnection::SW);

      adjU_->dconn(domain_.pointToIndex(p + I32Point{+1,-1}), DiagonalConnection::SW);
      adjU_->dconn(domain_.pointToIndex(p), DiagonalConnection::SW | DiagonalConnection::NE);
      adjU_->dconn(domain_.pointToIndex(p + I32Point{-1,+1}), DiagonalConnection::NE);

      adjU_->dconn(domain_.pointToIndex(p + I32Point{ 0,+1}), DiagonalConnection::NE);
      adjU_->dconn(domain_.pointToIndex(p + I32Point{+1, 0}), DiagonalConnection::SW);

      return v1v2;
    } 
    else {
      // TODO: Non-critical configuration.
      ValueType min = v0v3.min() < v1v2.min() ? v0v3.min() : v1v2.min();
      ValueType max = v0v3.max() > v1v2.max() ? v0v3.max() : v1v2.max();
      return IntervalType{min, max};
    }
  }


  template<class ValueType>
  I32Point KGrid<ValueType>::emergePoint(const I32Point &p) const
  {
    return I32Point{imgDomain_.left() + ((p.x() - imgDomain_.left()) / 2),
      imgDomain_.top() + ((p.y() - imgDomain_.top()) / 2)};
  }

  template<class ValueType>
  uint32 KGrid<ValueType>::emergePoint(uint32 p) const 
  {
    return imgDomain_.pointToIndex(emergePoint(domain_.indexToPoint(p)));
  }

  template<class ValueType>
  std::vector<I32Point> KGrid<ValueType>::emergeSet(const std::vector<I32Point> &set) const
  {
    std::vector<I32Point> eSet;
    eSet.reserve(set.size());
    for (const I32Point &p : set) {
      if (isZeroFace(p)) {
        eSet.push_back(emergePoint(p));
      }
    }

    return eSet;  
  }

  template<class ValueType>
  std::vector<uint32> KGrid<ValueType>::emergeSet(const std::vector<uint32> &set) const
  {
    std::vector<uint32> eSet;
    eSet.reserve(set.size());
    for (uint32 idx : set) {
      I32Point p = domain_.indexToPoint(idx);
      if (isZeroFace(p)) {
        eSet.push_back(imgDomain_.pointToIndex(emergePoint(p)));
      }
    }

    return eSet;
  }

  template<class ValueType>
  Box KGrid<ValueType>::emergeDomain() const
  {
    return imgDomain_;
  }

  template<class ValueType>
  I32Point KGrid<ValueType>::immersePoint(const I32Point &p) const
  {
    return I32Point{imgDomain_.left() + 2*(p.x() - imgDomain_.left()),
      imgDomain_.top() + 2 * (p.y() - imgDomain_.top())};
  }

  template<class ValueType>
  std::vector<I32Point> KGrid<ValueType>::immerseSet(const std::vector<I32Point> &set) const
  {
    std::vector<I32Point> iSet;
    iSet.reserve(set.size());
    for (const I32Point &p : set) {
      iSet.push_back(immersePoint(p));
    }
    
    return iSet;
  }

  template<class ValueType>
  std::vector<uint32> KGrid<ValueType>::immerseSet(const std::vector<uint32> &set) const
  {
    std::vector<uint32> iSet;
    iSet.reserve(set.size());
    for (uint32 p : set) {
      iSet.push_back(immersePoint(p));
    }

    return iSet;
  }

  template<class ValueType>
  uint32 KGrid<ValueType>::immersePoint(uint32 p) const
  {
    I32Point tp = immersePoint(imgDomain_.indexToPoint(p));
    return domain_.pointToIndex(tp);
  }
    
  
  template<class ValueType>
  Box KGrid<ValueType>::immerseDomain() const
  {
    return domain_;
  }

  template<class ValueType>
  bool KGrid<ValueType>::isZeroFace(const I32Point &p) const
  {
    return (((p.x() - domain_.left()) % 2) == 0) && (((p.y() - domain_.top()) % 2) == 0);
  }

  template<class ValueType>
  bool KGrid<ValueType>::isOneFace(const I32Point &p) const
  {
    return (((p.x() - domain_.left()) % 2) == 1) && (((p.y() - domain_.top()) % 2) == 0) || 
      (((p.x() - domain_.left()) % 2) == 0) && (((p.y() - domain_.top()) % 2) == 1);
  }

  template<class ValueType>
  bool KGrid<ValueType>::isTwoFace(const I32Point &p) const
  {
    return (((p.x() - domain_.left()) % 2) == 1) && (((p.y() - domain_.top()) % 2) == 1);
  }

  template<class ValueType>
  std::ostream& operator<<(std::ostream& out, const KGrid<ValueType> &grid)
  {
    Box domain = grid.immerseDomain();
    std::shared_ptr<AdjacencyUC> adj = grid.adj();

    for (uint32 y = domain.top(); y < domain.bottom(); ++y) {
      for (uint32 x = domain.left(); x < domain.right(); ++x) {
        out << grid.interval(x, y) << " - ";
      }
      out << grid.interval(domain.left(), y) << "\n";
      
      for (uint32 x = domain.left(); x < domain.right(); ++x) {
        uint32 idx = domain.pointToIndex(x, y);
        uint32 idxNext = domain.pointToIndex(x+1,y);
        out << "   |  ";

        if (adj->dconn(idx) & DiagonalConnection::SE)
          out << " \\ ";
        else if (adj->dconn(idxNext) & DiagonalConnection::SW)
          out << " / ";
        else
          out << "   ";
      }
      out << "   |  ";
      out << "\n";
    }
    
    for (uint32 x = domain.left(); x < domain.right(); ++x) {
      out << grid.interval(x, domain.bottom()) << " - ";
    }
    out << grid.interval(domain.right(), domain.bottom()) << "\n";
    return out;
  }
}