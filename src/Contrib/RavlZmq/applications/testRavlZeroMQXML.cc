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
#include "Ravl/StrStream.hh"
#include "Ravl/XMLFactory.hh"

using namespace RavlN;
using namespace RavlN::ZmqN;

int main(int nargs, char **argv)
{
  RavlN::OptionC opts(nargs, argv);
  opts.Check();

  RavlN::SysLogOpen("testRavlZeroMQ");

  StrIStreamC ss("<?xml version='1.0' encoding='UTF-8' ?>\n"
      "<?RAVL class='RavlN::XMLTreeC' ?>\n"
      "<Config>\n"
      "<ZmqContext typename=\"RavlN::ZmqN::ContextC\" />"
      "<Socket typename=\"RavlN::ZmqN::SocketC\" socketType=\"PUB\" verbose=\"true\" >"
      " <Properties>"
      "   <AutoBind value=\"*\" minPort=\"4502\" maxPort=\"4534\" mustBind=\"false\"  />"
      " </Properties>"
      "</Socket>"
      "</Config>");

  XMLTreeC xmlTree(true);
  if (!xmlTree.Read(ss))
    return __LINE__;

  UIntT numberOfSockets = 3;
  CollectionC<SocketC::RefT> sockets(numberOfSockets);
  XMLFactoryHC mainFactory("zeromq.xml", xmlTree);
  XMLFactoryContextC context(mainFactory);
  for (UIntT i = 0; i < numberOfSockets; i++) {
    SocketC::RefT socket;
    if (!context.CreateComponent("Socket", socket)) {
      RavlError("Failed to open Socket!");
      return __LINE__;
    }
    sockets.Append(socket);
  }

  return 0;
}
