// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVL_FEATURESELECTASYMMETRICADABOOST_HEADER
#define RAVL_FEATURESELECTASYMMETRICADABOOST_HEADER 1
///////////////////////////////////////////////////////////
//! rcsid="$Id$"
//! lib=RavlPatternRec
//! docentry="Ravl.API.Pattern Recognition.Feature Selection"
//! author="Robert Crida"
//! file="Ravl/PatternRec/FeatureSelection/FeatureSelectAsymmetricAdaBoost.hh"

#include "Ravl/PatternRec/FeatureSelector.hh"

namespace RavlN {
  
  //! userlevel=Develop
  //: Asymmetric ADA boost feature selector body.
  
  class FeatureSelectAsymmetricAdaBoostBodyC : public FeatureSelectorBodyC
  {
  public:
    FeatureSelectAsymmetricAdaBoostBodyC(RealT weight, UIntT numFeatures);
    //: Constructor.
    
    FeatureSelectAsymmetricAdaBoostBodyC(istream &strm);
    //: Load from stream.
    
    FeatureSelectAsymmetricAdaBoostBodyC(BinIStreamC &strm);
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
    //!param: classifier - final classifier which gave best performance with optimised feature set
    //!return: an array containing the feature indexes that are in the selected set
    
  private:
    bool AlreadyUsed(const SArray1dC<IndexC> &featureSet, IndexC feature) const;
    VectorC NormaliseWeights(const VectorC &weights) const;

  protected:
    RealT m_weight;
    UIntT m_numFeatures;
  };

  //! userlevel=Normal
  //: Asymmetric ADA boost feature selector.
  // It is able to give a relative weight to each of the labels.
  
  class FeatureSelectAsymmetricAdaBoostC : public FeatureSelectorC
  {
  public:
    FeatureSelectAsymmetricAdaBoostC()
    {
    }
    //: Default constructor.
    // Creates an invalid handle.

    FeatureSelectAsymmetricAdaBoostC(RealT weight, UIntT numFeatures) :
        FeatureSelectorC(*new FeatureSelectAsymmetricAdaBoostBodyC(weight, numFeatures))
    {
    }
    //: Constructor
    //!param: weight      - A priori probability for labels. Greater than 0.5 favours label1, less than 0.5 favours label0. 0 < weight < 1.
    //!param: numFeatures - determines how many features will be found to build the classifier.
    
    FeatureSelectAsymmetricAdaBoostC(istream &strm);
    //: Load from stream.
    
    FeatureSelectAsymmetricAdaBoostC(BinIStreamC &strm);
    //: Load from binary stream.
    
  protected:
    FeatureSelectAsymmetricAdaBoostC(FeatureSelectAsymmetricAdaBoostBodyC &bod) :
        FeatureSelectorC(bod)
    {
    }
    //: Body constructor.

    FeatureSelectAsymmetricAdaBoostC(FeatureSelectAsymmetricAdaBoostBodyC *bod) :
        FeatureSelectorC(bod)
    {
    }
    //: Body ptr constructor.
    
  };

}

#endif
