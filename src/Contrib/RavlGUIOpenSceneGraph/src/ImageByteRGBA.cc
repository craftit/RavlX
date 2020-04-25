// This file is part of RAVL, Recognition And Vision Library
// Copyright (C) 2010, OmniPerception Ltd.
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
////////////////////////////////////////////////////////////////
//! file = "Ravl/Contrib/OpenSceneGraph/ImageByteRGBA.cc"
//! lib=RavlGUIOpenSceneGraph
//! author = "Warren Moore"

#include "Ravl/OpenSceneGraph/ImageByteRGBA.hh"
#include "Ravl/XMLFactoryRegister.hh"
#include "Ravl/IO.hh"
#include "Ravl/RLog.hh"
#include <osg/PrimitiveSet>
#include <osg/Geode>
#include <osg/Texture2D>

#define DODEBUG 0
#if DODEBUG
#define ONDEBUG(x) x
#else
#define ONDEBUG(x)
#endif

namespace RavlOSGN
{

  using namespace osg;
  
  ImageByteRGBAC::ImageByteRGBAC(const RavlN::RealRange2dC &coords)
  : ImageC(coords)
  {
  }

  //: Factory constructor
  
  ImageByteRGBAC::ImageByteRGBAC(const XMLFactoryContextC &context)
   : ImageC(context)
  {
    RavlN::StringC filename = context.AttributeString("filename","");
    if(!filename.IsEmpty()) {
      RavlImageN::ImageC<RavlImageN::ByteRGBAValueC> image;
      rDebug("Loading image '%s' ",filename.data());
      if(RavlN::Load(filename,image)) {
        if(!SetImage(image)) {
          rError("Failed to setup image. %s ",context.Path().data());
          throw RavlN::ExceptionBadConfigC("Failed to setup image.");
        }
      } else {
        rWarning("Failed to load file '%s' ",filename.data());
        throw RavlN::ExceptionBadConfigC("Failed to load image.");
      }
    }
  }

  ImageByteRGBAC::~ImageByteRGBAC()
  {
  }


  bool ImageByteRGBAC::SetImage(RavlImageN::ImageC<RavlImageN::ByteRGBAValueC> &image)
  {
    if (!(image.IsValid() && (image.Size() > 0) && image.IsContinuous()))
      return false;

    if (!m_node)
      return false;
    
    ref_ptr<Geode> geodeRef = dynamic_cast<Geode*>(m_node.get());
    if (!geodeRef)
      return false;

    ref_ptr<Texture2D> textureRef = new Texture2D();
    textureRef->setDataVariance(Object::DYNAMIC);

    m_image = image;
    int width = image.Cols().V();
    int height = image.Rows().V();
    RavlN::BufferAccessC<RavlImageN::ByteRGBAValueC> rowAccess = image.RowPtr(0);
    unsigned char* imageData = reinterpret_cast<unsigned char*>(rowAccess.ReferenceVoid());
    if (imageData == NULL)
      return false;
    ref_ptr<Image> imageRef = new Image();
    imageRef->setImage(width, height, 1, GL_RGBA, GL_RGBA, GL_UNSIGNED_BYTE, (unsigned char*)imageData, Image::NO_DELETE, 1);

    textureRef->setImage(imageRef);
    
    ref_ptr<StateSet> stateSetRef = geodeRef->getOrCreateStateSet();
    stateSetRef->setTextureAttributeAndModes(0, textureRef, StateAttribute::ON);

    if (!UpdateAlphaRenderState(stateSetRef))
      return false;

    geodeRef->setStateSet(stateSetRef);

    return true;
  }
  
  //: Zero owners reached.
  void ImageByteRGBAC::ZeroOwners() 
  {
    ImageC::ZeroOwners();
  }


  void LinkImageByteRGBA()
  {}
  
  static RavlN::XMLFactoryRegisterConvertC<ImageByteRGBAC,ImageC> g_registerXMLFactoryImageRGBA("RavlOSGN::ImageByteRGBAC");

}
