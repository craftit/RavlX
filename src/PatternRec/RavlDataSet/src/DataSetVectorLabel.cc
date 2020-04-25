// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2003, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//! rcsid="$Id$"
//! lib=RavlPatternRec
//! file="Ravl/PatternRec/DataSet/DataSetVectorLabel.cc"

#include "Ravl/PatternRec/DataSetVectorLabel.hh"
#include "Ravl/PatternRec/DataSet2Iter.hh"
#include "Ravl/PatternRec/DataSet1Iter.hh"
#include "Ravl/SArray1dIter2.hh"
#include "Ravl/SumsNd2.hh"
#include "Ravl/Exception.hh"
#include "Ravl/XMLFactoryRegister.hh"

namespace RavlN {

  DataSetVectorLabelBodyC::DataSetVectorLabelBodyC(SampleStreamVectorLabelC & sampleStream)
   : DataSet2BodyC<SampleVectorC, SampleLabelC>(1000)
  {
    Tuple2C<VectorC, UIntT> data;
    while(sampleStream.Get(data)) {
      Append(data.Data1(), data.Data2());
    }
  }

  DataSetVectorLabelBodyC::DataSetVectorLabelBodyC(const SArray1dC<MeanCovarianceC> & stats)
   : DataSet2BodyC<SampleVectorC, SampleLabelC>(1000)
  {
    UIntT classLabel = 0;
    for (SArray1dIterC<MeanCovarianceC> it(stats); it; it++) {
      SampleVectorC sample(*it);
      Append(sample, classLabel);
      classLabel++;
    }
  }

  DataSetVectorLabelBodyC::DataSetVectorLabelBodyC(const XMLFactoryContextC & factory)
   : DataSet2BodyC<SampleVectorC, SampleLabelC>(1000)
  {
    UIntT classLabel = 0;
    for(DLIterC<XMLTreeC> it(factory.Children()); it; it++) {
      SampleVectorC sampleVector;
      if(factory.UseComponent(it->Name(), sampleVector)) {
        Append(sampleVector, classLabel);
        classLabel++;
      }
    }
  }
  
  //: Create a separate sample for each label.
  
  SArray1dC<SampleVectorC> DataSetVectorLabelBodyC::SeperateLabels() const
  {
    SArray1dC<SampleVectorC> ret(Sample2().MaxValue() + 1);
    for(DataSet2IterC<SampleVectorC, SampleLabelC> it(Sample1(), Sample2()); it; it++)
      ret[it.Data2()].Append(it.Data1());
    return ret;
  }

  VectorC DataSetVectorLabelBodyC::GlobalMean() const
  {
    return Sample1().Mean();
  }
  
  SArray1dC<VectorC> DataSetVectorLabelBodyC::ClassMeans() const
  {
    CollectionC<Tuple2C<VectorC, UIntT> > means(10);
    DataSet2IterC<SampleVectorC, SampleLabelC> it(Sample1(), Sample2());
    if(!it)
      return SArray1dC<VectorC>(); // No samples.
    UIntT dim = it.Data1().Size();
    for(; it; it++) {
      while (means.Size() <= it.Data2()) {
        VectorC vec(dim);
        vec.Fill(0);
        means.Insert(Tuple2C<VectorC, UIntT>(vec, 0));
      }
      means[it.Data2()].Data1() += it.Data1();
      means[it.Data2()].Data2()++;}
    SArray1dC<VectorC> ret(means.Size());
    for (SArray1dIter2C<Tuple2C<VectorC, UIntT>, VectorC> it(means.Array(), ret); it; it++)
      it.Data2() = it.Data1().Data1() / static_cast<RealT>(it.Data1().Data2());
    return ret;
  }

  DataSetVectorLabelC DataSetVectorLabelBodyC::ClassMeansLabels() const
  {
    SampleVectorC means(Sample2().MaxValue() + 1);
    SampleLabelC labels(Sample2().MaxValue() + 1);
    SArray1dC<VectorC> smeans = ClassMeans();
    means.Append(smeans);
    for (UIntT i = 0; i < smeans.Size(); i++)
      labels.Append(i);
    return DataSetVectorLabelC(means, labels);
  }

  SArray1dC<UIntT> DataSetVectorLabelBodyC::ClassNums() const
  {
    CollectionC<UIntT> num(10);
    for (DataSet1IterC<SampleLabelC> it(Sample2()); it; it++) {
      while (num.Size() <= *it)
        num.Insert(0);
      num[*it]++;
    }
    return num.Array();
  }
  
  SArray1dC<MeanCovarianceC> DataSetVectorLabelBodyC::ClassStats(bool sampleStatistics) const
  {
    CollectionC<SumsNd2C> stats(10);
    DataSet2IterC<SampleVectorC, SampleLabelC> it(Sample1(), Sample2());
    if (!it)
      return SArray1dC<MeanCovarianceC>();
    UIntT dim = it.Data1().Size();
    for (; it; it++) {
      while (stats.Size() <= it.Data2())
        stats.Insert(SumsNd2C(dim));
      stats[it.Data2()] += it.Data1();
    }
    SArray1dC<MeanCovarianceC> meancovs(stats.Size());
    for (SArray1dIter2C<MeanCovarianceC, SumsNd2C> it(meancovs, stats.Array()); it; it++)
      it.Data1() = it.Data2().MeanCovariance(sampleStatistics);
    return meancovs;
  }
  
