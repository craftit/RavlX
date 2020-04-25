// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2005, OmniPerception Ltd.
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//! rcsid="$Id$"
//! lib=RavlImageProc
//! file="Ravl/Image/Processing/Tools/ImagePointFeatureSet/FeatureIO.cc"

namespace RavlImageN {

  extern void InitImagePointFeatureSetFormat();
  extern void InitImagePointFeatureFormat();

  void InitImagePointFeatureIO() {
    InitImagePointFeatureSetFormat();
    InitImagePointFeatureFormat();
  }
  
}
