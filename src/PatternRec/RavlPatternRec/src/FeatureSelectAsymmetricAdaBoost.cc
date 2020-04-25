// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//! rcsid="$Id$"
//! lib=RavlPatternRec
//! file="Ravl/PatternRec/FeatureSelection/FeatureSelectAsymmetricAdaBoost.cc"

#include "Ravl/PatternRec/FeatureSelectAsymmetricAdaBoost.hh"
#include "Ravl/VirtualConstructor.hh"
#include "Ravl/BinStream.hh"
#include "Ravl/Vector2d.hh"
#include "Ravl/PatternRec/SampleIter.hh"
#include "Ravl/PatternRec/ClassifierLinearCombination.hh"
#include "Ravl/PatternRec/Error.hh"

#define DODEBUG 0
#if DODEBUG
#define ONDEBUG(x) x
#else
#define ONDEBUG(x)
#endif

namespace RavlN {

  //: Constructor

  FeatureSelectAsymmetricAdaBoostBodyC::FeatureSelectAsymmetricAdaBoostBodyC(RealT weight, UIntT numFeatures) :
      m_weight(weight), m_numFeatures(numFeatures)
  {
  }

  //: Load from stream.
  
  FeatureSelectAsymmetricAdaBoostBodyC::FeatureSelectAsymmetricAdaBoostBodyC(istream &strm) :
      FeatureSelectorBodyC(strm)
  {
    IntT version;
    strm >> version;
    if (version != 0)
      throw ExceptionOutOfRangeC("FeatureSelectAsymmetricAdaBoostBodyC(istream &), Unrecognised version number in stream. ");
    strm >> m_weight >> m_numFeatures;
  }
  
  //: Load from binary stream.
  
  FeatureSelectAsymmetricAdaBoostBodyC::FeatureSelectAsymmetricAdaBoostBodyC(BinIStreamC &strm) :
      FeatureSelectorBodyC(strm)
  {
    IntT version;
    strm >> version;
    if (version != 0)
      throw ExceptionOutOfRangeC("FeatureSelectAsymmetricAdaBoostBodyC(BinIStreamC &), Unrecognised version number in stream. ");
    strm >> m_weight >> m_numFeatures;
  }
  
  //: Writes object to stream, can be loaded using constructor
  
  bool FeatureSelectAsymmetricAdaBoostBodyC::Save(ostream &out) const
  {
    if (!FeatureSelectorBodyC::Save(out))
      return false;
    IntT version = 0;
    out << version << ' ' << m_weight << ' ' << m_numFeatures;
    return true;
  }
  
  //: Writes object to stream, can be loaded using constructor
  
  bool FeatureSelectAsymmetricAdaBoostBodyC::Save(BinOStreamC &out) const
  {
    if (!FeatureSelectorBodyC::Save(out))
      return false;
    IntT version = 0;
    out << version << m_weight << m_numFeatures;
    return true;
  }

