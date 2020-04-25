// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVL_BASEGRAPH_HEADER
#define RAVL_BASEGRAPH_HEADER 1
//////////////////////////////////////////////////////////////////////
//! file="Ravl/Core/Container/Graph/GraphBase.hh"
//! lib=RavlCore
//! userlevel=Develop
//! author="Radek Marik"
//! date="22/8/1994"
//! docentry="Ravl.API.Core.Graphs"
//! rcsid="$Id$"

// Based on the graph structure designed by George Matas
// (g.matas@ee.surrey.ac.uk) and his code in C language.

#include "Ravl/IntrDList.hh"
#include "Ravl/IntrDLIter.hh"
#include "Ravl/RCHandleV.hh"
#include "Ravl/Types.hh"

#define RAVL_GRAPH_DEBUG 1

namespace RavlN {
  
  class GraphBaseBodyC;
  class GraphNodeBaseBodyC; 
  class GraphEdgeBaseBodyC; 
  class GraphAdjIterBaseC; 
  class GraphBaseC;
  enum GraphTypeT {GRAPH_DIRECTED, GRAPH_UNDIRECTED};
  
  // ----------------------------------------------------------------------
  // ************ GrapAdjRepC *********************************************
  // ----------------------------------------------------------------------
    
  //: The class GraphAdjRepC represents a part of 
  //: triple <nodeA, nodeB, edgeAB> which is a basic element of graphs.
  // One can imagine that every edge has two touch points, so call adjacencies,
  // that connect nodes and edge together. Each node is connected to the edge
  // through one adjacency. Because of effectivity, the representation of
  // adjacency contains only the pair <nodeB, edgeAB>. The access to the nodeA
  // is solved outside of the this class.
  
  class GraphAdjRepC
    : public DLinkC // Link of a list of node adjacencies.
  {
  public:
    inline GraphAdjRepC(GraphNodeBaseBodyC & node, GraphEdgeBaseBodyC  & edge);
    // Initializes the graph link.
    
    inline const GraphNodeBaseBodyC & OtherNode() const;
    // Access to the constant adjacent node.

    inline GraphNodeBaseBodyC & OtherNode();
    // Returns the adjacent node.
    
    inline const GraphEdgeBaseBodyC & Edge() const;
    // Access to the constant edge joining the node possessing the list of
    // adjacent nodes and the adjacent node.

    inline GraphEdgeBaseBodyC & Edge();
    // Returns the edge joining the node possessing the list of
    // adjacent nodes and the adjacent node.
    
  private:
    
    GraphNodeBaseBodyC * otherNode;  // adjacent node
    GraphEdgeBaseBodyC * me;         // the edge joining both nodes
    
    inline void SwapNodes(GraphAdjRepC & adj);
    // Swaps the pointers to the nodes.
    
    friend class GraphEdgeBaseC; //SwapNodes()
  };

  std::ostream & operator<<(std::ostream & s, const GraphAdjRepC & link);
  // Saves information in the link to the output stream.

  // ----------------------------------------------------------------------
  // ************ GraphEdgeBaseBodyC *******************************************
  // ----------------------------------------------------------------------
  
  //: The class GraphEdgeBaseBodyC represents one edge of graph.
  
  class GraphEdgeBaseBodyC
    : public DLinkC
  {
  public:

// Disable "this used in member initialiser" warning
#if RAVL_COMPILER_VISUALCPP
#pragma warning ( push )
#pragma warning ( disable : 4355 )
#endif	  

	inline 
    GraphEdgeBaseBodyC(GraphNodeBaseBodyC & sourceNode, GraphNodeBaseBodyC & targetNode,size_t nId)
      : source(GraphAdjRepC(targetNode, *this)),
        target(GraphAdjRepC(sourceNode, *this)),
        id(nId)
      {}
    // Creates the edge between two nodes 'sourceNode' and 'targetNode'.

#if RAVL_COMPILER_VISUALCPP
#pragma warning ( pop )
#endif	  

    virtual ~GraphEdgeBaseBodyC()
      {}
    //: Destructor.
    // This ensures derived classes are deleted properly.
    
    inline const GraphNodeBaseBodyC & Source() const
    { return target.OtherNode(); }
    // Returns the constant source node of this edge.

    inline GraphNodeBaseBodyC & Source()
    { return target.OtherNode(); }
    // Returns the source node of this edge.

    inline const GraphNodeBaseBodyC & Target() const
    { return source.OtherNode(); }
    // Returns the constant target node of this edge.

    inline GraphNodeBaseBodyC & Target()
    { return source.OtherNode(); }
    // Returns the target node of this edge.
    
    inline const GraphNodeBaseBodyC & Node1() const
    { return Source(); }
    // Returns the first constant node of this edge.

    inline GraphNodeBaseBodyC & Node1()
    { return Source(); }
    // Returns the first node of this edge.

    inline const GraphNodeBaseBodyC & Node2() const
    { return Target(); }
    // Returns the second constant node of this edge.

    inline GraphNodeBaseBodyC & Node2()
    { return Target(); }
    // Returns the second node of this edge.
    
    inline SizeT Hash(void) const
    { return id; }
    //: Returns a hash value for the current node.
    // (Lower few bits are almost always zero.)
    
    size_t ID() const
      { return id; }
    //: Access edge id.
    
  protected:
    void SetID(size_t nid)
      { id = nid; }
    //: Set edge id.
    
  private:
    inline GraphAdjRepC & SourceAdj()
    { return source; }
    // Returns the the source adjacency.

    inline GraphAdjRepC & TargetAdj()
    { return target; }
    // Returns the target adjacency.

    GraphAdjRepC source; // adjacency with source node
    GraphAdjRepC target; // adjacency with target node
    size_t id;
    
    friend class GraphBaseBodyC; //InsEdge()
    friend class GraphAdjIterBaseC;  //OtherAdj()
    friend class GraphEdgeBaseC; //SourceAdj(), TargetAdj()
    friend std::ostream & operator<<(std::ostream & s, const GraphEdgeBaseBodyC & edge);
  };
  
