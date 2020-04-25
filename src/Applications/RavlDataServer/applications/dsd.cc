// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2005, OmniPerception Ltd
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//! rcsid="$Id$"
//! lib=RavlDataServer

#include "Ravl/DataServer/DataServer.hh"
#include "Ravl/Option.hh"

using namespace RavlN;

int main(int nargs,char **vargs) {
  OptionC opts(nargs,vargs);
  StringC serverName = opts.String("n","DataServer","Name of data server. ");
  StringC serverConf = opts.String("c","dsd.conf","Server configuration file. ");
  opts.Check();

  cerr << "Starting server. \n";
  DataServerC dataServer(serverName);
  
  if(!dataServer.ReadConfigFile(serverConf)) {
    cerr << "Failed to configure server from file '" << serverConf << "'. ";
    return 1;
  }
  
  cerr << "Server started ok. \n";
  
  dataServer.WaitForTerminate();
  
  cerr << "Server exiting. \n";
  
  return 0;
}
