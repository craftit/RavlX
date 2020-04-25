//! license=own
//! author="Doug Lea, Modified by Charles Galambos"
//! rcsid="$Id$"
//! lib=RavlCore
/* 
//! file="Ravl/Core/Base/String.cc"
Copyright (C) 1988 Free Software Foundation
    written by Doug Lea (dl@rocky.oswego.edu)

This file is part of the GNU C++ Library.  This library is free
software; you can redistribute it and/or modify it under the terms of
the GNU Library General Public License as published by the Free
Software Foundation; either version 2 of the License, or (at your
option) any later version.  This library is distributed in the hope
that it will be useful, but WITHOUT ANY WARRANTY; without even the
implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
PURPOSE.  See the GNU Library General Public License for more details.
You should have received a copy of the GNU Library General Public
License along with this library; if not, write to the Free Software
Foundation, 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
*/

/* 
  StringC class implementation
 */


#include "Ravl/config.h"
// On systems like linux on ix86 is better to use the tuned
// 'C' string code.
#define USE_CSTRING 1

#define USE_STREAMPUT 0

#if RAVL_COMPILER_VISUALCPP
#pragma warning ( disable : 4244 )
#pragma warning ( disable : 4267 )
#include <stdarg.h>
#endif

#include "Ravl/String.hh"
#include "Ravl/Index.hh"

#if RAVL_OS_IRIX
#include <std.h>
#endif

// a hack to use atoll with the -ansi flag ! 
#if RAVL_OS_SOLARIS9
extern "C" { extern long long atoll(const char *); } ;
#endif 

#include <ctype.h>
#include <limits.h>
#include <stdio.h>


#include <stdlib.h>
#include "Ravl/Assert.hh"

#if RAVL_HAVE_ANSICPPHEADERS
#include <new>
#include <iostream>
#include <string>
#else
#include <new.h>
#include <iostream.h>
#include <string.h>
#endif

#ifndef __GNUG__
//#include <builtin.h>
#endif

#if USE_CSTRING
#include <string.h>
#endif

#undef OK

namespace RavlN {


#if RAVL_OS_LINUX || RAVL_OS_LINUX64 || RAVL_OS_SOLARIS || RAVL_OS_MACOSX
  StrRepC  _nilStrRepC = { 0, 1, RAVL_ATOMIC_INIT(1) , { 0 } }; // nil strings point here (Reference will never reach 0)
#else
  StrRepC  _nilStrRepC = { 0, 1, 1 , { 0 } }; // nil strings point here (Reference will never reach 0)
#endif

  /*
    the following inline fcts are specially designed to work
    in support of StringC classes, and are not meant as generic replacements
    for libc "str" functions.
    
    inline copy fcts -  I like left-to-right from->to arguments.
    all versions assume that `to' argument is non-null
    
    These are worth doing inline, rather than through calls because,
    via procedural integration, adjacent copy calls can be smushed
    together by the optimiser.
  */
  
  // copy n bytes
  inline static void ncopy(const char* from, char* to, int n) {
    if (from == to) 
      return ;
#if !USE_CSTRING
    const char *e = &to[n];
    while (to != e) 
      *to++ = *from++;
#else
    memmove(to,from,n);
#endif
  }
  
  // copy n bytes, null-terminate
  inline static void ncopy0(const char* from, char* to, int n) {
    if (from != to)  {
#if !USE_CSTRING
      const char *e = &to[n];
      while (to != e) 
	*to++ = *from++;
      *to = 0;
#else
      memmove(to,from,n);
      to[n] = 0;
#endif
    } else
      to[n] = 0;
  }
  
  // copy until null
  inline static void scopy(const char* from, char* to,int maxBuffer) {
    if (from == 0) 
      return ;
#if !USE_CSTRING
    while((*to++ = *from++) != 0);
#else
#if RAVL_COMPILER_VISUALCPPNET_2005
   strcpy_s(to,maxBuffer,from);
#else
    strcpy(to,from);
#endif
#endif
  }
  
  // copy right-to-left
  inline static void revcopy(const char* from, char* to, int n) {
    if (from == 0) 
      return ;
    while (--n >= 0) *to-- = *from--;
  }
  
  // inline  strlen
  inline static int slen(const char* t) {
    if (t == 0)
      return 0;
#if USE_CSTRING
    return strlen(t);
#else
    const char* a = t;
    while (*a++ != 0);
    return a - 1 - t;
#endif
  }
  
  // minimum & maximum representable rep size

#define MAXStrRepC_SIZE   ((unsigned int) (1 << (sizeof(int) * CHAR_BIT - 1)) - 1)
#define MINStrRepC_SIZE   32

// Rasing this from 4 to 8 avoids some problems std::string giving valgrind errors when converting to std::string.
// FIXME:- Investigate and raise bug report ?

#ifndef MALLOC_MIN_OVERHEAD
#define MALLOC_MIN_OVERHEAD  8
#endif

  // The basic allocation primitive:
  // Always round request to something close to a power of two.
  // This ensures a bit of padding, which often means that
  // concatenations don't have to realloc. Plus it tends to
  // be faster when lots of StringCs are created and discarded,
  // since just about any version of malloc (op new()) will
  // be faster when it can reuse identically-sized chunks

  inline static StrRepC* Snew(int newsiz) {
    unsigned int siz = sizeof(StrRepC) + newsiz + MALLOC_MIN_OVERHEAD;
    unsigned int allocsiz = MINStrRepC_SIZE;
    while (allocsiz < siz) allocsiz <<= 1;
    allocsiz -= MALLOC_MIN_OVERHEAD;
    RavlAssertMsg(allocsiz < MAXStrRepC_SIZE,"Requested length out of range");
    StrRepC* rep = (StrRepC*) new char[allocsiz];
    rep->sz = allocsiz - sizeof(StrRepC);
    ravl_atomic_set(&(rep->refs),0);
    return rep;
  }
  
  // Do-something-while-allocating routines.
  
  // We live with two ways to signify empty Sreps: either the
  // null pointer (0) or a pointer to the nilStrRepC.
  
  // We always signify unknown source lengths (usually when fed a char*)
  // via len == -1, in which case it is computed.
  
  // allocate, copying src if nonull
  
