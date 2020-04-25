// This file is part of RAVL, Recognition And Vision Library
// Copyright (C) 2008, OmniPerception Ltd.
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// Python is used under the terms of the Python License
// Copyright (C) 2001, 2002, 2003, 2004 Python Software Foundation; All Rights Reserved
// file-header-ends-here
////////////////////////////////////////////////////////////////
//! file = "Ravl/Contrib/Python/Python.cc"
//! lib=RavlPython
//! author = "Warren Moore"

#include "Ravl/Python.hh"
#include "Ravl/PythonObject.hh"
#include "Ravl/PythonLock.hh"
#include "Ravl/HashIter.hh"
#include <iostream>
#include "Ravl/PythonException.hh"
#include "Ravl/Threads/Thread.hh"
#if !RAVL_OS_WIN32
#include <compile.h> // Python 2.3 does not include this in Python.h
#include <eval.h> // Python 2.3 does not include this in Python.h
#endif

#define DODEBUG 0
#if DODEBUG
#define ONDEBUG(x) x
#else
#define ONDEBUG(x)
#endif

namespace
{

  const char* g_defaultGlobalDictName = "__main__";
  const char* g_defaultBuiltInDictName = "__builtins__";
  const char* g_defaultBuiltInModuleName = "__builtin__";

}

namespace RavlN
{
  
  MutexC PythonBodyC::m_initLock;
  
  PythonBodyC::PythonBodyC() :
    m_threadState(NULL),
    m_subInterpreter(false)
  {
    RWLockHoldC lock(m_lock, RWLOCK_WRITE);
    MutexLockC initLock(m_initLock);
    
    ONDEBUG(std::cerr << "PythonBodyC::PythonBodyC(" << this << ")" << endl);
    
    if (!PythonGetMainStateBody())
    {
      // Enable threads and initialise the only instance of the main Python interpreter
      Py_Initialize();
      PyEval_InitThreads();
    
      if (Py_IsInitialized())
      {
        ONDEBUG(std::cerr << "PythonBodyC::PythonBodyC(" << this << ") initialised" << endl);
        
        // Get the thread state
        m_threadState = PyThreadState_Get();

        if (m_threadState)
        {
          // Create and store the main state
          m_mainState = PythonMainStateC(m_threadState);

          ONDEBUG(std::cerr << "PythonBodyC::PythonBodyC(" << this << ") main interpreter thread state(" << m_threadState << ")" << endl);

          // Store the thread state
          m_threadId = CurrentThreadID();
          m_hashThreadState.Update(m_threadId, m_threadState);

          InitialiseEnvironment();
        }
        else
        {
          std::cerr << "PythonBodyC::PythonBodyC(" << this << ") failed to initialise Python interpreter" << std::endl;
        }

        // TODO The thread state/lock release is a prime candidate for RAII
        // but PythonLockC is not suitable here
        try
        {
          CheckPythonException();
        }
        catch (PythonExceptionC &)
        {
          PyThreadState_Swap(NULL);
          PyEval_ReleaseLock();
          throw;
        }
        
        PyThreadState_Swap(NULL);
        PyEval_ReleaseLock();
      }
      else
      {
        std::cerr << "PythonBodyC::PythonBodyC(" << this << ") failed to initialise Python interpreter" << std::endl;
      }
    }
    else
    {
      PyEval_AcquireLock();
      PyThreadState_Swap(NULL);

      // Take a handle to the main interpreter thread state
      m_mainState = PythonMainStateC(*PythonGetMainStateBody());

      m_threadState = Py_NewInterpreter();

      if (m_threadState)
      {
        ONDEBUG(std::cerr << "PythonBodyC::PythonBodyC(" << this << ") sub-interpreter thread state(" << m_threadState << ")" << endl);

        // Note we are a Python sub-interpreter
        m_subInterpreter = true;

        m_threadId = CurrentThreadID();
        m_hashThreadState.Update(m_threadId, m_threadState);

        InitialiseEnvironment();
      }
      else
      {
        std::cerr << "PythonBodyC::PythonBodyC(" << this << ") failed to initialise Python sub-interpreter" << std::endl;
      }

      // TODO The thread state/lock release is a prime candidate for RAII
      // but PythonLockC is not suitable here
      try
      {
        CheckPythonException();
      }
      catch (PythonExceptionC &)
      {
        PyThreadState_Swap(NULL);
        PyEval_ReleaseLock();
        throw;
      }

      PyThreadState_Swap(NULL);
      PyEval_ReleaseLock();
    }
  }
  
