// This file is part of RAVL, Recognition And Vision Library
// Copyright (C) 2011, Charles Galambos
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//! lib=RavlZmq

#include "Ravl/Zmq/Context.hh"
#include "Ravl/Zmq/Socket.hh"
#include "Ravl/Option.hh"
#include "Ravl/OS/Date.hh"
#include "Ravl/OS/SysLog.hh"

using namespace RavlN;
using namespace RavlN::ZmqN;

int main(int nargs,char **argv) {
  RavlN::OptionC opts(nargs,argv);
  bool server = opts.Boolean("s",false,"Server");
  StringC addr = opts.String("a","tcp://192.168.42.89:5551","Address to connect to");
  StringC topic = opts.String("t","Hello!","Topic to use.");
  bool sndMessages = opts.Boolean("snd",false,"Send messages to socket.");
  StringC socketType = opts.String("st","DEALER","Socket type to send to ");
  opts.Check();

  RavlN::SysLogOpen("testRavlZeroMQ");

  ContextC::RefT ctxt = new RavlN::ZmqN::ContextC(1);

  if(sndMessages) {
    RavlN::ZmqN::SocketTypeT st = RavlN::ZmqN::SocketType(socketType);
    SocketC::RefT skt = new SocketC(*ctxt,st);
    if(server) {
      RavlDebug("Binding to '%s' ",addr.c_str());
      skt->Bind(addr);
    } else {
      RavlDebug("Connecting to '%s' ",addr.c_str());
      skt->Connect(addr);
    }
    for(int i = 0;i < 1000;i++) {
      RavlDebug("Sending Hello...");
      MessageC::RefT msg = new MessageC(topic);
      msg->Push("");
      skt->Send(*msg);
      RavlN::Sleep(0.2);
    }
    RavlN::Sleep(10);
    return 0;
  }

  if(server) {
    SocketC::RefT skt = new SocketC(*ctxt,ZST_PUBLISH);
    skt->Bind(addr.c_str());
    for(int i = 0;i < 1000;i++) {
      RavlDebug("Sending Hello...");
      MessageC::RefT msg = new MessageC(topic);
      skt->Send(*msg);
      RavlN::Sleep(0.2);
    }

  } else {
    SocketC::RefT skt = new SocketC(*ctxt,ZST_SUBCRIBE);
    skt->Connect(addr.c_str());
    skt->Subscribe(topic);
    while(1) {
      MessageC::RefT msg;
      skt->Recieve(msg);
      std::string txt;
      msg->Pop(txt);
      RavlDebug("Got '%s' ",txt.data());
    }

  }


  return 0;
}
