// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2002, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//! lib=RavlPatternRec
//! file="Ravl/PatternRec/Performance/Error.cc"

#include "Ravl/PatternRec/ErrorBinaryClassifier.hh"
#include "Ravl/VirtualConstructor.hh"
#include "Ravl/PatternRec/DataSet2Iter.hh"
#include "Ravl/PatternRec/FuncSubset.hh"
#include "Ravl/Sums1d2.hh"
#include "Ravl/Plot/Plot2d.hh"

namespace RavlN {

  //: Constructor.
  bool compare_score_descend(const Tuple2C<UIntT, RealT> & el1, const Tuple2C<UIntT, RealT> & el2)
  {
    return (el1.Data2() > el2.Data2());
  }

  //: Constructor.
  ErrorBinaryClassifierBodyC::ErrorBinaryClassifierBodyC(UIntT positiveLabel) :
      ErrorBodyC(), m_positiveLabel(positiveLabel)
  {
  }

  //: XML factory constructor
  ErrorBinaryClassifierBodyC::ErrorBinaryClassifierBodyC(const XMLFactoryContextC &factory) :
      ErrorBodyC(factory), m_positiveLabel(factory.AttributeInt("postiveLabel", 0))
  {
  }

  //: Load from stream.
  
  ErrorBinaryClassifierBodyC::ErrorBinaryClassifierBodyC(istream &strm) :
      ErrorBodyC(strm)
  {
    strm >> m_positiveLabel;
  }
  
  //: Load from binary stream.
  
  ErrorBinaryClassifierBodyC::ErrorBinaryClassifierBodyC(BinIStreamC &strm) :
      ErrorBodyC(strm)
  {
    strm >> m_positiveLabel;
  }

  //: Copy me.
  RavlN::RCBodyVC &ErrorBinaryClassifierBodyC::Copy() const
  {
    return *new ErrorBinaryClassifierBodyC(*this);
  }

  //: Writes object to stream, can be loaded using constructor
  
  bool ErrorBinaryClassifierBodyC::Save(std::ostream &out) const
  {
    if (!ErrorBodyC::Save(out))
      return false;
    out << m_positiveLabel << '\n';
    return true;
  }
  
  //: Writes object to stream, can be loaded using constructor
  
  bool ErrorBinaryClassifierBodyC::Save(BinOStreamC &out) const
  {
    if (!ErrorBodyC::Save(out))
      return false;
    out << m_positiveLabel;
    return true;
  }
  
  SArray1dC<Tuple2C<UIntT, RealT> > ErrorBinaryClassifierBodyC::Scores(const ClassifierC & classifier,
      const DataSetVectorLabelC & dset) const
  {

    // Collect the scores
    SArray1dC<Tuple2C<UIntT, RealT> > scores(dset.Size());
    UIntT c = 0;
    Sums1d2C positiveScores;
    Sums1d2C negativeScores;
    for (DataSet2IterC<SampleVectorC, SampleLabelC> it(dset); it; it++) {
      RealT score = classifier.LabelProbability(it.Data1(), m_positiveLabel);
      scores[c] = Tuple2C<UIntT, RealT>(it.Data2(), score);
      c++;
      if (it.Data2() == m_positiveLabel) {
        positiveScores += score;
      } else {
        negativeScores += score;
      }
    }

    // lets that the positive scores are higher than the negative
    if (negativeScores.Mean() > positiveScores.Mean()) {
      RavlError("Mean of negative scores are higher than mean of negative scores.  This assumes otherwise!");
    }

    // Sort scores so high is at the top
    scores.QuickSort(compare_score_descend);
    return scores;

  }

  RealT ErrorBinaryClassifierBodyC::Error(const ClassifierC & classifier, const DataSetVectorLabelC & dset) const
  {
    // Get the sorted scores
    SArray1dC<Tuple2C<UIntT, RealT> > scores = Scores(classifier, dset);

    // Lets compute error rate
    SArray1dC<UIntT> labelSums = dset.ClassNums();
    RealT labelSums0 = (RealT) labelSums[0];
    RealT labelSums1 = (RealT) labelSums[1];
    SArray1dC<UIntT> labelSoFar(2);
    labelSoFar.Fill(0);

    // we assume a high score for label 0
    RealT minError = RavlConstN::maxReal;
    for (SArray1dIterC<Tuple2C<UIntT, RealT> > it(scores); it; it++) {

      UIntT label = it.Data().Data1();
      //RealT score = it.Data().Data2();

      labelSoFar[label]++; // increase label count

      RealT errorLabel0 = (labelSums0 - (RealT) labelSoFar[0]) / labelSums0;
      RealT errorLabel1 = (RealT) labelSoFar[1] / labelSums1;

      RealT error = Abs(errorLabel0 - errorLabel1);
      if (error < minError) {
        minError = error;
      }

      //RavlInfo("Threshold %0.4f ErrorLabel0 %0.4f ErrorLabel1 %0.4f", score, errorLabel0, errorLabel1);

    }

    return minError;
  }
  
