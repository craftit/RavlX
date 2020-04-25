// This file is part of RAVL, Recognition And Vision Library
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVL_INDEX_HH
#define RAVL_INDEX_HH
////////////////////////////////////////////////////////////////////////
//! file="Ravl/Core/Base/Index.hh"
//! lib=RavlCore
//! userlevel=Basic
//! author="Radek Marik, Bill Christmas"
//! date="2/1/1996"
//! docentry="Ravl.API.Core.Indexing"
//! example=exIndex.cc
//! rcsid="$Id$"

#include "Ravl/Types.hh"
#include "Ravl/Assert.hh"
#include "Ravl/Math.hh"

namespace RavlN {

  //: Index class for indexing 1D arrays: an integer class with modified rounding and other operations

  // <p>The class <code>IndexC</code> is designed as a replacement for the built-in C
  // <code>int</code> type, in an attempt to correct some of the perceived
  // deficiencies of <code>int</code>:</p>

  // <dl>
  // <dt> Unsigned integers:
  // <dd> When combining signed and unsigned integers in a binary operation, C
  // always converts the signed integer to unsigned, which can generate
  // surprising results such as: <code>(unsigned int) 4 / (-2) = 0</code>.
  // <dt> Rounding:
  // <dd>With the <code>int</code> type, rounding is either towards zero (e.g. on conversion
  // from floating point representations), or implementation-dependent (e.g.
  // integer division operations).  Where negative numbers are
  // involved, this behaviour is often not what is required, particularly for
  // indexing purposes.
  // </dl>
  // <p>The behaviour of <code>IndexC</code> therefore differs from that of
  // <code>int</code>, to provide more consistent behaviour for indexing
  // purposes, and to improve portability of code.  The major differences are:
  // <dl>
  // <dt> Unsigned integers:
  // <dd> Where relevant, unsigned integers are converted to signed before
  // combining with <code>IndexC</code>.
  // <dt>Rounding:
  // <dd>Rounding is always to the nearest more negative integer, except as
  // stated below.  This affects:
  // <ul>
  // <li> Truncation of floating point values on conversion to
  // <code>IndexC</code>.
  // <li> Integer division operations involving <code>IndexC</code>:
  // <ul>
  // <li> If the divisor is positive, the quotient is always rounded towards
  // the nearest more negative integer, regardless of the sign of the dividend.
  // <li> If the divisor is negative, the signs of both divisor and quotient
  // are reversed.  This means the result is rounded <i>up</i>; this ensures
  // that the modulo operator always generates a +ve result.
  // </ul>
  // </ul>
  // Thus <code>(IndexC) -4 / 3</code> yields -2, and <code>(IndexC) -4 % 3
  // </code> yields 2.
  // With the <code>int</code> type, the results would probably be -1 and -1 respectively.
  // <dt> Modulo operator:
  // <dd> The modulo operator is always consistent with the divide operator, as
  // per the C and C++ standards.  I.e.:
  // <pre>   a == (a/b)*b + a%b</pre>
  // It accordingly always returns a non-negative value.
  // <dt> Word length:
  // <dd> The word length of an IndexC corresponds to that of a <em>pointer</em> variable (i.e. <a href=RavlN.html#SizeT><code>SizeT</code></a>) on that architecture.  So it is likely to be 64 bits on a 64-bit architecture, while an int will probably be 32 bits.
  // </dl>

  // Remember: IndexC is a class, not a built-in. So:<ul>

  // <li> The type conversion rules are not necessarily the same as for <code>int</code>.  For
  // a class, the type of the <b>first</b> operand of binary operator determines
  // the class member function used.

  // <li> Because there is no implicit conversion from IndexC to
  // <code>int</code>, you can't use IndexC everywhere you would use an
  // <code>int</code>.  In particular you can't use an IndexC object directly as
  // an <code>int</code> function argument.  You need to use the <a
  // href="#IndexC_V_void__const">V()</a> member function to achieve this.  (We <i>could</i> have provided the implicit conversion, but it would then be very difficult sometimes to know which was actually being used.) </ul>

  class IndexC {
  public:
    //:-
    // <h2>Constructors, copies, assignment, and destructor</h2>
    inline IndexC(const SizeC &i)
      : v(i.V())
    { RavlAssertMsg(v >= 0,"Range overflow in conversion."); }

    inline IndexC(IntT i = 0)
      : v(i)
    {}
    //: Creates the index with the value 'i'.

    inline IndexC(UInt64T s)
     : v((ISizeT) s)
    { RavlAssertMsg(v >= 0,"Range overflow in conversion."); }
    //: Creates the index with the same value as the value of size 's'.

    inline IndexC(Int64T s)
     : v((ISizeT) s)
    { RavlAssertMsg(v == s,"Range overflow in conversion."); }
    //: Creates the index with the same value as the value of size 's'.

    inline IndexC(UIntT s)
     : v((ISizeT) s)
    { RavlAssertMsg(v >= 0,"Range overflow in conversion."); }
    //: Creates the index with the same value as the value of size 's'.

