// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2005, OmniPerception Ltd.
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//! lib=RavlZLib

#include "Ravl/Compress.hh"
#include "Ravl/SArray1dIter2.hh"

using namespace RavlN;

int testZArray();
int testZString();

int main(int nargs,char **args) {
  int ln;
  if((ln = testZArray()) != 0){
    std::cerr << "Test failed on line " << ln << "\n";
    return 1;
  }
  if((ln = testZString()) != 0){
    std::cerr << "Test failed on line " << ln << "\n";
    return 1;
  }

  return 0;
}

int testZArray()
{
  SArray1dC<char> testBuff(1024);
  for(int i = 0;i < 256;i++)
    testBuff[i] = i;
  SArray1dC<char> compressedData;
  if(!CompressZLib(testBuff,compressedData)) {
    cerr << "Test failed 1 \n";
    return __LINE__;
  }
  cerr << "Compressed size=" << compressedData.Size() << "\n";
  SArray1dC<char> testBuff2(1024);
  if(!DecompressZLib(compressedData,testBuff2,1024)) {
    cerr << "Test failed 2 \n";
    return __LINE__;
  }
  for(SArray1dIter2C<char,char> it(testBuff,testBuff2);it;it++) {
    if(it.Data1() != it.Data2()) {
      cerr << "Test failed 3 \n";
      return __LINE__;
    }
  }
  return 0;
}
int testZString()
{
  StringC testBuff;
  for(int i = 0;i < 256;i++)
    testBuff  += (char) i;
  StringC compressedData;
  if(!CompressZLib(testBuff,compressedData)) {
    cerr << "Test failed 1 \n";
    return __LINE__;
  }
  cerr << "Compressed size=" << compressedData.Size() << "\n";
  StringC testBuff2(1024);
  if(!DecompressZLib(compressedData,testBuff2,1024)) {
    cerr << "Test failed 2 \n";
    return __LINE__;
  }
  if(testBuff2.Size() != testBuff.Size())
    return __LINE__;
  for(int i = 0;i < testBuff2.Size();i++) {
    if(testBuff[i] != testBuff2[i]) {
      cerr << "Test failed 3 \n";
      return __LINE__;
    }
  }
  return 0;
}

