#ifndef RAVL_STRING_HEADER
#define RAVL_STRING_HEADER 1
/////////////////////////////////////////////////////////
//! rcsid="$Id$"
//! docentry="Ravl.API.Core.Strings"
//! file="Ravl/Core/Base/String.hh"
//! lib=RavlCore
//! license=own
//! author="Doug Lea, Modified by Charles Galambos"
/* 
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
// This may look like C code, but it is really -*- C++ -*-

#define STRDEBUG 0
  
//#ifndef VISUAL_CPP
//#define USEREGEX 0 // Use regular expressions.
//#endif

#define USEREGEX 0 // Use regular expressions.

#include "Ravl/config.h"

#if RAVL_HAVE_SYS_TYPES_H
// This must be included before <iostream>
#include <sys/types.h>
#endif

#if RAVL_HAVE_ANSICPPHEADERS
#include <iostream>
#else
#include <iostream.h>
#endif

#include "Ravl/Types.hh"
#include "Ravl/Index.hh"
#include "Ravl/Atomic.hh"

#if USEREGEX
#include "Ravl/Regex.hh"
#endif

#if STRDEBUG
#include <stdio.h>
#include "Ravl/Assert.hh"
#endif
#undef OK

namespace RavlN {
  
  class BinIStreamC;
  class BinOStreamC;
  class StringC;
  class SubStringC;
  class IndexC;

  //! userlevel=Normal
  
  BinOStreamC &operator<<(BinOStreamC &out,const StringC &str);
  //: Write a string to a binary stream.
  
  BinOStreamC &operator<<(BinOStreamC &out,const SubStringC &str);
  //: Write a substring to a binary stream.
  
  BinIStreamC &operator>>(BinIStreamC &in,StringC &str);
  //: Read a string from a binary stream.
  
  int compare(const StringC&    x, const StringC&     y);
  //: Compare two strings
  // Returns 0 if equal, and +ve or -ve value depending on alphabetic order.
  
  int compare(const StringC&    x, const SubStringC&  y);
  //: Compare two strings
  // Returns 0 if equal, and +ve or -ve value depending on alphabetic order.

  int compare(const StringC&    x, const char* y);
  //: Compare two strings
  // Returns 0 if equal, and +ve or -ve value depending on alphabetic order.

  int compare(const SubStringC& x, const StringC&     y);
  //: Compare two strings
  // Returns 0 if equal, and +ve or -ve value depending on alphabetic order.
  
  int compare(const SubStringC& x, const SubStringC&  y);
  //: Compare two strings
  // Returns 0 if equal, and +ve or -ve value depending on alphabetic order.

  int compare(const SubStringC& x, const char* y);
  //: Compare two strings
  // Returns 0 if equal, and +ve or -ve value depending on alphabetic order.
  
  int fcompare(const StringC&   x, const StringC&     y);
  //! userlevel=Normal
  //: Compare two strings ignoring case.
  // Returns 0 if equal, and +ve or -ve value depending on alphabetic order.
  
  /////////////////////////////////////
  //! userlevel=Develop
  //: Internal string string representation
  
  struct StrRepC                     // internal StringC representations
  {
    unsigned int      len;         // string length
    unsigned int      sz;          // allocated space
    ravl_atomic_t          refs;        // Reference counts.
    char              s[1];        // the string starts here 
                                   // (at least 1 char for trailing null)
                                   // allocated & expanded via non-public fcts
  };
  
  /////////////////////////////////////
  //! userlevel=Develop
  //: Pointer to a string representation.
  
  class StrRepP {
  public:
    void SetPtr(StrRepC *New) {
      if(New != 0) {
        if(ravl_atomic_read(&(New->refs)) == 0) {
          // Short cut avoiding atomic operation, which can be slow on SMP systems.
          ravl_atomic_set(&(New->refs),1);
        } else
	  ravl_atomic_inc(&(New->refs));
      }
      if(Where != 0) {
	if(ravl_atomic_dec_and_test(&(Where->refs)))
	  delete [] ((char *) Where);
      }
      Where = New;
    }
    // Change the pointer to another value.
    
    StrRepP(StrRepC *SR) { 
      if(SR != 0) {
        if(ravl_atomic_read(&(SR->refs)) == 0) {
          // Short cut avoiding atomic operation, which can be slow on SMP systems.
          ravl_atomic_set(&(SR->refs),1);
        } else
          ravl_atomic_inc(&(SR->refs));
      }
      Where = SR; 
    }
    // Create from a pointer to a string rep.
    
    StrRepP(const StrRepC *SR) { 
      Where = const_cast<StrRepC *>(SR);      
      if(Where != 0) {
        if(ravl_atomic_read(&Where->refs) == 0) {
          // Short cut avoiding atomic operation, which can be slow on SMP systems.
          ravl_atomic_set(&Where->refs,1);
        } else
	  ravl_atomic_inc(&(Where->refs));
      }
    }
    // Create from a pointer to a string rep.
    
    StrRepP(const StrRepP &Oth) { 
      Where = const_cast<StrRepC *>(Oth.Where); 
      if(Where != 0) 
	ravl_atomic_inc(&(Where->refs)); 
    }
    //: Create from another string rep.
    
    StrRepP(void) 
    { Where = 0; }
    //: Default constructor.
    
    ~StrRepP(void) {  
      if(Where != 0) {
        if(ravl_atomic_read(&Where->refs) == 1) {
          // Short cut avoiding atomic operation, which can be slow on SMP systems.
          delete [] ((char *) Where);
        } else
        {
          if(ravl_atomic_dec_and_test(&(Where->refs)))
            delete [] ((char *) Where);
        }
      }
    }
    //: Destructor.
    
    StrRepP &operator=(const StrRepP &Oth) { SetPtr(Oth.Where); return *this; }
    StrRepC *operator=(StrRepC *Oth) { SetPtr(Oth); return Where; }
    StrRepC *operator->() const { return Where; }
    operator StrRepC *() const { return Where; }

    bool operator==(void *val) const
    { return Where == val; }

    bool operator!=(void *val) const
    { return Where != val; }
  private:
    StrRepC *Where;
  };
  
  // primitive ops on StrRepCs -- nearly all StringC fns go through these.
  
  StrRepC*     Salloc(StrRepC*, const char*, int, int);
  StrRepC*     Scopy(StrRepC*, const StrRepC*);
  StrRepC*     Scat(StrRepC*, const char*, int, const char*, int);
  StrRepC*     Scat(StrRepC*, const char*, int,const char*,int, const char*,int);
  StrRepC*     Sprepend(StrRepC*, const char*, int);
  StrRepC*     Sreverse(const StrRepC*, StrRepC*);
  StrRepC*     Supcase(const StrRepC*, StrRepC*);
  StrRepC*     Sdowncase(const StrRepC*, StrRepC*);
  StrRepC*     Scapitalize(const StrRepC*, StrRepC*);

  extern StrRepC  _nilStrRepC;
  
  StringC     common_prefix(const StringC& x, const StringC& y,
                                   int startpos = 0);

  StringC     common_suffix(const StringC& x, const StringC& y,
                                   int startpos = -1);

  ////////////////////////////////
  //! userlevel=Basic
  //: A text string.
  // BIG or SMALL object ??
  // In Ravl Strings are semi-big and use a technique similar to copy-on-write. 
  // Handles are created to existing representations, however, when a user attempts to change 
  // the representation, it is COPIED first. 

  // THREADING:
  // This string implementation uses atomic reference counting, making the
  // management of handles to strings thread safe. This however does NOT mean
  // that string operations themselves are thread safe. You should take the normal
  // precautions when writing code that access the same variable from different
  // threads.
  
  class StringC {
    friend class      SubStringC;
    
  protected:
    StrRepP           rep;   // StringCs are pointers to their representations
    
    // some helper functions
    
    int               search(int, int, const char*, int x= -1) const;
    int               search(int, int, char) const;
    int               match(int, int, int, const char*, int = -1) const;
    int               _gsub(const char*, int, const char* ,int);
#if USEREGEX
    int               _gsub(const Regex&, const char*, int);
#endif
    SubStringC         _substr(int, int);
    

    StringC(StrRepC *x)
      : rep(x)
    {}
    //: Create a new string from a rep.
  public:
    
    //:-----------------------------------------------
    //: Constructors, copies, assignment, and destructor
    
    StringC()
      : rep(&_nilStrRepC) 
    {}
    //: Create a handle to the NULL string.
    
    StringC(const StringC& x)
      : rep(x.rep) 
    {} 
    //: Create a new handle to string 'x'
    
    StringC(const SubStringC&  y);
    //: Make a copy of substring 'x' in a real string
    
    StringC(const char* t)
      : rep(Salloc(0, t, -1, -1)) 
    {}
    //: Copy C style 't' string into a StringC.
    // t should be 0 terminated.
    
    StringC(const char* t, const SizeT len)
      : rep(Salloc(0, t, -1, len)) 
    {}
    //: Allocate new string, with len bytes in it.
    
    StringC(const char* t, SizeT useLen,SizeT allocLen)
      : rep(Salloc(0, t, useLen, allocLen))
    {}
    //: Create a string of length allocLen;
    // copy useLen characters from t into new string.
    
    StringC(const SizeT len, const char * defaultString)
      : rep(Salloc(0, defaultString, -1, len)) 
    { rep->len = len; }
    //: Construct a string which is a copy of 'defaultString's first 'len' characters.
    // Caution: if default string is shorter than 'len' the contents
    // of the resulting string are undefined.
    
    explicit StringC(const IndexC n);
    //: Create a string from an IndexC
    
    explicit StringC(IntT n);
    //: Create a string from an Int
    
    explicit StringC(UIntT n);
    //: Create a string from an unsigned int
    
    explicit StringC(Int64T n);
    //: Create a string from an Int
    
    explicit StringC(UInt64T n);
    //: Create a string from an unsigned 64 bit int

    explicit StringC(const SizeC &n);
    //: Create a string from a size.

    explicit StringC(RealT val); 
    //: Create a string from a real value
    
    explicit StringC(float val); 
    //: Create a string from a float value
    
    explicit StringC(char c)
      : rep(Salloc(0, &c, 1, 1)) 
    {}
    //: Create a string from a char "c"
    
    explicit StringC(std::istream & in);
    //: Create a string from the stream "in"
    
    StringC(const std::string& x);
    //: Construct from a STL string.
    
    ~StringC()
    {}
    //: Destructor.
    
    inline StringC Copy() const
    { return StringC(Scopy(0,rep)); }
    //: Make a copy of this string.
    
    inline StringC DeepCopy(UIntT levels) const
    { return (levels == 0) ?  *this : Copy(); }
    //: Make a deep copy.
    
    unsigned int Hash(void) const; 
    //: Create a hash value that is "fairly unique" for string.
    
    inline StringC& operator=(const StringC& y) { 
      rep = y.rep;
      return *this;
    }
    //: Copy handle of string 'y'.
    
    inline StringC& operator=(const char* t) {
      rep = Salloc(rep, t, -1, -1);
      return *this;
    }
    //: Assign a 'c' string to this string.
    
    inline StringC& operator=(char c) {
      rep = Salloc(rep, &c, 1, 1);
      return *this;
    }
    //: Assign a character to this string.
    
    inline StringC& operator=(const SubStringC&  y);
    //: Assign a substring to this string.

    //:-
    //: Concatenation
    
    inline StringC& operator+=(const StringC&     y); 
    //: Copy string 'y' to the end of this one.
    
    inline StringC& operator+=(const SubStringC&  y);
    //: Copy sub string 'y' to the end of this string.
    
    inline StringC& operator+=(const char* t);
    //: Copy 'C' style string 'y' to the end of this one.
    
    inline StringC& operator+=(char        c);
    //: Copy character 'c' to the end of this string.
    
    inline void prepend(const StringC&     y)
    { rep = Sprepend(rep, y.chars(), y.length()); }
    //: Prepend string 'y' before this one.
    
    inline void prepend(const SubStringC&  y);
    //: Prepend string 'y' before this one.
    
    inline void prepend(const char* t)
    { rep = Sprepend(rep, t, -1); }
    //: Prepend 'C' style string 'y' before this one
    
    inline void prepend(char c)
    { rep = Sprepend(rep, &c, 1); }
    //: Prepend character 'c' to this string.
    
    //:-
    //: Searching and matching

    inline int index(char c, int startpos = 0) const
    { return search(startpos, length(), c); }
    //: Return the index of the first occurrence of 'c' in this string.
    // returns -1 on failure.  The search starts from index 'startpos'.
    
    inline int index(const StringC& y, int startpos = 0) const
    { return search(startpos, length(), y.chars(), y.length()); }
    //: Return the index of the first occurrence of  'y' in this string.
    // returns -1 on failure.  The search starts from index 'startpos'.
    
    inline int index(const SubStringC& y, int startpos = 0) const;
    //: Return the index of the first occurrence of  'y' in this string.
    // returns -1 on failure.  The search starts from index 'startpos'.
    
    inline int index(const char* t, int startpos = 0) const
    { return search(startpos, length(), t); }
    //: Return the index of the first occurrence of  'y' in this string.
    // returns -1 on failure.  The search starts from index 'startpos'.
    
#if USEREGEX
    inline int index(const Regex& r, int startpos = 0) const
    { int unused;  return r.search(chars(), length(), unused, startpos); }
    //: Return the index of the first occurance of  'y' in this string.
    // returns -1 on failure.  The search starts from index 'startpos'.
#endif
    
    inline int contains (char c) const
    { return search(0, length(), c) >= 0; }
    //: Returns 1 if this string contains the character 'c'.
    
    inline int contains(const StringC& y) const
    { return search(0, length(), y.chars(), y.length()) >= 0; }
    //: Returns 1 if this string contains the sub-string 'y'.
    
    inline int contains(const SubStringC&  y) const;
    //: Returns 1 if this string contains the sub-string 'y'.
    
    inline int contains(const char* t) const
    { return search(0, length(), t) >= 0; }
    //: Returns 1 if this string contains the sub-string 't'.
    
    inline int contains(char c, int pos) const
    { return search(pos, length(), &c, 1) >= 0; }
    //: return 1 if target appears anywhere after position pos 
    // (or before, if pos is negative) in StringC; else 0
    
    inline int contains(const StringC& y, int pos) const
    { return search(pos, length(), y.chars(), y.length()) >= 0; }
    //: return 1 if target appears anywhere after position pos 
    // (or before, if pos is negative) in StringC; else 0
      
    inline int contains(const SubStringC& y, int pos) const;
    //: return 1 if target appears anywhere after position pos 
    // (or before, if pos is negative) in StringC; else 0
    
     inline int contains(const char* t, int pos) const
     { return search(pos, length(), t) >= 0; }
     //: return 1 if target appears anywhere after position pos
     // (or before, if pos is negative) in StringC; else 0


#if USEREGEX
    inline int contains(const Regex& r) const
    { int unused;  return r.search(chars(), length(), unused, 0) >= 0; }
    //: Returns 1 if this string contains the sub-string 'r'.
    
    inline int contains(const Regex&      r, int pos) const
    { return r.search(chars(), length(), pos) >= 0; }
    //: return 1 if target appears anywhere after position pos 
    // (or before, if pos is negative) in StringC; else 0
#endif
    
    inline int matches(char c, int pos = 0, const bool exact = true) const
    { return match(pos, length(), exact, &c, 1) >= 0; }
    //: return 1 if target appears at position pos in StringC; else 0
    
    inline int matches(const StringC& y, int pos = 0, const bool exact = true ) const
    { return match(pos, length(), exact, y.chars(), y.length()) >= 0; }
    //: return 1 if target appears at position pos in StringC; else 0
    
    inline int matches(const SubStringC& y, int pos = 0, const bool exact = true ) const;
    //: return 1 if target appears at position pos in StringC; else 0
    
    inline int matches(const char* t, int pos = 0, const bool exact = true) const
    { return match(pos, length(), exact, t) >= 0; }
    //: return 1 if target appears at position pos in StringC; else 0
    
#if USEREGEX
    inline int matches(const Regex& r, int pos = 0) const {
      int l = (pos < 0)? -pos : length() - p;
      return r.match(chars(), length(), pos) == l;
    }
    //: return 1 if target appears at position pos in StringC; else 0
#endif
    
    inline int CaseCmp (const StringC & str) const
    { return fcompare(*this,str); }
    //: case-insensitive comparison of strings 
    // (c.f. strcasecmp (3))
    // i.e. returns 0 if =, -1 if <, +1 if >.
    
    int form(const char *format ...);
    //: Print to this string using good old 'C' style formating.
    // This isn't the safest function: it uses a fixed
    // buffer of 4096 bytes.


    int freq(char c) const; 
    //: return number of occurrences of target in StringC
    
    int freq(const StringC& y) const;
    //: return number of occurrences of target in StringC

    int freq(const SubStringC& y) const;
    //: return number of occurrences of target in StringC
    
    int freq(const char* t) const;
    //: return number of occurrences of target in StringC
    
    //:-
    //: SubStringC extraction
    
    // Note that you can't take a substring of a const StringC, since
    // this leaves open the possibility of indirectly modifying the
    // StringC through the SubStringC
    
    inline SubStringC at(int pos, int len);
    inline SubStringC operator()(int pos, int len); // synonym for at
    
    SubStringC at(const StringC& x, int startpos = 0); 
    SubStringC at(const SubStringC& x, int startpos = 0); 
    SubStringC at(const char* t, int startpos = 0);
    SubStringC at(char c, int startpos = 0);
#if USEREGEX
    SubStringC at(const Regex& r, int startpos = 0); 
#endif
    
    inline SubStringC before(int  pos);
    SubStringC before(const StringC& x, int startpos = 0);
    SubStringC before(const SubStringC& x, int startpos = 0);
    SubStringC before(const char* t, int startpos = 0);
    SubStringC before(char c, int startpos = 0);
#if USEREGEX
    SubStringC before(const Regex& r, int startpos = 0);
#endif
    
    SubStringC through(int  pos);
    SubStringC through(const StringC& x, int startpos = 0);
    SubStringC through(const SubStringC& x, int startpos = 0);
    SubStringC through(const char* t, int startpos = 0);
    SubStringC through(char c,int startpos = 0);
#if USEREGEX
    SubStringC through(const Regex&       r, int startpos = 0);
#endif
    
    inline SubStringC from(int  pos);
    SubStringC from(const StringC& x, int startpos = 0);
    SubStringC from(const SubStringC&  x, int startpos = 0);
    SubStringC from(const char* t, int startpos = 0);
    SubStringC from(char c, int startpos = 0);
#if USEREGEX
    SubStringC from(const Regex& r, int startpos = 0);
#endif
    
    SubStringC after(int pos);
    SubStringC after(const StringC& x, int startpos = 0);
    SubStringC after(const SubStringC&  x, int startpos = 0);
    SubStringC after(const char* t, int startpos = 0);
    SubStringC after(char c, int startpos = 0);
#if USEREGEX
    SubStringC after(const Regex&  r, int startpos = 0);
#endif
    
    //:-
    //: Deleting and substitution

    void del(int pos, int len);
    //: delete len chars starting at pos
    
    void del(const StringC& y, int startpos = 0);
    //: delete the first occurrence of target after startpos
    
    void del(const SubStringC& y, int startpos = 0);
    //: delete the first occurrence of target after startpos
    
    void del(const char* t, int startpos = 0);
    //: delete the first occurrence of target after startpos
    
    void del(char c, int startpos = 0);
    //: delete the first occurrence of target after startpos
    
#if USEREGEX
    void del(const Regex& r, int startpos = 0);
    //: delete the first occurrence of target after startpos
#endif
    
    inline int gsub(const StringC& pat, const StringC& repl)
    { return _gsub(pat.chars(), pat.length(), repl.chars(), repl.length()); }
    //: global substitution: substitute all occurrences of pat with repl
    
    inline int gsub(const SubStringC& pat, const StringC& repl);
    //: global substitution: substitute all occurrences of pat with repl
    
    inline int gsub(const char* pat, const StringC&  repl)
    { return _gsub(pat, -1, repl.chars(), repl.length()); }
    //: global substitution: substitute all occurrences of pat with repl
    
    inline int gsub(const char* pat, const char* repl)
    { return _gsub(pat, -1, repl, -1); }
    //: global substitution: substitute all occurrences of pat with repl
    
#if USEREGEX
    inline int gsub(const Regex& pat, const StringC& repl)
    { return _gsub(pat, r.chars(), r.length()); }
    //: global substitution: substitute all occurrences of pat with repl
#endif
    
    //:-
    //: Friends & utilities
    
    
    friend int        split(const StringC& x, StringC res[], int maxn, 
			    const StringC& sep);
    //: Split string into array res at separators; return number of elements
#if USEREGEX
    friend int        split(const StringC& x, StringC res[], int maxn, 
			    const Regex&  sep);
    //: Split string into array res at separators; return number of elements
#endif
    
    friend StringC     common_prefix(const StringC& x, const StringC& y, 
				     int startpos);
    //: Find common prefix for x &amp; y

    friend StringC     common_suffix(const StringC& x, const StringC& y, 
				     int startpos);
    //: Find common suffix for x &amp; y

    friend StringC     StrCreplicate(char c, int n);
    //: Return char c replicated n times

    friend StringC     StrCreplicate(const StringC&     y, int n);
    //: Return string y replicated n times

    friend StringC     StrCreplicateStrC(const StringC& y, int n);
    //: Return string y replicated n times
    //!deprecated: For Visual C++ compatibility

    friend StringC     join(StringC src[], int n, const StringC& sep);
    //: Returns string composed of concatenation of C-style array <code>src</code> of strings, interspersed with separators <code>sep</code>
    
    //:-
    // Simple builtin transformations
    
    friend inline StringC reverse(const StringC& x);
    friend inline StringC upcase(const StringC& x);
    friend inline StringC downcase(const StringC& x);
    friend inline StringC capitalize(const StringC& x);
    
    void reverse() 
    { rep = Sreverse(rep, rep); }
    //: Reverse order of characters in string, in place.
    
    void upcase() 
    { rep = Supcase(rep, rep); }
    //: Convert all characters to upper case, in place.
    
    void downcase() 
    { rep = Sdowncase(rep, rep); }
    //: Convert all characters to lower case, in place.
    
    void capitalize() 
    { rep = Scapitalize(rep, rep); }
    //: Convert all characters to upper case, in place.
    
    inline char&       operator[](UIntT i) { 
      RavlAssertMsg(((unsigned)i) < length(),"invalid index");
      return rep->s[i];
    }
    
    inline const char& operator[](UIntT i) const{ 
      RavlAssertMsg(((unsigned)i) < length(),"invalid index");
      return rep->s[i];
    }
    
    inline char&       operator[](IndexC i) { 
      RavlAssertMsg(((unsigned)i.V()) < length(),"invalid index");
      return rep->s[i.V()];
    }
    
    inline const char& operator[](IndexC i) const  { 
      RavlAssertMsg(((unsigned)i.V()) < length(),"invalid index");
      return rep->s[i.V()];
    }
    
    inline char&       operator[](int i) { 
      RavlAssertMsg(((unsigned)i) < length(),"invalid index");
      return rep->s[i];
    }
    
    inline const char& operator[](int i) const { 
      RavlAssertMsg(((unsigned)i) < length(),"invalid index");
      return rep->s[i];
    }
    
#if RAVL_CPUTYPE_64

    inline char&       operator[](SizeT i) { 
      RavlAssertMsg(i < length(),"invalid index");
      return rep->s[i];
    }
    
    inline const char& operator[](SizeT i) const { 
      RavlAssertMsg(i < length(),"invalid index");
      return rep->s[i];
    }
    
#endif

    char    elem(int i) const { 
      RavlAssertMsg(((unsigned)i) < length(),"invalid index");
      return rep->s[i];
    }
    
    
    char firstchar() const
    { return elem(0); }
    //: Get the first character in the string.
    
    char lastchar() const
    { return elem(length() - 1); }
    //: Get the last character in the string.
    
    operator const char*() const
    { return &(rep->s[0]); }    
    //: Access as a 'C' style string.
    
    inline const char* chars() const
    { return &(rep->s[0]); }    
    //: Access as a 'C' style string.
    
    inline const char* data() const
    { return &(rep->s[0]); }    
    //: Access string data, in the stl style.

    inline const char* c_str() const
    { return &(rep->s[0]); }
    //: Access as a 'C' style string, in the stl style.

    operator std::string () const;
    //: Convert to a STL string.
    
    // IO
    
    friend std::ostream& operator<<(std::ostream& s, const StringC& x);
    friend std::ostream& operator<<(std::ostream& s, const SubStringC& x);
    friend std::istream& operator>>(std::istream& s, StringC& x);
    friend BinIStreamC& operator>>(BinIStreamC& s, StringC& x);
    
    friend int readline(std::istream& s, StringC& x, 
			char terminator,
			bool discard_terminator);
    
    //:-
    //: Status
    
    inline unsigned int length() const
    { return rep->len; }    
    //: Get length of string.
    
    inline int empty() const
    { return rep->len == 0; } 
    //: Test if string is empty.
    
    inline bool IsEmpty() const
    { return rep->len == 0; } 
    //: Test if the string is empty.
    
    bool IsEqual(const StringC &oth) const;
    //: Test if this string is exactly the same as 'oth'. 
    // This is slightly faster than compare, as it doesn't
    // try and order the strings.
    
    void alloc(int newsize);
    //: Preallocate some space for StringC
  
    int allocation() const
    { return rep->sz; }    
    //: report current allocation (not length!)

    inline SizeT Size() const
    { return rep->len; }
    //: Get the length of this string.
    
    inline SizeT size() const
    { return rep->len; }
    //: Get the length of this string, stl style.
    
    int  OK() const;
    //: Check it valid.
    
    SubStringC TopAndTail() const;
    //: Remove while space from beginning and end of string.
    
    IntT IntValue() const;
    //: Interpret string as an integer.
    
    bool IntValue ( IntT & val ) const;
    //: Interpret string as an integer.
    // Returns conversion success.  If successful, value is returned as "val"

    UIntT UIntValue() const;
    //: Interpret string as an unsigned integer.
    
    Int64T Int64Value() const;
    //: Interpret string as a 64-bit integer.
    
    UInt64T UInt64Value() const;
    //: Interpret string as a 64-bit unsigned integer.
    
    RealT RealValue() const;
    //: Interpret string as a floating point value.
    
    /* concatenate first 2 args, store result in last arg */
    
    friend inline void     cat(const StringC&, const StringC&, StringC&);
    friend inline void     cat(const StringC&, const SubStringC&, StringC&);
    friend inline void     cat(const StringC&, const char*, StringC&);
    friend inline void     cat(const StringC&, char, StringC&);
    
    friend inline void     cat(const SubStringC&, const StringC&, StringC&);
    friend inline void     cat(const SubStringC&, const SubStringC&, StringC&);
    friend inline void     cat(const SubStringC&, const char*, StringC&);
    friend inline void     cat(const SubStringC&, char, StringC&);
    
    friend inline void     cat(const char*, const StringC&, StringC&);
    friend inline void     cat(const char*, const SubStringC&, StringC&);
    friend inline void     cat(const char*, const char*, StringC&);
    friend inline void     cat(const char*, char, StringC&);
    
    friend inline void     cat(const char, const StringC&, StringC&);
    friend inline void     cat(const char, const SubStringC&, StringC&);
    friend inline void     cat(const char, const char*, StringC&);
    friend inline void     cat(const char, char, StringC&);
    
    // double concatenation, by request. (yes, there are too many versions, 
    // but if one is supported, then the others should be too...)
    // Concatenate first 3 args, store in last arg
    
    friend inline void     cat(const StringC&,const StringC&, const StringC&,StringC&);
    friend inline void     cat(const StringC&,const StringC&,const SubStringC&,StringC&);
    friend inline void     cat(const StringC&,const StringC&, const char*, StringC&);
    friend inline void     cat(const StringC&,const StringC&, char, StringC&);
    friend inline void     cat(const StringC&,const SubStringC&,const StringC&,StringC&);
    friend inline void     cat(const StringC&,const SubStringC&,const SubStringC&,StringC&);
    friend inline void     cat(const StringC&,const SubStringC&, const char*, StringC&);
    friend inline void     cat(const StringC&,const SubStringC&, char, StringC&);
    friend inline void     cat(const StringC&,const char*, const StringC&,    StringC&);
    friend inline void     cat(const StringC&,const char*, const SubStringC&, StringC&);
    friend inline void     cat(const StringC&,const char*, const char*, StringC&);
    friend inline void     cat(const StringC&,const char*, char, StringC&);
    
    friend inline void     cat(const char*, const StringC&, const StringC&,StringC&);
    friend inline void     cat(const char*,const StringC&,const SubStringC&,StringC&);
    friend inline void     cat(const char*,const StringC&, const char*, StringC&);
    friend inline void     cat(const char*,const StringC&, char, StringC&);
    friend inline void     cat(const char*,const SubStringC&,const StringC&,StringC&);
    friend inline void     cat(const char*,const SubStringC&,const SubStringC&,StringC&);
    friend inline void     cat(const char*,const SubStringC&, const char*, StringC&);
    friend inline void     cat(const char*,const SubStringC&, char, StringC&);
    friend inline void     cat(const char*,const char*, const StringC&,    StringC&);
    friend inline void     cat(const char*,const char*, const SubStringC&, StringC&);
    friend inline void     cat(const char*,const char*, const char*, StringC&);
    friend inline void     cat(const char*,const char*, char, StringC&);
        
  };
  
  ////////////////////////////////
  //! userlevel=Normal
  //: Sub-string within a larger string.
  // This class holds the position and length of a sub string
  // within a string.  This holds a reference to the string
  // data and so memory management is taken care of automatically.
  
  class SubStringC {
  protected:
    StringC S;        // The StringC I'm a substring of
    unsigned int pos;      // starting position in S's rep
    unsigned int len;      // length of substring
    
    void assign(const StrRepC*, const char*, int = -1);
    
    SubStringC(StringC& x, int first, int l)
      : S(x), pos(first), len(l)
    {}
    //: Construct from a string.
    
  public:
    SubStringC()
      : pos(0),len(0)
    {}
    //: Default constructor.
    
    SubStringC(const SubStringC& x)
      : S(x.S), pos(x.pos), len(x.len) 
    {}
    //: Copy constructor.
    
    SubStringC(const char *txt)
      : S(txt), pos(0)
    { len = S.length(); }
    //: Construct from a 'C' string,
    
    SubStringC(const StringC& x)
      : S(x), pos(0), len(x.length()) 
    {}
    //: Construct from a StringC.
    
    // Note there are no public constructors. SubStringCs are always
    // created via StringC operations
    
    ~SubStringC()
    {}
    //: Destructor.
    
    SubStringC& operator=(const StringC& y) {
      S = y;
      pos = 0;
      len = y.length();
      return *this;
    }
    //: Make a substring from a complete string.
    
    SubStringC& operator=(const SubStringC& y) {
      S = y.S;
      pos = y.pos;
      len = y.len;
      return *this;
    }
    //: Copy a sub-string handle.
    
    SubStringC& operator=(const char* t)  {
      S = t;
      pos = 0;
      len = S.length();
      return *this;
    }
    //: Assign to a 'C' string.
    
    SubStringC& operator=(char c)  {
      S = c;
      pos = 0;
      len = S.length();      
      return *this;
    }
    //: Create a substring of a single character.
    
    int contains(char c) const
    { return S.search(pos, pos+len, c) >= 0; }
    //: return 1 if target appears anywhere in SubStringC; else 0
    
    int contains(const StringC& y) const
    { return S.search(pos, pos+len, y.chars(), y.length()) >= 0; }
    //: return 1 if target appears anywhere in SubStringC; else 0
    
    int contains(const SubStringC& y) const
    { return S.search(pos, pos+len, y.chars(), y.length()) >= 0; }
    //: return 1 if target appears anywhere in SubStringC; else 0
    
    int contains(const char* t) const
    { return S.search(pos, pos+len, t) >= 0; }
    //: return 1 if target appears anywhere in SubStringC; else 0
    
    SubStringC at(int spos, int len)
    { return S.at((int) pos + spos,(int) len); }
    //: Get a string of 'len' bytes starting at spos in sub-string.
    
    SubStringC before(int spos)
    { return S.at((int) pos,spos); }
    //: All characters before position 'spos' in substring.
    
    SubStringC through(int spos)
    { return S.at((int) pos,spos+1); }
    //: All characters before and including 'spos' in substring.
    
    SubStringC from(int  spos)
    { return S.at((int) pos+spos,len-spos); }
    //: All characters from position 'spos' in substring.
    
    SubStringC after(int spos)
    { spos++; return S.at((int) pos+spos,(int) len-spos); }
    //: All characters after position 'spos' in substring.
    
    inline int index(char c, int startpos = 0) const
    { return S.search(startpos+pos, length()+pos, c) - pos; }
    //: Return the index of the first occurrence of 'c' in this string.
    // returns -1 on failure.  The search starts from index 'startpos'.
    
    inline int index(const StringC& y, int startpos = 0) const
    { return S.search(startpos+pos, length()+pos, y.chars(), y.length()) - pos; }
    //: Return the index of the first occurrence of  'y' in this string.
    // returns -1 on failure.  The search starts from index 'startpos'.
    
    inline int index(const SubStringC& y, int startpos = 0) const
    { return S.search(startpos+pos, length()+pos, y.chars(), y.length()) - pos; }
    //: Return the index of the first occurrence of  'y' in this string.
    // returns -1 on failure.  The search starts from index 'startpos'.
    
    inline int index(const char* t, int startpos = 0) const
    { return S.search(startpos+pos, length()+pos, t)  - pos; }
    //: Return the index of the first occurrence of  'y' in this string.
    // returns -1 on failure.  The search starts from index 'startpos'.
    
#if USEREGEX
    inline int index(const Regex& r, int startpos = 0) const
    { int unused;  return r.search(chars(), length()+pos, unused, startpos); }
    //: Return the index of the first occurrence of  'y' in this string.
    // returns -1 on failure.  The search starts from index 'startpos'.
#endif
    
#if USEREGEX
    int contains(const Regex& r) const
    { int unused;  return r.search(chars(), len, unused, 0) >= 0; }
    // return 1 if target matches entire SubStringC
    
    inline int matches(const Regex&  r) const
    { return r.match(chars(), len, 0) == (int) len; }
#endif
    
    unsigned int Hash() const; 
    //: Return a hash number for substring.
    
    SubStringC TopAndTail() const;
    //: Remove while space from beginning and end of string.
    
    IntT IntValue() const;
    //: Interpret string as an integer.
    
    Int64T Int64Value() const;
    //: Interpret string as an integer.
    
    UInt64T UInt64Value() const;
    //: Interpret string as an integer.
    
    RealT RealValue() const;
    //: Interpret string as a floating point value.
   
    // IO 
    
    friend std::ostream& operator<<(std::ostream& s, const SubStringC& x);
    
    // status
    
    unsigned int length() const
    { return len; }    
    //: Get the length of the substring.

    inline SizeT Size() const
    { return len; }
    //: Access size of sub string.
    
    int empty() const
    { return len == 0; }    
    //: Test if the substring is empty.
    
    bool IsEmpty() const
    { return len == 0; }
    //: Test if sub string is empty.
    
    const char* chars() const
    { return &(S.rep->s[pos]); } 
    //: Convert to a 'c' style string.
    // note this will not put a 0 at the end of the substring!

    char &operator[](int i) {
      RavlAssertMsg(((unsigned)i) < length(),"invalid index");
      return (S.rep->s[pos+i]);
    }
    //: Access a character.
    
    char operator[](int i) const {
      RavlAssertMsg(((unsigned)i) < length(),"invalid index");
      return (S.rep->s[pos+i]);
    }
    //: Access a character.

    char &firstchar() 
    { return (S.rep->s[pos]); } 
    //: Access first character in substring.

    char &lastchar() 
    { return (S.rep->s[pos+len-1]); } 
    //: Access last character in substring.
    
    int OK() const; 
    //: Test if the substring is valid.

    friend class StringC;
  };

  int readline(std::istream& s, StringC& x, 
	       char terminator = '\n',
	       bool discard_terminator=true);
  //: Read a line of text from a stream.
  // Returns the number of characters read into the string 'x'.
  
  
  inline StringC::StringC(const SubStringC&  y)
    : rep(Salloc(0, y.chars(), y.length(), y.length())) 
  {}
  //: Make a copy of substring 'x' in a real string

  inline StringC& StringC::operator=(const SubStringC&  y) {
    rep = Salloc(rep, y.chars(), y.length(), y.length());
    return *this;
  }

  // a helper needed by at, before, etc.
  
  inline SubStringC StringC::_substr(int first, int l) {
    if (first < 0 || (unsigned)(first + l) > length() ) {
      StringC empty;
      return SubStringC(empty, 0, 0) ;
    }
    return SubStringC(*this, first, l);
  }
  
  inline void StringC::prepend(const SubStringC&  y)
  { rep = Sprepend(rep, y.chars(), y.length()); }
  
  inline int StringC::index(const SubStringC& y, int startpos) const
  { return search(startpos, length(), y.chars(), y.length()); }

  SubStringC StringC::at(int first, int len) 
  { return _substr(first, len); }
  
  SubStringC StringC::operator() (int first, int len) 
  { return _substr(first, len); }
  
  SubStringC StringC::before(int pos) 
  { return _substr(0, pos);}
  
  inline SubStringC StringC::through(int pos) 
  { return _substr(0, pos+1); }
  
  inline SubStringC StringC::after(int pos) 
  { return _substr(pos + 1, length() - (pos + 1)); }
  
  inline SubStringC StringC::from(int pos) 
  { return _substr(pos, length() - pos); }
  
  inline int StringC::contains(const SubStringC&  y) const
  { return search(0, length(), y.chars(), y.length()) >= 0; }
  
  inline int StringC::contains(const SubStringC& y, int pos) const
  { return match(pos, length(), 0, y.chars(), y.length()) >= 0; }
  
  inline int StringC::matches(const SubStringC& y, int pos, const bool exact) const
  { return match(pos, length(), exact, y.chars(), y.length()) >= 0; }
  
  inline int StringC::gsub(const SubStringC& pat, const StringC& repl)
  { return _gsub(pat.chars(), pat.length(), repl.chars(), repl.length()); }
    

  // Zillions of cats...
  
  inline void cat(const StringC& x, const StringC& y, StringC& r)
  { r.rep = Scat(r.rep, x.chars(), x.length(), y.chars(), y.length()); }

  inline void cat(const StringC& x, const SubStringC& y, StringC& r)
  { r.rep = Scat(r.rep, x.chars(), x.length(), y.chars(), y.length()); }
  
  inline void cat(const StringC& x, const char* y, StringC& r)
  { r.rep = Scat(r.rep, x.chars(), x.length(), y, -1); }
  
  inline void cat(const StringC& x, char y, StringC& r)
  { r.rep = Scat(r.rep, x.chars(), x.length(), &y, 1); }
  
  inline void cat(const SubStringC& x, const StringC& y, StringC& r)
  { r.rep = Scat(r.rep, x.chars(), x.length(), y.chars(), y.length()); }
  
  inline void cat(const SubStringC& x, const SubStringC& y, StringC& r)
  { r.rep = Scat(r.rep, x.chars(), x.length(), y.chars(), y.length()); }
  
  inline void cat(const SubStringC& x, const char* y, StringC& r)
  { r.rep = Scat(r.rep, x.chars(), x.length(), y, -1); }
  
  inline void cat(const SubStringC& x, char y, StringC& r)
  { r.rep = Scat(r.rep, x.chars(), x.length(), &y, 1); }
  
  inline void cat(const char* x, const StringC& y, StringC& r)
  { r.rep = Scat(r.rep, x, -1, y.chars(), y.length()); }
  
  inline void cat(const char* x, const SubStringC& y, StringC& r)
  { r.rep = Scat(r.rep, x, -1, y.chars(), y.length()); }
  
  inline void cat(const char* x, const char* y, StringC& r)
  { r.rep = Scat(r.rep, x, -1, y, -1); }
  
  inline void cat(const char* x, char y, StringC& r)
  { r.rep = Scat(r.rep, x, -1, &y, 1); }
  
  inline void cat(const char x, const StringC& y, StringC& r)
  { r.rep = Scat(r.rep, StringC(x), -1, y.chars(), y.length()); }
  
  inline void cat(const char x, const SubStringC& y, StringC& r)
  { r.rep = Scat(r.rep, StringC(x), -1, y.chars(), y.length()); }
  
  inline void cat(const char x, const char* y, StringC& r)
  { r.rep = Scat(r.rep, StringC(x), -1, y, -1); }
  
  inline void cat(const char x, char y, StringC& r)
  { r.rep = Scat(r.rep, StringC(x), -1, &y, 1); }
  
  inline void cat(const StringC& a, const StringC& x, const StringC& y, StringC& r)
  { r.rep = Scat(r.rep, a.chars(), a.length(), x.chars(), x.length(), y.chars(), y.length()); }
  
  inline void cat(const StringC& a, const StringC& x, const SubStringC& y, StringC& r)
  { r.rep = Scat(r.rep, a.chars(), a.length(), x.chars(), x.length(), y.chars(), y.length()); }
  
  inline void cat(const StringC& a, const StringC& x, const char* y, StringC& r)
  { r.rep = Scat(r.rep, a.chars(), a.length(), x.chars(), x.length(), y, -1); }

  inline void cat(const StringC& a, const StringC& x, char y, StringC& r)
  { r.rep = Scat(r.rep, a.chars(), a.length(), x.chars(), x.length(), &y, 1); }
  
  inline void cat(const StringC& a, const SubStringC& x, const StringC& y, StringC& r)
  { r.rep = Scat(r.rep, a.chars(), a.length(), x.chars(), x.length(), y.chars(), y.length()); }
  
  inline void cat(const StringC& a, const SubStringC& x, const SubStringC& y, StringC& r)
  { r.rep = Scat(r.rep, a.chars(), a.length(), x.chars(), x.length(), y.chars(), y.length()); }
  
  inline void cat(const StringC& a, const SubStringC& x, const char* y, StringC& r)
  { r.rep = Scat(r.rep, a.chars(), a.length(), x.chars(), x.length(), y, -1); }
  
  inline void cat(const StringC& a, const SubStringC& x, char y, StringC& r)
  { r.rep = Scat(r.rep, a.chars(), a.length(), x.chars(), x.length(), &y, 1); }
  
  inline void cat(const StringC& a, const char* x, const StringC& y, StringC& r)
  { r.rep = Scat(r.rep, a.chars(), a.length(), x, -1, y.chars(), y.length()); }
  
  inline void cat(const StringC& a, const char* x, const SubStringC& y, StringC& r)
  { r.rep = Scat(r.rep, a.chars(), a.length(), x, -1, y.chars(), y.length()); }
  
  inline void cat(const StringC& a, const char* x, const char* y, StringC& r)
  { r.rep = Scat(r.rep, a.chars(), a.length(), x, -1, y, -1); }
  
  inline void cat(const StringC& a, const char* x, char y, StringC& r)
  { r.rep = Scat(r.rep, a.chars(), a.length(), x, -1, &y, 1); }
  
  inline void cat(const char* a, const StringC& x, const StringC& y, StringC& r)
  { r.rep = Scat(r.rep, a, -1, x.chars(), x.length(), y.chars(), y.length()); }
  
  inline void cat(const char* a, const StringC& x, const SubStringC& y, StringC& r)
  { r.rep = Scat(r.rep, a, -1, x.chars(), x.length(), y.chars(), y.length()); }
  
  inline void cat(const char* a, const StringC& x, const char* y, StringC& r)
  { r.rep = Scat(r.rep, a, -1, x.chars(), x.length(), y, -1); }
  
  inline void cat(const char* a, const StringC& x, char y, StringC& r)
  { r.rep = Scat(r.rep, a, -1, x.chars(), x.length(), &y, 1); }
  
  inline void cat(const char* a, const SubStringC& x, const StringC& y, StringC& r)
  { r.rep = Scat(r.rep, a, -1, x.chars(), x.length(), y.chars(), y.length()); }
  
  inline void cat(const char* a, const SubStringC& x, const SubStringC& y, StringC& r)
  { r.rep = Scat(r.rep, a, -1, x.chars(), x.length(), y.chars(), y.length()); }
  
  inline void cat(const char* a, const SubStringC& x, const char* y, StringC& r)
  { r.rep = Scat(r.rep, a, -1, x.chars(), x.length(), y, -1); }
  
  inline void cat(const char* a, const SubStringC& x, char y, StringC& r)
  { r.rep = Scat(r.rep, a, -1, x.chars(), x.length(), &y, 1); }
  
  inline void cat(const char* a, const char* x, const StringC& y, StringC& r)
  { r.rep = Scat(r.rep, a, -1, x, -1, y.chars(), y.length()); }
  
  inline void cat(const char* a, const char* x, const SubStringC& y, StringC& r)
  { r.rep = Scat(r.rep, a, -1, x, -1, y.chars(), y.length()); }
  
  inline void cat(const char* a, const char* x, const char* y, StringC& r)
  { r.rep = Scat(r.rep, a, -1, x, -1, y, -1); }
  
  inline void cat(const char* a, const char* x, char y, StringC& r)
  { r.rep = Scat(r.rep, a, -1, x, -1, &y, 1); }
  
  // operator versions
  
  inline StringC& StringC::operator+=(const StringC& y) {
    cat(*this, y, *this);
    return *this;
  }
  
  inline StringC& StringC::operator+=(const SubStringC& y) {
    cat(*this, y, *this);
    return *this;
  }
  
  inline StringC& StringC::operator+=(const char* y) {
    cat(*this, y, *this);
    return *this;
  }
  
  inline StringC& StringC:: operator+=(char y){
    cat(*this, y, *this);
    return *this;
  }
  
  inline StringC operator+(const StringC& x, const StringC& y) { 
    StringC r;  
    cat(x, y, r);  
    return r; 
  }
  
  inline StringC operator+(const StringC& x, const SubStringC& y) { 
    StringC r; cat(x, y, r); return r; 
  }

  inline StringC operator+(const StringC& x, const char* y)  { 
    StringC r; cat(x, y, r); return r; 
  }

  inline StringC operator+(const StringC& x, char y) { 
    StringC r; cat(x, y, r); return r; 
  }

  inline StringC operator+(const SubStringC& x, const StringC& y) { 
    StringC r; cat(x, y, r); return r; 
  }

  inline StringC operator+(const SubStringC& x, const SubStringC& y)  { 
    StringC r; cat(x, y, r); return r; 
  }
  
  inline StringC operator+(const SubStringC& x, const char* y) { 
    StringC r; cat(x, y, r); return r; 
  }

  inline StringC operator+(const SubStringC& x, char y) { 
    StringC r; cat(x, y, r); return r; 
  }

  inline StringC operator+(const char* x, const StringC& y) { 
    StringC r; cat(x, y, r); return r; 
  }
  
  inline StringC operator+(const char* x, const SubStringC& y) { 
    StringC r; cat(x, y, r); return r; 
  }

  inline StringC operator+(const char x, const StringC& y) { 
    StringC r; cat(x, y, r); return r; 
  }
  
  inline StringC operator+(const char x, const SubStringC& y) { 
    StringC r; cat(x, y, r); return r; 
  }

  inline StringC reverse(const StringC& x) { 
    StringC r; r.rep = Sreverse(x.rep, r.rep); return r; 
  }

  inline StringC upcase(const StringC& x) { 
    StringC r; r.rep = Supcase(x.rep, r.rep); return r; 
  }

  inline StringC downcase(const StringC& x) { 
    StringC r; r.rep = Sdowncase(x.rep, r.rep); return r; 
  }

  inline StringC capitalize(const StringC& x) { 
    StringC r; r.rep = Scapitalize(x.rep, r.rep); return r; 
  }

  inline StringC operator+(const StringC& x, const IndexC& y) { 
    StringC r; cat(x, StringC(y), r); return r; 
  }
  
  inline StringC operator+(const StringC& x, const IntT& y) { 
    StringC r; cat(x, StringC(y), r); return r; 
  }

  inline StringC operator+(const StringC& x, const UIntT& y)  { 
    StringC r; cat(x, StringC(y), r); return r; 
  }
  
  inline StringC operator+(const StringC& x, const RealT& y)  { 
    StringC r; cat(x, StringC(y), r); return r; 
  }
  
  std::ostream& operator<<(std::ostream& s, const StringC& x);
  
  // a zillion comparison operators
  
  inline bool operator==(const StringC& x, const StringC& y) 
  { return x.IsEqual(y);  }
  
  inline bool operator!=(const StringC& x, const StringC& y)
  { return !x.IsEqual(y);  }
  
  inline bool operator>(const StringC& x, const StringC& y)
  { return compare(x, y) > 0;  }
  
  inline bool operator>=(const StringC& x, const StringC& y)
  { return compare(x, y) >= 0; }
  
  inline bool operator<(const StringC& x, const StringC& y)
  { return compare(x, y) < 0;  }
  
  inline bool operator<=(const StringC& x, const StringC& y)
  { return compare(x, y) <= 0;  }
  
  inline bool operator==(const StringC& x, const SubStringC&  y) 
  { return compare(x, y) == 0;  }
  
  inline bool operator!=(const StringC& x, const SubStringC&  y)
  { return compare(x, y) != 0;  }
  
  inline bool operator>(const StringC& x, const SubStringC&  y)      
  { return compare(x, y) > 0;  }
 
  inline bool operator>=(const StringC& x, const SubStringC&  y)
  { return compare(x, y) >= 0;  }
 
  inline bool operator<(const StringC& x, const SubStringC&  y) 
  { return compare(x, y) < 0; }
  
  inline bool operator<=(const StringC& x, const SubStringC&  y)
  { return compare(x, y) <= 0; }
  
  inline bool operator==(const StringC& x, const char* t) 
  { return compare(x, t) == 0; }
  
  inline bool operator!=(const StringC& x, const char* t) 
  { return compare(x, t) != 0; }
  
  inline bool operator>(const StringC& x, const char* t)  
  { return compare(x, t) > 0; }
  
  inline bool operator>=(const StringC& x, const char* t) 
  { return compare(x, t) >= 0; }
  
  inline bool operator<(const StringC& x, const char* t)  
  { return compare(x, t) < 0; }
  
  inline bool operator<=(const StringC& x, const char* t) 
  { return compare(x, t) <= 0; }
  
  inline bool operator==(const SubStringC& x, const StringC& y) 
  { return compare(y, x) == 0; }
  
  inline bool operator!=(const SubStringC& x, const StringC& y)
  { return compare(y, x) != 0; }
  
  inline bool operator>(const SubStringC& x, const StringC& y)      
  { return compare(y, x) < 0; }
  
  inline bool operator>=(const SubStringC& x, const StringC& y)     
  { return compare(y, x) <= 0; }
 
  inline bool operator<(const SubStringC& x, const StringC& y)      
  { return compare(y, x) > 0; }
 
  inline bool operator<=(const SubStringC& x, const StringC& y)     
  { return compare(y, x) >= 0; }
  
  inline bool operator==(const SubStringC& x, const SubStringC&  y) 
  { return compare(x, y) == 0; }
  
  inline bool operator!=(const SubStringC& x, const SubStringC&  y)
  { return compare(x, y) != 0; }
  
  inline bool operator>(const SubStringC& x, const SubStringC&  y)      
  { return compare(x, y) > 0; }
  
  inline bool operator>=(const SubStringC& x, const SubStringC&  y)
  { return compare(x, y) >= 0; }
  
  inline bool operator<(const SubStringC& x, const SubStringC&  y) 
  { return compare(x, y) < 0; }
  
  inline bool operator<=(const SubStringC& x, const SubStringC&  y)
  { return compare(x, y) <= 0; }
  
  inline bool operator==(const SubStringC& x, const char* t) 
  { return compare(x, t) == 0; }
  
  inline bool operator!=(const SubStringC& x, const char* t) 
  { return compare(x, t) != 0; }
  
  inline bool operator>(const SubStringC& x, const char* t)  
  { return compare(x, t) > 0; }
  
  inline bool operator>=(const SubStringC& x, const char* t) 
  { return compare(x, t) >= 0; }
  
  inline bool operator<(const SubStringC& x, const char* t)  
  { return compare(x, t) < 0; }
  
  inline bool operator<=(const SubStringC& x, const char* t) 
  { return compare(x, t) <= 0; }
  
  StringC Indent(int level);
  //: Helper function for producing indented output.
  // create a string with 'level' spaces.
}
#endif
