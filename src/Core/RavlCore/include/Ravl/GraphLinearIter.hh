// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVL_GRAPHLINEARITER_HEADER 
#define RAVL_GRAPHLINEARITER_HEADER 1
//////////////////////////////////////////////////////////////
//! file="Ravl/Core/Container/Graph/GraphLinearIter.hh"
//! lib=RavlCore
//! userlevel=Normal
//! author="Charles Galambos"
//! date="9/12/1996"
//! docentry="Ravl.API.Core.Graphs"
//! rcsid="$Id$"

#include "Ravl/GraphBaseLinearIter.hh"
#include "Ravl/GraphNode.hh"

namespace RavlN {

  //: Linear graph iterator.
  // Iterate through nodes of an acyclic directed graph in an
  // order consistant with the nodes direction. <p>
  // Expects the graph to be acyclic. <p>
  // This effectively does a toplogical sort on the graph. <p>
  // NB. This class uses the node Markers !! So you can only use
  // single iter at a time.
  
  template<class NodeT,class EdgeT>
  class GraphLinearIterC : public GraphBaseLinearIterC {
  public:
    GraphLinearIterC(GraphC<NodeT,EdgeT> &AGraph) 
      : GraphBaseLinearIterC(AGraph) {}
    //: Constructor
    
    inline GraphNodeIterC<NodeT,EdgeT> Node(void);
    //: Get a node iterator.
    
    inline NodeT &Data(void);
    //: Get some data.  
    
  };
  
  //////////////////////////
  // Get a node iterator.
  
  template<class NodeT,class EdgeT>
  inline GraphNodeIterC<NodeT,EdgeT> GraphLinearIterC<NodeT,EdgeT>::Node(void) {
    return GraphNodeIterC<NodeT,EdgeT>(GraphBaseLinearIterC::Node()); 
  }
  
  //////////////////////////
  // Get some data.
  
  template<class NodeT,class EdgeT>
  inline NodeT &GraphLinearIterC<NodeT,EdgeT>::Data(void) { 
    return Node().Data(); 
  }
}


#endif
