// This file is part of RAVL, Recognition And Vision Library
// Copyright (C) 2005, OmniPerception Ltd.
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVLIMAGE_AAMVIEWLIB_HEADER
#define RAVLIMAGE_AAMVIEWLIB_HEADER 1
//! lib=RavlAAMGUI

#include "Ravl/Types.hh"

namespace RavlImageN {

  class AAMShapeModelC;
  class AAMAppearanceModelC;

  void AAMDisplay(const AAMShapeModelC &shapeModel, const AAMAppearanceModelC &appearanceModel, RavlN::IntT varRange, RavlN::RealT scale);

}

#endif /* RAVLIMAGE_AAMVIEWLIB_HEADER */
