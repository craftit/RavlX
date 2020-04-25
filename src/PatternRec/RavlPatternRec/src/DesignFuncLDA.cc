// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2003, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//! rcsid="$Id$"
//! lib=RavlPatternRec
//! file="Ravl/PatternRec/Modeling/DimensionReduction/DesignFuncLDA.cc"

#include "Ravl/config.h"
#include "Ravl/PatternRec/DesignFuncLDA.hh"
#include "Ravl/DP/SPort.hh"
#include "Ravl/Math.hh"
#include "Ravl/PatternRec/SampleStreamVector.hh"
#include "Ravl/DP/Compose.hh"
#include "Ravl/DP/SPortAttach.hh"

#if RAVL_COMPILER_MIPSPRO 
#pragma instantiate RavlN::DPIPortBodyC<RavlN::Tuple2C<RavlN::VectorC,RavlN::StringC> > 
#endif

#define DODEBUG	0
#if DODEBUG
#define ONDEBUG(x) x
#else
#define ONDEBUG(x)
#endif

namespace RavlN {
  
  //: Default constructor.
  
  DesignFuncLDABodyC::DesignFuncLDABodyC(RealT variationPreserved)
    : DesignFuncReduceBodyC(variationPreserved),
      forceHimDim(false), 
      classSamp(0)
  {}
  
  //: Load from stream.
  
  DesignFuncLDABodyC::DesignFuncLDABodyC(std::istream &strm)
    : DesignFuncReduceBodyC(strm),
      forceHimDim(false),
      classSamp(0)
  {
    strm >> mean >> lda;
    classSamp = 0;
  }
  
  //: Load from binary stream.
  
  DesignFuncLDABodyC::DesignFuncLDABodyC(BinIStreamC &strm)
    : DesignFuncReduceBodyC(strm),
      forceHimDim(false),
      classSamp(0)
  {
    strm >> mean >> lda;
    classSamp = 0;
  }
  
  //: Writes object to stream, can be loaded using constructor
  
  bool DesignFuncLDABodyC::Save(ostream &out) const {
    if(!DesignFuncReduceBodyC::Save(out))
      return false;
    out << " " << mean << " " << lda;
    return true;
  }
  
  //: Writes object to stream, can be loaded using constructor
  
  bool DesignFuncLDABodyC::Save(BinOStreamC &out) const {
    if(!DesignFuncReduceBodyC::Save(out))
      return false;
    out << mean << lda;
    return true;
  }

  //: Create the LDA  function from the given data.
  
  FunctionC DesignFuncLDABodyC::Apply(const DataSetVectorLabelC &in) {

    //SampleC<VectorC>   inVecs(in.Sample1().Size());
    //for(SampleIterC<VectorC> it(in.Sample1()); it ;it++)
    //inVecs += *it;
  
    //UIntT N  =  inVecs.Size();
    //UIntT d =  inVecs.First().Size();
    //UIntT maxN =  UIntT(0.2* (RealT)d);   //maximum nof data to use for the PCA step
    //UIntT maxN =  d;   //maximum nof data to use for the PCA step
    //SampleC<VectorC> inVecsPca(Min(N,maxN));
    //if(N <maxN)  inVecsPca= inVecs;
    //else    for(UIntT i = 0; i<maxN;i++)  inVecsPca.Append(inVecs.Pick());

    DataSetVectorLabelC train;
    if(classSamp == 0)  train = in;
    else train = in.ExtractPerLabel(classSamp);

    SampleC<VectorC> inVecsPca = train.Sample1();
      
    DesignFuncPCAC pca(varPreserved);
    FunctionC pcaFunc = pca.Apply(inVecsPca);
    ONDEBUG(cerr << "Dimensionality reduction. Preserving " << varPreserved << ", using PCA from "  << pcaFunc.InputSize() << " to " <<   pcaFunc.OutputSize() << endl);
    mean = pca.Mean();
    SampleC<VectorC> outPca = pcaFunc.Apply(inVecsPca);

    DataSetVectorLabelC inLda(outPca, train.Sample2());

    MatrixC Sb = inLda.BetweenClassScatter ();
    MatrixC Sw = inLda.WithinClassScatter ();
    MatrixC matLDA = DesignMatrixTransformation(Sw, Sb);
    
    lda = matLDA.SubMatrix(matLDA.Rows(),Min(matLDA.Cols(),SizeT (train.Sample2().MaxValue()) )).T() * pca.Pca().Matrix();

    return FuncMeanProjectionC(mean, lda);
  }


