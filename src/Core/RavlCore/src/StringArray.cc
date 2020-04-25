// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//////////////////////////////////////////////////////////////
//! lib=RavlCore
//! file="Ravl/Core/System/StringArray.cc"

#include <string.h>
#include "Ravl/StringArray.hh"
#include "Ravl/SArray1dIter.hh"

namespace RavlN {
  
  StringArrayC StringArrayC::Split(const StringC &str, const char* delim,bool allowEmptyColumn)
  {
    // Assume the first delimiter is most likely...
    UIntT num = str.freq(delim[0])+1;
    if(num < 4) num = 4;
    StringArrayC ret(num);
    ret.Parse(str, delim,allowEmptyColumn);
    return ret;
  }
  //: Split a string into parts.

  StringArrayC StringArrayC::Split(const char* str, const char* delim,bool allowEmptyColumn)
  {
    StringC tehStr(str);
    return StringArrayC::Split(tehStr,delim,allowEmptyColumn);
  }
  //: Split a string into parts.

  StringArrayC StringArrayC::SplitQuote(const StringC &text, const char* rdelim,bool allowEmptyColumn)
  {
    StringArrayC ret;
    if(text.length() < 1)
      return StringArrayC();
    RavlAssert(rdelim != 0);
    bool delim[256];
    for(int i =0;i < 256;i++)
      delim[i] = false;
  //  RavlAssert(!delim[0]); // bool defaults to false, but in case this changes.
    const char *place,*eos;
    // Fill out delim table.
    for(place = rdelim;*place != 0;place++)
      delim[((unsigned char)*place)] = true;
    // Begin search of string.
    eos = &text.chars()[text.length()];
    for(place = text.chars();place != eos;) {
      // Skip spaces.
      if(delim[((unsigned char)*place)]) {
        place++;
        if(!allowEmptyColumn)
          continue;
      }
      // Found string.
      StringC work;
      bool inQuote(false);
      for(;place != eos;place++) {
        if(delim[(int) ((unsigned char)*place)] && !inQuote)
          break;
        switch(*place)
          {
          case '"': // a quote.
            inQuote = !inQuote;
            break;
          case '\\': // Escape char ?
            place++;
            if(place == eos) {
              ret.Append(work);
              return ret;
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
      ret.Append(work);
    }  
    return ret;
  }
  
  IndexC StringArrayC::CaseFind(const StringC &str) const {
    IndexC pos(0);
    for(SArray1dIterC<StringC> i(Array());i.IsElm();i.Next(),pos++)
      if (i.Data().CaseCmp(str) == 0)
	return pos;
    return -1;
  }
  
  //: finds position of string in list
  // returns -1 if string is not found

  IndexC StringArrayC::Find(const StringC &str) const {
    IndexC pos(0);
    for(SArray1dIterC<StringC> i(Array());i.IsElm();i.Next(),pos++)
      if (i.Data() == str)
        return pos;
    return -1;
  }

  StringC StringArrayC::Cat (const StringC &sep) const {
    StringC ret;
    SArray1dIterC<StringC> i(Array());
    if(!i.IsElm())
      return ret;
    ret += i.Data();
    i.Next();
    for (; i.IsElm(); i.Next())
      ret += sep + i.Data();
    return ret;
  }
  
  std::ostream & operator<<(std::ostream & s, const StringArrayC & list) {
    s << list.Cat();
    return s;
  }
  
  //: Parses string into list
  void StringArrayC::Parse (const StringC &text, const char* rdelim,bool allowEmptyColumn) {
    if(text.length() < 1)
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
    eos = &text.chars()[text.length()];
    for(place = text.chars();place != eos;) {
      // Skip spaces.
      if(delim[(int) *place]) {
	place++;
	if(!allowEmptyColumn)
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
      Append(StringC(lstart,len,len));
    }

  }

  //: converts a string list into an escaped string
  // "," will be converted to "\," and the string will be "," delimited.
  StringC StringArrayC::Escape() const
  {
    StringC ret;
    bool firstPass = true;
    for (SArray1dIterC<StringC> iter (Array()); iter.IsElm() ; iter.Next() )
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
  StringArrayC StringArrayC::UnEscape(const StringC & str)
  {
    StringArrayC ret;

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
          ret.Append(work);
          work = StringC();
          break;

        default:
          work+=*place;
          break;

      }
    }
    work.gsub("\\,", ",");
    ret.Append(work);
    return ret;
  }


}