  StrRepC* Salloc(StrRepC* old, const char* src, int srclen,int newlen) {
    if(old != 0) {
      if(ravl_atomic_read(&old->refs) > 1) // If there's more than 1 ref replace it.
	old = 0;
      if(old == &_nilStrRepC) 
	old = 0;
    }
    if(srclen < 0) srclen = slen(src);
    if(newlen < srclen) newlen = srclen;
    StrRepC* rep;
    if (old == 0 || newlen > ((int) old->sz))
      rep = Snew(newlen);
    else
      rep = old;    
    rep->len = srclen;
    if(src != 0)
      ncopy0(src, rep->s, srclen);
    else
      rep->s[srclen] = 0; // Make sure its null terminated.
    return rep;
  }
  
  // reallocate: Given the initial allocation scheme, it will
  // generally be faster in the long run to get new space & copy
  // than to call realloc
  
  static StrRepC* Sresize(StrRepC* old, int newlen) {
    if(old != 0) {
      if(ravl_atomic_read(&old->refs) > 1) // If there's more than 1 ref replace it.
	old = 0;
      if(old == &_nilStrRepC) old = 0;
    }
    StrRepC* rep;
    if (old == 0)
      rep = Snew(newlen);
    else if (newlen > ((int) old->sz)) {
      rep = Snew(newlen);
      ncopy0(old->s, rep->s, old->len);
    } else
      rep = old;
    rep->len = newlen;
    return rep;
  }
  
  void
  StringC::alloc (int newsize) {
    unsigned int old_len = rep->len;
    rep = Sresize(rep, newsize);
    rep->len = old_len;
  }
  
  // like allocate, but we know that src is a StrRepC
  
  StrRepC* Scopy(StrRepC* old, const StrRepC* s) {
    if(old != 0) {
      if(ravl_atomic_read(&old->refs) > 1) // If there's more than 1 ref replace it.
	old = 0;
      if(old == &_nilStrRepC) old = 0;
    }
    if (s == &_nilStrRepC) s = 0;
    if (old == s) 
      return (old == 0)? &_nilStrRepC : old;
    else if (s == 0) {
      old->s[0] = 0;
      old->len = 0;
      return old;
    } else  {
      StrRepC* rep;
      int newlen = s->len;
      if (old == 0 || newlen > ((int) old->sz)) {
#if 0
	if (old != 0)
	  old->DecRefs();
#endif
	rep = Snew(newlen);
      }
      else
	rep = old;
      rep->len = newlen;
      ncopy0(s->s, rep->s, newlen);
      return rep;
    }
  }
  
  // allocate & concatenate
  
  StrRepC* Scat(StrRepC* old, const char* s, int srclen, const char* t, int tlen) {
    if(old != 0) {
      if(ravl_atomic_read(&old->refs) > 1) // If there's more than 1 ref replace it.
	old = 0;
      if(old == &_nilStrRepC) old = 0;
    }
    if (srclen < 0) srclen = slen(s);
    if (tlen < 0) tlen = slen(t);
    int newlen = srclen + tlen;
    StrRepC* rep;
    
    if (old == 0 || newlen > ((int) old->sz) || 
	(t >= old->s && t < &(old->s[old->len]))) // beware of aliasing
      rep = Snew(newlen);
    else
      rep = old;
    
    rep->len = newlen;
    
    ncopy(s, rep->s, srclen);
    ncopy0(t, &(rep->s[srclen]), tlen);
    return rep;
  }
  
  // double-concatenate
  
  StrRepC* Scat(StrRepC* old, const char* s, int srclen, const char* t, int tlen,
		const char* u, int ulen) {
    if(old != 0) {
      if(ravl_atomic_read(&old->refs) > 1) // If there's more than 1 ref replace it.
	old = 0;
      if(old == &_nilStrRepC) old = 0;
    }
    if (srclen < 0) srclen = slen(s);
    if (tlen < 0) tlen = slen(t);
    if (ulen < 0) ulen = slen(u);
    int newlen = srclen + tlen + ulen;
    StrRepC* rep;
    if (old == 0 || newlen > ((int) old->sz) || 
	(t >= old->s && t < &(old->s[old->len])) ||
	(u >= old->s && u < &(old->s[old->len])))
      rep = Snew(newlen);
    else
      rep = old;
    
    rep->len = newlen;
    
    ncopy(s, rep->s, srclen);
    ncopy(t, &(rep->s[srclen]), tlen);
    ncopy0(u, &(rep->s[srclen+tlen]), ulen);
    return rep;
  }
  
  // like cat, but we know that new stuff goes in the front of existing rep
  
  StrRepC* Sprepend(StrRepC* old, const char* t, int tlen) {
    char* s;
    int srclen;
    if(old != 0) {
      if(ravl_atomic_read(&old->refs) > 1) // If there's more than 1 ref replace it.
	old = 0;
      if(old == &_nilStrRepC) old = 0;
    }
    if (old == 0) {
      s = 0; old = 0; srclen = 0;
    } else {
      s = old->s; srclen = old->len;
    }
    if (tlen < 0) tlen = slen(t);
    int newlen = srclen + tlen;
    StrRepC* rep;
    if (old == 0 || newlen > ((int) old->sz) || 
	(t >= old->s && t < &(old->s[old->len])))
      rep = Snew(newlen);
    else
      rep = old;
    
    rep->len = newlen;
    
    revcopy(&(s[srclen]), &(rep->s[newlen]), srclen+1);
    ncopy(t, rep->s, tlen);
    return rep;
  }
  
  
  // string compare: first argument is known to be non-null
  
  inline static int scmp(const char* a, const char* b) {
    if (b == 0)
      return *a != 0;
    signed char diff = 0;
    while ((diff = *a - *b++) == 0 && *a++ != 0);
    return diff;
  }
  

  inline static int ncmp(const char* a, int al, const char* b, int bl) {
    int n = (al <= bl)? al : bl;
    signed char diff;
    const char *eos = &a[n];
    while (a != eos) 
      if ((diff = *a++ - *b++) != 0) 
	return diff;
    return al - bl;
  }
  
