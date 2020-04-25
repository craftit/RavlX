// This file is part of RAVL, Recognition And Vision Library
// Copyright (C) 2002, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVL_DPPROCESSFUNCTION_HEADER
#define RAVL_DPPROCESSFUNCTION_HEADER 1
///////////////////////////////////////////////////////////////
//! lib=RavlPatternRec
//! author="Charles Galambos, based on code by Robert Crida"
//! docentry="Ravl.API.Pattern Recognition.Functions"
//! file="Ravl/PatternRec/DataSet/Function.hh"

#include "Ravl/PatternRec/Function.hh"
#include "Ravl/PatternRec/Function1.hh"
#include "Ravl/DP/Process.hh"

namespace RavlN {

  //! userlevel=Develop
  //! Convert a function into a process.

  class DPProcessFunctionBodyC
    : public DPProcessBodyC<VectorC,VectorC>
  {
  public:
    DPProcessFunctionBodyC();
    //: Constructor

    DPProcessFunctionBodyC(const FunctionC &func);
    //: Constructor

    virtual VectorC Apply(const VectorC &avec);
    //: Apply operation.

  protected:
    FunctionC m_func;
  };

  //! userlevel=Develop
  //! Convert a function into a process.

  class DPProcessFunctionC
    : public DPProcessC<VectorC,VectorC>
  {
  public:
    DPProcessFunctionC()
    {}
    //: Constructor

    DPProcessFunctionC(const FunctionC &func)
     : DPProcessC<VectorC,VectorC>(new DPProcessFunctionBodyC(func))
    {}
    //: Constructor

  };

  //! userlevel=Develop
  //! Convert a function into a process.

  class DPProcessFunction1BodyC
    : public DPProcessBodyC<VectorC,RealT>
  {
  public:
    DPProcessFunction1BodyC();
    //: Constructor

    DPProcessFunction1BodyC(const Function1C &func);
    //: Constructor

    virtual RealT Apply(const VectorC &avec);
    //: Apply operation.

  protected:
    Function1C m_func;
  };

  //! userlevel=Develop
  //! Convert a function into a process.

  class DPProcessFunction1C
    : public DPProcessC<VectorC,RealT>
  {
  public:
    DPProcessFunction1C()
    {}
    //: Constructor

    DPProcessFunction1C(const Function1C &func)
     : DPProcessC<VectorC,RealT>(new DPProcessFunction1BodyC(func))
    {}
    //: Constructor

  };

}

#endif
