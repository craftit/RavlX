// Copyright (C) 2003, Omniperception Ltd.
// file-header-ends-here
// Copyright (C) 2002, Omniperception Ltd.
//! rcsid="$Id$"
//! docentry="Ravl.API.Applications.Image.Face"
//! lib=RavlFace
//! file="Ravl/Contrib/Face/Xml/modifyFaceXml.cc"

#include "Ravl/Option.hh"
#include "Ravl/OS/Filename.hh"
#include "Ravl/EntryPnt.hh"
#include "Ravl/Stream.hh"
#include "Ravl/IO.hh"
#include "Ravl/Face/FaceInfoDb.hh"
#include "Ravl/StringList.hh"
#include "Ravl/Text/TextFile.hh"
#include "Ravl/Collection.hh"
#include "Ravl/CollectionIter.hh"
#include "Ravl/RLog.hh"

using namespace RavlN;
using namespace RavlN::FaceN;

//! userlevel=User
//: Allows you to modify XML files

int modifyXml(int argc, char **argv)
{
  OptionC opt(argc, argv);
  DListC<StringC> FaceInfoDbFiles = opt.List("db", "", "input FaceInfoDb files");
  StringC OrigPathFrom = opt.String("from", "", "change the path of the original images to");
  StringC OrigPathTo = opt.String("to", "", "change the path of the original images to");
  StringC Concatenate = opt.String("cat", "", "concatenate all files into this one");
  RealT Pick = opt.Real("pick", 0, "Pick random faces and save to file!  Save remainder.");
  StringC tag = opt.String("tag", "", "Tag to search on (e.g. faceId)");
  StringC value = opt.String("value", "", "Value to search tag on");
  bool exactMatch = opt.Boolean("exactMatch", false, "Do we want an exact match");
  StringListC SubjectList = opt.List("keepSubjects", "just keep these subjects");
  StringC Pose = opt.String("pose", "", "just keep this pose");
  StringC UpdatePose = opt.String("updatePose", "YPR 0 0 *", "update all the poses to this one");
  StringListC DeleteSubjectList = opt.List("deleteSubjects", "delete these subjects");
  StringListC DeleteFaceIds = opt.List("deleteFaces", "delete these face ids's");
  StringC OutXmlFile = opt.String("o", "out.xml", "output xml file");
  bool CheckEyes = opt.Boolean("eyes", false, "check eye positions are the right way round");
  RealT minEyeDistance = opt.Real("minEyeDistance", 0.0, "Only keep faces with this minimum eye distance!");

  // RLog options
  bool verbose = opt.Boolean("v", false, "Verbose mode. ");
  StringC logFile = opt.String("l", "stderr", "Checkpoint log file. ");
  StringC logLevel = opt.String("ll", "debug", "Logging level (debug, info, warning, error)");

  //: check options selection
  opt.Check();

  // Set up logging
  RavlN::RLogInit(argc, argv, logFile, verbose);
  RavlN::RLogSubscribeL(logLevel.chars());

  // Load in all the XML files into one master one!
  FaceInfoDbC faceInfoDb(FaceInfoDbFiles);

  if (opt.IsOnCommandLine("tag")) {
    faceInfoDb = faceInfoDb.Search(tag, value, exactMatch);
  }

  // Modify an original image path
  if (opt.IsOnCommandLine("from")) {
    rInfo("Modifying image path from '%s' to '%s'", OrigPathFrom.data(), OrigPathTo.data());
    faceInfoDb.ModifyImagePath(OrigPathFrom, OrigPathTo);
  }

  // Select faces based on pose
  if (opt.IsOnCommandLine("pose")) {
    FaceInfoDbC poseDb;
    for (HashIterC<StringC, DListC<FaceInfoC> > it(faceInfoDb.Sort(true)); it; it++) {
      for (DLIterC<FaceInfoC> faceIt(it.Data()); faceIt; faceIt++) {
        if (Pose == faceIt.Data().Pose()) {
          poseDb.Insert(faceIt.Data().FaceId(), *faceIt);
        }
      }
    }
    faceInfoDb = poseDb;
  }

  // Select faces based on pose
  if (opt.IsOnCommandLine("updatePose")) {
    FaceInfoDbC poseDb;
    rInfo("Updating pose to '%s'", UpdatePose.data());
    for (HashIterC<StringC, DListC<FaceInfoC> > it(faceInfoDb.Sort(true)); it; it++) {
      for (DLIterC<FaceInfoC> faceIt(it.Data()); faceIt; faceIt++) {
        faceIt.Data().Pose() = UpdatePose;
        poseDb.Insert(faceIt.Data().FaceId(), *faceIt);
      }
    }
    faceInfoDb = poseDb;
  }

  if (opt.IsOnCommandLine("keepSubjects")) {
    FaceInfoDbC keepDb;
    for (HashIterC<StringC, DListC<FaceInfoC> > it(faceInfoDb.Sort(true)); it; it++) {
      for (DLIterC<StringC> subIt(SubjectList); subIt; subIt++) {
        if (it.Key() == *subIt) {
          rInfo("Keeping subject '%s'", it.Key().data());
          for (DLIterC<FaceInfoC> faceIt(it.Data()); faceIt; faceIt++) {
            keepDb.Insert(faceIt.Data().FaceId(), *faceIt);
          }
        }
      }
    }
    faceInfoDb = keepDb;
  }

  if (opt.IsOnCommandLine("deleteSubjects")) {
    FaceInfoDbC excludeDb;
    for (HashIterC<StringC, DListC<FaceInfoC> > it(faceInfoDb.Sort(true)); it; it++) {

      // check whether it is an excluded subject
      bool excluded = false;
      for (DLIterC<StringC> subIt(DeleteSubjectList); subIt; subIt++) {
        if (it.Key() == *subIt) {
          rInfo("Deleting subject '%s'", it.Key().data());
          excluded = true;
          break;
        }
      }

      // if it isnt excluded then add it to the new XML
      if (!excluded) {
        for (DLIterC<FaceInfoC> faceIt(it.Data()); faceIt; faceIt++)
          excludeDb.Insert(faceIt.Data().FaceId(), *faceIt);
      }
    }
    faceInfoDb = excludeDb;
  }

  // Delete faces
  if (opt.IsOnCommandLine("deleteFaces")) {
    for (HashIterC<StringC, FaceInfoC> faceIt(faceInfoDb); faceIt; faceIt++) {
      for (DLIterC<StringC> deleteIt(DeleteFaceIds); deleteIt; deleteIt++) {
        if (*deleteIt == faceIt.Key()) {
          rInfo("Deleting %s", faceIt.Key().data());
          faceIt.Del();
        }
      }
    }
  }

  // Only keep faces above a minimum eye distance
  if (opt.IsOnCommandLine("minEyeDistance")) {
    FaceInfoDbC newDb;
    for (HashIterC<StringC, FaceInfoC> faceIt(faceInfoDb); faceIt; faceIt++) {
      RealT dist = faceIt.Data().FeatureSet().Position("LeftEyeCentre").EuclidDistance(faceIt.Data().FeatureSet().Position("RightEyeCentre"));
      if (dist > minEyeDistance) {
        rInfo("Keeping face %s with dist %0.2f", faceIt.Key().data(), dist);
        newDb.Insert(faceIt.Key(), faceIt.Data());
      } else {
        rInfo("Deleting face %s with dist %0.2f", faceIt.Key().data(), dist);
      }
    }
    faceInfoDb = newDb;
  }

  //: Do we just want to pick some random faces
  if (opt.IsOnCommandLine("pick")) {
    UIntT p = Round(Pick);
    if(Pick < 1.0) {
      p = Round(Pick * (RealT)faceInfoDb.Size());
    }
    FaceInfoDbC master;
    CollectionC<FaceInfoC> collection(faceInfoDb.Size());
    for (HashIterC<StringC, FaceInfoC> faceIt(faceInfoDb); faceIt; faceIt++)
      collection.Insert(*faceIt);
    if ((UIntT) faceInfoDb.Size() < p)
      p = faceInfoDb.Size();
    for (UIntT i = 0; i < p; i++) {
      FaceInfoC faceInfo = collection.Pick();
      master.Insert(faceInfo.FaceId(), faceInfo);
    }
    // Lets save remainder as well
    FaceInfoDbC remainder;
    for(CollectionIterC<FaceInfoC>it(collection);it;it++) {
      remainder.Insert(it.Data().FaceId(), *it);
    }
    rInfo("Saving remainder of pick operation 'remainder.xml'");
    Save("remainder.xml", remainder);
    // Set master set, this will get saved later
    faceInfoDb = master;
  }

//: It is easy to get left and right eyes muddled up
//: This routin goes through and checks for this and modifies xml if it finds examples
  if (CheckEyes) {
    rInfo("Checking eye positions!");
    for (HashIterC<StringC, FaceInfoC> faceIt(faceInfoDb); faceIt; faceIt++) {
      ImagePointFeatureSetC fset = faceIt.Data().FeatureSet();
      if (fset.IsValid()) {
        Point2dC leftEye = fset.Position("LeftEyeCentre");
        Point2dC rightEye = fset.Position("RightEyeCentre");
        if (leftEye.Col() < rightEye.Col()) {
          rInfo("Eyes round wrong way, fixing '%s'", faceIt.Data().FaceId().data());
          fset.Set("LeftEyeCentre", rightEye);
          fset.Set("RightEyeCentre", leftEye);
        }
      }
    }
  } // end CheckEyes

  rInfo("Saving XML file to '%s'", OutXmlFile.data());
  if (!Save(OutXmlFile, faceInfoDb)) {
    rError("Trouble saving XML file '%s'", OutXmlFile.data());
    return 1;
  }

  return 0;
}

//: This puts a wrapper around the main program that catches
//: exceptions and turns them into readable error messages.

RAVL_ENTRY_POINT(modifyXml);

