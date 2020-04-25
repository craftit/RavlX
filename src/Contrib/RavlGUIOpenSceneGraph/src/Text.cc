// This file is part of RAVL, Recognition And Vision Library
// Copyright (C) 2010, OmniPerception Ltd.
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
////////////////////////////////////////////////////////////////
//! file = "Ravl/Contrib/OpenSceneGraph/Text.cc"
//! lib=RavlGUIOpenSceneGraph
//! author = "Warren Moore"

#include "Ravl/OpenSceneGraph/Text.hh"
#include "Ravl/XMLFactoryRegister.hh"
#include "Ravl/RLog.hh"
#include <osgText/Text>

#define DODEBUG 0
#if DODEBUG
#define ONDEBUG(x) x
#else
#define ONDEBUG(x)
#endif

namespace
{
  
  osgText::Text::AlignmentType g_lookupTextAlignment[] =
  {
    osgText::TextBase::LEFT_TOP,
    osgText::TextBase::LEFT_CENTER,
    osgText::TextBase::LEFT_BOTTOM,
    osgText::TextBase::CENTER_TOP,
    osgText::TextBase::CENTER_CENTER,
    osgText::TextBase::CENTER_BOTTOM,
    osgText::TextBase::RIGHT_TOP,
    osgText::TextBase::RIGHT_CENTER,
    osgText::TextBase::RIGHT_BOTTOM,
    osgText::TextBase::LEFT_BASE_LINE,
    osgText::TextBase::CENTER_BASE_LINE,
    osgText::TextBase::RIGHT_BASE_LINE,
    osgText::TextBase::LEFT_BOTTOM_BASE_LINE,
    osgText::TextBase::CENTER_BOTTOM_BASE_LINE,
    osgText::TextBase::RIGHT_BOTTOM_BASE_LINE
  };
  
  const char * g_lookupTextAlignmentStr[] = 
  {
    "LeftTop",
    "LeftCentre",
    "LeftBottom",
    "CentreTop",
    "CentreCentre",
    "CentreBottom",
    "RightTop",
    "RightCentre",
    "RightBottom",
    "LeftBaseLine",
    "CentreBaseLine",
    "RightBaseLine",
    "LeftBottomBaseLine",
    "CentreBottomBaseLine",
    "RightBottomBaseLine",
    0
  };
  


  osgText::Text::AxisAlignment g_lookupTextAxisAlignment[] =
  {
    osgText::TextBase::XY_PLANE,
    osgText::TextBase::REVERSED_XY_PLANE,
    osgText::TextBase::XZ_PLANE,
    osgText::TextBase::REVERSED_XZ_PLANE,
    osgText::TextBase::YZ_PLANE,
    osgText::TextBase::REVERSED_YZ_PLANE,
    osgText::TextBase::SCREEN
  };

  const char * g_lookupTextAxisAlignmentStr[] = 
  {
    "XYPlane",
    "XYPlaneReversed",
    "XZPlane",
    "XZPlaneReversed",
    "YZPlane",
    "YZPlaneReversed",
    "Screen"
  };
}

namespace RavlOSGN
{
  using namespace osg;
  using namespace osgText;

  // --------------------------------------------------------------------
  
  std::istream &operator>>(std::istream &strm,TextAlignmentT &textAlignment) {
    std::string name;
    strm >> name;
    for(unsigned i = 0;g_lookupTextAlignmentStr[i] != 0;i++) {
      if(name == g_lookupTextAlignmentStr[i]) {
        textAlignment = static_cast<TextAlignmentT>(i);
        return strm;
      }
    }
    rError("Unexpected alignment type '%s' ",name.data());
    throw RavlN::ExceptionInvalidStreamC("Unexpected alignment type.");
    return strm;
  }
  
  std::ostream &operator<<(std::ostream &strm,TextAlignmentT textAlignment) {
    strm << g_lookupTextAlignmentStr[(int) textAlignment];
    return strm;
  }

  // --------------------------------------------------------------------

  std::istream &operator>>(std::istream &strm,TextAxisAlignmentT &textAxisAlignment) {
    std::string name;
    strm >> name;
    for(unsigned i = 0;g_lookupTextAxisAlignmentStr[i] != 0;i++) {
      if(name == g_lookupTextAxisAlignmentStr[i]) {
        textAxisAlignment = static_cast<TextAxisAlignmentT>(i);
        return strm;
      }
    }
    rError("Unexpected alignment type '%s' ",name.data());
    throw RavlN::ExceptionInvalidStreamC("Unexpected alignment type.");
    return strm;
  }
  
  std::ostream &operator<<(std::ostream &strm,TextAxisAlignmentT textAxisAlignment) {
    strm << g_lookupTextAxisAlignmentStr[(int) textAxisAlignment];
    return strm;
  }
  
  // --------------------------------------------------------------------
  