  int fcompare(const StringC& x, const StringC& y) {
    const char* a = x.chars();
    const char* b = y.chars();
    int al = x.length();
    int bl = y.length();
    int n = (al <= bl)? al : bl;
    signed char diff = 0;
    while (n-- > 0) {
      char ac = *a++;
      char bc = *b++;
      if ((diff = ac - bc) != 0) {
	if (ac >= 'a' && ac <= 'z')
	  ac = ac - 'a' + 'A';
	if (bc >= 'a' && bc <= 'z')
	  bc = bc - 'a' + 'A';
	if ((diff = ac - bc) != 0)
	  return diff;
      }
    }
    return al - bl;
  }
  
  // these are not inline, but pull in the above inlines, so are 
  // pretty fast
  
  int compare(const StringC& x, const char* b) {
    return scmp(x.chars(), b);
  }
  
  int compare(const StringC& x, const StringC& y) {
    return scmp(x.chars(), y.chars());
  }
  
  int compare(const StringC& x, const SubStringC& y) {
    return ncmp(x.chars(), x.length(), y.chars(), y.length());
  }
  
  int compare(const SubStringC& x, const StringC& y) {
    return ncmp(x.chars(), x.length(), y.chars(), y.length());
  }
  
  int compare(const SubStringC& x, const SubStringC& y) {
    return ncmp(x.chars(), x.length(), y.chars(), y.length());
  }
  
  int compare(const SubStringC& x, const char* b) {
    if (b == 0)
      return x.length();
    const char* a = x.chars();
    signed char diff;
    const char *eos = &a[x.length()];
    while (a != eos)
      if ((diff = *a++ - *b++) != 0) 
	return diff;
    return (*b == 0) ? 0 : -1;
  }
  
  //////////////////////////////////////
  // Non-inlines constructors.
  
  StringC::StringC(IndexC n)
    : rep(&_nilStrRepC)
  {
    char TBuff[32];
#if RAVL_CPUTYPE_64 
  sprintf(TBuff,"%ld",n.V());
#else 
#if RAVL_COMPILER_VISUALCPPNET_2005
  sprintf_s(TBuff,32,"%d",n.V());
#else
  sprintf(TBuff,"%d",n.V());
#endif
#endif 
    *this = TBuff;
  }

  StringC::StringC(IntT n)
    : rep(&_nilStrRepC)
  {
    char TBuff[32];
#if RAVL_COMPILER_VISUALCPPNET_2005
    sprintf_s(TBuff,32,"%d",n);
#else
    sprintf(TBuff,"%d",n);
#endif
    *this = TBuff;
  }

  StringC::StringC(UIntT n)
    : rep(&_nilStrRepC)
  {
    char TBuff[32];
#if RAVL_COMPILER_VISUALCPPNET_2005
    sprintf_s(TBuff,32,"%d",n);
#else
    sprintf(TBuff,"%d",n);
#endif
    *this = TBuff;
  }
  
  StringC::StringC(double f) 
    : rep(&_nilStrRepC)
  {
    char TBuff[64];
#if RAVL_COMPILER_VISUALCPPNET_2005
    sprintf_s(TBuff,64,"%f",f);
#else
	sprintf(TBuff,"%f",f);
#endif
    *this = TBuff;
  }
  
  //: Create a string from a float value
  StringC::StringC(float f) 
    : rep(&_nilStrRepC)
  {
    char TBuff[64];
#if RAVL_COMPILER_VISUALCPPNET_2005
    sprintf_s(TBuff,64,"%f",f);
#else
	sprintf(TBuff,"%f",f);
#endif
    *this = TBuff;
  }

  //: Create a string from Int
  
  StringC::StringC(Int64T n) {
    char TBuff[64];
#if RAVL_COMPILER_VISUALCPPNET_2005
    sprintf_s(TBuff,64,"%lld",n);
#else
#if RAVL_CPUTYPE_64
    sprintf(TBuff,"%ld",n);
#else
    sprintf(TBuff,"%lld",n);
#endif
#endif
    *this = TBuff;
  }
    
  //: Create a string from a size.

  StringC::StringC(const SizeC &n) {
    char TBuff[64];
#if RAVL_COMPILER_VISUALCPPNET_2005
    sprintf_s(TBuff,64,"%zu",n.V());
#else
    sprintf(TBuff,"%zu",n.V());
#endif
    *this = TBuff;
  }
  //: Create a string from unsigned int
  
  StringC::StringC(UInt64T n) {
    char TBuff[64];
#if RAVL_COMPILER_VISUALCPPNET_2005
   sprintf_s(TBuff,64,"%llu",n);
#else
#if RAVL_CPUTYPE_64
    sprintf(TBuff,"%lu",n);
#else
    sprintf(TBuff,"%llu",n);
#endif
#endif
    *this = TBuff;    
  }

  StringC::StringC(std::istream & in) {
    in >> *this;
  }

  //: Construct from a STL string.
  
  StringC::StringC(const std::string& x) 
    : rep(Salloc(0, x.data(), -1, x.size())) 
  {}
  
  StringC::operator std::string () const {
    return std::string(chars(),static_cast<size_t>(Size()));
  }
  
  /* 
     A Hash function found in the gcc compiler (so it must be good.)
  */

  unsigned int StringC::Hash(void) const {
    int len = length();
    unsigned int hash = 0;
    unsigned int c;
    const unsigned char *s = (const unsigned char *)chars();
    const unsigned char *e = &s[len];
    while (s != e) {
      c = *s++;
      hash += c + (c << 17);
      hash ^= hash >> 2;
    }
    hash += len + (len << 17);
    return hash;
  }
  
  /*
    index fcts
  */
  
  int StringC::search(int start, int sl, char c) const {
    const char* s = chars();
    if (sl > 0) {
      if (start >= 0) {
	const char* a = &(s[start]);
	const char* lasta = &(s[sl]);
	while (a < lasta) if (*a++ == c) return --a - s;
      } else {
	const char* a = &(s[sl + start + 1]);
	while (--a >= s) if (*a == c) return a - s;
      }
    }
    return -1;
  }
  
  int StringC::search(int start, int sl, const char* t, int tl) const {
    const char* s = chars();
    if (tl < 0) tl = slen(t);
    if (sl > 0 && tl > 0) {
      if (start >= 0) {
	const char* lasts = &(s[sl - tl]);
	const char* lastt = &(t[tl]);
	const char* p = &(s[start]);
	
	while (p <= lasts) {
	  const char* x = p++;
	  const char* y = t;
	  while (*x++ == *y++) if (y >= lastt) return --p - s;
	}
      } else {
	const char* firsts = &(s[tl - 1]);
	const char* lastt =  &(t[tl - 1]);
	const char* p = &(s[sl + start + 1]); 
	
	while (--p >= firsts) {
	  const char* x = p;
	  const char* y = lastt;
	  while (*x-- == *y--) if (y < t) return ++x - s;
	}
      }
    }
    return -1;
  }
  
