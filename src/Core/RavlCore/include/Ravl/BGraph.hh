// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVL_BGRAPH_HEADER
#define RAVL_BGRAPH_HEADER 1
///////////////////////////////////////////////////////////////
//! file="Ravl/Core/Container/Branch/BGraph.hh"
//! lib=RavlCore
//! userlevel=Normal
//! author="Charles Galambos"
//! date="19/2/1997"
//! docentry="Ravl.API.Core.Branch"
//! rcsid="$Id$"

#include "Ravl/BGraphBase.hh"

namespace RavlN {
  
  template<class NT,class ET> class BGraphEdgeC;
  template<class NT,class ET> class BGraphNodeC;
  template<class NT,class ET> class BGraphNodeHC;
  template<class NT,class ET> class BGraphLinearIterC;
  
  //: Branching graph.
  
  template<class NT,class ET>
  class BGraphC 
    : public BGraphTypesC 
  {
  public:
    inline BGraphC()
      : Base(*new BGraphBaseC())
    {}
    //: Default constructor.
    
    NodeHandleT InsNode(const NT &Data);
    // Add a node
    
    BGraphEdgeC<NT,ET> &InsEdge(NodeHandleT From,NodeHandleT To,const ET &Data);
    // Add an edge
    
    inline BGraphEdgeC<NT,ET> &InsEdge(BGraphNodeHC<NT,ET> From,BGraphNodeHC<NT,ET> To,const ET &Data)
    { return InsEdge(From.ID(),To.ID(),Data); }
    // Add an edge
    
    inline BGraphNodeC<NT,ET> &NodeObj(NodeHandleT H)
    { return *(static_cast<BGraphNodeC<NT,ET > *> (&Base->Node(H))); }
    // Convert a NodeHandle to a node.
    
    inline BGraphNodeHC<NT,ET> NodeObjH(NodeHandleT H);
    // Convert a NodeHandle to a node.
    
    inline NT & NodeData(NodeHandleT H);
    // Access node data.
    
    inline IntT NoNodes() const
    { return Base->NoNodes(); }
    // Get number of nodes.
    
    inline BGraphBaseC &BaseGraph()
    { return *Base; }
    // Access base graph.
    
    inline bool IsValid(NodeHandleT H) const
    { return Base->IsValid(H); }
    // Check a node handle is valid. (For debuging.)
    
    inline bool IsValid() const
    { return Base.IsValid(); }
    // Check its valid handle.
    
  protected:
    inline BGraphC(BGraphBaseC &BG);
    // Constructor.
  
    inline void DupNode(NodeHandleT ID);
    // Duplicate node.
  
  private:
    SmartPtrC<BGraphBaseC> Base;
  
    friend class BGraphLinearIterC<NT,ET>;
  };
  
}

#include "Ravl/BGraphNode.hh"
#include "Ravl/BGraphEdge.hh"

namespace RavlN {
  
  //////////////////////////////////////////////////
  
  template<class NT,class ET> 
  inline 
  BGraphNodeHC<NT,ET> BGraphC<NT,ET>::NodeObjH(BGraphTypesC::NodeHandleT H) 
  { return BGraphNodeHC<NT,ET>(NodeObj(H)); }

  ///////////////////////////////////////////////////////
  
  template<class NT,class ET> 
  inline 
  NT & BGraphC<NT,ET>::NodeData(BGraphTypesC::NodeHandleT H) 
  { return NodeObj(H).Data(); }
  
  ///////////////////////////////////////////////////////

  template<class NT,class ET> 
  BGraphTypesC::NodeHandleT BGraphC<NT,ET>::InsNode(const NT &Data) {
    Base = *new BGraphBaseC(*Base,1);
    BGraphNodeC<NT,ET> *tmp = new BGraphNodeC<NT,ET>(Data);
    Base->Insert(*tmp);
    //printf("Insert node %d \n",Tmp->ID().V());
    return tmp->ID();
  }
  ///////////////////////////////////////////////////////

  template<class NT,class ET> 
  inline 
  void BGraphC<NT,ET>::DupNode(BGraphTypesC::NodeHandleT ID) {
    RavlAssert(IsValid(ID));
    Base->Dup(*new BGraphNodeC<NT,ET>(NodeObj(ID)));
  }
  
  ///////////////////////////////////////////////////////
  
  template<class NT,class ET> 
  BGraphEdgeC<NT,ET> &BGraphC<NT,ET>::InsEdge(BGraphTypesC::NodeHandleT From,
					      BGraphTypesC::NodeHandleT To,
					      const ET &data) {
    Base = *new BGraphBaseC(*Base);
    DupNode(From);
    DupNode(To);
    BGraphEdgeC<NT,ET> *Tmp = new BGraphEdgeC<NT,ET>(From,
						     To,
						     data);
    Base->Insert(*Tmp);
    return *Tmp;
  }
  
}

#endif
