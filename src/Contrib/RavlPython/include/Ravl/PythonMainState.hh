// This file is part of RAVL, Recognition And Vision Library
// Copyright (C) 2008, OmniPerception Ltd.
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// Python is used under the terms of the Python License
// Copyright (C) 2001, 2002, 2003, 2004 Python Software Foundation; All Rights Reserved
// file-header-ends-here
//////////////////////////////////////////////////
//! file = "Ravl/Contrib/Python/PythonMainState.hh"
//! lib=RavlPython
//! author = "Warren Moore"
//! docentry = "Ravl.API.Python"

#ifndef RAVL_PYTHONMAINSTATE_HEADER
#define RAVL_PYTHONMAINSTATE_HEADER 1

#define register
#include <Python.h>
#undef register

#include "Ravl/RefCounter.hh"

namespace RavlN
{
  
  //! userlevel = Develop
  //: Class to manage the lifetime of the main interpreter thread state.
  
  class PythonMainStateBodyC :
    public RCBodyC
  {
  public:
    explicit PythonMainStateBodyC(PyThreadState *threadState);
    //: Constructor
    // Should only be called from PythonBodyC while holding the initialisation lock.
    
    ~PythonMainStateBodyC();
    //: Destructor
    // This will close the main interpreter thread state.
    // You should ensure this is only called from PythonBodyC while holding the initialisation lock.

  private:
    PyThreadState* m_threadState;
  };

  //! userlevel = Develop
  //: Class to manage the lifetime of the main interpreter thread state.
  // BIG OBJECT

  class PythonMainStateC :
    public RCHandleC<PythonMainStateBodyC>
  {
  public:
    PythonMainStateC()
    {}
    //: Constructor.

  private:
    PythonMainStateC(PyThreadState *threadState) :
      RCHandleC<PythonMainStateBodyC>(*new PythonMainStateBodyC(threadState))
    {}
    //: Constructor.
    // Should only be called from PythonBodyC while holding the initialisation lock.

    PythonMainStateC(PythonMainStateBodyC &body) :
      RCHandleC<PythonMainStateBodyC>(body)
    {}
    //: Body constructor.

    PythonMainStateBodyC& Body()
    { return static_cast<PythonMainStateBodyC &>(RCHandleC<PythonMainStateBodyC>::Body()); }
    //: Body Access.

    const PythonMainStateBodyC& Body() const
    { return static_cast<const PythonMainStateBodyC &>(RCHandleC<PythonMainStateBodyC>::Body()); }
    //: Body Access.

    friend class PythonBodyC;
  };
  
  PythonMainStateBodyC* PythonGetMainStateBody();
  //: Called to get the body pointer for the main interpreter thread state.

}

#endif
