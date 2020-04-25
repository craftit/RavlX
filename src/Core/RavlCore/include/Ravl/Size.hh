// This file is part of RAVL, Recognition And Vision Library
// Copyright (C) 2001-12, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVL_SIZE_HEADER
#define RAVL_SIZE_HEADER 1
//! lib=RavlCore

#include "Ravl/config.h"
#include <stdlib.h>
#include "Ravl/Types.hh"

namespace RavlN {
  class BinOStreamC;
  class BinIStreamC;

  class SizeC {
  public:
    SizeC()
    {}
    //: Default constructor.
    // Value is not initialised.

    SizeC(const size_t &value)
     : m_v(value)
    {}

    inline size_t V() const
    { return m_v; }
    //: Returns the current value of the index.

    inline size_t & V()
    { return m_v; }
    //: Returns the current value of the index.

    operator size_t () const
    { return m_v; }
    //: Convert to a size_t

    SizeC &operator--()
    { m_v--; return *this; }
    //: Decrement

    SizeC &operator++()
    { m_v++; return *this; }
    //: Increment

    SizeC operator--(int)
    { size_t val = m_v; m_v--; return val; }
    //: Decrement

    SizeC operator++(int)
    { size_t val = m_v; m_v++; return val; }
    //: Increment

#if !RAVL_COMPILER_VISUALCPP
    SizeC operator-() const
    { return -m_v; }
    //: Negation
#endif

    template<typename DataT>
    inline bool operator<(const DataT &v2) const
    { return V() < v2; }

    template<typename DataT>
    inline bool operator>(const DataT &v2) const
    { return V() > v2; }

    template<typename DataT>
    inline bool operator<=(const DataT &v2) const
    { return V() <= v2; }

    template<typename DataT>
    inline bool operator>=(const DataT &v2) const
    { return V() >= v2; }

    template<typename DataT>
    inline bool operator==(const DataT &v2) const
    { return V() == v2; }

    template<typename DataT>
    inline bool operator!=(const DataT &v2) const
    { return V() != v2; }

    template<typename DataT>
    inline SizeC operator-(const DataT &v2) const
    { return V() - v2; }

    template<typename DataT>
    inline SizeC operator+(const DataT &v2) const
    { return V() + v2; }

    template<typename DataT>
    inline SizeC operator*(const DataT &v2) const
    { return V() * v2; }

    template<typename DataT>
    inline SizeC operator/(const DataT &v2) const
    { return V() / v2; }

    template<typename DataT>
    inline SizeC operator%(const DataT &v2) const
    { return V() % v2; }

    inline double operator-(const double &v2) const
    { return V() - v2; }

    inline double operator+(const double &v2) const
    { return V() + v2; }

    inline double operator*(const double &v2) const
    { return V() * v2; }

    inline double operator/(const double &v2) const
    { return V() / v2; }

    inline float operator-(const float &v2) const
    { return V() - v2; }

    inline float operator+(const float &v2) const
    { return V() + v2; }

    inline float operator*(const float &v2) const
    { return V() * v2; }

    inline float operator/(const float &v2) const
    { return V() / v2; }

    inline size_t operator*(const size_t &v2) const
    { return V() * v2; }

    inline size_t operator/(const size_t &v2) const
    { return V() / v2; }

    inline size_t operator+(const size_t &v2) const
    { return V() + v2; }

    inline size_t operator-(const size_t &v2) const
    { return V() - v2; }

    inline size_t operator%(const size_t &v2) const
    { return V() % v2; }

  protected:
    size_t m_v;
  };

  template<typename DataT>
  SizeC &operator+=(SizeC &s,const DataT &val)
  { s.V() += val; return s; }

  template<typename DataT>
  SizeC &operator-=(SizeC &s,const DataT &val)
  { s.V() -= val; return s; }

  template<typename DataT>
  SizeC &operator/=(SizeC &s,const DataT &val)
  { s.V() /= val; return s; }

  template<typename DataT>
  SizeC &operator*=(SizeC &s,const DataT &val)
  { s.V() *= val; return s; }

  //---------------------

  // Pointer arithmetic.
  template<typename DataT>
  inline DataT *operator-(DataT *v1,const SizeC &v2)
  { return v1 - v2.V(); }

