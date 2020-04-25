// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//////////////////////////////////////////////////////////////
// Graph/GrBConnIt.hh      12/12/96        By Charles Galambos
// $Id$
//! rcsid="$Id$"
//! lib=RavlCore
//! file="Ravl/Core/Container/Graph/GraphBConIt.cc"

#include <assert.h>
#include "Ravl/GraphBConIt.hh"

#define VDEBUG 0

#if VDEBUG
#include <stdio.h>
#define ONDEBUG(x) x
#else
#define ONDEBUG(x)
#endif

namespace RavlN {
  /////////////////////
  // Default constructor.
  
  GraphBaseConnIterC::GraphBaseConnIterC() 
    : done(),
      open()
  {}
  
  /////////////////////
  // Copy constructor.
  
  GraphBaseConnIterC::GraphBaseConnIterC(const GraphBaseConnIterC &Oth)
    : done(Oth.done),
      open(Oth.open)
  { // I have not tested the copy constructor for queues.
    assert(0);
  }
  
  /////////////////////
  // Constructor.
  
  GraphBaseConnIterC::GraphBaseConnIterC(const GraphNodeBaseC &Start) 
    : done(),
      open()
  {
    First(Start);
  }
  
  /////////////////////
  // Goto first node.
  
  void GraphBaseConnIterC::First(const GraphNodeBaseC &Nd) {
    open.Empty();
    done.Empty();
    Visited(Nd);
    open.InsLast(Nd);
    ONDEBUG(printf("GraphBaseConnIterC::First(), Adding:%d \n",Nd.Hash()));
  }
  
  ////////////////////
  // Goto next node, add current nodes outgoing links to those
  // in the open list.
  
  void GraphBaseConnIterC::Next(void) {
    assert(!open.IsEmpty());
    ONDEBUG(printf("GraphBaseConnIterC::Next(), %d \n",open.First().Hash()));
    GraphAdjIterBaseC It(open.GetFirst().Out());
    for(;It.IsElm();It.Next()) {
      if(!IsVisited(It.OtherNode())) {
	ONDEBUG(printf("GraphBaseConnIterC::Next(), Adding:%d \n",It.OtherNode().Hash()));
	AddOpen(It.OtherNode());
      } else
	ONDEBUG(printf("GraphBaseConnIterC::Next(), Have done:%d \n",It.OtherNode().Hash()));
    }
  }
  
  ////////////////////
  // Move to next node, don't consider any child of this 
  // node for investigation.
  
  void GraphBaseConnIterC::Prune(void) {
    assert(!open.IsEmpty());
    ONDEBUG(printf("GraphBaseConnIterC::Pruning(), %d \n",open.First().Hash()));
    open.DelFirst();
  }

}
  
