// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2005, OmniPerception Ltd.
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef _RAVL_CPUINFO_HH_
#define	 _RAVL_CPUINFO_HH_ 1
//! author=CPUInfo
//! docentry="Ravl.API.OS.Exec"
//! file=
//! lib=RavlOS
//! userlevel=Normal
//! date="16 September 2009"


namespace RavlN {

  long int GetNumCores(void);
  //: Get the number of cores on this system.
  // Not implemented for windows, will return 1;
}

#endif	/* _RAVL_CPUINFO_HH_ */