    inline IndexC(const IndexC & i)
      : v(i.v)
    {}
    //: Creates the index with the same value as the index 'i' has.

    inline IndexC(RealT i)
      : v((IntT) RavlN::Floor(i)) // floor is needed for correct round of negative numbers.
    {}
    //: Creates the index = Floor(i)

    //:-
    // <h2>Access functions</h2>

    inline ISizeT V() const
    { return v; }
    //: Returns the current value of the index.

    inline ISizeT & V()
    { return v; }
    //: Returns the current value of the index.

    inline bool IsInRange(SizeT size) const
    { return SizeT(v) < size; }
    //: True if 0 <= this < size

    //:-
    // <h2>Conversions</h2>
    // The obvious conversions from index to integers
    // are missing to avoid automatic conversion of an index expression
    // into integer expression. In this way such a conversion
    // is detected in compilation time and one can decide which
    // type of expression is the proper one.

    inline operator bool() const
    { return v!=0; }
    //: Returns true if the index value is different from 0.

    inline operator ByteT() const
    { return (ByteT) v; }
    //: Truncates the index value and returns it as ByteT.

    inline operator SByteT() const
    { return (SByteT) v; }
    //: Truncates the index value and returns it as SByteT.

    inline operator FloatT() const
    { return (FloatT) v; }
    //: Converts the index value into float representation.

    inline operator RealT() const
    { return (RealT) v; }
    //: Converts the index value into double representation.

    inline operator SizeC() const
    { return SizeC(v); }
    //: Converts the index value into SizeC.

    //:-
    // <h2>Arithmetical operators</h2>

    //:-
    // <h3>Unary operators</h3>

    inline IndexC operator-() const
    { return -v; }
    //: Returns opposite index (unary minus operator).

    inline IndexC & operator++()
    { ++v; return *this; }
    //: The prefix operator increments the index value by 1.

    inline IndexC operator++(int)
    { return v++ ; }
    //: The postfix operator increments the index value by 1.

    inline IndexC & operator--()
    { --v; return *this; }
    //: The prefix operator decrements the index value by 1.

    inline IndexC operator--(int)
    { return v--; }
    //: The postfix operator decrements the index value by 1.

    //:-
    // <h3>Arithmetical operators with signed 32-bit integers</h3>

    inline IndexC operator+(IntT i) const
    { return v + i; }
    //: Returns a new index with value of this index added by integer 'i'.

    inline IndexC operator-(IntT i) const
    { return v - i; }
    //: Returns a new index with value of this index subtracted by integer 'i'.

    inline IndexC operator*(IntT i) const
    { return v * i; }
    //: Returns a new index with value of this index multiplied by integer 'i'.

    inline IndexC operator/(IntT i) const {
      if(i >= 0) return (v >= 0) ? (v/i) : (v-i+1)/i;
      else return -((*this)/(-i));
    }
    //: Returns a new index with value of this index divided by integer 'i'.

    inline IndexC operator%(IntT i) const {
      if(i >= 0) return (v >= 0) ? (v%i) : i-(-v)%i;
      else return (*this)%(-i);
    }
    //: Returns a new index with value of modulo operation between
    //: this index and integer 'i'.

    inline const IndexC & operator+=(IntT i)
    { v += i; return *this; }
    //: Returns this index added by integer 'i'.

    inline const IndexC & operator-=(IntT i)
    { v -= i; return *this; }
    //: Returns this index subtracted by integer 'i'.

    inline const IndexC & operator*=(IntT i)
    { v *= i; return *this; }
    //: Returns this index multiplied by integer 'i'.

    inline const IndexC & operator/=(IntT i) {
      (*this) = (*this)/i; // Use proper division, defined above.
      return *this;
    }
    //: Returns this index divided by integer 'i'.

    //:-
    // <h3>Arithmetical operators with unsigned 32-bit integers</h3>

    inline IndexC operator+(const UIntT i) const
    { return v + ISizeT(i); }
    //: Returns a new index with value of this index added by integer 'i'.

    inline IndexC operator-(const UIntT i) const
    { return v - ISizeT(i); }
    //: Returns a new index with value of this index subtracted by integer 'i'.

    inline IndexC operator*(const UIntT i) const
    { return ISizeT(v * ISizeT(i)); }
    //: Returns a new index with value of this index multiplied by integer 'i'.

    inline IndexC operator/(const UIntT i) const
    { return (v>=0)  ? (v/ISizeT(i)) : (v-ISizeT(i)+1)/ISizeT(i); }
    //: Returns a new index with value of this index divided by integer 'i'.

    inline IndexC operator%(const UIntT i) const
    { return (v>=0) ? (v%i) : i-(-v)%i; }
    //: Returns a new index with value of modulo operation between
    //: this index and integer 'i'.

    inline const IndexC & operator+=(const UIntT i)
    { v += i; return *this; }
    //: Returns this index added by integer 'i'.

    inline const IndexC & operator-=(const UIntT i)
    { v -= i; return *this; }
    //: Returns this index subtracted by integer 'i'.

    inline const IndexC & operator*=(const UIntT i)
    { v *= i; return *this; }
    //: Returns this index multiplied by integer 'i'.

