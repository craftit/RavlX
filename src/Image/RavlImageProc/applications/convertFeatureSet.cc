// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2005, OmniPerception Ltd.
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//! rcsid="$Id$"
//! lib=RavlImageProc
//! file="Ravl/Image/Processing/Tools/ImagePointFeatureSet/convertFeatureSet.cc"

#include "Ravl/Option.hh"
#include "Ravl/OS/Filename.hh"
#include "Ravl/OS/Directory.hh"
#include "Ravl/EntryPnt.hh"
#include "Ravl/Stream.hh"
#include "Ravl/IO.hh"
#include "Ravl/CDLIter.hh"

#include "Ravl/Image/ImagePointFeatureSet.hh"

using namespace RavlImageN;

int convertFeatureSet(int argc, char **argv)
{  
  OptionC   opt(argc,argv);

  //: The data for xm2vts data
  DirectoryC    InPos = opt.String("pos", "", "directory of where .pos file live");
  DirectoryC    InImage = opt.String("image", "/vol/vssp/eyes/xm2vts", "directory of where the images are");
  bool banca = opt.Boolean("banca", false, "data is stored in banca format");

  //: Additional stuff
  DirectoryC   OutputDir = opt.String("dir", "", "directory of where to put all the feature set files");
  opt.Compulsory("dir");
  opt.Check();

  //: Get list of files
  ////////////////////
  StringC filt = "*.pos";
  DListC<StringC>posFiles = InPos.FiltList(filt);

  //: If Xm2vts 
  ////////////////////////////////////////////////////
  if(!banca) {
    RealT rr, rc, lr, lc;
    
    for(ConstDLIterC<StringC>It(posFiles);It.IsElm();It.Next()) {
      cout << "Loading in file: " << It.Data() << endl;
      FilenameC file(InPos + (StringC)"/" + It.Data());
      FilenameC name = file.BaseNameComponent();
      FilenameC outName = OutputDir + "/" + name + ".xml";
      
      //: Read in the data in .pos format
      IStreamC ifs(file);
      ifs >> rc >> rr >> lc >> lr;

      //: Output to a FeatureSet
      ImagePointFeatureSetC set(true);
      ImagePointFeatureC right(0, (StringC)"RightEyeCentre", rr, rc);
      ImagePointFeatureC left(1, (StringC)"LeftEyeCentre", lr, lc);
      set.AddFeature(right);
      set.AddFeature(left);
      set.ImageFile() = InImage + (StringC)"/" + name + ".ppm";
      Save(outName,set);
    }
  }
  
  else {   
    RealT RightEyeCornerOuterR, RightEyeCornerOuterC;
    RealT RightEyeCentreR, RightEyeCentreC;
    RealT RightEyeCornerInnerR, RightEyeCornerInnerC;
   
    RealT LeftEyeCornerOuterR, LeftEyeCornerOuterC;
    RealT LeftEyeCentreR, LeftEyeCentreC;
    RealT LeftEyeCornerInnerR, LeftEyeCornerInnerC;

    RealT RightNostrilR, RightNostrilC;
    RealT LeftNostrilR, LeftNostrilC;

    RealT RightMouthCornerR, RightMouthCornerC;
    RealT LeftMouthCornerR, LeftMouthCornerC;

    for(ConstDLIterC<StringC>It(posFiles);It.IsElm();It.Next()) {
      cout << "Loading in file: " << It.Data() << endl;
      FilenameC file(InPos + (StringC)"/" + It.Data());
      FilenameC name = file.BaseNameComponent();
      FilenameC outName = OutputDir + "/" + name + ".xml";
      
      //: Read in the data in .pos format
      IStreamC ifs(file);
      // right eye
      ifs >> RightEyeCornerOuterC >> RightEyeCornerOuterR >> RightEyeCentreC >> RightEyeCentreR >> RightEyeCornerInnerC >> RightEyeCornerInnerR;
      
      // left eye
      ifs >> LeftEyeCornerInnerC >> LeftEyeCornerInnerR >>  LeftEyeCentreC >> LeftEyeCentreR >>  LeftEyeCornerOuterC >> LeftEyeCornerOuterR;

      // nostril
      ifs >> RightNostrilC >> RightNostrilR >> LeftNostrilC >> LeftNostrilR;

      // mouth
      ifs >> RightMouthCornerC >> RightMouthCornerR >> LeftMouthCornerC >> LeftMouthCornerR;

      //: Output to a FeatureSet
      ImagePointFeatureSetC set(true);

      // right eye
      ImagePointFeatureC RightEyeCornerOuter(0, "RightEyeCornerOuter", RightEyeCornerOuterR, RightEyeCornerOuterC);
      ImagePointFeatureC RightEyeCentre(1, "RightEyeCentre", RightEyeCentreR, RightEyeCentreC);
      ImagePointFeatureC RightEyeCornerInner(2, "RightEyeCornerInner", RightEyeCornerInnerR, RightEyeCornerInnerC);
      // left eye
      ImagePointFeatureC LeftEyeCornerInner(3, "LeftEyeCornerInner", LeftEyeCornerInnerR, LeftEyeCornerInnerC);
      ImagePointFeatureC LeftEyeCentre(4, "LeftEyeCentre", LeftEyeCentreR, LeftEyeCentreC);
      ImagePointFeatureC LeftEyeCornerOuter(5, "LeftEyeCornerOuter", LeftEyeCornerOuterR, LeftEyeCornerOuterC);
      
      // nostril
      ImagePointFeatureC RightNostril(6, "RightNostril", RightNostrilR, RightNostrilC);
      ImagePointFeatureC LeftNostril(7, "LeftNostril", LeftNostrilR, LeftNostrilC);
      
      // mouth
      ImagePointFeatureC RightMouthCorner(8, "RightMouthCorner", RightMouthCornerR, RightMouthCornerC);  
      ImagePointFeatureC LeftMouthCorner(9, "LeftMouthCorner", LeftMouthCornerR, LeftMouthCornerC);  
      
      // add the features to the set
      set.AddFeature(RightEyeCornerOuter);
      set.AddFeature(RightEyeCentre);
      set.AddFeature(RightEyeCornerInner);
      set.AddFeature(LeftEyeCornerInner);
      set.AddFeature(LeftEyeCentre);
      set.AddFeature(LeftEyeCornerOuter);
      set.AddFeature(RightNostril);
      set.AddFeature(LeftNostril);
      set.AddFeature(RightMouthCorner);
      set.AddFeature(LeftMouthCorner);

      set.ImageFile() = InImage + (StringC)"/" + name + ".ppm";
      Save(outName, set);
      Save(outName,set);
    }
  }
  

 
  return 0;
}

//: This puts a wrapper around the main program that catches
//: exceptions and turns them into readable error messages.


RAVL_ENTRY_POINT(convertFeatureSet);
