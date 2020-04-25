// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2003, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//! rcsid="$Id$"
//! lib=RavlPatternRec
//! file="Ravl/PatternRec/Modeling/DimensionReduction/DesignFuncPCA.cc"

#include "Ravl/PatternRec/DesignFuncPCA.hh"
#include "Ravl/PatternRec/FuncMeanProjection.hh"
#include "Ravl/BinStream.hh"
#include "Ravl/PatternRec/SampleVector.hh"
#include "Ravl/PatternRec/SampleStreamVector.hh"
#include "Ravl/Collection.hh"
#include "Ravl/PatternRec/SampleIter.hh"
#include "Ravl/MeanCovariance.hh"

#define DODEBUG 0
#if DODEBUG
#define ONDEBUG(x) x
#else
#define ONDEBUG(x)
#endif

namespace RavlN {
  
  //: Default constructor.
  
  DesignFuncPCABodyC::DesignFuncPCABodyC(RealT variationPreserved)
    : DesignFuncReduceBodyC(variationPreserved),
      forceHimDim(false)
  {}
  
  //: Load from stream.
  
  DesignFuncPCABodyC::DesignFuncPCABodyC(istream &strm) 
    : DesignFuncReduceBodyC(strm)
  {
    strm >> forceHimDim;
  }
  
  //: Load from binary stream.
  
  DesignFuncPCABodyC::DesignFuncPCABodyC(BinIStreamC &strm)
    : DesignFuncReduceBodyC(strm)
  {
    strm >> forceHimDim;
  }
  
  //: Writes object to stream, can be loaded using constructor
  
  bool DesignFuncPCABodyC::Save(ostream &out) const {
    if(!DesignFuncReduceBodyC::Save(out))
      return false;
    out << ' ' << forceHimDim << ' ';
    return true;
  }
  
  //: Writes object to stream, can be loaded using constructor
  
  bool DesignFuncPCABodyC::Save(BinOStreamC &out) const {
    if(!DesignFuncReduceBodyC::Save(out))
      return false;
    out << forceHimDim;
    return true;
  }

  //: Create function from the given data.
  
  FunctionC DesignFuncPCABodyC::Apply(const SampleC<VectorC> &in) {
    FunctionC ret;
    if(in.Size() == 0)
      return FunctionC(); // Can't do anything without data.
    VectorC vec = in.First();
    if(vec.Size() > in.Size() || forceHimDim)
      return DesignHighDim(in,varPreserved);
    return DesignLowDim(in,varPreserved);
  }
  

  //: Design the transform.
  // This routine takes advantage of the fact that if there are less samples than dimensions 
  // in the sample vector, you can reduce the size of the covariance matrix you compute the eigen 
  // values on.   This can make the PCA routine significantly faster in some cases.
  