  PythonBodyC::~PythonBodyC()
  {
    RWLockHoldC lock(m_lock, RWLOCK_WRITE);
    MutexLockC initLock(m_initLock);
  
    ONDEBUG(std::cerr << "PythonBodyC::~PythonBodyC(" << this << ")" << endl);

    if (Py_IsInitialized())
    {
      PyEval_AcquireLock();
      PyThreadState_Swap(m_threadState);

      // Don't delete the main thread
      m_hashThreadState.Del(m_threadId);
      ONDEBUG(std::cerr << "  PythonBodyC::~PythonBodyC(" << this << ") deleting (" << m_hashThreadState.Size() << ") thread states" << endl);
      for (HashIterC<SizeT, PyThreadState*> iterThreadState(m_hashThreadState); iterThreadState; iterThreadState++)
      {
        PyThreadState_Clear(iterThreadState.Data());
        PyThreadState_Delete(iterThreadState.Data());
      }

      for (HashIterC<StringC, PyObject*> it(m_hashModules); it; it++)
      {
        ONDEBUG(std::cerr << "  PythonBodyC::~PythonBodyC(" << this << ") DECREF module(" << it.Key() << ")" << endl);
        Py_DECREF(it.Data());
      }

      if (m_subInterpreter)
      {
        ONDEBUG(std::cerr << "  PythonBodyC::~PythonBodyC(" << this << ") sub-interpreter destroyed" << endl);

        Py_EndInterpreter(m_threadState);
      }

      PyThreadState_Swap(NULL);
      PyEval_ReleaseLock();
      
      m_mainState.Invalidate();
    }
  }
  
  bool PythonBodyC::AppendSystemPath(const StringC &path)
  {
    ONDEBUG(std::cerr << "PythonBodyC::AppendSystemPath(" << this << ") path(" << path << ")" << endl);
    RavlAssert(Initialised());

    if (Py_IsInitialized())
    {
      PythonLockC pythonLock(GetThreadState());
      
      bool ret = false;

      PyObject *dictGlobal = GetModuleDictionary(g_defaultGlobalDictName);

      if (dictGlobal)
      {
        PyObject *sysModule = PyImport_ImportModuleEx(const_cast<char*>("sys"), dictGlobal, dictGlobal, NULL);
        if (sysModule)
        {
          ONDEBUG(std::cerr << "PythonBodyC::AppendSystemPath(" << this << ") imported sys" << endl);
          // Returns NULL on failure
          PyObject *sysPath = PyObject_GetAttrString(sysModule, "path");
          if (sysPath)
          {
            ONDEBUG(std::cerr << "PythonBodyC::AppendSystemPath(" << this << ") found sys.path" << endl);
            // Returns NULL on failure
            PyObject *newPath = PyString_FromString(const_cast<char*>(path.chars()));
            if (newPath)
            {
              ONDEBUG(std::cerr << "PythonBodyC::AppendSystemPath(" << this << ") appending to sys.path" << endl);

              // Returns NULL on failure and sets Python exception
              PyList_Append(sysPath, newPath);
              Py_DECREF(newPath);

              ret = true;
            }

            Py_DECREF(sysPath);
          }
        }

        Py_DECREF(dictGlobal);
      }

      CheckPythonException();

      return ret;
    }
    
    return false;
  }
  
  bool PythonBodyC::Import(const StringC &module)
  {
    ONDEBUG(std::cerr << "PythonBodyC::Import(" << this << ") name(" << module << ")" << endl);
    RavlAssert(Initialised());
    
    if (Py_IsInitialized())
    {
      PythonLockC pythonLock(GetThreadState());
      
      bool ret = false;
      PyObject *mainModule = NULL;

      RWLockHoldC lock(m_lock, RWLOCK_WRITE);

      // Get the '__main__' module pointer
      if (m_hashModules.Lookup(g_defaultGlobalDictName, mainModule))
      {
        // Import the module
        // Returns NULL on failure and sets Python exception
        PyObject *modulePtr = PyImport_ImportModule(const_cast<char*>(module.chars()));
        if (modulePtr)
        {
          // PyModule_AddObject steals a reference, so increment ours so we can safely store it
          Py_INCREF(modulePtr);

          // Returns 0 on success, -1 on error
          if (PyModule_AddObject(mainModule, const_cast<char*>(module.chars()), modulePtr) == 0)
          {
            ONDEBUG(std::cerr << "  PythonBodyC::Import(" << this << ") imported(" << module << ")" << endl);

            m_hashModules.Update(module, modulePtr);
            
            ret = true;
          }
          else
          {
            // We're not storing the module pointer, so release it
            Py_DECREF(modulePtr);
          }
        }
      }

      CheckPythonException();
      
      return ret;
    }
    
    return false;
  }
  
