// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVL_GRAPHNODE_HEADER
#define RAVL_GRAPHNODE_HEADER 1
//////////////////////////////////////////////////////////////
//! file="Ravl/Core/Container/Graph/GraphNode.hh"
//! lib=RavlCore
//! author="Charles Galambos"
//! date="10/12/1996"
//! docentry="Ravl.API.Core.Graphs"
//! rcsid="$Id$"
//! userlevel=Normal
//! example=exGraph.cc

#include "Ravl/GraphBase.hh"
#include "Ravl/Assert.hh"

namespace RavlN {
  
  template<class NodeT,class EdgeT> class GraphNodeDatC;
  template<class NodeT,class EdgeT> class GraphEdgeIterC;
  template<class NodeT,class EdgeT> class GraphAdjIterC;
  template<class NodeT,class EdgeT> class GraphC;
  template<class NodeT,class EdgeT> class GraphNodeHC;
  template<class NodeT,class EdgeT> class GraphLinearIterC;
  template<class NodeT,class EdgeT> class GraphConnIterC;

  const int GraphNodeDat_CheckNum = 0x12345678;
  
  //! userlevel=Develop
  //: Graph node data.
  
  template<class NodeT,class EdgeT>
  class GraphNodeDatC 
    : public GraphNodeBaseBodyC 
  {
  public:  
    inline GraphNodeDatC(GraphBaseBodyC &gr,const NodeT &Dat,UIntT nid);
    // Constructor.
    
    ~GraphNodeDatC();
    // Destructor.
    
    NodeT &Data(void) { return UserData; }  
    // Get at data.
    
    const NodeT &Data(void) const { return UserData; }
    // Get at data.
    
#if RAVL_GRAPH_DEBUG
    bool IsValid() const { return CheckVal == GraphNodeDat_CheckNum; }
#else
    bool IsValid() const { return true; }
#endif
  protected:  
    NodeT UserData;
#if RAVL_GRAPH_DEBUG
    int CheckVal;
#endif
    friend class GraphNodeHC<NodeT,EdgeT>;
  };
  
  //! userlevel=Normal
  //: Node iterator.
  // Iterates through all the nodes in a graph.
  