    inline const IndexC & operator/=(const UIntT i) {
      (*this) = (*this)/i; // Use proper division, see above.
      return *this;
    }
    //: Returns this index divided by integer 'i'.

#if RAVL_CPUTYPE_64
    inline bool operator==(const Int64T i) const
    { return v == ISizeT(i); }
    //: Returns true if the value of this index is equal to
    //: the integer 'i'.

    inline bool operator!=(const Int64T i) const
    { return v != ISizeT(i); }
    //: Returns true if the value of this index is not equal to
    //: the integer 'i'.

    inline bool operator<(const Int64T i) const
    { return v < ISizeT(i); }
    //: Returns true if the value of this index is smaller than
    //: the integer 'i'.

    inline bool operator<=(const Int64T i) const
    { return v <= ISizeT(i); }
    //: Returns true if the value of this index is smaller than
    //: or equal to the integer 'i'.

    inline bool operator>(const Int64T i) const
    { return v > ISizeT(i); }
    //: Returns true if the value of this index is greater than
    //: the integer 'i'.

    inline bool operator>=(const Int64T i) const
    { return v >= ISizeT(i); }
    //: Returns true if the value of this index is greater than
    //: or equal to the integer 'i'.

#if 1
    inline IndexC operator+(const Int64T i) const
    { return v + i; }
    //: Returns a new index with value of this index added by integer 'i'.

    inline IndexC operator-(const Int64T i) const
    { return v - i; }
    //: Returns a new index with value of this index subtracted by integer 'i'.

    inline IndexC operator*(const Int64T i) const
    { return v * i; }
    //: Returns a new index with value of this index multiplied by integer 'i'.

    inline IndexC operator/(const Int64T i) const
    { return (v>=0)  ? (v/i) : (v-ISizeT(i)+1)/ISizeT(i); }
    //: Returns a new index with value of this index divided by integer 'i'.

    inline IndexC operator%(const Int64T i) const
    { return (v>=0) ? (v%i) : i-(-v)%i; }
    //: Returns a new index with value of modulo operation between
    //: this index and integer 'i'.
#endif
#if 1
    inline const IndexC & operator+=(Int64T i)
    { v += i; return *this; }
    //: Returns this index added by integer 'i'.

    inline const IndexC & operator-=(Int64T i)
    { v -= i; return *this; }
    //: Returns this index subtracted by integer 'i'.

    inline const IndexC & operator*=(Int64T i)
    { v *= i; return *this; }
    //: Returns this index multiplied by integer 'i'.

    inline const IndexC & operator/=(Int64T i) {
      (*this) = (*this)/i; // Use proper division, see above.
      return *this;
    }
#endif
#endif
#if RAVL_CPUTYPE_64
    //:-
    // <h3>Arithmetical operators with unsigned 64-bit integers</h3>

    inline IndexC operator+(const UInt64T i) const
    { return v + i; }
    //: Returns a new index with value of this index added by integer 'i'.

    inline IndexC operator-(const UInt64T i) const
    { return v - i; }
    //: Returns a new index with value of this index subtracted by integer 'i'.

    inline IndexC operator*(const UInt64T i) const
    { return v * i; }
    //: Returns a new index with value of this index multiplied by integer 'i'.

    inline IndexC operator/(const UInt64T i) const
    { return (v>=0)  ? (v/i) : (v-ISizeT(i)+1)/ISizeT(i); }
    //: Returns a new index with value of this index divided by integer 'i'.

    inline IndexC operator%(const UInt64T i) const
    { return (v>=0) ? (v%i) : i-(-v)%i; }
    //: Returns a new index with value of modulo operation between
    //: this index and integer 'i'.

    inline const IndexC & operator+=(const UInt64T i)
    { v += i; return *this; }
    //: Returns this index added by integer 'i'.

    inline const IndexC & operator-=(const UInt64T i)
    { v -= i; return *this; }
    //: Returns this index subtracted by integer 'i'.

    inline const IndexC & operator*=(const UInt64T i)
    { v *= i; return *this; }
    //: Returns this index multiplied by integer 'i'.

    inline const IndexC & operator/=(const UInt64T i) {
      (*this) = (*this)/i; // Use proper division, see above.
      return *this;
    }
    //: Returns this index divided by integer 'i'.

#endif

    //:-
    // <h3>Arithmetical operators with floating point numbers</h3>

    inline RealT operator+(RealT i) const
    { return (RealT) v + i; }
    //: Returns a RealT with value of this index, converted to RealT, + 'i'

    inline RealT operator-(RealT i) const
    { return (RealT) v - i; }
    //: Returns a RealT with value of this index, converted to RealT, - 'i'

    inline RealT operator*(RealT i) const
    { return (RealT) v * i; }
    //: Returns a RealT with value of this index, converted to RealT, * 'i'

    inline RealT operator/(RealT i) const
    { return (RealT) v / i; }
    //: Returns a RealT with value of this index, converted to RealT, / 'i'

    inline const IndexC & operator+=(RealT i)
    { v += IntT(RavlN::Floor(i)); return *this; }
    //: This index is incremented by 'i' rounded <i>down</i> to integer value.