  FunctionC DesignFuncLDABodyC::Apply(SampleStreamVectorC & inPca, SampleStreamVectorLabelC & inLda) {

    //: first we need to do the pca
    ONDEBUG(cerr << "Starting PCA" << endl);
    DesignFuncPCAC pca(varPreserved);    
    FunctionC pcaFunc = pca.Apply(inPca);
        
    UIntT D = pcaFunc.InputSize();
    UIntT Dpca = pcaFunc.OutputSize();
    ONDEBUG(cerr << "Dimensionality reduction using PCA from: "  << D  << " to " << Dpca   << endl);
    VectorC mean1 = pca.Mean();

    //: Now we need to calculate the mean of the database first.
    //: Note that we assumed that input data for pca and lda are not necessarily equal
    Tuple2C<VectorC, UIntT> data;
    VectorC mean2(D);
    mean2.Fill(0.0);
    RealT N = 0.0;
    while(inLda.Get(data)){
      mean2 += data.Data1();
      N++;
    }
    mean2  /= N;
    mean = mean2;
    VectorC meanPca = pcaFunc.Apply(mean2);

    //: Now scatter matrices are calculated
    inLda.First();
    MatrixC Sb(Dpca, Dpca, 0.0), Sw(Dpca, Dpca, 0.0);
    MeanCovarianceC meancov(Dpca);
    IntT prevID = -1;
    UIntT numClasses= 0;
    while(inLda.Get(data)) {
      VectorC vec = data.Data1();
      VectorC vecPca = pcaFunc.Apply(vec);
      IntT ID  = (IntT)data.Data2();
      if((ID != prevID) && (prevID != -1)) { 
	VectorC m = meancov.Mean();
	MatrixC cov = meancov.Covariance();
	RealT p = (RealT) meancov.Number()/N;
	VectorC diff = m - meanPca;
	Sb += (diff.OuterProduct (diff, p));
	Sw += (cov * p);
	meancov.SetZero();
	numClasses++;
      }
      prevID = ID;
      meancov += vecPca;
    }
    {
      VectorC m = meancov.Mean();
      MatrixC cov = meancov.Covariance();
      RealT p = (RealT) meancov.Number()/N;
      VectorC diff = m - meanPca;
      Sb += (diff.OuterProduct (diff, p));
      Sw += (cov * p);
    }

    MatrixC matLDA = DesignMatrixTransformation(Sw, Sb);
    
    lda = matLDA.SubMatrix(matLDA.Rows(),Min(matLDA.Cols(), SizeT(numClasses) )).T() * pca.Pca().Matrix();

    return FuncMeanProjectionC(mean, lda);
  }
  
  
  FunctionC DesignFuncLDABodyC::Apply(SampleStreamC<VectorC> &inPca,  SampleStream2C<VectorC, StringC> &inLda) {
    
    //: first we need to do the pca
    DesignFuncPCAC pca(varPreserved);
    
#if 0 // Eek...no idea why it was limited to 2000 samples?
    SampleStreamVectorC ssv(inPca);
    // eek no idea why this was limited to 2000 samples
    SampleVectorC sv;
    {
      SArray1dC<VectorC> vecs(2000);
      IntT n = ssv.GetArray(vecs);
      cerr << n << "  samples are used for dimensionality reduction using PCA \n";
      SArray1dC<VectorC> subvecs(vecs, n);
      sv = SampleVectorC(subvecs);
    }
    FunctionC pcaFunc = pca.Apply(sv);
#endif
    FunctionC pcaFunc = pca.Apply(inPca);
    
    UIntT D = pcaFunc.InputSize();
    UIntT Dpca = pcaFunc.OutputSize();
    ONDEBUG(cerr << "Dimensionality reduction using PCA from: "  << D  << " to " << Dpca   << endl);
    VectorC mean1 = pca.Mean();

    //: Now we need to calculate the mean of the database first.
    //: Note that we assumed that input data for pca and lda are not necessarily equal
    Tuple2C<VectorC, StringC> data;
    VectorC mean2(D);
    mean2.Fill(0.0);
    RealT N = 0.0;
    while(inLda.Get(data)){
      mean2 += data.Data1();
      N++;
    }
    mean2  /= N;
    mean = mean2;
    VectorC meanPca = pcaFunc.Apply(mean2);

    //: Now scatter matrices are calculated
    inLda.First();
    MatrixC Sb(Dpca, Dpca, 0.0), Sw(Dpca, Dpca, 0.0);
    MeanCovarianceC meancov(Dpca);
    StringC prevID = "X";
    UIntT numClasses= 0;
    while(inLda.Get(data)) {
      VectorC vec = data.Data1();
      VectorC vecPca = pcaFunc.Apply(vec);
      StringC ID  = data.Data2();
      if((ID != prevID) && (prevID != "X")) { 
	VectorC m = meancov.Mean();
	MatrixC cov = meancov.Covariance();
	RealT p = (RealT) meancov.Number()/N;
	VectorC diff = m - meanPca;
	Sb += (diff.OuterProduct (diff, p));
	Sw += (cov * p);
	meancov.SetZero();
	numClasses++;
      }
      prevID = ID;
      meancov += vecPca;
    }
    {
      VectorC m = meancov.Mean();
      MatrixC cov = meancov.Covariance();
      RealT p = (RealT) meancov.Number()/N;
      VectorC diff = m - meanPca;
      Sb += (diff.OuterProduct (diff, p));
      Sw += (cov * p);
    }

    MatrixC matLDA = DesignMatrixTransformation(Sw, Sb);
    
    lda = matLDA.SubMatrix(matLDA.Rows(),Min(matLDA.Cols(), SizeT(numClasses) )).T() * pca.Pca().Matrix();

    return FuncMeanProjectionC(mean, lda);
  }

  

