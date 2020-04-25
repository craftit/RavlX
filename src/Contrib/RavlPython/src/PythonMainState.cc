// This file is part of RAVL, Recognition And Vision Library
// Copyright (C) 2008, OmniPerception Ltd.
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// Python is used under the terms of the Python License
// Copyright (C) 2001, 2002, 2003, 2004 Python Software Foundation; All Rights Reserved
// file-header-ends-here
////////////////////////////////////////////////
//! file = "Ravl/Contrib/Python/PythonMainState.cc"
//! lib=RavlPython
//! author = "Warren Moore"

#include "Ravl/PythonMainState.hh"
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
  
  PythonMainStateBodyC* g_pythonMainStateBody = NULL;

  PythonMainStateBodyC* PythonGetMainStateBody()
  { return g_pythonMainStateBody; }

	PythonMainStateBodyC::PythonMainStateBodyC(PyThreadState *threadState) :
    m_threadState(threadState)
	{
    RavlAssert(m_threadState != NULL);
    RavlAlwaysAssert(!g_pythonMainStateBody);

    ONDEBUG(std::cerr << "PythonMainStateBodyC::PythonMainStateBodyC(" << m_threadState << ")" << std::endl);

    g_pythonMainStateBody = this;
	}

  PythonMainStateBodyC::~PythonMainStateBodyC()
  {
    RavlAssert(m_threadState != NULL);
    
    ONDEBUG(std::cerr << "PythonMainStateBodyC::~PythonMainStateBodyC(" << m_threadState << ")" << std::endl);

    PyEval_AcquireLock();
    PyThreadState_Swap(m_threadState);

    Py_Finalize();

    g_pythonMainStateBody = NULL;
  }
  
}
