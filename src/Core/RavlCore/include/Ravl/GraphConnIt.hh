// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef GRAPHCONNITER_HEADER 
#define GRAPHCONNITER_HEADER 1
//////////////////////////////////////////////////////////////
//! file="Ravl/Core/Container/Graph/GraphConnIt.hh"
//! lib=RavlCore
//! userlevel=Normal
//! author="Charles Galambos"
//! date="2/1/1997"
//! docentry="Ravl.API.Core.Graphs"
//! rcsid="$Id$"

#include "Ravl/GraphBConIt.hh"
#include "Ravl/GraphNode.hh"

namespace RavlN {
  //: Connected node iterator.
  // Iterate all nodes that maybe reached from the given starting 
  // point. This does not necessarily include the start point itself.
  
  template<class NodeT,class EdgeT>
  class GraphConnIterC : public GraphBaseConnIterC {
  public:
    GraphConnIterC(GraphNodeIterC<NodeT,EdgeT> &Nd) :
      GraphBaseConnIterC(Nd.BaseNodeIter()) {}
    // Constructor
    
    GraphConnIterC(GraphConnIterC<NodeT,EdgeT> &Oth) :
      GraphBaseConnIterC(Oth) {}
    // Copy constructor
    
    inline GraphNodeIterC<NodeT,EdgeT> Node(void)
      { return GraphNodeIterC<NodeT,EdgeT>(GraphBaseConnIterC::Data()); }
    // Get a node iterator.
    
    inline NodeT &Data(void)
      { return static_cast<GraphNodeDatC<NodeT,EdgeT> &>(GraphBaseConnIterC::Data().NodeRep()).Data(); }
    // Get some data.  
    
    inline bool operator==(const GraphNodeIterC<NodeT,EdgeT> &Nd) const
      { return (&Nd.BaseNodeIter().NodeRep() == &GraphBaseConnIterC::Data().NodeRep()); }
    // At a particular node ?
    
    void AddOpen(const GraphNodeIterC<NodeT,EdgeT> &Nd) 
      { GraphBaseConnIterC::AddOpen(Nd.BaseNodeIter()); }
    // Add node to open list.
    
  };
  

}

#endif