  int StringC::match(int start, int sl, int exact, const char* t, int tl) const {
    if (tl < 0) tl = slen(t);
    
    if (start < 0) {
      start = sl + start - tl + 1;
      if (start < 0 || (exact && start != 0))
	return -1;
    } else 
      if (exact && sl - start != tl)
	return -1;
    
    if (sl == 0 || tl == 0 || sl - start < tl || start >= sl)
      return -1;
    
    int n = tl;
    const char* s = &(rep->s[start]);
    while (--n >= 0) if (*s++ != *t++) return -1;
    return tl;
  }
  
  void SubStringC::assign(const StrRepC* ysrc, const char* ys, int ylen) {
    //if (&S == &_nilStringC) return;
    if (ylen < 0) ylen = slen(ys);
    StrRepC* targ = S.rep;
    int sl = targ->len - len + ylen;
    
    if (ysrc == targ || sl >= ((int) targ->sz)) {
      StrRepC* oldtarg = targ;
      targ = Sresize(0, sl);
      ncopy(oldtarg->s, targ->s, pos);
      ncopy(ys, &(targ->s[pos]), ylen);
      scopy(&(oldtarg->s[pos + len]), &(targ->s[pos + ylen]),(ylen-pos)+1);
#if 0
      oldtarg->DecRefs();
#endif
    }
    else if (((int)len) == ylen)
      ncopy(ys, &(targ->s[pos]), len);
    else if (ylen < ((int) len)) {
      ncopy(ys, &(targ->s[pos]), ylen);
      scopy(&(targ->s[pos + len]), &(targ->s[pos + ylen]),ylen-pos+1);
    } else {
      revcopy(&(targ->s[targ->len]), &(targ->s[sl]), targ->len-pos-len +1);
      ncopy(ys, &(targ->s[pos]), ylen);
    }
    targ->len = sl;
    S.rep = targ;
  }
  
  /*
   * substitution
   */
  
  
  int StringC::_gsub(const char* pat, int pl, const char* r, int rl) {
    int nmatches = 0;
    if (pl < 0) pl = slen(pat);
    if (rl < 0) rl = slen(r);
    int sl = length();
    if (sl <= 0 || pl <= 0 || sl < pl)
      return nmatches;
    
    const char* s = chars();
    
    // prepare to make new rep
    StrRepP nrep;
    int nsz = 0;
    char* x = 0;
    
    int si = 0;
    int xi = 0;
    int remaining = sl;
    
    while (remaining >= pl) {
      int pos = search(si, sl, pat, pl);
      if (pos < 0)
	break;
      else {
	++nmatches;
	int mustfit = xi + remaining + rl - pl;
	if (mustfit >= nsz) {
	  if (nrep != 0) nrep->len = xi;
	  nrep = Sresize(nrep, mustfit);
	  nsz = nrep->sz;
	  x = nrep->s;
	}
	pos -= si;
	ncopy(&(s[si]), &(x[xi]), pos);
	ncopy(r, &(x[xi + pos]), rl);
	si += pos + pl;
	remaining -= pos + pl;
	xi += pos + rl;
      }
    }
    
    if (nrep == 0) {
      if (nmatches == 0)
	return nmatches;
      else
	nrep = Sresize(nrep, xi+remaining);
    }
    
    ncopy0(&(s[si]), &(x[xi]), remaining);
    nrep->len = xi + remaining;
    
    rep = nrep;
    return nmatches;
  }
  
#if USEREGEX
  int StringC::_gsub(const Regex& pat, const char* r, int rl) {
    int nmatches = 0;
    int sl = length();
    if (sl <= 0)
      return nmatches;
    
    if (rl < 0) rl = slen(r);
    
    const char* s = chars();
    
    StrRepC* nrep = 0;
    int nsz = 0;

    char* x = 0;
    
    int si = 0;
    int xi = 0;
    int remaining = sl;
    int  pos, pl = 0;				  // how long is a regular expression?
    
    while (remaining > 0) {
      pos = pat.search(s, sl, pl, si); // unlike string search, the pos returned here is absolute
      if (pos < 0 || pl <= 0)
	break;
      else {
	++nmatches;
	int mustfit = xi + remaining + rl - pl;
	if (mustfit >= nsz) {
	  if (nrep != 0) nrep->len = xi;
	  nrep = Sresize(nrep, mustfit);
	  x = nrep->s;
	  nsz = nrep->sz;
	}
	pos -= si;
	ncopy(&(s[si]), &(x[xi]), pos);
	ncopy(r, &(x[xi + pos]), rl);
	si += pos + pl;
	remaining -= pos + pl;
	xi += pos + rl;
      }
    }
    
    if (nrep == 0) {
      if (nmatches == 0)
	return nmatches;
      else
	nrep = Sresize(nrep, xi+remaining);
    }
    
    ncopy0(&(s[si]), &(x[xi]), remaining);
    nrep->len = xi + remaining;
    
    if (nrep->len <= rep->sz) {   // fit back in if possible
      rep->len = nrep->len;
      ncopy0(nrep->s, rep->s, rep->len);
#if 0
      nrep->DecRefs();
#endif
    } else {
#if 0
      rep->DecRefs();
#endif
      rep = nrep;
    }
    return nmatches;
  }
#endif
  
  /*
   * deletion
   */
  
  void StringC::del(int pos, int len) {
    if (pos < 0 || len <= 0 || (unsigned)(pos + len) > length()) return;
    int nlen = length() - len;
    int first = pos + len;
    if(ravl_atomic_read(&rep->refs) > 1) { // If there's more than 1 ref replace it.
      StrRepC *nrep = Snew(nlen); // Allocate new string of appropriate size.
      ncopy(rep->s,nrep->s,pos); // Copy head of string.
      ncopy0(&rep->s[first],&nrep->s[pos],length() - first); // Copy tail of string.
      rep = nrep;
      rep->len = nlen;
      return ;
    }
    ncopy0(&(rep->s[first]), &(rep->s[pos]), length() - first);
    rep->len = nlen;
  }
  
#if USEREGEX
  void StringC::del(const Regex& r, int startpos) {
    int mlen;
    int first = r.search(chars(), length(), mlen, startpos);
    del(first, mlen);
  }
#endif
  