  FunctionC DesignFuncPCABodyC::DesignHighDim(const SampleC<VectorC> &sample,RealT variation) {
    // FIXME :- This could be implemented more efficiently.
    
    SampleVectorC sv(sample);
    UIntT N = sample.Size(); // number of training samples
    if(N == 0)
      return FunctionC();
    UIntT d = sample.First().Size(); // input dimension
    mean = sv.Mean();
    MatrixC A (d, N);
    {
      IndexC index = 0;
      for(SampleIterC<VectorC> it(sample);it;it++,index++)
	A.SetColumn (index, *it - mean);
    }
    
    // Construct L=A^transpose A

    MatrixC AT = A.T();
    MatrixC L = AT.AAT();
    L /= N;

    // Calculate Eigenvectors.
    VectorMatrixC Leigenvecs  =  EigenVectors(L);
    //cout << "finished computing eigen vecs: " << endl << flush;
    
    ONDEBUG(cerr << "Values=" << Leigenvecs.Vector() << "\n");

    Leigenvecs.Sort();
    // Create output matrix Wpca
    MatrixC Wpca = A * Leigenvecs.Matrix();
    
    VectorC smallVec(d); 
    smallVec.Fill(0.0);
    
    for (IndexC index2 = 0; index2 < Wpca.Cols(); index2++) {
      if (Leigenvecs.Vector()[index2] < 1e-10) 
	Wpca.SetColumn (index2,smallVec);
      else {
	Slice1dC<RealT> slice = Wpca.SliceColumn(index2);
	slice /= slice.Modulus();
      }
    }
    VectorC temp (Wpca.Cols());
    temp.Fill (0.0);
    
    // Do dot products on transposed matrices its faster.
    MatrixC WpcaT = Wpca.T();
    for (IndexC index3 = 0; index3 < temp.Size(); index3++) {
      VectorC sli(WpcaT.SliceRow(index3));
      for(UIntT index4 = 0;index4 < AT.Rows();index4++) {
	temp[index3] += Sqr(VectorC(AT.SliceRow(index4)).Dot(sli));
      }
    }
    
    temp /= N;
    RealT total = temp.Sum();
    RealT runningTotal = 0.0;
    UIntT numComponents = 0;
    //ONDEBUG(cerr << "Values=" << temp << "\n");
    if (variation <= 1.0) {
      RealT limit = variation * total;
      while ((runningTotal += temp[numComponents++]) < limit);
    } else {
      numComponents = UIntT(variation < temp.Size() ? variation : static_cast<RealT>(temp.Size()));
      for (UIntT i = 0; i < numComponents; i++)
	runningTotal += temp[i];
    }
    
    pca = VectorMatrixC (temp.From(0,numComponents),
			 Wpca.SubMatrix(d,numComponents).T());
    
    //cerr << "VectorSize=" << pca.Vector().Size() << " Matrix=" << pca.Matrix().Size1() << " " << pca.Matrix().Size2()<< " Components=" << numComponents << "\n";
    // ONDEBUG(cerr << "Mean=" << mean << "\n");
    // ONDEBUG(cerr << "Proj=" << proj << "\n");
    return FuncMeanProjectionC(mean,pca.Matrix());
  }
  
  
  //: Design the transform.
  
  FunctionC DesignFuncPCABodyC::DesignLowDim(const SampleC<VectorC> &sample,RealT variation) {
    if(sample.IsEmpty())
      return FunctionC();
    SampleVectorC sv(sample);
    MeanCovarianceC stats = sv.MeanCovariance();
    return DesignLowDim(stats);
  }

  //: Create function from the given data, and sample weights.
  
  FunctionC DesignFuncPCABodyC::Apply(const SampleC<VectorC> &sample,const SampleC<RealT> &weight) {
    if(sample.IsEmpty())
      return FunctionC();
    SampleVectorC sv(sample);
    MeanCovarianceC stats = sv.MeanCovariance(weight);
    return DesignLowDim(stats);    
  }
  
  
  //: Create function from the given data.
  
  FunctionC DesignFuncPCABodyC::Apply(SampleStreamC<VectorC> &sample) {
    SampleStreamVectorC ssv(sample);
    MeanCovarianceC stats = ssv.MeanCovariance();
    return DesignLowDim(stats);
  }

  //: Build pca from mean and covariance.
  
  FunctionC DesignFuncPCABodyC::DesignLowDim(const MeanCovarianceC &stats) {
    if(stats.Number() == 0)
      return FunctionC();
    mean = stats.Mean();
    RealT variation = varPreserved;
    const UIntT dim = mean.Size();
    VectorMatrixC Leigenvecs =  EigenVectors(stats.Covariance());
    
    //: need to sort matrix into order
    Leigenvecs.Sort();

    ONDEBUG(cerr << "Values=" << Leigenvecs.Vector() << "\n");
    
    RealT total = Leigenvecs.Vector().Sum();
    RealT runningTotal = 0.0;
    UIntT numComponents = 0;
    
    if (variation < 1.0) 
      while ((runningTotal += Leigenvecs.Vector()[numComponents++]) < variation*total);
    else {
      numComponents = UIntT(variation < dim? variation: dim);
      for (UIntT i = 0; i < numComponents; i++)
	runningTotal += Leigenvecs.Vector()[i];
    }
    varPreserved = runningTotal / total;
    
    pca = VectorMatrixC (Leigenvecs.Vector().From(0,numComponents),
			 Leigenvecs.Matrix().SubMatrix(dim,numComponents).T());
    return FuncMeanProjectionC(mean,pca.Matrix());    
  }

}
