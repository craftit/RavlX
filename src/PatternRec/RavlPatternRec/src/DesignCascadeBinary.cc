// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2003, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//! rcsid="$Id: DesignCascadeBinary.cc 7590 2010-02-23 12:03:11Z kier $"
//! lib=RavlPatternRec
//! file="Ravl/PatternRec/Classify/DesignCascadeBinary.cc"

#include "Ravl/PatternRec/DesignCascadeBinary.hh"
#include "Ravl/PatternRec/ClassifierOneAgainstAll.hh"
#include "Ravl/VirtualConstructor.hh"
#include "Ravl/BinStream.hh"
#include "Ravl/PatternRec/DataSetVectorLabel.hh"
#include "Ravl/SArray1dIter2.hh"
#include "Ravl/XMLFactoryRegister.hh"
#include "Ravl/Collection.hh"
#include "Ravl/PatternRec/DataSet2Iter.hh"
#include "Ravl/PatternRec/DataSet3Iter.hh"
#include "Ravl/PatternRec/FuncSubset.hh"
#include "Ravl/PatternRec/ClassifierCascade.hh"
#include "Ravl/PatternRec/Error.hh"
#include "Ravl/PatternRec/FeatureSelectPlusLMinusR.hh"
#include "Ravl/PatternRec/FeatureSelectAsymmetricAdaBoost.hh"
#include "Ravl/PatternRec/ErrorBinaryClassifier.hh"

namespace RavlN {
  
  //: Constructor.
  
  DesignCascadeBinaryBodyC::DesignCascadeBinaryBodyC(const DesignClassifierSupervisedC & design,
      const FeatureSelectorC & featureSelector,
      UIntT maxStages,
      RealT targetErrorRate) :
      DesignCascadeBodyC(design), m_featureSelector(featureSelector), m_maxStages(maxStages), m_targetErrorRate(targetErrorRate)
  {
  }
  
  DesignCascadeBinaryBodyC::DesignCascadeBinaryBodyC(const XMLFactoryContextC & factory) :
      DesignCascadeBodyC(factory),
      m_maxStages(factory.AttributeInt("maxStages", 10)),
      m_targetErrorRate(factory.AttributeReal("targetErrorRate", 0.01))
  {
    if (!factory.UseComponent("FeatureSelection", m_featureSelector)) {
      throw ExceptionC("Unable to load feature selector from XML config file.");
    }
    RavlInfo("Cascade Binary Stages %d and target error rate of %0.4f", m_maxStages, m_targetErrorRate);
  }
  
  //: Load from stream.
  
  DesignCascadeBinaryBodyC::DesignCascadeBinaryBodyC(std::istream &strm) :
      DesignCascadeBodyC(strm)
  {
    int version;
    strm >> version;
    if (version != 0)
      throw ExceptionOutOfRangeC("DesignCascadeBinaryBodyC::DesignCascadeBinaryBodyC(std::istream &), Unrecognised version number in stream. ");
    strm >> m_featureSelector >> m_maxStages >> m_targetErrorRate;
  }
  
  //: Load from binary stream.
  
  DesignCascadeBinaryBodyC::DesignCascadeBinaryBodyC(BinIStreamC &strm) :
      DesignCascadeBodyC(strm)
  {
    int version;
    strm >> version;
    if (version != 0)
      throw ExceptionOutOfRangeC("DesignCascadeBinaryBodyC::DesignCascadeBinaryBodyC(BinIStreamC &), Unrecognised version number in stream. ");
    strm >> m_featureSelector >> m_maxStages >> m_targetErrorRate;
  }
  
  //: Writes object to stream, can be loaded using constructor
  
  bool DesignCascadeBinaryBodyC::Save(std::ostream &out) const
  {
    if (!DesignCascadeBodyC::Save(out))
      return false;
    int version = 0;
    out << ' ' << version << ' ' << m_featureSelector << ' ' << m_maxStages << ' ' << m_targetErrorRate;
    return true;
  }
  
  //: Writes object to stream, can be loaded using constructor
  
  bool DesignCascadeBinaryBodyC::Save(BinOStreamC &out) const
  {
    if (!DesignCascadeBodyC::Save(out))
      return false;
    int version = 0;
    out << version << m_featureSelector << m_maxStages << m_targetErrorRate;
    return true;
  }
  