    inline const IndexC & operator-=(RealT i)
    { v -= IntT(RavlN::Ceil(i)); return *this; }
    //: This index is decremented by 'i' rounded <i>up</i> to integer value.

    inline const IndexC & operator*=(RealT i)
    { v = IntT(RavlN::Floor(v * i)); return *this; }
    //: This index is converted to RealT, multiplied by 'i', and replaced
    //: with the result rounded down to IndexC.

    inline const IndexC & operator/=(RealT i)
    { v = IntT(RavlN::Floor(v / i)); return *this; }
    //: This index is converted to RealT, divided by 'i', and replaced
    //: with the result rounded down to IndexC.

    //:-
    // <h3>Arithmetical operators with indexes</h3>

    inline IndexC operator+(const IndexC & i) const
    { return v+i.v; }
    //: Returns a new index with value of this index plus the value
    //: of index 'i'.

    inline IndexC operator-(const IndexC & i) const
    { return v-i.v; }
    //: Returns a new index with value of this index minus the value
    //: of index 'i'.

    inline IndexC operator*(const IndexC & i) const
    { return v*i.v; }
    //: Returns a new index with value of this index multiplied by the value
    //: of index 'i'.

    inline IndexC operator/(const IndexC & i) const {
      if(i.v >= 0) return (v >= 0) ? (v/i.v) : (v-i.v+1)/i.v;
      else return -((*this)/(-i));
    }
    //: Returns a new index with value of this index divided by the value
    //: of index 'i'.

    inline IndexC operator%(const IndexC & i) const {
      if(i.v >= 0) return (v >= 0) ? (v%i.v) : i.v-(-v)%i.v;
      else return (*this)%(-i);
    }
    //: Returns a new index with value of this index modulo the value
    //: of index 'i'.

    inline const IndexC & operator+=(const IndexC & i)
    { v += i.v; return *this; }
    //: Returns this index added by index 'i'.

    inline const IndexC & operator-=(const IndexC & i)
    { v -= i.v; return *this; }
    //: Returns this index subtracted by index 'i'.

    inline const IndexC & operator*=(const IndexC & i)
    { v *= i.v; return *this; }
    //: Returns this index multiplied by index 'i'.

    inline const IndexC & operator/=(const IndexC & i)
    { *this = *this / i; return *this; }
    //: Returns this index divided by index 'i'.

    //:-
    // <h2>Shifting indexes</h2>

    inline IndexC operator<<(IntT i) const
    { return v << i; }
    //: Returns the new index with the value equal to the value of this
    //: index shifted 'i' position to the left.

    inline IndexC operator>>(IntT i) const
    { return v >> i; }
    //: Returns the new index with the value equal to the value of this
    //: index shifted 'i' position to the right.

    //:-
    // <h2>Logical operators</h2>

    // <h3>Logical operators with signed 32-bit integers</h3>
    inline bool operator==(IntT i) const
    { return v == i; }
    //: Returns true if the value of this index is equal to
    //: the integer 'i'.

    inline bool operator!=(IntT i) const
    { return v != i; }
    //: Returns true if the value of this index is not equal to
    //: the integer 'i'.

    inline bool operator<(IntT i) const
    { return v < i; }
    //: Returns true if the value of this index is smaller than
    //: the integer 'i'.

    inline bool operator<=(IntT i) const
    { return v <= i; }
    //: Returns true if the value of this index is smaller than
    //: or equal to the integer 'i'.

    inline bool operator>(IntT i) const
    { return v > i; }
    //: Returns true if the value of this index is greater than
    //: the integer 'i'.


    inline bool operator>=(IntT i) const
    { return v >= i; }
    //: Returns true if the value of this index is greater than
    //: or equal to the integer 'i'.

    //:-
    // <h3>Logical operators with unsigned 32-bit integers</h3>

    inline bool operator==(const UIntT i) const
    { return v == IntT(i); }
    //: Returns true if the value of this index is equal to
    //: the integer 'i'.

    inline bool operator!=(const UIntT i) const
    { return v != IntT(i); }
    //: Returns true if the value of this index is not equal to
    //: the integer 'i'.

    inline bool operator<(const UIntT i) const
    { return v < IntT(i); }
    //: Returns true if the value of this index is smaller than
    //: the integer 'i'.

    inline bool operator<=(const UIntT i) const
    { return v <= IntT(i); }
    //: Returns true if the value of this index is smaller than
    //: or equal to the integer 'i'.

    inline bool operator>(const UIntT i) const
    { return v > IntT(i); }
    //: Returns true if the value of this index is greater than
    //: the integer 'i'.

    inline bool operator>=(const UIntT i) const
    { return v >= IntT(i); }
    //: Returns true if the value of this index is greater than
    //: or equal to the integer 'i'.

    //:-
    // <h3>Logical operators with unsigned 64-bit integers</h3>

    inline bool operator==(const UInt64T i) const
    { return v == ISizeT(i); }
    //: Returns true if the value of this index is equal to
    //: the integer 'i'.