  TextC::TextC(const std::string &text)
  {
    ref_ptr<Text> textRef = new Text();
    if (textRef)
      textRef->setText(text);

    m_drawable = textRef;
  }

  //: Factory
  
  TextC::TextC(const XMLFactoryContextC &context)
  {
    ref_ptr<Text> textRef = new Text();
    m_drawable = textRef;
    RavlN::StringC text = context.AttributeString("text","");
    textRef->setText(text.data());
    float size;
    if(context.Attribute("size",size)) {
      if(!SetSize(size)) {
        rError("Failed to set size to %f ",size);
        throw RavlN::ExceptionBadConfigC("Failed to set size.");
      }
    }
    
    std::string fontFilename;
    if(context.Attribute("font",fontFilename)) {
      if(!SetFont(fontFilename)) {
        rError("Failed to set font to %s ",fontFilename.data());
        throw RavlN::ExceptionBadConfigC("Failed to set font.");        
      }
    }
    
    TextAlignmentT alignment;
    if(context.Attribute("alignment",alignment)) {
      if(!SetAlignment(alignment)) {
        rError("Failed to set alignment to %s ",RavlN::StringOf(alignment).data());
        throw RavlN::ExceptionBadConfigC("Failed to set alignment.");                
      }
    }
    
    TextAxisAlignmentT axis;
    if(context.Attribute("axisAlignment",axis)) {
      if(!SetAxisAlignment(axis)) {
        rError("Failed to set axis alignment to %s ",RavlN::StringOf(axis).data());
        throw RavlN::ExceptionBadConfigC("Failed to set axis alignment.");                        
      }
    }
    
    RavlN::Vector3dC position;
    if(context.Attribute("position",position,RavlN::Vector3dC(0,0,0))) {
      if(!SetPosition(position)) {
        rError("Failed to set position to %s ",RavlN::StringOf(position).data());
        throw RavlN::ExceptionBadConfigC("Failed to set position.");                        
      }      
    }
    
    DrawableC::Setup(context);
  }
  
  TextC::~TextC()
  {
  }

  bool TextC::SetText(const std::string &text)
  {
    if (!m_drawable)
      return false;

    ref_ptr<Text> textRef = dynamic_cast<Text*>(m_drawable.get());
    if (!textRef)
      return false;

    textRef->setText(text);

    return true;
  }

  bool TextC::SetFont(const std::string &filename)
  {
    if (!m_drawable)
      return false;

    ref_ptr<Text> textRef = dynamic_cast<Text*>(m_drawable.get());
    if (!textRef)
      return false;

    textRef->setFont(filename);

    return true;
  }

  bool TextC::SetAlignment(TextAlignmentT alignment)
  {
    if (!m_drawable)
      return false;

    ref_ptr<Text> textRef = dynamic_cast<Text*>(m_drawable.get());
    if (!textRef)
      return false;

    TextBase::AlignmentType alignmentType = g_lookupTextAlignment[alignment];
    textRef->setAlignment(alignmentType);
    
    return true;
  }

  bool TextC::SetAxisAlignment(TextAxisAlignmentT axis)
  {
    if (!m_drawable)
      return false;

    ref_ptr<Text> textRef = dynamic_cast<Text*>(m_drawable.get());
    if (!textRef)
      return false;

    TextBase::AxisAlignment axisAlignment = g_lookupTextAxisAlignment[axis];
    textRef->setAxisAlignment(axisAlignment);

    return true;
  }

  bool TextC::SetColour(const RavlImageN::RealRGBAValueC &colour)
  {
    if (!m_drawable)
      return false;

    ref_ptr<Text> textRef = dynamic_cast<Text*>(m_drawable.get());
    if (!textRef)
      return false;

    textRef->setColor(Vec4(colour.Red(), colour.Green(), colour.Blue(), colour.Alpha()));

    return true;
  }

  bool TextC::SetPosition(const RavlN::Vector3dC &position)
  {
    if (!m_drawable)
      return false;

    ref_ptr<Text> textRef = dynamic_cast<Text*>(m_drawable.get());
    if (!textRef)
      return false;

    textRef->setPosition(Vec3(position.X(), position.Y(), position.Z()));

    return true;
  }

  bool TextC::SetSize(RavlN::RealT size)
  {
    if (!m_drawable)
      return false;

    ref_ptr<Text> textRef = dynamic_cast<Text*>(m_drawable.get());
    if (!textRef)
      return false;

    textRef->setCharacterSize(size);

    return true;
  }
  
  void LinkText()
  {}
  
  //: Called when owner handles drop to zero.
  
  void TextC::ZeroOwners() {
    DrawableC::ZeroOwners();
  }
  
  static RavlN::XMLFactoryRegisterConvertC<TextC,DrawableC> g_registerXMLFactorySphere("RavlOSGN::TextC");

}
