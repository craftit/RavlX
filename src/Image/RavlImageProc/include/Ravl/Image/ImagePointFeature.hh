// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2005, OmniPerception Ltd.
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVLIMAGE_IMAGEPOINTFEATURE_HEADER
#define RAVLIMAGE_IMAGEPOINTFEATURE_HEADER 1
//! rcsid="$Id$"
//! lib=RavlImageProc
//! docentry="Ravl.API.Images.Feature Points"
//! userlevel="Basic"
//! file="Ravl/Image/Processing/Tools/ImagePointFeature.hh"

#include "Ravl/XMLStream.hh"
#include "Ravl/String.hh"
#include "Ravl/Point2d.hh"
#include "Ravl/RefCounter.hh"

namespace RavlImageN {

  using namespace RavlN;
  class ImagePointFeatureC;
  
  //! userlevel=Develop
  //: Image Feature
  
  class ImagePointFeatureBodyC  
    : public RCBodyC
  {    
  public:
    ImagePointFeatureBodyC();
    //: Default constructor

    ImagePointFeatureBodyC(IntT id, StringC description, RealT row,RealT col);
    //: Constructor

    ImagePointFeatureBodyC(IntT id, StringC description, const Point2dC &pnt);
    //: Constructor
    
    ImagePointFeatureBodyC(IntT id, IntT typeId,StringC description, RealT row,RealT col,bool visible = true);
    //: Constructor
    
  protected:

    ImagePointFeatureBodyC(XMLIStreamC& xml);
    //: Constructor from XML stream

    XMLOStreamC& SaveXML(XMLOStreamC &xml) const;
    //: Save to xml stream
    
  public:
    
    RCBodyC &Copy() const;
    //: Make copy of body.
    // This should be provided in derived classes.
    // this funtion will issue an assertion failure if called.

    void Move(const Point2dC &change);
    //: Move this feature point

    IntT ID(void) const 
    { return m_iID; }
    //: Get the ID
    
    IntT TypeID() const
    { return m_iTypeID; }
    //: Get feature type.
    
    const StringC &Description(void) const
    { return m_strDescription; }
    //: Get the description string

    StringC& Description(void)
    { return m_strDescription; }
    //: Get the description string (non-const)

    bool IsVisible() const
    { return isVisible; }
    //: Is feature visible ?
    
    void SetVisible(bool vis)
    { isVisible = vis; }
    //: Set visibility flag.
    
    ImagePointFeatureBodyC &operator=(const Point2dC &pnt)
    { location = pnt; return *this; }
    //: Set position of feature.
    
    Point2dC &Location()
    { return location; }
    //: Access location of object.
    
    const Point2dC &Location() const
    { return location; }
    //: Access location of object.
    
  protected:
    Point2dC location;
    //: location of feature.
    
    IntT m_iID;
    //: ID number - should be unique in a set
    
    IntT m_iTypeID;
    //: Type id.
    
    StringC m_strDescription;
    //: Description

    bool isVisible; 
    // Is feature visible.?

    
    friend XMLIStreamC &operator>>(XMLIStreamC &xml, ImagePointFeatureC &data);
    friend XMLOStreamC &operator<<(XMLOStreamC &xml, const ImagePointFeatureC &data);
    friend class ImagePointFeatureC;
  };

  //! userlevel="Basic"
  //: Image feature.
  
  class ImagePointFeatureC  
    : public RCHandleC<ImagePointFeatureBodyC>
  {
  public:
    ImagePointFeatureC()
    {}
    //: Default constructor.
    // Creates an invalid handle.

    ImagePointFeatureC(bool)
      : RCHandleC<ImagePointFeatureBodyC>(*new ImagePointFeatureBodyC())
    {}
    //: Construct a valid object, but don't setup.
    
    ImagePointFeatureC(IntT id, StringC description, RealT row,RealT col)
      : RCHandleC<ImagePointFeatureBodyC>(*new ImagePointFeatureBodyC(id,description,row,col))
    {}
    //: Constructor
    
    ImagePointFeatureC(IntT id, StringC description, const Point2dC &pnt)
      : RCHandleC<ImagePointFeatureBodyC>(*new ImagePointFeatureBodyC(id,description,pnt))
    {}
    //: Constructor
    
    ImagePointFeatureC(IntT id, IntT typeId,StringC description, RealT row,RealT col,bool visible = true)
      : RCHandleC<ImagePointFeatureBodyC>(*new ImagePointFeatureBodyC(id,typeId,description,row,col,visible))
    {}
    //: Constructor
    
  protected:
    ImagePointFeatureC(ImagePointFeatureBodyC &bod)
      : RCHandleC<ImagePointFeatureBodyC>(bod)
    {}
    //: Access body.

    ImagePointFeatureC(XMLIStreamC& xml)
      : RCHandleC<ImagePointFeatureBodyC>(*new ImagePointFeatureBodyC(xml))
    {}
    //: Constructor from XML stream
    
    XMLOStreamC& SaveXML(XMLOStreamC &xml) const
    { return Body().SaveXML(xml); }
    //: Save to xml stream
    
  public:
    
    ImagePointFeatureC Copy() const
    { return ImagePointFeatureC(static_cast<ImagePointFeatureBodyC &>(Body().Copy())); }
    //: Make a deep copy of this object.

    Point2dC &Location()
    { return Body().Location(); }
    //: Access location of object.
    
    const Point2dC &Location() const
    { return Body().Location(); }
    //: Access location of object.
    
    void Move(const Point2dC &change)
    { Body().Move(change); }
    //: Move this feature point

    IntT ID(void) const 
    { return Body().ID(); }
    //: Get the ID
    
    IntT TypeID() const
    { return Body().TypeID(); }
    //: Get feature type.
    
    const StringC &Description(void) const
    { return Body().Description(); }
    //: Get the description string

    StringC& Description(void)
    { return Body().Description(); }
    //: Get the description string (non-const)
    
    bool IsVisible() const
    { return Body().IsVisible(); }
    //: Is feature visible ?
    
    void SetVisible(bool vis)
    { return Body().SetVisible(vis); }
    //: Set visibility flag.
    
    RealT Row() const
    { return Body().Location().Row(); }
    //: Access row of feature.
    
    RealT Col() const
    { return Body().Location().Col(); }
    //: Access row of feature.

    RealT &Row()
    { return Body().Location().Row(); }
    //: Access row of feature.
    
    RealT &Col()
    { return Body().Location().Col(); }
    //: Access row of feature.
    
    operator const Point2dC &() const
    { return Body().Location(); }
    //: Convert to a point.
    
    friend XMLIStreamC &operator>>(XMLIStreamC &xml, ImagePointFeatureC &data);
    friend XMLOStreamC &operator<<(XMLOStreamC &xml, const ImagePointFeatureC &data);
  };

  
  XMLIStreamC &operator>>(XMLIStreamC &xml, ImagePointFeatureC &data);
  //: XML Input
  // DO NOT USE DIRECTLY unless you're using this inside another XML loader and you know what you're doing.
  // Use Load() instead.

  XMLOStreamC &operator<<(XMLOStreamC &xml, const ImagePointFeatureC &data);
  //: XML Output
  // DO NOT USE DIRECTLY unless you're using this inside another XML output func and you know what you're doing.
  // Use Save() instead.

};


#endif
