// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2003, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//////////////////////////////////////////////////////////////
//! author="Charles Galambos"
//! rcsid="$Id$"
//! lib=RavlOS
//! file="Ravl/OS/Exec/FileUser.cc"

#define RAVL_USE_PAWD 0
#include "Ravl/config.h"

#if RAVL_COMPILER_VISUALCPP
// Turn off deprecated warnings for now, they're not deprecated on other platforms
// may introduce more platform specific fixes later.
#define _CRT_SECURE_NO_DEPRECATE 1
#endif

#include "Ravl/OS/FilePermission.hh"
#include "Ravl/OS/UserInfo.hh"
#include "Ravl/OS/Filename.hh"
#include "Ravl/StrStream.hh"
#if RAVL_USE_PAWD
#include "Ravl/OS/ChildOSProcess.hh"
#endif

#if !RAVL_COMPILER_VISUALCPP
#include <unistd.h>
#else
#include <direct.h>
#endif

namespace RavlN {
#if RAVL_USE_PAWD
  static bool gotPAWD = true; // Try and use PAWD, if false don't bother.
#endif
  
  static StringC GetCwd() {
    char buff[2048];
#if RAVL_COMPILER_VISUALCPPNET_2005
    return StringC(_getcwd(buff,2048));
#else
	return StringC(getcwd(buff,2048));
#endif
  }
  
  /////////////////////////////////
  //: Make an absolute path.
  // If removeAMD is true, any automount prefix will also
  // be remove from the path.

  FilenameC FilenameC::Absolute(bool removeAMD) const 
  {
    FilenameC ret = (*this);
    if(ret.IsEmpty())
      ret = GetCwd();

    char currentDir[2];
    currentDir[0] = '.';
    currentDir[1] = RavlN::filenameSeperator;

    while(ret[0] != RavlN::filenameSeperator) {
      if(ret == StringC(".")) {
        ret = GetCwd();
	break;
      }

      if(ret.matches(currentDir,0,false)) {
        ret = GetCwd();
        break;
      }
      if(ret[0] == '~') { // Relative to home directory ?
	StringC tmp = ret.after(1);
	StringC userId = tmp.before(RavlN::filenameSeperator);
	StringC theRest = tmp.after(RavlN::filenameSeperator);
	if(userId.IsEmpty())
	  userId = tmp; // Maybe only user is defined.
	UserInfoC ui(userId);
	ret = ui.HomeDir() + RavlN::filenameSeperator + theRest;
	break;
      }
      ret = GetCwd() + RavlN::filenameSeperator + ret;
      break;
    };
  
    if(!removeAMD)
      return ret; // Done.
    if(!ret.matches(AMDPrefix,0,false))
      return ret; // No AMD prefix to remove.
  
    // Removee the AMD prefix. (if possible.)
    FilenameC org(ret);
    bool orgExists = org.PathComponent().Exists(); // Check directory exists...
    ret = StringC(ret.after(AMDPrefix)); // Skip AMD prefix.
    ret = StringC(ret.from(RavlN::filenameSeperator)); // Skip matchine name.
    FilenameC newun(ret);
    // Is everything ok ?
    if(orgExists) { // Only if we can tell...
      if(!newun.PathComponent().Exists()) {
#if RAVL_USE_PAWD
	if(!gotPAWD)
	  return org; // Just go with original...
	// Try pawd.
	ChildOSProcessC pawd(StringC("pawd ") + org,true);
	if(!pawd.IsRunning()) {
	  gotPAWD = false;
	  cerr << "WARNING: Failed to run 'pawd'. \n";
	  return org; 
	}
	// And hope 'pawd' doesn't hang...
	StrOStreamC strStrm;
	if(pawd.StdOut().CopyTo(strStrm,1000) <= 0) {
	  cerr << "WARNING: Error reading results from 'pawd'. \n";
	  return org;
	}     
	if(!pawd.Wait(5)) {
	  gotPAWD = false;
	  cerr << "WARNING: Timeout waiting for 'pawd'. \n";
	  pawd.Terminate(1);
	  return org; 
	}
	// Give result of out investigations to user.
	return strStrm.String();
#else
	return org;
#endif
      }
    }
    return ret;
  }
}
