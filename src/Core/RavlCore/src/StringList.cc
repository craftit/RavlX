// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//////////////////////////////////////////////////////////////
//! lib=RavlCore
//! file="Ravl/Core/System/StringList.cc"

#include <string.h>
#include "Ravl/StringList.hh"
#include "Ravl/CDLIter.hh"

namespace RavlN {
  
  StringListC::StringListC (const StringC &string, bool fullParse,const char* rdelim) {
    if(!fullParse) {
      (*this) = StringListC(string,rdelim);
      return ;
    }
    if(string.length() < 1)
      return ;
    RavlAssert(rdelim != 0);
    bool delim[256];
    for(int i =0;i < 256;i++)
      delim[i] = false;
  //  RavlAssert(!delim[0]); // bool defaults to false, but in case this changes.
    const char *place,*eos;
    // Fill out delim table.
    for(place = rdelim;*place != 0;place++)
      delim[(int) *place] = true;
    // Begin search of string.
    eos = &string.chars()[string.length()];
    for(place = string.chars();place != eos;) {
      // Skip spaces.
      if(delim[(int) *place]) {
	place++;
	continue;
      }
      // Found string.
      StringC work;
      bool inQuote(false);
      for(;place != eos;place++) {
	if(delim[(int) *place] && !inQuote)
	  break;
	switch(*place)
	  {
	  case '"': // a quote.
	    inQuote = !inQuote;
	    break;
	  case '\\': // Escape char ?
	    place++;
	    if(place == eos) {
	      InsLast(work);
	      return ;
	    }
	    
	    switch(*place)
	      {
	      case 'n':
		work += '\n';
		break;
	      case 't':
		work += '\t';
	      break;
	      case 'r':
		work += '\r';
		break;
	      case 0: // Ignore.
		break;
	      default:
		work += *place;
		break;
	      }
	    break;
	  default:
	    work += *place;
	    break;
	  }
      }
      //cerr << "Str: " << len << " = " << lstart << std::endl;
      // At end of string.
      InsLast(work);
    }  
  }
  
  IndexC StringListC::CasePos (const StringC &str) const {
    IndexC pos(0);
    for(ConstDLIterC<StringC> i(*this);i.IsElm();i.Next(),pos++)
      if (i.Data().CaseCmp(str) == 0)
	return pos;
    return -1;
  }
  
  StringC StringListC::Cat (const StringC &sep) const {
    StringC ret;
    ConstDLIterC<StringC> i(*this);
    if(!i.IsElm())
      return ret;
    ret += i.Data();
    i.Next();
    for (; i.IsElm(); i.Next())
      ret += sep + i.Data();
    return ret;
  }
  
  std::ostream & operator<<(std::ostream & s, const StringListC & list) {
    s << list.Cat();
    return s;
  }
  
  //: Parses string into list
  void StringListC::Parse (const StringC &string, const char* rdelim) {
    if(string.length() < 1)
      return ;
    RavlAssert(rdelim != 0);
    bool delim[256];
    for(int i = 0;i < 256;i++)
      delim[i] = false;
    const char *place,*eos,*lstart = 0;
    // Fill out delim table.
    for(place = rdelim;*place != 0;place++)
      delim[(int) *place] = true;
    // Begin search of string.
    eos = &string.chars()[string.length()];
    for(place = string.chars();place != eos;) {
      // Skip spaces.
      if(delim[(int) *place]) {
	place++;
	continue;
      }
      // Found string.
      lstart = place;
      SizeT len = 0;
      for(;place != eos;place++) {
	if(delim[(int) *place])
	  break;
	len++;
      }
      //cerr << "Str: " << len << " = " << lstart << std::endl;
      // At end of string.
      InsLast(StringC(lstart,len,len));
    }

  }





    //: converts a string list into an escaped string
    // "," will be converted to "\," and the string will be "," delimited.
    StringC StringListC::Escape() const
    {
      StringC ret;
      bool firstPass = true;
      for (ConstDLIterC<StringC> iter (*this); iter.IsElm() ; iter.Next() )
      { 
        StringC tmp = iter.Data(); //.Copy(); // copy not needed after RAVL gsub fix.
        tmp.gsub(",", "\\,");
        if (!firstPass)
          ret += ',';
        else
          firstPass = false;

        ret += tmp;
      }
      return ret;
    }



    //: converts and Escaped string back into a string list
    StringListC StringListC::UnEscape(const StringC & str)
    {
      StringListC ret;

      // ignore empty strings.
      if (str.length() < 1)
       return ret ;

      // end of string marker.
      const char * eos = &str.chars()[str.length()];

      StringC work;

      // iterate the string
      const char * place = str.chars();
      for ( ; place != eos ; ++place)
      {
        switch (*place)
        {
          // found an escape character.
          case '\\':
            work += *place;
            // move to next character.
            ++place;
            // don't care what is after an escape.
            work += *place;
            break;

          // found a non-escaped delimiter - so split.
          case ',':
            work.gsub("\\,", ",");
            ret.InsLast(work);
            work = StringC();
            break;

          default:
            work+=*place; 
            break;

        }
      }
      work.gsub("\\,", ",");
      ret.InsLast(work); 
      return ret;
    }


}


