// This file is part of RAVL, Recognition And Vision Library
// Copyright (C) 2008, OmniPerception Ltd.
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// Python is used under the terms of the Python License
// Copyright (C) 2001, 2002, 2003, 2004 Python Software Foundation; All Rights Reserved
// file-header-ends-here
//////////////////////////////////////////////////
//! file = "Ravl/Contrib/Python/PythonException.hh"
//! lib=RavlPython
//! author = "Warren Moore"
//! docentry = "Ravl.API.Python"

#ifndef RAVL_PYTHONEXCEPTION_HEADER
#define RAVL_PYTHONEXCEPTION_HEADER 1

#include "Ravl/Exception.hh"
#include "Ravl/String.hh"
#define register
#include <Python.h>
#undef register

namespace RavlN
{
  
  //! userlevel = Normal
  //: A Python exception
  
  class PythonExceptionC :
    public ExceptionC
  {
  public:
  	PythonExceptionC(const StringC &type, const StringC &value, const StringC &trace);
    //: Constructor
    //!param: type - String representation of the Python exception sys.exc_type value
    //!param: value - String representation of the Python exception sys.exc_value value
    //!param: trace - String representation of the Python exception sys.exc_traceback value
  	
  	StringC Type()
  	{ return m_exceptionType; }
  	//! Get the exception type
  	
  	StringC Value()
  	{ return m_exceptionValue; }
  	//! Get the exception value
  	
  	StringC Trace()
  	{ return m_exceptionTrace; }
  	//! Get the exception trace
  
  private:
  	StringC m_exceptionType, m_exceptionValue, m_exceptionTrace;
  };

  //! userlevel = Develop
  //: Python exception management functions.
  // If PythonLockC could be used in the PythonC constructor,
  // making these member functions of PythonLockC would ensure
  // they were used safely.
  
  void CheckPythonException();
  //: Check and clear a Python exception.
  // This function should only be called after a PythonLockC object
  // has been created with a valid Python thread state.
  //!throw: PythonExceptionC - If a Python exception is set

  StringC GetPythonClassNameAsString(PyObject *object);
  //: Get a string representation of a class name.
  // This function should only be called after a PythonLockC object
  // has been created with a valid Python thread state.
  // Will set a Python exception on failure.
  //!return: An empty string on failure

  StringC GetPythonObjectAsString(PyObject *object);
  //: Get a string representation of an object.
  // This function should only be called after a PythonLockC object
  // has been created with a valid Python thread state.
  // Will set a Python exception on failure.
  //!return: An empty string on failure

  StringC GetPythonTraceAsString(PyObject *object);
  //: Get a string representation of a traceback object.
  // This function should only be called after a PythonLockC object
  // has been created with a valid Python thread state.
  // Will set a Python exception on failure.
  //!return: An empty string on failure

}

#endif

