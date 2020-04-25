// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//! rcsid="$Id$"
//! lib=RavlCore
//! file="Ravl/Core/Container/Graph/testGraph.cc"
//! docentry="Ravl.API.Core.Graphs"
//! author="Charles Galambos"
//! userlevel=Develop

#include "Ravl/Graph.hh"
#include "Ravl/IntC.hh"
#include "Ravl/Stream.hh"

using namespace RavlN;

GraphC<IntC,IntC> CreateGraph() 
{
  GraphC<IntC,IntC> G;
  GraphNodeIterC<IntC,IntC> From = G.InsNode(IntC(1));
  GraphNodeIterC<IntC,IntC> To = G.InsNode(IntC(2));
  GraphNodeIterC<IntC,IntC> Another = G.InsNode(IntC(3));
  G.InsEdge(From,To,IntC(4));
  G.InsEdge(To,From,IntC(4));
  G.InsEdge(From,Another,IntC(6));
  //G.InsEdge(From,Other,IntC(7));
  return G;
};

int main() {
  
  // Creating a graph.
  
  // Once this is fixed to use proper temp filenames
  // it can be included normaly.
  GraphC<IntC,IntC> x(CreateGraph());
  
  {
    OStreamC out("/tmp/testGraph");
    out << x;
  }
  
  GraphC<IntC,IntC> G;
  {
    IStreamC in("/tmp/testGraph");
    in >> G;
  }

  if(G.NoNodes() != x.NoNodes()) {
    std::cerr << "ERROR: Node number mismatch. \n";
    return 1;
  }
  
  if(G.NoEdges() != x.NoEdges()) {
    std::cerr << "ERROR: Edge number mismatch. \n";
    return 1;
  }

  // Interate nodes.
  UIntT nodes = 0;
  UIntT edges = 0;
  GraphNodeIterC<IntC,IntC> Iter(G);
  for(;Iter.IsElm();Iter.Next()) {
    std::cout << "Node:" << Iter.Data() << " \n";
    nodes++;
    GraphAdjIterC<IntC,IntC> LIt(Iter.In());
    for(;LIt.IsElm();LIt.Next()) {
      edges++;
      std::cout << " In  " << LIt.Data() << " : " << LIt.SourceData() << "->" << LIt.TargetData() << " \n";
    }
    LIt = Iter.Out();
    for(;LIt.IsElm();LIt.Next()) {
      edges++;
      std::cout << " Out  " << LIt.Data() << " : " << LIt.SourceData() << "->" << LIt.TargetData() << " \n";
    }
  }

  if(G.NoNodes() != nodes) {
    std::cerr << "ERROR: Iterator node number mismatch. \n";
    return 1;
  }
  
  if((G.NoEdges()*2) != edges) {
    std::cerr << "ERROR: Iterator edges number mismatch. \n";
    return 1;
  }
  
  std::cout << "Cyclic:" << G.IsCyclic() << " \n";

  if(!G.IsCyclic()) {
    std::cerr << "Cyclic check failed. \n";
    return 1;
  }
  
  return 0;
}

namespace RavlN {
  template
  class GraphC<IntC, int>;

  template
  class GraphNodeIterC<IntC, int>;

  template
  class GraphEdgeIterC<IntC, int>;

  template
  class GraphAdjIterC<IntC, int>;
}