  //: Create function from the 2 streams.
  // This method uses streams so that you don't have to store all the data in memory.<br>
  //!param: inPca - uses this stream to do some initial PCA dimension reduction (could be same stream as inLda)
  //!param: inLda - uses this labelled stream of vectors to do dimension reduction using LDA

  static Tuple2C<VectorC,StringC> vecStrFloat2vecStr(const Tuple2C<TVectorC<float>,StringC> &in)
  { return Tuple2C<VectorC,StringC>(in.Data1(),in.Data2()); }

  FunctionC DesignFuncLDABodyC::Apply(SampleStreamC<TVectorC<float> > &inPca,
                                         SampleStream2C<TVectorC<float>, StringC> &inLda)
  {
    SampleStreamVectorC ssv(inPca);
    SampleStream2C<VectorC, StringC> inRealLda = SPort(inLda >> Process(vecStrFloat2vecStr));
    return Apply(ssv,inRealLda);
  }

  //: Design the transform using the matrix transformation method
  
  MatrixC DesignFuncLDABodyC::DesignMatrixTransformation(const MatrixC &sw,const MatrixC &sb) {

    RealT smallValue = 1.0e-10;
    VectorMatrixC eigensSw = EigenVectors (sw);
    eigensSw.Sort ();
    VectorC eigenValsSw = eigensSw.Vector();
    MatrixC eigenVecsSw = eigensSw.Matrix();

    //: check if there is any unvalid eigen values
    UIntT firstZeroIndex = eigenValsSw.Size()-1;
    while ((eigenValsSw[firstZeroIndex ]<smallValue) && (firstZeroIndex>0)) firstZeroIndex --;
    firstZeroIndex ++;
    VectorC eigenValsSw2 = eigenValsSw.From(0, firstZeroIndex);
    MatrixC eigenVecsSw2 = eigenVecsSw.SubMatrix(eigenVecsSw.Rows(), firstZeroIndex);

    VectorC vec(eigenValsSw2.Size());
    vec.Fill (0.0);
    for (IndexC ind = 0; ind < vec.Size(); ind++)  vec[ind] = 1.0 / sqrt(eigenValsSw2[ind]);
    
    MatrixC diag(vec.Size(),vec.Size());
    diag.Fill(0);
    diag.SetDiagonal(vec);
    
    MatrixC B = eigenVecsSw2 * diag;
    MatrixC BT = B.T();
    
    MatrixC S_hat = (BT * sb) * B;
    
    VectorMatrixC eigenVecsS_hat = EigenVectors (S_hat);
    eigenVecsS_hat.Sort ();
    VectorC temp = eigenVecsS_hat.Vector();
    firstZeroIndex = temp.Size()-1;
    while ((temp[firstZeroIndex ]<smallValue) && (firstZeroIndex>0)) firstZeroIndex --;
    firstZeroIndex ++;
    MatrixC V_hat = eigenVecsS_hat.Matrix().SubMatrix(eigenVecsS_hat.Matrix().Rows(), firstZeroIndex);
    MatrixC matLda = B * V_hat;

    for (IndexC index2 = 0; index2 < matLda.Cols(); index2++) {
      Slice1dC<RealT> slice = matLda.SliceColumn(index2);
      slice /= slice.Modulus();
    }

    return matLda;
  }

}
