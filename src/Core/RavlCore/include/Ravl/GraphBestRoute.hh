// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVL_GRBESTROUTE_HEADER
#define RAVL_GRBESTROUTE_HEADER 1
//////////////////////////////////////////
//! docentry="Ravl.API.Core.Graphs"
//! rcsid="$Id$"
//! file="Ravl/Core/Container/Graph/GraphBestRoute.hh"
//! lib=RavlCore
//! author="Charles Galambos"
//! date="13/08/98"
//! userlevel=Normal

#include "Ravl/Graph.hh"
#include "Ravl/Hash.hh"
#include "Ravl/DList.hh"
#include "Ravl/PriQueue.hh"
#include "Ravl/StdMath.hh"

namespace RavlN {
  
  //////////////////////////////////
  //! userlevel=Normal
  //: Find the shortest route between to nodes in a graph.
  
  template<class NodeT,class EdgeT,class CostT>  
  DListC<GraphEdgeIterC<NodeT,EdgeT> > GraphBestRoute(const GraphC<NodeT,EdgeT> &graph,
						      GraphNodeHC<NodeT,EdgeT> from,
						      GraphNodeHC<NodeT,EdgeT> to,
						      CostT &costOut,
						      CostT (*EvalT)(const EdgeT &dat))
	{
    PriQueueC<CostT,GraphNodeHC<NodeT,EdgeT> > open(64); // List of open nodes.
    GraphNodeHC<NodeT,EdgeT> at = from;               // Place marker in graph.
    HashC<GraphNodeHC<NodeT,EdgeT>,CostT> dist(21); // Cost mapping.
    open.Insert(0,from); // Setup start node.
    dist[from] = 0;
    // Fill all distances until 'to' is found.
    CostT maxdist(0);
    bool found = false;
    while(!open.IsEmpty()) {
      CostT ccost = open.TopKey();
      if(ccost > maxdist)
        maxdist = ccost;
      at = open.GetTop();
      if(at == to) {
        found = true;
        break;
      }
      for(GraphAdjIterC<NodeT,EdgeT> it(at.Out());it.IsElm();it.Next()) {
        if(it.OtherNodeH() == at)
          continue; // Ignore loops to self.
        RealT distval = EvalT(it.Data());
        RavlAssert(distval >= 0); // All distances must be positive, else we'll loop forever.
        RealT val = ccost + distval;
        if(dist.IsElm(it.OtherNode())) {
          if(dist[it.OtherNode()] <= val)
            continue;
          }
        dist[it.OtherNode()] = val;
        open.Insert(val,it.OtherNode());
      }
    }
    DListC<GraphEdgeIterC<NodeT,EdgeT> > ret;
    if(!found) 
      return ret; // Not found, return empty list.
    costOut = dist[to];
    // Backtrack along shortest path.
    // FIXME :- Would this be faster if we remembered the the link that had
    // the shortest path to each node ? That way back tracking would be
    // a trivial operation.  Probably depends on the properties of the graph.
    CostT lastDist = maxdist;
    while(at != from) {
      GraphEdgeIterC<NodeT,EdgeT> back;
      GraphAdjIterC<NodeT,EdgeT> it(at.In());
      while(!dist.IsElm(it.OtherNode()) || it.OtherNodeH() == at) {
        it++;
        RavlAssert(it); // There should be at least 1 node with a distance.
      }
      CostT mindist = dist[it.OtherNode()] + EvalT(it.Data());
      back = it.Edge();
      GraphNodeHC<NodeT,EdgeT> nat = it.OtherNode();
      for(it++;it;it++) {
        // Ignore loops to self and nodes with no distances.
        if(!dist.IsElm(it.OtherNode()) || it.OtherNodeH() == at)
          continue;
        CostT cv = dist[it.OtherNode()] + EvalT(it.Data());
        if(cv <= mindist) {
          mindist = cv;
          back = it.Edge();
          nat = it.OtherNode();
          // If the cost is equal or smaller than that of the current node we know
          // we've found the smallest.
          // Note: It may not be exactly equal because of rounding errors, so if its
          // more we have to continue until we've checked all posiblities.
          if(mindist <= lastDist)
            break;
        }
      }
      lastDist = dist[nat];
      RavlAssert(back.IsValid()); // Just to check sanity.
      ret.InsFirst(back);
      at = nat;
    }
    return ret;
  }
  
}
#endif
