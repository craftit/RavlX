// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2002, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//! rcsid="$Id$"
//! lib=RavlPatternRec
//! file="Ravl/PatternRec/Modeling/Basic/testBasicModels.cc"
//! userlevel=Develop
//! author="Charles Galambos"
//! docentry="Ravl.API.Pattern Recognition.Numerical Modeling"

#include "Ravl/PatternRec/DesignFuncLSQ.hh"
#include "Ravl/PatternRec/SampleVector.hh"
#include "Ravl/PatternRec/FuncMeanProjection.hh"
#include "Ravl/PatternRec/FuncLinear.hh"
#include "Ravl/SArray1dIter2.hh"
#include "Ravl/BinStream.hh"
#include "Ravl/StrStream.hh"

using namespace RavlN;

int testDesignFuncLSQ();
int testModelIO();
int testModelSingleIO();

int main() {
  int ln;
  if((ln = testDesignFuncLSQ()) != 0) {
    std::cerr << "Test failed line " << ln << "\n";
    return 1;
  }
  if((ln = testModelSingleIO()) != 0) {
    std::cerr << "Test failed line " << ln << "\n";
    return 1;
  }
  if((ln = testModelIO()) != 0) {
    std::cerr << "Test failed line " << ln << "\n";
    return 1;
  }
  std::cerr << "Test passed ok. \n";
  return 0;
}

int testDesignFuncLSQ() {
  std::cout << "testDesignFuncLSQ(), Called. \n";
  SampleVectorC ins(10);
  SampleVectorC outs(10);
  
  for(int i = 0;i < 3;i++) {
    for(int j = 0;j < 3;j++) {
      ins += VectorC(i,j,1);
      outs += VectorC(i * 2 + 1,j * i);
    }
  }
  
  DesignFuncLSQC design(1,false);
  FunctionC func = design.Apply(ins,outs);
  if(!func.IsValid()) return __LINE__;
  VectorC to = func.Apply(VectorC(1,2,1));
  std::cerr << "Testout=" << to << "\n";
  if((to - VectorC(3,2)).SumOfSqr() > 0.000000001) return __LINE__;
  
  return 0;
}

FunctionC designRandomFunc(UIntT inSize,UIntT outSize,UIntT order,bool orth) {
  UIntT samples = (inSize * order) + 1;
  SampleVectorC ins(samples);
  SampleVectorC outs(samples);
  
  for(UIntT i = 0;i < samples;i++) {
    ins += RandomVector(inSize);
    outs += RandomVector(outSize);
  }
  
  DesignFuncLSQC design(order,orth);
  FunctionC func = design.Apply(ins,outs);
  return func;
}

