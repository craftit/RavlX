// This file is part of RAVL, Recognition And Vision Library
// Copyright (C) 2010, OmniPerception Ltd.
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//! lib=RavlXMPPLoudmouth

#include "Ravl/Option.hh"
#include "Ravl/XMPP/LMConnection.hh"
#include "Ravl/OS/Date.hh"


bool PrintMessage(std::string &from,std::string &message) {
  std::cout << " " << from << " -> " << message << std::endl;
  return true;
}

int main(int nargs,char **argv) {
  RavlN::OptionC opts(nargs,argv);
  RavlN::StringC server = opts.String("s","programmer-art.org","Server to user");
  RavlN::StringC user = opts.String("u","cento","User name");
  RavlN::StringC password = opts.String("p","","Password for user");
  opts.Check();

  RavlN::XMPPN::LMConnectionC::RefT lmc = new RavlN::XMPPN::LMConnectionC(server,user,password);

  std::cerr << "Main: Opening connection \n";
  if(!lmc->Open()) {
    std::cerr << "Failed to open connection \n";
    return 1;
  }

  RavlN::Connect(lmc->SigTextMessage(),&PrintMessage);
  
  std::cerr << "Main: Waiting for connection \n";
  while(!lmc->IsReady()) {
    RavlN::Sleep(4);
  }
  std::cerr << "Main: Sending message \n";

  lmc->SendText("craftit@jabber.org","Hello");


  std::cerr << "Main: Waiting \n";
  while(1)
    RavlN::Sleep(1);
  return 0;
}
