// This file is part of RAVL, Recognition And Vision Library
// Copyright (C) 2008, OmniPerception Ltd.
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// Python is used under the terms of the Python License
// Copyright (C) 2001, 2002, 2003, 2004 Python Software Foundation; All Rights Reserved
// file-header-ends-here
////////////////////////////////////////////////
//! file = "Ravl/Contrib/Python/PythonException.cc"
//! lib=RavlPython
//! author = "Warren Moore"

#include "Ravl/PythonException.hh"

#define DODEBUG 0
#if DODEBUG
#define ONDEBUG(x) x
#else
#define ONDEBUG(x)
#endif

namespace RavlN
{
  
	PythonExceptionC::PythonExceptionC(const StringC &type, const StringC &value, const StringC &trace) :
		ExceptionC("PythonException"),
		m_exceptionType(type),
		m_exceptionValue(value),
		m_exceptionTrace(trace)
	{
	}

  void CheckPythonException()
  {
    if (PyErr_Occurred())
    {
    	// Pointers for fetched exception details
    	PyObject *exType = NULL;
    	PyObject *exValue = NULL;
    	PyObject *exTrace = NULL;
    	PyErr_Fetch(&exType, &exValue, &exTrace);
      
      // Not all exceptions are apparently created equal...
      // SyntaxErrors are reported differently with the following code, and
      // PyErr_NormalizeException seems to produce strange results for all
      // exceptions. The 'correct' behaviour can be found in the source for
      // the Python function PyErr_PrintEx, but this implementation works
      // well for most at the moment...
    	
    	// If the object exists, create and store a string representation of it
  		StringC exceptionType = (exType ? GetPythonClassNameAsString(exType) : StringC());
  		StringC exceptionValue = (exValue ? GetPythonObjectAsString(exValue) : StringC());
  		StringC exceptionTrace = (exTrace ? GetPythonTraceAsString(exTrace) : StringC());
    	
  		// Clear the exception objects (XDECREF as they may be NULL)
  		Py_XDECREF(exType);
  		Py_XDECREF(exValue);
  		Py_XDECREF(exTrace);
    	
  		// Clear the exception flags
      PyErr_Clear();
      
      // Package and throw the exception
      throw PythonExceptionC(exceptionType, exceptionValue, exceptionTrace);
    }
  }
  
  StringC GetPythonClassNameAsString(PyObject *object)
  {
  	RavlAssert(object != NULL);

    StringC classNameString;

  	PyObject *classObject = NULL;
    // Return NULL on failure
  	if ((classObject = PyObject_GetAttrString(object, "__class__")) != NULL)
  	{
    	PyObject *nameObject = NULL;
      // Return NULL on failure
      if ((nameObject = PyObject_GetAttrString(object, "__name__")) != NULL)
      {
        // Returns NULL on failure and sets Python exception
        const char * const nameString = PyString_AsString(nameObject);

        classNameString = StringC(nameString);

        Py_DECREF(nameObject);
      }

  		Py_DECREF(classObject);
    }

  	return classNameString;
  }

  StringC GetPythonObjectAsString(PyObject *object)
  {
  	RavlAssert(object != NULL);

  	PyObject *objectStr = NULL;
    // Return NULL on failure
  	if ((objectStr = PyObject_Str(object)) != NULL)
  	{
      // Returns NULL on failure and sets Python exception
  		const char * const chars = PyString_AsString(objectStr);
  		StringC str(chars);

  		Py_DECREF(objectStr);

  		return str;
    }

  	return StringC();
  }

  StringC GetPythonTraceAsString(PyObject *object)
  {
  	RavlAssert(object != NULL);
  	
  	StringC ret;
  	
  	// Import the traceback module to do the formatting
    // Returns NULL on failure and sets Python exception
    PyObject *traceModule = PyImport_ImportModule("traceback");
    if (traceModule)
    {
    	// 'formatRet = traceback.format_tb(sys.exc_traceback)'
      // Returns NULL on failure, script may set an exception
    	PyObject *formatRet = PyObject_CallMethod(traceModule, "format_tb", "O", object);
    	if (formatRet)
    	{
    		// 'joinRet = "".join(formatRet)'
        // Return NULL on failure
      	PyObject *emptyStr = PyString_FromString("");
        
        // Returns NULL on failure, script may set an exception
      	PyObject *joinRet = PyObject_CallMethod(emptyStr, "join", "O", formatRet);
      	if (joinRet)
      	{
          // Returns NULL on failure and sets Python exception
      		const char * const chars = PyString_AsString(joinRet);
      		ret = StringC(chars);
      		
      		Py_DECREF(joinRet);
      	}
      	
      	Py_DECREF(emptyStr);
      	Py_DECREF(formatRet);
    	}

      Py_DECREF(traceModule);
    }
    
    return ret;
  }

}
