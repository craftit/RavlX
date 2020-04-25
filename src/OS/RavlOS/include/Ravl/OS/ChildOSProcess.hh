// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVL_CHILDEXECPROCESS_HEADER
#define RAVL_CHILDEXECPROCESS_HEADER 1
/////////////////////////////////////////////////////////////////
//! example=exChild.cc
//! file="Ravl/OS/Exec/ChildOSProcess.hh"
//! lib=RavlOS
//! author="Charles Galambos"
//! docentry="Ravl.API.OS.Exec"
//! rcsid="$Id$"
//! date="24/11/1998"

#include "Ravl/Stream.hh"
#include "Ravl/String.hh"
#include "Ravl/OS/Filename.hh"
#include "Ravl/StringList.hh"
#include "Ravl/OS/OSProcess.hh"

namespace RavlN {
  
  ////////////////////////////////////////////////////
  //! userlevel=Develop
  //: Child process body.
  // This class forks off a child process.
  
  class ChildOSProcessBodyC 
    : public OSProcessBodyC
  {
  public:
    ChildOSProcessBodyC();
    //: Default constructor.
  
    ChildOSProcessBodyC(StringC args,bool useStdOut = false,bool useStdErr = false,bool useStdIn = false);
    //: Start a child process.
    
    ChildOSProcessBodyC(StringC args,FilenameC out,bool redirectStderr = false,bool useStdIn = false);
    //: Start a child process.
  
    ChildOSProcessBodyC(StringListC args, bool useStdOut,bool useStdErr,bool useStdIn);
    //: Start a child process.
    // where the first arg is the name of the program to run.
    
    virtual ~ChildOSProcessBodyC();
    //: Destructor
    
    bool Run(StringListC args,bool useStdOut = false,bool useStdErr = false,bool useStdIn = false);
    //: Run child process.
    // where the first arg is the name of the program to run.
    // Will return false if failed, or if child is already
    // running.
    // NB. If StdErr or StdOut are false, the output will go to 
    // StdErr or StdOut of this process.   
    
    bool Run(StringListC args,FilenameC out,bool redirectStderr = false,bool useStdIn = false);
    //: Run, sending output to a file.
    
    IStreamC &StdOut() 
    { return pstdout; }
    //: Get the stdout of the process.
    // Is invalid, if output is not directed back to this process.
    
    IStreamC &StdErr() 
    { return pstderr; }
    //: Get the stderr of the process.
    // Is invalid, if output is not directed back to this process.
    
    OStreamC &StdIn() 
    { return pstdin; }
    //: Send to the stdin of the process.
    // Is invalid, if output is not directed back to this process.
    
    int GetPID() const 
    { return pid; }
    //: Get the process ID.
    
    bool IsRunning();
    //: Test if child is still running.
    
    bool ExitedOk() 
    { return exitok; }
    //: Test eixted ok flag.
    // only valid if child has actual exited cleanly
    // as apposed to a seg fault or signal.
    
    bool Wait();
    //: Wait for program to exit.
    // This function will not return until the program has
    // exited.
    
    bool Wait(double maxTime);
    //: Wait for program to exit.
    // maxTime in the maximum time in seconds,
    // if it is negative it won't time out.
    
    IntT ExitCode() const { return exitcode; }
    //: Get exit code.
    // Return the exit code of the program, if the program
    // failed to exit normal this is set to the signal number
    // which ended the program. This variable is only valid if
    // program has exited.
    
    bool Terminate(double maxTime = 10);
    //: Terminate a process politely.
    
    bool GotExitCode(int code);
    //: Call if you get the exit code from OSProcessC::WaitForChild()
    // Returns true as ExitedOk().
    
  protected:
    bool Exec(StringListC args,int infd,int outfd,int errfd);
    //: Execute a program.
    // if infd,outfd or errfd are negative there not changed.
    // otherwise they are used by the child process as
    // standard input, output and error.
  
    int SetupPipe(IStreamC &strm);
    //: Setup input stream.
    // Connect strm to one end, and return a file descriptor
    // for the other. 
    // If failed returns -1.
    
    int SetupPipe(OStreamC &strm);
    //: Setup output stream.
    // Connect strm to one end, and return a file descriptor
    // for the other.
    // If failed returns -1.
    
    bool CheckExit(bool block = false);
    //: Check exit status of child.
    // Returns true if child has exited, and false if
    // is still running or was never run.
    
    IStreamC pstdout;
    IStreamC pstderr;
    OStreamC pstdin;
    
    bool running; // Child process process running last time we looked ?
    bool exitok;  // Did child exit normally ?
    int exitcode;
  };
  
