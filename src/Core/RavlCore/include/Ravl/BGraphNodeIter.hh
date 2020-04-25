// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVL_BGRAPHNODEITER_HEADER
#define RAVL_BGRAPHNODEITER_HEADER 1
///////////////////////////////////////////////////////////////
//! file="Ravl/Core/Container/Branch/BGraphNodeIter.hh"
//! lib=RavlCore
//! userlevel=Normal
//! author="Charles Galambos"
//! docentry="Ravl.API.Core.Branch"
//! date="19/2/1997"
//! rcsid="$Id$"

#include "Ravl/BGraphNode.hh"
#include "Ravl/BGraph.hh"

namespace RavlN {
  
  //! userlevel=Normal
  //: Interate through nodes in a BGraph.
  
  template<class NT,class ET>
  class BGraphNodeIterC  {
  public:
    inline BGraphNodeIterC(BGraphC<NT,ET> &nGraph)
      : graph(nGraph)
    { First(); }
    //: Constructor.
    
    inline BGraphNodeIterC(BGraphC<NT,ET> &nGraph,BGraphNodeC<NT,ET> &Nd)
      : graph(nGraph),
	i(Nd.ID())
    {}
    //: Constructor.
    
    void First() 
    { i = 0; }
    //: Goto first node.
    
    void Next() 
    { i++; }
    //: Goto next node.
    
    void operator++(int)
    { Next(); }
    //: Goto next element in list.
    
    bool IsElm() const 
    { return i < graph.NoNodes(); }
    //: At a valid element ?
    
    operator bool() const
    { return IsElm(); }
    //: At a valid element ?
    
    NT &Data() 
    { return graph.NodeObj(i).Data(); }
    //: Get data.
    
    NT &operator*()
    { return graph.NodeObj(i).Data(); }
    //: Get data.    
    
    const NT &operator*() const
    { return graph.NodeObj(i).Data(); }
    //: Get data.    
    
    BGraphNodeC<NT,ET> &Node()
    { return graph.NodeObj(i); }
    //: Get Node.
    
  private:
    BGraphC<NT,ET> graph; // Graph.
    IndexC i;
  };

}
#endif
