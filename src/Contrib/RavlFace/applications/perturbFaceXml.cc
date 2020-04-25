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
#include "Ravl/RandomGauss.hh"

using namespace RavlN;
using namespace RavlN::FaceN;

//! userlevel=User
//: Allows you to modify XML files

int modifyXml(int argc, char **argv)
{
  OptionC opt(argc, argv);
  DListC<StringC> FaceInfoDbFiles = opt.List("db", "", "input FaceInfoDb files");
  StringC OutXmlFile = opt.String("o", "out.xml", "output xml file");
  RealT fraction = opt.Real("frac", 0.05, "Amount to peturb eyes - measured by interocular distance.!");
  UIntT numberPerFace = opt.Int("n", 1, "number of samples per face");

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
  RandomGaussC rand;
  FaceInfoDbC newDb;
  for (HashIterC<StringC, FaceInfoC> it(faceInfoDb); it; it++) {

    for (UIntT i = 0; i < numberPerFace; i++) {
      Point2dC leftEye = it.Data().FeatureSet().Position("LeftEyeCentre");
      Point2dC rightEye = it.Data().FeatureSet().Position("RightEyeCentre");
      RealT dist = leftEye.EuclidDistance(rightEye);

      Point2dC newLeftEye(leftEye.Row() + rand.Generate() * fraction * dist, leftEye.Col() + rand.Generate() * fraction * dist);
      Point2dC newRightEye(rightEye.Row() + rand.Generate() * fraction * dist, rightEye.Col() + rand.Generate() * fraction * dist);

      StringC newFaceId;
      newFaceId.form("%s_perturb_%03d", it.Data().FaceId().data(), i);
      FaceInfoC newFace(it.Data().ActualId(), newFaceId, it.Data().OrigImage());
      ImagePointFeatureSetC featureSet(true);
      ImagePointFeatureC le(0, "LeftEyeCentre", newLeftEye);
      ImagePointFeatureC re(1, "RightEyeCentre", newRightEye);
      featureSet.AddFeature(le);
      featureSet.AddFeature(re);
      newFace.FeatureSet() = featureSet;
      newDb.Insert(newFaceId, newFace);
    }

  }

  rInfo("Saving XML file to '%s'", OutXmlFile.data());
  if (!Save(OutXmlFile, newDb)) {
    rError("Trouble saving XML file '%s'", OutXmlFile.data());
    return 1;
  }

  return 0;
}

//: This puts a wrapper around the main program that catches
//: exceptions and turns them into readable error messages.

RAVL_ENTRY_POINT(modifyXml);

