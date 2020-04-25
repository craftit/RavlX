// This file is part of RAVL, Recognition And Vision Library
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//////////////////////////////////////////////////////////////
//! rcsid="$Id: testSTL.cc 6723 2008-04-17 14:18:15Z craftit $"
//! lib=RavlCore
//! file="Ravl/Core/System/testSTL.cc"
//! docentry="Ravl.API.Core.Misc"
//! author="Robert Crida"

#include "Ravl/STL.hh"
#include "Ravl/UnitTest.hh"

using namespace RavlN;

int testVectorIntIO();
int testVectorStringIO();

int main()
{
  int err;
  if((err = testVectorIntIO()) != 0) {
    std::cerr << "Test failed line :" << err <<"\n";
    return 1;
  }
  if((err = testVectorStringIO()) != 0) {
    std::cerr << "Test failed line :" << err <<"\n";
    return 1;
  }

  std::cerr << "STL test passed. \n";
  return 0;
}

int testVectorIntIO() {
  vector<int> intVec;
  intVec.push_back(0);
  intVec.push_back(1);
  intVec.push_back(2);

  vector<int> loadedVec;
  if (!TestBinStreamIO(intVec, loadedVec)) return __LINE__;
  if (intVec != loadedVec) return __LINE__;
  return 0;
}

int testVectorStringIO() {
  vector<string> strVec;
  strVec.push_back("first");
  strVec.push_back("second");
  strVec.push_back("third");

  vector<string> loadedVec;
  if (!TestBinStreamIO(strVec, loadedVec)) return __LINE__;
  if (strVec != loadedVec) return __LINE__;
  return 0;
}