  std::ostream & operator<<(std::ostream & s, const GraphEdgeBaseBodyC & edge);
  // Saves information about the edge into the output stream.

  // ----------------------------------------------------------------------
  // ************ GraphNodeBaseBodyC *******************************************
  // ----------------------------------------------------------------------
  
  //: The class GraphNodeBaseBodyC is a representation of graph node. 
  // It can
  // be linked into a list of graph node. An instance of this class
  // contains information about the edges which go into the node and
  // which go out.
  
  class GraphNodeBaseBodyC 
    : public DLinkC
  {
  public:
    inline GraphNodeBaseBodyC(GraphBaseBodyC &gr ,size_t nId)
	: graph(&gr),
	  id(nId)
	{}
    //: Creates the node of a graph.
    
    inline GraphNodeBaseBodyC();
    // Creates the node of a graph.
    
    inline virtual ~GraphNodeBaseBodyC();
    //: Destructor.
    // This ensures derived classes are deleted properly.
    
    inline const IntrDListC<GraphAdjRepC> & In() const;
    // Returns the constant list of graph adjacencies which have this node
    // as target node.
    
    inline const IntrDListC<GraphAdjRepC> & Out() const;
    // Returns the constant list of graph adjacencies which have this node
    // as source node.
    
    inline IntrDListC<GraphAdjRepC> & In();
    // Returns the list of graph adjacencies which have this node
    // as target node.

    inline IntrDListC<GraphAdjRepC> & Out();    
    // Returns the list of graph adjacencies which have this node
    // as source node.
  
    inline void DelInEdges();
    // Delete all input edges.
  
    inline void DelOutEdges();
    // Delete all output edges.
    
    inline SizeT Hash(void) const
      { return id; }
    //: Returns a hash value for the current node.
    // (Lower few bits of a ptr are almost always zero.)

    size_t ID() const
      { return id; }
    //: Access node id.

  protected:
    void SetID(size_t nid)
      { id = nid; }
    //: Set node id.
    
    const GraphBaseBodyC &BaseGraph() const 
      { return *graph; }
    // Get the graph to which the node belongs.
    
    GraphBaseBodyC &BaseGraph()
      { return *graph; }
    // Get the graph to which the node belongs.

    inline GraphTypeT GraphType() const;
    //: Get the type of graph we're in.
    
  private:
    IntrDListC<GraphAdjRepC> in;        // the list of input edges
    IntrDListC<GraphAdjRepC> out;       // the list of output edges for directed graph.
    GraphBaseBodyC * graph; // Pointer to the graph.
    size_t id;
  };

  // ----------------------------------------------------------------------
  // ************ GraphEdgeBaseC **********************************************
  // ----------------------------------------------------------------------
  
  class GraphNodeBaseC;
  class GraphBaseBodyC;
  
  //: Graph Edge handle.
  // The class also serves as an iterator of the list of graph edges.
  
