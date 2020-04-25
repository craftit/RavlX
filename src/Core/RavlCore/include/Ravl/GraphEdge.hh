// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVL_GRAPHEDGE_HEADER
#define RAVL_GRAPHEDGE_HEADER 1
//////////////////////////////////////////////////////////////
//! file="Ravl/Core/Container/Graph/GraphEdge.hh"
//! lib=RavlCore
//! author="Charles Galambos"
//! date="10/12/1996"
//! docentry="Ravl.API.Core.Graphs"
//! rcsid="$Id$"
//! example=exGraph.cc
//! userlevel=Normal

#include "Ravl/GraphBase.hh"
#include "Ravl/Assert.hh"

namespace RavlN {
  template<class NodeT,class EdgeT> class GraphC;
  template<class NodeT,class EdgeT> class GraphNodeIterC;
  template<class NodeT,class EdgeT> class GraphAdjIterC;
  template<class NodeT,class EdgeT> class GraphNodeHC;
  template<class NodeT,class EdgeT> class GraphNodeDatC;

  const int GraphEdgeDat_CheckNum = 0x56781234;
  
  //! userlevel=Develop
  //: Edge data.
  
  template<class EdgeT>
  class GraphEdgeDatC 
    : public GraphEdgeBaseBodyC 
  {
  public:  
    inline GraphEdgeDatC(GraphNodeBaseBodyC &From,
			 GraphNodeBaseBodyC &To,
			 const EdgeT &Dat,
			 UIntT nid
			 );
    // Constructor.
    
    inline ~GraphEdgeDatC();
    // Destructor.
    
    EdgeT &Data(void) { return UserData; }  
    // Get at data.
    
    const EdgeT &Data(void) const { return UserData; }
    // Get at data.
    
#if RAVL_GRAPH_DEBUG
    bool IsValid() const { return CheckVal == GraphEdgeDat_CheckNum; }
#else
    bool IsValid() const { return true; }
#endif
  protected:
    EdgeT UserData;
#if RAVL_GRAPH_DEBUG
    int CheckVal;
#endif  
  };
  
  /////////////////////////////////////
  //! userlevel=Normal
  //: Edge iterator.
  // Iterates through the edges in a graph.
  
  template<class NodeT,class EdgeT>
  class GraphEdgeIterC 
    : public GraphEdgeBaseC 
  {
    typedef GraphEdgeDatC<EdgeT> EdgeDatT;
  public:
    
    GraphEdgeIterC(GraphC<NodeT,EdgeT> &AGraph);
    // Constructor.
    
    inline GraphEdgeIterC();
    // Default Constructor.
    
    inline GraphEdgeIterC(const GraphEdgeIterC<NodeT,EdgeT> &Another);
    // Copy constructor.
    
    ~GraphEdgeIterC();
    // Destructor.
    
    inline const GraphEdgeIterC<NodeT,EdgeT> &operator=(const GraphEdgeIterC<NodeT,EdgeT> &Oth);
    // Assigment.
    
    inline bool operator==(const GraphEdgeIterC<NodeT,EdgeT> &Oth) const;
    // Comparison.
    
    inline EdgeT &Data(void);
    // Access data.
    
    inline const EdgeT &Data(void) const ;
    // Access data.
    
    inline GraphNodeIterC<NodeT,EdgeT> Node1(void);
    // Get a node from an undirected graph
    
    inline GraphNodeIterC<NodeT,EdgeT> Node2(void);
    // Get a node from an undirected graph
    
    inline NodeT &Node1Data(void);
    // Get data from node 1.
    
    inline NodeT &Node2Data(void);
    // Get data from node 2.
    
    inline const NodeT &Node1Data(void) const;
    // Get data from node 1.
    
    inline const NodeT &Node2Data(void) const;
    // Get data from node 2.
    
    NodeT &SourceData(void) { return Node1Data(); }
    // Data from source node.
    
    NodeT &TargetData(void) { return Node2Data(); }
    // Data from target node.
    
    const NodeT &SourceData(void) const { return Node1Data(); }
    // Data from source node.
    
    const NodeT &TargetData(void) const { return Node2Data(); }
    // Data from target node.
    
    GraphNodeIterC<NodeT,EdgeT> Source(void);
    // Get source node iter.
    
    GraphNodeIterC<NodeT,EdgeT> Target(void);
    // Get target node iter.
    
    GraphNodeHC<NodeT,EdgeT> SourceH(void);
    // Get source node handle.
    
    GraphNodeHC<NodeT,EdgeT> TargetH(void);
    // Get target node handle.
    
    inline void Invalidate();
    // Invalidate handle, IsValid() will return false after this
    // call until the handle is assigmed to another edge.
    
    inline void Del();
    // Unlinks edge from the graph and deletes it. The  iterator
    // will point to the previous edge in the graph edge list.
    
#if GRAPH_DEBUG
    bool IsEdgeValid() const { 
      if(!IsValid()) return true;
      return EdgeDat().IsValid();
    }
    bool IsIterValid() const { 
      if(!IsValid()) return false;
      return EdgeDat().IsValid();
    }
#else
    bool IsEdgeValid() const { return true; }
    bool IsIterValid() const { return true; }
#endif
    
  protected:
    inline GraphNodeDatC<NodeT,EdgeT> &SourceRep(void);
    // Get source node.
    
    inline GraphNodeDatC<NodeT,EdgeT> &TargetRep(void);
    // Get target node repre.
    
    inline GraphEdgeDatC<EdgeT> &EdgeDat();
    // Get a edge rep.
    
    inline const GraphEdgeDatC<EdgeT> &EdgeDat() const;
    // Get a edge rep.
    
  private:
    GraphEdgeIterC(const GraphEdgeBaseC &anEdge) 
      : GraphEdgeBaseC(anEdge)
      {}
    // Create an iter from a GraphEdgeC.
    
    GraphEdgeIterC(GraphEdgeBaseBodyC & edge, IntrDListC<GraphEdgeBaseBodyC> & list)
      : GraphEdgeBaseC(edge,list) 
      {}
    // Creates the iterator pointing to the edge which belongs to the 'list'.
    
    friend class GraphC<NodeT,EdgeT>;
    friend class GraphAdjIterC<NodeT,EdgeT>;
  };

}

