// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//////////////////////////////////////////////////////////////
// Contain/Branch/GraphLinearIter.cc   9/12/96    By Charles Galambos
// $Id$
//! rcsid="$Id$"
//! lib=RavlCore
//! file="Ravl/Core/Container/Branch/BGraphLinearIter.cc"

#include "Ravl/Assert.hh"
#include "Ravl/BGraphLinearIter.hh"
#include "Ravl/BGraphBaseIter.hh"

#define DODEBUG 0
#if DODEBUG
#define ONDEBUG(x) x
#else
#define ONDEBUG(x)
#endif

namespace RavlN {
  
  ////////////////////////
  // Done a node.
  
  inline void BGraphBaseLinearIterC::DoneNode(NodeHandleT Nd) {
    //if(Nd.Out().IsElm())
    //   RavlAssert(Nd.Out().OtherNode() == Nd);
    
    for(BGraphBaseAdjIterC li(Graph.Node(Nd).Out());li.IsElm();li.Next()) {
      if(--(Mark[li.FarNodeH()]) < 1) {
	Open.Push(li.FarNodeH());  // Add to list of open nodes.
	ONDEBUG(cerr << "GraphBaseLinearIterC::DoneNode(), Open:" << ((void *)&li.OtherNode().NodeRep()) << " \n");
      }
    }
    RavlAssert(NodesLeft > 0);
    NodesLeft--;
  }

  //////////////////////
  // Goto first.
  
  bool BGraphBaseLinearIterC::First() {
    Open.Empty();
    NodesLeft = 0;
    Mark = SArray1dC<IntT>(Graph.NoNodes());
    for(BGraphBaseNodeIterC iter(Graph);iter.IsElm();iter.Next()) {
      NodesLeft++;
      IntT ACount = 0;
      for(BGraphBaseAdjIterC li(iter.Node().In());li.IsElm();li.Next())
	ACount++;
      if(ACount == 0) {
	Open.Push(iter.NodeH()); // Add to list of open nodes.
	ONDEBUG(cerr << "GraphBaseLinearIterC::First(), Open:" << ((void *) &iter.NodeRep()) << " " << nodesLeft << " \n");
      } else {
	Mark[iter.NodeH()]  = ACount; // Add count to table.
	ONDEBUG(cerr << "GraphBaseLinearIterC::First(), In:" << ((void *) &iter.NodeRep()) << " Left:" << aCount << " \n");
      }
    }
    if(Open.IsEmpty())
      return false;
    return true;
  }

  //////////////////////
  // Goto next.
  
  bool BGraphBaseLinearIterC::Next() {
    RavlAssert(!Open.IsEmpty());
    ONDEBUG(cerr << "GraphBaseLinearIterC::Next(), ANode:" << ((void *) &Data().NodeRep()) << " Left:" << NodesLeft << "\n");
    NodeHandleT ANode = Open.Pop();
    DoneNode(ANode);
    if(Open.IsEmpty())
      return false;
    return true;
  }
}