  PythonObjectC PythonBodyC::NewObject()
  {
    return PythonObjectC(PythonC(*this));
  }
  
  PythonObjectC PythonBodyC::Call(const StringC &function)
  {
    return Call(g_defaultGlobalDictName, function, PythonObjectC(PythonC(*this)));
  }

  PythonObjectC PythonBodyC::Call(const StringC &function,
                                  const PythonObjectC &args)
  {
    return Call(g_defaultGlobalDictName, function, args);
  }

  PythonObjectC PythonBodyC::Call(const StringC &module,
                                  const StringC &function)
  {
    return Call(module, function, PythonObjectC(PythonC(*this)));
  }

  PythonObjectC PythonBodyC::Call(const StringC &module,
                                  const StringC &function,
                                  const PythonObjectC &args)
  {
    ONDEBUG(std::cerr << "PythonBodyC::Call(" << this << ") module(" << module << ") function(" << function << ")" << endl);
    RavlAssert(Initialised());

    if (Py_IsInitialized())
    {
      PythonLockC pythonLock(GetThreadState());

      PyObject *ret = NULL;
      PyObject *modulePtr = NULL;

      RWLockHoldC lock(m_lock, RWLOCK_READONLY);

      if (m_hashModules.Lookup(module, modulePtr))
      {
        lock.Unlock();

        // Returns NULL on failure
        PyObject *func = PyObject_GetAttrString(modulePtr, const_cast<char*>(function.chars()));

        // Returns 1 if callable, 0 otherwise
        if (func && PyCallable_Check(func))
        {
          if (args.GetObject())
          {
            if (PyTuple_Check(args.GetObject()))
            {
              // Returns NULL on failure, script may set an exception
              ret = PyObject_CallObject(func, args.GetObject());

              ONDEBUG(std::cerr << "  PythonBodyC::Call(" << this << ") function(" << function << ") args(" << PyTuple_Size(args.GetObject()) << ") " << (ret ? "OK" : "FAILED") << endl);
            }
            else
            {
              std::cerr << "PythonBodyC::Call tuple not supplied as arguments" << std::endl;
            }
          }
          else
          {
            // Returns NULL on failure, script may set an exception
            ret = PyObject_CallObject(func, NULL);

            ONDEBUG(std::cerr << "  PythonBodyC::Call(" << this << ") function(" << function << ") " << (ret ? "OK" : "FAILED") << endl);
          }

        }
        else
        {
          std::cerr << "PythonBodyC::Call(" << this << ") failed to find function("  << function << ")" << std::endl;
        }

        Py_XDECREF(func);
      }

      CheckPythonException();

      return PythonObjectC(PythonC(*this), ret);
    }

    return PythonObjectC(PythonC(*this));
  }

  PythonObjectC PythonBodyC::CallMethod(const StringC &object,
                                        const StringC &function)
  {
    return CallMethod(g_defaultGlobalDictName, object, function, PythonObjectC(PythonC(*this)));
  }

  PythonObjectC PythonBodyC::CallMethod(const StringC &object,
                                        const StringC &function,
                                        const PythonObjectC &args)
  {
    return CallMethod(g_defaultGlobalDictName, object, function, args);
  }

  PythonObjectC PythonBodyC::CallMethod(const StringC &module,
                                        const StringC &object,
                                        const StringC &function)
  {
    return CallMethod(module, object, function, PythonObjectC(PythonC(*this)));
  }

