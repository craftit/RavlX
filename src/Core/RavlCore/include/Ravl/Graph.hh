// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVL_GRAPH_HEADER
#define RAVL_GRAPH_HEADER 1
//////////////////////////////////////////////////////////////
//! file="Ravl/Core/Container/Graph/Graph.hh"
//! lib=RavlCore
//! author="Charles Galambos"
//! date="10/12/1996"
//! docentry="Ravl.API.Core.Graphs"
//! rcsid="$Id$"
//! userlevel=Normal
//! example=exGraph.cc

#include "Ravl/GraphBase.hh"
#include "Ravl/Hash.hh"

namespace RavlN {
  
  template<class NodeT,class EdgeT> class GraphC;
  template<class NodeT,class EdgeT> class GraphNodeIterC;
  template<class NodeT,class EdgeT> class GraphNodeHC;
  template<class NodeT,class EdgeT> class GraphEdgeIterC;
  template<class NodeT,class EdgeT> class GraphLinearIterC;
  
  //! userlevel=Normal
  //: Templated graphs.
  
  template<class NodeT,class EdgeT>
  class GraphC 
    : public GraphBaseC 
  {
  public:  
    GraphC(GraphTypeT type = GRAPH_DIRECTED)
      : GraphBaseC(type) 
    {}
    //: Creates an empty graph, directed or undirected.
    
    GraphC(const GraphC<NodeT,EdgeT> & g) 
      : GraphBaseC(g) 
    {}
    //: Creates another access to the graph 'g'. 
    // The constructor does not
    // change the content of the graph 'g', but its reference counting.
    // However, the content of the graph 'g' can be changed through
    // the this new access later.
    
    ~GraphC()
    {}
    //: Destructor.
    
    typedef GraphEdgeIterC<NodeT,EdgeT> EdgeIter;
    typedef GraphNodeIterC<NodeT,EdgeT> NodeIter;
    typedef GraphC<NodeT,EdgeT> Graph;
    
    inline GraphC<NodeT,EdgeT> Copy();
    //: Make a copy of this graph.
    
    GraphC<NodeT,EdgeT> Copy(HashC<NodeIter,NodeIter> & NodeMap);
    //: Make a copy of this graph.
    // The mapping between new and old nodes is in NodeMap.
    
    const GraphC<NodeT,EdgeT> & operator=(const GraphC<NodeT,EdgeT> & g) 
    { GraphBaseC::operator=(g); return *this; }
    //: Creates another access to the graph 'g'. 
    // The assigment does not
    // change the content of the graph 'g', but its reference counting.
    // However, the content of the graph 'g' can be changed through
    // the this new access later.
    
    //:------------------------------------------
    //: Creation of graph structure.
    
    inline GraphNodeIterC<NodeT,EdgeT> InsNode(const NodeT &Dat);
    //: Inserts one node to the graph. 
    // Returns the node iterator pointing to the new element.
    
    inline GraphEdgeIterC<NodeT,EdgeT> InsEdge(NodeIter &fromNode, 
					       NodeIter &toNode,
					       const EdgeT &Dat
					       );
    //: Inserts one egde to the graph.
    // Returns an edge iterator pointing to the new element.
    
    inline GraphEdgeIterC<NodeT,EdgeT> InsEdge(const GraphNodeHC<NodeT,EdgeT> &fromNode,
					       const GraphNodeHC<NodeT,EdgeT> &toNode,
					       const EdgeT &Dat
					       );
    //: Inserts one egde to the graph. 
    // Returns an edge iterator pointing to the new element.
    
    SizeT NoNodes() const
    { return Nodes().Size(); }
    //: Count the number of nodes in the graph.
    // This actuall iterates through the list and so is slow.
    
    SizeT NoEdges() const
    { return Edges().Size(); }
    //: Count the number of edges in the graph.
    // This actuall iterates through the list and so is slow.
    
    //:---------------------------------------
    //: Tests on graph structure.
    
    bool IsCyclic() 
    { return GraphBaseC::IsCyclic(); }
    //: Test if a directed graph contains cycles.

  protected:
    
    GraphBaseBodyC &Body()
    { return GraphBaseC::Body(); }
    //: Access body.

    const GraphBaseBodyC &Body() const
    { return GraphBaseC::Body(); }
    //: Access body.
    
    friend class GraphNodeIterC<NodeT,EdgeT>;
    friend class GraphEdgeIterC<NodeT,EdgeT>;
    friend class GraphLinearIterC<NodeT,EdgeT>;
  };
  
}

#include "Ravl/GraphNode.hh"
#include "Ravl/GraphEdge.hh"

namespace RavlN {
  
  ///////////////////////////////
  // Make a copy of this graph.
  
  template<class NodeT,class EdgeT>
  inline 
  GraphC<NodeT,EdgeT>  GraphC<NodeT,EdgeT>::Copy() {
    HashC<NodeIter,NodeIter> NodeMap;
    return Copy(NodeMap);
  }
  
