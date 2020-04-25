// This file is part of RAVL, Recognition And Vision Library
// Copyright (C) 2013, Charles Galambos
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//! lib=RavlPatternRec

#include "Ravl/Option.hh"
#include "Ravl/XMLFactory.hh"
#include "Ravl/SysLog.hh"
#include "Ravl/Resource.hh"
#include "Ravl/IO.hh"
#include "Ravl/OS/Filename.hh"
#include "Ravl/DP/ProcessPlayList.hh"

using namespace RavlN;

#define USE_EXCEPTIONS 0

// Simple program which performs leave one out

int main(int nargs, char **argv) {

  OptionC opts(nargs, argv);

  StringC installDir = opts.String("i", PROJECT_OUT, "Install directory.");
  RavlN::SetResourceRoot(installDir);
  StringC configFile = opts.String("c", RavlN::Resource("Ravl", "doBatchProcess.xml"),
      "Classifier config file.");
  DListC<StringC> fileList = opts.List("fl","File list");
  StringC playListFile = opts.String("pl","","Play list file");
  StringC outputTemplate = opts.String("ot","%f.out.abs","Output template");
  bool verbose = opts.Boolean("v",false,"Verbose");
  opts.Check();

  SysLogOpen("doProcess",false,true,true,-1,verbose);

  PlayListC playList;

  if(opts.IsOnCommandLine("fl")) {
    playList = PlayListC(fileList);
  } else {
    if(!playListFile.IsEmpty() && !Load(playListFile,playList)) {
      RavlError("Failed to load play list '%s' ",playListFile.c_str());
      return 1;
    }
  }
  if(!playList.IsValid()) {
    RavlError("No files to process.");
    return 1;
  }

#if USE_EXCEPTIONS
  try {
#endif
    XMLFactoryContextC context(configFile);

    // Get operation to perform
    DPStreamOpC process;
    if(!context.UseComponent("Process",process)) {
      RavlError("Failed to create process");
      return 1;
    }
    RavlAssert(process.IsValid());

    // Set things up.
    ProcessPlayListC doProcess(playList,process,outputTemplate,verbose);

    // Do some work.
    if(!doProcess.ProcessList()) {
      RavlError("Failed to process list");
      return 1;
    }

#if USE_EXCEPTIONS
  } catch (const RavlN::ExceptionC &exc) {
    RavlError( "Exception:%s", exc.Text());
  } catch (...) {
    RavlError( "Unknown exception");
  }
#endif
  return 0;
}
