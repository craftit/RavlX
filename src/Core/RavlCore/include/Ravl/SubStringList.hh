// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVL_SUBSTRINGLIST_HEADER
#define RAVL_SUBSTRINGLIST_HEADER
////////////////////////////////////////////////////////////////////////
//! file="Ravl/Core/System/SubStringList.hh"
//! lib=RavlCore
//! userlevel=Normal
//! author="Bill Christmas"
//! docentry="Ravl.API.Core.Lists;Ravl.API.Core.Strings"
//! rcsid="$Id$"
//! date="12/07/1996"

#include "Ravl/String.hh"
#include "Ravl/DList.hh"
#include "Ravl/Types.hh"
 
namespace RavlN {
  
  //: List of SubStringC's
  
  class SubStringListC 
    : public DListC<SubStringC> 
  {
  public:
    inline SubStringListC()
      {}
    //: Default constructor
    
    inline SubStringListC (const SubStringListC &oth)
      : DListC<SubStringC>(oth)
      {}
    //: Copy constructor
    
    inline SubStringListC (const DListC<SubStringC> &alist)
      : DListC<SubStringC>(alist)
      {}
    //: Constructor
    
    SubStringListC (const SubStringC &str, const char* delim = " \n\t\0");
    //: Constructor that tokenises string into string list
    
    inline const SubStringListC& operator+=(const SubStringListC & list) {
      DListC<SubStringC>::operator+=(list);
      return *this;
    }
    //: Concatenation.
    // The operator moves the whole 'list to the end
    // of this list. The 'list' will be EMPTY after the operation. <p>
    // The arg is only constant so that 'list' may be a tempory without
    // causing warnings. <p> 
    // Returns this list.
    
    StringC Cat (const char *sep = " ") const;
    //: concatenates string list into a single string
    
  private:
    
    friend std::ostream & operator<<(std::ostream & s, const SubStringListC & list);
  };
  
  std::ostream & operator<<(std::ostream & s, const SubStringListC & list);
  // Saves the string list into the output stream 's'.
  
  
}
  
#endif
