// This file is part of CxxDoc, The RAVL C++ Documentation tool 
// Copyright (C) 2003, University of Surrey
// This code may be redistributed under the terms of the GNU General 
// Public License (GPL). See the gpl.licence file for details or
// see http://www.gnu.org/copyleft/gpl.html
// file-header-ends-here
#ifndef RAVLCXXDOC_EXECUTABLES_HEADER
#define RAVLCXXDOC_EXECUTABLES_HEADER 1
//////////////////////////////////////////////////////////
//! rcsid="$Id$"
//! userlevel=Normal
//! docentry="Ravl.API.Source Tools.CxxDoc.Internal"
//! lib=RavlCxxDoc
//! author="Charles Galambos"
//! date="12/05/2003"
//! file="Ravl/SourceTools/CxxDoc/Executables.hh"

#include "Ravl/CxxDoc/Object.hh"

namespace RavlCxxDocN {

  //! userlevel=Develop
  //: Executable object
  
  class ObjectExeBodyC
    : public ObjectBodyC
  {
  public:
    ObjectExeBodyC()
    {}
    //: Default constructor.

    ObjectExeBodyC(const StringC &name);
    //: Constructor.
    
    virtual const char *TypeName() const 
    { return "exe"; }
    //: Get name of object type.
    
  protected:
  };
  
  //! userlevel=Normal
  //: Executable object.
  
  class ObjectExeC
    : public ObjectC
  {
  public:
    ObjectExeC()
    {}
    //: Default constructor.

    ObjectExeC(const StringC &name)
      : ObjectC(*new ObjectExeBodyC(name))
    {}
    //: Constructor.
    
  };
}

#endif
