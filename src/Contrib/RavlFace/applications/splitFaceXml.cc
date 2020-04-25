// Copyright (C) 2003, Omniperception Ltd.
// file-header-ends-here
//! rcsid="$Id$"
//! docentry="Ravl.API.Applications.Image.Face"
//! lib=RavlFace
//! file="Ravl/Contrib/Face/splitFaceXml.cc"

#include "Ravl/Option.hh"
#include "Ravl/OS/Filename.hh"
#include "Ravl/EntryPnt.hh"
#include "Ravl/Stream.hh"
#include "Ravl/IO.hh"
#include "Ravl/Face/FaceInfoDb.hh"
#include "Ravl/Face/EnrolSession.hh"
#include "Ravl/Face/ClaimSession.hh"
#include "Ravl/StringList.hh"
#include "Ravl/Text/TextFile.hh"
#include "Ravl/DLIter.hh"
#include "Ravl/SysLog.hh"

using namespace RavlN::FaceN;

//! userlevel=User
//: Allows you to modify XML files

int splitXml(int argc, char **argv) {
  OptionC opt(argc, argv);
  DListC<StringC> FaceInfoDbFiles = opt.List("db", "input FaceInfoDb files");
  StringC galleryXmlFile = opt.String("o1", "gallery.xml", "output gallery xml file");
  StringC probeXmlFile = opt.String("o2", "probe.xml", "output probe xml file");
  UIntT n1 = opt.Int("ng", 1, "number of faces per subject in gallery set");
  IntT n2 = opt.Int("np", -1, "number of faces per subject in probe set (-1 is do rest)");
  UIntT maxSubjects = opt.Int("subjects", 0, "Maximum number of clients");

  //: check options selection
  opt.Check();

  //: Lets modify any of the FaceInfoDb files
  //=================================
  FaceInfoDbC faceInfoDb(FaceInfoDbFiles);

  FaceInfoDbC galleryXml;
  FaceInfoDbC probeXml;
  UIntT subjects = 0;
  if(n2 == -1) {
    n2 = 1e6; // make it a stupid number that will not be reached
  }

  for (HashIterC<StringC, DListC<FaceInfoC> >it(faceInfoDb.Sort(true)); it; it++) {

    UIntT c = 0;
    for (DLIterC<FaceInfoC>it2(it.Data()); it2; it2++) {
      if (c < n1) {
        galleryXml.Insert(it2.Data().FaceId(), it2.Data());
      } else if (c < (n1 + n2)) {
        probeXml.Insert(it2.Data().FaceId(), it2.Data());
      }
      c++;
    }

    subjects++;
    // check subject limit not reached
    if(maxSubjects != 0 && subjects >= maxSubjects)
      break;
  }

  //: Lets save some
  RavlInfo("Saving gallery to '%s' with %d subjects and %d faces.", galleryXmlFile.data(), galleryXml.NoClients(), galleryXml.NoFaces());
  Save(galleryXmlFile, galleryXml);
  RavlInfo("Saving probe to '%s' with %d subjects and %d faces.", probeXmlFile.data(), probeXml.NoClients(), probeXml.NoFaces());
  Save(probeXmlFile, probeXml);


  return 0;
}

//: This puts a wrapper around the main program that catches
//: exceptions and turns them into readable error messages.


RAVL_ENTRY_POINT(splitXml);
