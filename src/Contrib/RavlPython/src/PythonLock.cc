// This file is part of RAVL, Recognition And Vision Library
// Copyright (C) 2008, OmniPerception Ltd.
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// Python is used under the terms of the Python License
// Copyright (C) 2001, 2002, 2003, 2004 Python Software Foundation; All Rights Reserved
// file-header-ends-here
////////////////////////////////////////////////
//! file = "Ravl/Contrib/Python/PythonLock.cc"
//! lib=RavlPython
//! author = "Warren Moore"

#include "Ravl/PythonLock.hh"
#include "Ravl/Assert.hh"
#include <iostream>

#define DODEBUG 0
#if DODEBUG
#define ONDEBUG(x) x
#else
#define ONDEBUG(x)
#endif

namespace RavlN
{
  
	PythonLockC::PythonLockC(PyThreadState *threadState)
	{
    RavlAssert(threadState != NULL);
    ONDEBUG(std::cerr << "PythonLockC::PythonLockC(" << threadState << ")" << std::endl);
    PyEval_AcquireLock();
    PyThreadState_Swap(threadState);
	}

  PythonLockC::~PythonLockC()
  {
    ONDEBUG(std::cerr << "PythonLockC::~PythonLockC(" << PyThreadState_Get() << ")" << std::endl);
    PyThreadState_Swap(NULL);
    PyEval_ReleaseLock();
  }
  
}