#include "Ravl/Graph.hh"
#include "Ravl/GraphNode.hh"

namespace RavlN {
  ///////////////////////////////////////////////
  // GraphEdgeDat.
  
  template<class EdgeT>
  inline  
  GraphEdgeDatC<EdgeT>::GraphEdgeDatC(GraphNodeBaseBodyC &From,
				      GraphNodeBaseBodyC &To,
				      const EdgeT &Dat,
				      UIntT nid)
      : GraphEdgeBaseBodyC(From,To,nid),
      UserData(Dat)
  {
#if GRAPH_DEBUG
    CheckVal = GraphEdgeDat_CheckNum;
#endif
  }
  
  template<class EdgeT>
  inline 
  GraphEdgeDatC<EdgeT>::~GraphEdgeDatC()  {
#if GRAPH_DEBUG
    RavlAssert(IsValid());
    CheckVal = 0x0f0f0f0f;
#endif
  }
  
  ///////////////////////////////////////////////
  // GraphEdgeIter.
  
  template<class NodeT,class EdgeT>
  GraphEdgeIterC<NodeT,EdgeT>::GraphEdgeIterC(GraphC<NodeT,EdgeT> &aGraph)
    : GraphEdgeBaseC(aGraph.Body())
  {}

  template<class NodeT,class EdgeT>
  inline GraphEdgeIterC<NodeT,EdgeT>::GraphEdgeIterC() 
    : GraphEdgeBaseC()
  {}

  template<class NodeT,class EdgeT>
  inline 
  GraphEdgeIterC<NodeT,EdgeT>::GraphEdgeIterC(const GraphEdgeIterC<NodeT,EdgeT> &Another)
    : GraphEdgeBaseC(Another) 
  {}

  template<class NodeT,class EdgeT>
  GraphEdgeIterC<NodeT,EdgeT>::~GraphEdgeIterC() {
    RavlAssert(IsEdgeValid());
  }
  
  template<class NodeT,class EdgeT>
  inline const GraphEdgeIterC<NodeT,EdgeT> &
  GraphEdgeIterC<NodeT,EdgeT>::operator=(const GraphEdgeIterC<NodeT,EdgeT> &Oth)  {
    RavlAssert(Oth.IsEdgeValid());
    ((GraphEdgeBaseC &)(*this)) = Oth;
    return *this;
  }
  
  template<class NodeT,class EdgeT>
  inline bool 
  GraphEdgeIterC<NodeT,EdgeT>::operator==(const GraphEdgeIterC<NodeT,EdgeT> &Oth) const  { 
    RavlAssert(Oth.IsEdgeValid());
    RavlAssert(IsEdgeValid());
    return GraphEdgeBaseC::operator==(Oth); 
  }
  
  template<class NodeT,class EdgeT>
  inline EdgeT &
  GraphEdgeIterC<NodeT,EdgeT>::Data(void) { 
    RavlAssert(IsIterValid());
    return EdgeDat().Data(); 
  }
  
  template<class NodeT,class EdgeT>
  inline const EdgeT &
  GraphEdgeIterC<NodeT,EdgeT>::Data(void) const  { 
    RavlAssert(IsIterValid());
    return EdgeDat().Data(); 
  }
  
  template<class NodeT,class EdgeT>
  inline void GraphEdgeIterC<NodeT,EdgeT>::Invalidate()  {
    IntrDLIterC<GraphEdgeBaseBodyC>::Invalidate();
  }

  template<class NodeT,class EdgeT>
  inline void GraphEdgeIterC<NodeT,EdgeT>::Del() {
    RavlAssert(IsIterValid());
    EdgeDatT *edge = &EdgeDat();
    Prev();
    delete edge;
  }
  
