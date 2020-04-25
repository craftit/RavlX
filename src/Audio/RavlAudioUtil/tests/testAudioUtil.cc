// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2003, OmniPerception Ltd.
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//! rcsid="$Id$"
//! lib=RavlAudioUtil
//! author="Charles Galambos"
//! docentry="Ravl.API.Audio.Feature Extraction"
//! userlevel=Develop
//! file="Ravl/Audio/Util/testAudioUtil.cc"

#include "Ravl/Audio/WindowSignal.hh"
#include "Ravl/DP/ListIO.hh"
#include "Ravl/Random.hh"
#include "Ravl/DP/Compose.hh"

using namespace RavlAudioN;
using namespace RavlN;

int testSignalWindowFilters();
int testSignalWindow();

int main() {
  int ln;
  if((ln = testSignalWindowFilters()) != 0) {
    cerr << "Test failed on line " << ln << "\n";
    return 1;
  }
  if((ln = testSignalWindow()) != 0) {
    cerr << "Test failed on line " << ln << "\n";
    return 1;
  }
  return 0;
}

bool CheckSymmetric(SArray1dC<RealT> &filter) {
  if(filter.Size() & 1)
    return false;
  int size = filter.Size()-1;
  int halfSize = size/2;
  for(int i = 0;i < halfSize;i++) {
    if(Abs(filter[i] - filter[size - i]) > 0.02) {
      cerr << i << " " << filter[i] << " " << filter[size - i] <<"\n";
      return false;
    }
  }
  cerr << "Gain=" << (filter.Sum() / ((RealT) filter.Size())) << "\n";
  return true;
}

int testSignalWindowFilters() {
  
  WindowSignalC<IntT,IntT> test1(RAWRamp,128,32);
  //cerr <<" Filter=" << test1.Filter() << "\n";
  if(test1.Filter().Size() != 128) return __LINE__;
  if(!CheckSymmetric(test1.Filter())) return __LINE__;
  
  WindowSignalC<IntT,IntT> test2(RAWHamming,128,32);
  //cerr <<" Filter=" << test2.Filter() << "\n";
  if(!CheckSymmetric(test2.Filter())) return __LINE__;
  if(test2.Filter().Size() != 128) return __LINE__;

  WindowSignalC<IntT,IntT> test3(RAWHanning,128,32);
  //cerr <<" Filter=" << test3.Filter() << "\n";
  if(!CheckSymmetric(test3.Filter())) return __LINE__;
  if(test3.Filter().Size() != 128) return __LINE__;
  
  WindowSignalC<IntT,IntT> test4(RAWBlackman,128,32);
  if(!CheckSymmetric(test4.Filter())) return __LINE__;
  if(test4.Filter().Size() != 128) return __LINE__;
  //cerr <<" Filter=" << test4.Filter() << "\n";
  
  return 0;
}

int testSignalWindow() {
  DListC<IntT> list;
  for(int i= 0;i < 1000;i++)
    list.InsLast(i);
  DPISListC<IntT> ip(list);
  IntT frameSep = 32;
  WindowSignalC<IntT,IntT> filter(RAWNone,128,frameSep);
  DPIPortC<SArray1dC<IntT> > op = ip >> filter;
  SArray1dC<IntT> data;
  for(int j = 0;j < 4;j++) {
    if(!op.Get(data)) return __LINE__;
    if(data.Size() != 128) return __LINE__;
    //cerr << data << "\n";
    int i = j * frameSep;
    for(SArray1dIterC<IntT> it(data);it;it++,i++)
      if(*it != i) return __LINE__;
  }
  return 0;
}
