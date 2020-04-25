// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2006, Omniperception Ltd.
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVLGUI_MARKUPINFO_HEADER
#define RAVLGUI_MARKUPINFO_HEADER 1
//! rcsid="$Id: fixSource.cfg 5642 2006-06-23 10:51:11Z craftit $"
//! lib=RavlGUI2D
//! docentry="Ravl.API.Graphics.GTK.Markup Canvas"

#include "Ravl/RCHandleV.hh"
#include "Ravl/Point2d.hh"
#include "Ravl/Stream.hh"
#include "Ravl/RealRange2d.hh"
#include "Ravl/DList.hh"
#include "Ravl/Hash.hh"

namespace RavlGUIN {
  using namespace RavlN;
  class MouseEventC;
  
  enum MouseEventTypeT 
    { MET_Press,MET_Move,MET_Release };

  class GUIMarkupCanvasBodyC;
  class GUIMarkupCanvasC;
  
  //! userlevel=Normal
  //: Info about a particular markup.
  
  class MarkupInfoBodyC 
    : public RCBodyVC
  {
  public:
    MarkupInfoBodyC(Int64T _id = -1,IntT zOrder = -1);
    //: Default constructor.
    
    virtual Point2dC Centroid(GUIMarkupCanvasBodyC &mv) const;
    //: Find center of object
    
    virtual RealRange2dC Coverage(GUIMarkupCanvasBodyC &mv) const;
    //: Range of pixels covered by this one (Garanteed) 
    
    virtual RealRange2dC Extent(GUIMarkupCanvasBodyC &mv) const;
    //: Extent of object.
    
    virtual bool CanClip() const
    { return false; }
    //: Can render method clip the region it updates ?
    
    virtual bool Render(GUIMarkupCanvasBodyC &mv,const RealRange2dC &area,bool selected);
    //: Method for rendering frame.
    
    virtual RealT SelectTest(GUIMarkupCanvasBodyC &mv,const Point2dC &at,const MouseEventC &me);
    //: Check if we've selected something in this object.
    //!return: Score on how close the selection is, the smaller the value the better (must be positive.) 
    
    virtual bool MouseEventPress(GUIMarkupCanvasBodyC &mv,const Point2dC &at,const MouseEventC &me,IntT &mouseState,bool &refresh);
    //: Handle mouse event.
    // Returns true if even has been handled.
    
    virtual bool MouseEventMove(GUIMarkupCanvasBodyC &mv,const Point2dC &at,const MouseEventC &me,IntT &mouseState,bool &refresh);
    //: Handle mouse event.
    // Returns true if even has been handled.
    
    virtual bool MouseEventRelease(GUIMarkupCanvasBodyC &mv,const Point2dC &at,const MouseEventC &me,IntT &mouseState,bool &refresh);
    //: Handle mouse event.
    // Returns true if even has been handled.
    
    Int64T Id() const
    { return id; }
    //: Get markup ID.
    
    void SetID(Int64T newId)
    { id = newId; }
    //: Setup markup ID.
    
    IntT ZOrder() const
    { return zOrder; }
    //: Return Z oreder id
    
    void PushLayer(const IntT layerId);
    //: Add a layer
    
    IntT PopLayer();
    //: Remove a layer
    
    void SetLayerList(DListC<IntT> layerId);
    //: Set the layer id list
    
    const DListC<IntT> LayerList() const
    { return m_layerId; }
    //: Get the layer id list
    
    HashC<StringC, StringC> &Info()
    { return m_info; }
    //: Get extra markup info
    
  protected:
    bool SetDrawStyle(GUIMarkupCanvasBodyC &mv);
    //: Setup up the drawing context
    // Returns false if the object need not be drawn
  
    Int64T id;
    IntT zOrder;
    RealRange2dC coverage;
    RealRange2dC extent;
    DListC<IntT> m_layerId;           //: Ordered list of render layers affecting this markup
    HashC<StringC, StringC> m_info;   //: Hash of extra parameters
  };
  
  //! userlevel=Normal
  //: Info about a particular markup. 
  //!cwiz:author
  
