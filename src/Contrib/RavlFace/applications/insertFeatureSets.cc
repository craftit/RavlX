// Copyright (C) 2003, Omniperception Ltd.
// file-header-ends-here
// Copyright (C) 2002, Omniperception Ltd.
//! rcsid="$Id$"
//! docentry="Ravl.API.Applications.Image.Face"
//! lib=RavlFace
//! file="Ravl/Contrib/Face/Xml/modifyXml.cc"

#include "Ravl/Option.hh"
#include "Ravl/OS/Filename.hh"
#include "Ravl/EntryPnt.hh"
#include "Ravl/Stream.hh"
#include "Ravl/IO.hh"
#include "Ravl/Face/FaceInfoDb.hh"
#include "Ravl/Face/EnrolSession.hh"
#include "Ravl/Face/ClaimSession.hh"
#include "Ravl/StringList.hh"

using namespace RavlN::FaceN;

//! userlevel=Normal
//: Inserts FeatureSets into an xml file

int main(int argc, char **argv)
{
  OptionC opt(argc, argv);
  DListC < StringC > FaceInfoDbFiles = opt.List("dbs", "input FaceInfoDb files");
  StringC Path = opt.String("path", ".", "path to feature set files");
  //: check options selection
  opt.Compulsory("dbs");
  opt.Check();
  
  for (DLIterC < StringC > it(FaceInfoDbFiles); it; it++) {
    FaceInfoDbC faceInfoDb;
    //: Load in the data to do the enrollment
    cout << "Loading enrol session file: " << *it << endl;
    if (!Load(*it, faceInfoDb))
      RavlIssueError("Problems loading the FaceInfoDb file:" + *it);

    for (HashIterC < StringC, FaceInfoC > faceIt(faceInfoDb); faceIt; faceIt++) {
      FilenameC origImage = faceIt->OrigImage();
      FilenameC fsetFile = Path + "/" + origImage.NameComponent() + ".xml";
      if (fsetFile.Exists()) {
        ImagePointFeatureSetC fset;
        if (!Load(fsetFile, fset))
          RavlIssueError("trouble loading feature set file");
        faceIt->FeatureSet() = fset;
      }
    }
    
    if (!Save(*it, faceInfoDb))
      RavlIssueError("Problems saving the FaceInfoDb file:" + *it);
    
  }
  
  return 0;
}