  class GraphEdgeBaseC
    : public IntrDLIterC<GraphEdgeBaseBodyC>
  {
  public:
    
    // Constructors.
    // -------------
    
    inline GraphEdgeBaseC(GraphBaseBodyC & graph);
    // Creates the edge iterator of the 'graph'. The iterator will
    // point to the first edge in the edge list of the graph.
    
    inline GraphEdgeBaseC(const GraphEdgeBaseC & edge)
      : IntrDLIterC<GraphEdgeBaseBodyC>(edge)
    {}
    // Copy constructor.
    
    inline bool operator==(const GraphEdgeBaseC &Oth) const
    { return &Data() == &Oth.Data(); }
    // True when both iterators are pointing at the same
    // edge.
    
    inline SizeT Hash(void) const
    { return Data().Hash(); }
    // Returns a hash value for the current edge.
    
    // Access to the neighboring graph lists.
    // --------------------------------------
    
    inline GraphNodeBaseC Source();
    // Returns the source node of this directed edge.
    
    inline GraphNodeBaseC Target();
    // Returns the target node of this directed edge.

    inline GraphNodeBaseC Node1();
    // Returns the first node of this undirected edge.
    
    inline GraphNodeBaseC Node2();
    // Returns the second node of this undirected edge.

    inline GraphAdjIterBaseC SourceAdj();
    // Returns the adjacency of the source node of this directed edge.

    inline GraphAdjIterBaseC TargetAdj();
    // Returns the adjacency of the target node of this directed edge.

    inline GraphAdjIterBaseC Node1Adj();
    // Returns the adjacency of the first node of this undirected edge.
 
    inline GraphAdjIterBaseC Node2Adj();
    // Returns the adjacency of the second node of this undirected edge.
    
    // Changing of the edge list.
    // --------------------------

    inline void Del();
    // Unlinks edge from the graph and deletes it. The  iterator
    // will point to the previous edge in the graph edge list.
    
    void Redirect();
    // Changes the orientation of the edge. In other words it means
    // swapping of the meaning of Node1(), Source() with Node2(), Target().
    // If the graph is undirected the order of edges is held.
    // If the graph is directed the order of edges of both nodes
    // is changed.
    
    inline GraphEdgeBaseC()
      {}
    // The default constructor used by constructors of arrays.
    // It constructs the invalid object.

    UIntT ID() const
      { return Data().ID(); }
    //: Access edge id.
    
  protected:
    // Direct access to the graph elements.
    // ------------------------------------

    inline GraphNodeBaseBodyC & TargetRep()
    { return Data().Target(); }
    // Returns the target node of this directed edge.
    
    inline GraphNodeBaseBodyC & SourceRep()
    { return Data().Source(); }
    // Returns the source node of this directed edge.
    
    inline GraphNodeBaseBodyC & Node1Rep()
    { return SourceRep(); }
    // Returns the first node of this undirected edge.
 
    inline GraphNodeBaseBodyC & Node2Rep()
    { return TargetRep(); }
    // Returns the second node of this undirected edge.
    
    inline 
    GraphEdgeBaseC(GraphEdgeBaseBodyC & edge, IntrDListC<GraphEdgeBaseBodyC> & list)
      : IntrDLIterC<GraphEdgeBaseBodyC>(list, edge)
	{}
    // Creates the iterator pointing to the edge which belongs to the 'list'.
    
    inline GraphBaseBodyC & Graph();
    //: Returns the graph.
    
    friend class GraphNodeBaseC;
    friend class GraphAdjIterBaseC;
    friend class GraphBaseBodyC;
  };
  
  // ----------------------------------------------------------------------
  // ************ GraphNodeBaseC **********************************************
  // ----------------------------------------------------------------------

  //: Graph node handle.
  // This class also serves as an iterator of the list of graph nodes.
  
  class GraphNodeBaseC
    : public IntrDLIterC<GraphNodeBaseBodyC>
  {
  public:
    inline GraphNodeBaseC(GraphBaseBodyC & graph);
    // Creates the node iterator of the 'graph'. The iterator will
    // point to the first node in the node list of the graph.
    
    inline GraphNodeBaseC(const GraphNodeBaseC & node)
      : IntrDLIterC<GraphNodeBaseBodyC>(node)
    {}
    // Copy constructor.
    
    inline GraphNodeBaseC(const IntrDLIterC<GraphNodeBaseBodyC> &it)
      :  IntrDLIterC<GraphNodeBaseBodyC>(it)
      {}
    //: Base class constructor.
    
    inline bool operator==(const GraphNodeBaseC &Oth) const
    { return &NodeRep() == &Oth.NodeRep(); }
    // True when both iterators are pointing at the same
    // node.
    
    inline SizeT Hash(void) const
    { return NodeRep().Hash(); }
    // Returns a hash value for the current node.
    
    inline GraphNodeBaseBodyC & NodeRep()
    { return Data(); }
    // Returns the node.
    
    inline const GraphNodeBaseBodyC & NodeRep() const
    { return Data(); }
    // Returns the node.
    
    inline GraphAdjIterBaseC In();
    // Returns the iterator that is set to the first item of the list
    // of the graph adjacencies in which this node is the target node.
    
    inline GraphAdjIterBaseC Out();
    // Returns the iterator that is set to the first item of the list
    // of the graph adjacencies in which this node is the source node.
    
    inline void Del();
    // Unlinks the node from the graph and deletes it. The  iterator
    // will point to the previous node in the graph node list.
    
    inline GraphNodeBaseC()
      {}
    // The default constructor used by constructors of arrays.
    // It constructs the unvalid object which must be initialized by the
    // member function LateItemSet().
    
    UIntT ID() const
      { return Data().ID(); }
    //: Access edge id.
    
  protected:
    inline GraphNodeBaseC(GraphNodeBaseBodyC & node, IntrDListC<GraphNodeBaseBodyC> & list);
    //: Creates the iterator of graph nodes which points to the 'node'.
    
    inline GraphNodeBaseC(GraphNodeBaseBodyC &aNode,GraphBaseBodyC &bg);
    //: Creates the iterator of graph nodes which points to the 'node'.
    
    inline GraphNodeBaseC(GraphNodeBaseBodyC &aNode,GraphBaseC &bg);
    //: Creates the iterator of graph nodes which points to the 'node'.
    
    inline GraphBaseBodyC & Graph();
    //: Returns the whole graph.
    
    friend class GraphAdjIterBaseC;
    friend class GraphEdgeBaseC;
    friend class GraphBaseBodyC;
    friend class GraphBaseC;
  };
  
