 // This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//////////////////////////////////////////////////////
//! rcsid="$Id$"
//! lib=RavlNet
//! file="Ravl/OS/Network/exEndPoint.cc"
//! author="Charles Galambos"
//! date="15/02/2000"
//! docentry="Ravl.API.OS.Network"
//! userlevel=Normal

#include "Ravl/Option.hh"
#include "Ravl/EntryPnt.hh"
#include "Ravl/OS/NetEndPoint.hh"
#include "Ravl/OS/Date.hh"
#include "Ravl/OS/Socket.hh"

using namespace RavlN;


// ------------------------------------------------
// --------- Message handling class --------------- 
// ------------------------------------------------
class MessageHandlerC {

 public:

   // message id 1 is reserved
   
   bool HandleMessage2(StringC msg) {
     cerr << "MessageHandlerC::HandleMessage2() : Got '" << msg << "' \n";
     return true; } ; 
   //: Handle Message of type 1


   bool HandleMessage3(IntT msg) {
     cerr << "MessageHandlerC::HandleMessage3() : Got '" << msg << "' \n";
     return true; } ; 
   //: Handle Message of type 2
   
   
   bool HandleMessage4(RealT msg) {
     cerr << "MessageHandlerC::HandleMessage4() : Got '" << msg << "' \n";
     return true; } ;
   //: Handle Message of type 3
   
};



// --------------------------------------------------------------------------------------------------
// ------------------------------------- NetEndPointC Example --------------------------------------- 
// --------------------------------------------------------------------------------------------------
//: This program provides an example of how to use NetEndPointC. This program is split into two parts, 
// a server part and a client part, determined by the "s" option. Client and server parts are both able 
// to send and receive messages. 



int exEndPoint(int nargs,char *args[]) 
{  

  // Process the options    
  OptionC opt(nargs,args);
  StringC addr = opt.String("a","localhost:4041","Address to connect to. ");
  bool serv = opt.Boolean("s",false,"Server. ");
  opt.Check();

  MessageHandlerC  handler ; 
  // ---------------------
  // ---- Server Part ---- 
  // ---------------------
  if(serv) {

  
    
    // Open a socket
    SocketC sktserv(addr,true);
    if(!sktserv.IsOpen()) {
      cerr << "Failed to open server socket. \n";
      return 1;
    }

    // Listen for connections
    while(1) {
      SocketC skt = sktserv.Listen(); // this will block untill a connection is received
      NetEndPointC ep(skt,false) ;

      // Register the callacks (id 1 is reserved !)                         
      ep.RegisterR(2,"Test Message type 2", handler, & MessageHandlerC::HandleMessage2 ) ;
      ep.RegisterR(3,"Test Message type 3", handler, & MessageHandlerC::HandleMessage3 ) ;  
      ep.RegisterR(4,"Test Message type 4", handler, & MessageHandlerC::HandleMessage4 ) ;
       
      // Do handshake to ensure setup is complete on both ends.
      ep.Ready();
      ep.WaitSetupComplete();
      
      // Send some messages to the client
      StringC msg2("Hello from server.");
      ep.Send(2,msg2);

      IntT msg3 = 100 ; 
      ep.Send(3,msg3) ; 

      RealT msg4 = 3.141 ; 
      ep.Send(4,msg4) ; 
    }
  }






  // ----------------------
  // ---- Client Part -----
  // ----------------------
  else 
  {

    NetEndPointC ep(addr,false); // connect to the server 
 
    // Register the callacks  (id 1 is reserved !) 
    ep.Register(2,"Test Message type 2", handler, &MessageHandlerC::HandleMessage2 ) ;
    ep.Register(3,"Test Message type 3", handler, &MessageHandlerC::HandleMessage3 ) ;  
    ep.Register(4,"Test Message type 4", handler, &MessageHandlerC::HandleMessage4 ) ;
    
    // Do handshake to ensure setup is complete on both ends.
    ep.Ready();
    ep.WaitSetupComplete();
    
    // Send some messages to server 
    StringC msg2("Hello from client.");
    ep.Send(2,msg2);
    
    IntT msg3 = 200 ; 
    ep.Send(3,msg3) ; 
 
    RealT msg4 = 6.282 ; 
    ep.Send(4,msg4) ; 

    // wait a while 
    Sleep(10);
    ep.Close();
  }  
  return 0;
}

RAVL_ENTRY_POINT(exEndPoint);
