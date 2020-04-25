// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2003, OmniPerception Ltd.
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVL_TestFunctions_HEADER
#define RAVL_TestFunctions_HEADER 1

//! lib=RavlPatternRec
//! author="Kieron Messer"
//! docentry="Ravl.API.Pattern Recognition.Functions.TestFunctions"
//! file="Ravl/PatternRec/DataSet/TestFunction.hh"

#include "Ravl/PatternRec/Function.hh"

namespace RavlN {

  /*
   * Some useful functions for testing things.
   */

  //! userlevel=Develop
  //: Ackley function. Has global minimum at x = [0.0, ...., 0.0]
  
  class FunctionAckleyBodyC : public FunctionBodyC
  {
  public:
    
    FunctionAckleyBodyC(UIntT numberOfDimensions = 2);
    //: Default constructor.

    virtual VectorC Apply(const VectorC &data) const;
    //: Apply function to 'data'
    
  protected:
    
  };
  
  //! userlevel=Normal
  //: Ackley function
  
  class FunctionAckleyC : public FunctionC
  {
  public:
    
    FunctionAckleyC(UIntT numberOfDimensions = 2) :
        FunctionC(*new FunctionAckleyBodyC(numberOfDimensions))
    {
    }
    //: Construct from number of dimentions
    
  };

  //! userlevel=Develop
  //: Sphere function

  class FunctionSphereBodyC : public FunctionBodyC
  {
  public:

    FunctionSphereBodyC(UIntT numberOfDimensions = 2);
    //: Default constructor.
    
    virtual VectorC Apply(const VectorC &data) const;
    //: Apply function to 'data'

  protected:

  };
  
  //! userlevel=Normal
  //: Sphere function.  Has global minimum at x = [0.0, ...., 0.0]
  
  class FunctionSphereC : public FunctionC
  {
  public:

    FunctionSphereC(UIntT numberOfDimensions = 2) :
        FunctionC(*new FunctionSphereBodyC(numberOfDimensions))
    {
    }
    //: Construct from number of dimentions

  };

  //! userlevel=Develop
   //: Griewank function

   class FunctionGriewankBodyC : public FunctionBodyC
   {
   public:

     FunctionGriewankBodyC(UIntT numberOfDimensions = 2);
     //: Default constructor.

     virtual VectorC Apply(const VectorC &data) const;
     //: Apply function to 'data'

   protected:

   };

   //! userlevel=Normal
   //: Griewank function.  Has global minimum at x = [0.0, ...., 0.0]
   // http://mathworld.wolfram.com/GriewankFunction.html

   class FunctionGriewankC : public FunctionC
   {
   public:

     FunctionGriewankC(UIntT numberOfDimensions = 2) :
         FunctionC(*new FunctionGriewankBodyC(numberOfDimensions))
     {
     }
     //: Construct from number of dimentions

   };


}

#endif