  // ----------------------------------------------------------------------
  // ************ GraphAdjIterBaseC ***********************************************
  // ----------------------------------------------------------------------
  
  //: The class GraphAdjIterBaseC is the iterator of the list of graph adjacencies
  //: of a graph node. 
  // Futhermore, the class provides access to the graph 
  // elements which are in the neighbourhood of the edge.
  
  class GraphAdjIterBaseC
    : public IntrDLIterC<GraphAdjRepC>
  {
  public:
    inline GraphAdjIterBaseC(const GraphAdjIterBaseC & adj);
    // Copy constructor.
  
    inline const GraphEdgeBaseBodyC & EdgeRep() const
    { return Data().Edge(); }
    // Returns the edge which is connected to this graph adjacency.
    
    inline GraphEdgeBaseBodyC & EdgeRep()
    { return Data().Edge(); }
    // Returns the edge which is connected to this graph adjacency.

    inline const GraphNodeBaseBodyC & SourceRep() const
    { return Data().Edge().Source(); }
    // Returns the source node of the edge which is connected
    // to this graph adjacency.

    inline GraphNodeBaseBodyC & SourceRep()
    { return Data().Edge().Source(); }
    // Returns the source node of the edge which is connected
    // to this graph adjacency.

    inline const GraphNodeBaseBodyC & TargetRep() const
    { return Data().Edge().Target(); }
    // Returns the target node of the edge which is connected
    // to this graph adjacency.

    inline GraphNodeBaseBodyC & TargetRep()
    { return Data().Edge().Target(); }
    // Returns the target node of the edge which is connected
    // to this graph adjacency.

    inline const GraphNodeBaseBodyC & Node1Rep() const
    { return SourceRep(); }
    // Returns the first node of the edge which is connected
    // to this graph adjacency.
    
    inline GraphNodeBaseBodyC & Node1Rep()
    { return SourceRep(); }
    // Returns the first node of the edge which is connected
    // to this graph adjacency.
    
    inline const GraphNodeBaseBodyC & Node2Rep() const
    { return TargetRep(); }
    // Returns the second node of the edge which is connected
    // to this graph adjacency.

    inline GraphNodeBaseBodyC & Node2Rep()
    { return TargetRep(); }
    // Returns the second node of the edge which is connected
    // to this graph adjacency.
    
    inline GraphNodeBaseC  Node1()
    { return GraphNodeBaseC(SourceRep(),Graph()); }
    // Returns the first node of the edge which is connected
    // to this graph adjacency.
    
    inline GraphNodeBaseC Node2()
    { return GraphNodeBaseC(TargetRep(),Graph()); }
    // Returns the second node of the edge which is connected
    // to this graph adjacency.
    
    inline GraphEdgeBaseC Edge();
    // Returns the edge which is connected to this graph adjacency.

    inline GraphNodeBaseC Node();
    // Returns the node which is the owner of the list to which
    // this graph adjacency belongs to.
 
    inline GraphNodeBaseC OtherNode();
    // Returns the node which is not the owner of the list to which
    // this graph adjacency belongs to.
 
    inline GraphAdjIterBaseC & OtherAdj();
    // Moves to the graph adjacency of the other node.
    
    inline GraphAdjIterBaseC();
    // The default constructor used by constructors of arrays.
    
  protected:
    inline 
    GraphAdjIterBaseC(const IntrDLIterC<GraphAdjRepC> & iter, GraphBaseBodyC & grph);
    // Creates the graph adjacency iterator from the iterator of adjacency and
    // the graph which the iterator belongs to.

    inline 
    GraphAdjIterBaseC(GraphAdjRepC &              adj, 
              IntrDListC<GraphAdjRepC> & list, 
              GraphBaseBodyC &                grph);
    // Creates the graph adjacency iterator from the adjacency, the list
    // of adjacencies which the adjacency belongs to and
    // the graph which the adjacency belongs to.
    
    inline GraphBaseBodyC & Graph();
    // Returns the whole graph.
    
  private:
    GraphBaseBodyC * graph; // Pointer to the graph.
    
    friend class GraphNodeBaseC;
    friend class GraphEdgeBaseC;
  };
  
