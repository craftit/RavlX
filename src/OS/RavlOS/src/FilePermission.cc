// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//////////////////////////////////////////////////////////////
//! rcsid="$Id$"
//! lib=RavlOS
//! file="Ravl/OS/FileSystem/FilePermission.cc"

#include "Ravl/OS/FilePermission.hh"

#if !RAVL_COMPILER_VISUALCPP
#include <unistd.h>
#endif
#include <stdio.h>

#ifndef NGROUPS_UMAX
#define NGROUPS_UMAX 256
#endif

namespace RavlN {
  ////////////////////////////////////////////
  // Do we have read permission ?
  
  bool FilePermissionC::IsReadable() const {
#if !RAVL_COMPILER_VISUALCPP
    if((Mode() & S_IROTH) != 0)
      return true;
    uid_t me = getuid();
    if(me == 0)
      return true; // Root has access to everything.
    if((Mode() & S_IRUSR) != 0 && me == Owner())
      return true;
    if((Mode() & S_IRGRP) != 0) {  
      gid_t GrpArr[NGROUPS_UMAX];
      int ngrps = getgroups(NGROUPS_UMAX,GrpArr);
      if(ngrps < 0)
	perror("FilePermissionC::IsReadable() Unexpected error ");
      else {
	for(int i = 0;i < ngrps;i++) {
	if(GrpArr[i] == Group())
	  return true;
	}
      }
    }
    return false;
#else
    return true;
#endif
  }
  
  ////////////////////////////////////////////
  // Do we have read permission for this process ?
  
  bool FilePermissionC::IsWritable() const {
#if !RAVL_COMPILER_VISUALCPP
    if((Mode() & S_IWOTH) != 0)
      return true;
    uid_t me = getuid();
    if(me == 0)
      return true; // Root has access to everything.
    if((Mode() & S_IWUSR) != 0 && me == Owner())
      return true;
    if((Mode() & S_IWGRP) != 0) {  
      gid_t GrpArr[NGROUPS_UMAX];
      int ngrps = getgroups(NGROUPS_UMAX,GrpArr);
      if(ngrps < 0)
	perror("FilePermissionC::IsWritable() Unexpected error ");
      else {
      for(int i = 0;i < ngrps;i++) {
	if(GrpArr[i] == Group())
	  return true;
      }
      }
    }
    return false;
#else
    return true;
#endif
  }
  
  ////////////////////////////////////////////
  //: Does owner have write access ?
  
  bool FilePermissionC::IsOwnerWritable() const {
#if !RAVL_COMPILER_VISUALCPP
    if((Mode() & S_IWUSR) != 0)
      return true;
    return false;
#else
    return true;
#endif
  }

  ////////////////////////////////////////////
  //: Does world have write access ?
  
  bool FilePermissionC::IsWorldWritable() const {
#if !RAVL_COMPILER_VISUALCPP
    if((Mode() & S_IWOTH) != 0)
      return true;
    return false;
#else
    return true;
#endif  
  }
  
  ////////////////////////////////////////////
  // Set the process mask to this mode.
  
  FilePermissionC FilePermissionC::SetUMask() const {
#if !RAVL_COMPILER_VISUALCPP
    return FilePermissionC(umask(Mode()));
#else
    return FilePermissionC();
#endif
  }

}
