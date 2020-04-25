// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVL_FeatureSelectPlusLMinusR_HEADER
#define RAVL_FeatureSelectPlusLMinusR_HEADER 1
///////////////////////////////////////////////////////////
//! rcsid="$Id$"
//! lib=RavlPatternRec
//! docentry="Ravl.API.Pattern Recognition.Feature Selection"
//! author="Kieron Messer"
//! file="Ravl/PatternRec/FeatureSelection/FeatureSelectPlusLMinusR.hh"

#include "Ravl/PatternRec/FeatureSelector.hh"


namespace RavlN {
  
  //! userlevel=Develop
  //: Plus L Minus R feature selection algorithm
  
  class FeatureSelectPlusLMinusRBodyC : public FeatureSelectorBodyC
  {
  public:
    FeatureSelectPlusLMinusRBodyC(UIntT l, UIntT r, RealT deltaError, UIntT numFeatures, UIntT numThreads);
    //: Constructor.
    
    FeatureSelectPlusLMinusRBodyC(const XMLFactoryContextC & factory);
    //: Construct from XML factory

    FeatureSelectPlusLMinusRBodyC(istream &strm);
    //: Load from stream.
    
    FeatureSelectPlusLMinusRBodyC(BinIStreamC &strm);
    //: Load from binary stream.
    
    virtual bool Save(ostream &out) const;
    //: Writes object to stream, can be loaded using constructor
    
    virtual bool Save(BinOStreamC &out) const;
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
    //!param: classifier - final classifier which gave best performance with optimised feature set. Note, you will need to ammend features or use a pre-process function to select the features from the dataset before using it.
    //!return: an array containing the feature indexes that are in the selected set
    
  protected:
    UIntT m_l; //!< How many forward
    UIntT m_r; //!< How many backwards
    RealT m_deltaError; //!< Return if error has not changed by more than this at next iteration
    UIntT m_numFeatures; //!< The maximum number of features - if not deltaError
    UIntT m_numberOfThreads; //!< How many threads we want to run in parallel
  };

  //! userlevel=Normal
  //: Plus L Minus R Feature Selection
  
  class FeatureSelectPlusLMinusRC : public FeatureSelectorC
  {
  public:
    FeatureSelectPlusLMinusRC()
    {
    }
    //: Default constructor.
    // Creates an invalid handle.

    FeatureSelectPlusLMinusRC(UIntT l, UIntT r, RealT deltaError = 0.001, UIntT numFeatures = 25, UIntT numThreads = 8) :
        FeatureSelectorC(*new FeatureSelectPlusLMinusRBodyC(l, r, deltaError, numFeatures, numThreads))
    {
    }
    //: Constructor
    //!param: l      - How many steps forward. l must be bigger than r
    //!param: r      - How many steps backward.
    //!param: numFeatures - determines how many features will be found to build the classifier.
    
    FeatureSelectPlusLMinusRC(const XMLFactoryContextC &factory) :
        FeatureSelectorC(*new FeatureSelectPlusLMinusRBodyC(factory))
    {
    }
    //: Construct from XML factory

    FeatureSelectPlusLMinusRC(istream &strm);
    //: Load from stream.
    
    FeatureSelectPlusLMinusRC(BinIStreamC &strm);
    //: Load from binary stream.
    
  protected:
    FeatureSelectPlusLMinusRC(FeatureSelectPlusLMinusRBodyC &bod) :
        FeatureSelectorC(bod)
    {
    }
    //: Body constructor.

    FeatureSelectPlusLMinusRC(FeatureSelectPlusLMinusRBodyC *bod) :
        FeatureSelectorC(bod)
    {
    }
    //: Body ptr constructor.
    
  };

}

#endif
