// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
/////////////////////////////////////////////////////////////
//! rcsid="$Id$"
//! lib=RavlOS
//! file="Ravl/OS/FileSystem/FileOwner.cc"

#include "Ravl/OS/Filename.hh"
#include "Ravl/OS/UserInfo.hh"

namespace RavlN {
  
  //: Get information on owner of file.
  
  UserInfoC FilePermissionC::OwnerInfo() const { 
    return UserInfoC(Owner()); 
  }
  
  UserInfoC FilenameC::OwnerInfo() const { 
    return UserInfoC(Permissions().Owner()); 
  }
  
}
