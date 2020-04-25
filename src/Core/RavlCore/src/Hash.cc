// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//! rcsid="$Id$"
//! lib=RavlCore
//! file="Ravl/Core/Container/Hash/Hash.cc"

#include "Ravl/Hash.hh"
#include <string.h>

namespace RavlN {
  
  static const int numPrimes = 31;
  static const size_t primeList[numPrimes] = {
    7ul,          11ul,         29ul,
    53ul,         97ul,         193ul,       389ul,       769ul,
    1543ul,       3079ul,       6151ul,      12289ul,     24593ul,
    49157ul,      98317ul,      196613ul,    393241ul,    786433ul,
    1572869ul,    3145739ul,    6291469ul,   12582917ul,  25165843ul,
    50331653ul,   100663319ul,  201326611ul, 402653189ul, 805306457ul, 
    1610612741ul, 3221225473ul, 4294967291ul
  };
  
  SizeT HashBaseC::NextPrime(SizeT n) {
    // Do something a little more clever.
    const size_t* last = primeList + numPrimes;
    for(const size_t* pos = primeList;pos != last;pos++)
      if(*pos > n) {
	return *pos;
      }
    return *(last-1);
  }
  
  bool HashIsEqual(const char *d1,const char *d2) 
  { return (strcmp(d1,d2) == 0); }
  
  // Hash function based on one found in the GNU gcc compiler.
  
  size_t StdHash(const char *str)  {
    size_t hash = 0;
    size_t c;
    const char *s = str;
    while (*s != 0) {
      c = *s++;
      hash += c + (c << 17);
      hash ^= hash >> 2;
    }
    const unsigned int len = (unsigned int)(str - s);
    hash += len + (len << 17);
    hash ^= hash >> 2;
    return hash;
  }
}

