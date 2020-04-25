// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//! lib=RavlPatternRec
//! file="Ravl/PatternRec/DataSet/SampleVector.cc"

#include "Ravl/PatternRec/SampleVector.hh"
#include "Ravl/DArray1dIter.hh"
#include "Ravl/DArray1dIter2.hh"
#include "Ravl/DArray1dIter3.hh"
#include "Ravl/MeanCovariance.hh"
#include "Ravl/MatrixRUT.hh"
#include "Ravl/PatternRec/DataSet2Iter.hh"
#include "Ravl/PatternRec/SampleIter.hh"
#include "Ravl/SArray1dIter2.hh"
#include "Ravl/SumsNd2.hh"
#include "Ravl/VectorMatrix.hh"
#include "Ravl/XMLFactoryRegister.hh"
#include "Ravl/RandomGauss.hh"
#include "Ravl/TMatrix.hh"

namespace RavlN {

  //: Construct from a sample of floats.

  SampleVectorC::SampleVectorC(const SampleC<TVectorC<float> > &svec, const SArray1dC<FieldInfoC> & fieldInfo)
      : SampleC<VectorC>(svec.Size()),
        m_fieldInfo(fieldInfo)
  {
    for (DArray1dIterC<TVectorC<float> > it(svec.DArray()); it; it++)
      Append(VectorC(*it));
  }

  //: Construct a new sample set with a reduced set of features

  SampleVectorC::SampleVectorC(const SampleC<VectorC> &svec,
                                const SArray1dC<IndexC> &featureSet,
                                const SArray1dC<FieldInfoC> & fieldInfo)
      : SampleC<VectorC>(svec.Size()),
        m_fieldInfo(fieldInfo)
  {
    UIntT numFeatures = featureSet.Size();
    for (DArray1dIterC<VectorC> it(svec.DArray()); it; it++) {
      VectorC out(numFeatures);
      for (SArray1dIter2C<IndexC, RealT> itf(featureSet, out); itf; itf++) {
        itf.Data2() = it.Data()[itf.Data1()];
      }
      Append(out);
    }
  }

  SampleVectorC::SampleVectorC(const MeanCovarianceC & meanCovariance)
      : SampleC<VectorC>(Floor(meanCovariance.Number()))
  {
    // Random number generator
    RandomGaussC random;

    // Need to work out roa
    MatrixC evectors;
    VectorC evalues = EigenVectors(meanCovariance.Covariance(), evectors);
    MatrixC di(meanCovariance.Mean().Size(), meanCovariance.Mean().Size());
    di.Fill(0.0);
    for (SArray1dIterC<RealT> it(evalues); it; it++)
      *it = Sqrt(*it);
    di.AddDiagonal(evalues);
    MatrixC rot = evectors * di;

    // lets make the sample, can I remember how?
    for (IntT i = 0; i < Floor(meanCovariance.Number()); i++) {

      // first create unrotated data of mean=0 and variance=1.0
      VectorC vec(meanCovariance.Mean().Size());
      for (SArray1dIterC<RealT> vecIt(vec); vecIt; vecIt++)
        *vecIt = random.Generate();
      vec = (rot * vec) + meanCovariance.Mean();
      Append(vec);
    }
  }

  SampleVectorC::SampleVectorC(const XMLFactoryContextC & factory)
      : SampleC<VectorC>(10000) {

    MeanCovarianceC meanCovariance;
    if (!factory.UseComponent("stats", meanCovariance))
      RavlIssueError("Unable to load stats from XML factory");

    // Random number generator
    RandomGaussC random;

    // Need to work out roa
    MatrixC evectors;
    VectorC evalues = EigenVectors(meanCovariance.Covariance(), evectors);
    MatrixC di(meanCovariance.Mean().Size(), meanCovariance.Mean().Size());
    di.Fill(0.0);
    for (SArray1dIterC<RealT> it(evalues); it; it++)
      *it = Sqrt(*it);
    di.AddDiagonal(evalues);
    MatrixC rot = evectors * di;

    // lets make the sample, can I remember how?
    for (IntT i = 0; i < Floor(meanCovariance.Number()); i++) {

      // first create unrotated data of mean=0 and variance=1.0
      VectorC vec(meanCovariance.Mean().Size());
      for (SArray1dIterC<RealT> vecIt(vec); vecIt; vecIt++)
        *vecIt = random.Generate();
      vec = (rot * vec) + meanCovariance.Mean();
      Append(vec);
    }

  }

  //: Get the size of vectors in this sample.

