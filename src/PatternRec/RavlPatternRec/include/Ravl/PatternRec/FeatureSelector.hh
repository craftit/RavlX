// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVL_FEATURESELECTOR_HEADER
#define RAVL_FEATURESELECTOR_HEADER 1
///////////////////////////////////////////////////////////
//! rcsid="$Id$"
//! lib=RavlPatternRec
//! docentry="Ravl.API.Pattern Recognition.Feature Selection"
//! author="Robert Crida"
//! file="Ravl/PatternRec/FeatureSelection/FeatureSelector.hh"

#include "Ravl/PatternRec/DesignClassifierSupervised.hh"
#include "Ravl/PatternRec/DataSetVectorLabel.hh"
#include "Ravl/XMLFactory.hh"

namespace RavlN {
  
  //! userlevel=Develop
  //: Generic feature selector body.
  
  class FeatureSelectorBodyC
    : public RCBodyVC
  {
  public:
    FeatureSelectorBodyC() {}
    //: Constructor.
    
    FeatureSelectorBodyC(const XMLFactoryContextC & factory);
    //: Construct from XML factory

    FeatureSelectorBodyC(istream &strm);
    //: Load from stream.
    
    FeatureSelectorBodyC(BinIStreamC &strm);
    //: Load from binary stream.
    
    virtual bool Save (ostream &out) const;
    //: Writes object to stream, can be loaded using constructor
    
    virtual bool Save (BinOStreamC &out) const;
    //: Writes object to stream, can be loaded using constructor
    
    virtual SArray1dC<IndexC> SelectFeatures(DesignClassifierSupervisedC &designer,
					     const DataSetVectorLabelC &train, 
					     const DataSetVectorLabelC &test,
					     ClassifierC &classifier) const;
    //: Select feature subset which gives optimal performance
    // This is evaluated using a given 
    //!param: designer - supervised classifier designer used to create classifiers
    //!param: train - training dataset with feature vectors and labels
    //!param: test - testing dataset with feature vectors and labels
    //!param: classifier - final classifier which gave best performance with optimised feature set
    //!return: an array containing the feature indexes that are in the selected set
    
  };

  //! userlevel=Normal
  //: Generic feature selector.
  
  class FeatureSelectorC 
    : public RCHandleVC<FeatureSelectorBodyC>
  {
  public:
    FeatureSelectorC()
    {}
    //: Default constructor.
    // Creates an invalid handle.
    
    FeatureSelectorC(const XMLFactoryContextC & factory)
      : RCHandleVC<FeatureSelectorBodyC>(*new FeatureSelectorBodyC(factory))
    {
    }
    //: Construct from XML factory

    FeatureSelectorC(istream &strm);
    //: Load from stream.
    
    FeatureSelectorC(BinIStreamC &strm);
    //: Load from binary stream.
    
  protected:
    FeatureSelectorC(FeatureSelectorBodyC &bod)
      : RCHandleVC<FeatureSelectorBodyC>(bod)
    {}
    //: Body constructor.

    FeatureSelectorC(FeatureSelectorBodyC *bod)
      : RCHandleVC<FeatureSelectorBodyC>(bod)
    {}
    //: Body ptr constructor.
    
    FeatureSelectorBodyC &Body()
    { return RCHandleC<FeatureSelectorBodyC>::Body(); }
    //: Access body.
    
    const FeatureSelectorBodyC &Body() const
    { return RCHandleC<FeatureSelectorBodyC>::Body(); }
    //: Access body.
    
  public:
    SArray1dC<IndexC> SelectFeatures(DesignClassifierSupervisedC &designer,
				     const DataSetVectorLabelC &train, 
				     const DataSetVectorLabelC &test,
				     ClassifierC &classifier) const
    { return Body().SelectFeatures(designer,train,test,classifier); }
    //: Select feature subset which gives optimal performance
    // This is evaluated using a given 
    //!param: designer - supervised classifier designer used to create classifiers
    //!param: train - training dataset with feature vectors and labels
    //!param: test - testing dataset with feature vectors and labels
    //!param: classifier - final classifier which gave best performance with optimised feature set
    //!return: an array containing the feature indexes that are in the selected set
    
  };
  
  inline istream &operator>>(istream &strm,FeatureSelectorC &obj) {
    obj = FeatureSelectorC(strm);
    return strm;
  }
  //: Load from a stream.
  // Uses virtual constructor.
  
  inline ostream &operator<<(ostream &out,const FeatureSelectorC &obj) {
    obj.Save(out);
    return out;
  }
  //: Save to a stream.
  // Uses virtual constructor.
  
  inline BinIStreamC &operator>>(BinIStreamC &strm,FeatureSelectorC &obj) {
    obj = FeatureSelectorC(strm);
    return strm;
  }
  //: Load from a binary stream.
  // Uses virtual constructor.
  
  inline BinOStreamC &operator<<(BinOStreamC &out,const FeatureSelectorC &obj) {
    obj.Save(out);
    return out;
  }
  //: Save to a stream.
  // Uses virtual constructor.
  
}

#endif
