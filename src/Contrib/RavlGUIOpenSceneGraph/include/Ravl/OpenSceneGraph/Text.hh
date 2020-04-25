// This file is part of RAVL, Recognition And Vision Library
// Copyright (C) 2010, OmniPerception Ltd.
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
////////////////////////////////////////////////////////////////
//! file = "Ravl/Contrib/OpenSceneGraph/Text.hh"
//! lib=RavlGUIOpenSceneGraph
//! author = "Warren Moore"
//! docentry = "Ravl.API.Graphics.OpenSceneGraph"
//! example = "exOpenSceneGraphWidget.cc"

#ifndef RAVLGUI_OPENSCENEGRAPHTEXT_HEADER
#define RAVLGUI_OPENSCENEGRAPHTEXT_HEADER 1

#include "Ravl/OpenSceneGraph/Drawable.hh"
#include "Ravl/Image/RealRGBAValue.hh"
#include "Ravl/Vector3d.hh"
#include <string>

namespace RavlOSGN
{

  enum TextAlignmentT
  {
    TextAlignLeftTop = 0,
    TextAlignLeftCentre,
    TextAlignLeftBottom,
    TextAlignCentreTop,
    TextAlignCentreCentre,
    TextAlignCentreBottom,
    TextAlignRightTop,
    TextAlignRightCentre,
    TextAlignRightBottom,
    TextAlignLeftBaseLine,
    TextAlignCentreBaseLine,
    TextAlignRightBaseLine,
    TextAlignLeftBottomBaseLine,
    TextAlignCentreBottomBaseLine,
    TextAlignRightBottomBaseLine
  };

  std::istream &operator>>(std::istream &,TextAlignmentT &textAlignment);
  std::ostream &operator<<(std::ostream &,TextAlignmentT textAlignment);
  
  enum TextAxisAlignmentT
  {
    TextAxisXYPlane = 0,
    TextAxisXYPlaneReversed,
    TextAxisXZPlane,
    TextAxisXZPlaneReversed,
    TextAxisYZPlane,
    TextAxisYZPlaneReversed,
    TextAxisScreen
  };

  std::istream &operator>>(std::istream &,TextAxisAlignmentT &textAxisAlignment);
  std::ostream &operator<<(std::ostream &,TextAxisAlignmentT textAxisAlignment);

  //! userlevel=Normal
  //: Text object.

  class TextC
  : public DrawableC
  {
  public:
    TextC(const std::string &text);
    //: Ctor.
    //!param: text - Initial text value.
    
    TextC(const XMLFactoryContextC &context);
    //: Factory
    
    virtual ~TextC();
    //: Dtor.

    bool SetText(const std::string &text);
    //: Set the text.

    bool SetFont(const std::string &filename);
    //: Set the font filename.

    bool SetAlignment(TextAlignmentT alignment);
    //: Set the text alignment from the position coordinates.

    bool SetAxisAlignment(TextAxisAlignmentT axis);
    //: Set the alignment to the axes (or screen of you want billboard alignment).
    
    virtual bool SetColour(const RavlImageN::RealRGBAValueC &colour);
    //: Set the text colour.

    bool SetPosition(const RavlN::Vector3dC &position);
    //: Set the position.

    bool SetSize(RavlN::RealT size);
    //: Set the text size in scene graph coordinates.

    typedef RavlN::SmartOwnerPtrC<TextC> RefT;
    typedef RavlN::SmartCallbackPtrC<TextC> CBRefT;
    
  protected:
    virtual void ZeroOwners();
    //: Called when owner handles drop to zero.

  };

}

#endif