    inline bool operator!=(const UInt64T i) const
    { return v != ISizeT(i); }
    //: Returns true if the value of this index is not equal to
    //: the integer 'i'.

    inline bool operator<(const UInt64T i) const
    { return v < ISizeT(i); }
    //: Returns true if the value of this index is smaller than
    //: the integer 'i'.

    inline bool operator<=(const UInt64T i) const
    { return v <= ISizeT(i); }
    //: Returns true if the value of this index is smaller than
    //: or equal to the integer 'i'.

    inline bool operator>(const UInt64T i) const
    { return v > ISizeT(i); }
    //: Returns true if the value of this index is greater than
    //: the integer 'i'.

    inline bool operator>=(const UInt64T i) const
    { return v >= ISizeT(i); }
    //: Returns true if the value of this index is greater than
    //: or equal to the integer 'i'.


    // <h3>Logical operators with SizeC integers</h3>

    inline bool operator==(const SizeC &i) const
    {
      if(v < 0) return false;
      return static_cast<size_t>(v) == i.V();
    }
    //: Returns true if the value of this index is equal to
    //: the integer 'i'.

    inline bool operator!=(const SizeC & i) const
    {
      if(v < 0) return true;
      return static_cast<size_t>(v) != i.V();
    }
    //: Returns true if the value of this index is not equal to
    //: the integer 'i'.

    inline bool operator<(const SizeC & i) const
    {
      if(v < 0) return true;
      return static_cast<size_t>(v) < i.V();
    }
    //: Returns true if the value of this index is smaller than
    //: the integer 'i'.

    inline bool operator<=(const SizeC & i) const
    {
      if(v < 0) return true;
      return static_cast<size_t>(v) <= i.V();
    }
    //: Returns true if the value of this index is smaller than
    //: or equal to the integer 'i'.

    inline bool operator>(const SizeC & i) const
    {
      if(v < 0) return false;
      return static_cast<size_t>(v) > i.V();
    }
    //: Returns true if the value of this index is greater than
    //: the integer 'i'.

    inline bool operator>=(const SizeC &i) const
    {
      if(v < 0) return false;
      return static_cast<size_t>(v) >= i.V();
    }
    //: Returns true if the value of this index is greater than
    //: or equal to the integer 'i'.

    inline IndexC &operator+=(const SizeC & i)
    { v += i.V(); return *this; }
    //: Add a size.

    inline IndexC &operator-=(const SizeC & i)
    { v -= i.V(); return *this; }
    //: Subtract a size.

    inline IndexC &operator/=(const SizeC & i)
    { (*this) = (*this) / ISizeT(i.V()); return *this; }
    //: Add a size.

    inline IndexC &operator*=(const SizeC & i)
    { v *= i.V(); return *this; }
    //: Subtract a size.

    //:-
    // <h3>Logical operators with double numbers</h3>

    inline bool operator==(RealT i ) const
    { return v == i; }
    //: Returns true if the value of this index is equal to
    //: the integer 'i'.

    inline bool operator!=(RealT i ) const
    { return v != i; }
    //: Returns true if the value of this index is not equal to
    //: the integer 'i'.

    inline bool operator<(RealT i) const
    { return v < i; }
    //: Returns true if the value of this index is smaller than
    //: the double number 'i'.

    inline bool operator<=(RealT i) const
    { return v <= i; }
    //: Returns true if the value of this index is smaller than
    //: or equal to the double number 'i'.

    inline bool operator>(RealT i) const
    { return v > i; }
    //: Returns true if the value of this index is greater than
    //: the double number 'i'.

    inline bool operator>=(RealT i) const
    { return v >= i; }
    //: Returns true if the value of this index is greater than
    //: or equal to the double number 'i'.

    //:-
    // <h3>Logical operators with another index</h3>

    inline bool operator==(const IndexC & i ) const
    { return v==i.v; }
    //: Returns true if the value of this index and index 'i' have got
    //: the same value.

    inline bool operator!=(const IndexC & i ) const
    { return v!=i.v; }
    //: Returns true if the value of this index and index 'i' are different.

    inline bool operator<(const IndexC & i) const
    { return v < i.v; }
    //: Returns true if the value of this index is smaller than
    //: the value of index 'i'.

    inline bool operator<=(const IndexC & i) const
    { return v <= i.v; }
    //: Returns true if the value of this index is smaller than
    //: or equal to the value of index 'i'.

    inline bool operator>(const IndexC & i) const
    { return v > i.v; }
    //: Returns true if the value of this index is greater than
    //: the value of index 'i'.

    inline bool operator>=(const IndexC & i) const
    { return v >= i.v; }
    //: Returns true if the value of this index is greater than
    //: or equal to the value of index 'i'.

    //:-
    // <h2>Miscellaneous functions</h2>

    inline IndexC Abs() const
    { return RavlN::Abs(v); }
    //: Returns the index whose value is equal to the absolute value
    //: of this index.

    inline IndexC & SetAbs()
    { v = RavlN::Abs(v); return *this; }
    //: Changes this index to have the absolute value
    //: of the original index value.

