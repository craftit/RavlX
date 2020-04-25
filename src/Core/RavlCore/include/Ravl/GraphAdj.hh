// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVL_GRAPHADJ_HEADER
#define RAVL_GRAPHADJ_HEADER 1
//////////////////////////////////////////////////////////////
//! file="Ravl/Core/Container/Graph/GraphAdj.hh"
//! lib=RavlCore
//! userlevel=Normal
//! author="Charles Galambos"
//! date="10/12/1996"
//! docentry="Ravl.API.Core.Graphs"
//! example=exGraph.cc
//! rcsid="$Id$"

#include "Ravl/GraphBase.hh"
#include "Ravl/Assert.hh"

namespace RavlN {
  
  template<class NodeT,class EdgeT> class GraphEdgeIterC;
  template<class NodeT,class EdgeT> class GraphNodeIterC;
  template<class NodeT,class EdgeT> class GraphC;
  template<class NodeT,class EdgeT> class GraphNodeDatC;
  template<class NodeT,class EdgeT> class GraphNodeHC;
  
  ////////////////////////////
  //: Adjacency iterator.
  
  template<class NodeT,class EdgeT>
  class GraphAdjIterC 
    : public GraphAdjIterBaseC {
  public:  
    GraphAdjIterC(const GraphAdjIterC<NodeT,EdgeT> &Another)
      : GraphAdjIterBaseC(Another) 
      {}
    // Copy constructor.
    
    inline const GraphAdjIterC<NodeT,EdgeT> &operator=(const GraphAdjIterC<NodeT,EdgeT> &Oth);
    // Assigment.
    
    inline EdgeT &Data(void);
    // Access data.
  
    inline const EdgeT &Data(void) const;
    // Constant access to data.
    
    inline NodeT &Node1Data(void);
    // Get data from node 1.
    
    inline NodeT &Node2Data(void);
    // Get data from node 2.
    
    NodeT &SourceData(void) { return Node1Data(); }
    // Data from source node.
    
    NodeT &TargetData(void) { return Node2Data(); }
    // Data from target node.
    
    inline GraphNodeIterC<NodeT,EdgeT> Node1(void);
    // Get a node from an undirected graph
  
    inline GraphNodeIterC<NodeT,EdgeT> Node2(void);
    // Get a node from an undirected graph
    
    inline GraphNodeIterC<NodeT,EdgeT> Node(void);
    // See GraphAdjIterBaseC::Node()
  
    inline GraphNodeIterC<NodeT,EdgeT> OtherNode(void);
    // See GraphAdjIterBaseC::OtherNode()
  
    inline GraphNodeHC<NodeT,EdgeT> NodeH(void);
    // See GraphAdjIterBaseC::Node()
    
    inline GraphNodeHC<NodeT,EdgeT> OtherNodeH(void);
    // See GraphAdjIterBaseC::OtherNode()
    
    inline GraphNodeIterC<NodeT,EdgeT> Source(void);
    // Get source node.
    
    inline GraphNodeIterC<NodeT,EdgeT> Target(void);
    // Get target node.
    
    inline GraphNodeHC<NodeT,EdgeT> SourceH();
    // Get source node handle.
    
    inline GraphNodeHC<NodeT,EdgeT> TargetH();
    // Get target node handle.
    
    inline GraphEdgeIterC<NodeT,EdgeT> Edge(void);
    // Get associated edge.
    
    inline void Del();
    // Unlinks edge from the graph and deletes it. The  iterator
    // will point to the previous edge in the graph edge list.
    
    inline void DelMoveNext();
    // Unlinks edge from the graph and deletes it. The  iterator
    // will point to the next edge in the graph edge list.
    
  protected:
    inline GraphNodeDatC<NodeT,EdgeT> &SourceRep(void);
    // Get source node.
    
    inline GraphNodeDatC<NodeT,EdgeT> &TargetRep(void);
    // Get target node repre.
    
  private:
    GraphAdjIterC(const IntrDLIterC<GraphAdjRepC> & iter,GraphBaseBodyC & grph)
      : GraphAdjIterBaseC(iter,grph)
      {}
    
    GraphAdjIterC(IntrDListC<GraphAdjRepC> & list,GraphBaseBodyC & grph)
      : GraphAdjIterBaseC(list,grph)
      {}
    // Creates the graph adjacency iterator from the iterator of adjacency and
    // the graph which the iterator belongs to.
    
    GraphAdjIterC(const GraphAdjIterBaseC &Adj) 
      : GraphAdjIterBaseC(Adj) 
      {}
    // Constructor from base GraphAdjIterBaseC.
    
    friend class GraphNodeIterC<NodeT,EdgeT>;
    friend class GraphNodeDatC<NodeT,EdgeT>;
    friend class GraphNodeHC<NodeT,EdgeT>;
  };
  
}
#include "Ravl/GraphNode.hh"
#include "Ravl/GraphEdge.hh"

namespace RavlN {
  
  template<class NodeT,class EdgeT>
  inline 
  const GraphAdjIterC<NodeT,EdgeT> &
  GraphAdjIterC<NodeT,EdgeT>::operator=(const GraphAdjIterC<NodeT,EdgeT> &Oth)  {
    ((GraphAdjIterBaseC &)(*this)) = Oth;
    return *this;
  }
  
  template<class NodeT,class EdgeT>
  inline
  EdgeT &GraphAdjIterC<NodeT,EdgeT>::Data(void)  
  { return static_cast<GraphEdgeDatC<EdgeT> &>(GraphAdjIterBaseC::EdgeRep()).Data(); }
  
