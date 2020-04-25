// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVL_DABGRAPH_HEADER
#define RAVL_DABGRAPH_HEADER 1
/////////////////////////////////////////////////////////////
//! file="Ravl/Core/Container/Branch/BDAGraph.hh"
//! lib=RavlCore
//! userlevel=Normal
//! author="Charles Galambos"
//! date="21/2/1997"
//! docentry="Ravl.API.Core.Branch"
//! rcsid="$Id$"

#include "Ravl/BGraph.hh"
#include "Ravl/BGraphEdge.hh"
#include "Ravl/BGraphEdgeIter.hh"
#include "Ravl/BlkQueue.hh"
#include "Ravl/SArray1d.hh"

namespace RavlN {
  
  //! userlevel=Normal
  //: Directed acyclic graph.
  
  template<class NT,class ET>
  class BDAGraphC 
    : public BGraphC<NT,ET> 
  {
  public:
    inline BDAGraphC()
    {}
    //: Default constructor.
    
    inline BDAGraphC(const BDAGraphC<NT,ET> &Oth)
      : BGraphC<NT,ET>(Oth)
    {}
    //: Constructor.
    
    bool TryEdge(BGraphTypesC::NodeHandleT fromNode,BGraphTypesC::NodeHandleT toNode);
    //: Test if adding an edge is possible.
  
    inline bool TryEdge(BGraphNodeHC<NT,ET> fromNode,BGraphNodeHC<NT,ET> toNode) 
    { return TryEdge(fromNode.ID(),toNode.ID()); }
    //: Test if adding an edge is possible.
    
    inline BGraphEdgeHC<NT,ET> InsEdge(BGraphTypesC::NodeHandleT From,
				       BGraphTypesC::NodeHandleT To,const ET &Data) {
      if(!TryEdge(From,To))
	return BGraphEdgeHC<NT,ET>();
      return BGraphEdgeHC<NT,ET>(BGraphC<NT,ET>::InsEdge(From,To,Data));
    }
    //: Add an edge, test if possible first.
    
    inline BGraphEdgeHC<NT,ET> InsEdge(BGraphNodeHC<NT,ET> From,BGraphNodeHC<NT,ET> To,const ET &Data) 
    { return InsEdge(From.ID(),To.ID(),Data); }
    
  };
  
  ////////////////////////////////////////////////////////////////////////////
  
  template<class NT,class ET>
  bool BDAGraphC<NT,ET>::TryEdge(BGraphTypesC::NodeHandleT fromNode,BGraphTypesC::NodeHandleT toNode) {
    SArray1dC<bool> done(this->NoNodes());
    done.Fill(false);
    if(fromNode == toNode)
      return false;
    BlkQueueC<BGraphTypesC::NodeHandleT> open;
    open.InsLast(toNode);
    done[toNode] = true;
    while (!open.IsEmpty()) {
      for(BGraphAdjIterC<NT,ET> It(this->NodeObj(open.GetFirst()),this->Bkw);
	  It.IsElm();
	  It.Next()) {
	BGraphTypesC::NodeHandleT NN = It.FarNodeH();
	if(done[NN])
	  continue;
	if(NN == fromNode)
	  return false;
	done[NN] = true;
	open.InsLast(NN);
      }
    }
    return true;
  }
  
  
}
#endif
