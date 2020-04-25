// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2002, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//! rcsid="$Id$"
//! lib=RavlOptimise
//! file="Ravl/Math/Optimisation/ObservationManager.cc"

#include "Ravl/ObservationManager.hh"
#include "Ravl/Random.hh"

namespace RavlN {

  //: Set the "selected" flag for all observations to false
  void ObservationManagerBodyC::UnselectAllObservations()
  {
    RavlAssertMsg(0,"ObservationManagerBodyC::UnselectAllObservations(), Abstract method called ");
  }

  //: Generate a random sample of observations
  DListC<ObservationC> ObservationManagerBodyC::RandomSample(UIntT minNumConstraints)
  {
    DListC<ObservationC> emptyObsList;
    RavlAssertMsg(0,"ObservationManagerBodyC::RandomSample(UIntT), Abstract method called ");
    return emptyObsList;
  }
    
  //: Generate the set of observations to be evaluated
  DListC<ObservationC> ObservationManagerBodyC::ObservationList(
					const StateVectorC &stateVec) const
  {
    DListC<ObservationC> emptyObsList;
    RavlAssertMsg(0,"ObservationManagerBodyC::ObservationList(DListC<ObservationC>, const StateVectorC &), Abstract method called ");
    return emptyObsList;
  }
}

namespace RavlN {

  //: Constructor.
  ObservationListManagerBodyC::ObservationListManagerBodyC(DListC<ObservationC> nobsList)
    : ObservationManagerBodyC()
  {
    // copy a reference to the list
    obsList = nobsList;

    // convert list of observations to array
    obsArray = SArray1dC<ObservationC>(obsList.Size());
    IntT i=0;
    for(DLIterC<ObservationC> it(obsList);it;it++,i++)
      obsArray[i] = it.Data();
  }

  //: Set the "selected" flag for all observations to false
  void ObservationListManagerBodyC::UnselectAllObservations()
  {
    for(SArray1dIterC<ObservationC> it(obsArray);it;it++)
      it.Data().SetUnSelected();
  }

  //: Generate a random sample of observations
  DListC<ObservationC> ObservationListManagerBodyC::RandomSample(UIntT minNumConstraints) {
    SArray1dC<IntT> index(minNumConstraints);
    UIntT numConstraints=0;

    // check that there are enough observations
    for(SArray1dIterC<ObservationC> it(obsArray);it;it++) {
      numConstraints += it.Data().GetNumConstraints();
      if(numConstraints >= minNumConstraints)
	break;
    }

    if(numConstraints < minNumConstraints)
      throw ExceptionC("Not enough data for sample in ObservationListManagerC::RandomSample(UIntT). ");

    DListC<ObservationC> sample;
    // generate random sample
    numConstraints = 0;
    for(SArray1dIterC<IntT> it2(index);it2;it2++) {
      for(;;) {
	// add index of random observation to sample
	*it2 = RandomInt() % obsArray.Size();
	
	// check whether this observation is already selected
	if(obsArray[*it2].GetSelected())
	  continue;
	
	// set selected flag for observation
	obsArray[*it2].SetSelected();
	
	// accumulate the number of constraints
	numConstraints += obsArray[*it2].GetNumConstraints();
	
	// Add to sample.
	sample.InsLast(obsArray[*it2]);

	// get next element in sample
	break;
      }

      // check whether we have got enough constraints now
      if(numConstraints >= minNumConstraints)
	break;
    }

    return sample;
  }
    
  //: Generate the set of observations to be evaluated
  DListC<ObservationC> ObservationListManagerBodyC::ObservationList(
					const StateVectorC &stateVec) const
  {
    return obsList;
  }
}