    inline IndexC ILog2() const{
      IntT mex = 0;
      IntT i   = v;
      while((i/=2) != 0) mex++;
      return mex;
    }
    //: Returns the index which is logarithm of this index value
    //: with base 2.

    inline IndexC Min(const IndexC & i) const
    { return v <= i.v ? *this : i; }
    //: Returns the index with the smaller value.

    inline IndexC Max(const IndexC & i) const
    { return v >= i.v ? *this : i; }
    //: Returns the index with the larger value.

    //:-
    // <h2>Special member functions</h2>

    inline SizeT Hash() const
    { return SizeT(v); }
    //: Generates a randomised hash value for this index.

    inline SizeT Size() const
    { return dim; }
    //: Returns the number of dimensions indexed.

    //:-
    // <h2>See also <a href= "../Tree/Ravl.API.Core.Indexing.html">helper functions</a></h2>

  private:

    // Object Representation
    enum {dim = 1};  // number of indexes

    ISizeT v; // The value of the index.

 };

  // I/O operators
  // -------------

  std::istream & operator>>(std::istream & s, IndexC & r);
  //: Reads value for index from an input stream

  std::ostream & operator<<(std::ostream & s, const IndexC & r);
  //: Writes the index index to an output stream


  // Operations of integers and indexes
  // -----------------------------------------

  inline IndexC operator+(IntT i, const IndexC & j)
  { return i+j.V(); }

  //: Returns an index constructed from addition of integer 'i'
  //: and index value 'j'.

  inline IndexC operator-(IntT i, const IndexC & j)
  { return i-j.V(); }
  //: Returns an index constructed from subtraction of index value 'j'
  //: from integer 'i'.

  inline IndexC operator*(IntT i, const IndexC & j)
  { return i*j.V(); }
  //: Returns an index constructed from multiplication
  //: of integer 'i' and index value 'j'.

  inline IndexC operator/(IntT i, const IndexC & j)
  { return (IndexC(i)/j); }
  //: Returns an index constructed from division
  //: of integer 'i' by index value 'j'.

  inline const IntT & operator+=(IntT & i, const IndexC & j)
  { return i += j.V(); }
  //: Adds index value 'j' to integer 'i'.

  inline const IntT & operator-=(IntT & i, const IndexC & j)
  { return i -= j.V(); }
  //: Subtracts index value 'j' from integer 'i'.

  inline const IntT & operator*=(IntT & i, const IndexC & j)
  { return i *= j.V(); }
  //: Multiplies index value 'j' by integer 'i'.

  inline const IntT & operator/=(IntT & i, const IndexC & j) {
    i = (i / j).V();
    return i;
  }
  //: Divides index value 'j' by integer 'i'.

  inline bool operator==(IntT i, const IndexC & j)
  { return i==j.V(); }
  //: Returns true if the value of index 'j' and
  //: integer 'i' are equal.

  inline bool operator!=(IntT i, const IndexC & j)
  { return i!=j.V(); }
  //: Returns true if the value of index 'j' and
  //: integer 'i' are not equal.

  inline bool operator<(IntT i, const IndexC & j)
  { return i < j.V(); }
  //: Returns true if integer 'i' is less than
  //: the value of index 'j' .

  inline bool operator<=(IntT i, const IndexC & j)
  { return i <= j.V(); }
  //: Returns true if integer 'i' is less than
  //: or equal to the value of index 'j' .

  inline bool operator>(IntT i, const IndexC & j)
  { return i > j.V(); }
  //: Returns true if integer 'i' is greater than
  //: the value of index 'j' .

  inline bool operator>=(IntT i, const IndexC & j)
  { return i >= j.V(); }
  //: Returns true if integer 'i' is greater than
  //: or equal to the value of index 'j' .

  // Operations of 32-bit unsigned integers and indexes
  // --------------------------------------------------

  inline IndexC operator+(const UIntT i, const IndexC & j)
  { return IndexC(i+j.V()); }
  //: Returns the integer constructed as addition of integer 'i'
  //: and index value 'j'.

  inline IndexC operator-(const UIntT i, const IndexC & j)
  { return IndexC(i-j.V()); }
  //: Returns the integer constructed as subtraction of index value 'j'
  //: from integer 'i'.

  inline IndexC operator*(const UIntT i, const IndexC & j)
  { return IndexC(i*j.V()); }
  //: Returns the integer constructed as multiplication
  //: of integer 'i' and index value 'j'.

  inline IndexC operator/(const UIntT i, const IndexC & j) {
    RavlAssertMsg(j>0,
	  "Illegal division of unsigned by non-+ve.\n"
	  "In function: IntT operator/(const UIntT i, const IndexC & j)");
    return i/j.V();
  }
  //: Returns the integer constructed as division
  //: of integer 'i' and index value 'j'.

  inline const UIntT & operator+=(UIntT & i, const IndexC & j)
  { return i += j.V(); }
  //: Adds index value 'j' to integer 'i'.

  inline const UIntT & operator-=(UIntT & i, const IndexC & j)
  { return i -= j.V(); }
  //: Subtracts index value 'j' from integer 'i'.

