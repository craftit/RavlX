// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVL_BGRAPHLINEARITER_HEADER
#define RAVL_BGRAPHLINEARITER_HEADER 1
///////////////////////////////////////////////////////
//! file="Ravl/Core/Container/Branch/BGraphLinearIter.hh"
//! lib=RavlCore
//! author="Charles Galambos"
//! date="25/11/1997"
//! docentry="Ravl.API.Core.Branch"
//! rcsid="$Id$"

#include "Ravl/BGraph.hh"
#include "Ravl/Stack.hh"
#include "Ravl/HSet.hh"
#include "Ravl/SArray1d.hh"

namespace RavlN {
  
  //! userlevel=Develop
  //: Base Graph linear iterator.
  
  class BGraphBaseLinearIterC 
    : public BGraphTypesC 
  {
  public:
    inline BGraphBaseLinearIterC(BGraphBaseC &aGraph)
      : Graph(aGraph)
    { First(); }
    //: Constructor
    
    bool First();
    //: Goto first.
    // Returns false if list is empty, true otherwise.
    
    bool Next();
    //: Goto next.
    // Return true if it at a valid element after operation.
    
    inline bool IsElm() const
    { return !Open.IsEmpty(); }
    //: At a valid element ?
    // Returns true if iterator is at a valid element.
    
    inline bool IsCycle() const
    { return NodesLeft != 0; }
    //: Is graph cyclic ? 
    // Results only valid after iteration complete.
    // i.e. IsElm() returns False.
    
    inline IntT NodesRemaining() const 
    { return NodesLeft; }
    //: Return the number of unprocessed nodes.
    // Directly after First(), this will be the
    // number of nodes in the graph - 1.
    
    inline NodeHandleT NodeH(void) const 
    { return Open.First(); }
    //: Get some data.
    
  protected:
    inline void DoneNode(NodeHandleT Nd);
    
    BGraphBaseC Graph;         // Relavent graph.
    StackC<NodeHandleT> Open;  // Queue of open nodes.
    SArray1dC<IntT> Mark;         // Array of markers.
    IntT NodesLeft;            // Number of unprocessed nodes. 
  };

  //! userlevel=Normal
  //: Graph linear iterator.
  // This expects the grapg to be acyclic

  template<class NT,class ET>
  class BGraphLinearIterC 
    : public BGraphBaseLinearIterC 
  {
  public:
    inline BGraphLinearIterC(BGraphC<NT,ET> &aGraph)
      : BGraphBaseLinearIterC(aGraph.BaseGraph())
    {}
    //: Construct.
    
    inline BGraphNodeC<NT,ET> &NodeObj(NodeHandleT H)
    { return *(static_cast<BGraphNodeC<NT,ET > *> (&Graph.Node(H))); }
    //: Convert a handle to a node.

    inline BGraphNodeC<NT,ET> &Node()
    { return NodeObj(NodeH()); }
    //: Get current node.
    
    inline NT &Data()
    { return Node().Data(); }
    //: Access data in current node.
  };  
  
}

#endif
