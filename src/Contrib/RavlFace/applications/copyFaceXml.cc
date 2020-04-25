// Copyright (C) 2003, Omniperception Ltd.
// file-header-ends-here
// Copyright (C) 2002, Omniperception Ltd.
//! rcsid="$Id$"
//! docentry="Ravl.API.Applications.Image.Face"
//! lib=RavlFace
//! file="Ravl/Contrib/Face/Xml/modifyXml.cc"

#include "Ravl/Option.hh"
#include "Ravl/OS/Directory.hh"
#include "Ravl/EntryPnt.hh"
#include "Ravl/Stream.hh"
#include "Ravl/IO.hh"
#include "Ravl/Face/FaceInfoDb.hh"
#include "Ravl/RLog.hh"
#include "Ravl/Image/Image.hh"
#include "Ravl/Image/ByteRGBValue.hh"
#include "Ravl/StringList.hh"

using namespace RavlN;
using namespace RavlN::FaceN;
using namespace RavlImageN;

//! userlevel=User
//: Copys an XML and image data to a new directory

int main(int argc, char **argv)
{
  OptionC opt(argc, argv);
  StringListC from = opt.List("db", "input FaceInfoDb files");
  DirectoryC to = opt.String("to", "", "change the path of the original images to");
  bool jpg = opt.Boolean("jpg", false, "copy images to jpg (if they are not already)");
  StringC filename = opt.String("o", "out.xml", "Filename to give XML file.");

  // RLog options
  bool verbose = opt.Boolean("v", false, "Verbose mode. ");
  StringC logFile = opt.String("l", "stderr", "Checkpoint log file. ");
  StringC logLevel = opt.String("ll", "debug", "Logging level (debug, info, warning, error)");

  //: check options selection
  opt.Compulsory("db");
  opt.Compulsory("to");
  opt.Check();

  // Set up logging
  RavlN::RLogInit(argc, argv, logFile, verbose);
  RavlN::RLogSubscribeL(logLevel.chars());

  // Load in all the XML files into one master one!
  FaceN::FaceInfoDbC faceInfoDb(from);

  if (!to.Exists()) {
    if (!to.MakeDir()) {
      rError("Failed to make directory '%s'", to.data());
      return 1;
    }
  }

  DirectoryC imageDir = to + "/images";
  if (!imageDir.Exists()) {
    if (!imageDir.MakeDir()) {
      rError("Failed to make image dir '%s'", imageDir.data());
      return 1;
    }
  }

  rInfo("Directory '%s'", to.data());
  rInfo("Image dir '%s'", imageDir.data());

  FaceInfoDbC newDb;
  newDb.AddImagePath(imageDir);
  for (HashIterC<StringC, FaceInfoC> it(faceInfoDb); it; it++) {

    FilenameC filename = it.Data().OrigImage();
    FilenameC newFile;
    if (!jpg || filename.HasExtension("jpg")) {

      newFile = imageDir + "/" + filename.NameComponent();
      // Check whether the new file already exists.  This is tricky as
      // it could be genuine, or a problem as I am writing all the images
      // into a single directory.
      if (!newFile.Exists()) {

        if (!filename.CopyTo(newFile)) {
          rWarning("Failed to copy from '%s' to '%s'", filename.data(), newFile.data());
          continue;
        }
      } else {
        rDebug("File '%s' exists, no need to copy!", newFile.data());
      }
    } else {
      // We have
      newFile = imageDir + "/" + filename.BaseNameComponent() + ".jpg";
      // Check whether the new file already exists.  This is tricky as
      // it could be genuine, or a problem as I am writing all the images
      // into a single directory.
      if (!newFile.Exists()) {
        // Load image
        ImageC<ByteRGBValueC> image;
        if (!Load(filename, image)) {
          rWarning("Trouble loading image '%s'", filename.data());
          continue;
        }
        // Convert to JPG and save
        if (!Save(newFile, image)) {
          rDebug("File '%s' exists, no need to copy!", newFile.data());
          continue;
        }
      } else {
        rDebug("File '%s' exists, no need to convert!", newFile.data());
      }
    }
    // Insert into our new db
    it.Data().OrigImage() = newFile;
    newDb.Insert(it.Key(), it.Data());
  }

  FilenameC outFile = to + "/" + filename;
  rInfo("Saving XML file '%s'.", outFile.data());
  if (!Save(outFile, newDb)) {
    rError("Unable to save '%s'", outFile.data());
    return 1;
  }
  return 0;
}