  inline const UIntT & operator*=(UIntT & i, const IndexC & j)
  { return i *= j.V(); }
  //: Multiplies index value 'j' by integer 'i'.

  inline const UIntT & operator/=(UIntT & i, const IndexC & j){
    i = (i / j).V();
    return i;
  }
  //: Divides index value 'j' by integer 'i'.

  inline bool operator==(const UIntT i, const IndexC & j)
  { return IntT(i) == j.V(); }
  //: Returns true if the value of index 'j' is equal to
  //: the integer 'i'.

  inline bool operator!=(const UIntT i, const IndexC & j)
  { return IntT(i) != j.V(); }
  //: Returns true if the values of index 'j' is not equal to
  //: the integer 'i'.

  inline bool operator<(const UIntT i, const IndexC & j)
  { return IntT(i) < j.V(); }
  //: Returns true if the integer 'i' is smaller than
  //: the value of index 'j' .

  inline bool operator<=(const UIntT i, const IndexC & j)
  { return IntT(i) <= j.V(); }
  //: Returns true if the integer 'i' is smaller than
  //: or equal to the value of index 'j' .

  inline bool operator>(const UIntT i, const IndexC & j)
  { return IntT(i) > j.V(); }
  //: Returns true if the integer 'i' is greater than
  //: the value of index 'j' .

  inline bool operator>=(const UIntT i, const IndexC & j)
  { return IntT(i) >= j.V(); }
  //: Returns true if the integer 'i' is greater than
  //: or equal to the value of index 'j' .

  // ---------------------------------------------------
  // SizeC interactions.

  inline IndexC operator+(const IndexC &i, const SizeC & j)
  { return i.V() + ISizeT(j.V()); }
  //: Returns the integer constructed as addition of integer 'i'
  //: and index value 'j'.

  inline IndexC operator-(const IndexC &i, const SizeC & j)
  { return i.V() - ISizeT(j.V()); }
  //: Returns the integer constructed as subtraction of index value 'j'

  inline IndexC operator*(const IndexC &i, const SizeC & j)
  { return IndexC(i.V() *ISizeT(j.V())); }
  //: Multiply with size.

  inline IndexC operator/(const IndexC &i, const SizeC & j)
  { return IndexC(i/ISizeT(j.V())); }
  //: Divide by size.


  // Operations of 64-bit unsigned integers and indexes
  // --------------------------------------------------

  inline IndexC operator+(const UInt64T i, const IndexC & j)
  { return IndexC(i+j.V()); }
  //: Returns the integer constructed as addition of integer 'i'
  //: and index value 'j'.

  inline IndexC operator-(const UInt64T i, const IndexC & j)
  { return IndexC(i-j.V()); }
  //: Returns the integer constructed as subtraction of index value 'j'
  //: from integer 'i'.

  inline IndexC operator*(const UInt64T i, const IndexC & j)
  { return IndexC(i*j.V()); }
  //: Returns the integer constructed as multiplication
  //: of integer 'i' and index value 'j'.

  inline IndexC operator/(const UInt64T i, const IndexC & j) {
    RavlAssertMsg(j>0,
	  "Illegal division of unsigned by non-+ve.\n"
	  "In function: IntT operator/(const UInt64T i, const IndexC & j)");
    return i/j.V();
  }
  //: Returns the integer constructed as division
  //: of integer 'i' and index value 'j'.

  inline const UInt64T & operator+=(UInt64T & i, const IndexC & j)
  { return i += j.V(); }
  //: Adds index value 'j' to integer 'i'.

  inline const UInt64T & operator-=(UInt64T & i, const IndexC & j)
  { return i -= j.V(); }
  //: Subtracts index value 'j' from integer 'i'.

  inline const UInt64T & operator*=(UInt64T & i, const IndexC & j)
  { return i *= j.V(); }
  //: Multiplies index value 'j' by integer 'i'.

  inline const UInt64T & operator/=(UInt64T & i, const IndexC & j){
    i = (i / j).V();
    return i;
  }
  //: Divides index value 'j' by integer 'i'.

  inline bool operator==(const UInt64T i, const IndexC & j)
  { return IntT(i) == j.V(); }
  //: Returns true if the value of index 'j' is equal to
  //: the integer 'i'.

  inline bool operator!=(const UInt64T i, const IndexC & j)
  { return IntT(i) != j.V(); }
  //: Returns true if the values of index 'j' is not equal to
  //: the integer 'i'.

  inline bool operator<(const UInt64T i, const IndexC & j)
  { return IntT(i) < j.V(); }
  //: Returns true if the integer 'i' is smaller than
  //: the value of index 'j' .

  inline bool operator<=(const UInt64T i, const IndexC & j)
  { return IntT(i) <= j.V(); }
  //: Returns true if the integer 'i' is smaller than
  //: or equal to the value of index 'j' .

  inline bool operator>(const UInt64T i, const IndexC & j)
  { return IntT(i) > j.V(); }
  //: Returns true if the integer 'i' is greater than
  //: the value of index 'j' .

