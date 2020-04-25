// This file is part of RAVL, Recognition And Vision Library
// Copyright (C) 2008, OmniPerception Ltd.
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// Python is used under the terms of the Python License
// Copyright (C) 2001, 2002, 2003, 2004 Python Software Foundation; All Rights Reserved
// file-header-ends-here
//////////////////////////////////////////////////
//! file = "Ravl/Contrib/Python/PythonLock.hh"
//! lib=RavlPython
//! author = "Warren Moore"
//! docentry = "Ravl.API.Python"

#ifndef RAVL_PYTHONLOCK_HEADER
#define RAVL_PYTHONLOCK_HEADER 1

#define register
#include <Python.h>
#undef register

namespace RavlN
{
  
  //! userlevel = Develop
  //: A wrapper object managing the Python interpreter thread state and lock using RAII.
  
  class PythonLockC
  {
  public:
    explicit PythonLockC(PyThreadState *threadState);
    //: Constructor
    //!param: threadState - The current interpreter thread state. Must not be NULL.
    // This will acquire the interpreter lock and set the specified thread state.
    
    ~PythonLockC();
    //: Destructor
    // This will clear the thread state and release the interpreter lock.
  };

}

#endif

