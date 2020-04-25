// This file is part of CxxDoc, The RAVL C++ Documentation tool 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU General 
// Public License (GPL). See the gpl.licence file for details or
// see http://www.gnu.org/copyleft/gpl.html
// file-header-ends-here
#ifndef RAVL_BUILDINFO_HEADER
#define RAVL_BUILDINFO_HEADER 1
///////////////////////////////////////////////////
//! rcsid="$Id$"
//! lib=RavlSourceTools
//! author="Charles Galambos"
//! docentry="Ravl.API.Source Tools"
//! file="Ravl/SourceTools/CodeManager/BuildInfo.hh"

#include "Ravl/SourceTools/LibInfo.hh"
#include "Ravl/SourceTools/DefsFile.hh"
#include "Ravl/Hash.hh"
#include "Ravl/HSet.hh"

namespace RavlN {
  
  //! userlevel=Develop
  //: Collects information about library dependancies.
  
  class BuildInfoBodyC 
    : public RCBodyC
  {
  public:
    BuildInfoBodyC();
    //: Default constructor.

    bool ScanDirectory(StringC &where,DefsMkFileC &defs);
    //: Add information from directory to database.
    
    DListC<StringC> LibDepends(StringC &lib);
    //: List libraries the named library depends on.
    
    DListC<StringC> Depends(const DListC<StringC> &uses);
    //: Given a list of libraries used directly, expand it to a full list.
    
  protected:
    bool LibDepends(StringC &lib,DListC<StringC> &list,HSetC<StringC> &done);
    //: List libraries the named library depends on.
    
    HashC<StringC,LibInfoC> libs; // Library lookup.
    
  };

  //! userlevel=Normal
  //: Collects information about library dependancies.
  
  class BuildInfoC 
    : public RCHandleC<BuildInfoBodyC>
  {
  public:
    BuildInfoC()
      : RCHandleC<BuildInfoBodyC>(*new BuildInfoBodyC())
    {}
    //: Default constructor.
    
    bool ScanDirectory(StringC &where,DefsMkFileC &defs)
    { return Body().ScanDirectory(where,defs); }
    //: Add information from directory to database.
    
    DListC<StringC> LibDepends(StringC &lib) 
    { return Body().LibDepends(lib); }
    //: List libraries this depends on.
    
    DListC<StringC> Depends(const DListC<StringC> &uses)
    { return Body().Depends(uses); }
    //: Given a list of libraries used directly, expand it to a full list.
    
  };
  
}

#endif
