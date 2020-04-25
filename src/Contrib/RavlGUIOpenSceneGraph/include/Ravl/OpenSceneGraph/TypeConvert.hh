// This file is part of RAVL, Recognition And Vision Library
// Copyright (C) 2011, OmniPerception Ltd.
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
/* 
 * File:   TypeConvert.hh
 * Author: charlesgalambos
 *
 * Created on April 22, 2011, 1:24 PM
 */
//! lib=RavlGUIOpenSceneGraph

#ifndef RAVLOSG_TYPECONVERT_HH
#define	RAVLOSG_TYPECONVERT_HH

#include <osg/Vec3>
#include <osg/Vec4>
#include "Ravl/Vector3d.hh"
#include "Ravl/Vector4d.hh"

namespace RavlOSGN {
  
  inline osg::Vec3 MakeVec3(const RavlN::Vector3dC &vec)
  { return osg::Vec3(vec.X(),vec.Y(),vec.Z()); }
  
  
  inline osg::Vec4 MakeVec4(const RavlN::Vector4dC &vec)
  { return osg::Vec4(vec.X(),vec.Y(),vec.Z(),vec.W()); }

  inline RavlN::Vector3dC MakeVector3d(const osg::Vec3 &vec)
  { return RavlN::Vector3dC(vec.x(),vec.y(),vec.z()); }
  
  
  inline RavlN::Vector4dC MakeVector4d(const osg::Vec4 &vec)
  { return RavlN::Vector4dC(vec.x(),vec.y(),vec.z(),vec.w()); }

}


#endif	/* TYPECONVERT_HH */