  ////////////////////////////////////////////////////
  //! userlevel=Normal
  //: Child process handle.
  
  class ChildOSProcessC 
    : public OSProcessC
  {
  public:
    ChildOSProcessC()
    {}
    //: Default constructor.
    // Creates an invalid handle.
    
    ChildOSProcessC(StringC cmd,bool useStdOut = false,bool useStdErr = false,bool useStdIn = false);
    //: Start a child process.
    // if useStdOut, useStdErr or useStdIn is true, the matching channel
    // of the child process is connect to the appropriate stream.
    
    ChildOSProcessC(StringC cmd,FilenameC out,bool redirectStderr = false,bool useStdIn = false);
    //: Start a child process.
    // Send output to file 'out', if redirectStderr is true send
    // stderr there too. 

    ChildOSProcessC(StringListC args, bool useStdOut=false, bool useStdErr=false, bool useStdIn=false);
    //: Start a child process.
    // where the first arg is the name of the program to run.
    
  protected:
    ChildOSProcessBodyC &Body() 
    { return static_cast<ChildOSProcessBodyC &>(OSProcessC::Body()); }
    //: Access body.
    
    const ChildOSProcessBodyC &Body() const 
    { return static_cast<const ChildOSProcessBodyC &>(OSProcessC::Body()); }
    //: Access body.
    
  public:    
    IStreamC &StdOut() 
    { return Body().StdOut(); }
    //: Get the stdout of the process.
    // Is invalid, if output is not directed back to this process.
    
    IStreamC &StdErr() 
    { return Body().StdErr(); }
    //: Get the stderr of the process.
    // Is invalid, if output is not directed back to this process.
    
    OStreamC &StdIn() 
    { return Body().StdIn(); }
    //: Send to the stdin of the process.
    // Is invalid, if output is not directed back to this process.
    
    int GetPID() const 
    { return Body().GetPID(); }
    //: Get the process ID.
    
    bool IsRunning() 
    { return Body().IsRunning(); }
    //: Test if child is still running.
    
    bool ExitedOk() 
    { return Body().ExitedOk(); }
    //: Test eixted ok flag.
    // only valid if child has actual exited.
    
    inline bool Wait() 
    { return Body().Wait(); }
    //: Wait for program to exit.
    // It returns false if the wait failed. Possibly because
    // the child program has terminated already, or was never 
    // started.
  
    inline bool Wait(double maxTime) 
    { return Body().Wait(maxTime); }
    //: Wait for program to exit.
    // maxTime in the maximum time in seconds,
    // if it is negative it won't time out. <p>
    // It returns false if the wait failed. Possibly because
    // the child program has terminated already, or was never 
    // started.
    
    IntT ExitCode() const 
    { return Body().ExitCode(); }
    //: Get exit code.
    // Return the exit code of the program, if the program
    // failed to exit normally this is set to the signal number
    // which ended the program. This variable is only valid if
    // program has exited.
    
    bool Terminate(double maxDelay = 10) 
    { return Body().Terminate(maxDelay); }
    //: Close the processes politely but firmly.
    // This first sends a SIGQUIT, then after maxDelay if the process
    // hasn't exited in sends a SIGTERM.  It will wait up to another maxDelay seconds
    // if the process doesn't terminate, then return false if failed true if
    // terminated.
  };

}

#endif
