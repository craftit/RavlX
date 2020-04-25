// This file is part of RAVL, Recognition And Vision Library
// Copyright (C) 2005-12, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//! lib=RavlOS



#include "Ravl/OS/CPUInfo.hh"
#include "Ravl/Assert.hh"

#include <stdio.h>

#if !RAVL_OS_WIN32
#include <unistd.h>
#endif
#include <stdlib.h>

#if !defined(_SC_NPROCESSORS_ONLN) && !defined(_SC_NPROC_ONLN) && !RAVL_OS_WIN32
#if RAVL_OS_MACOSX
#define _DARWIN_C_SOURCE 1
#endif
#include <sys/types.h>
#include <sys/sysctl.h>
#endif

#define DODEBUG 0
#if DODEBUG
  #define ONDEBUG(x) x
#else
  #define ONDEBUG(x)
#endif

namespace RavlN {

  //: Get the number of cores
  long int GetNumCores(void)
  {
  // windows
#if RAVL_OS_WIN32
    RavlIssueWarning("Not implemented for win32, will default to 1 core");
    return 1;
#endif

#if !RAVL_OS_WIN32
    long int val;
#ifdef _SC_NPROCESSORS_ONLN
    val = sysconf(_SC_NPROCESSORS_ONLN);
#elif defined(_SC_NPROC_ONLN)
    val = sysconf(_SC_NPROC_ONLN);  /* One Porcessor default */
#else
	  {
	    // The follow works on MACOSX
	    size_t len = sizeof(int);
	    int ncpu = 1;
	    if(sysctlbyname("hw.ncpu",&ncpu,&len,NULL,0) == 0) {
	      val = ncpu;
	    } else
	      val = 1;  /* One Processor default */
	  }
#endif //_SC_NPOCESSORS_ONLN
    return val; 
#endif //!RAVL_OS_WIN32
  }
};
