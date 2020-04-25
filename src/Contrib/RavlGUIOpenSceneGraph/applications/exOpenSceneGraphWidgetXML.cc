// This file is part of RAVL, Recognition And Vision Library
// Copyright (C) 2010, OmniPerception Ltd.
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
////////////////////////////////////////////////////////////////
//! file = "Ravl/Contrib/OpenSceneGraph/exOpenSceneGraphWidget.cc"
//! lib=RavlGUIOpenSceneGraph
//! author = "Warren Moore"

#include "Ravl/GUI/Manager.hh"
#include "Ravl/EntryPnt.hh"
#include "Ravl/Option.hh"
#include "Ravl/GUI/Window.hh"
#include "Ravl/GUI/Button.hh"
#include "Ravl/GUI/LBox.hh"
#include "Ravl/DP/FileFormatIO.hh"
#include "Ravl/GUI/MouseEvent.hh"
#include "Ravl/OS/Date.hh"
#include "Ravl/OpenSceneGraph/OpenSceneGraphWidget.hh"
#include "Ravl/Image/RealRGBAValue.hh"
#include "Ravl/Resource.hh"
#include "Ravl/RLog.hh"

using namespace RavlN;
using namespace RavlGUIN;
using namespace RavlOSGN;

bool pressFunc(MouseEventC &me,OpenSceneGraphWidgetC &osgWidget) {
  cerr << "Press " << me.Row() << " " << me.Col() << "\n";
  return true;
}

bool releaseFunc(MouseEventC &me) {
  cerr << "Release " << me.Row() << " " << me.Col() << "\n";
  return true;
}

int DoMain(int argc, char *argv[])
{
  //: parse options
  RavlN::OptionC opt(argc, argv);
  RavlN::SetResourceRoot(opt.String("i", PROJECT_OUT, "Install location. "));
  RavlN::StringC configFile = opt.String("c", RavlN::Resource("RAVL/OpenSceneGraph", "exOpenSceneGraph.xml"), "Configuration file");
  RavlN::StringC logFile = opt.String("l", "stderr", "Log file. ");
  RavlN::StringC logLevel = opt.String("ll", "debug", "Logging level. ");

  RavlN::StringC logModuleName = opt.String("lm","","Log messages from a specific module ");
  RavlN::StringC logModulePath = opt.String("lmp","","Log messages from a specific path ");
  
  bool verbose = opt.Boolean("v", true, "Verbose, Output context information. ");
  bool loadOnly = opt.Boolean("lo", false, "Load model only. ");
  bool formatList = opt.Boolean("fl", false, "Format list. ");

  RavlGUIN::Manager.Init(argc, argv);
  opt.Check();

  RavlN::RLogInit(argc, argv, logFile.chars(), verbose);
  RavlN::RLogSubscribeL(logLevel.chars());
  if(!logModuleName.IsEmpty() || !logModulePath.IsEmpty())
    RavlN::RLogSubscribe(logModuleName,logModulePath,rlog::Log_Debug);
  
  rInfo("exOpenSceneGraphXML Built:%s ",__DATE__);
  
  RavlN::XMLFactoryC::RefT mainFactory = new RavlN::XMLFactoryC(configFile);
  RavlN::XMLFactoryContextC factory(*mainFactory);
      
  WindowC win(100, 100, "OpenSceneGraph");
  OpenSceneGraphWidgetC osgWidget;
  
  if(!factory.UseComponent("OpenSceneGraphWidget",osgWidget,false,typeid(OpenSceneGraphWidgetC))) {
    rError("No open scene graph widget found. ");
    return 1;
  }
  
  Connect(osgWidget.Signal("button_press_event"), &pressFunc,MouseEventC(),osgWidget);
  Connect(osgWidget.Signal("button_release_event"), &releaseFunc);
  
  RavlGUIN::LBoxC vbox = VBox(osgWidget);

  win.Add(vbox);

  Manager.Execute();

  win.Show();

  Manager.Wait();

  return 0;
}

RAVL_ENTRY_POINT(DoMain);