  MatrixC DataSetVectorLabelBodyC::BetweenClassScatter() const
  {
    CollectionC<Tuple2C<VectorC, UIntT> > means(1024);
    DataSet2IterC<SampleVectorC, SampleLabelC> it(Sample1(), Sample2());
    if (!it)
      return MatrixC(); // No samples.
    UIntT dim = it.Data1().Size();
    for (; it; it++) {
      while (means.Size() <= it.Data2()) {
        VectorC vec(dim);
        vec.Fill(0);
        means.Insert(Tuple2C<VectorC, UIntT>(vec, 0));
      }
      means[it.Data2()].Data1() += it.Data1();
      means[it.Data2()].Data2()++;}
    SArray1dIterC<Tuple2C<VectorC, UIntT> > mit(means.Array());
    VectorC globalMean = mit->Data1().Copy();
    RealT total = mit->Data2();
    for (mit++; mit; mit++) {
      globalMean += mit->Data1();
      total += mit->Data2();
    }
    globalMean /= total;
    MatrixC Sb(globalMean.Size(), globalMean.Size(), 0.0);
    for (mit.First(); mit; mit++) {
      if (mit->Data2() > 0) {
        VectorC diff = (mit->Data1() / mit->Data2()) - globalMean;
        Sb.AddOuterProduct(diff, diff);
      }
    }
    return Sb;
  }
  
  MatrixC DataSetVectorLabelBodyC::WithinClassScatter(bool sampleStatistics) const
  {
    SArray1dC<MeanCovarianceC> stats = ClassStats(sampleStatistics);
    SArray1dIterC<MeanCovarianceC> it(stats);
    while (it && it->Number() < 2)
      it++;
    if (!it)
      throw ExceptionNumericalC("DataSetVectorLabelBodyC::WithinClassScatter, No enough data to compute inclass scatter. ");
    MatrixC Sw = it->Covariance() * it->Number();
    RealT total = it->Number();
    for (it++; it; it++) {
      if (it->Number() < 2)
        continue;
      total += it->Number();
      Sw.MulAdd(it->Covariance(), it->Number());
    }
    if (total < 1)
      throw ExceptionNumericalC("DataSetVectorLabelBodyC::WithinClassScatter, Not enough data to compute inclass scatter. ");
    Sw /= total;
    return Sw;
  }
  
  DataSetVectorLabelC DataSetVectorLabelBodyC::ExtractPerLabel(UIntT numSamples) const
  {

    SArray1dC<SampleVectorC> arr = SeperateLabels();
    for (SArray1dIterC<SampleVectorC> it(arr); it; it++) {
      if (numSamples > (UIntT) it.Data().Size()) {
        RavlWarning("Number of samples for class less than requested in ExtractPerLabel. Setting to this minimum!");
        numSamples = Min((UIntT) it.Data().Size(), numSamples);
      }
    }
    UIntT noClasses = arr.Size();
    SampleVectorC input(numSamples * noClasses);
    SampleLabelC output(numSamples * noClasses);
    for (SArray1dIterC<SampleVectorC> it(arr); it; it++) {
      for (UIntT i = 0; i < numSamples; i++) {
        input.Append(it.Data().Pick());
        output.Append(it.Index().V());
      }
      // Has a class name been set?  If so lets copy it to the extracted set.
      StringC className;
      if(Sample2().GetClassName(it.Index().V(), className)) {
        output.SetClassName(it.Index().V(), className);
      }
    }
    DataSetVectorLabelC ret(input, output);
    return ret;
  }

  // Create a dataset

  DataSetVectorLabelC CreateDataSet(UIntT dimension, UIntT classes, UIntT samplesPerClass, RealT dist)
  {

    DataSetVectorLabelC dset(classes * samplesPerClass);

    // The first class has zero mean and unit covariance
    VectorC mean(dimension);
    mean.Fill(0.0);
    MatrixC cov(dimension, dimension);
    cov.Fill(0.0);
    VectorC ones(dimension);
    ones.Fill(1.0);
    cov.SetDiagonal(ones);
    SampleVectorC sv(MeanCovarianceC(samplesPerClass, mean, cov));
    dset.Append(sv, 0);

    for (UIntT label = 1; label < classes; label++) {
      // OK just keep Euclidean distance equal between classes
      mean += dist / Sqrt((RealT) dimension);
      SampleVectorC sv(MeanCovarianceC(samplesPerClass, mean, cov));
      dset.Append(sv, label);
    }

    return dset;
  }

  //: Just get some standard info about the data set
  StringC DataSetVectorLabelBodyC::Info() const
  {

    SizeT dimension = Sample1().First().Size();
    SArray1dC<UIntT> labelSums = Sample2().LabelSums();
    RCHashC<UIntT, StringC> label2names = Sample2().Label2ClassNames();
    StringC info = "Input Dimension '" + StringOf(dimension) + "'.";
    UIntT label = 0;
    for (SArray1dIterC<UIntT> it(labelSums); it; it++) {
      StringC l;
      if (label2names.IsElm(label))
        l.form(" Label '%d' ('%s') has '%d' samples.", label, label2names[*it].data(), *it);
      else
        l.form(" Label '%d' has '%d' samples.", label, *it);
      label++;
      info += l;
    }
    return info;

  }

  RavlN::XMLFactoryRegisterHandleC<DataSetVectorLabelC> g_registerXMLFactoryDataSetVectorLabel("RavlN::DataSetVectorLabelC");

  void linkDataSetVectorLabel()
  {
  }

}

