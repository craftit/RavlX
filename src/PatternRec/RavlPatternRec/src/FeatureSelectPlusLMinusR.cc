// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//! rcsid="$Id$"
//! lib=RavlPatternRec
//! file="Ravl/PatternRec/FeatureSelection/FeatureSelectPlusLMinusR.cc"

#include "Ravl/PatternRec/FeatureSelectPlusLMinusR.hh"
#include "Ravl/VirtualConstructor.hh"
#include "Ravl/BinStream.hh"
#include "Ravl/Vector2d.hh"
#include "Ravl/PatternRec/SampleIter.hh"
#include "Ravl/PatternRec/ClassifierLinearCombination.hh"
#include "Ravl/PatternRec/Error.hh"
#include "Ravl/PatternRec/ErrorBinaryClassifier.hh"
#include "Ravl/Threads/LaunchThread.hh"
#include "Ravl/CallMethodPtrs.hh"
#include "Ravl/SArray1dIter2.hh"
#include "Ravl/PatternRec/DesignDiscriminantFunction.hh"
#include "Ravl/PatternRec/DesignFuncLSQ.hh"
#include "Ravl/XMLFactoryRegister.hh"

#define DODEBUG	0
#if DODEBUG
#define ONDEBUG(x) x
#else
#define ONDEBUG(x)
#endif

namespace RavlN {

  //: Constructor

  FeatureSelectPlusLMinusRBodyC::FeatureSelectPlusLMinusRBodyC(UIntT l, UIntT r, RealT deltaError, UIntT numFeatures, UIntT numThreads) :
      m_l(l), m_r(r), m_deltaError(deltaError), m_numFeatures(numFeatures), m_numberOfThreads(numThreads)
  {
  }

  FeatureSelectPlusLMinusRBodyC::FeatureSelectPlusLMinusRBodyC(const XMLFactoryContextC & factory) :
       m_l(factory.AttributeInt("plusL", 2)),
       m_r(factory.AttributeInt("minusR", 1)),
       m_deltaError(factory.AttributeReal("deltaError", 0.001)),
       m_numFeatures(factory.AttributeInt("features", 10)),
       m_numberOfThreads(factory.AttributeInt("threads", 8))
   {
   }


  //: Load from stream.
  
  FeatureSelectPlusLMinusRBodyC::FeatureSelectPlusLMinusRBodyC(istream &strm) :
      FeatureSelectorBodyC(strm)
  {
    IntT version;
    strm >> version;
    if (version != 0)
      throw ExceptionOutOfRangeC("FeatureSelectPlusLMinusRBodyC(istream &), Unrecognised version number in stream. ");
    strm >> m_l >> m_r >> m_numFeatures >> m_deltaError >> m_numberOfThreads;
  }
  
  //: Load from binary stream.
  
  FeatureSelectPlusLMinusRBodyC::FeatureSelectPlusLMinusRBodyC(BinIStreamC &strm) :
      FeatureSelectorBodyC(strm)
  {
    IntT version;
    strm >> version;
    if (version != 0)
      throw ExceptionOutOfRangeC("FeatureSelectPlusLMinusRBodyC(BinIStreamC &), Unrecognised version number in stream. ");
    strm >> m_l >> m_r >> m_numFeatures >> m_deltaError >> m_numberOfThreads;
  }
  
  //: Writes object to stream, can be loaded using constructor
  
  bool FeatureSelectPlusLMinusRBodyC::Save(ostream &out) const
  {
    if (!FeatureSelectorBodyC::Save(out))
      return false;
    IntT version = 0;
    out << version << ' ' << m_l << ' ' << m_r << ' ' << m_numFeatures << ' ' << m_deltaError << ' ' << m_numberOfThreads;
    return true;
  }
  
  //: Writes object to stream, can be loaded using constructor
  
  bool FeatureSelectPlusLMinusRBodyC::Save(BinOStreamC &out) const
  {
    if (!FeatureSelectorBodyC::Save(out))
      return false;
    IntT version = 0;
    out << version << m_l << m_r << m_numFeatures << m_deltaError << m_numberOfThreads;
    return true;
  }

