// This file is part of RAVL, Recognition And Vision Library
// Copyright (C) 2011, OmniPerception Ltd.
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//! lib=RavlGUIOpenSceneGraph

namespace RavlOSGN
{

  bool RegisterGtkGlExtInit();
  bool LinkOpenSceneGraphWidget();
  void LinkImageByteRGBA();
  void LinkImageByteRGB();
  void LinkSphere();
  void LinkGroup();
  void LinkText();
  void LinkTransform();
  void LinkHUD();
  void LinkBox();
  void LinkModelFile();
  void LinkLayout();
  void LinkLayoutStack();
  void LinkLayoutGrid();
  void LinkCylinder();
  void LinkCone();
  void LinkCapsule();
  void LinkInfinitePlane();

  void LinkGtkGlExtInit()
  {
    RegisterGtkGlExtInit();
    LinkOpenSceneGraphWidget();
    
    LinkImageByteRGBA();
    LinkImageByteRGB();
    LinkSphere();
    LinkGroup();
    LinkText();
    LinkTransform();
    LinkHUD();
    LinkBox();
    LinkModelFile();
    LinkLayout();
    LinkLayoutStack();
    LinkLayoutGrid();
    LinkCylinder();
    LinkCone();
    LinkCapsule();
    LinkInfinitePlane();
  }
}
