// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2002, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVLMATH_OBSERVATIONMANAGER_HEADER
#define RAVLMATH_OBSERVATIONMANAGER_HEADER 1
//! userlevel=Normal
//! author="Phil McLauchlan"
//! date="24/7/2002"
//! rcsid="$Id$"
//! docentry="Ravl.API.Pattern Recognition.Optimisation2"
//! example="OrthogonalRegressionTest.cc Homography2dFitTest.cc FitQuadraticTest.cc"
//! lib=RavlOptimise
//! file="Ravl/Math/Optimisation/ObservationManager.hh"

#include "Ravl/RefCounter.hh"
#include "Ravl/StateVector.hh"
#include "Ravl/Observation.hh"
#include "Ravl/DList.hh"

namespace RavlN {

  //! userlevel=Develop
  //: Observation sampling base class
  class ObservationManagerBodyC
    : public RCBodyVC
  {
  public:
    ObservationManagerBodyC()
    {}
    //: Default constructor.
    
    virtual void UnselectAllObservations();
    //: Set the "selected" flag for all observations to false

    virtual DListC<ObservationC> RandomSample(UIntT minNumConstraints);
    //: Generate a random sample of observations

    virtual DListC<ObservationC> ObservationList(const StateVectorC &stateVec) const;
    //: Generate the set of observations to be evaluated
  };

  //! userlevel=Normal
  //! autoLink=on
  //: Observation sampling base class
  // This class manages the set of observations to fitted to by RANSAC etc.
  // Derive a sub-class directly from this class if you don't want to construct
  // the complete set of candidate observations up front.
  class ObservationManagerC
    : public RCHandleC<ObservationManagerBodyC>
  {
  public:    
    ObservationManagerC()
    {}
    //: Default constructor.
    // Creates an invalid handle.
    
  protected:
    ObservationManagerC(ObservationManagerBodyC &bod)
      : RCHandleC<ObservationManagerBodyC>(bod)
    {}
    //: Body constructor.
    
    ObservationManagerC(const ObservationManagerBodyC *bod)
      : RCHandleC<ObservationManagerBodyC>(bod)
    {}
    //: Body constructor.
    
    ObservationManagerBodyC &Body()
    { return RCHandleC<ObservationManagerBodyC>::Body(); }
    //: Access body.

    const ObservationManagerBodyC &Body() const
    { return RCHandleC<ObservationManagerBodyC>::Body(); }
    //: Access body.
    
  public:
    void UnselectAllObservations()
    { Body().UnselectAllObservations(); }
    //: Set the "selected" flag for all observations to false

    DListC<ObservationC> RandomSample(UIntT minNumConstraints)
    { return Body().RandomSample(minNumConstraints); }
    //: Generate a random sample of observations
    // This virtual function generates a small random sample of observations,
    // which provide at least the given number of constraints. The constraints
    // may contain degeneracies, which should be picked up when you attempt to
    // fit the state vector parameters to the sample observations;
    // see the class FitToSampleC.
    // <p>
    // The sample should ignore all previously selected observations since the
    // last call to UnselectAllObservations(). This allows you to specify a
    // second (usually larger) samples excluding the original sample.

    DListC<ObservationC> ObservationList(const StateVectorC &stateVec) const
    { return Body().ObservationList(stateVec); }
    //: Generate the set of observations to be evaluated
    // This virtual function returns the set of observations to be evaluated
    // by RANSAC. It can use the provided fitted state vector as a context for
    // preselecting observations from a larger internal list.
    // <p>
    // This can be written to ignore all previously selected observations,
    // so that observations from samples already selected will be ignored,
    // on the assumption that their contribution to the evaluation is already
    // known. Use the "selected" flags in the ObservationC class to achieve
    // this.
  };

  //! userlevel=Develop
  //: Observation sampling class with explicit observation list
  class ObservationListManagerBodyC
    : public ObservationManagerBodyC
  {
  public:
    ObservationListManagerBodyC()
    {}
    //: Default constructor.
    
    ObservationListManagerBodyC(DListC<ObservationC> nobsList);
    //: Constructor.
    
    virtual void UnselectAllObservations();
    //: Set the "selected" flag for all observations to false

    virtual DListC<ObservationC> RandomSample(UIntT minNumConstraints);
    //: Generate a random sample of observations

    virtual DListC<ObservationC> ObservationList(const StateVectorC &stateVec) const;
    //: Generate the set of observations to be evaluated

  protected:
    DListC<ObservationC> obsList; // list of observations
    SArray1dC<ObservationC> obsArray; // array of observations
  };

  //! userlevel=Normal
  //! autoLink=on
  //: Observation sampling class with explicit observation list
  // This class manages the set of observations to fitted to by RANSAC etc,
  // given an explicit list of the complete set of observations.
  class ObservationListManagerC
    : public ObservationManagerC
  {
  public:
    ObservationListManagerC()
    {}
    //: Default constructor.
    // Creates an invalid handle.
    
    ObservationListManagerC(DListC<ObservationC> nobsList)
      : ObservationManagerC(*new ObservationListManagerBodyC(nobsList))
    {}
    //: Constructor
    
    ObservationListManagerC(const ObservationManagerC &obsManager)
      : ObservationManagerC(dynamic_cast<const ObservationListManagerBodyC *>(BodyPtr(obsManager)))
    {}
    //: Base class constructor.
    
  protected:
    ObservationListManagerC(ObservationListManagerBodyC &bod)
      : ObservationManagerC(bod)
    {}
    //: Body constructor.
    
    ObservationListManagerC(const ObservationListManagerBodyC *bod)
      : ObservationManagerC(bod)
    {}
    //: Body constructor.
    
    ObservationListManagerBodyC &Body()
    { return static_cast<ObservationListManagerBodyC &>(ObservationManagerC::Body()); }
    //: Access body.

    const ObservationListManagerBodyC &Body() const
    { return static_cast<const ObservationListManagerBodyC &>(ObservationManagerC::Body()); }
    //: Access body.
    
    friend class ObservationManagerC;
  };
}


#endif