  bool Criterion(DesignClassifierSupervisedC & design,
      const DataSetVectorLabelC & train,
      const DataSetVectorLabelC & test,
      const SArray1dC<IndexC> & featureSet,
      Tuple2C<ClassifierC, RealT> * result)
  {
    ClassifierC classifier = design.Apply(train.Sample1(), train.Sample2(), featureSet);

    RealT pmc = -1.0;
#if 0
    if (classifier.NumLabels() == 2) {
      ErrorBinaryClassifierC error;
      RealT threshold;
      RealT falseRejectRate = 0.001; // FIXME: Should not be hardwired here!
      pmc = error.FalseAcceptRate(classifier,
          DataSetVectorLabelC(SampleVectorC(test.Sample1(), featureSet.SArray1d()), test.Sample2()),
          falseRejectRate,
          threshold);
    } else {
      ErrorC error;
      pmc = error.Error(classifier, test, featureSet.SArray1d());
    }
#else
    ErrorC error;
    pmc = error.Error(classifier, test, featureSet.SArray1d());
#endif

    result->Data1() = classifier;
    result->Data2() = pmc;
    return true;
  }

  SArray1dC<IndexC> FeatureSelectPlusLMinusRBodyC::SelectFeatures(DesignClassifierSupervisedC &design,
      const DataSetVectorLabelC &train,
      const DataSetVectorLabelC &test,
      ClassifierC & bestClassifier) const
  {



    // do some checks
    if (m_l <= m_r) {
      RavlError("l must be bigger than r");
      return SArray1dC<IndexC>();
    }

    // The feature set we currently have
    SizeT numberOfFeatures = train.Sample1().First().Size();

    ONDEBUG(RavlDebug("Plus %d Minus %d Feature Selection with number of features %s", m_l, m_r, StringOf(numberOfFeatures).data()));

    CollectionC<IndexC> featureSetRemaining(numberOfFeatures);
    for (IndexC i = 0; i < numberOfFeatures; i++) {
      featureSetRemaining.Append(i);
    }

    // Where we want to get to
    CollectionC<IndexC> featureSet(m_numFeatures);
    bool done = false;
    RealT previousError = 1.0;

    while (!done) {

      RealT currentError = RavlConstN::maxReal;

      /*
       * Go forwards
       */

      for (UIntT l = 0; l < m_l; l++) {
        RealT lowestError = RavlConstN::maxReal;
        IndexC bestFeature(-1);
        IndexC bestFeatureIndex(-1);

        // Iterate through all the remaining features to
        // find the best one
        CollectionIterC<IndexC> it(featureSetRemaining);

        for (;;) {

          SArray1dC<ClassifierC> classifiers(m_numberOfThreads);
          SArray1dC<RealT> pmc(m_numberOfThreads);
          CollectionC<LaunchThreadC> threads(m_numberOfThreads);
          CollectionC<Tuple2C<IndexC, IndexC> > feature(m_numberOfThreads);
          CollectionC<Tuple2C<ClassifierC, RealT> > results(m_numberOfThreads);

          for (UIntT i = 0; i < m_numberOfThreads; i++) {

            // Append feature to set
            featureSet.Append(*it);

            // Run the criterion function on a thread
            TriggerC trig = Trigger(&Criterion, design, train, test, featureSet.SArray1d().Copy(), &results[i]);

            //TriggerC trig = Trigger(&Criterion2);
            threads.Append(LaunchThreadC(trig));

            feature.Append(Tuple2C<IndexC, IndexC>(it.Index(), *it));
            //  Delete the feature from the list
            featureSet.Delete(featureSet.Size() - 1);

            it++; // increase feature

            // Is it a valid feature?
            if (!it.IsElm()) {
              // OK we need to finish pronto!
              break;
            }

          }

          // Lets wait for all threads to finish
          for (CollectionIterC<LaunchThreadC> threadIt(threads); threadIt; threadIt++) {
            threadIt.Data().WaitForExit();
          }

          for (SArray1dIter2C<Tuple2C<IndexC, IndexC>, Tuple2C<ClassifierC, RealT> > resIt(feature.SArray1d(), results.SArray1d()); resIt;
              resIt++) {
            RealT pmc = resIt.Data2().Data2();
            if (pmc < lowestError) {
              lowestError = pmc;
              bestClassifier = resIt.Data2().Data1();
              bestFeature = resIt.Data1().Data2();
              bestFeatureIndex = resIt.Data1().Data1();
            }
          }

          if (!it.IsElm()) {
            break;
          }
        } // end feature it

        //ONDEBUG(RavlInfo("Adding feature '%s' with lowest error of %0.4f", StringOf(bestFeature).data(), lowestError));
        featureSet.Append(bestFeature);
        featureSetRemaining.Delete(bestFeatureIndex);
        currentError = lowestError;
      } // end l

      /*
       * Go backwards
       */

      for (UIntT r = 0; r < m_r; r++) {

        IndexC bestRemovedFeature(-1);
        IndexC bestRemovedFeatureIndex(-1);
        RealT lowestError = RavlConstN::maxReal;

        UIntT useThreads = m_numberOfThreads;
        if(featureSet.Size() <= useThreads) {
          useThreads = featureSet.Size();
        }

        // Iterate through all the remaining features to
        // find the best one
        CollectionIterC<IndexC> it(featureSet);

        for (;;) {

          SArray1dC<ClassifierC> classifiers(useThreads);
          SArray1dC<RealT> pmc(useThreads);
          CollectionC<LaunchThreadC> threads(useThreads);
          CollectionC<Tuple2C<IndexC, IndexC> > feature(useThreads);
          CollectionC<Tuple2C<ClassifierC, RealT> > results(useThreads);

          for (UIntT i = 0; i < useThreads; i++) {

            // Make a copy of all the remaing features
            CollectionC<IndexC> remove = featureSet.Copy();

            // Remove the one we are testing
            remove.Delete(it.Index());

            // Run the criterion function on a thread
            TriggerC trig = Trigger(&Criterion, design, train, test, remove.SArray1d(), &results[i]);
            //TriggerC trig = Trigger(&Criterion2);
            threads.Append(LaunchThreadC(trig));
            feature.Append(Tuple2C<IndexC, IndexC>(it.Index(), *it));

            it++; // increase feature

            // Is it a valid feature?
            if (!it.IsElm()) {
              // OK we need to finish pronto!
              break;
            }

          }

          // Lets wait for all threads to finish
          for (CollectionIterC<LaunchThreadC> threadIt(threads); threadIt; threadIt++) {
            threadIt.Data().WaitForExit();
          }

          for (SArray1dIter2C<Tuple2C<IndexC, IndexC>, Tuple2C<ClassifierC, RealT> > resIt(feature.SArray1d(), results.SArray1d()); resIt;
              resIt++) {
            RealT pmc = resIt.Data2().Data2();
            if (pmc < lowestError) {
              lowestError = pmc;
              bestClassifier = resIt.Data2().Data1();
              bestRemovedFeature = resIt.Data1().Data2();
              bestRemovedFeatureIndex = resIt.Data1().Data1();
            }
          }

          if (!it.IsElm()) {
            break;
          }
        } // end feature it

        //ONDEBUG(RavlDebug("Removing feature '%s' with lowest error of %0.4f", StringOf(bestRemovedFeature).data(), lowestError));
        featureSet.Delete(bestRemovedFeatureIndex); // delete from master list
        featureSetRemaining.Append(bestRemovedFeature); // add back to the pool
        currentError = lowestError;
      }

      /*
       * Finished iteration of Plus L Minus R
       */

      RealT thisDeltaError = previousError - currentError;
      ONDEBUG(RavlInfo("Currently have %s features with performance %0.4f, delta %0.4f (%0.4f)", StringOf(featureSet.Size()).data(), currentError, thisDeltaError, m_deltaError));
      if (thisDeltaError <= m_deltaError) {
        RavlInfo("Delta Error not large enough!  Finishing feature selection early!");
        done = true;
      }
      previousError = currentError;

      // Have we met our finish criterion??
      // FIXME: Should be able to specify performance as well
      if (featureSet.Size() >= m_numFeatures) {
        ONDEBUG(RavlInfo("Maximum number of features reached. Finished feature selection!"));
        done = true;
      }

    }

    return featureSet.SArray1d();
  }
  RavlN::XMLFactoryRegisterHandleConvertC<FeatureSelectPlusLMinusRC, FeatureSelectorC> g_registerXMLFactoryFeatureSelectPlusLMinusR("RavlN::FeatureSelectPlusLMinusRC");

  RAVL_INITVIRTUALCONSTRUCTOR_FULL(FeatureSelectPlusLMinusRBodyC, FeatureSelectPlusLMinusRC, FeatureSelectorC);

}
