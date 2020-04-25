// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2002, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVLMATH_STATEVECTOR_HEADER
#define RAVLMATH_STATEVECTOR_HEADER 1
//! userlevel=Normal
//! author="Phil McLauchlan"
//! date="24/7/2002"
//! rcsid="$Id$"
//! docentry="Ravl.API.Pattern Recognition.Optimisation2"
//! lib=RavlOptimise
//! file="Ravl/Math/Optimisation/StateVector.hh"

#include "Ravl/RefCounter.hh"
#include "Ravl/Vector.hh"
#include "Ravl/MatrixRS.hh"

namespace RavlN {
  
  //! userlevel=Develop
  //: State vector body class
  class StateVectorBodyC
    : public RCBodyVC
  {
  public:
    StateVectorBodyC()
    {}
    //: Default constructor.

    StateVectorBodyC(const VectorC &x);
    //: Constructor.
    
    StateVectorBodyC(const VectorC &x, const VectorC &xstep);
    //: Constructor.
    
    virtual RCBodyVC &Copy() const
    { return *new StateVectorBodyC(x.Copy(),xstep.Copy()); }
    //: Make a copy of the body.
    
    virtual bool IncrementLS(MatrixRSC &A, VectorC &a);
    //: Increment the linear system

    virtual bool Postprocess();
    //: Postprocess state vector after an adjustment to the parameters

    inline const VectorC &GetXStep() const
    { return xstep; }
    //: Get step sizes for numerical differentiation
    
    inline const VectorC &GetX() const
    { return x; }
    //: Get state vector

    bool SetX(VectorC &xnew);
    //: Set state vector

    bool IncX(VectorC &xinc);
    //: Increment state vector

    bool IncElX(IntT index, RealT val);
    //: Increment element of state vector
    
    bool DecElX(IntT index, RealT val);
    //: Decrement element of state vector

  protected:
    VectorC x;
    //: State vector

    VectorC xstep;
    //: Step sizes for numerical differentiation with respect to x
  };

  //! userlevel=Normal
  //! autoLink=on
  //: State vector class
  // This is the base class in a class hierarchy to represent state vectors
  // for the purpose of parameter estimation. For a "vanilla" estimation
  // problem with no special features to the state vector, the base class
  // may be used. The parameters are stored together in a vector x.
  // The following example derived classes are provided.
  // <ul>
  //   <li>StateVectorLine2dC represents the parameters lx,ly,lz of
  //       a 2D line lx*X + ly*Y + lz*Zh = 0 on a plane X/Y (Zh is a
  //       homogeneous 3rd plane coordinate which can be set to one).
  //       This can be used to fit a line to data points (X,Y).
  //   <li>StateVectorQuadraticC represents the parameters a,b,c of
  //       a quadratic curve Y = aX^2 + bX + X. This can be used to
  //       fit a quadratic to data points (X,Y).
  //   <li>StateVectorHomog2dC represents  the 2D homography relating two
  //       planes. The main use of this is to estimate the 2D motion between
  //       a pair of images.
  //   <li>StateVectorRot3dC represents a 3D rotation.
  // </ul>
  // The idea is to encapsulate the meaning of the parameters you want to
  // estimate, separating this from how the parameters relate to any
  // observations you make. Then the observations themselves are separate
  // entities, Deriving a sub-class allows you to strongly type the methods
  // in your observation sub-class (i.e. sub-class of ObservationC) so that
  // they ensure they are dealing with the correct state vector class.
  // It is also often necessary to add features into a state vector sub-class
  // which are switched off in the base class, through overriding the
  // definition of the Postprocess() and Restore() virtual methods.
  // See StateVectorHomog2dC and StateVectorRot3dC for examples of the use
  // of these methods.
  
  class StateVectorC
    : public RCHandleC<StateVectorBodyC>
  {
  public:    
    StateVectorC()
    {}
    //: Default constructor.
    // Creates an invalid handle.
    
    StateVectorC(const VectorC &x)
      : RCHandleC<StateVectorBodyC>(*new StateVectorBodyC(x))
    {}
    //: Constructor
    // Builds a state vector from a vector x of state parameters.

    StateVectorC(const VectorC &x, const VectorC &xstep)
      : RCHandleC<StateVectorBodyC>(*new StateVectorBodyC(x,xstep))
    {}
    //: Constructor
    // As well as the vector x of state parameters, supplies a vector of step
    // sizes xstep, the same size as x, to use for numerical differentiation
    // with respect to the elements of x, overriding the default step size
    // (1e-6).

    virtual ~StateVectorC(){}
    //: Virtual destructor,
    // Included so that the class is polymorphic and so
    // we can make dynamic casts to subclasses
    
  protected:
    StateVectorBodyC &Body()
    { return RCHandleC<StateVectorBodyC>::Body(); }
    //: Access body.

    const StateVectorBodyC &Body() const
    { return RCHandleC<StateVectorBodyC>::Body(); }
    //: Access body.
    
    StateVectorC(StateVectorBodyC &bod)
      : RCHandleC<StateVectorBodyC>(bod)
    {}
    //: Body constructor.
    
    StateVectorC(const StateVectorBodyC *bod)
      : RCHandleC<StateVectorBodyC>(bod)
    {}
    //: Body constructor.
    
  public:
    StateVectorC Copy() const { 
      if(!IsValid()) return StateVectorC();
      return StateVectorC(static_cast<StateVectorBodyC &>(Body().Copy())); 
    }
    //: Make a copy of the body.
    
    bool IncrementLS(MatrixRSC &A, VectorC &a)
    { return Body().IncrementLS(A,a); }
    //: Increment the linear system

    bool Postprocess()
    { return Body().Postprocess(); }
    //: Postprocess state vector after an adjustment to the parameters

    const VectorC &GetXStep() const
    { return Body().GetXStep(); }
    //: Get step sizes for numerical differentiation

    const VectorC &GetX() const
    { return Body().GetX(); }
    //: Get state vector

    bool SetX(VectorC &xnew)
    { return Body().SetX(xnew); }
    //: Set state vector

    bool IncX(VectorC &xinc)
    { return Body().IncX(xinc); }
    //: Increment state vector

    bool IncElX(IntT index, RealT val)
    { return Body().IncElX(index,val); }
    //: Increment element of state vector

    bool DecElX(IntT index, RealT val)
    { return Body().DecElX(index,val); }
    //: Decrement element of state vector
    
    friend class StateVectorBodyC;
  };
}


#endif