  class MarkupInfoC
    : public RCHandleC<MarkupInfoBodyC>
  {
  public:
    
    MarkupInfoC(Int64T _id,IntT zOrder = - 1) 
      : RCHandleC<MarkupInfoBodyC>(*new MarkupInfoBodyC(_id, zOrder))
    {}
    //: Default constructor. 
    //!cwiz:author
    
    MarkupInfoC(Int64T _id) 
      : RCHandleC<MarkupInfoBodyC>(*new MarkupInfoBodyC(_id))
    {}
    //: Default constructor. 
    //!cwiz:author
    
    MarkupInfoC()
    {}
    //: Default constructor
    // Creates an invalid handle.
    
    Point2dC Centroid(GUIMarkupCanvasBodyC &mv) const
    { return Body().Centroid(mv); }
    //: Find center of object
    
    RealRange2dC Coverage(GUIMarkupCanvasBodyC &mv) const
    { return Body().Coverage(mv);  }
    //: Range of pixels covered by this one (Garanteed) 
    
    RealRange2dC Extent(GUIMarkupCanvasBodyC &mv) const
    { return Body().Extent(mv); }
    //: Extent of object.
    
    bool CanClip() const
    { return Body().CanClip(); }
    //: Can render method clip the region it updates ?
    
    bool Render(GUIMarkupCanvasBodyC & mv,const RealRange2dC &area,bool selected) 
    { return Body().Render(mv,area,selected); }
    //: Method for rendering frame. 
    //!cwiz:author
    
    RealT SelectTest(GUIMarkupCanvasBodyC &mv,const Point2dC &at,const MouseEventC &me)
    { return Body().SelectTest(mv,at,me); }
    //: Check if we've selected something in this object.
    //!return: Score on how close the selection is, the smaller the value the better (must be positive.) 
    
    bool MouseEventPress(GUIMarkupCanvasBodyC & mv,const Point2dC & at,const MouseEventC & me,IntT & mouseState,bool & refresh) 
    { return Body().MouseEventPress(mv,at,me,mouseState,refresh); }
    //: Handle mouse event. 
    // Returns true if even has been handled.
    //!cwiz:author
    
    bool MouseEventMove(GUIMarkupCanvasBodyC & mv,const Point2dC & at,const MouseEventC & me,IntT & mouseState,bool & refresh) 
    { return Body().MouseEventMove(mv,at,me,mouseState,refresh); }
    //: Handle mouse event. 
    // Returns true if even has been handled.
    //!cwiz:author
    
    bool MouseEventRelease(GUIMarkupCanvasBodyC & mv,const Point2dC & at,const MouseEventC & me,IntT & mouseState,bool & refresh) 
    { return Body().MouseEventRelease(mv,at,me,mouseState,refresh); }
    //: Handle mouse event. 
    // Returns true if even has been handled.
    //!cwiz:author
    
    Int64T Id() const
    { return Body().Id(); }
    //: Get markup ID. 
    //!cwiz:author
    
    void SetID(Int64T newId) 
    { Body().SetID(newId); }
    //: Setup markup ID. 
    //!cwiz:author
    
    IntT ZOrder() const
    { return Body().ZOrder(); }
    //: Return Z order id 
    //!cwiz:author
    
    void PushLayer(const IntT layerId)
    { Body().PushLayer(layerId); }
    //: Add a layer
    
    IntT PopLayer()
    { return Body().PopLayer(); }
    //: Remove a layer
    
    void SetLayerList(DListC<IntT> layerId)
    { Body().SetLayerList(layerId); }
    //: Set the layer id list
    
    const DListC<IntT> LayerList() const
    { return Body().LayerList(); }
    //: Get the layer id list
    
    HashC<StringC, StringC> &Info()
    { return Body().Info(); }
    //: Get extra markup info
    
  protected:
    MarkupInfoC(MarkupInfoBodyC &bod)
     : RCHandleC<MarkupInfoBodyC>(bod)
    {}
    //: Body constructor. 
    
    MarkupInfoC(const MarkupInfoBodyC *bod)
      : RCHandleC<MarkupInfoBodyC>(bod)
    {}
    //: Body constructor. 
    
    MarkupInfoBodyC& Body()
    { return static_cast<MarkupInfoBodyC &>(RCHandleC<MarkupInfoBodyC>::Body()); }
    //: Body Access. 
    
    const MarkupInfoBodyC& Body() const
    { return static_cast<const MarkupInfoBodyC &>(RCHandleC<MarkupInfoBodyC>::Body()); }
    //: Body Access. 
    
  };
  
  std::ostream &operator<<(std::ostream &out,const MarkupInfoC &obj);
  //: Stream operator.
  
  BinOStreamC &operator<<(BinOStreamC &out,const MarkupInfoC &obj);
  //: Binary stream operator.
  
  std::istream &operator>>(std::istream &in,MarkupInfoC &obj);
  //: Stream operator.
  
  BinIStreamC &operator>>(BinIStreamC &in,MarkupInfoC &obj);
  //: Binary stream operator.

}

#endif