  ///////////////////////////////
  // Make a copy of this graph, return Node-Map.
  
  template<class NodeT,class EdgeT>
  GraphC<NodeT,EdgeT> GraphC<NodeT,EdgeT>::Copy(HashC<GraphNodeIterC<NodeT,EdgeT>,GraphNodeIterC<NodeT,EdgeT> > &NodeMap) {
    GraphC<NodeT,EdgeT> Newun;
    for(GraphNodeIterC<NodeT,EdgeT> NIt(*this);NIt.IsElm();NIt.Next())
      NodeMap[NIt] = Newun.InsNode(NIt.Data());
    for(GraphEdgeIterC<NodeT,EdgeT> EIt(*this);EIt.IsElm();EIt.Next())
      Newun.InsEdge(NodeMap[EIt.Source()],NodeMap[EIt.Target()],EIt.Data());
    return Newun;  
  }

  ///////////////////////////////
  // Inserts one node to the graph. Returns the node iterator. 
  
  template<class NodeT,class EdgeT>
  inline 
  GraphNodeIterC<NodeT,EdgeT> GraphC<NodeT,EdgeT>::InsNode(const NodeT &dat)  {
    return GraphNodeIterC<NodeT,EdgeT>(GraphBaseC::InsNode(*new GraphNodeDatC<NodeT,EdgeT>(Body(),
											   dat,
											   AllocNodeID()
											   )));
  }
  
  //////////////////////////////////
  // Inserts one egde to the graph. Returns the edge iterator. 
  
  template<class NodeT,class EdgeT>
  inline 
  GraphEdgeIterC<NodeT,EdgeT> GraphC<NodeT,EdgeT>::InsEdge(GraphNodeIterC<NodeT,EdgeT> &fromNode,
							   GraphNodeIterC<NodeT,EdgeT> & toNode,
							   const EdgeT &Dat) {
    return GraphEdgeIterC<NodeT,EdgeT>(GraphBaseC::InsEdge(*new GraphEdgeDatC<EdgeT>(fromNode.NodeRep(),
										     toNode.NodeRep(),
										     Dat,
										     AllocEdgeID())));
  }
  
  template<class NodeT,class EdgeT>
  inline 
  GraphEdgeIterC<NodeT,EdgeT> GraphC<NodeT,EdgeT>::InsEdge(const GraphNodeHC<NodeT,EdgeT> &fromNode, 
							   const GraphNodeHC<NodeT,EdgeT> &toNode,
							   const EdgeT &Dat
							   ) {
    return GraphEdgeIterC<NodeT,EdgeT>(GraphBaseC::InsEdge(*new GraphEdgeDatC<EdgeT>(fromNode.NodeRep(),
										     toNode.NodeRep(),
										     Dat,
										     AllocEdgeID())));
  }
  
  //: Write graph out to a stream.
  
  template<class NodeT,class EdgeT>
  std::ostream &operator<<(std::ostream &out,const GraphC<NodeT,EdgeT> &g) {
    HashC<GraphNodeHC<NodeT,EdgeT>,UIntT > nodeMap;
    // Output all the nodes and give them an id each.
    out << g.NoNodes() << "\n"; // Write the number of nodes.
    UIntT id = 1;
    for(GraphNodeIterC<NodeT,EdgeT> nIt(const_cast<GraphC<NodeT,EdgeT> &>(g));nIt;nIt++) {
      nodeMap[GraphNodeHC<NodeT,EdgeT>(nIt)] = id++;
      out << nIt.Data() << "\n";
    }
    for(GraphEdgeIterC<NodeT,EdgeT> nIt(const_cast<GraphC<NodeT,EdgeT> &>(g));nIt;nIt++) {
      out << nodeMap[nIt.SourceH()] << " " << nodeMap[nIt.TargetH()] << "\n";
      out << nIt.Data() << "\n";
    }
    out << "0\n"; // Write end of edge marker.
    return out;
  }
  
  //: Read graph from a stream.
  
  template<class NodeT,class EdgeT>
  std::istream &operator>>(std::istream &in,GraphC<NodeT,EdgeT> &g)  {
    HashC< UIntT,GraphNodeHC<NodeT,EdgeT> > nodeMap;
    // Output all the nodes and give them an id each.
    UIntT nodes;
    in >> nodes;
    UIntT id = 1;
    for(;id <= nodes;id++) {
      NodeT dat;
      in >> dat;
      nodeMap[id] = g.InsNode(dat);
    }
    UIntT n1,n2;
    while(1) {
      in >> n1;
      if(n1 == 0) // Got terminator ?
	break;
      in >> n2;
      EdgeT dat;
      in >> dat;
      g.InsEdge(nodeMap[n1],nodeMap[n2],dat);
    }
    return in;
  }
}
#endif