  template<typename DataT>
  inline DataT *operator+(DataT *v1,const SizeC &v2)
  { return v1 + v2.V(); }

  // -----------------------

  inline SizeC &operator+=(SizeC &s,const SizeC &val)
  { s.V() += val.V(); return s; }

  inline SizeC &operator-=(SizeC &s,const SizeC &val)
  { s.V() -= val.V(); return s; }

  inline SizeC &operator/=(SizeC &s,const SizeC &val)
  { s.V() /= val.V(); return s; }

  inline SizeC &operator*=(SizeC &s,const SizeC &val)
  { s.V() *= val.V(); return s; }

  inline SizeC &operator^=(SizeC &s,const SizeC &val)
  { s.V() ^= val.V(); return s; }

  inline bool operator<(const SizeC &v1,const SizeC &v2)
  { return v1.V() < v2.V(); }

  inline bool operator>(const SizeC &v1,const SizeC &v2)
  { return v1.V() > v2.V(); }

  inline bool operator<=(const SizeC &v1,const SizeC &v2)
  { return v1.V() <= v2.V(); }

  inline bool operator>=(const SizeC &v1,const SizeC &v2)
  { return v1.V() >= v2.V(); }

  inline bool operator==(const SizeC &v1,const SizeC &v2)
  { return v1.V() == v2.V(); }

  inline bool operator!=(const SizeC &v1,const SizeC &v2)
  { return v1.V() != v2.V(); }

  inline SizeC operator-(const SizeC &v1,const SizeC &v2)
  { return v1.V() - v2.V(); }

  inline SizeC operator+(const SizeC &v1,const SizeC &v2)
  { return v1.V() + v2.V(); }

  inline SizeC operator*(const SizeC &v1,const SizeC &v2)
  { return v1.V() * v2.V(); }

  inline SizeC operator/(const SizeC &v1,const SizeC &v2)
  { return v1.V() / v2.V(); }

  // ---------------------------------

  inline bool operator>(const SizeC &v1,const int &v2) {
    if(v2 < 0) return true;
    return v1.V() > (size_t) v2;
  }

  inline bool operator>=(const SizeC &v1,const int &v2) {
    if(v2 < 0) return true;
    return v1.V() >= (size_t) v2;
  }

  inline bool operator<(const SizeC &v1,const int &v2) {
    if(v2 < 0) return false;
    return v1.V() < (size_t) v2;
  }

  inline bool operator<=(const SizeC &v1,const int &v2) {
    if(v2 < 0) return false;
    return v1.V() <= (size_t) v2;
  }

  inline bool operator==(const SizeC &v1,const int &v2) {
    if(v2 < 0) return false;
    return v1.V() == (size_t) v2;
  }

  inline bool operator!=(const SizeC &v1,const int &v2) {
    if(v2 < 0) return true;
    return v1.V() != (size_t) v2;
  }

  // ---------------------------------

  inline bool operator>(const SizeC &v1,const Int64T &v2) {
    if(v2 < 0) return true;
    return v1.V() > (size_t) v2;
  }

  inline bool operator>=(const SizeC &v1,const Int64T &v2) {
    if(v2 < 0) return true;
    return v1.V() >= (size_t) v2;
  }

  inline bool operator<(const SizeC &v1,const Int64T &v2) {
    if(v2 < 0) return false;
    return v1.V() < (size_t) v2;
  }

  inline bool operator<=(const SizeC &v1,const Int64T &v2) {
    if(v2 < 0) return false;
    return v1.V() <= (size_t) v2;
  }

  inline bool operator==(const SizeC &v1,const Int64T &v2) {
    if(v2 < 0) return false;
    return v1.V() == (size_t) v2;
  }

  inline bool operator!=(const SizeC &v1,const Int64T &v2) {
    if(v2 < 0) return true;
    return v1.V() != (size_t) v2;
  }

  // --------------------------

  inline SizeC operator+(const unsigned  &v1,const SizeC &v2)
  { return SizeC(v1 + v2.V()); }

  inline SizeC operator-(const unsigned  &v1,const SizeC &v2)
  { return SizeC(v1 - v2.V()); }

  inline SizeC  operator*(const unsigned  &v1,const SizeC &v2)
  { return SizeC(v1 * v2.V()); }

  inline SizeC  operator/(const unsigned  &v1,const SizeC &v2)
  { return SizeC(v1 / v2.V()); }

