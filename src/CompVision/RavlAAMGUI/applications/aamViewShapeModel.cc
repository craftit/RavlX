// This file is part of RAVL, Recognition And Vision Library
// Copyright (C) 2005, OmniPerception Ltd.
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//! rcsid="$Id$"
//! lib=RavlAAMGUI
//! file="Ravl/CompVision/ActiveAppearanceModels/aamViewShapeModel.hh"
//! docentry="Ravl.API.Images.AAM"
//! userlevel="Normal"
//! author="Jean-Yves Guillemaut"

#include "Ravl/Image/AAMShapeModel.hh"
#include "Ravl/Image/AAMAppearanceModel.hh"
#include "Ravl/IO.hh"
#include "Ravl/Option.hh"
#include "Ravl/EntryPnt.hh"
#include "Ravl/Image/AAMViewLib.hh"


using namespace RavlN;
using namespace RavlImageN;

//: Visualise shape or appearance model.
//  This programs allows to visualise interactively the effects of varying the parameters
// controling the main modes of variation of the shape or appearance model. For more information type "aamViewShapeModel -help".
int viewShapeModel(int nargs,char **argv) {
  OptionC opt(nargs,argv);
  bool appear = opt.Boolean("a",false,"Load appearance? true=yes. Load only shape if set to false.");
  StringC fileName = opt.String("f",appear ? "am.abs" : "sm.abs","Input model.");
  IntT varRange = opt.Int("vr",40,"Range to put on the slider. ");
  RealT scale = opt.Real("sc",1,"Scale image. ");
  bool verb = opt.Boolean("v",false,"Verbose mode. ");
  bool use32BitMode = opt.Boolean("32",false,"Force the use 32 bit file mode. ");
  bool use64BitMode = opt.Boolean("64",false,"Force the use 64 bit file mode. ");
  opt.Check();

  if(use32BitMode)
    SetCompatibilityMode32Bit(true);
  if(use64BitMode)
    SetCompatibilityMode32Bit(false);
  if(verb) {
    if(DefaultToCompatibilityMode32Bit()) {
      std::cout << "In 32 bit binary file mode. \n";
    } else {
      std::cout << "In 64 bit binary file mode. \n";
    }
  }


  AAMShapeModelC sm; // Shape model.
  AAMAppearanceModelC am; // appearance model.

  if(appear) {
    if(!Load(fileName,am)) {
      cerr << "Failed to load file " << fileName << "\n";
      return 1;
    }
  } else {
    if(!Load(fileName,sm)) {
      cerr << "Failed to load file " << fileName << "\n";
      return 1;
    }
    // out << sm << "\n";
  }
  AAMDisplay(sm, am, varRange, scale);

  return 0;
}


RAVL_ENTRY_POINT(viewShapeModel);