  template<class NodeT,class EdgeT>
  class GraphNodeIterC 
    : public GraphNodeBaseC 
  {
  public:
    GraphNodeIterC(GraphC<NodeT,EdgeT> &AGraph);  
    // Constructor.
    
    inline GraphNodeIterC();  
    // Default constructor.
    
    inline GraphNodeIterC(const GraphNodeIterC<NodeT,EdgeT> &Another);
    // Copy constructor.
    
    ~GraphNodeIterC();
    // Destructor.
    
    inline const GraphNodeIterC<NodeT,EdgeT> &operator=(const GraphNodeIterC<NodeT,EdgeT> &Oth);
    // Assigment.
    
    inline bool operator==(const GraphNodeIterC<NodeT,EdgeT> &Oth) const;
    // Comparison.
    
    inline bool operator!=(const GraphNodeIterC<NodeT,EdgeT> &Oth) const;
    // Comparison.
    
    inline NodeT &Data(void);
    // Access data.
    
    inline const NodeT &Data(void) const;
    // Access data.
    
    inline GraphAdjIterC<NodeT,EdgeT> In();
    // Returns the iterator that is set to the first item of the list
    // of the graph adjacencies in which this node is the target node.
    
    inline GraphAdjIterC<NodeT,EdgeT> Out();
    // Returns the iterator that is set to the first item of the list
    // of the graph adjacencies in which this node is the source node.
    
    inline void Invalidate();
    // Invalidate handle, IsValid() will return false after this
    // call until the handle is assigmed to another node.
    
    inline void Del();
    // Unlinks edge from the graph and deletes it. The  iterator
    // will point to the previous edge in the graph edge list.  
    
    inline void DelMoveNext();
    // Unlinks edge from the graph and deletes it. The  iterator
    // will point to the next edge in the graph edge list.  
    
#if RAVL_GRAPH_DEBUG
    bool IsNodeValid() const { 
      if(!IsElm()) return true;
      return NodeDat().IsValid();
    }
    bool IsIterValid() const { 
      if(!IsElm()) return false;
      return NodeDat().IsValid();
    }
#else
    bool IsNodeValid() const { return true; }
    bool IsIterValid() const { return true; }
#endif
    
  protected:
    GraphNodeBaseC &BaseNodeIter() { return *this; }
    // Access to the base graph node iterator.
    // Use ONLY for implementing new base graph operations/iteratiors.
    
    const GraphNodeBaseC &BaseNodeIter() const { return *this; }
    // Access to the base graph node iterator.
    // Use ONLY for implementing new base graph operations/iterators.
    
    GraphNodeIterC(const GraphNodeBaseC &aNode) 
      : GraphNodeBaseC(aNode) 
      {}
    // Construct from a base node iterator.
    
    GraphNodeIterC(GraphNodeBaseBodyC &aNode,GraphBaseC &bg) 
      : GraphNodeBaseC(aNode,bg)
      {}
    // Construct from a base node iterator.

    GraphNodeIterC(GraphNodeBaseBodyC &aNode,GraphBaseBodyC &bg) 
      : GraphNodeBaseC(aNode,bg)
      {}
    // Construct from a base node iterator.
    
    inline GraphNodeDatC<NodeT,EdgeT> &NodeDat();
    // Get a node rep.
    
    inline const GraphNodeDatC<NodeT,EdgeT> &NodeDat() const;
    // Get a node rep.
    
    friend class GraphC<NodeT,EdgeT>;
    friend class GraphLinearIterC<NodeT,EdgeT>;
    friend class GraphConnIterC<NodeT,EdgeT>;
    friend class GraphAdjIterC<NodeT,EdgeT>;
    friend class GraphEdgeIterC<NodeT,EdgeT>;
    friend class GraphNodeHC<NodeT,EdgeT>;
  };
  
  //: Node handle.
  // Handle for a single node in a graph.
  
  template<class NodeT,class EdgeT>
  class GraphNodeHC {
  public:
    inline GraphNodeHC();
    // Default constructor.
    
    inline GraphNodeHC(const GraphNodeHC<NodeT,EdgeT> &Oth);
    // Copy constructor.
    
    inline GraphNodeHC(const GraphNodeIterC<NodeT,EdgeT> &Oth);
    // Construct from a node iter.
    
    inline GraphNodeHC(GraphNodeDatC<NodeT,EdgeT> &Rep);
    // Construct form a node rep.
    
    inline bool IsValid() const;
    // Is a valid handle ?
    
    inline const GraphNodeHC<NodeT,EdgeT> &operator=(const GraphNodeHC<NodeT,EdgeT> &Oth);
    // Assigment.
    
    inline const GraphNodeHC<NodeT,EdgeT> &operator=(const GraphNodeIterC<NodeT,EdgeT> &Oth);
    // Assignment.
    
    inline bool operator==(const GraphNodeHC<NodeT,EdgeT> &Oth) const ;
    // Comparison.
    
    inline bool operator!=(const GraphNodeHC<NodeT,EdgeT> &Oth) const ;
    // Comparison.
    
    inline NodeT &Data(void);
    // Access data.
    
    inline const NodeT &Data(void) const;
    // Access data.
    
    inline GraphAdjIterC<NodeT,EdgeT> In();
    // Returns the iterator that is set to the first item of the list
    // of the graph adjacencies in which this node is the target node.
    
    inline GraphAdjIterC<NodeT,EdgeT> Out();
    // Returns the iterator that is set to the first item of the list
    // of the graph adjacencies in which this node is the source node.
    
    inline void Invalidate();
    // Invalidate handle, IsValid() will return false after this
    // call until the handle is assigmed to another node.
    
    inline void Del();
    // Unlinks edge from the graph and deletes it. The  iterator
    // will point to the previous edge in the graph edge list.  
    
    inline GraphNodeBaseBodyC & NodeRep() const { return *Node; }
    // Returns the node.
    
