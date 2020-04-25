// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
///////////////////////////////////////////////////////////////
//! rcsid="$Id$"
//! lib=RavlCore
//! file="Ravl/Core/Container/Branch/testBGraph.cc"
//! userlevel=Develop
//! author="Charles Galambos"
//! docentry="Ravl.API.Core.Branch"

//: Test Branching Graphs.

#include "Ravl/BDAGraph.hh"
#include "Ravl/BGraphNodeIter.hh"
#include "Ravl/BGraphEdgeIter.hh"
#include "Ravl/HSet.hh"
#include "Ravl/Stream.hh"

using namespace RavlN;

class DataC {
public:
  DataC() { a = 0; }
  //: Constructor.
  
  DataC(int v) { a = v; }
  //: Constructor.
  
  operator int() { return a; }
  // Conv to int.
  
  bool operator==(const DataC &oth)
  { return a == oth.a; }
  //: Comparison.
  
  int a;
};

int testBGraph();

int main() {
  int ln;
  std::cout << "Testing branching graphs... \n";
  if((ln = testBGraph()) != 0) {
    std::cerr << "Test faile at line " << ln << "\n";
    return 1;
  }
  std::cout << "Test passed ok. \n";
  return 0;
}


int testBGraph() {
  typedef BGraphTypesC::NodeHandleT NodeH;
  typedef int DataT;
  
  BDAGraphC<DataT,DataT> G;
  
  NodeH H1 = G.InsNode(1);
  NodeH H2 = G.InsNode(2);
  NodeH H3 = G.InsNode(3);
  NodeH H4 = G.InsNode(4);
  G.InsEdge(H1,H2,4);
  G.InsEdge(H2,H3,5);
  G.InsEdge(H4,H2,6);
  int ec = 0,nc = 0;
  HSetC<int> check;
  for(BGraphNodeIterC<DataT,DataT> It(G);
      It.IsElm();
      It.Next()) {
    check += It.Data();
    //cout << "Node:" << It.Data() << std::endl;
    nc++;
  }
  if(check.Size() != 4) return __LINE__;
  if(nc != 4) return __LINE__;
  check.Empty();
  for(BGraphAdjIterC<DataT,DataT> ItAdj(G.NodeObj(H2),BGraphTypesC::Fwd);
      ItAdj.IsElm();
      ItAdj.Next()) {
    //cout << "Edge:" << ItAdj.Data() << std::endl;
    check += ItAdj.Data();
    ec++;
  }
  if(check.Size() != 2) return __LINE__;
  if(ec != 2) return __LINE__;
  return 0;
}