  void StringC::del(const char* t, int startpos) {
    int tlen = slen(t);
    int p = search(startpos, length(), t, tlen);
    del(p, tlen);
  }
  
  void StringC::del(const StringC& y, int startpos) {
    del(search(startpos, length(), y.chars(), y.length()), y.length());
  }
  
  void StringC::del(const SubStringC& y, int startpos) {
    del(search(startpos, length(), y.chars(), y.length()), y.length());
  }
  
  void StringC::del(char c, int startpos) {
    del(search(startpos, length(), c), 1);
  }

  //: Print to string using good old 'C' sytle formating.
  // This isn't the saftest function, it uses a fixed
  // buffer of 4096 bytes.  <p>
  
  int StringC::form(const char *format ...) {
    const int formSize = 4096;
    va_list args;
    va_start(args,format);
    char buff[formSize];
    int x;
#if RAVL_COMPILER_VISUALCPPNET_2005
    if((x = vsprintf_s(buff,formSize,format,args)) < 0)
      std::cerr << "WARNING: StringC::form(...), String truncated!! \n";
    StringC oth(buff);
    (*this) = oth;  // Slower, but saves memory.
#elif RAVL_COMPILER_VISUALCPP
    if((x = _vsnprintf(buff,formSize,format,args)) < 0)
      std::cerr << "WARNING: StringC::form(...), String truncated!! \n";
    (*this) = StringC(buff);  // Slower, but saves memory.
#else
    if((x = vsnprintf(buff,formSize,format,args)) < 0)
      std::cerr << "WARNING: StringC::form(...), String truncated!! \n";
    (*this) = StringC(buff);  // Slower, but saves memory.
#endif
    va_end(args);
    return x;
  }
  
  /*
   * substring extraction
   */
  
  SubStringC StringC::at(const StringC& y, int startpos) {
    int first = search(startpos, length(), y.chars(), y.length());
    return _substr(first,  y.length());
  }
  
  SubStringC StringC::at(const SubStringC& y, int startpos) {
    int first = search(startpos, length(), y.chars(), y.length());
    return _substr(first, y.length());
  }
  
#if USEREGEX
  SubStringC StringC::at(const Regex& r, int startpos) {
    int mlen;
    int first = r.search(chars(), length(), mlen, startpos);
    return _substr(first, mlen);
  }
#endif
  
  SubStringC StringC::at(const char* t, int startpos) {
    int tlen = slen(t);
    int first = search(startpos, length(), t, tlen);
    return _substr(first, tlen);
  }
  
  SubStringC StringC::at(char c, int startpos) {
    int first = search(startpos, length(), c);
    return _substr(first, 1);
  }
  
  SubStringC StringC::before(const StringC& y, int startpos) {
    int last = search(startpos, length(), y.chars(), y.length());
    return _substr(0, last);
  }
  
  SubStringC StringC::before(const SubStringC& y, int startpos) {
    int last = search(startpos, length(), y.chars(), y.length());
    return _substr(0, last);
  }
  
#if USEREGEX
  SubStringC StringC::before(const Regex& r, int startpos) {
    int mlen;
    int first = r.search(chars(), length(), mlen, startpos);
    return _substr(0, first);
  }
#endif
  SubStringC StringC::before(char c, int startpos) {
    int last = search(startpos, length(), c);
    return _substr(0, last);
  }
  
  SubStringC StringC::before(const char* t, int startpos) {
    int tlen = slen(t);
    int last = search(startpos, length(), t, tlen);
    return _substr(0, last);
  }
  
  SubStringC StringC::through(const StringC& y, int startpos) {
    int last = search(startpos, length(), y.chars(), y.length());
    if (last >= 0) last += y.length();
    return _substr(0, last);
  }
  
  SubStringC StringC::through(const SubStringC& y, int startpos) {
    int last = search(startpos, length(), y.chars(), y.length());
    if (last >= 0) last += y.length();
    return _substr(0, last);
  }
  
#if USEREGEX
  SubStringC StringC::through(const Regex& r, int startpos) {
    int mlen;
    int first = r.search(chars(), length(), mlen, startpos);
    if (first >= 0) first += mlen;
    return _substr(0, first);
  }
#endif
  
  SubStringC StringC::through(char c, int startpos) {
    int last = search(startpos, length(), c);
    if (last >= 0) last += 1;
    return _substr(0, last);
  }
  
  SubStringC StringC::through(const char* t, int startpos) {
    int tlen = slen(t);
    int last = search(startpos, length(), t, tlen);
    if (last >= 0) last += tlen;
    return _substr(0, last);
  }
  
  SubStringC StringC::after(const StringC& y, int startpos) {
    int first = search(startpos, length(), y.chars(), y.length());
    if (first >= 0) first += y.length();
    return _substr(first, length() - first);
  }
  
  SubStringC StringC::after(const SubStringC& y, int startpos) {
    int first = search(startpos, length(), y.chars(), y.length());
    if (first >= 0) first += y.length();
    return _substr(first, length() - first);
  }

  SubStringC StringC::after(char c, int startpos) {
    int first = search(startpos, length(), c);
    if (first >= 0) first += 1;
    return _substr(first, length() - first);
  }
  
#if USEREGEX
  SubStringC StringC::after(const Regex& r, int startpos) {
    int mlen;
    int first = r.search(chars(), length(), mlen, startpos);
    if (first >= 0) first += mlen;
    return _substr(first, length() - first);
  }
#endif
  
  SubStringC StringC::after(const char* t, int startpos) {
    int tlen = slen(t);
    int first = search(startpos, length(), t, tlen);
    if (first >= 0) first += tlen;
    return _substr(first, length() - first);
  }

  SubStringC StringC::from(const StringC& y, int startpos) {
    int first = search(startpos, length(), y.chars(), y.length());
    return _substr(first, length() - first);
  }
  
