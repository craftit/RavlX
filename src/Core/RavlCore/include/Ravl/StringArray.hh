// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVL_STRINGARRAY_HEADER
#define RAVL_STRINGARRAY_HEADER
////////////////////////////////////////////////////////////////////////
//! file="Ravl/Core/System/StringArray.hh"
//! lib=RavlCore
//! userlevel=Normal
//! author="Bill Christmas"
//! docentry="Ravl.API.Core.Lists;Ravl.API.Core.Strings"
//! date="12/07/1996"

#include "Ravl/String.hh"
#include "Ravl/Collection.hh"
 
namespace RavlN {
  
  //! userlevel=Normal
  //: List of StringC's
  
  class StringArrayC
    : public CollectionC<StringC>
  {
  public:
    inline StringArrayC(UIntT maxSize = 16)
     : CollectionC<StringC>(maxSize)
    {}
    //: Constructor
    // The optional argument reserves space in the array to make reading quicker.
    
    inline StringArrayC (const StringArrayC &oth)
      : CollectionC<StringC>(oth)
    {}
    //: Copy constructor

    inline StringArrayC (const CollectionC<StringC> &alist)
      : CollectionC<StringC>(alist)
    {}
    //: Constructor

    static StringArrayC Split(const StringC &str, const char* delim = " \n\t\0",bool allowEmptyColumn = false);
    //: Split a string into parts.
    // When parsing csv files where adjacent comma's represent an empty field 'allowEmptyColumn' should be set
    // to true.

    static StringArrayC Split(const char* str, const char* delim = " \n\t\0",bool allowEmptyColumn = false);
    //: Split a string into parts.
    // When parsing csv files where adjacent comma's represent an empty field 'allowEmptyColumn' should be set
    // to true.

    static StringArrayC SplitQuote(const StringC &str, const char* delim = " \n\t\0",bool allowEmptyColumn = false);
    //: Split a string into parts, respecting any double quotes found along the way.
    // Escaping quotes with '\' allows them to be treated as any other character.

    static StringArrayC UnEscape(const StringC & str);
    //: converts and Escaped string back into a string list

    IndexC Find(const StringC &str) const;
    //: finds position of string in list
    // returns -1 if string is not found

    IndexC CaseFind(const StringC &str) const;
    //: finds position of string in list - case insensitive
    // returns -1 if string is not found
    
    StringC Cat (const StringC &sep = " ") const;
    //: concatenates string list into a single string
    
    StringC Escape() const;
    //: converts a string list into an escaped string
    // "," will be converted to "\," and the string will be "," delimited.


  private:
    void Parse (const StringC &str, const char* delim,bool allowEmptyColumn = false);
    //: Parses string into list
    // When parsing csv files where adjacent comma's represent an empty field 'allowEmptyColumn' should be set
    // to true.

    friend std::ostream & operator<<(std::ostream & s, const StringArrayC & list);
  };
  
  std::ostream & operator<<(std::ostream & s, const StringArrayC & list);
  // Saves the string list into the output stream 's'.
  
}
  
#endif