    inline GraphNodeDatC<NodeT,EdgeT> & NodeRef() { return *Node; }
    // Returns the node.
    
    //protected:
    GraphNodeHC(const GraphNodeBaseC &ANode) 
      : Node(&((GraphNodeIterC<NodeT,EdgeT>(ANode)).NodeDat())) {}
    // Construct from a base node iterator.
    
    UIntT Hash() const { 
#if RAVL_CPUTYPE_64
      return (UInt64T) Node >> 2; 
#else
      return (UIntT) Node >> 2; 
#endif
    }
    // Get hash value for node.
    
  private:
    GraphNodeDatC<NodeT,EdgeT> *Node;
  };
  
  template<class NodeT,class EdgeT>
  std::ostream &operator<<(std::ostream &strm,const GraphNodeHC<NodeT,EdgeT> &node) {
    RavlAssertMsg(0,"not implemented");
    return strm;
  }
  //: Text stream output.
  // Not implemented
  
  template<class NodeT,class EdgeT>
  std::istream &operator>>(std::istream &strm,GraphNodeHC<NodeT,EdgeT> &node) {
    RavlAssertMsg(0,"not implemented");
    return strm;
  }
  //: Text stream input.
  // Not implemented
  
  template<class NodeT,class EdgeT>
  BinOStreamC &operator<<(BinOStreamC &strm,const GraphNodeHC<NodeT,EdgeT> &node) {
    RavlAssertMsg(0,"not implemented");
    return strm;
  }
  //: Binary stream output.
  // Not implemented
  
  template<class NodeT,class EdgeT>
  BinIStreamC &operator>>(BinIStreamC &strm,GraphNodeHC<NodeT,EdgeT> &node) {
    RavlAssertMsg(0,"not implemented");
    return strm;
  }
  //: Binary stream input.
  // Not implemented
  
}

#include "Ravl/GraphEdge.hh"
#include "Ravl/GraphAdj.hh"
 
namespace RavlN {
  
  template<class NodeT,class EdgeT>
  inline 
  GraphNodeDatC<NodeT,EdgeT>::GraphNodeDatC(GraphBaseBodyC &graph,const NodeT &Dat,UIntT nid)
    : GraphNodeBaseBodyC(graph,nid),
      UserData(Dat)
  {
#if RAVL_GRAPH_DEBUG
    CheckVal = GraphNodeDat_CheckNum;
#endif 
  }

  template<class NodeT,class EdgeT>
  GraphNodeDatC<NodeT,EdgeT>::~GraphNodeDatC()  {
    RavlAssert(IsValid());
    GraphAdjIterC<NodeT,EdgeT> ItOut(Out(),BaseGraph());
    while(ItOut.IsElm())
      ItOut.DelMoveNext();
    GraphAdjIterC<NodeT,EdgeT> ItIn(In(),BaseGraph());
    while(ItIn.IsElm())
      ItIn.DelMoveNext();
#if RAVL_GRAPH_DEBUG
    CheckVal = 0xf0f0f0f0;
#endif
  }
  
  /////////////////////////////////////////////////////////////////////////
  // GraphNodeIterC.
  
  template<class NodeT,class EdgeT>
  inline GraphNodeIterC<NodeT,EdgeT>::GraphNodeIterC()
    : GraphNodeBaseC()
  {}
  
  template<class NodeT,class EdgeT>
  GraphNodeIterC<NodeT,EdgeT>::GraphNodeIterC(GraphC<NodeT,EdgeT> &aGraph) 
      : GraphNodeBaseC(aGraph.Nodes()) 
  {}

  template<class NodeT,class EdgeT>
  inline 
  GraphNodeIterC<NodeT,EdgeT>::GraphNodeIterC(const GraphNodeIterC<NodeT,EdgeT> &Another) 
   : GraphNodeBaseC(Another) 
  { RavlAssert(Another.IsNodeValid()); }

  template<class NodeT,class EdgeT>
  GraphNodeIterC<NodeT,EdgeT>::~GraphNodeIterC()
  { RavlAssert(IsNodeValid()); }