  inline bool operator<(const unsigned  &v1,const SizeC &v2)
  { return v1 < v2.V(); }

  inline bool operator>(const unsigned  &v1,const SizeC &v2)
  { return v1 > v2.V(); }

  inline bool operator<=(const unsigned  &v1,const SizeC &v2)
  { return v1 <= v2.V(); }

  inline bool operator>=(const unsigned  &v1,const SizeC &v2)
  { return v1 >= v2.V(); }

  inline bool operator==(const unsigned  &v1,const SizeC &v2)
  { return v1 == v2.V(); }

  inline bool operator!=(const unsigned  &v1,const SizeC &v2)
  { return v1 != v2.V(); }

  inline unsigned  &operator+=(unsigned  &s,const SizeC &val)
  { s += val.V(); return s; }

  inline unsigned  &operator-=(unsigned  &s,const SizeC &val)
  { s -= val.V(); return s; }

  inline unsigned  &operator/=(unsigned  &s,const SizeC &val)
  { s /= val.V(); return s; }

  inline unsigned  &operator*=(unsigned  &s,const SizeC &val)
  { s *= val.V(); return s; }


  // double,SizeC ops.

  inline double operator*(const double&v1,const SizeC &v2)
  { return v1 * v2.V(); }

  inline double operator/(const double&v1,const SizeC &v2)
  { return v1 / v2.V(); }

  inline double operator+(const double&v1,const SizeC &v2)
  { return v1 + v2.V(); }

  inline double operator-(const double&v1,const SizeC &v2)
  { return v1 - v2.V(); }

  inline bool operator>(const double &v1,const SizeC &v2)
  { return v1 > v2.V(); }

  inline bool operator<(const double &v1,const SizeC &v2)
  { return v1 < v2.V(); }

  inline bool operator>=(const double &v1,const SizeC &v2)
  { return v1 >= v2.V(); }

  inline bool operator<=(const double &v1,const SizeC &v2)
  { return v1 <= v2.V(); }

  inline double  &operator+=(double &s,const SizeC &val)
  { s += val.V(); return s; }

  inline double &operator-=(double &s,const SizeC &val)
  { s -= val.V(); return s; }

  inline double &operator/=(double &s,const SizeC &val)
  { s /= val.V(); return s; }

  inline double &operator*=(double &s,const SizeC &val)
  { s *= val.V(); return s; }

  // float,SizeC ops.

  inline float operator*(const float &v1,const SizeC &v2)
  { return v1 * v2.V(); }

  inline float operator/(const float &v1,const SizeC &v2)
  { return v1 / v2.V(); }

  inline float operator+(const float &v1,const SizeC &v2)
  { return v1 + v2.V(); }

  inline float operator-(const float &v1,const SizeC &v2)
  { return v1 - v2.V(); }

  inline bool operator>(const float &v1,const SizeC &v2)
  { return v1 > v2.V(); }

  inline bool operator<(const float &v1,const SizeC &v2)
  { return v1 < v2.V(); }

  inline bool operator>=(const float &v1,const SizeC &v2)
  { return v1 >= v2.V(); }

  inline bool operator<=(const float &v1,const SizeC &v2)
  { return v1 <= v2.V(); }

  inline float &operator+=(float &s,const SizeC &val)
  { s += val.V(); return s; }

  inline float &operator-=(float &s,const SizeC &val)
  { s -= val.V(); return s; }

  inline float &operator/=(float &s,const SizeC &val)
  { s /= val.V(); return s; }

  inline float &operator*=(float &s,const SizeC &val)
  { s *= val.V(); return s; }

  // int,SizeC ops.

  inline bool operator>(const int &v1,const SizeC &v2) {
    if(v1 < 0) return false;
    return (size_t) v1 > v2.V();
  }

  inline bool operator<(const int &v1,const SizeC &v2) {
    if(v1 < 0) return false;
    return (size_t) v1 < v2.V();
  }

  inline bool operator>=(const int &v1,const SizeC &v2) {
    if(v1 < 0) return false;
    return (size_t) v1 >= v2.V();
  }

  inline bool operator<=(const int &v1,const SizeC &v2) {
    if(v1 < 0) return false;
    return (size_t) v1 <= v2.V();
  }

