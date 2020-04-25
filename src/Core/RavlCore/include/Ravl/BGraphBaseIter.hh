// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVL_BGRAPHBASEITER_HEADER
#define RAVL_BGRAPHBASEITER_HEADER 1
///////////////////////////////////////////////////////////////
//! file="Ravl/Core/Container/Branch/BGraphBaseIter.hh"
//! lib=RavlCore
//! userlevel=Normal
//! author="Charles Galambos"
//! docentry="Ravl.API.Core.Branch"
//! date="26/02/1997"
//! rcsid="$Id$"

#include "Ravl/BGraphBase.hh"

namespace RavlN {
  
  //! userlevel=Develop
  //: Adjacency iterator base class.
  
  class BGraphBaseAdjIterC 
    : public BGraphTypesC 
  {
  public:
    inline BGraphBaseAdjIterC(BGraphNodeBaseC &nnode,Dir nWay)
      : way(nWay)
    { edgeP = static_cast<BGraphEdgeBaseC *> (nnode.Edges(nWay)); }
    //: Constructor.
    
    inline void Next()
    { edgeP = &edgeP->NextEdge(way); }
    //: Goto next element
    
    void operator++(int)
    { Next(); }
    //: Goto next element
    
    inline bool IsElm() const
    { return edgeP != 0; }
    //: At valid element ?
    
    operator bool() const
    { return edgeP != 0; }
    //: At a valid element.
    
    inline BGraphEdgeBaseC &Edge() 
    { return *edgeP; }
    //: Access edge.
    
    inline const BGraphEdgeBaseC &Edge() const 
    { return *edgeP; }
    //: Access edge.
    
    NodeHandleT FarNodeH() const 
    { return edgeP->Node(!way); }
    //: Get node handle at other end.
    
    NodeHandleT NearNodeH() const 
    { return edgeP->Node(way); }
    //: Get node handle for this end.
    
    void Dump();
    //: Dump to stdout.
  private:
    BGraphEdgeBaseC *edgeP;
    Dir way;
  };
  
  //! userlevel=Develop
  //: Node iterator base class.
  
  class BGraphBaseNodeIterC 
    : public BGraphTypesC 
  {
  public:
    inline BGraphBaseNodeIterC(BGraphBaseC &nGraph)
      : G(nGraph)
    { First(); }
    //: Constructor.
    
    inline BGraphBaseNodeIterC(BGraphBaseC &nGraph,NodeHandleT nd)
      : G(nGraph),
	i(nd)
    {}
    //: Constructor.
    
    void First() 
    { i = 0; }
    //: Goto first node.
    
    void Next() 
    { i++; }
    //: Goto next node.
    
    void operator++(int)
    { i++; }
    //: Goto next node.
    
    bool IsElm() const 
    { return i < G.NoNodes(); }
    //: At a valid element ?
    
    operator bool() const
    { return IsElm(); }
    //: At a valid element ?
    
    BGraphNodeBaseC &Node() 
    { return G.Node(i); }
    //: Get Node.
    
    NodeHandleT NodeH() const 
    { return i; }
    //: Node handle.
    
    void Dump(void);
    //: Dump to stdout.
  private:
    BGraphBaseC G; // Graph.
    IndexC i;
  };
  
}
#endif