  template<class NodeT,class EdgeT>
  inline const GraphNodeIterC<NodeT,EdgeT> &
  GraphNodeIterC<NodeT,EdgeT>::operator=(const GraphNodeIterC<NodeT,EdgeT> &Oth)  {
    RavlAssert(Oth.IsNodeValid());
    ((GraphNodeBaseC &) *this) = Oth;
    return *this;
  }
  
  template<class NodeT,class EdgeT>
  inline bool 
  GraphNodeIterC<NodeT,EdgeT>::operator==(const GraphNodeIterC<NodeT,EdgeT> &Oth) const  { 
    RavlAssert(IsNodeValid());
    RavlAssert(Oth.IsNodeValid());
    return GraphNodeBaseC::operator==(Oth); 
  }
  
  template<class NodeT,class EdgeT>
  inline bool 
  GraphNodeIterC<NodeT,EdgeT>::operator!=(const GraphNodeIterC<NodeT,EdgeT> &Oth) const  { 
    RavlAssert(IsNodeValid());
    RavlAssert(Oth.IsNodeValid());
    return !GraphNodeBaseC::operator==(Oth); 
  }
  
  template<class NodeT,class EdgeT>
  inline NodeT &
  GraphNodeIterC<NodeT,EdgeT>::Data(void)  { 
    RavlAssert(IsIterValid());
    return static_cast<GraphNodeDatC<NodeT,EdgeT> &>(NodeRep()).Data(); 
  }
  
  template<class NodeT,class EdgeT>
  inline const NodeT &
  GraphNodeIterC<NodeT,EdgeT>::Data(void) const { 
    RavlAssert(IsIterValid());
    return static_cast<const GraphNodeDatC<NodeT,EdgeT> &>(NodeRep()).Data(); 
  }
  
  template<class NodeT,class EdgeT>
  inline GraphAdjIterC<NodeT,EdgeT> 
  GraphNodeIterC<NodeT,EdgeT>::In() {
    RavlAssert(IsIterValid());
    return GraphAdjIterC<NodeT,EdgeT>(GraphNodeBaseC::NodeRep().In(),Graph());
  }
  
  template<class NodeT,class EdgeT>
  inline 
  GraphAdjIterC<NodeT,EdgeT> 
  GraphNodeIterC<NodeT,EdgeT>::Out()  {
    RavlAssert(IsIterValid());
    return GraphAdjIterC<NodeT,EdgeT>(GraphNodeBaseC::NodeRep().Out(),Graph());
  }
  
  
  template<class NodeT,class EdgeT>
  inline void GraphNodeIterC<NodeT,EdgeT>::Invalidate() 
  { IntrDLIterC<GraphNodeBaseBodyC>::Invalidate(); }
 
  template<class NodeT,class EdgeT>
  inline void 
  GraphNodeIterC<NodeT,EdgeT>::Del() {
    RavlAssert(IsIterValid());
    GraphNodeDatC<NodeT,EdgeT> *node = &static_cast<GraphNodeDatC<NodeT,EdgeT> &>(NodeRep());
    Prev();
    delete node;
  }
  
  template<class NodeT,class EdgeT>
  inline void 
  GraphNodeIterC<NodeT,EdgeT>::DelMoveNext()  {
    RavlAssert(IsIterValid());
    GraphNodeDatC<NodeT,EdgeT> *node = &NodeDat();
    Next();
    delete node;
  }
  
  template<class NodeT,class EdgeT>
  inline GraphNodeDatC<NodeT,EdgeT> &
  GraphNodeIterC<NodeT,EdgeT>::NodeDat() { 
    return static_cast<GraphNodeDatC<NodeT,EdgeT> &>(GraphNodeBaseC::NodeRep()); 
  }
  
  template<class NodeT,class EdgeT>
  inline const
  GraphNodeDatC<NodeT,EdgeT> &
  GraphNodeIterC<NodeT,EdgeT>::NodeDat() const { 
    return static_cast<const GraphNodeDatC<NodeT,EdgeT> &>(GraphNodeBaseC::NodeRep()); 
  }
  
  
  ////////////////////////////////////////////////////////////////////////////////////
  // GraphNodeHC Stuff.
  