  SArray1dC<IndexC> FeatureSelectAsymmetricAdaBoostBodyC::SelectFeatures(DesignClassifierSupervisedC &designer,
      const DataSetVectorLabelC &train,
      const DataSetVectorLabelC &test,
      ClassifierC &classifier) const
  {
    SArray1dC<ClassifierC> weakClassifiers(0);
    SArray1dC<RealT> classifierWeights(0);
    SArray1dC<ClassifierC> oneClassifier(1);
    SArray1dC<IndexC> featureSet(0);
    SArray1dC<RealT> oneWeight(1);
    SArray1dC<IndexC> oneFeature(1);

    RealT sqrtk = Sqrt(m_weight / (1.0 - m_weight));
    Vector2dC multiplier(exp(-log(sqrtk) / m_numFeatures), exp(log(sqrtk) / m_numFeatures));
    // initialise the weights
    VectorC weights(train.Size());
    Vector2dC sampleCount;
    sampleCount.Fill(0.0);
    for (SampleIterC<UIntT> it1(train.Sample2()); it1; it1++)
      sampleCount[*it1]++;
    for (SampleIterC<UIntT> it2(train.Sample2()); it2; it2++)
      weights[it2.Index()] = 1.0 / (2.0 * sampleCount[*it2]) * multiplier[*it2];
    // for each feature that is added
    for (UIntT t = 0; t < m_numFeatures; t++) {
      RavlDebug("Selecting feature (%d/%d)", t, m_numFeatures);
      weights = NormaliseWeights(weights);
      IndexC totalFeatures = train.Sample1()[0].Size();
      // for each feature, train a classifier and evaluate error
      ClassifierC bestClassifier;
      SArray1dC<IndexC> bestFeature(1);
      RealT lowestError = 1e30;
      for (IndexC j = 0; j < totalFeatures; j++) {
        if (AlreadyUsed(featureSet, j))
          continue;
        oneFeature[0] = j;
        ClassifierC weakClassifier = designer.Apply(train.Sample1(), train.Sample2(), oneFeature, weights);
        RealT error = 0.0 + 1e-6;
        SArray1dIterC<RealT> itw(weights);
#if 0
        // KM Note: This makes no sense to me to replace with other data
        // as you are weighting the datapoints for training!
        // Also, if train.Size() != test.Size() then it will seg fault as
        // weights index will be different.
        SampleIterC<VectorC> itf(test.Sample1());
        SampleIterC<UIntT> itl(test.Sample2());
#else
        SampleIterC<VectorC> itf(train.Sample1());
        SampleIterC<UIntT> itl(train.Sample2());
#endif
        for (; itw; itw++, itf++, itl++) {
          error += *itw * (weakClassifier.Classify(*itf, oneFeature) != *itl);
        }
        if (lowestError > error) {
          lowestError = error;
          bestClassifier = weakClassifier;
          bestFeature[0] = oneFeature[0];
        }
      }
      // update the strong classifier by adding the newest weak classifier
      RealT betaT = lowestError / (1.0 - lowestError);
      RealT alphaT = log(1.0 / betaT);
      if (bestClassifier.IsValid()) {
        oneClassifier[0] = bestClassifier;
        oneWeight[0] = alphaT;
        weakClassifiers.Append(oneClassifier);
        classifierWeights.Append(oneWeight);
        featureSet.Append(bestFeature);
      } else {
        RavlWarning("Didn't assign classifier");
      }
      classifier = ClassifierLinearCombinationC(weakClassifiers, classifierWeights, 0.5);
      // update the weights
      for (IndexC i = 0; i < train.Size(); i++) {
        RealT ei = classifier.Classify(train.Sample1()[i], featureSet) != train.Sample2()[i];
        weights[i] *= Pow(betaT, 1.0 - ei) * multiplier[train.Sample2()[i]];
      }
      RavlDebug(" - best Feature %s with error %0.4f", StringOf(bestFeature[0]).data(), lowestError);
    }

    RealT bestTh = 1.0;
    RealT bestRate = 0.0;
    for (RealT th = 0.9; th > 0.0; th -= 0.025) {
      classifier = ClassifierLinearCombinationC(weakClassifiers, classifierWeights, th);
      // determine weighted success rates
      Vector2dC sums(0.0, 0.0);
      SampleIterC<VectorC> itf(test.Sample1());
      SampleIterC<UIntT> itl(test.Sample2());
      for (; itf; itf++, itl++) {
        if (classifier.Classify(*itf, featureSet) == *itl)
          sums[*itl]++;
      }
      sums[0] /= sampleCount[0];
      sums[1] /= sampleCount[1];
      //cout << sums << endl;
      sums[0] *= 1.0 - m_weight;
      sums[1] *= m_weight;
      //cout << sums << endl;
      if (sums[0] + sums[1] > bestRate + 1e-4) {
        bestRate = sums[0] + sums[1];
        bestTh = th;
      }
      //cout << bestRate << " " << bestTh << endl;
      classifier = ClassifierLinearCombinationC(weakClassifiers, classifierWeights, bestTh);
    }
    return featureSet;
  }

  bool FeatureSelectAsymmetricAdaBoostBodyC::AlreadyUsed(const SArray1dC<IndexC> &featureSet, IndexC feature) const
  {
    for (SArray1dIterC<IndexC> i(featureSet); i; i++)
      if (*i == feature)
        return true;
    return false;
  }
  
  VectorC FeatureSelectAsymmetricAdaBoostBodyC::NormaliseWeights(const VectorC &weights) const
  {
    VectorC out(weights.Size());
    RealT sum = weights.Sum();
    for (SArray1dIter2C<RealT, RealT> it(weights, out); it; it++)
      it.Data2() = it.Data1() / sum;
    return out;
  }
  
  RAVL_INITVIRTUALCONSTRUCTOR_FULL(FeatureSelectAsymmetricAdaBoostBodyC, FeatureSelectAsymmetricAdaBoostC, FeatureSelectorC);

}
