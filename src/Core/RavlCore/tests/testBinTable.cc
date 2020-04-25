// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//! rcsid="$Id$"
//! lib=RavlCore
//! file="Ravl/Core/Container/Misc/testBinTable.cc"
//! author="Charles Galambos"
//! docentry="Ravl.API.Core.Misc"
//! userlevel=Develop

#include "Ravl/BinTable.hh"
#include "Ravl/BinIter.hh"
#include "Ravl/BinList.hh"
#include "Ravl/TFVector.hh"
#include "Ravl/Index2d.hh"
#include "Ravl/String.hh"

using namespace RavlN;

int main() {
  TFVectorC<RealT,2> scale; scale[0] = 2; scale[1] = 2;
  BinTableC<TFVectorC<RealT,2>,Index2dC,StringC> tab(scale);
  TFVectorC<RealT,2> at1; at1[0] = 1; at1[1] = 1;
  TFVectorC<RealT,2> at2; at2[0] = 2; at2[1] = 1;
  TFVectorC<RealT,2> at3; at3[0] = 3; at3[1] = 1; 
  tab[at1] = "Hello1";
  tab[at2] = "Hello2";
  tab[at3] = "Hello3";
  BinIterC<TFVectorC<RealT,2>,Index2dC,StringC> it(tab);
  for(;it.IsElm();it.Next())
    std::cout << it.Data();
  
  TFVectorC<RealT,2> at4; at4[0] = -1; at4[1] = -1;
  TFVectorC<RealT,2> at5; at5[0] = -3.2; at5[1] = -3.2;
  TFVectorC<RealT,2> bincentre1 = tab.BinCentre(at1);
  TFVectorC<RealT,2> bincentre4 = tab.BinCentre(at4);
  TFVectorC<RealT,2> bincentre5 = tab.BinCentre(at5);
  
  std::cout << " Bin centre for " << at1 << " is " <<  bincentre1 << "\n";
  std::cout << " Bin centre for " << at4 << " is " <<  bincentre4 << "\n";
  std::cout << " Bin centre for " << at5 << " is " <<  bincentre5 << "\n";
  
  std::cout << at1 << " falls in bin " <<  tab.Scale(at1) << "\n";
  std::cout << at4 << " falls in bin " <<  tab.Scale(at4) << "\n";
  std::cout << at5 << " falls in bin " <<  tab.Scale(at5) << "\n";
  
  return 0;
}

namespace RavlN {
  template
  class BinTableC<TFVectorC<RealT, 2>, Index2dC, StringC>;

  template
  class BinIterC<TFVectorC<RealT, 2>, Index2dC, StringC>;
}
