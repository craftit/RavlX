// This file is part of RAVL, Recognition And Vision Library
// Copyright (C) 2010, OmniPerception Ltd.
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
////////////////////////////////////////////////////////////////
//! file = "Ravl/Contrib/OpenSceneGraph/ImageByteRGBA.hh"
//! lib=RavlGUIOpenSceneGraph
//! author = "Warren Moore"
//! docentry = "Ravl.API.Graphics.OpenSceneGraph"
//! example = "exOpenSceneGraphWidget.cc"

#ifndef RAVLGUI_OPENSCENEGRAPHIMAGEBYTERGBA_HEADER
#define RAVLGUI_OPENSCENEGRAPHIMAGEBYTERGBA_HEADER 1

#include "Ravl/OpenSceneGraph/Image.hh"
#include "Ravl/Image/ByteRGBAValue.hh"

namespace RavlOSGN
{

  //! userlevel=Normal
  //: Node object containing a ByteRGBAValueC image.

  class ImageByteRGBAC
  : public ImageC
  {
  public:
    ImageByteRGBAC(const XMLFactoryContextC &context);
    //: Factory constructor
    
    ImageByteRGBAC(const RavlN::RealRange2dC &coords = RavlN::RealRange2dC(1, 1));
    //: Ctor.
    //!param: coords - The 2D position of the image in the X/Y plane.
    
    virtual ~ImageByteRGBAC();
    //: Dtor.

    bool SetImage(RavlImageN::ImageC<RavlImageN::ByteRGBAValueC> &image);
    //: Set the object image.

    typedef RavlN::SmartOwnerPtrC<ImageByteRGBAC> RefT;
    //: Reference type.
    
    typedef RavlN::SmartCallbackPtrC<ImageByteRGBAC> CBRefT;
    //: Reference type.

  protected:
    virtual void ZeroOwners();
    //: Zero owners reached.

    RavlImageN::ImageC<RavlImageN::ByteRGBAValueC> m_image;
  };

}

#endif
