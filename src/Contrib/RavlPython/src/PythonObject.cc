// This file is part of RAVL, Recognition And Vision Library
// Copyright (C) 2008, OmniPerception Ltd.
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// Python is used under the terms of the Python License
// Copyright (C) 2001, 2002, 2003, 2004 Python Software Foundation; All Rights Reserved
// file-header-ends-here
////////////////////////////////////////////////
//! file = "Ravl/Contrib/Python/PythonObject.cc"
//! lib=RavlPython
//! author = "Warren Moore"

#include "Ravl/PythonObject.hh"
#include "Ravl/PythonLock.hh"
#include "Ravl/String.hh"
#include "Ravl/Python.hh"
#include "Ravl/PythonException.hh"

#define DODEBUG 0
#if DODEBUG
#define ONDEBUG(x) x
#else
#define ONDEBUG(x)
#endif

namespace RavlN
{

  PythonObjectC::PythonObjectC(const PythonC &interpreter, PyObject *object) :
    m_interpreter(interpreter),
    m_object(object)
  {
    RavlAssert(m_interpreter.IsValid());
    
    // Ref counter for the Python object is either incremented by the 
    // function that returns the object, or incremented externally if 'borrowed'
  }
  
  PythonObjectC::PythonObjectC(const PythonObjectC &copy)
  {
    RavlAssert(copy.m_interpreter.IsValid());
    
    // Up the ref counter on the interpreter to ensure it remains 
    // until the object is no longer needed
    m_interpreter = copy.m_interpreter;
    
    if (Py_IsInitialized())
    {
      PythonLockC lock(m_interpreter.GetThreadState());
      
      m_object = copy.m_object;
      if (m_object)
      {
        ONDEBUG(if (m_object) std::cerr << "PythonObjectC::PythonObjectC(copy) INCREF" << endl);
        Py_INCREF(m_object);
      }
    }
  }
  
  PythonObjectC::~PythonObjectC()
  {
    RavlAssert(m_interpreter.IsValid());

    if (Py_IsInitialized())
    {
      PythonLockC lock(m_interpreter.GetThreadState());
      
      ONDEBUG(if (m_object) std::cerr << "PythonObjectC::~PythonObjectC DECREF" << endl);
      Py_XDECREF(m_object);
    }
  }

  PythonObjectC &PythonObjectC::operator= (const PythonObjectC &rhs)
  {
    RavlAssert(m_interpreter.IsValid());

    if (Py_IsInitialized())
    {
      PythonLockC lock(m_interpreter.GetThreadState());
      
      ONDEBUG(if (m_object) std::cerr << "PythonObjectC::operator= DECREF" << endl);
      Py_XDECREF(m_object);
      
      m_object = rhs.m_object;
      
      if (m_object)
      {
        ONDEBUG(if (m_object) std::cerr << "PythonObjectC::operator= INCREF" << endl);
        Py_INCREF(m_object);
      }
    }
    
    return *this;
  }
  
  bool PythonObjectC::BuildInt(const IntT val)
  {
    RavlAssert(m_interpreter.IsValid());

    if (Py_IsInitialized())
    {
      PythonLockC lock(m_interpreter.GetThreadState());
      
      Py_XDECREF(m_object);
      m_object = NULL;
      
      m_object = PyInt_FromLong(val);
      
      CheckPythonException();
      
      return (m_object != NULL);
    }
    
    return false;
  }
  
  bool PythonObjectC::BuildInt64(const Int64T val)
  {
    RavlAssert(m_interpreter.IsValid());

    if (Py_IsInitialized())
    {
      PythonLockC lock(m_interpreter.GetThreadState());
      
      Py_XDECREF(m_object);
      m_object = NULL;
      
      m_object = PyLong_FromLongLong(val);
      
      CheckPythonException();
      
      return (m_object != NULL);
    }
    
    return false;
  }
  
  bool PythonObjectC::BuildReal(const RealT val)
  {
    RavlAssert(m_interpreter.IsValid());

    if (Py_IsInitialized())
    {
      PythonLockC lock(m_interpreter.GetThreadState());
      
      Py_XDECREF(m_object);
      m_object = NULL;
      
      m_object = PyFloat_FromDouble(val);
      
      CheckPythonException();
      
      return (m_object != NULL);
    }
    
    return false;
  }

  bool PythonObjectC::BuildString(const StringC &val)
  {
    RavlAssert(m_interpreter.IsValid());

    if (Py_IsInitialized())
    {
      PythonLockC lock(m_interpreter.GetThreadState());
      
      Py_XDECREF(m_object);
      m_object = NULL;
      
      m_object = PyString_FromString(val.chars());
      
      CheckPythonException();
      
      return (m_object != NULL);
    }
    
    return false;
  }
  
