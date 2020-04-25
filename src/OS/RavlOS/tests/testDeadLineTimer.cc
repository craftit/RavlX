// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//! rcsid="$Id$"
//! lib=RavlOS
//! file="Ravl/OS/Time/testDeadLineTimer.cc"
//! docentry="Ravl.API.OS.Time"
//! userlevel=Develop

#include "Ravl/Option.hh"
#include "Ravl/OS/DeadLineTimer.hh"

using namespace RavlN;

int main(int nargs,char *args[])
{
  OptionC option(nargs,args,true);
  option.Check();
  
  DeadLineTimerC dl1(0.5);
  DeadLineTimerC dl2(4.0);
  
  bool d1(false),d2(false);
  cerr << "Waiting for signals. \n";
  while((!d1) && (!d2))
    {
      if(d1 != dl1.IsTimeUp()) {
	cerr << "Timer 1 expired. \n";
	d1 = dl1.IsTimeUp();
      }
      if(d2 != dl2.IsTimeUp()) {
	cerr << "Timer 2 expired. \n";
	d2 = dl2.IsTimeUp();
      }
    };
  
  cout << "Checking return condition.\n";
  if(!d1) {
    cerr << "ERROR: Bad sequence. \n";
    return 2;
  }
  if(dl1.WaitForIt()) { // Should have expired already.
    cerr << "ERROR: WaitForIt returned wrong condition. \n";
    return 3;
  }
  cout << "Setting up timer 3. \n";
  DeadLineTimerC dl3(0.2);
  if(!dl3.Reset(0.1)) {
    cerr << "ERROR: Reset failed. \n";
    return 1;
  }
  if(dl2.IsTimeUp()) {
    cerr << "Four seconds up already! \n";
  }
  dl2.WaitForIt(); // Wait for 4 second timer to expire.
  if(!dl2.IsTimeUp()) {
    cerr << "ERROR: dl2 hasn't expired. \n";
    return 5;
  }
  if(!dl3.IsTimeUp()) { // This should have expired.
    cerr << "ERROR: WaitForIt() Failed. \n";
    return 4;
  }
  cout << "Checking virtual timer... \n";
  // Checking virtual timer.
  if(!dl3.Reset(2)) {
    cerr << "dl3 Reset failed... \n";
    return 7;
  }
  DeadLineTimerC dl4(0.1,false);
  cout << "Spin... \n";
  while(!dl4.IsTimeUp() && !dl3.IsTimeUp()) ;
  if(dl3.IsTimeUp()) {
    cerr << "ERROR: Virtual timer test failed. \n This may also occure if machine is VERY highly loaded. \n";
    return 6;
  }
  cerr << "Done. \n";
  return 0;
}
