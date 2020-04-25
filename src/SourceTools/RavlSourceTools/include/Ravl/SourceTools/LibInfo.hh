// This file is part of CxxDoc, The RAVL C++ Documentation tool 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU General 
// Public License (GPL). See the gpl.licence file for details or
// see http://www.gnu.org/copyleft/gpl.html
// file-header-ends-here
#ifndef RAVL_LIBINFO_HEADER
#define RAVL_LIBINFO_HEADER 1
////////////////////////////////////////////////////////////////
//! rcsid="$Id$"
//! userlevel=Normal
//! docentry="Ravl.API.Source Tools"
//! lib=RavlSourceTools
//! author="Charles Galambos"
//! file="Ravl/SourceTools/CodeManager/LibInfo.hh"

#include "Ravl/RefCounter.hh"
#include "Ravl/String.hh"
#include "Ravl/DList.hh"

namespace RavlN {

  class DefsMkFileC;

  //! userlevel=Develop
  //: Header info. 
  // All you wanted to know about header files, but where
  // afraid to ask.
  
  class HeaderInfoC {
  public:
    HeaderInfoC(const StringC &headerName,const StringC &nPackage = StringC(),const StringC &sourceFile = StringC())
      : name(headerName),
	package(nPackage),
	src(sourceFile)
    {}
    //: Constructor.
    
    StringC &Name()
    { return name; }
    //: Name of headerfile (without path.)
    
    const StringC &Name() const
    { return name; }
    //: Name of headerfile (without path.)
    
    StringC &Package()
    { return package; }
    //: Where is should be installed.
    
    StringC &SrcFile()
    { return src; }
    //: Where it is in the source tree.
    
  protected:
    StringC name;    // Name of headerfile (without path.)
    StringC package; // Where is should be installed.
    StringC src;     // Where it is in the source tree.
  };
  
  //! userlevel=Develop
  //: Information about library.
  
  class LibInfoBodyC
    : public RCBodyC
  {
  public:
    LibInfoBodyC(const StringC &ln,bool isDummy = false)
      : dummy(isDummy),
	libName(ln.Copy())
    {}
    //: Constructor.
    
    bool Add(DefsMkFileC &defs,const StringC &fromDir);
    //: Add information from defs file 'defs'.
    // True is returned if operation succeeded.
    
    const StringC &Name() const
    { return libName; }
    //: Get library name.
    
    DListC<StringC> &UsesLibs()
    { return useslibs; }
    //: Libraries required by this one.
    
    DListC<StringC> Sources()
    { return sources; }
    //: Source files in the library.
    
    DListC<HeaderInfoC> Headers()
    { return headers; }
    //: Headers for the library.
    
    DListC<StringC> &AuxFiles()
    { return auxfiles; }
    //: List of aux files.
    
    StringC &AuxDir()
    { return auxdir; }
    //: Aux directory.
    
    bool IsDummy() const
    { return dummy; }
    //: Is this a dummy library ?
    
    const StringC &MustLink() const
    { return mustLink; }
    //: Required object file.
    // If none this is an empty string.
    
  protected:
    bool dummy; // Is this a dummy library ?
    StringC libName;
    DListC<StringC> useslibs;// Libraries required by this one.
    DListC<StringC> sources;  // Source files in the library.
    DListC<StringC> auxfiles;  // Source files in the library.
    DListC<HeaderInfoC > headers; // Headers for the library.
    StringC auxdir;
    StringC mustLink; // Is there an object file required for linking ?
  };

  //! userlevel=Normal
  //: Information about library.
  
  class LibInfoC
    : public RCHandleC<LibInfoBodyC>
  {
  public:
    LibInfoC()
    {}
    //: Default constructor,
    // creates an invalid handle.
    
    LibInfoC(const StringC &libraryName,bool isDummy = false)
      : RCHandleC<LibInfoBodyC>(*new LibInfoBodyC(libraryName,isDummy))
    {}
    //: Constructor.
    
    bool Add(DefsMkFileC &defs,const StringC &fromDir)
    { return Body().Add(defs,fromDir); }
    //: Add information from defs file 'defs'.
    // True is returned if operation succeeded.
    
    const StringC &Name() const
    { return Body().Name(); }
    //: Get library name.
    
    DListC<StringC> &UsesLibs()
    { return Body().UsesLibs(); }
    //: Libraries directly required by this one.
    
    DListC<StringC> Sources()
    { return Body().Sources(); }
    //: Source files in the library.
    
    DListC<HeaderInfoC> Headers()
    { return Body().Headers(); }
    //: Headers for the library.

    DListC<StringC> &AuxFiles()
    { return Body().AuxFiles(); }
    //: List of aux files.
    
    StringC &AuxDir()
    { return Body().AuxDir(); }
    //: Aux directory.

    bool IsDummy() const
    { return Body().IsDummy(); }
    //: Is this a dummy library ?
    
    const StringC &MustLink() const
    { return Body().MustLink(); }
    //: Required object file.
    // If none this is an empty string.
    
  };
  
}

#endif
