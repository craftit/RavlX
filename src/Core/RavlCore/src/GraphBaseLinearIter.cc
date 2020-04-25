// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//////////////////////////////////////////////////////////////
// Graph/GraphLinearIter.cc   9/12/96    By Charles Galambos
// $Id$
//! rcsid="$Id$"
//! lib=RavlCore
//! file="Ravl/Core/Container/Graph/GraphBaseLinearIter.cc"

#include <assert.h>
#include "Ravl/GraphBaseLinearIter.hh"

#define DODEBUG 0

#if DODEBUG
#define ONDEBUG(x) x
#include <stdio.h>
#else
#define ONDEBUG(x)
#endif

namespace RavlN {
  
  ///////////////////////////
  // Constructor
  
  GraphBaseLinearIterC::GraphBaseLinearIterC(GraphBaseC &aGraph)
    : graph(aGraph),
      markers(aGraph.NodeIDs()),
      nodesLeft(0)
  { First(); }
  
  /////////////////////////
  // Done a node.
  
  inline void GraphBaseLinearIterC::DoneNode(GraphNodeBaseC &nd) {
    //if(Nd.Out().IsElm())
    //   assert(Nd.Out().Node() == Nd);
    
    for(GraphAdjIterBaseC lI(nd.Out());lI.IsElm();lI.Next()) {
      if(--(markers[lI.OtherNode().ID()]) < 1) {
	open.Push(lI.OtherNode());  // Add to list of open nodes.
	ONDEBUG(printf("GraphBaseLinearIterC::DoneNode(), Open:%p \n",&lI.Node().NodeRep()));
      }
    }
    RavlAssert(nodesLeft > 0);
    nodesLeft--;
  }
  
  //////////////////////
  // Goto first.
  
  bool GraphBaseLinearIterC::First() {
    //markers.Fill(0);
    open.Empty();
    nodesLeft = 0;
    for(GraphNodeBaseC iter(graph.Nodes());iter.IsElm();iter.Next()) {
      nodesLeft++;
      IntT aCount = 0;
      for(GraphAdjIterBaseC lI(iter.In());lI.IsElm();lI.Next())
	aCount++;
      if(aCount == 0) {
	open.Push(iter); // Add to list of open nodes.
	ONDEBUG(printf("GraphBaseLinearIterC::First(), Open:%p %d \n",&iter.NodeRep(),nodesLeft));
      } else {
	markers[iter.ID()] = aCount; // Add count to table.
	ONDEBUG(printf("GraphBaseLinearIterC::First(), In:%p Left:%d \n",&iter.NodeRep(),aCount));
      }
    }
    if(open.IsEmpty())
      return false;
    return true;
  }

  //////////////////////
  // Goto next.

  bool GraphBaseLinearIterC::Next() {
    RavlAssert(!open.IsEmpty());
    ONDEBUG(printf("GraphBaseLinearIterC::Next(), ANode:%p Left:%d\n",&Data().NodeRep(),nodesLeft));
    GraphNodeBaseC aNode = open.Pop();
    DoneNode(aNode);
    if(open.IsEmpty())
      return false;
    return true;
  }
  
}