  SubStringC StringC::from(const SubStringC& y, int startpos) {
    int first = search(startpos, length(), y.chars(), y.length());
    return _substr(first, length() - first);
  }
  
#if USEREGEX
  SubStringC StringC::from(const Regex& r, int startpos) {
    int mlen;
    int first = r.search(chars(), length(), mlen, startpos);
    return _substr(first, length() - first);
  }
#endif
  
  SubStringC StringC::from(char c, int startpos) {
    int first = search(startpos, length(), c);
    return _substr(first, length() - first);
  }
  
  SubStringC StringC::from(const char* t, int startpos) {
    int tlen = slen(t);
    int first = search(startpos, length(), t, tlen);
    return _substr(first, length() - first);
  }
  
  /*
   * split/join
   */
  
  
  int split(const StringC& src, StringC results[], int n, const StringC& sep) {
    StringC x = src;
    const char* s = x.chars();
    int sl = x.length();
    int i = 0;
    int pos = 0;
    while (i < n && pos < sl) {
      int p = x.search(pos, sl, sep.chars(), sep.length());
      if (p < 0)
	p = sl;
      results[i].rep = Salloc(results[i].rep, &(s[pos]), p - pos, p - pos);
      i++;
      pos = p + sep.length();
    }
    return i;
  }
  
#if USEREGEX
  int split(const StringC& src, StringC results[], int n, const Regex& r) {
    StringC x = src;
    const char* s = x.chars();
    int sl = x.length();
    int i = 0;
    int pos = 0;
    int p, matchlen;
    while (i < n && pos < sl) {
      p = r.search(s, sl, matchlen, pos);
      if (p < 0)
	p = sl;
      results[i].rep = Salloc(results[i].rep, &(s[pos]), p - pos, p - pos);
      i++;
      pos = p + matchlen;
    }
    return i;
  }
#endif
  
#if defined(__GNUG__) && !defined(_G_NO_NRV) && !defined(RAVL_COMPILER_GCC)
#define RETURN(r) return
#define RETURNS(r) return r;
#define RETURN_OBJECT(TYPE, NAME) /* nothing */
#else /* _G_NO_NRV */
#define RETURN(r) return r
#define RETURNS(r) /* nothing */
#define RETURN_OBJECT(TYPE, NAME) TYPE NAME;
#endif

  StringC join(StringC src[], int n, const StringC& separator) RETURNS(x) {
    RETURN_OBJECT(StringC,x)
      StringC sep = separator;
    int xlen = 0;
    int i;
    for (i = 0; i < n; ++i)
      xlen += src[i].length();
    xlen += (n - 1) * sep.length();
    
    x.rep = Sresize (x.rep, xlen);
    
    int j = 0;
    
    for (i = 0; i < n - 1; ++i) {
      ncopy(src[i].chars(), &(x.rep->s[j]), src[i].length());
      j += src[i].length();
      ncopy(sep.chars(), &(x.rep->s[j]), sep.length());
      j += sep.length();
    }
    ncopy0(src[i].chars(), &(x.rep->s[j]), src[i].length());
    RETURN(x);
  }
  
  /*
    misc
  */
  
  StrRepC* Sreverse(const StrRepC* src, StrRepC* dest) {
    int n = src->len;
    if (src != dest)
      dest = Salloc(dest, src->s, n, n);
    if (n > 0){
      char* a = dest->s;
      char* b = &(a[n - 1]);
      while (a < b){
	char t = *a;
	*a++ = *b;
	*b-- = t;
      }
    }
    return dest;
  }
  
  
  StrRepC* Supcase(const StrRepC* src, StrRepC* dest) {
    int n = src->len;
    if (src != dest) 
      dest = Salloc(dest, src->s, n, n);
    else {
      if(ravl_atomic_read(&dest->refs) > 1) // If there's more than 1 ref replace it.
	dest = Scopy(0,dest);
    }
    char* p = dest->s;
    char* e = &(p[n]);
    for (; p < e; ++p) if (islower(*p)) *p = toupper(*p);
    return dest;
  }
  
  StrRepC* Sdowncase(const StrRepC* src, StrRepC* dest) {
    int n = src->len;
    if (src != dest) 
      dest = Salloc(dest, src->s, n, n);
    else {
      if(ravl_atomic_read(&dest->refs) > 1) // If there's more than 1 ref replace it.
	dest = Scopy(0,dest);
    }
    char* p = dest->s;
    char* e = &(p[n]);
    for (; p < e; ++p) if (isupper(*p)) *p = tolower(*p);
    return dest;
  }
  
  StrRepC* Scapitalize(const StrRepC* src, StrRepC* dest) {
    int n = src->len;
    if (src != dest) 
      dest = Salloc(dest, src->s, n, n);
    else {
      if(ravl_atomic_read(&dest->refs) > 1) // If there's more than 1 ref replace it.
	dest = Scopy(0,dest);
    }    
    char* p = dest->s;
    char* e = &(p[n]);
    for (; p < e; ++p) {
      int at_word;
      if ((at_word = islower(*p)))
	*p = toupper(*p);
      else 
	at_word = isupper(*p) || isdigit(*p);
      
      if (at_word) {
	while (++p < e) {
	  if (isupper(*p))
	    *p = tolower(*p);
	  /* A '\'' does not break a word, so that "Nathan's" stays
	     "Nathan's" rather than turning into "Nathan'S". */
	  else if (!islower(*p) && !isdigit(*p) && (*p != '\''))
	    break;
	}
      }
    }
    return dest;
  }
  
#if defined(__GNUG__) && !defined(_G_NO_NRV) && !defined(RAVL_COMPILER_GCC)
  
  StringC StrCreplicate(char c, int n) return w; {
    w.rep = Sresize(w.rep, n);
    char* p = w.rep->s;
    while (n-- > 0) *p++ = c;
    *p = 0;
  }
  
  StringC StrCreplicateStrC(const StringC& y, int n) return w {
    int len = y.length();
    w.rep = Sresize(w.rep, n * len);
    char* p = w.rep->s;
    while (n-- > 0) {
      ncopy(y.chars(), p, len);
      p += len;
    }
    *p = 0;
  }
  
  StringC common_prefix(const StringC& x, const StringC& y, int startpos) return r; {
    const char* xchars = x.chars();
    const char* ychars = y.chars();
    const char* xs = &(xchars[startpos]);
    const char* ss = xs;
    const char* topx = &(xchars[x.length()]);
    const char* ys = &(ychars[startpos]);
    const char* topy = &(ychars[y.length()]);
    int l;
    for (l = 0; xs < topx && ys < topy && *xs++ == *ys++; ++l);
    r.rep = Salloc(r.rep, ss, l, l);
  }
  