  bool PythonObjectC::BuildTuple(const DListC<PythonObjectC> &valList)
  {
    RavlAssert(m_interpreter.IsValid());

    if (Py_IsInitialized())
    {
      PythonLockC lock(m_interpreter.GetThreadState());
      
      Py_XDECREF(m_object);
      m_object = NULL;
      
      if (valList.Size() > 0)
      {
        ONDEBUG(std::cerr << "PythonObjectC::BuildTuple size=" << valList.Size() << endl);
        m_object = PyTuple_New(valList.Size());
        
        if (m_object)
        {
          UIntT pos = 0;
          for (DLIterC<PythonObjectC> it(valList); it; it++)
          {
            if (it->IsValid())
            {
              ONDEBUG(int ret = )Py_INCREF(it->m_object); // Reference is stolen by SetItem
              ONDEBUG(std::cerr << "PythonObjectC::BuildTuple adding item " << (ret == 0 ? "OK" : "FAILED") << endl);
              PyTuple_SetItem(m_object, pos, it->m_object);
            }
            else
              break;
            
            pos++;
          }
          
          if (pos != valList.Size())
          {
            Py_DECREF(m_object);
            m_object = NULL;
          }
        }
      }
      
      CheckPythonException();
      
      return (m_object != NULL);
    }
    
    return false;
  }
  
  bool PythonObjectC::IsInt()
  {
    RavlAssert(m_interpreter.IsValid());

    if (IsValid() && Py_IsInitialized())
    {
      PythonLockC lock(m_interpreter.GetThreadState());
      
      return PyInt_Check(m_object);
    }
    
    return false;
  }
  
  IntT PythonObjectC::Int()
  {
    RavlAssert(m_interpreter.IsValid());

    if (IsValid() && Py_IsInitialized())
    {
      PythonLockC lock(m_interpreter.GetThreadState());
      
      IntT ret = PyInt_AsLong(m_object);
      
      CheckPythonException();
      
      return ret;
    }
    
    return -1;
  }
  
  bool PythonObjectC::IsInt64()
  {
    RavlAssert(m_interpreter.IsValid());

    if (IsValid() && Py_IsInitialized())
    {
      PythonLockC lock(m_interpreter.GetThreadState());
      
      return PyLong_Check(m_object);
    }
    
    return false;
  }
  
  Int64T PythonObjectC::Int64()
  {
    RavlAssert(m_interpreter.IsValid());

    if (IsValid() && Py_IsInitialized())
    {
      PythonLockC lock(m_interpreter.GetThreadState());
      
      Int64T ret = PyLong_AsLongLong(m_object);
      
      CheckPythonException();
      
      return ret;
    }
    
    return -1;
  }
  
  bool PythonObjectC::IsReal()
  {
    RavlAssert(m_interpreter.IsValid());

    if (IsValid() && Py_IsInitialized())
    {
      PythonLockC lock(m_interpreter.GetThreadState());
      
      return PyFloat_Check(m_object);
    }
    
    return false;
  }
  
  RealT PythonObjectC::Real()
  {
    RavlAssert(m_interpreter.IsValid());

    if (IsValid() && Py_IsInitialized())
    {
      PythonLockC lock(m_interpreter.GetThreadState());
      
      RealT ret = PyFloat_AsDouble(m_object);
      
      CheckPythonException();
      
      return ret;
    }
    
    return -1;
  }
  
  bool PythonObjectC::IsString()
  {
    RavlAssert(m_interpreter.IsValid());

    if (IsValid() && Py_IsInitialized())
    {
      PythonLockC lock(m_interpreter.GetThreadState());
      
      return PyString_Check(m_object);
    }
    
    return false;
  }
  
  StringC PythonObjectC::String()
  {
    RavlAssert(m_interpreter.IsValid());

    if (IsValid() && Py_IsInitialized())
    {
      PythonLockC lock(m_interpreter.GetThreadState());
      
      char *charStr = PyString_AsString(m_object);
      
      CheckPythonException();
      
      if (charStr)
      {
        return StringC(charStr);
      }
    }
    
    return StringC();
  }
  
  bool PythonObjectC::IsTuple()
  {
    RavlAssert(m_interpreter.IsValid());

    if (IsValid() && Py_IsInitialized())
    {
      PythonLockC lock(m_interpreter.GetThreadState());
      
      return PyTuple_Check(m_object);
    }
    
    return false;
  }

  DListC<PythonObjectC> PythonObjectC::Tuple()
  {
    RavlAssert(m_interpreter.IsValid());

    if (IsValid() && Py_IsInitialized())
    {
      PythonLockC lock(m_interpreter.GetThreadState());
      
      DListC<PythonObjectC> ret;
      for (IntT i = 0; i < PyTuple_Size(m_object); i++)
      {
        PyObject *item = PyTuple_GetItem(m_object, i);
        if (item)
        {
          Py_INCREF(item); // GetItem returns borrowed reference
          ret.InsLast(PythonObjectC(m_interpreter, item));
        }
      }
      
      CheckPythonException();
      
      return ret;
    }
    
    return DListC<PythonObjectC>();
  }

}
