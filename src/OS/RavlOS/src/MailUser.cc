// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
///////////////////////////////////////////////////////////////////////
//! date="29.8.97"
//! author="Charles Galambos"
//! rcsid="$Id$"
//! lib=RavlOS
//! file="Ravl/OS/Exec/MailUser.cc"

#include "Ravl/OS/UserInfo.hh"
#include "Ravl/OS/ChildOSProcess.hh"
#include "Ravl/Assert.hh"
#include <stdlib.h>
#include <errno.h>
#include <stdio.h>

#define DODEBUG	0
#if DODEBUG
#define ONDEBUG(x) x
#else
#define ONDEBUG(x)
#endif

namespace RavlN {
  ////////////////////////////
  // Attempt to mail the user
  
  StringC UserInfoC::RAVLMailProg("/usr/lib/sendmail -t ");

  bool UserInfoC::Mail(StringC Content) {
    if(!Exists())
      return false;
    ChildOSProcessC mailProc(RAVLMailProg,false,false,true);
    mailProc.StdIn() << Content << "\n.\n"  << flush ; // "." flags eof for sendmail
    //mailProc.StdIn().Close();
    bool ok = mailProc.Wait(10);
    if(ok)
      ok = mailProc.ExitedOk();
    // Report an error ?
    if(!ok) 
      cerr << "Warning: Failed to send mail to " << login << "\n"
           << "Exit code " << mailProc.ExitCode() << endl;
    return ok;
  }
  
  ////////////////////////////
  // Attempt to mail the user
  
  bool UserInfoC::Mail(StringC Subject,StringC Content) {
    if(!Exists())
      return false;
    StringC passOn;
    passOn += StringC("To:") + login + "\n";
    passOn += StringC("Subject:") + Subject + "\n";
    passOn += StringC("\n\n") + Content;  
    return Mail(passOn);
  }
  
  ////////////////////////////
  // Attempt to mail the user
  
  bool UserInfoC::Mail(StringC /*Subject*/,TextFileC &/*Content*/) {
    if(!Exists())
      return false;
    RavlAssert(0);
    return false;
  }
  
  ////////////////////////////
  // Attempt to mail the user
  
  bool UserInfoC::MailFile(StringC Subject,StringC Filename) {
    if(!Exists())
      return false;
    
#if 0
    if(system("cat " + Filename + " |" + RAVLMailProg + " -subject \""+Subject +"\" " + login) < 0) {
      perror("ERROR: Mail to " + login + " failed ");
      return false;
    }
#endif
    RavlAssert(0);
    return false;
  }

}
