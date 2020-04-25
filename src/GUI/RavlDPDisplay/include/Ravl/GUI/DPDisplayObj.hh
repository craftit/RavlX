// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVLGUI_DPDISPLAYOBJ_HEADER
#define RAVLGUI_DPDISPLAYOBJ_HEADER 1
/////////////////////////////////////////////////////////////////////
//! rcsid="$Id$"
//! lib=RavlDPDisplay
//! file="Ravl/GUI/DPDisplay/DPDisplayObj.hh"
//! author="Charles Galambos"
//! docentry="Ravl.API.Graphics.Data Display"

#include "Ravl/RCHandleV.hh"
#include "Ravl/IndexRange2d.hh"
#include "Ravl/Vector2d.hh"
#include "Ravl/String.hh"
#include "Ravl/DList.hh"
#include "Ravl/GUI/FrameMarkup.hh"

namespace RavlGUIN {
  using namespace RavlN;
  
  class DPDisplayViewBodyC;
  
  //! userlevel=Develop
  //: Display object body
  
  class DPDisplayObjBodyC 
    : public RCBodyVC
  {
  public:
    DPDisplayObjBodyC();
    //: Default constructor.
    
    void SetLayerNo(IntT layerNo)
    { m_layerNo = layerNo; }
    //: Set the layer id for this object
    
    IntT LayerNo() const
    { return m_layerNo; }
    //: Access the layer number of this object.
    
    virtual bool Draw(FrameMarkupC &markup);
    //: Method to build markup list.
    
    virtual IndexRange2dC Frame() const;
    //: Find bounding box for object.
    
    virtual bool Query(const Vector2dC &pnt,StringC &text);
    //: Query a point in the display.
    // Return true if point is within object.
    
    using RCBodyVC::Save;

    virtual bool Save(const StringC &str) const;
    //: Save to a file.
    
  protected:
    Int64T m_id;
    IntT m_layerNo;
  };
  
  //! userlevel=Normal
  //: Display object.
  
  class DPDisplayObjC 
    : public RCHandleC<DPDisplayObjBodyC>
  {
  public:
    DPDisplayObjC()
    {}
    //: Default constructor.
    // Creates invalid handle.
    
  protected:
    DPDisplayObjC(DPDisplayObjBodyC &body)
      : RCHandleC<DPDisplayObjBodyC>(body)
    {}
    //: Body constructor.
    
    DPDisplayObjC(const DPDisplayObjBodyC *body)
      : RCHandleC<DPDisplayObjBodyC>(body)
    {}
    //: Body constructor.
    
    DPDisplayObjBodyC &Body()
    { return RCHandleC<DPDisplayObjBodyC>::Body(); }
    //: Access body.

    const DPDisplayObjBodyC &Body() const
    { return RCHandleC<DPDisplayObjBodyC>::Body(); }
    //: Access body.
  public:
    
    void SetLayerNo(IntT layerNo)
    { Body().SetLayerNo(layerNo); }
    //: Set the layer id for this object
    
    IntT LayerNo() const
    { return Body().LayerNo(); }
    //: Access the layer number of this object.
    
    bool Draw(FrameMarkupC &markup)
    { return Body().Draw(markup); }
    //: Draw object to canvas.
    
    IndexRange2dC Frame() const
    { return Body().Frame(); }
    //: Find bounding box for object.
    
    bool Query(const Vector2dC &pnt,StringC &text)
    { return Body().Query(pnt,text); }
    //: Query a point in the display.
    // Return true if point is within object.
    
    bool Save(const StringC &str) const
    { return Body().Save(str); }
    //: Save to a file.
    
  };
  
  BinOStreamC &operator<<(BinOStreamC &strm,const DPDisplayObjC  &obj);
  //: Write to binary stream.
  // NOT IMPLEMENTED.
  
  BinIStreamC &operator>>(BinIStreamC &strm,DPDisplayObjC  &obj);
  //: Read from binary stream.
  // NOT IMPLEMENTED.

  std::ostream &operator<<(std::ostream &strm,const DPDisplayObjC  &obj);
  //: Write to stream.
  // NOT IMPLEMENTED.
  
  std::istream &operator>>(std::istream &strm,DPDisplayObjC  &obj);
  //: Read from stream.
  // NOT IMPLEMENTED.

}


#endif