  RealT ErrorBinaryClassifierBodyC::Error(const ClassifierC & classifier,
      const DataSetVectorLabelC & dset,
      bool falseAccept,
      RealT desiredError,
      RealT & threshold) const
  {

    // Get the sorted scores
    SArray1dC<Tuple2C<UIntT, RealT> > scores = Scores(classifier, dset);

    // Lets compute error rate
    SArray1dC<UIntT> labelSums = dset.ClassNums();
    RealT positiveSums;
    RealT negativeSums;

    if (m_positiveLabel == 0) {
      positiveSums = (RealT) labelSums[0];
      negativeSums = (RealT) labelSums[1];
    } else {
      positiveSums = (RealT) labelSums[1];
      negativeSums = (RealT) labelSums[0];
    }

    // We start with the high scores and go down
    // Therefore the false accept rate will start at 0 and go up
    // and the false reject rate with start at 1.0 and go down
    RealT falseRejectRate = -1.0;
    RealT falseAcceptRate = -1.0;
    RealT positiveSoFar = 0.0;
    RealT negativeSoFar = 0.0;
    CollectionC<Point2dC> points(scores.Size());
    for (SArray1dIterC<Tuple2C<UIntT, RealT> > it(scores); it; it++) {

      UIntT label = it.Data().Data1();
      RealT score = it.Data().Data2();

      // have we got a positive or negative sample
      if (label == m_positiveLabel) {
        positiveSoFar += 1.0;
      } else {
        negativeSoFar += 1.0;
      }

      /*
       * We can work out the current FA/FR rates
       */
      falseRejectRate = (positiveSums - positiveSoFar) / positiveSums;
      falseAcceptRate = negativeSoFar / negativeSums;

      //RavlInfo("Label %d Score %0.4f FA %0.4f FR %0.4f", label, score, falseAcceptRate, falseRejectRate);

      // The false reject rate is going down

      if (falseAccept) {
        // we want the false reject rate
        if (falseAcceptRate >= desiredError) {
          threshold = score;
          return falseRejectRate;
        }
      } else {
        if (falseRejectRate <= desiredError) {
          threshold = score;
          return falseAcceptRate;
        }
      }
    }

    return 1.0;
  }

  RealT ErrorBinaryClassifierBodyC::FalseRejectRate(const ClassifierC & classifier,
      const DataSetVectorLabelC & dset,
      RealT falseAcceptRate,
      RealT & threshold) const
  {
    return Error(classifier, dset, true, falseAcceptRate, threshold);
  }
  //: Return the false reject rate at a given false accept rate.  This assumes label 0 is the positive class.

  RealT ErrorBinaryClassifierBodyC::FalseAcceptRate(const ClassifierC & classifier,
      const DataSetVectorLabelC & dset,
      RealT falseRejectRate,
      RealT & threshold) const
  {
    return Error(classifier, dset, false, falseRejectRate, threshold);
  }
  //: Return the false accept rate at a given false reject rate.  This assumes label 0 is the positive class.

  bool ErrorBinaryClassifierBodyC::Plot(const ClassifierC & classifier, const DataSetVectorLabelC & dset) const
  {

    std::cerr << "plotting..." << std::endl;

    // Get the sorted scores
    SArray1dC<Tuple2C<UIntT, RealT> > scores = Scores(classifier, dset);

    // Lets compute error rate
    SArray1dC<UIntT> labelSums = dset.ClassNums();
    RealT positiveSums;
    RealT negativeSums;

    if (m_positiveLabel == 0) {
      positiveSums = (RealT) labelSums[0];
      negativeSums = (RealT) labelSums[1];
    } else {
      positiveSums = (RealT) labelSums[1];
      negativeSums = (RealT) labelSums[0];
    }

    // We start with the high scores and go down
    // Therefore the false accept rate will start at 0 and go up
    // and the false reject rate with start at 1.0 and go down
    RealT falseRejectRate = -1.0;
    RealT falseAcceptRate = -1.0;
    RealT positiveSoFar = 0.0;
    RealT negativeSoFar = 0.0;
    CollectionC<Point2dC> points(scores.Size());
    for (SArray1dIterC<Tuple2C<UIntT, RealT> > it(scores); it; it++) {

      UIntT label = it.Data().Data1();
      //RealT score = it.Data().Data2();

      // have we got a positive or negative sample
      if (label == m_positiveLabel) {
        positiveSoFar += 1.0;
      } else {
        negativeSoFar += 1.0;
      }

      /*
       * We can work out the current FA/FR rates
       */
      falseRejectRate = (positiveSums - positiveSoFar) / positiveSums;
      falseAcceptRate = negativeSoFar / negativeSums;
      points.Append(Point2dC(falseAcceptRate, falseRejectRate));
      //RavlInfo("Label %d Score %0.4f FA %0.4f FR %0.4f", label, score, falseAcceptRate, falseRejectRate);

    }

    /*
     * Make a plot of the ROC
     */
    RavlDebug("Making plot!");
    Plot2dC::RefT plot = CreatePlot2d("ROC Curve");
    plot->SetLineStyle("lines");
    plot->SetXLabel("False Acceptance");
    plot->SetYLabel("False Acceptance");
    plot->Plot(points.SArray1d());

    return 1.0;
  }

  RAVL_INITVIRTUALCONSTRUCTOR(ErrorBinaryClassifierBodyC);

}
