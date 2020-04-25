// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
////////////////////////////////////////////////////
//! rcsid="$Id$"
//! lib=RavlCore
//! file="Ravl/Core/System/StreamParse.cc"

#include "Ravl/Stream.hh"
#include "Ravl/StreamType.hh"
#include "Ravl/FixedQueue.hh"

#include <ctype.h>

#define DODEBUG 0

#if DODEBUG
#define ONDEBUG(x) x
#else
#define ONDEBUG(x)
#endif

namespace RavlN {

  char IStreamC::GetChar() {
    char c;
    read(&c,1);
    if(is().gcount() != 1)
      throw ExceptionEndOfStreamC("End of stream found in 'IStreamC::GetChar()' ");
    return c;
  }

  //: Skip white space characters.
  // returns the first non-white space character found.
  
  char IStreamC::SkipWhiteSpace() {
    char c;
    for(c = GetChar();*this && isspace(c);c = GetChar()) 
      ;
    return c;
  }

  //: Skip all 'delim' characters.
  
  bool IStreamC::Skip(const char *delim) { 
    char c;
    for(;;) {
      if(!*this)
	return false;
      c = GetChar();
      const char *d = delim;
      for(;*d != 0 && (*d != c);d++) ;
      if(*d == 0) break;
    }
    Unget(&c,1);
    return true;
  }

  //: Skip to after word if it matches the characters in the stream.
  //: otherwise return false and leave stream where it is.

  bool IStreamC::SkipWord(const char *word) {
    RavlAssert(word != 0);
    const char *at = word;
    StringC buff;
    while(*at != 0) {
      char c = GetChar();
      buff += c; // Build up buffer of whats been read.
      if(*at != c) {
        Unget(buff,buff.size());
        return false;
      }
      at++;
    }
    return true;
  }

  //: Skip through stream until endStr is found.
  
  bool IStreamC::SkipTo(const StringC &endStr) {
    char c;
    RavlAssert(endStr.length() != 0);
    if(endStr.length() == 1)
      return SkipTo(endStr.firstchar());
    FixedQueueC<char> fq(endStr.Size());
    // Fill queue, can't match until the queue is filled.
    while(fq.Size() != endStr.Size()) {
      read(&c,1);
      if(is().gcount() != 1)
	break;
      fq.ForceInsLast(c);
    }
    FixedQueueIterC<char> it;
    const char *at;
    while(1) {
      at = &endStr[0];
      for(it = fq;it;it++,at++) {
	//ONDEBUG(std::cerr << "Str cmp '" << *it << "' == '" << *at << "'\n");
	if(*it != *at)
	  break;
      }
      if(!it.IsElm())
	return true; // Found!
      read(&c,1);
      if(is().gcount() != 1)
	break;
      //ONDEBUG(std::cerr << "Nope! got '" << c <<"'\n");
      fq.ForceInsLast(c);
    }
    return false;
  }

  //: Skip through stream until 'let' is found.
  
  bool IStreamC::SkipTo(char let) {
    char c;
    while(*this) {
      read(&c,1);
      if(is().gcount() != 1)
	break;
      if(let == c)
	return true;
    }
    return false;
  }

  //: Return all characters before let.
  
  StringC IStreamC::ClipTo(char let) {
    StringC ret;
    char c;
    while(*this) {
      read(&c,1);
      if(is().gcount() != 1)
	break;
      if(let == c)
	return ret;
      ret += c;
    }
    return ret;
  }

  //: Return all characters before 'endStr'.
  // this leaves the stream positioned at the
  // first character after the string. 
  
  StringC IStreamC::ClipTo(const StringC &endStr) {
    StringC ret;
    char c;
    RavlAssert(endStr.length() != 0);
    if(endStr.length() == 1)
      return ClipTo(endStr.firstchar());
    FixedQueueC<char> fq(endStr.Size());
    // Fill queue, can't match until the queue is filled.
    while(fq.Size() != endStr.Size()) {
      read(&c,1);
      if(is().gcount() != 1)
	break;
      fq.ForceInsLast(c);
      ret += c;
    }
    FixedQueueIterC<char> it;
    const char *at;
    while(1) {
      at = &endStr[0];
      for(it = fq;it;it++,at++) {
	ONDEBUG(std::cerr << "Str cmp '" << *it << "' == '" << *at << "'\n");
	if(*it != *at)
	  break;
      }
      if(!it.IsElm()) {
        ONDEBUG(std::cerr << "Return '" << ret <<"'\n");
	ret = ret.before((int) ret.length() - (int) endStr.length()); // Clip out 'endStr'.
	return ret; // Found!
      }
      read(&c,1);
      if(is().gcount() != 1)
	break;
      ONDEBUG(std::cerr << "Nope! got '" << c <<"' (" << ret << ")\n");
      fq.ForceInsLast(c);
      ret += c;
    }
    ONDEBUG(std::cerr << "Return '" << ret <<"'\n");
    
    return ret;    
  }

  //: Clip word until one of 'delim' characters are found.
  
  StringC IStreamC::ClipWord(const char *delim,bool initalSkipDelim) {
    StringC ret;
    if(initalSkipDelim)
      if(!Skip(delim))
	return ret;
    char c;
    
    for(;;) {
      if(!*this)
	return ret;
      read(&c,1);
      if(is().gcount() != 1)
	return ret;
      const char *d = delim;
      for(;*d != 0 && (*d != c);d++) ;
      if(*d != 0)
	break;
      ret += c;
    }
    Unget(&c,1);
    return ret;
  }

  //: Clip a number, digits 0-9 '.' '-' '+' and 'e'

  StringC IStreamC::ClipNumber()
  {
    StringC ret;
    char c;
    for(;;) {
      if(!*this)
        return ret;
      read(&c,1);
      if(is().gcount() != 1)
        return ret;
      if(!isdigit(c) && c != '-' && c != 'e' && c != '.' && c != '+')
        break;
      ret += c;
    }
    Unget(&c,1);
    return ret;
  }

}
