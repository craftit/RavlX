// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2002, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//! rcsid="$Id$"
//! lib=RavlPatternRec
//! file="Ravl/PatternRec/FeatureSelection/FeatureSelector.cc"

#include "Ravl/PatternRec/FeatureSelector.hh"
#include "Ravl/VirtualConstructor.hh"

namespace RavlN {

  //: Load from XML factory

  FeatureSelectorBodyC::FeatureSelectorBodyC(const XMLFactoryContextC & factory) {
  }

  //: Load from stream.
  
  FeatureSelectorBodyC::FeatureSelectorBodyC(istream &strm)
    : RCBodyVC(strm)
  {}
  
  //: Load from binary stream.
  
  FeatureSelectorBodyC::FeatureSelectorBodyC(BinIStreamC &strm)
    : RCBodyVC(strm)
  {}
  
  //: Writes object to stream, can be loaded using constructor
  
  bool FeatureSelectorBodyC::Save (ostream &out) const {
    if(!RCBodyVC::Save(out))
      return false;
    return true;
  }
  
  //: Writes object to stream, can be loaded using constructor
  
  bool FeatureSelectorBodyC::Save (BinOStreamC &out) const {
    if(!RCBodyVC::Save(out))
      return false;
    return true;
  }
  
  SArray1dC<IndexC> FeatureSelectorBodyC::SelectFeatures(DesignClassifierSupervisedC &designer,
							 const DataSetVectorLabelC &train, 
							 const DataSetVectorLabelC &test,
							 ClassifierC &classifier) const {
    RavlAssertMsg(0,"FeatureSelectorBodyC::SelectFeatures(), Abstract method called. ");
    return SArray1dC<IndexC>();
  }
  
  //: Load from stream.
  
  FeatureSelectorC::FeatureSelectorC(istream &strm) 
    : RCHandleVC<FeatureSelectorBodyC>(RAVL_VIRTUALCONSTRUCTOR(strm,FeatureSelectorBodyC))
  {}
    
  //: Load from binary stream.
  
  FeatureSelectorC::FeatureSelectorC(BinIStreamC &strm) 
    : RCHandleVC<FeatureSelectorBodyC>(RAVL_VIRTUALCONSTRUCTOR(strm,FeatureSelectorBodyC))    
  {}

  //: Setup virtual constructor.
  // As this is an abstract class its not really needed, but its usefull to
  // have an example of how it should work here.
  
  //RAVL_INITVIRTUALCONSTRUCTOR(FeatureSelectorBodyC);
  
}
