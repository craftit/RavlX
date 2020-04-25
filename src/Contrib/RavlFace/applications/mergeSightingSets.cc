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
#include "Ravl/Face/SightingSet.hh"
#include "Ravl/StringList.hh"
#include "Ravl/Text/TextFile.hh"
#include "Ravl/Collection.hh"
#include "Ravl/CollectionIter.hh"
#include "Ravl/RLog.hh"

using namespace RavlN;
using namespace RavlN::FaceN;

//! userlevel=User
//: Allows you to merge sighting sets

int mergeSightingSets(int argc, char **argv)
{
  OptionC opt(argc, argv);
  StringListC SightingFiles = opt.List("i", "", "input sighting files");
  StringC OutXmlFile = opt.String("o", "sighting.xml", "output sighting file");

  // RLog options
  bool verbose = opt.Boolean("v", false, "Verbose mode. ");
  StringC logFile = opt.String("l", "stderr", "Checkpoint log file. ");
  StringC logLevel = opt.String("ll", "debug", "Logging level (debug, info, warning, error)");

  //: check options selection
  opt.Check();

  // Set up logging
  RavlN::RLogInit(argc, argv, logFile, verbose);
  RavlN::RLogSubscribeL(logLevel.chars());

  SightingSetC sightingSet;
  for(DLIterC<StringC>it(SightingFiles);it;it++) {


    SightingSetC thisSightingSet;
    if(!Load(*it, thisSightingSet)) {
      rWarning("Unable to load sighting set '%s'", it.Data().data());
      continue;
    }
    rInfo("Loaded '%s' and has %s sightings", it.Data().data(), StringOf(thisSightingSet.Size()).data());
    sightingSet.Append(thisSightingSet);
  }

  rInfo("Saving XML file to '%s' with %s sightings", OutXmlFile.data(), StringOf(sightingSet.Size()).data());
  if (!Save(OutXmlFile, sightingSet)) {
    rError("Trouble saving XML file '%s'", OutXmlFile.data());
    return 1;
  }

  return 0;
}

//: This puts a wrapper around the main program that catches
//: exceptions and turns them into readable error messages.

RAVL_ENTRY_POINT(mergeSightingSets);

