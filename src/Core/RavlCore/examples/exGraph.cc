// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//! rcsid="$Id$"
//! lib=RavlCore
//! file="Ravl/Core/Container/Graph/exGraph.cc"
//! author="Charles Galambos"
//! date="10/12/1996"
//! docentry="Ravl.API.Core.Graphs"
//! userlevel=Normal

#include "Ravl/Graph.hh"

using namespace RavlN;

int main() {

  GraphC<int,int> graph;

  GraphNodeHC<int,int> node1 = graph.InsNode(1);
  GraphNodeHC<int,int> node2 = graph.InsNode(2);
  GraphNodeHC<int,int> node3 = graph.InsNode(3);
  
  GraphEdgeIterC<int,int> edge1 = graph.InsEdge(node1,node2,1);
  GraphEdgeIterC<int,int> edge2 = graph.InsEdge(node1,node3,2);
  
  // Go through edges that leave node1.
  
  for(GraphAdjIterC<int,int> adjIter(node1.Out());adjIter;adjIter++)
    std::cerr << "Out edge " << adjIter.Data() << "\n";
  
  // Go through edges that enter node2.

  for(GraphAdjIterC<int,int> adjIter(node2.In());adjIter;adjIter++)
    std::cerr << "In edge " << adjIter.Data() << "\n";
  
  // Go through all nodes in graph.
  
  for(GraphNodeIterC<int,int> nodeIter(graph);nodeIter;nodeIter++) 
    std::cerr << "Node " << nodeIter.Data() << "\n";
  
  // Go through all edges in graph.

  for(GraphEdgeIterC<int,int> edgeIter(graph);edgeIter;edgeIter++) 
    std::cerr << "Edge " << edgeIter.Data() << "\n";
  
  return 0;
}
