// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2005, OmniPerception Ltd.
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//! rcsid="$Id$"
//! lib=RavlImageProc
//! file="Ravl/Image/Processing/Tools/ImagePointFeatureSet/ImagePointFeature.cc"

#include "Ravl/Image/ImagePointFeature.hh"
#include "Ravl/TypeName.hh"
#include "Ravl/DP/FileFormatXMLStream.hh"

namespace RavlImageN {

  using namespace RavlN;

  ImagePointFeatureBodyC::ImagePointFeatureBodyC() 
    : location(0,0),
      m_iID(0),
      m_iTypeID(-1),
      isVisible(true)
  {}
  
  ImagePointFeatureBodyC::ImagePointFeatureBodyC(int id,StringC description, RealT row, RealT col) 
    : location(row,col),
      m_iID(id),
      m_iTypeID(-1),
      m_strDescription(description),
      isVisible(true)
  {}

  //: Constructor
  
  ImagePointFeatureBodyC::ImagePointFeatureBodyC(IntT id, StringC description, const Point2dC &pnt)
    : location(pnt),
      m_iID(id),
      m_iTypeID(-1),
      m_strDescription(description),
      isVisible(true)
  {}    

  
  ImagePointFeatureBodyC::ImagePointFeatureBodyC(int id, IntT typeId,StringC description, RealT row, RealT col,bool visible) 
    : location(row,col),
      m_iID(id),
      m_iTypeID(typeId),
      m_strDescription(description),
      isVisible(visible)
  {}
  
  ImagePointFeatureBodyC::ImagePointFeatureBodyC(XMLIStreamC& xml) 
  {
    RCHashC<StringC,StringC> oAttribs;
    StringC strType("");
    
    // Read tags until we get a Feature
    while (xml && xml.ReadTag(strType,oAttribs) != XMLEmptyTag && strType != "Feature");
    
    // Read attributes
    m_iID = oAttribs["id"].IntValue();
    if(oAttribs.IsElm("typeid"))
      m_iTypeID = oAttribs["typeid"].IntValue();
    else
      m_iTypeID = -1;
    if(oAttribs.IsElm("visible"))
      isVisible = oAttribs["visible"].IntValue() != 0;
    else
      isVisible = true;
    m_strDescription = oAttribs["desc"];
    location[0] = oAttribs["row"].RealValue();
    location[1] = oAttribs["col"].RealValue();
    
    // Skip to the end...
    xml.SkipElement();
    
  }

  void ImagePointFeatureBodyC::Move(const Point2dC &change) {
    location += change;
  }
  
  XMLOStreamC &ImagePointFeatureBodyC::SaveXML(XMLOStreamC &xml) const {  

    // Start tag
    xml.StartTag("Feature");
  
    // Write attributes
    xml.SetAttrib("id",m_iID);
    xml.SetAttrib("typeid",m_iTypeID);
    xml.SetAttrib("desc",m_strDescription);
    xml.SetAttrib("visible",isVisible);
    xml.SetAttrib("row",location[0]);
    xml.SetAttrib("col",location[1]);
    

    // End Tag
    xml.EndTag();

    return xml;
  }

  RCBodyC & ImagePointFeatureBodyC::Copy() const
  {
    ImagePointFeatureBodyC *feat = new ImagePointFeatureBodyC();
    
    feat->location = this->location;
    feat->m_iID = this->m_iID;
    feat->m_iTypeID = this->m_iTypeID;
    feat->m_strDescription = this->m_strDescription;
    feat->isVisible = this->isVisible; 

    return *feat;
  }
  
  /////////////////////////////////////////////////////////////////////
  
  XMLIStreamC &operator>>(XMLIStreamC &xml,ImagePointFeatureC &data) {
    data = ImagePointFeatureC(xml);
    return xml;
  }
  
  XMLOStreamC &operator<<(XMLOStreamC &xml, const ImagePointFeatureC& data) {
    return data.SaveXML(xml); 
  }

  void InitImagePointFeatureFormat() {}
  
  static TypeNameC typeNameImagePointFeatureBodyC(typeid(ImagePointFeatureC),"RavlImage::ImagePointFeatureC");
  
  FileFormatXMLStreamC<ImagePointFeatureC> FileFormatXMLStream_ImagePointFeatureC;
  
}