  PythonObjectC PythonBodyC::CallMethod(const StringC &module,
                                        const StringC &object,
                                        const StringC &function,
                                        const PythonObjectC &args)
  {
    ONDEBUG(std::cerr << "PythonBodyC::Call(" << this << ") module(" << module << ") object(" << object << ") function(" << function << ")" << endl);
    RavlAssert(Initialised());

    if (Py_IsInitialized())
    {
      PythonLockC pythonLock(GetThreadState());

      PyObject *ret = NULL;
      PyObject *modulePtr = NULL;

      RWLockHoldC lock(m_lock, RWLOCK_READONLY);

      if (m_hashModules.Lookup(module, modulePtr))
      {
        lock.Unlock();

        // Returns NULL on failure
        PyObject *objectPtr = PyObject_GetAttrString(modulePtr, const_cast<char*>(object.chars()));
        PyObject *functionPtr = PyString_FromString(const_cast<char*>(function.chars()));

        if (objectPtr && functionPtr)
        {
          if (args.GetObject())
          {
            if (PyTuple_Check(args.GetObject()))
            {
              // Returns NULL on failure, script may set an exception
              ret = PyObject_CallMethodObjArgs(objectPtr, functionPtr, args.GetObject(), NULL);

              ONDEBUG(std::cerr << "  PythonBodyC::Call(" << this << ") object(" << object << ") function(" << function << ") args(" << PyTuple_Size(args.GetObject()) << ") " << (ret ? "OK" : "FAILED") << endl);
            }
            else
            {
              std::cerr << "PythonBodyC::Call tuple not supplied as arguments" << std::endl;
            }
          }
          else
          {
            // Returns NULL on failure, script may set an exception
            ret = PyObject_CallMethodObjArgs(objectPtr, functionPtr, NULL);

            ONDEBUG(std::cerr << "  PythonBodyC::Call(" << this << ") object(" << object << ") function(" << function << ") " << (ret ? "OK" : "FAILED") << endl);
          }
        }
        else
        {
          std::cerr << "PythonBodyC::Call(" << this << ") failed to find object("  << function << ")" << std::endl;
        }

        Py_XDECREF(objectPtr);
        Py_XDECREF(functionPtr);
      }

      CheckPythonException();

      return PythonObjectC(PythonC(*this), ret);
    }

    return PythonObjectC(PythonC(*this));
  }

  bool PythonBodyC::Run(const StringC &script, const StringC &traceName)
  {
    ONDEBUG(std::cerr << "PythonBodyC::Run(" << this << ")" << endl);
    RavlAssert(Initialised());
    
    if (Py_IsInitialized())
    {
      PythonLockC pythonLock(GetThreadState());
      
      PyObject *resultObj = NULL;
      
      // Can we compile the code?
      PyObject *compiledCodeObj = Py_CompileString(script, traceName.chars(), Py_file_input);
      if (compiledCodeObj)
      {
        // Get the scope
        PyObject *dictGlobal = GetModuleDictionary(g_defaultGlobalDictName);
        
        if (dictGlobal)
        {
          // Have we got valid code?
          if (PyCode_Check(compiledCodeObj))
          {
            resultObj = PyEval_EvalCode(reinterpret_cast<PyCodeObject*>(compiledCodeObj), dictGlobal, dictGlobal);
            Py_XDECREF(resultObj);
          }

          Py_DECREF(dictGlobal);
        }

        Py_DECREF(compiledCodeObj);
      }
      
      CheckPythonException();

      return resultObj != NULL;
    }
    
    return false;
  }
  
  PythonObjectC PythonBodyC::GetValue(const StringC &name)
  {
    return GetValue(g_defaultGlobalDictName, name);
  }

  PythonObjectC PythonBodyC::GetValue(const StringC &module, const StringC &name)
  {
    ONDEBUG(std::cerr << "PythonBodyC::GetValue(" << this << ") module(" << module << ") name(" << name << ")" << endl);
    RavlAssert(Initialised());

    if (Py_IsInitialized())
    {
      PythonLockC pythonLock(GetThreadState());

      PyObject *resultObj = NULL;

      PyObject *dictModule = GetModuleDictionary(module);
      if (dictModule)
      {
        // Return NULL on failure
        resultObj = PyDict_GetItemString(dictModule, name);
        Py_XINCREF(resultObj);
        ONDEBUG(if (resultObj == NULL) std::cerr << "  PythonBodyC::GetValue(" << this << ") failed to find name(" << name << ") in module(" << module << ")" << endl);

        Py_DECREF(dictModule);
      }

      CheckPythonException();

      return PythonObjectC(PythonC(*this), resultObj);
    }

    return PythonObjectC(PythonC(*this));
  }