  UIntT SampleVectorC::VectorSize() const {
    if (IsEmpty())
      return 0;
    return First().Size();
  }

  //: Find the mean vector.

  VectorC SampleVectorC::Mean() const {
    DArray1dIterC<VectorC> it(*this);
    if (!it)
      return VectorC();
    VectorC total = it->Copy();
    for (it++; it; it++)
      total += *it;
    return total / ((RealT) Size());
  }

  //: Find the weighted mean vector of the sample.

  VectorC SampleVectorC::Mean(const SampleC<RealT> &weights) const {
    DArray1dIter2C<VectorC, RealT> it(*this, weights.DArray());
    if (!it)
      return VectorC();
    VectorC total = it.Data1() * it.Data2();
    RealT n = 0;
    for (it++; it; it++) {
      total.MulAdd(it.Data1(), it.Data2());
      n += it.Data2();
    }
    return total / n;
  }

  //: Find the mean and covariance of the sample

  MeanCovarianceC SampleVectorC::MeanCovariance(bool sampleStatistics) const {
    DArray1dIterC<VectorC> it(*this);
    if (!it)
      return MeanCovarianceC();
    SumsNd2C sums(1.0, it->Copy(), OuterProductRUT(*it));
    for (it++; it; it++)
      sums += *it;
    return sums.MeanCovariance(sampleStatistics);
  }

  //: Find the mean and covariance of a weighted sample

  MeanCovarianceC SampleVectorC::MeanCovariance(const SampleC<RealT> &weights, bool sampleStatistics) const {
    RavlAssert(Size() == weights.Size());
    DArray1dIter2C<VectorC, RealT> it(*this, weights.DArray());
    if (!it)
      return MeanCovarianceC();
    SumsNd2C sums(it.Data2(), it.Data1() * it.Data2(), OuterProductRUT(it.Data1(), it.Data2()));
    for (it++; it; it++)
      sums.Add(it.Data1(), it.Data2());
    return sums.MeanCovariance(sampleStatistics);
  }

  //: Compute the sum of the outerproducts.

  MatrixRUTC SampleVectorC::SumOuterProducts() const {
    MatrixRUTC ret;
    DArray1dIterC<VectorC> it(*this);
    if (!it)
      return ret; // No samples.
    ret = OuterProductRUT(*it);
    for (it++; it; it++)
      ret.AddOuterProduct(*it);
    return ret;
  }

  //: Compute the sum of the outerproducts.
  // sam2 must have the same size as this sample vector.

  MatrixC SampleVectorC::TMul(const SampleC<VectorC> &sam2) const {
    RavlAssert(Size() == sam2.Size());
    MatrixC ret;
    DArray1dIter2C<VectorC, VectorC> it(*this, sam2.DArray());
    if (!it)
      return ret; // No samples.
    ret = it.Data1().OuterProduct(it.Data2());
    for (it++; it; it++)
      ret.AddOuterProduct(it.Data1(), it.Data2());
    return ret;
  }

  //: Compute the sum of the outerproducts weighting each with the corresponding value from 'w'.

  MatrixRUTC SampleVectorC::SumOuterProducts(const SampleC<RealT> &w) const {
    RavlAssert(Size() == w.Size());
    MatrixRUTC ret;
    DArray1dIter2C<VectorC, RealT> it(*this, w.DArray());
    if (!it)
      return ret; // No samples.
    ret = OuterProductRUT(it.Data1(), it.Data2());
    for (it++; it; it++)
      ret.AddOuterProduct(it.Data1(), it.Data2());
    return ret;
  }

  //: Compute the sum of the outer products weighting each with the corresponding value from 'w'.
  // sam2 must have the same size as this sample vector.

  MatrixC SampleVectorC::TMul(const SampleC<VectorC> &sam2, const SampleC<RealT> &w) const {
    RavlAssert(sam2.Size() == w.Size());
    RavlAssert(Size() == sam2.Size());
    MatrixC ret;
    DArray1dIter3C<VectorC, VectorC, RealT> it(*this, sam2.DArray(), w.DArray());
    if (!it)
      return ret; // No samples.
    ret = it.Data1().OuterProduct(it.Data2(), it.Data3());
    for (it++; it; it++)
      ret.AddOuterProduct(it.Data1(), it.Data2(), it.Data3());
    return ret;
  }



