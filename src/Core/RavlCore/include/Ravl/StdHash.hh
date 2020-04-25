// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVL_STDHASH_HEADER
#define RAVL_STDHASH_HEADER 1
///////////////////////////////////////////////////////////////////
//! file="Ravl/Core/Base/StdHash.hh"
//! lib=RavlCore
//! author="Charles Galambos"
//! date="1/9/1996"
//! docentry="Ravl.API.Core.Hash Tables"
//! example=WordFreq.cc
//! userlevel=Normal
//! rcsid="$Id$"

#include "Ravl/config.h"
#include "Ravl/Types.hh"

namespace RavlN {
  
  template<class K>
  inline size_t StdHash(const K &dat)
  { return dat.Hash(); }
  //: Default hash function.
  
  template<class K>
  inline bool HashIsEqual(const K &d1,const K &d2) 
  { return (bool) (d1 == d2); }
  
  bool HashIsEqual(const char *d1,const char *d2);
  //: Compair 'C' style strings.
  
  size_t StdHash(const char *dat);
  //: Hash 'C' style strings.
  
  inline size_t StdHash(bool dat)
  { return (size_t) dat; }
  //: Hash a bool.
  // Not alot you can do with this.
  
  inline size_t StdHash(const short dat)
  { return (size_t) (dat >> 7) ^ dat; }
  //: Hash short.
  
  inline size_t StdHash(const unsigned short dat)
  { return (size_t) (dat >> 7) ^ dat; }
  //: Hash unsigned short.
  
  inline size_t StdHash(const int dat)
  { return (size_t) (dat >> 11) ^ dat; }
  //: Hash int.
  
  inline size_t StdHash(const unsigned int dat)
  { return (size_t) (dat >> 11) ^ dat; }
  //: Hash unsigned int.
  
  inline size_t StdHash(const long dat)
  { return (size_t) (dat >> 11) ^ dat; }
  //: Hash long value.
  
  inline size_t StdHash(const unsigned long dat)
  { return (size_t) (dat >> 11) ^ dat; }
  //: Hash unsigned long.

  //: for windows. ------------------------------------
  //inline size_t Stdhash(const signed long int dat)
  //{ return (size_t) (dat >> 11) ^ dat; }
  //: Hash long value.
  
  
  inline size_t StdHash(const char dat)
  { return (size_t) (dat >> 3) ^ dat; }
  //: Hash char.
  
  inline size_t StdHash(const unsigned char dat)
  { return (size_t) (dat >> 3) ^ dat; }
  //: Hash unsigned char.

  inline size_t StdHash(const SizeT size)
  { return (size_t) size.V(); }
  //: Hash unsigned char.

  inline size_t StdHash(const RealT dat)
  { 
    union {
      RealT rv;
      UIntT iv[2];
    } tmp;
    tmp.rv = dat;
    if(sizeof(RealT) >= sizeof(UIntT)*2) // This should be optimised out at compile time
      tmp.iv[0] ^= tmp.iv[1];
    return tmp.iv[0];
  }
  //: Hash a real number.
  //: This is not good practice, but is useful
  //: to satisfy some template instances.
  
  inline size_t StdHash(const FloatT dat)
  { 
    union {
      FloatT fv;
      UIntT iv;
    } tmp;
    tmp.fv = dat;
    return tmp.iv;
  }
  //: Hash a real number.
  //: This is not good practice, but is useful
  //: to satisfy some template instances. 
  
// Disable "possible loss of data" warning
#if RAVL_COMPILER_VISUALCPP
#pragma warning ( push )
#pragma warning ( disable : 4244 )
#pragma warning ( disable : 4311 )
#endif

  inline
  size_t StdHash(const void *ptr) {
#if RAVL_CPUTYPE_64
	return (reinterpret_cast<UInt64T>(ptr) >> 2 ) ^ (reinterpret_cast<UInt64T>(ptr)>>15) ;
#else 
	return (reinterpret_cast<size_t>(ptr) >> 2) ^ (reinterpret_cast<size_t>(ptr) >> 15);
#endif 
}
  //: Hash function for void ptr's.
  
  inline
  size_t StdHash(void *ptr)
  { return StdHash(static_cast<const void *>(ptr)); }
  //: Hash function for void ptr's.

#if RAVL_CPUTYPE_32
  inline size_t StdHash(const Int64T dat)
  { return (size_t) (dat >> 17) ^ (dat >> 32) ^ dat; }
  //: Hash 64 bit int.
  
  inline size_t StdHash(const UInt64T dat)
  { return (size_t) (dat >> 17) ^ (dat >> 32) ^ dat; }
  //: Hash unsigned 64 bit int.
#endif
  
#if RAVL_COMPILER_VISUALCPP
#pragma warning ( pop )
#endif

}

#endif