  ClassifierC DesignCascadeBinaryBodyC::Apply(const DataSetVectorLabelC & trainingSet, const DataSetVectorLabelC & validationSet)
  {
#if 1

    /*
     * A simple Cascade classifier in which the FP's of a previous round
     * are used to train the stage.....We strive to get to a low FA rate and
     * kinda hope the FR is not too high :-)
     * It is good for highly assymetric data sets.
     */
    RavlInfo("Designing Cascade Classifier!");

    RavlAssertMsg(trainingSet.Size() == trainingSet.Size(),
        "DesignCascadeBinaryBodyC::Apply(), Sample of vector and labels should be the same size.");
    RavlAssertMsg(trainingSet.Sample2().LabelSums().Size() == 2, "DesignCascadeBinaryBodyC::Apply() only supports two classes.");

    RavlInfo("Starting to design CascadeBinary classifier with stages %d and target error %0.4f", m_maxStages, m_targetErrorRate);

    SArray1dC<SampleVectorC> trainingSamples = trainingSet.SeperateLabels();
    SampleVectorC designVectors = trainingSet.Sample1();
    SampleLabelC designLabels = trainingSet.Sample2();

    SampleVectorC validationVectors = validationSet.Sample1();
    SampleLabelC validationLabels = validationSet.Sample2();

    SArray1dC<UIntT> labelSums = validationLabels.LabelSums();

    CollectionC<ClassifierC> classifiers(m_maxStages);
    CollectionC<RealT> thresholds(m_maxStages);
    CollectionC<FuncSubsetC> features(m_maxStages);

    RealT actualErrorRate = 1.0;

    while (m_targetErrorRate < actualErrorRate) {

      RavlInfo(" *** Cascade Layer %d ***", classifiers.Size().V() + 1);

      /*
       * Train node using feature selection algorithm.
       * This will continue until criterion has not changed much,
       * or max features reached.
       */
      SizeT numberOfDimensions = designVectors.First().Size();
      SArray1dC<UIntT> ls = designLabels.LabelSums();
      RavlInfo("Selecting features Label 0: %d Label 1: %d", ls[0], ls[1]);
      ClassifierC bestClassifier;
      SArray1dC<IndexC> selectedFeatures = m_featureSelector.SelectFeatures(m_design,
          DataSetVectorLabelC(designVectors, designLabels),
          DataSetVectorLabelC(validationVectors, validationLabels),
          bestClassifier);
      FuncSubsetC funcSubset(selectedFeatures, numberOfDimensions);
      ///bestClassifier = ClassifierPreprocessC(FuncSubsetC(selectedFeatures, numberOfDimensions), bestClassifier);
      classifiers.Append(bestClassifier);
      features.Append(funcSubset);

      /*
       * In a stage classifier we want to make sure that the
       * false rejection is actually very low...so we want to chose a threshold
       * for the classifier appropriately.  We can do this by setting a threshold
       * in which the FA rate is very high
       */
      ErrorBinaryClassifierC errorBinaryClassifier;
      DataSetVectorLabelC dset(SampleVectorC(validationVectors, selectedFeatures), validationLabels);
      RealT threshold = -1.0;
      RealT falseAcceptRate = 0.5;
      RealT falseRejectRate = errorBinaryClassifier.FalseRejectRate(bestClassifier, dset, falseAcceptRate, threshold);
      RavlInfo("Stage classifier, FA: %0.4f FR %0.4f at Threshold %0.4f", falseAcceptRate, falseRejectRate, threshold);
      thresholds.Append(threshold);

      /*
       * Now we build the cascade
       */
      ClassifierCascadeC cascade(classifiers.SArray1d(), thresholds.SArray1d(), features.SArray1d());

      /*
       * Now we need to find the training data which is not correctly classified
       */
      SampleVectorC falsePositiveVecs(validationSet.Size());
      SArray1dC<UIntT> labelSums(2);
      labelSums.Fill(0);
      SArray1dC<UIntT> errorCount(4); //TP, FR, TN, FA
      errorCount.Fill(0);
      for (DataSet2IterC<SampleVectorC, SampleLabelC> it(validationSet.Sample1(), validationSet.Sample2()); it; it++) {

        UIntT label = cascade.Classify(it.Data1());

        // It is label 0
        if (it.Data2() == 0) {
          // we always want to add the training vectors, whether right or wrong
          if (label == 0) {
            errorCount[0]++;
          } else {
            errorCount[1]++;
          }
        }

        // It is label 1
        else {
          // Here we only want the mis-classified examples for the next stage training.
          if (label == 1) {
            errorCount[2]++;
          } else {
            falsePositiveVecs.Append(it.Data1());
            errorCount[3]++;
          }
        }

      }

      // Re-set the training data
      DataSetVectorLabelC newTrainingDataSet(trainingSet.Size());
      newTrainingDataSet.Append(trainingSamples[0], 0);
      newTrainingDataSet.Append(falsePositiveVecs, 1);
      SArray1dC<UIntT> sums = newTrainingDataSet.Sample2().LabelSums();
      if (sums[0] < sums[1]) {
        newTrainingDataSet = newTrainingDataSet.ExtractPerLabel(sums[0]); // equalise to the number of training samples
      }
      designVectors = newTrainingDataSet.Sample1();
      designLabels = newTrainingDataSet.Sample2();

      RavlInfo("Positive Examples %d Negative Examples %d", sums[0], sums[1]);
      /*
       * For our actual error rate we are measuring the level of false acceptances....
       */
      RealT tp = (RealT) errorCount[0] / ((RealT) errorCount[0] + (RealT) errorCount[1]);
      RealT fr = (RealT) errorCount[1] / ((RealT) errorCount[0] + (RealT) errorCount[1]);
      RealT tn = (RealT) errorCount[2] / ((RealT) errorCount[2] + (RealT) errorCount[3]);
      RealT fa = (RealT) errorCount[3] / ((RealT) errorCount[2] + (RealT) errorCount[3]);
      RavlInfo("tp %0.4f, fr %0.4f, tn %0.4f, fa %0.4f", tp, fr, tn, fa);

      // we are using fr as
      actualErrorRate = fa;

      RavlInfo("Cascade error rate %0.4f (the false positive rate)", actualErrorRate);

      if (classifiers.Size() > m_maxStages) {
        RavlInfo("Max Stages recached. Leaving cascade training before error is met!");
        break;
      }

    }

    return ClassifierCascadeC(classifiers.SArray1d(), thresholds.SArray1d(), features.SArray1d());
#else
    return ClassifierC();
#endif

  }
  //: Create a classifier from training and validation set
  
  //////////////////////////////////////////////////////////
  RavlN::XMLFactoryRegisterHandleConvertC<DesignCascadeBinaryC, DesignCascadeC> g_registerXMLFactoryDesignCascadeBinary("RavlN::DesignCascadeBinaryC");

  RAVL_INITVIRTUALCONSTRUCTOR_FULL(DesignCascadeBinaryBodyC, DesignCascadeBinaryC, DesignCascadeC);

  void linkDesignCascadeBinary()
  {
  }

}
