// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVL_BGRAPHNODE_HEADER
#define RAVL_BGRAPHNODE_HEADER 1
///////////////////////////////////////////////////////////////
//! file="Ravl/Core/Container/Branch/BGraphNode.hh"
//! lib=RavlCore
//! author="Charles Galambos"
//! date="19/2/1997"
//! docentry="Ravl.API.Core.Branch"
//! rcsid="$Id$"

#include "Ravl/BGraphBase.hh"

namespace RavlN {
  
  template<class NT,class ET> class BGraphNodeC;
  template<class NT,class ET> class BGraphEdgeC;
  template<class NT,class ET> class BGraphC;
  template<class NT,class ET> class BGraphAdjIterC;

  //! userlevel=Develop
  //: BGraph node

  template<class NT,class ET>
  class BGraphNodeC 
    : public BGraphNodeBaseC 
  {
  public:
    BGraphNodeC(const NT &nData)
      : BGraphNodeBaseC(),
	Dat(nData)
    {}
    //: Constructor.
    
    inline NT &Data()
    { return Dat; }
    //: Access data.
  
    inline const NT &Data() const
    { return Dat; }
    //: Constant access to data.
    
    inline IndexC ID() const
    { return BGraphNodeBaseC::ID(); }
    //: Get ID of node.
    
    inline bool operator==(const BGraphNodeC &oth) const
    { return oth.ID() == ID();  }
    //: Are they the same node ?
    
    inline BGraphAdjIterC<NT,ET> Out(BGraphC<NT,ET> &G);
    //: Get links leaving node.
  
    inline BGraphAdjIterC<NT,ET> In(BGraphC<NT,ET> &G);
    //: Get links entering node.
  
    inline SizeT Hash() const
    { return ID().V(); }
    //: Get hash value.
  
  private:
    NT Dat;
  
    friend class BGraphC<NT,ET>;
    friend class BGraphEdgeC<NT,ET>;
    friend class BGraphAdjIterC<NT,ET>;
  };

  //! userlevel=Normal
  //: BGraph node handle

  template<class NT,class ET>
  class BGraphNodeHC : public BGraphTypesC {
  public:
    inline BGraphNodeHC() 
    {}
    //: Default constructor.
    
    inline BGraphNodeHC(BGraphNodeC<NT,ET> &annode)
      : node(annode) 
    {}
    //: Default constructor.

    inline BGraphNodeHC(BGraphC<NT,ET> &g,BGraphTypesC::NodeHandleT nd)
      : node(g.NodeObj(nd)) 
    {}
    //: Default constructor.
    
    inline const BGraphNodeHC<NT,ET> &operator=(const BGraphNodeHC<NT,ET> &oth) {
      node=oth.node;
      return *this;
    }
    
    //: Assignment.
  
    inline bool IsValid() const
    { return node.IsValid(); }
    //: Is handle valid ?
    
    inline BGraphNodeC<NT,ET> & Node()
    { return *node; }
    //: Get Node.
  
    inline const BGraphNodeC<NT,ET> & Node() const
    { return *node; }
    //: Get Node.
    
    inline NT &Data()
    { return node->Data(); }
    //: Access data.
    
    inline const NT &Data() const
    { return node->Data(); }
    //: Constant access to data.
    
    inline bool operator==(const BGraphNodeHC<NT,ET> &oth) const
    { return node == oth.node; }
    //: Comparison.
    
    inline bool operator==(BGraphTypesC::NodeHandleT oth) const
    { return node->ID() == oth; }
    //: Comparison.
    
    inline BGraphAdjIterC<NT,ET> Out(BGraphC<NT,ET> &G);
    //: Get links leaving node.
  
    inline BGraphAdjIterC<NT,ET> In(BGraphC<NT,ET> &G);
    //: Get links entering node.
  
    inline void Invalidate()
    { node = 0; }
    //: Invalidate handle.
  
    inline SizeT Hash() const
    { return (SizeT) node->ID().V(); }
    //: Get hash value.
    
    inline IndexC ID() const
    { return node->ID(); }
    //: Get nodes ID.
  
  private:
    SmartPtrC<BGraphNodeC<NT,ET> > node;
  };

}

#include "Ravl/BGraph.hh"
#include "Ravl/BGraphEdgeIter.hh"

namespace RavlN {
  
  ////////////////////////////////////////////////////////////////////
  
  template<class NT,class ET> 
  inline 
  BGraphAdjIterC<NT,ET> BGraphNodeC<NT,ET>::Out(BGraphC<NT,ET> &G) 
  { return BGraphAdjIterC<NT,ET>(G.NodeObj(ID()),Bkw); }

  template<class NT,class ET> 
  inline 
  BGraphAdjIterC<NT,ET> BGraphNodeC<NT,ET>::In(BGraphC<NT,ET> &G) 
  { return BGraphAdjIterC<NT,ET>(G.NodeObj(ID()),Fwd); }
  

  ////////////////////////////////////////////////////////////////////
  
  template<class NT,class ET>
  inline 
  BGraphAdjIterC<NT,ET> BGraphNodeHC<NT,ET>::Out(BGraphC<NT,ET> &G) 
  { return BGraphAdjIterC<NT,ET>(G.NodeObj(ID()),Bkw); }

  template<class NT,class ET>
  inline 
  BGraphAdjIterC<NT,ET> BGraphNodeHC<NT,ET>::In(BGraphC<NT,ET> &G) 
  { return BGraphAdjIterC<NT,ET>(G.NodeObj(ID()),Fwd); }
  
}
#endif