  StringC common_suffix(const StringC& x, const StringC& y, int startpos) return r; {
    const char* xchars = x.chars();
    const char* ychars = y.chars();
    const char* xs = &(xchars[x.length() + startpos]);
    const char* botx = xchars;
    const char* ys = &(ychars[y.length() + startpos]);
    const char* boty = ychars;
    int l;
    for (l = 0; xs >= botx && ys >= boty && *xs == *ys ; --xs, --ys, ++l);
    r.rep = Salloc(r.rep, ++xs, l, l);
  }
  
#else

  StringC StrCreplicate(char c, int n) {
    StringC w;
    w.rep = Sresize(w.rep, n);
    char* p = w.rep->s;
    while (n-- > 0) *p++ = c;
    *p = 0;
    return w;
  }
  
  StringC StrCreplicate(const StringC& y, int n) {
    StringC w;
    int len = y.length();
    w.rep = Sresize(w.rep, n * len);
    char* p = w.rep->s;
    while (n-- > 0) {
      ncopy(y.chars(), p, len);
      p += len;
    }
    *p = 0;
    return w;
  }
  
  StringC common_prefix(const StringC& x, const StringC& y, int startpos) {
    StringC r;
    const char* xchars = x.chars();
    const char* ychars = y.chars();
    const char* xs = &(xchars[startpos]);
    const char* ss = xs;
    const char* topx = &(xchars[x.length()]);
    const char* ys = &(ychars[startpos]);
    const char* topy = &(ychars[y.length()]);
    int l = 0;
    for (; xs < topx && ys < topy && *xs++ == *ys++; ++l);
    r.rep = Salloc(r.rep, ss, l, l);
    return r;
  }
  
  StringC common_suffix(const StringC& x, const StringC& y, int startpos)  {
    StringC r;
    const char* xchars = x.chars();
    const char* ychars = y.chars();
    const char* xs = &(xchars[x.length() + startpos]);
    const char* botx = xchars;
    const char* ys = &(ychars[y.length() + startpos]);
    const char* boty = ychars;
    int l = 0;
    for (; xs >= botx && ys >= boty && *xs == *ys ; --xs, --ys, ++l);
    r.rep = Salloc(r.rep, ++xs, l, l);
    return r;
  }
  
#endif

// IO

  std::istream& operator>>(std::istream& s, StringC& x) {
    if (!s || (!(s.flags() & std::ios::skipws) && !ws(s))) {
      s.clear(std::ios::failbit|s.rdstate()); // Redundant if using GNU iostreams.
      return s;
    }
    int ch;
    int i = 0;
    x.rep = Sresize(x.rep, 20);
    std::streambuf *sb = s.rdbuf();
    std::ios_base::iostate new_state;
    // Skip spaces.
    while ((ch = sb->sbumpc()) != EOF) {
      if (!isspace(ch))
	break;
    }
    // Get rest of string.
    if(ch != EOF) {
      x.rep->s[i++] = ch;// Store first character.
      
      // Get string.
      while ((ch = sb->sbumpc()) != EOF) {
	if (isspace(ch))
	  break;
	if (i >= ((int) x.rep->sz) - 1)
	  x.rep = Sresize(x.rep, i+1);
	x.rep->s[i++] = ch;
      }
    }
    x.rep->s[i] = 0;
    x.rep->len = i;
    new_state = s.rdstate();
    if (i == 0) new_state |= std::ios::failbit;
    if (ch == EOF) new_state |= std::ios::eofbit;
#if RAVL_COMPILER_GCC && !RAVL_COMPILER_LLVM
    s.clear(new_state);
#else
    s.clear(new_state);
#endif
    return s;
  }
  
  int readline(std::istream& s, StringC& x, char terminator, bool discard) {
#if RAVL_COMPILER_GCC
    const int eof = std::istream::traits_type::eof();
#else
    // Should check what this does before just disabling it.
    if (!s.ipfx(0))
      return 0;
    const int eof = EOF;
#endif
    int ch = 0;
    int i = 0;
    x.rep = Sresize(x.rep, 80);
    std::streambuf *sb = s.rdbuf();
    while ((ch = sb->sbumpc()) != eof) {
      if (ch != terminator || !discard) {
	if (i >= ((int) x.rep->sz) - 1)
	  x.rep = Sresize(x.rep, i+1);
	x.rep->s[i++] = ch;
      }
      if (ch == terminator)
	break;
    }
    x.rep->s[i] = 0;
    x.rep->len = i;
#if RAVL_COMPILER_GCC
    if (ch == eof) 
      s.setstate(std::ios::eofbit | std::ios::failbit);
#else
    if (ch == eof)
      s.clear(ios::eofbit | ios::failbit);
#endif
    return i;
  }
  
  
  std::ostream& operator<<(std::ostream & s, const SubStringC& x) { 
#if USE_STREAMPUT
    const char* a = x.chars();
    const char* lasta = &(a[x.length()]);
    while (a < lasta)
      s.put(*a++);
#else
    s.write(x.chars(),x.length());
#endif
    return(s);
  }
  
  std::ostream& operator<<(std::ostream & s, const StringC& x) {
#if USE_STREAMPUT
    s << x.chars(); 
#else
    s.write(x.chars(),x.length());
#endif
    return s; 
  }
  
  // from John.Willis@FAS.RI.CMU.EDU
  
  int StringC::freq(const SubStringC& y) const {
    int found = 0;
    for (unsigned int i = 0; i < length(); i++) 
      if (match(i,length(),0,y.chars(), y.length())>= 0) found++;
    return(found);
  }
  
  int StringC::freq(const StringC& y) const {
    int found = 0;
    for (unsigned int i = 0; i < length(); i++) 
      if (match(i,length(),0,y.chars(),y.length()) >= 0) found++;
    return(found);
  }
  
  int StringC::freq(const char* t) const {
    int found = 0;
    for (unsigned int i = 0; i < length(); i++) 
      if (match(i,length(),0,t) >= 0) found++;
    return(found);
  }
  