  // ----------------------------------------------------------------------
  // ************ GraphBaseBodyC **************************************************
  // ----------------------------------------------------------------------
  
  class GraphNodeBaseC;
  class GraphEdgeBaseC;
  class GraphBaseC;
  
  // The class GraphBaseBodyC represents graph  which can be directed or
  // undirected. It contains a collection of nodes and a collection of
  // edges between nodes. 
  
  class GraphBaseBodyC
    : private IntrDListC<GraphNodeBaseBodyC>,  // nodes
      private IntrDListC<GraphEdgeBaseBodyC>,   // edges
      public RCBodyVC
  {
  public:
    
    inline GraphBaseBodyC(GraphTypeT type = GRAPH_DIRECTED)
      : IntrDListC<GraphNodeBaseBodyC>(true), // Ensure its deleted..
	IntrDListC<GraphEdgeBaseBodyC>(true), // Ensure its deleted..
	gType(type),
        edgeIDs(0),
        nodeIDs(0)
      {}
    // Creates an empty graph, directed or undirected.
    
    virtual ~GraphBaseBodyC()
      {}
    //: Destructor.
    
    inline GraphTypeT Type() const
    { return gType; }
    // Returns the type of the graph, if it is directed or undirected.
    
    inline GraphNodeBaseC InsNode();
    // Inserts one node to the graph. Returns the node iterator. 
    
    inline GraphEdgeBaseC InsEdge(GraphNodeBaseC & fromNode, GraphNodeBaseC & toNode);
    // Inserts one egde to the graph. Returns the edge iterator. 

    inline const IntrDListC<GraphNodeBaseBodyC> & Nodes() const
    { return *this; }
    // Returns the constant list of nodes.

    inline IntrDListC<GraphNodeBaseBodyC> & Nodes()
    { return *this; }
    // Returns the list of nodes.

    inline const IntrDListC<GraphEdgeBaseBodyC> & Edges() const
    { return *this; }
    // Returns the constant list of edges.
    
    inline IntrDListC<GraphEdgeBaseBodyC> & Edges()
    { return *this; }
    // Returns the list of edges.
    
    UIntT EdgeIDs() const
      { return edgeIDs; }
    //: Return the number of edge ID's issused.
    
    UIntT NodeIDs() const
      { return nodeIDs; }
    //: Return the number of node ID's issused.
    
  protected:
    inline GraphNodeBaseC InsNode(GraphNodeBaseBodyC &ANode);
    // Inserts one node to the graph. Returns the node iterator. 
    // ANode must be a GraphNodeBaseBodyC that's just been created,
    // and not a part of another graph.
  
    inline GraphEdgeBaseC InsEdge(GraphEdgeBaseBodyC &AnEdge);
    // Inserts one edge to the graph. Returns the edge iterator. 
    // AnEdge must be a GraphEdgeBaseBodyC that's just been created,
    // and not a part of another graph.
    
    UIntT AllocEdgeID() 
      { return edgeIDs++; }
    //: Allocate a new edge ID.

    UIntT AllocNodeID() 
      { return nodeIDs++; }
    //: Allocate a new node ID.
    
  private:
    GraphTypeT gType; // The type of the graph: directed x undirected
    UIntT edgeIDs;    // Count of edge id's issused
    UIntT nodeIDs;    // Count of node id's issused.
    
    friend class GraphBaseC;
    friend class GraphEdgeBaseC;
    friend class GraphNodeBaseC;
  };
  
  // -------------------------------------------------------------------------
  // ********** GraphBaseC ***************************************************
  // -------------------------------------------------------------------------

  //: Reference counted graph.
  // Use GraphBaseC for all new code. <p>
  // The class GraphBaseC represents reference counted graph
  // which can be directed or undirected. It contains a collection
  // of nodes and a collection of edges between nodes. 
  
