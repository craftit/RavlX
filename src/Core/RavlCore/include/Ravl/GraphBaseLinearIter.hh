// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef GRAPHBASELINEARITER_HEADER 
#define GRAPHBASELINEARITER_HEADER 1
//////////////////////////////////////////////////////////////
//! file="Ravl/Core/Container/Graph/GraphBaseLinearIter.hh"
//! lib=RavlCore
//! userlevel=Develop
//! author="Charles Galambos"
//! date="9/12/1996"
//! docentry="Ravl.API.Core.Graphs"
//! rcsid="$Id$"

#include "Ravl/GraphBase.hh"
#include "Ravl/Stack.hh"
#include "Ravl/SArray1d.hh"
#include "Ravl/IntC.hh"

namespace RavlN {
  //: Iterate through nodes of an acyclic directed graph in an
  //: order consistant with the nodes direction.
  // This effectively does a toplogical sort on the graph. <p>
  // NB. This class uses the node Markers !! So you can only use
  // single iter at a time.
  
  class GraphBaseLinearIterC {
  public:
    GraphBaseLinearIterC(GraphBaseC &aGraph);
    // Constructor
    
    bool First();
    // Goto first.
    
    bool Next();
    //: Goto next node.
    // AMMA compatability function, use ++
    
    void operator++(int)
      {  Next(); }
    //: Goto next node.
    
    bool operator++()
      { return Next(); }
    //: Goto next node.
    // Return true if it is a valid element.
    
    bool IsElm() const { return !open.IsEmpty(); }
    // At a valid element ?
    // AMMA compatability function, use 'operator bool()'
    
    operator bool() const
      { return !open.IsEmpty(); }
    //: Test if the iterator is valid.
    
    bool IsCycle() const { return nodesLeft != 0; }
    // Results only valid after iteration complete.
    // i.e. IsElm() returns False.
    
    IntT NodesRemaining() const { return nodesLeft; }
    // Return the number of unprocessed nodes.
    // Directly after First(), this will be the
    // number of nodes in the graph - 1.
    
    GraphNodeBaseC &Node(void) { return open.First(); }
    //: Get current node.
    
    const GraphNodeBaseC &Node(void) const { return open.First(); }
    //: Get current node.
    
  private:  
    inline void DoneNode(GraphNodeBaseC &Nd);
    
    GraphBaseC graph;           // Relavent graph.
    StackC<GraphNodeBaseC> open;// Queue of open nodes.
    SArray1dC<IntT> markers;     // Array of markers.
    IntT nodesLeft;             // Number of unprocessed nodes. 
  };
}

#endif
