// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVL_BGRAPHEDGEITER_HEADER
#define RAVL_BGRAPHEDGEITER_HEADER 1
///////////////////////////////////////////////////////////////
//! file="Ravl/Core/Container/Branch/BGraphEdgeIter.hh"
//! lib=RavlCore
//! userlevel=Normal
//! author="Charles Galambos"
//! docentry="Ravl.API.Core.Branch"
//! date="19/2/1997"
//! rcsid="$Id$"

#include "Ravl/BGraphBaseIter.hh"
#include "Ravl/BGraphNode.hh"
#include "Ravl/BGraphEdge.hh"
#include "Ravl/BGraph.hh"

namespace RavlN {
  
  //! userlevel=Normal
  //: BGraph Adjacency iterator.
  
  template<class NT,class ET>
  class BGraphAdjIterC 
    : public BGraphBaseAdjIterC 
  {
  public:
    inline BGraphAdjIterC(BGraphNodeC<NT,ET> &nNode,Dir nWay)
      : BGraphBaseAdjIterC(nNode,nWay)
    {}
    //: Constructor.
    
    inline BGraphAdjIterC(const BGraphAdjIterC<NT,ET> &nOth)
      : BGraphBaseAdjIterC(nOth)
    {}
    //: Copy constructor.
    
    inline ET &Data()
    { return Edge().Data();  }
    //: Access data.
    
    inline const ET &Data() const
    { return Edge().Data();  }
    //: Constant access to data.
    
    inline BGraphEdgeC<NT,ET> &Edge()
    { return *((BGraphEdgeC<NT,ET> *) &BGraphBaseAdjIterC::Edge()); }
    //: Access edge.

    inline const BGraphEdgeC<NT,ET> &Edge() const
    { return *((BGraphEdgeC<NT,ET> *) &BGraphBaseAdjIterC::Edge()); }
    //: Access edge.

  };
  
}
#endif
