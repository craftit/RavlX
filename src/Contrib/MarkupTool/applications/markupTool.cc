// Copyright (C) 2003, Omniperception Ltd.
// file-header-ends-here
//! rcsid="$Id$"

#include "Ravl/Option.hh"
#include "Ravl/EntryPnt.hh"
#include "Ravl/GUI/Manager.hh"
#include "Ravl/OS/Directory.hh"
#include "Ravl/RLog.hh"
#include "ControlWin.hh"

using namespace RavlN;
using namespace RavlGUIN;
using namespace RavlN::FaceN;

int ViewDB(int argc, char * argv[])
{
  // Get command line options
  Manager.Init(argc,argv);
  OptionC opts(argc,argv);
  StringC logFile = opts.String("l", "stderr", "Checkpoint log file. ");
  StringC logLevel = opts.String("ll", "info", "Logging level (debug, info, warning, error)");
  StringListC dbs = opts.List("db", "The input XML faces");
  StringListC imagePath = opts.List("imagePath", "Pick up images from these image paths instead");
  StringListC sightingSets = opts.List("sighting", "Sighting sets, if available");
  bool autoScale = opts.Boolean("scale", true, "Automatically scale images to fit window");
  bool Verbose = opts.Boolean("v", false, "Do things verbosely");
  opts.Check();

  RLogInit(argc, argv, logFile.chars(), Verbose);
  RLogSubscribeL(logLevel.chars());

  rInfo("\nMarkup Tool for Ravl Face XML files!");

  // Create main window
  FaceInfoDbC db(dbs, imagePath);
  ControlWinC win(db, sightingSets, autoScale);
  win.Show();

  // Start GUI
  Manager.Start();

  // Wait for GUI to finish
  //Manager.Wait();


  return 0;
}

RAVL_ENTRY_POINT(ViewDB);