  inline bool operator>=(const UInt64T i, const IndexC & j)
  { return IntT(i) >= j.V(); }
  //: Returns true if the integer 'i' is greater than
  //: or equal to the value of index 'j' .


  // Operations of byte (unsigned char) numbers and indexes
  // --------------------------------------------------
  // ByteT binary operators omitted, as I don't feel they make much sense
  // (maybe I'm wrong)


  inline const UByteT & operator+=(UByteT & i, const IndexC & j)
  { return i += j.V(); }
  //: Adds index value 'j' to byte number 'i'.

  inline const UByteT & operator-=(UByteT & i, const IndexC & j)
  { return i -= j.V(); }
  //: Subtracts index value 'j' from byte number 'i'.

  inline const UByteT & operator*=(UByteT & i, const IndexC & j)
  { return i *= j.V(); }
  //: Multiplies index value 'j' by byte number 'i'.

  inline const UByteT & operator/=(UByteT & i, const IndexC & j) {
    RavlAssertMsg(j>0,
	  "Illegal division of unsigned by non-+ve\n"
	  "const UByteT & operator/=(UByteT & i, const IndexC & j))");
    return i /= j.V();
  }
  //: Divides index value 'j' by byte number 'i'.

  inline bool operator==(const UByteT i, const IndexC & j)
  { return IntT(i) == j.V(); }
  //: Returns true if the value of index 'j' is equal to
  //: the byte number 'i'.

  inline bool operator!=(const UByteT i, const IndexC & j)
  { return IntT(i) != j.V(); }
  //: Returns true if the values of index 'j' is not equal to
  //: the byte number 'i'.

  inline bool operator<(const UByteT i, const IndexC & j)
  { return IntT(i) < j.V(); }
  //: Returns true if the byte number 'i' is smaller than
  //: the value of index 'j' .

  inline bool operator<=(const UByteT i, const IndexC & j)
  { return IntT(i) <= j.V(); }
  //: Returns true if the byte number 'i' is smaller than
  //: or equal to the value of index 'j' .

  inline bool operator>(const UByteT i, const IndexC & j)
  { return IntT(i) > j.V(); }
  //: Returns true if the byte number 'i' is greater than
  //: the value of index 'j' .

  inline bool operator>=(const UByteT i, const IndexC & j)
  { return IntT(i) >= j.V(); }
  //: Returns true if the byte number 'i' is greater than
  //: or equal to the value of index 'j' .

  // Operations of double numbers and indexes
  // ----------------------------------------

  inline RealT operator+(RealT i, const IndexC & j)
  { return i+j.V(); }
  //: Returns the double number constructed as addition of double number 'i'
  //: and index value 'j'.

  inline RealT operator-(RealT i, const IndexC & j)
  { return i-j.V(); }
  //: Returns the double number constructed as subtraction of index value 'j'
  //: from double number 'i'.

  inline RealT operator*(RealT i, const IndexC & j)
  { return i*j.V(); }
  //: Returns the double number constructed as multiplication
  //: of double number 'i' and index value 'j'.

  inline RealT operator/(RealT i, const IndexC & j)
  { return i/j.V(); }
  //: Returns the double number constructed as division
  //: of double number 'i' and index value 'j'.

  inline const RealT & operator+=(RealT & i, const IndexC & j)
  { return i += j.V(); }
  //: Adds double number 'i' to index value 'j'.

  inline const RealT & operator-=(RealT & i, const IndexC & j)
  { return i -= j.V(); }
  //: Subtracts index value 'j' from double number 'i'.

  inline const RealT & operator*=(RealT & i, const IndexC & j)
  { return i *= j.V(); }
  //: Multiplies index value 'j' by double number 'i'.

  inline const RealT & operator/=(RealT & i, const IndexC & j)
  { return i /= j.V(); }
  //: Divides index value 'j' by double number 'i'.

  inline bool operator==(RealT i, const IndexC & j)
  { return i==j.V(); }
  //: Returns true if the value of index 'j' is equal to
  //: the double number 'i'.

  inline bool operator!=(RealT i, const IndexC & j)
  { return i!=j.V(); }
  //: Returns true if the values of index 'j' is not equal to
  //: the double number 'i'.

  inline bool operator<(RealT i, const IndexC & j)
  { return i < j.V(); }
  //: Returns true if the double number 'i' is smaller than
  //: the value of index 'j' .

  inline bool operator<=(RealT i, const IndexC & j)
  { return i <= j.V(); }
  //: Returns true if the double number 'i' is smaller than
  //: or equal to the value of index 'j' .

  inline bool operator>(RealT i, const IndexC & j)
  { return i > j.V(); }
  //: Returns true if the double number 'i' is greater than
  //: the value of index 'j' .

  inline bool operator>=(RealT i, const IndexC & j)
  { return i >= j.V(); }
  //: Returns true if the double number 'i' is greater than
  //: or equal to the value of index 'j' .

  inline IndexC StdCopy(IndexC x) { return IndexC(x); }
   //: Copy double.

  inline void SetZero(IndexC &p)
  { p = 0; }
  //: Set index to 0.

}

#endif

// IAPS - Image analysis program system.
// End of include file Index.hh