  template<class NodeT,class EdgeT>
  inline 
  const EdgeT &GraphAdjIterC<NodeT,EdgeT>::Data(void) const  
  { return static_cast<const GraphEdgeDatC<EdgeT> &>(GraphAdjIterBaseC::EdgeRep()).Data(); }
  
  template<class NodeT,class EdgeT>
  inline 
  NodeT &GraphAdjIterC<NodeT,EdgeT>::Node1Data(void)  
  { return static_cast<GraphNodeDatC<NodeT,EdgeT> &>(GraphAdjIterBaseC::Node1Rep()).Data(); }
  
  template<class NodeT,class EdgeT>
  inline 
  NodeT &GraphAdjIterC<NodeT,EdgeT>::Node2Data(void)  
  { return static_cast<GraphNodeDatC<NodeT,EdgeT> &>(GraphAdjIterBaseC::Node2Rep()).Data(); }
  
  template<class NodeT,class EdgeT>
  inline 
  GraphNodeIterC<NodeT,EdgeT> GraphAdjIterC<NodeT,EdgeT>::Node1(void) 
  { return GraphNodeIterC<NodeT,EdgeT>(GraphAdjIterBaseC::Node1()); }
  
  template<class NodeT,class EdgeT>
  inline 
  GraphNodeIterC<NodeT,EdgeT> GraphAdjIterC<NodeT,EdgeT>::Node2(void) 
  { return GraphNodeIterC<NodeT,EdgeT>(GraphAdjIterBaseC::Node2()); }
  
  template<class NodeT,class EdgeT>
  inline
  GraphNodeIterC<NodeT,EdgeT> GraphAdjIterC<NodeT,EdgeT>::Source(void)  
  { return GraphNodeIterC<NodeT,EdgeT>(GraphAdjIterBaseC::EdgeRep().Source(),Graph()); }
  
  template<class NodeT,class EdgeT>
  inline
  GraphNodeIterC<NodeT,EdgeT> GraphAdjIterC<NodeT,EdgeT>::Target(void) 
  { return GraphNodeIterC<NodeT,EdgeT>(GraphAdjIterBaseC::EdgeRep().Target(),Graph()); }
  
  template<class NodeT,class EdgeT>
  inline
  GraphNodeIterC<NodeT,EdgeT> GraphAdjIterC<NodeT,EdgeT>::Node(void) 
  { return GraphNodeIterC<NodeT,EdgeT>(GraphAdjIterBaseC::Node()); }
  
  template<class NodeT,class EdgeT>
  inline 
  GraphNodeIterC<NodeT,EdgeT>  GraphAdjIterC<NodeT,EdgeT>::OtherNode() 
  { return GraphNodeIterC<NodeT,EdgeT>(GraphAdjIterBaseC::OtherNode()); }
  
  template<class NodeT,class EdgeT>
  inline 
  GraphNodeHC<NodeT,EdgeT> GraphAdjIterC<NodeT,EdgeT>::NodeH(void)  
  { return GraphNodeHC<NodeT,EdgeT>(GraphAdjIterBaseC::Node()); }
  
  template<class NodeT,class EdgeT>
  inline 
  GraphNodeHC<NodeT,EdgeT> GraphAdjIterC<NodeT,EdgeT>::OtherNodeH(void) 
  { return GraphNodeHC<NodeT,EdgeT>(GraphAdjIterBaseC::OtherNode()); }
  
  template<class NodeT,class EdgeT>
  inline
  GraphEdgeIterC<NodeT,EdgeT> GraphAdjIterC<NodeT,EdgeT>::Edge(void) 
  { return GraphEdgeIterC<NodeT,EdgeT>(GraphAdjIterBaseC::Edge()); }
  
  template<class NodeT,class EdgeT>
  inline
  GraphNodeHC<NodeT,EdgeT> GraphAdjIterC<NodeT,EdgeT>::SourceH()  
  { return GraphNodeHC<NodeT,EdgeT>(SourceRep()); }
  
  template<class NodeT,class EdgeT>
  inline 
  GraphNodeHC<NodeT,EdgeT> GraphAdjIterC<NodeT,EdgeT>::TargetH() 
  { return GraphNodeHC<NodeT,EdgeT>(TargetRep()); }
  
  template<class NodeT,class EdgeT>
  inline void GraphAdjIterC<NodeT,EdgeT>::Del() {
    GraphEdgeDatC<EdgeT> *edge = &static_cast<GraphEdgeDatC<EdgeT> &>(GraphAdjIterBaseC::EdgeRep());
    Prev();
    delete edge;
  }
  
  template<class NodeT,class EdgeT>
  inline void GraphAdjIterC<NodeT,EdgeT>::DelMoveNext()  {
    GraphEdgeDatC<EdgeT> *edge = &static_cast<GraphEdgeDatC<EdgeT> &>(GraphAdjIterBaseC::EdgeRep());
    Next();
    delete edge;
  }
  
  template<class NodeT,class EdgeT>
  inline 
  GraphNodeDatC<NodeT,EdgeT> & GraphAdjIterC<NodeT,EdgeT>::SourceRep(void)
  { return static_cast<GraphNodeDatC<NodeT,EdgeT> &>(GraphAdjIterBaseC::SourceRep()); }
  
  template<class NodeT,class EdgeT>
  inline 
  GraphNodeDatC<NodeT,EdgeT> &GraphAdjIterC<NodeT,EdgeT>::TargetRep(void)  
  { return static_cast<GraphNodeDatC<NodeT,EdgeT> &>(GraphAdjIterBaseC::TargetRep()); }
}
#endif
