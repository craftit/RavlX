// This file is part of RAVL, Recognition And Vision Library
// Copyright (C) 2008, OmniPerception Ltd.
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// Python is used under the terms of the Python License
// Copyright (C) 2001, 2002, 2003, 2004 Python Software Foundation; All Rights Reserved
// file-header-ends-here
//////////////////////////////////////////////////
//! file = "Ravl/Contrib/Python/PythonObject.hh"
//! lib=RavlPython
//! author = "Warren Moore"
//! docentry = "Ravl.API.Python"

#ifndef RAVL_PYTHONOBJECT_HEADER
#define RAVL_PYTHONOBJECT_HEADER 1

#define register
#include <Python.h>
#undef register

#include "Ravl/Python.hh"
#include "Ravl/String.hh"
#include "Ravl/DList.hh"

namespace RavlN
{

  class PythonC;

  //! userlevel = Normal
  //: Class encapsulating a Python object.
  
  class PythonObjectC
  {
  public:
    PythonObjectC(const PythonObjectC &copy);
    //: Copy constructor.
    
    ~PythonObjectC();
    //: Destructor.
    
    bool IsValid()
    { return m_object != NULL; }
    //: Is the object valid?
    
    PythonObjectC &operator= (const PythonObjectC &rhs);
    //: Assignment.
  
    bool BuildInt(const IntT val);
    //: Make the object represent an int.
    //!return: False on failure
    //!throw: PythonExceptionC - If a Python exception is set
    
    bool BuildInt64(const Int64T val);
    //: Make the object represent a 64-bit int.
    //!return: False on failure
    //!throw: PythonExceptionC - If a Python exception is set
    
    bool BuildReal(const RealT val);
    //: Make the object represent a real.
    //!return: False on failure
    //!throw: PythonExceptionC - If a Python exception is set
    
    bool BuildString(const StringC &val);
    //: Make the object represent a string.
    //!return: False on failure
    //!throw: PythonExceptionC - If a Python exception is set
    
    bool BuildTuple(const DListC<PythonObjectC> &valList);
    //: Make the object represent a list of python objects.
    //!return: False on failure
    //!throw: PythonExceptionC - If a Python exception is set
    
    bool IsInt();
    //: Does the object represent an int?
    //!return: False if object is not an int
    
    IntT Int();
    //: Get the int object value.
    //!return: Object value as an int
    //!throw: PythonExceptionC - If a Python exception is set
    
    bool IsInt64();
    //: Does the object represent a 64-bit int?
    //!return: False if object is not a 64-bit int
    
    Int64T Int64();
    //: Get the 64-bit int object value.
    //!return: Object value as a 64-bit int
    //!throw: PythonExceptionC - If a Python exception is set
    
    bool IsReal();
    //: Does the object represent a real?
    //!return: False if object is not an real
    
    RealT Real();
    //: Get the real object value.
    //!return: Object value as a real
    //!throw: PythonExceptionC - If a Python exception is set
    
    bool IsString();
    //: Does the object represent a string?
    //!return: False if object is not a string
    
    StringC String();
    //: Get the string object value.
    //!return: Object value as a string
    //!throw: PythonExceptionC - If a Python exception is set
    
    bool IsTuple();
    //: Does the object represent a tuple?
    //!return: False if object is not a tuple
    
    DListC<PythonObjectC> Tuple();
    //: Get the tuple object list.
    //!return: Object value as a tuple
    //!throw: PythonExceptionC - If a Python exception is set
    
  private:
    PythonObjectC();
    //: Constructor.
    // Private and undefined as cannot be created without an interpreter handle
    
    PythonObjectC(const PythonC &interpreter, PyObject *object = NULL);
    //: Constructor.
    // Construct with an interpreter handle and optionally an existing object
    
    PyObject *GetObject() const
    { return m_object; }
    //: Get the object handle.
    
  private:
    PythonC m_interpreter;
    PyObject *m_object;
    
    friend class PythonBodyC;
  };
  
}

#endif