  class GraphBaseC
    : public RCHandleVC<GraphBaseBodyC> 
  {
    public:
    
    // Constructors, assigment, and destructor
    // ---------------------------------------
    
    inline GraphBaseC(GraphTypeT atype = GRAPH_DIRECTED)
      : RCHandleVC<GraphBaseBodyC>(*new GraphBaseBodyC(atype))
      {}
    //: Creates an empty graph, directed or undirected.
    
    inline GraphBaseC(const GraphBaseC & g)
      : RCHandleVC<GraphBaseBodyC>(g)
      {}
    // Creates another access to the graph 'g'. The constructor does not
    // change the content of the graph 'g', but its reference counting.
    // However, the content of the graph 'g' can be changed through
    // the this new access later.
    
    // Creation of the structure of the graph.
    // ---------------------------------------
    
    inline GraphNodeBaseC InsNode()
    { return Body().InsNode(); }
    // Inserts one node to the graph. Returns the node iterator. 
    
    inline GraphEdgeBaseC InsEdge(GraphNodeBaseC & fromNode, GraphNodeBaseC & toNode)
     { return Body().InsEdge(fromNode, toNode); }
    // Inserts one edge to the graph. Returns the edge iterator.
    
    // Access to the parts of the graph
    // --------------------------------
    
    inline GraphTypeT Type() const
    { return Body().Type(); }
    // Returns the type of the graph, if it is directed or undirected.
    
    inline const IntrDListC<GraphNodeBaseBodyC> & Nodes() const
    { return Body().Nodes(); }
    // Returns the constant list of nodes.
  
    inline IntrDListC<GraphNodeBaseBodyC> & Nodes()
    { return Body().Nodes(); }
    // Returns the list of nodes.
  
    inline const IntrDListC<GraphEdgeBaseBodyC> & Edges() const
    { return Body().Edges(); }
    // Returns the constant list of edges.
    
    inline IntrDListC<GraphEdgeBaseBodyC> & Edges()
    { return Body().Edges(); }
    // Returns the list of edges.
    
    bool IsCyclic();
    // Test if a directed graph contains cycles.

    UIntT EdgeIDs() const
      { return Body().EdgeIDs(); }
    //: Return the number of edge ID's issued.
    
    UIntT NodeIDs() const
      { return Body().NodeIDs(); }
    //: Return the number of node ID's issued.
    
  protected:
    UIntT AllocEdgeID() 
      { return Body().AllocEdgeID(); }
    //: Allocate a new edge ID.

    UIntT AllocNodeID() 
      { return Body().AllocNodeID(); }
    //: Allocate a new node ID.

    GraphBaseBodyC &Body()
      { return RCHandleC<GraphBaseBodyC>::Body(); }
    //: Access body.

    const GraphBaseBodyC &Body() const
      { return RCHandleC<GraphBaseBodyC>::Body(); }
    //: Access body.

    inline GraphNodeBaseC InsNode(GraphNodeBaseBodyC &aNode)
    { return Body().InsNode(aNode); }
    // Inserts one node to the graph. Returns the node iterator. 
    // ANode must be a GraphNodeBaseBodyC that's just been created,
    // and not a part of another graph.
    
    inline GraphEdgeBaseC InsEdge(GraphEdgeBaseBodyC &anEdge)
    { return Body().InsEdge(anEdge); }
    // Inserts one edge to the graph. Returns the edge iterator. 
    // AnEdge must be a GraphEdgeBaseBodyC that's just been created,
    // and not a part of another graph.
    
    friend class GraphNodeBaseBodyC;
  };
  
  // -------------------------------------------------------------------------
  // ********** GraphAdjRepC ***************************************************
  // -------------------------------------------------------------------------
  
  inline
  GraphAdjRepC::GraphAdjRepC(GraphNodeBaseBodyC & node, GraphEdgeBaseBodyC  & edge)
    : otherNode(&node), 
      me(&edge) 
  {}

  inline 
  const GraphNodeBaseBodyC & 
  GraphAdjRepC::OtherNode() const
  { return *otherNode; }

  inline 
  const GraphEdgeBaseBodyC & 
  GraphAdjRepC::Edge() const
  { return *me; }

  inline 
  GraphNodeBaseBodyC & 
  GraphAdjRepC::OtherNode()
  { return *otherNode; }

  inline 
  GraphEdgeBaseBodyC & 
  GraphAdjRepC::Edge()
  { return *me; }

  inline 
  void 
  GraphAdjRepC::SwapNodes(GraphAdjRepC & adj) {
    GraphNodeBaseBodyC * tmp = otherNode;
    otherNode           = adj.otherNode;
    adj.otherNode       = tmp;
  }
  
  // ----------------------------------------------------------------------
  // ************ GraphNodeBaseBodyC *******************************************
  // ----------------------------------------------------------------------
  
