// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2008, OmniPerception Ltd.
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//! lib=RavlPatternRec
//! author="Charles Galambos"
//! docentry="Ravl.API.Pattern Recognition.Numerical Modeling"

#include "Ravl/Option.hh"
#include "Ravl/PatternRec/DesignFuncLSQ.hh"
#include "Ravl/PatternRec/DataSet2Iter.hh"
#include "Ravl/StringList.hh"
#include "Ravl/IO.hh"

using namespace RavlN;

bool ReadData(const StringC &filename,int noOutputs,SampleC<VectorC> &inSamples,SampleC<VectorC> &outSamples) {

  inSamples = SampleC<VectorC>(1024);
  outSamples = SampleC<VectorC> (1024);
  
  IStreamC istrm(filename);
  if(!istrm.IsOpen()) {
    std::cerr << "Failed to open " << filename << "\n";
    return false;
  }
  IntT inSize = -1;
  while(istrm) {
    StringC line;
    if(readline(istrm,line,'\n',true) == 0)
      continue;
    if(line.firstchar() == '#')
      continue;
    StringListC strList(line," \t,");
    if(strList.IsEmpty())
      continue;
    if(inSize < 0) {
      inSize = strList.Size() - noOutputs;
      if(inSize < 0)
	continue;
    } else {
      if(strList.Size() != (noOutputs + inSize)) {
	std::cerr << "Bad input line. \n";
      }
    }
    VectorC inVec(inSize);
    VectorC outVec(noOutputs);
    UIntT at = 0;
    for(DLIterC<StringC> it(strList);it;it++,at++) {
      if(at < (UIntT) inSize) {
	inVec[at] = it->RealValue();
      } else {
	outVec[at - inSize] = it->RealValue();
      }
    }
    inSamples.Append(inVec);
    outSamples.Append(outVec);
  }
  return true;
}

int main(int nargs,char **argv) {
  OptionC opts(nargs,argv);
  UIntT order = opts.Int("p",2,"Order of polynomial.");
  bool orthogonal = opts.Boolean("orth",false,"Orthogonal polynomial. 1=Linear 2=Quadratic ...");
  UIntT noOutputs = opts.Int("no",1,"Number of output's ");
  bool residual = opts.Boolean("r",false,"Compute the residual of the fit. ");
  bool verbose = opts.Boolean("v",false,"Verbose. ");
  bool swap = opts.Boolean("s",false,"Swap inputs and output.");
  StringC infile = opts.String("","","in.dat");
  StringC outfile = opts.String("","","out.abs");
  opts.Check();

  DesignFuncLSQC df(order,orthogonal);
  
  SampleC<VectorC> inSamples;
  SampleC<VectorC> outSamples;
  
  if(!ReadData(infile,noOutputs,inSamples,outSamples)) {
    // Assume error has already been reported.
    return 1;
  }
  if(swap)
    RavlN::Swap(inSamples,outSamples);
  
  std::cout << "Got " << inSamples.Size() << " samples. \n";
  if(inSamples.Size() < 2) {
    std::cerr << "Not enought data!\n";
    return 1;
  }
  FunctionC func = df.Apply(inSamples,outSamples);
  if(!func.IsValid()) {
    std::cerr << "Failed to design function.\n";
    return 1;
  }
  func.Describe(std::cout);
  if(!outfile.IsEmpty()) {
    if(!Save(outfile,func)) {
      std::cerr << "Error writting file " << outfile << ".\n";
      return 1;
    }
  }
  if(residual) {
    VectorC sumOfSqr(noOutputs);
    sumOfSqr.Fill(0);
    for(DataSet2IterC<SampleC<VectorC>,SampleC<VectorC> > it(inSamples,outSamples);it;it++) {
      VectorC eval = func.Apply(it.Data1());
      VectorC diff = (it.Data2() - eval);
      if(verbose) {
	std::cout << " " << eval << " : " << it.Data2() <<"\n";
      }
      
      sumOfSqr += diff * diff;
    }
    std::cout << "Residuals:" << (sumOfSqr/inSamples.Size()) << " (Sum of the squares of the difference.) \n";
  }
  return 0;
}
