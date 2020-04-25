// This file is part of CxxDoc, The RAVL C++ Documentation tool 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU General 
// Public License (GPL). See the gpl.licence file for details or
// see http://www.gnu.org/copyleft/gpl.html
// file-header-ends-here
#ifndef RAVLDEFSFILE_HEADER
#define RAVLDEFSFILE_HEADER 1
///////////////////////////////////////////////////////////////////////
//! file="Ravl/SourceTools/CodeManager/DefsFile.hh"
//! author="Charles Galambos"
//! lib=RavlSourceTools
//! rcsid="$Id$"
//! date="14/05/98"
//! docentry="Ravl.API.Source Tools"

#include "Ravl/Hash.hh"
#include "Ravl/StringList.hh"

namespace RavlN {
  
  class TextCursorC;
  class TextFragmentC;
  class TextFileC;
  
  //! userlevel=Normal
  //: Interograte a defs.mk file.
  
  class DefsMkFileC {
  public:
    DefsMkFileC() {}
  //: Default constructor.
    
    DefsMkFileC(const StringC &fn,bool doCheck = true);
    //: Filename.
    
    bool Load(const StringC &fn,bool doCheck = true);
    //: Load a def file.
    
    bool Load(TextFileC &buf,bool doCheck,HashC<StringC,TextFragmentC> &frags);
    //: Parse a def file from 'buf'.
    // This generates a table of fragements for each variable.
    
    bool CheckTag(StringC tag);
    //: Check if a tag is valid.
    
    StringC Value(const StringC &tag) { return tab[tag]; }
    //: Get value of tag.
    
    StringC &operator[](const StringC &tag) { return tab[tag]; }
    //:  Get value of tag.
    
    bool IsDefined(StringC tag) { return !tab[tag].IsEmpty(); }
    //: Is tag defined ?
    
    bool IsValid() const { return true; }
    //: Has def file been loaded ok ?
    
    StringListC AllSources();
    //: Make a list of all source files that should exists.
    
    StringListC AllNested(); 
    //: Make a list of nested directories.
    // This returns both active and inactive directories.
    // the .r's are striped from all strings.
    
    StringListC Nested(); 
    //: Make a list of nested directories.
    // This returns only active directories.
    // the .r's are striped from all strings.

    inline const StringC & Name(void) { return defsFilename ; } 
    //: Access the name of the defs file. 
    
  protected:
    StringC defsFilename;
    HashC<StringC,StringC> tab;
  };
  
}

#endif
