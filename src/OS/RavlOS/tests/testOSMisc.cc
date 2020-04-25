// This file is part of RAVL, Recognition And Vision Library
// Copyright (C) 2001-12, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//! lib=RavlOS

#include "Ravl/OS/UnixStreamIO.hh"
#include "Ravl/Stream.hh"
#include "Ravl/OS/SysLog.hh"

#include <unistd.h>

int testUnixStreamIO();

int main()
{
  int ln;
  RavlN::SysLogLevelStdErr(RavlN::SYSLOG_DEBUG);
  
  if((ln = testUnixStreamIO()) != 0) {
    std::cerr << "Test failed on line " << ln << "\n";
    return 1;
  }
  return 0;
}

int testUnixStreamIO() {
#if RAVL_HAVE_INTFILEDESCRIPTORS
  int pipefds[2];
  
  if(pipe(pipefds) != 0) {
    return __LINE__;
  }
  
  //std::cerr << "Pipe fds=" << pipefds[0] << " " << pipefds[1] << "\n";
  
  RavlN::UnixStreamIOC unixStrmI(pipefds[0],5.0,5.0);
  RavlN::UnixStreamIOC unixStrmO(pipefds[1],5.0,5.0);
  
  unixStrmI.SetFailOnReadTimeout(true);
  unixStrmI.SetFailOnWriteTimeout(true);
  
  // Read when no data is available, this should timeout.
  std::cerr << "Attempting to read 1 byte. \n";
  char buffer[3];
  if(unixStrmI.Read(buffer,1) == 1) 
    return __LINE__;
  
#if 0
  // Write's don't seem to buffer reliably on linux,
  // not sure why.
  std::cerr << "Writing 2 bytes. \n";
  // Write 2 byte
  buffer[0] = 1;
  buffer[1] = 2;
  if(unixStrmO.Write(buffer,2) != 1) 
    return __LINE__;
  
  std::cerr << "Reading 1 byte. \n";
  
  // We should be able to read 1 byte
  if(unixStrmI.Read(buffer,1) != 1) 
    return __LINE__;
  
  std::cerr << "Reading 1 byte. \n";
  
  // Look for two bytes, this should time out with 1 byte.
  if(unixStrmI.Read(buffer,2) != 1) 
    return __LINE__;
#endif
#endif  
  
  return 0;
}
