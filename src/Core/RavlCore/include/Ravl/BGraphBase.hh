// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVL_BGRAPHBASE_HEADER
#define RAVL_BGRAPHBASE_HEADER 1
///////////////////////////////////////////////////////////////
//! file="Ravl/Core/Container/Branch/BGraphBase.hh"
//! lib=RavlCore
//! userlevel=Develop
//! author="Charles Galambos"
//! docentry="Ravl.API.Core.Branch"
//! date="19/02/1997"

#include "Ravl/SDArray1d.hh"
#include "Ravl/SmartPtr.hh"
#include "Ravl/Index.hh"

namespace RavlN {
  
  class BGraphEdgeBaseC;
  class BGraphBaseC;
  class BGraphNodeBaseC;
  class BGraphBaseAdjIterC;
  
  class BGraphTypesC {
  public:
    typedef IndexC NodeHandleT;
    typedef IntT Dir; // Directions based on edges.
    enum DirT { Fwd = 0,Bkw = 1 };
  };

  //! userlevel=Develop
  //: Base Edge in BGraph.
  
  class BGraphEdgeBaseC 
    : public RCBodyC, 
      public BGraphTypesC 
  {
  public:
    inline BGraphEdgeBaseC(NodeHandleT from,NodeHandleT to){
      Nodes[Fwd] = to;   // Link into forward list.
      Nodes[Bkw] = from;  // Link into backward list.
    }
    //: Constructor.
    
    inline NodeHandleT Node(Dir Way) const
    { return Nodes[Way]; }
    //: Get nodes at ends of edge.
    
    inline BGraphEdgeBaseC &NextEdge(Dir Way)
    { return *Next[Way]; }
    //: Get edges.
    
    void Dump(std::ostream &out);
    //: Dump to stream.
    
    inline SizeT Hash() const
    { return StdHash(reinterpret_cast<const void *>(this)); }
    //: Get hash value.
    
  private:
    NodeHandleT Nodes[2];       // Nodes linked by edge.
    SmartPtrC<BGraphEdgeBaseC> Next[2];// Next edges in list.
    
    friend class BGraphBaseC;
  };

  //! userlevel=Develop
  //: Base Node in BGraph.
  
  class BGraphNodeBaseC 
    : public RCBodyC, 
      public BGraphTypesC 
  {
  public:
    inline BGraphNodeBaseC()
    {}
    //: Constructor.
    
    inline BGraphNodeBaseC(const BGraphNodeBaseC &Oth)    
      : NodeID(Oth.NodeID)
    {
      IO[0] = Oth.IO[0];
      IO[1] = Oth.IO[1];
    }
    
    //: Copy constructor.
    
    inline NodeHandleT ID() const
    { return NodeID; }
    //: Get ID for node.
    
    inline SmartPtrC<BGraphEdgeBaseC> &Edge(Dir Way)
    { return IO[Way]; }
    //: Get edges.
    
    inline BGraphEdgeBaseC *Edges(Dir Way)
    { return IO[Way]; }
    //: Get edge lists.
    
    void Dump(std::ostream &out);
    //: Dump to stream.
    
    inline SizeT Hash() const
    { return NodeID.V(); }
    //: Get hash value.
    
    BGraphBaseAdjIterC In();
    //: Iterate links goind into this node.
    
    BGraphBaseAdjIterC Out();
    //: Iterate links goind out of this node.
    
  private:
    NodeHandleT NodeID; // Nodes ID.
    SmartPtrC<BGraphEdgeBaseC> IO[2]; // In & out from this node.
  
    friend class BGraphBaseC;
  };
  
  //! userlevel=Develop
  //: Base BGraph
  
  class BGraphBaseC 
    : public RCBodyC, 
      public BGraphTypesC 
  {
  public:
    inline BGraphBaseC()
    {}
    //: Constructor.
    
    inline BGraphBaseC(const BGraphBaseC &Oth)
      : Nodes(Oth.Nodes)
    {}
    //: Copy constructor.
    
    inline BGraphBaseC(const BGraphBaseC &Oth,SizeT Delta)
      : Nodes(Oth.Nodes,Delta)
    {}
    //: Copy constructor.
    // Expand array to add 'Delta' extra nodes.
    
    inline BGraphNodeBaseC &Node(NodeHandleT ID) {
      RavlAssert(Nodes[ID].IsValid()); 
      return *Nodes[ID]; 
    }
    //: Get a node.
    
    inline IntT NoNodes() const
    { return Nodes.Size(); }
    //: Get number of nodes.
    
    inline void Insert(BGraphNodeBaseC &node)  {
      //RavlAssert(IsNotConst());
      node.NodeID = Nodes.Add(node);
    }
    //: Insert a node, modifies current graph
    
    inline void Insert(BGraphEdgeBaseC &Edge);
    //: Insert an edge, modifies current graph.
    
    inline void Dup(BGraphNodeBaseC &nd) {
      //RavlAssert(IsNotConst());
      Nodes[nd.NodeID] = &nd;
    }
    //: Substitue a node structure.
    
    inline bool IsValid(NodeHandleT H) const
    { return (H >= 0) && (H < Nodes.Size()); }
    //: Check a node handle is valid. (For debuging.)
  
    void Dump(std::ostream &out);
    //: Dump to stream.
    
  private:
    SDArray1dC<SmartPtrC<BGraphNodeBaseC> > Nodes;  // Array of node ptrs.
  };
  
  //////////////////////////////////////////////////////////////
  
  inline 
  void BGraphBaseC::Insert(BGraphEdgeBaseC &Edge) {
    //RavlAssert(IsNotConst());
    // Link into forward list.
    BGraphNodeBaseC &To = Node(Edge.Nodes[Fwd]);
    Edge.Next[Fwd] = To.Edge(Fwd);
    To.Edge(Fwd) = Edge;
    
    // Link into backward list.
    BGraphNodeBaseC &From = Node(Edge.Nodes[Bkw]);
    Edge.Next[Bkw] = From.Edge(Bkw);
    From.Edge(Bkw) = Edge; 
  }
  
}
#endif