  inline SizeC operator*(const int &v1,const SizeC &v2)
  { return SizeC(v1 * v2.V()); }

  inline SizeC operator/(const int &v1,const SizeC &v2)
  { return SizeC(v1 / v2.V()); }

  inline SizeC operator+(const int &v1,const SizeC &v2)
  { return SizeC(v1 + v2.V()); }

  inline SizeC operator-(const int &v1,const SizeC &v2)
  { return SizeC(v1 - v2.V()); }

  inline int &operator+=(int &s,const SizeC &val)
  { s += val.V(); return s; }

  inline int &operator-=(int &s,const SizeC &val)
  { s -= val.V(); return s; }

  inline int &operator/=(int &s,const SizeC &val)
  { s /= val.V(); return s; }

  inline int &operator*=(int &s,const SizeC &val)
  { s *= val.V(); return s; }

  // Int64T,Size ops

  inline bool operator>(const Int64T &v1,const SizeC &v2) {
    if(v1 < 0) return false;
    return (size_t) v1 > v2.V();
  }

  inline bool operator<(const Int64T &v1,const SizeC &v2) {
    if(v1 < 0) return false;
    return (size_t) v1 < v2.V();
  }

  inline bool operator>=(const Int64T &v1,const SizeC &v2) {
    if(v1 < 0) return false;
    return (size_t) v1 >= v2.V();
  }

  inline bool operator<=(const Int64T &v1,const SizeC &v2) {
    if(v1 < 0) return false;
    return (size_t) v1 <= v2.V();
  }

  inline Int64T operator*(const Int64T &v1,const SizeC &v2)
  { return v1 * v2.V(); }

  inline Int64T operator/(const Int64T &v1,const SizeC &v2)
  { return v1 / v2.V(); }

  inline Int64T operator+(const Int64T &v1,const SizeC &v2)
  { return v1 + v2.V(); }

  inline Int64T operator-(const Int64T &v1,const SizeC &v2)
  { return v1 - v2.V(); }

  inline Int64T &operator+=(Int64T &s,const SizeC &val)
  { s += val.V(); return s; }

  inline Int64T &operator-=(Int64T &s,const SizeC &val)
  { s -= val.V(); return s; }

  inline Int64T &operator/=(Int64T &s,const SizeC &val)
  { s /= val.V(); return s; }

  inline Int64T &operator*=(Int64T &s,const SizeC &val)
  { s *= val.V(); return s; }

  // UInt64T,Size ops

  inline bool operator>(const UInt64T &v1,const SizeC &v2)
  { return (size_t) v1 > v2.V(); }

  inline bool operator<(const UInt64T &v1,const SizeC &v2)
  { return (size_t) v1 < v2.V(); }

  inline bool operator>=(const UInt64T &v1,const SizeC &v2)
  { return (size_t) v1 >= v2.V(); }

  inline bool operator<=(const UInt64T &v1,const SizeC &v2)
  { return (size_t) v1 <= v2.V(); }

  inline UInt64T operator*(const UInt64T &v1,const SizeC &v2)
  { return v1 * v2.V(); }

  inline UInt64T operator/(const UInt64T &v1,const SizeC &v2)
  { return v1 / v2.V(); }

  inline UInt64T operator+(const UInt64T &v1,const SizeC &v2)
  { return v1 + v2.V(); }

  inline UInt64T operator-(const UInt64T &v1,const SizeC &v2)
  { return v1 - v2.V(); }

  inline UInt64T &operator+=(UInt64T &s,const SizeC &val)
  { s += val.V(); return s; }

  inline UInt64T &operator-=(UInt64T &s,const SizeC &val)
  { s -= val.V(); return s; }

  inline UInt64T &operator/=(UInt64T &s,const SizeC &val)
  { s /= val.V(); return s; }

  inline UInt64T &operator*=(UInt64T &s,const SizeC &val)
  { s *= val.V(); return s; }

  // Streaming operators.

  BinOStreamC &operator<<(BinOStreamC &strm,const SizeC &size);
  //: Write to a binary stream

  BinIStreamC &operator>>(BinIStreamC &strm,SizeC &size);
  //: Read from a binary stream

  std::ostream &operator<<(std::ostream &strm,const SizeC &size);
  //: Write to text stream

  std::istream &operator>>(std::istream &strm,SizeC &size);
  //: Write to text stream
}

#endif
