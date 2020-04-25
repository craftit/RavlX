// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVL_BGRAPHEDGE_HEADER
#define RAVL_BGRAPHEDGE_HEADER 1
///////////////////////////////////////////////////////////////
//! file="Ravl/Core/Container/Branch/BGraphEdge.hh"
//! lib=RavlCore
//! author="Charles Galambos"
//! date="19/2/1997"
//! docentry="Ravl.API.Core.Branch"
//! rcsid="$Id$"

#include "Ravl/BGraphBase.hh"

namespace RavlN {
  
  template<class NT,class ET> class BGraphNodeC;
  template<class NT,class ET> class BGraphNodeHC;
  template<class NT,class ET> class BGraphNodeIterC;
  template<class NT,class ET> class BGraphEdgeC;
  template<class NT,class ET> class BGraphAdjIterC;
  template<class NT,class ET> class BGraphC;
  
  //! userlevel=Develop
  //: BGraph Edge.
  
  template<class NT,class ET>
  class BGraphEdgeC 
    : public BGraphEdgeBaseC 
  {
  public:
    BGraphEdgeC(BGraphTypesC::NodeHandleT From,BGraphTypesC::NodeHandleT To,const ET &nDat);
    // Constructor.
    
    inline BGraphTypesC::NodeHandleT Source() const;
    // Source of edge.
    
    //  inline const BGraphNodeC<NT,ET> &Source() const;
    // Source of edge.
    
    inline BGraphTypesC::NodeHandleT Target() const;
    // Target of edge.
    
    //inline const BGraphNodeC<NT,ET> &Target() const ;
    // Target of edge.
    
    ET &Data() { return Dat; }
    // Access edge data.
    
    const ET &Data() const { return Dat; }
    // Constant access to edge data.
    
    NodeHandleT NodeH(Dir Way) const { return BGraphEdgeBaseC::Node(Way); }
    // Get nodes at ends of edge.
    
    bool operator==(const BGraphEdgeC &oth) const { return &oth == this; }
    // Are they the same edge ?
    
    SizeT Hash() const { return BGraphEdgeBaseC::Hash(); }
    // Get a hash value of this edge
    
  private:
    ET Dat;
    
    friend class BGraphC<NT,ET>;
    friend class BGraphAdjIterC<NT,ET>;
  };
  
  //! userlevel=Normal
  //: BGraph edge handle.

  template<class NT,class ET>
  class BGraphEdgeHC {
  public:
    BGraphEdgeHC() 
    {}
    //: Default constructor.
    
    BGraphEdgeHC(BGraphEdgeC<NT,ET> &anedge)
      : edge(anedge) 
    {}
    //: Constructor.
    
    bool IsValid() const 
    { return edge.IsValid(); }
    //: Is handle valid ?
    
    BGraphEdgeC<NT,ET> & Edge() 
    { return *edge; }
    //: Get edge.
    
    const BGraphEdgeC<NT,ET> & Edge() const 
    { return *edge; }
    //: Get edge.
    
    ET &Data() 
    { return edge->Data(); }
    //: Access data.
    
    const ET &Data() const 
    { return edge->Data(); }
    //: Constant access to data.
    
    bool operator==(const BGraphEdgeHC<NT,ET> &oth) const 
    { return edge == oth.edge; }
    //: Comparison.
  
    void Invalidate() 
    { edge.Invalidate(); }
    //: Invalidate handle.
    
    inline BGraphTypesC::NodeHandleT Source() const;
    //: Get source of link.
    
    inline BGraphTypesC::NodeHandleT Target() const;
    //: Get target of link.
    
    inline SizeT Hash() const
    { return edge->Hash(); }
    //: Get hash value.
    
  private:
    SmartPtrC<BGraphEdgeC<NT,ET> > edge;
  };
  
}

#include "Ravl/BGraphNode.hh"
 
namespace RavlN { 
  ////////////////////////////////////////////////////////////////////
  
  template<class NT,class ET> 
  BGraphEdgeC<NT,ET>::BGraphEdgeC(BGraphTypesC::NodeHandleT From,
				  BGraphTypesC::NodeHandleT To,
				const ET &nDat)
    : BGraphEdgeBaseC(From,To),
      Dat(nDat)
  {}
  
  template<class NT,class ET> 
  BGraphTypesC::NodeHandleT BGraphEdgeC<NT,ET>::Source() const 
  { return Node(Bkw); }
  
  template<class NT,class ET> 
  BGraphTypesC::NodeHandleT BGraphEdgeC<NT,ET>::Target() const 
  { return Node(Fwd); }

  /////////////////////////////////////////////////////////
  
  template<class NT,class ET> 
  inline BGraphTypesC::NodeHandleT BGraphEdgeHC<NT,ET>::Source() const 
  { return edge->Source(); }
  
  template<class NT,class ET> 
  inline BGraphTypesC::NodeHandleT BGraphEdgeHC<NT,ET>::Target() const 
  { return edge->Target(); }
}


#endif
