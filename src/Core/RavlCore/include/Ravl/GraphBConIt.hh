// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVLGraphBaseConnIterC_HEADER
#define RAVLGraphBaseConnIterC_HEADER 1
//////////////////////////////////////////////////////////////
//! file="Ravl/Core/Container/Graph/GraphBConIt.hh"
//! lib=RavlCore
//! userlevel=Develop
//! date="12/12/1996"
//! author="Charles Galambos"
//! docentry="Ravl.API.Core.Graphs"
//! rcsid="$Id$"

#include "Ravl/GraphBase.hh"
#include "Ravl/BlkQueue.hh"
#include "Ravl/HSet.hh"

namespace RavlN {
  //: Connected node iterator
  // Iterate all nodes that maybe reached from the given starting 
  // point (This may not necessarily include the start point itself).
  
  class GraphBaseConnIterC {
  public:
    GraphBaseConnIterC();
    // Default constructor.
    
    GraphBaseConnIterC(const GraphNodeBaseC &Start);
    // Constructor.
  
    GraphBaseConnIterC(const GraphBaseConnIterC &Oth);
    // Copy constructor.
    
    void First(const GraphNodeBaseC &Nd);
    // Goto first node.
    
    void Next(void);
    // Goto next node, add current nodes outgoing links to those
    // in the open list.
    
    void Prune(void);
    // Move to next node, don't include this nodes
    // children for iteration.
    
    inline bool IsElm(void) const
      { return !open.IsEmpty(); }
    // At valid element.
    
    inline bool IsVisited(const GraphNodeBaseC &Nd) const
      { return done.IsMember(Nd); }
    // Have visited node ?
    
    inline void Visited(const GraphNodeBaseC &Nd) 
      { done += Nd; }
    // Flag node as being visited.
    
    inline bool AddOpen(const GraphNodeBaseC &Nd);
    // Add node to open list.
    
    inline GraphNodeBaseC &Data(void)
      { return open.First(); }
    // Get some data.
    
    inline const GraphNodeBaseC &Data(void) const
      { return open.First(); }
    // Get some data.
    
  private:
    HSetC<GraphNodeBaseC> done; // Table of visited nodes.
    BlkQueueC<GraphNodeBaseC> open; // Stack of open nodes.
  };

  //////////////////////////////////////////////////////////
  
  
  inline 
  bool GraphBaseConnIterC::AddOpen(const GraphNodeBaseC &Nd) { 
    open.InsLast(Nd); 
    Visited(Nd);
    return true;
  }
  
}

#endif