  void SampleVectorC::Normalise(const MeanCovarianceC & stats) {
    if (stats.Number() < 2) {
      RavlAssertMsg(0,"Degerate stat's");
      return; // Can't normalise only 1 sample!
    }
    UIntT d = VectorSize();
    VectorC stdDev(d);
    for (UIntT i = 0; i < d; i++) {
      if (stats.Covariance()[i][i] > 0)
        stdDev[i] = stats.Covariance()[i][i];
      else
        stdDev[i] = 1.0;
    }
    for (UIntT i = 0; i < d; i++)
      stdDev[i] = Sqrt(stdDev[i]);
    stdDev.Reciprocal();

    for (SampleIterC<VectorC> it(*this); it; it++)
      *it = (*it - stats.Mean()) * stdDev;
  }

  //: Apply a function in place
  void SampleVectorC::Normalise(const FunctionC & func) {
    for (SampleIterC<VectorC> it(*this); it; it++) {
      *it = func.Apply(*it);
    }
  }


  //: Undo the normalisation done by 'Normalise()'.

  void SampleVectorC::UndoNormalisation(const MeanCovarianceC & stats) {
    if (stats.Number() < 2) {
      RavlAssertMsg(0,"Degerate stat's");
      return; // Can't normalise only 1 sample!
    }
    UIntT d = VectorSize();
    VectorC stdDev(d);
    for (UIntT i = 0; i < d; i++) {
      if (stats.Covariance()[i][i] > 0)
        stdDev[i] = stats.Covariance()[i][i];
      else
        stdDev[i] = 1.0;
    }
    for (UIntT i = 0; i < d; i++)
      stdDev[i] = Sqrt(stdDev[i]);

    for (SampleIterC<VectorC> it(*this); it; it++)
      *it = (*it * stdDev) + stats.Mean();
  }

  //: Normalises the input vectors using given stats.
  // In order to achieve zero mean and unity variance this function should be
  // called with the return value from MeanCovariance. Subsequent data sets can
  // then be normalised the same way by recording the MeanCovarianceC returned by
  // MeanCovariance.
  void SampleVectorC::Normalise(const MeanCovarianceC & stats,SampleVectorC &sampleVector,bool addBiasElement) const {
    if (stats.Number() < 2) {
      RavlAssertMsg(0,"Degerate stat's");
      return; // Can't normalise only 1 sample!
    }
    UIntT d = VectorSize();
    VectorC stdDev(d);
    for (UIntT i = 0; i < d; i++) {
      if (stats.Covariance()[i][i] > 0)
        stdDev[i] = stats.Covariance()[i][i];
      else
        stdDev[i] = 1.0;
    }
    for (UIntT i = 0; i < d; i++)
      stdDev[i] = Sqrt(stdDev[i]);
    stdDev.Reciprocal();

    if(addBiasElement) {
      VectorC bias(1);
      bias[0] = 1;
      for (SampleIterC<VectorC> it(*this); it; it++) {
        VectorC input = (*it - stats.Mean()) * stdDev;
        sampleVector.Append(bias.Join(input));
      }
    } else {
      for (SampleIterC<VectorC> it(*this); it; it++)
        sampleVector.Append((*it - stats.Mean()) * stdDev);
    }
  }

  //: Undo the normalisation done by 'Normalise()'.
  void SampleVectorC::UndoNormalisation(const MeanCovarianceC & stats,SampleVectorC &sampleVector,bool removeBiasElement) const {
    if (stats.Number() < 2) {
      RavlAssertMsg(0,"Degerate stat's");
      return; // Can't normalise only 1 sample!
    }
    UIntT d = VectorSize();
    VectorC stdDev(d);
    for (UIntT i = 0; i < d; i++) {
      if (stats.Covariance()[i][i] > 0)
        stdDev[i] = stats.Covariance()[i][i];
      else
        stdDev[i] = 1.0;
    }
    for (UIntT i = 0; i < d; i++)
      stdDev[i] = Sqrt(stdDev[i]);

    if(removeBiasElement) {
      for (SampleIterC<VectorC> it(*this); it; it++) {
        sampleVector.Append((VectorC((*it).From(1)) * stdDev) + stats.Mean());
      }
    } else {
      for (SampleIterC<VectorC> it(*this); it; it++) {
        sampleVector.Append((*it * stdDev) + stats.Mean());
      }
    }
  }




  // Set the field info
  bool SampleVectorC::SetFieldInfo(const SArray1dC<FieldInfoC> & fieldInfo) {
    m_fieldInfo = fieldInfo;
    return true;
  }

  RavlN::XMLFactoryRegisterHandleC<SampleVectorC> g_registerXMLFactorySampleVector("RavlN::SampleVectorC");

  void linkSampleVector() {
  }

}