  inline GraphNodeBaseBodyC::GraphNodeBaseBodyC()
  {}
  
  inline const IntrDListC<GraphAdjRepC> & GraphNodeBaseBodyC::In() const
  { return in; }
  
  inline const IntrDListC<GraphAdjRepC> & GraphNodeBaseBodyC::Out() const {
    if(GraphType() == GRAPH_DIRECTED) return out;
    return in;
  }
  
  inline IntrDListC<GraphAdjRepC> & GraphNodeBaseBodyC::In()
  { return in; }
  
  inline IntrDListC<GraphAdjRepC> & GraphNodeBaseBodyC::Out() {
    if(GraphType() == GRAPH_DIRECTED) return out;
    return in;
  }
  
  inline 
  void 
  GraphNodeBaseBodyC::DelInEdges() {
    while (!In().IsEmpty())
      delete &In().First().Edge();
  }
  
  inline 
  void 
  GraphNodeBaseBodyC::DelOutEdges() {
    while (!Out().IsEmpty())
      delete &Out().First().Edge();
  }
  
  inline
  GraphNodeBaseBodyC::~GraphNodeBaseBodyC() {
    DelInEdges();
    DelOutEdges();
    Unlink();       // Unlink itself from the list of nodes.
  }
  
  // ----------------------------------------------------------------------
  // ************ GraphNodeBaseC ******************************************
  // ----------------------------------------------------------------------
  
  inline GraphNodeBaseC::GraphNodeBaseC(GraphNodeBaseBodyC & node, IntrDListC<GraphNodeBaseBodyC> & list)
    : IntrDLIterC<GraphNodeBaseBodyC>(list, node)
  {}
  
  inline GraphNodeBaseC::GraphNodeBaseC(GraphNodeBaseBodyC &aNode,GraphBaseBodyC &bg)
    : IntrDLIterC<GraphNodeBaseBodyC>(bg.Nodes(), aNode)
  {}

  inline GraphNodeBaseC::GraphNodeBaseC(GraphNodeBaseBodyC &aNode,GraphBaseC &bg)
    : IntrDLIterC<GraphNodeBaseBodyC>(bg.Nodes(), aNode)
  {}

  inline GraphTypeT GraphNodeBaseBodyC::GraphType()  const
  { return graph->Type(); }
  
  // ----------------------------------------------------------------------
  // ************ GraphAdjIterBaseC ***************************************
  // ----------------------------------------------------------------------
  
  inline
  GraphAdjIterBaseC::GraphAdjIterBaseC(const IntrDLIterC<GraphAdjRepC> & iter, GraphBaseBodyC & grph)
    : IntrDLIterC<GraphAdjRepC>(iter), 
      graph(&grph)
  {}

  inline 
  GraphAdjIterBaseC::GraphAdjIterBaseC(GraphAdjRepC & adj, 
		       IntrDListC<GraphAdjRepC> & list, 
		       GraphBaseBodyC & grph)
    : IntrDLIterC<GraphAdjRepC>(list, adj), 
      graph(&grph)
  {}

  inline 
  GraphAdjIterBaseC::GraphAdjIterBaseC()
    : IntrDLIterC<GraphAdjRepC>()
  {}
  
  // ----------------------------------------------------------------------
  // ************ GraphEdgeBaseC *******************************************
  // ----------------------------------------------------------------------
  
   
  inline GraphEdgeBaseC::GraphEdgeBaseC(GraphBaseBodyC & graph)
    : IntrDLIterC<GraphEdgeBaseBodyC>(graph.Edges())
  {}
  
  inline GraphNodeBaseC GraphEdgeBaseC::Source()
  { return GraphNodeBaseC(SourceRep(), Graph().Nodes()); }

  inline GraphNodeBaseC GraphEdgeBaseC::Target()
  { return GraphNodeBaseC(TargetRep(), Graph().Nodes()); } 

  inline GraphNodeBaseC GraphEdgeBaseC::Node1()
  { return Source(); }
    
  inline GraphNodeBaseC GraphEdgeBaseC::Node2()
  { return Target(); }

  inline GraphAdjIterBaseC GraphEdgeBaseC::SourceAdj()
  { return GraphAdjIterBaseC(Data().SourceAdj(), SourceRep().Out(), Graph()); }

  inline GraphAdjIterBaseC GraphEdgeBaseC::TargetAdj()
  { return GraphAdjIterBaseC(Data().TargetAdj(), TargetRep().In(), Graph()); }
  
  inline GraphAdjIterBaseC GraphEdgeBaseC::Node1Adj()
  { return GraphAdjIterBaseC(Data().SourceAdj(), SourceRep().Out(), Graph()); }
  