  template<class NodeT,class EdgeT>
  inline GraphNodeIterC<NodeT,EdgeT> 
  GraphEdgeIterC<NodeT,EdgeT>::Node1(void)  {
    RavlAssert(IsIterValid());
    return GraphNodeIterC<NodeT,EdgeT>(GraphEdgeBaseC::Node1());
  }
  
  template<class NodeT,class EdgeT>
  inline GraphNodeIterC<NodeT,EdgeT> 
  GraphEdgeIterC<NodeT,EdgeT>::Node2(void) {
    RavlAssert(IsIterValid());
    return GraphNodeIterC<NodeT,EdgeT>(GraphEdgeBaseC::Node2());
  }
  
  template<class NodeT,class EdgeT>
  inline NodeT &GraphEdgeIterC<NodeT,EdgeT>::Node1Data(void) { 
    RavlAssert(IsIterValid());
    return static_cast<GraphNodeDatC<NodeT,EdgeT> &>(GraphEdgeBaseC::Node1().NodeRep()).Data();
  }
  
  template<class NodeT,class EdgeT>
  inline NodeT &GraphEdgeIterC<NodeT,EdgeT>::Node2Data(void)  { 
    RavlAssert(IsIterValid());
    return static_cast<GraphNodeDatC<NodeT,EdgeT> &>(GraphEdgeBaseC::Node2().NodeRep()).Data();
  }

  template<class NodeT,class EdgeT>
  inline const NodeT &GraphEdgeIterC<NodeT,EdgeT>::Node1Data(void) const {
    RavlAssert(IsIterValid());
    return static_cast<const GraphNodeDatC<NodeT,EdgeT> &>(const_cast<GraphEdgeIterC<NodeT,EdgeT> &>(*this).GraphEdgeBaseC::Node1().NodeRep()).Data();
  }
  
  template<class NodeT,class EdgeT>
  inline const NodeT &GraphEdgeIterC<NodeT,EdgeT>::Node2Data(void) const { 
    RavlAssert(IsIterValid());
    return static_cast<const GraphNodeDatC<NodeT,EdgeT> &>(const_cast<GraphEdgeIterC<NodeT,EdgeT> &>(*this).GraphEdgeBaseC::Node2().NodeRep()).Data();
  }
  
  template<class NodeT,class EdgeT>
  inline GraphNodeIterC<NodeT,EdgeT>
  GraphEdgeIterC<NodeT,EdgeT>::Source(void) {
    RavlAssert(IsIterValid());
    return GraphNodeIterC<NodeT,EdgeT>(GraphEdgeBaseC::Source());
  }
  
  template<class NodeT,class EdgeT>
  inline GraphNodeIterC<NodeT,EdgeT>
  GraphEdgeIterC<NodeT,EdgeT>::Target(void)  {
    RavlAssert(IsIterValid());
    return GraphNodeIterC<NodeT,EdgeT>(GraphEdgeBaseC::Target());
  }
  
  template<class NodeT,class EdgeT>
  inline GraphNodeHC<NodeT,EdgeT>
  GraphEdgeIterC<NodeT,EdgeT>::SourceH()  {
    RavlAssert(IsIterValid());
    return GraphNodeHC<NodeT,EdgeT>(SourceRep());
  }
  
  template<class NodeT,class EdgeT>
  inline GraphNodeHC<NodeT,EdgeT> 
  GraphEdgeIterC<NodeT,EdgeT>::TargetH()  {
    RavlAssert(IsIterValid());
    return GraphNodeHC<NodeT,EdgeT>(TargetRep());
  }
  
  template<class NodeT,class EdgeT>
  inline GraphNodeDatC<NodeT,EdgeT> &
  GraphEdgeIterC<NodeT,EdgeT>::SourceRep(void) {
    RavlAssert(IsIterValid());
    return static_cast<GraphNodeDatC<NodeT,EdgeT> &>(GraphEdgeBaseC::SourceRep());
  }
  
  template<class NodeT,class EdgeT>
  inline GraphNodeDatC<NodeT,EdgeT> &
  GraphEdgeIterC<NodeT,EdgeT>::TargetRep(void)  {
    RavlAssert(IsIterValid());
    return static_cast<GraphNodeDatC<NodeT,EdgeT> &>(GraphEdgeBaseC::TargetRep());
  }
  
  template<class NodeT,class EdgeT>
  inline GraphEdgeDatC<EdgeT> &
  GraphEdgeIterC<NodeT,EdgeT>::EdgeDat() {
    return static_cast<GraphEdgeDatC<EdgeT> &>(GraphEdgeBaseC::Data());
  }
  
  
  template<class NodeT,class EdgeT>
  inline const GraphEdgeDatC<EdgeT> &
  GraphEdgeIterC<NodeT,EdgeT>::EdgeDat() const {
    return static_cast<const GraphEdgeDatC<EdgeT> &>(GraphEdgeBaseC::Data());
  }
  
}
#endif
