// This file is part of CxxDoc, The RAVL C++ Documentation tool 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU General 
// Public License (GPL). See the gpl.licence file for details or
// see http://www.gnu.org/copyleft/gpl.html
// file-header-ends-here
#ifndef RAVL_PROGINFO_HEADER
#define RAVL_PROGINFO_HEADER 1
////////////////////////////////////////////////
//! rcsid="$Id$"
//! docentry="Ravl.API.Source Tools"
//! lib=RavlSourceTools
//! userlevel=Normal
//! file="Ravl/SourceTools/CodeManager/ProgInfo.hh"

#include "Ravl/RefCounter.hh"
#include "Ravl/String.hh"
#include "Ravl/DList.hh"

namespace RavlN {

  class DefsMkFileC;
  
  //! userlevel=Develop
  //: Information about an executable.
  
  class ProgInfoBodyC
    : public RCBodyC
  {
  public:
    ProgInfoBodyC(const StringC &progName,DefsMkFileC &defs,const StringC &fromDir);
    //: Constructor.
    
    bool Add(DefsMkFileC &defs,const StringC &fromDir);
    //: Add information from defs file 'defs'.
    // True is returned if operation succeeded.
    
    const StringC &Name() const
    { return progName; }
    //: Get program name.
    
    DListC<StringC> &UsesLibs()
    { return useslibs; }
    //: Libraries required by this one.
    
    DListC<StringC> Sources()
    { return sources; }
    //: Source files in the program.
    
    const StringC &MainSource() const
    { return progSource; }
    //: Get name of source file.
    
  protected:
    StringC progName; // Unqualitified program name, (Has code extention though.)
    StringC progSource;
    DListC<StringC> useslibs;// Libraries required by this one.
    DListC<StringC> sources;  // Source files in the program (fully qualified names).
  };
  
  //! userlevel=Normal
  //: Information about an executable.
  
  class ProgInfoC
    : public RCHandleC<ProgInfoBodyC>
  {
  public:
    ProgInfoC()
    {}
    //: Default constructor,
    // Creates an invalid handle.
    
    ProgInfoC(const StringC &programName,DefsMkFileC &defs,const StringC &fromDir)
      : RCHandleC<ProgInfoBodyC>(*new ProgInfoBodyC(programName,defs,fromDir))
    {}
    //: Constructor.
    
    bool Add(DefsMkFileC &defs,const StringC &fromDir)
    { return Body().Add(defs,fromDir); }
    //: Add information from defs file 'defs'.
    // True is returned if operation succeeded.
    
    const StringC &Name() const
    { return Body().Name(); }
    //: Get program name.
    
    DListC<StringC> &UsesLibs()
    { return Body().UsesLibs(); }
    //: Libraries required by this one.
    
    DListC<StringC> Sources()
    { return Body().Sources(); }
    //: Source files in the program.
    
    const StringC &MainSource() const
    { return Body().MainSource(); }
    //: Get name of source file.
    
  };

}

#endif
