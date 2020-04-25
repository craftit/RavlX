// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2002, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVLMATH_OBSVECTOR_HEADER
#define RAVLMATH_OBSVECTOR_HEADER 1
//! userlevel=Normal
//! author="Phil McLauchlan"
//! date="24/7/2002"
//! rcsid="$Id$"
//! docentry="Ravl.API.Pattern Recognition.Optimisation2"
//! lib=RavlOptimise
//! file="Ravl/Math/Optimisation/ObsVector.hh"

#include "Ravl/RefCounter.hh"
#include "Ravl/Vector.hh"
#include "Ravl/MatrixRS.hh"

namespace RavlN {
  
  //! userlevel=Develop
  //: Observation vector body class
  class ObsVectorBodyC
    : public RCBodyVC
  {
  public:
    ObsVectorBodyC()
    {}
    //: Default constructor.
    
    ObsVectorBodyC(const VectorC &z, const MatrixRSC &Ni);
    //: Constructor.
    
    ObsVectorBodyC(const VectorC &z, const MatrixRSC &Ni,
		   const VectorC &zstep);
    //: Constructor

    virtual double Residual(const VectorC &v, const MatrixRSC &Ni);
    //: Return residual adjusted for any robust aspects to the observation

    virtual bool AdjustInformation(MatrixRSC &Aterm, VectorC &aterm);
    //: Adjust information matrix/vector term for any robustness

    virtual bool Restore();
    //: Restore values are an aborted modification

    inline const VectorC &GetZ() const
    { return z; }
    //: Get observation vector
    
    inline const MatrixRSC &GetNi() const
    { return Ni; }
    //: Get observation inverse covariance
    
    inline bool SetElZ(IntT el, RealT val)
    { z[el] = val; return true; }
    //: Set element of observation vector
    
    inline bool SetElNi(IntT row, IntT col, RealT val)
    { Ni[row][col] = val; return true; }
    //: Set element of observation inverse covariance
    
    inline const VectorC &GetZStep() const
    { return zstep; }
    //: Get step sizes for numerical differentiation

  protected:
    VectorC z;
    //: Observation vector

    VectorC zstep;
    //: Step sizes for numerical differentiation with respect to z

    MatrixRSC Ni;
    //: Inverse covariance matrix N^-1 of noise vector w added to z
  };

  //! userlevel=Normal
  //! autoLink=on
  //: Observation vector class
  // This class encapsulates an observation vector z, such as an image point or
  // other observed data object, together with an error distribution,
  // represented in base form by an inverse covariance matrix Ni.
  // <p>
  // This is the base class of a class hierarchy to represent different
  // observation types (e.g. points, lines) under different error models.
  // This base class is sufficient to represent zero-mean Gaussian errors.
  // For robust or otherwise modified error models you will need to supplement
  // the error model in sub-classes. A bi-Gaussian sub-class is provided in
  // the class ObsVectorBiGaussianC, which provides simple support for inlier
  // and outlier error distributions. Putting the inverse covariance Ni
  // here in the base class supposes that it will always be relevant for
  // defining error distributions. If you do not agree with this presumption,
  // let us know!
  // <p>
  // It might be better in future to separate the error models into a separate
  // class hierarchy (ObsErrorModelC?), and make an instance of an
  // ObsErrorModelC a member of the ObsVectorC class. Then sub-classes of
  // ObsVectorC would deal strictly with different data types.
  class ObsVectorC
    : public RCHandleC<ObsVectorBodyC>
  {
  public:    
    ObsVectorC()
    {}
    //: Default constructor.
    // Creates an invalid handle.
    
    ObsVectorC(const VectorC &z, const MatrixRSC &Ni)
      : RCHandleC<ObsVectorBodyC>(*new ObsVectorBodyC(z,Ni))
    {}
    //: Constructor
    // Builds an observation vector base class object given the vector z and
    // inverse covariance matrix Ni.

    ObsVectorC(const VectorC &z, const MatrixRSC &Ni,
	       const VectorC &zstep)
      : RCHandleC<ObsVectorBodyC>(*new ObsVectorBodyC(z,Ni,zstep))
    {}
    //: Constructor
    // Builds an observation vector base class object given the vector z and
    // inverse covariance matrix Ni, together with a vector of step sizes
    // zstep, the same size as z, to use for numerical differentiation
    // with respect to the elements of z,
    // overriding the default step size (1e-6).

  protected:
    ObsVectorC(ObsVectorBodyC &bod)
      : RCHandleC<ObsVectorBodyC>(bod)
    {}
    //: Body constructor.
    
    ObsVectorC(const ObsVectorBodyC *bod)
      : RCHandleC<ObsVectorBodyC>(bod)
    {}
    //: Body constructor.
    
    ObsVectorBodyC &Body()
    { return RCHandleC<ObsVectorBodyC>::Body(); }
    //: Access body.
    
    const ObsVectorBodyC &Body() const
    { return RCHandleC<ObsVectorBodyC>::Body(); }
    //: Access body.
    
  public:
    double Residual(const VectorC &v, const MatrixRSC &Ni)
    { return Body().Residual(v,Ni); }
    //: Return residual adjusted for any robust aspects to the observation
    // This is to be precise the negative log likelihood of the error v
    // given the observation model.
    // <p>
    // There is an assumption here that the error pdf is "local" (from the
    // terminology of M-estimates), i.e. dependent only on the relative
    // error v, not the absolute value of the observation vector z.

    bool AdjustInformation(MatrixRSC &Aterm, VectorC &aterm)
    { return Body().AdjustInformation(Aterm,aterm); }
    //: Adjust information matrix/vector term for robust error models
    // The linear system of normal equations for non-linear optimisation
    // is constructed from terms for each observation. This method takes
    // the terms produced for a single observation and adjusts them to take
    // into account the particular error model being used for this observation.
    // So outlier observations will tend to reduce Aterm and aterm.
    // <p>
    // If a non-robust error model is being used for this observation, this
    // does nothing (the default).
    // <p>
    // This scheme may not be general enough for all error models, so we may
    // need to rethink this if someone wants to implement a Lorentzian
    // (Cauchy) error distribution. It's really designed with the bi-Gaussian
    // inlier/outlier distribution in mind.

    bool Restore()
    { return Body().Restore(); }
    //: Restore values are an aborted modification
    // When the residual is calculated using the Residual() method,
    // internal fields of the observation may be updated, e.g. to indicate
    // whether the observation is an inlier or an outlier. If the state vector
    // is changed, the new residual calculated and the state modification
    // rejected, we will want to restore the old state vector parameters,
    // and also restore the internal observation fields to their original
    // state. This method is designed for this purpose. In this base
    // class this method does nothing.

    const VectorC &GetZ() const
    { return Body().GetZ(); }
    //: Get observation vector

    const MatrixRSC &GetNi() const
    { return Body().GetNi(); }
    //: Get observation inverse covariance

    bool SetElZ(IntT el, RealT val)
    { return Body().SetElZ(el,val); }
    //: Set element of observation vector

    bool SetElNi(IntT row, IntT col, RealT val)
    { return Body().SetElNi(row,col,val); }
    //: Set element of observation inverse covariance

    const VectorC &GetZStep() const
    { return Body().GetZStep(); }
    //: Get step sizes for numerical differentiation
  };
}


#endif