  int StringC::freq(char c) const {
    int found = 0;
    for (unsigned int i = 0; i < length(); i++) 
      if (match(i,length(),0,&c,1) >= 0) found++;
    return(found);
  }
  
  //: Remove while space from begining and end of string.
  
  SubStringC StringC::TopAndTail() const {
    int i,j;
    // find Top position.
    for (i = 0; i < ((int) length()); i++) {
      if(!isspace((*this)[i]))
	break;
    }
    // find Tail position
    for (j = ((int) length())-1; j >= 0; j--) {
      if(!isspace((*this)[j]))
	break;
    }
    // Then chop
    return SubStringC(const_cast<StringC &>(*this).at( i, (j-i)+1));
  }

  //: Remove while space from begining and end of string.
  
  SubStringC SubStringC::TopAndTail() const {
    int i,j;
    // find Top position.
    for (i = 0; i < ((int) length()); i++) {
      if(!isspace((*this)[i]))
	break;
    }
    // find Tail position
    for (j = ((int) length())-1; j >= 0; j--) {
      if(!isspace((*this)[j]))
	break;
    }
    // Then chop
    return SubStringC(const_cast<SubStringC &>(*this).at( i, (j-i)+1));
  }

  //: Interpret stream as an integer.
  
  IntT StringC::IntValue() const {
    return atoi(chars());
  }
  
  //: Interpret string as an integer, returning success value.

  bool StringC::IntValue ( IntT & val ) const {
    char *endptr;
    val = strtol(chars(), &endptr, 10);
    return ((Size()>0) && *endptr == '\0');
  }

  //: Interpret stream as an unsigned integer.
  
  UIntT StringC::UIntValue() const {
    return (UIntT) atoi(chars());
  }
  
  //: Interpret string as a 64-bit integer.
  
  Int64T StringC::Int64Value() const {
#if RAVL_COMPILER_VISUALCPP
    if (matches("0x", 0, false))
    {
      Int64T val = 0;
      sscanf_s(chars(), "0x%llx", &val);
      return val;
    }
    return atol(chars());
#else
    if (matches("0x", 0, false))
    {
      Int64T val = 0;
#if RAVL_CPUTYPE_64
      sscanf(chars(), "0x%lx", &val);
#else
      sscanf(chars(), "0x%llx", &val);
#endif
      return val;
    }
    return atoll(chars());
#endif
  }
  
  //: Interpret string as a 64-bit unsigned integer.
  
  UInt64T StringC::UInt64Value() const {
#if RAVL_COMPILER_VISUALCPP
    if (matches("0x", 0, false))
    {
      Int64T val = 0;
      sscanf_s(chars(), "0x%llx", &val);
      return static_cast<UInt64T>(val);
    }
    return (UInt64T) atol(chars());
#else
    if (matches("0x", 0, false))
    {
      Int64T val = 0;
#if RAVL_CPUTYPE_32
      sscanf(chars(), "0x%llx", &val);
#else
      sscanf(chars(), "0x%lx", &val);
#endif
      return static_cast<UInt64T>(val);
    }
    return (UInt64T) atoll(chars());
#endif
  }
  
  //: Interpret stream as a floating point value.
  
  RealT StringC::RealValue() const {
    return atof(chars());
  }
  
  //: Interpret string as an integer.
  
  IntT SubStringC::IntValue() const {
    return atoi(chars());
  }
  
  //: Interpret stream as an integer.
  
  UInt64T SubStringC::UInt64Value() const {
#if RAVL_COMPILER_VISUALCPP
    return (UInt64T) atol(chars());
#else
    return (UInt64T) atoll(chars());
#endif
  }
  
  //: Interpret string as an integer.
  
  Int64T SubStringC::Int64Value() const {
#if RAVL_COMPILER_VISUALCPP
    return atol(chars());
#else
    return atoll(chars());
#endif
  }
  
  //: Interpret string as a floating point value.
  
  RealT SubStringC::RealValue() const {
    return atof(chars());
  }
  
  //: Test if this string is exactly the same as 'oth'. 
  // This is slightly faster than compare, as it doesn't
  // try and order the strings.
  
  bool StringC::IsEqual(const StringC &str2) const {
    if(Size() != str2.Size()) return false;
    const char *a = chars();
    const char *b = str2.chars();
    
    // Check if its a no brainer
    if(a == b) return true;
    const char *e = &(a[Size()]);
    
    // Do what we can in 4 byte chunks
    int x = Size() / sizeof(int);
    while(x-- > 0) {
      if(*reinterpret_cast<const int *>(a) != *reinterpret_cast<const int *>(b))
	return false;
      a += sizeof(int);
      b += sizeof(int);
    }
    
    // Finish off the last few letters
    for(;a != e;a++,b++) 
      if(*a != *b) return false;
    return true;
  }
  
  int StringC::OK() const {
    if (rep == 0             // don't have a rep
	|| rep->len > rep->sz     // string oustide bounds
	|| rep->s[rep->len] != 0)   // not null-terminated
      RavlAssertMsg(0,"invariant failure");
    return 1;
  }
  
  
  unsigned int SubStringC::Hash(void) const {
    unsigned int hash = 0;
    unsigned int c;
    const unsigned char *s = (const unsigned char *)&(S[pos]);
    const unsigned char *e = &s[len];
    while (s != e) {
      c = *s++;
      hash += c + (c << 17);
      hash ^= hash >> 2;
    }
    hash += len + (len << 17);
    return hash;
  }
  
  int SubStringC::OK() const {
    int v = S != (const char*)0; // have a StringC;
    v &= S.OK();                 // that is legal
    v &= pos + len >= S.rep->len;// pos and len within bounds
    RavlAssertMsg(v,"SubStringC invariant failure");
    return v;
  }


  //: Helper function for producing debug output.
  // create a string with 'level' spaces.
  StringC Indent(int level) {
    static StringC retEmpty;
    static StringC ret1(" ");
    static StringC ret2("  ");
    static StringC ret3("   ");
    static StringC ret4("    ");
    static StringC ret5("     ");
    static StringC ret6("      ");
    switch(level) {
      case 0: return retEmpty;
      case 1: return ret1;
      case 2: return ret2;
      case 3: return ret3;
      case 4: return ret4;
      case 5: return ret5;
      case 6: return ret6;
    }
    StringC ret;
    for(;level > 0;level--)
      ret += ' ';
    return ret;
  }


}