  inline GraphAdjIterBaseC GraphEdgeBaseC::Node2Adj()
  { return GraphAdjIterBaseC(Data().TargetAdj(), TargetRep().Out(), Graph()); }

  inline 
  void GraphEdgeBaseC::Del() {
    GraphEdgeBaseBodyC * edge = & Data();
    Prev();
    delete edge;
  }

  inline GraphBaseBodyC & GraphEdgeBaseC::Graph()
  { return const_cast<GraphBaseBodyC&>(static_cast<const GraphBaseBodyC&>(List())); }
  
  // ----------------------------------------------------------------------
  // ************ GraphNodeBaseC *******************************************
  // ----------------------------------------------------------------------

  inline GraphNodeBaseC::GraphNodeBaseC(GraphBaseBodyC & graph)
    : IntrDLIterC<GraphNodeBaseBodyC>(graph.Nodes())
  {}

  inline GraphAdjIterBaseC GraphNodeBaseC::In()
  { return GraphAdjIterBaseC(Data().In(), Graph()); } 
    
  inline GraphAdjIterBaseC GraphNodeBaseC::Out()
  { return GraphAdjIterBaseC(Data().Out(), Graph()); }

  inline 
  void 
  GraphNodeBaseC::Del() {
    GraphNodeBaseBodyC * node = & Data();
    Prev();
    delete node;
  }

  inline GraphBaseBodyC & GraphNodeBaseC::Graph()
   { return const_cast<GraphBaseBodyC &> (static_cast<const GraphBaseBodyC &>(List())); }
  // A little nasty but easyer than fixing List().
  
  // -------------------------------------------------------------------------
  // ********** GraphBaseBodyC ***************************************************
  // -------------------------------------------------------------------------
  
  inline 
  GraphNodeBaseC 
  GraphBaseBodyC::InsNode() {
    GraphNodeBaseBodyC *rep = new GraphNodeBaseBodyC(*this,AllocNodeID());
    Nodes().InsLast(*rep);
    return GraphNodeBaseC(*rep, Nodes());
  }
  
  inline 
  GraphEdgeBaseC 
  GraphBaseBodyC::InsEdge(GraphNodeBaseC & fromNode, GraphNodeBaseC & toNode) {
    GraphEdgeBaseBodyC & edge = *new GraphEdgeBaseBodyC(fromNode.Data(),toNode.Data(),AllocEdgeID());
    Edges().InsLast(edge);
    fromNode.Data().Out().InsLast(edge.source);
    toNode.Data().In().InsLast(edge.target);
    return GraphEdgeBaseC(edge, Edges());
  }
  
  inline 
  GraphNodeBaseC 
  GraphBaseBodyC::InsNode(GraphNodeBaseBodyC &Rep) 
  { 
    Nodes().InsLast(Rep);
    return GraphNodeBaseC(Rep,Nodes()); 
  }
  
  inline 
  GraphEdgeBaseC 
  GraphBaseBodyC::InsEdge(GraphEdgeBaseBodyC &edge) {
    Edges().InsLast(edge);
    edge.Source().Out().InsLast(edge.source); // Seems strange, but look 
    edge.Target().In().InsLast(edge.target);  // at GraphEdgeBaseBodyC constructor.
    return GraphEdgeBaseC(edge, Edges());
  }
  
  // ----------------------------------------------------------------------
  // ************ GraphAdjIterBaseC ***********************************************
  // ----------------------------------------------------------------------
  
  inline
  GraphAdjIterBaseC::GraphAdjIterBaseC(const GraphAdjIterBaseC & adj)
    : IntrDLIterC<GraphAdjRepC>(adj),
      graph(adj.graph)
  {}
  
  inline 
  GraphEdgeBaseC 
  GraphAdjIterBaseC::Edge()
  { return GraphEdgeBaseC(Data().Edge(), graph->Edges()); }

  inline 
  GraphNodeBaseC 
  GraphAdjIterBaseC::Node() {
    return GraphNodeBaseC(&Data() == &(Data().Edge().target)
		      ? Data().Edge().Target() 
		      : Data().Edge().Source(),
		      graph->Nodes());
  }
  
  inline 
  GraphNodeBaseC 
  GraphAdjIterBaseC::OtherNode()
  { return GraphNodeBaseC(Data().OtherNode(), graph->Nodes()); }

  inline 
  GraphAdjIterBaseC & 
  GraphAdjIterBaseC::OtherAdj() {
    if (&Data() == &(Data().Edge().target)) 
      SetIter(Data().OtherNode().Out(),Data().Edge().source);
    else
      SetIter(Data().OtherNode().In(),Data().Edge().target);
    return *this;
  }
  
  inline GraphBaseBodyC &GraphAdjIterBaseC::Graph() { 
    RavlAssert(graph != 0);
    return *graph; 
  }



}

#endif