  void PythonBodyC::InitialiseEnvironment()
  {
    ONDEBUG(std::cerr << "PythonBodyC::InitialiseEnvironment(" << this << ")" << endl);
    RavlAssert(Initialised());
    RavlAssert(Py_IsInitialized());
    RavlAssert(m_threadId == CurrentThreadID());

    // Get access to the main module
    // Returns NULL on failure and sets Python exception
    PyObject *mainModule = PyImport_AddModule(g_defaultGlobalDictName);
    if (!mainModule)
    {
      std::cerr << "PythonBodyC::PythonBodyC(" << this << ") failed to initialise main environment" << std::endl;
      return;
    }

    Py_INCREF(mainModule); // AddModule returns borrowed reference
    m_hashModules.Update(g_defaultGlobalDictName, mainModule);
    
    PyObject *mainDict = PyModule_GetDict(mainModule);
    if (!mainDict)
    {
      std::cerr << "PythonBodyC::PythonBodyC(" << this << ") failed to initialise main environment" << std::endl;
      return;
    }

    Py_INCREF(mainDict);

    PyDict_Clear(mainDict);

#if 0
    PyObject *builtins = PyEval_GetBuiltins();
#else
    PyObject *builtins = PyImport_AddModule(g_defaultBuiltInModuleName);
#endif
    PyDict_SetItemString(mainDict, g_defaultBuiltInDictName, builtins);

    Py_DECREF(mainDict);

    if (PyEval_GetRestricted())
    {
      std::cerr << "PythonBodyC::PythonBodyC(" << this << ") restricted mode" << std::endl;
    }
  }
  
  PyObject *PythonBodyC::GetModuleDictionary(const StringC &name)
  {
    ONDEBUG(std::cerr << "PythonBodyC::GetModuleDictionary(" << this << ") name(" << name << ")" << endl);
    RavlAssert(Initialised());
    RavlAssert(Py_IsInitialized());
    
    PyObject *mainModule = NULL;
    PyObject *mainDict = NULL;

    RWLockHoldC lock(m_lock, RWLOCK_READONLY);

    if (m_hashModules.Lookup(name, mainModule))
    {
      lock.Unlock();
      
      // Returns NULL on failure
      mainDict = PyModule_GetDict(mainModule);
      Py_INCREF(mainDict);
    }

    return mainDict;
  }
  
  PyThreadState* PythonBodyC::GetThreadState()
  {
    RavlAssert(Initialised());
    RavlAssert(Py_IsInitialized());

    PyThreadState* threadState = NULL;
    SizeT threadId = CurrentThreadID();

    RWLockHoldC lock(m_lock, RWLOCK_READONLY);

    if (!m_hashThreadState.Lookup(threadId, threadState))
    {
      lock.Unlock();
      lock.LockWr();

      if (!m_hashThreadState.Lookup(threadId, threadState))
      {
        threadState = PyThreadState_New(m_threadState->interp);
        ONDEBUG(std::cerr << "PythonBodyC::DoGetThreadState(" << this << ") created new thread state(" << threadState << ")" << endl);

        m_hashThreadState.Update(threadId, threadState);
      }
    }

    return threadState;
  }

  // Following functions are defined here to avoid cyclic dependency on PythonObject
  
  PythonObjectC PythonC::NewObject()
  {
    return Body().NewObject();
  }

  PythonObjectC PythonC::Call(const StringC &name)
  {
    return Body().Call(name);
  }

  PythonObjectC PythonC::Call(const StringC &name,
                              const PythonObjectC &args)
  {
    return Body().Call(name, args);
  }

  PythonObjectC PythonC::Call(const StringC &module,
                              const StringC &name)
  {
    return Body().Call(module, name);
  }

  PythonObjectC PythonC::Call(const StringC &module,
                              const StringC &name,
                              const PythonObjectC &args)
  {
    return Body().Call(module, name, args);
  }

  PythonObjectC PythonC::CallMethod(const StringC &object,
                                    const StringC &function)
  {
    return Body().CallMethod(object, function);
  }

  PythonObjectC PythonC::CallMethod(const StringC &object,
                                    const StringC &function,
                                    const PythonObjectC &args)
  {
    return Body().CallMethod(object, function, args);
  }

  PythonObjectC PythonC::CallMethod(const StringC &module,
                                    const StringC &object,
                                    const StringC &function)
  {
    return Body().CallMethod(module, object, function);
  }

  PythonObjectC PythonC::CallMethod(const StringC &module,
                                    const StringC &object,
                                    const StringC &function,
                                    const PythonObjectC &args)
  {
    return Body().CallMethod(module, object, function, args);
  }

  PythonObjectC PythonC::GetValue(const StringC &name)
  {
    return Body().GetValue(name);
  }

  PythonObjectC PythonC::GetValue(const StringC &module, const StringC &name)
  {
    return Body().GetValue(module, name);
  }

}