int testModelIO() {
  std::cout << "testModelIO(), Called. \n";
  FuncMeanProjectionC fmp(RandomVector(3),RandomMatrix(3,3));
  FuncLinearC fl(RandomMatrix(3,3),RandomVector(3));
  SArray1dC<FuncLinearCoeffC> funcs(10);
  for(UIntT i = 0;i < funcs.Size();i++)
    funcs[i] = designRandomFunc(RandomInt() % 8 + 2,RandomInt() % 8 + 2,((i/2) % 2)+1,(i % 1) == 0);
  
  // -------------- Test ascii IO ---------------------
  {
    StrOStreamC ostr;
    OStreamC bos(ostr);
    
    bos << fmp << '\n' << fl << '\n' << funcs << '\n';
    
    StringC str = ostr.String();
    //cerr << "Data=" << str << "\n";
    StrIStreamC istr(str);
    IStreamC bis(istr);
    
    FuncMeanProjectionC nfmp;
    FuncLinearC nfl;
    SArray1dC<FuncLinearCoeffC> nfuncs;
    
    bis >> nfmp >> nfl >> nfuncs;
    
    // Check we've loaded what we saved.
    if(!nfmp.IsValid()) return __LINE__;
    if((nfmp.Mean() - fmp.Mean()).SumOfSqr() > 0.0000001) return __LINE__;
    if((nfmp.Projection() - fmp.Projection()).SumOfSqr() > 0.0000001) return __LINE__;
    if(!nfl.IsValid()) return __LINE__;
    if((nfl.Mean() - fl.Mean()).SumOfSqr() > 0.0000001) return __LINE__;
    if((nfl.Projection() - fl.Projection()).SumOfSqr() > 0.0000001) return __LINE__;
    
    for(SArray1dIter2C<FuncLinearCoeffC,FuncLinearCoeffC> it(nfuncs,funcs);it;it++) {
      if(!it.Data2().IsValid()) return __LINE__;
      if(!it.Data1().IsValid()) return __LINE__;
      if((it.Data1().Transform() - it.Data2().Transform()).SumOfSqr() > 0.000001) return __LINE__;
    }
  }
  
  // -------------- Test binary IO ---------------------
  {
    StrOStreamC ostr;
    BinOStreamC bos(ostr);
    
    bos << fmp << fl << funcs;
    
    StrIStreamC istr(ostr.String());
    BinIStreamC bis(istr);
    
    FuncMeanProjectionC nfmp;
    FuncLinearC nfl;
    SArray1dC<FuncLinearCoeffC> nfuncs;
    
    bis >> nfmp >> nfl >> nfuncs;
    
    // Check we've loaded what we saved.
    if(!nfmp.IsValid()) return __LINE__;
    if((nfmp.Mean() - fmp.Mean()).SumOfSqr() > 0.0000001) return __LINE__;
    if((nfmp.Projection() - fmp.Projection()).SumOfSqr() > 0.0000001) return __LINE__;
    if(!nfl.IsValid()) return __LINE__;
    if((nfl.Mean() - fl.Mean()).SumOfSqr() > 0.0000001) return __LINE__;
    if((nfl.Projection() - fl.Projection()).SumOfSqr() > 0.0000001) return __LINE__;
    
    for(SArray1dIter2C<FuncLinearCoeffC,FuncLinearCoeffC> it(nfuncs,funcs);it;it++) {
      if(!it.Data2().IsValid()) return __LINE__;
      if(!it.Data1().IsValid()) return __LINE__;
      if((it.Data1().Transform() - it.Data2().Transform()).SumOfSqr() > 0.000001) return __LINE__;
    }
  }
  
  
  return 0;
}

int testModelSingleIO() {
  std::cout << "testModelSingleIO(), Called. \n";
  
  for(int i = 0;i < 20;i++) {
    FuncLinearCoeffC func = designRandomFunc(RandomInt() % 8 + 2,RandomInt() % 8 + 2,((i/2) % 2)+1,(i % 1) == 0);


    // -------------- Test ascii IO ---------------------
    {
      StrOStreamC ostr;
      OStreamC bos(ostr);
      
      bos << func;
      
      StringC str = ostr.String();
      StrIStreamC istr(str);
      IStreamC bis(istr);
      
      FuncLinearCoeffC nfunc;
      
      bis >> nfunc;
      
      if(!nfunc.IsValid()) return __LINE__;
      if((nfunc.Transform() - func.Transform()).SumOfSqr() > 0.000001) return __LINE__;
      
      while(bis.good()) {
	StringC val;
	bis >> val;
	if(!val.IsEmpty()) {
	  std::cerr << "Full=" << str << "\n";
	  std::cerr << "Text remaining... '" << val << "'\n";
	  
	  return __LINE__;
	}
      }
    }
    
    // -------------- Test binary IO ---------------------
    {
      StrOStreamC ostr;
      BinOStreamC bos(ostr);
      
      bos << func;
      
      StrIStreamC istr(ostr.String());
      BinIStreamC bis(istr);
      
      FuncLinearCoeffC nfunc;
      
      bis >> nfunc;
      
      if(!nfunc.IsValid()) return __LINE__;
      if((nfunc.Transform() - func.Transform()).SumOfSqr() > 0.000001) return __LINE__;
    }
  }
  
  return 0;
}