  template<class NodeT,class EdgeT>
  inline 
  GraphNodeHC<NodeT,EdgeT>::GraphNodeHC() 
    : Node(0)
  {}

  template<class NodeT,class EdgeT>
  inline 
  GraphNodeHC<NodeT,EdgeT>::GraphNodeHC(const GraphNodeHC<NodeT,EdgeT> &Oth) 
    : Node(Oth.Node) 
  {}

  template<class NodeT,class EdgeT>
  inline 
  GraphNodeHC<NodeT,EdgeT>::GraphNodeHC(const GraphNodeIterC<NodeT,EdgeT> &Oth)
    : Node(&((GraphNodeIterC<NodeT,EdgeT> &) Oth).NodeDat()) 
  {}

  template<class NodeT,class EdgeT>
  inline
  GraphNodeHC<NodeT,EdgeT>::GraphNodeHC(GraphNodeDatC<NodeT,EdgeT> &Rep)
    : Node(&Rep) 
  {}

  template<class NodeT,class EdgeT>
  inline bool 
  GraphNodeHC<NodeT,EdgeT>::IsValid(void) const { 
    return Node != 0;
  }
  
  template<class NodeT,class EdgeT>
  inline 
  const GraphNodeHC<NodeT,EdgeT> &
  GraphNodeHC<NodeT,EdgeT>::operator=(const GraphNodeHC<NodeT,EdgeT> &Oth) { 
    Node = Oth.Node; 
    return *this; 
  }
  
  template<class NodeT,class EdgeT>
  inline 
  const GraphNodeHC<NodeT,EdgeT> &
  GraphNodeHC<NodeT,EdgeT>::operator=(const GraphNodeIterC<NodeT,EdgeT> &Oth)  {
    Node = &((GraphNodeIterC<NodeT,EdgeT> &)Oth).NodeDat();
    return *this;
  }
  
  template<class NodeT,class EdgeT>
  inline bool 
  GraphNodeHC<NodeT,EdgeT>::operator==(const GraphNodeHC<NodeT,EdgeT> &Oth) const   
  { return Oth.Node == Node;  }

  template<class NodeT,class EdgeT>
  inline bool 
  GraphNodeHC<NodeT,EdgeT>::operator!=(const GraphNodeHC<NodeT,EdgeT> &Oth) const 
  { return Oth.Node != Node;  }

  template<class NodeT,class EdgeT>
  inline NodeT &
  GraphNodeHC<NodeT,EdgeT>::Data(void) 
  { return Node->Data(); }

  template<class NodeT,class EdgeT>
  inline const NodeT &
  GraphNodeHC<NodeT,EdgeT>::Data(void) const 
  { return Node->Data(); }

  template<class NodeT,class EdgeT>
  inline GraphAdjIterC<NodeT,EdgeT> 
  GraphNodeHC<NodeT,EdgeT>::In() { 
    return GraphAdjIterC<NodeT,EdgeT>(Node->In(), // Yep, its right !
				      Node->BaseGraph()); 
  }
  
  template<class NodeT,class EdgeT>
  inline GraphAdjIterC<NodeT,EdgeT> 
  GraphNodeHC<NodeT,EdgeT>::Out() { 
    return GraphAdjIterC<NodeT,EdgeT>(Node->Out(), // Yep, its right !
				      Node->BaseGraph()); 
  }
  
  template<class NodeT,class EdgeT>
  inline void 
  GraphNodeHC<NodeT,EdgeT>::Invalidate()
  { Node = 0; }
  
  template<class NodeT,class EdgeT>
  inline void 
  GraphNodeHC<NodeT,EdgeT>::Del()  {
    GraphNodeDatC<NodeT,EdgeT> *nd = static_cast<GraphNodeDatC<NodeT,EdgeT> *>(Node);
    Node = 0; // Invalidate.
    delete nd;
  }
  
}
#endif
