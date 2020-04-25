// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
/////////////////////////////////////////////////////////////////
//! rcsid="$Id$"
//! lib=RavlOS
//! file="Ravl/OS/Exec/ChildOSProcess.cc"

#include "Ravl/OS/ChildOSProcess.hh"

#ifndef _POSIX_C_SOURCE
#define _POSIX_C_SOURCE 3
#endif

#if 0
#ifdef __linux__
extern "C" {
  int kill(pid_t pid, int sig);
};
#endif
#endif

#include <sys/types.h>
#if !RAVL_COMPILER_VISUALCPP
#include <sys/wait.h>
#include <unistd.h>
#else
#include <direct.h>
#endif
#include <fcntl.h>
#include <stdio.h>
#include <errno.h>
#include <signal.h>

#define DODEBUG 0
#if DODEBUG
#define ONDEBUG(x) x
#else
#define ONDEBUG(x)
#endif

#include "Ravl/OS/Filename.hh"

#if RAVL_OS_LINUX || RAVL_OS_LINUX64
#define RAVL_LINIX_WAITPIDBUG 1
#endif

namespace RavlN {
  
  ///////////////////////////////////////////////////////
//: Start a child process.

ChildOSProcessC::ChildOSProcessC(StringC cmd,bool useStdOut,bool useStdErr,bool useStdIn)
  : OSProcessC(*new ChildOSProcessBodyC(cmd,useStdOut,useStdErr,useStdIn))
{}
  
  //: Start a child process.
  
  ChildOSProcessC::ChildOSProcessC(StringC cmd,FilenameC out,bool redirectStderr,bool useStdIn)
    : OSProcessC(*new ChildOSProcessBodyC(cmd,out,redirectStderr,useStdIn))
  {}

   //: Start a child process.

  ChildOSProcessC::ChildOSProcessC(StringListC args, bool useStdOut,bool useStdErr,bool useStdIn)
    : OSProcessC(*new ChildOSProcessBodyC(args, useStdOut, useStdErr, useStdIn))
  {}


  ///////////////////////////////////////////////////////
  //: Default constructor.
  
  ChildOSProcessBodyC::ChildOSProcessBodyC()
    : OSProcessBodyC(-1),
      running(false),
      exitok(false),
      exitcode(-1)
  {}
  
  //: Start a child process.
  
  ChildOSProcessBodyC::ChildOSProcessBodyC(StringC cmd,FilenameC out,bool redirectStderr,bool useStdIn)
    : OSProcessBodyC(-1),
      running(false),
      exitok(false),
      exitcode(-1)
  {
    StringListC strlst(cmd);
    Run(strlst,out,redirectStderr,useStdIn);
  }
  
  //: Start a child process.
  
  ChildOSProcessBodyC::ChildOSProcessBodyC(StringC cmd,bool useStdOut,bool useStdErr,bool useStdIn)
    : OSProcessBodyC(-1),
      running(false),
      exitok(false),
      exitcode(-1)
  {
    StringListC strlst(cmd);
    Run(strlst,useStdOut,useStdErr,useStdIn);
  }
  
  //: Start a child process.
  // where the first arg is the name of the program to run.
  
  ChildOSProcessBodyC::ChildOSProcessBodyC(StringListC args,bool useStdOut,bool useStdErr,bool useStdIn)
    : OSProcessBodyC(-1),
      running(false),
      exitok(false),
      exitcode(-1)
  {
    Run(args,useStdOut,useStdErr,useStdIn);
  }
  
  //: Destructor
  
  ChildOSProcessBodyC::~ChildOSProcessBodyC() {
    ONDEBUG(cerr << "ChildOSProcessBodyC::~ChildOSProcessBodyC(), Called. \n");
  }

  // Use the most appropriate fork for the architecture
  // for doing an exec.
  
#define EXECFORK fork

  //: Run child process.
  // Will return false if failed, or if child is already
  // running.
  
  bool ChildOSProcessBodyC::Run(StringListC args,bool useStdOut,bool useStdErr,bool useStdIn) {
    ONDEBUG(cerr << "ChildOSProcessBodyC::Run(), Called. \n");
    if(args.IsEmpty())
      return false; // No program to run.
    
    // Setup stdio stuff.
    int stdoutfd = -1,stderrfd = -1,stdinfd = -1;
    
    if(useStdOut) 
      stdoutfd = SetupPipe(pstdout);
    
    if(useStdErr) 
      stderrfd = SetupPipe(pstderr);
    
    if(useStdIn) 
      stdinfd = SetupPipe(pstdin);
    
    // And run it.
    return Exec(args,stdinfd,stdoutfd,stderrfd);
  }
  
  //////////////////////////////////
  //: Run, sending output to a file.
  
  bool ChildOSProcessBodyC::Run(StringListC args,FilenameC out,bool redirectStderr,bool useStdIn) {
#if !RAVL_COMPILER_VISUALCPP
    if(args.IsEmpty())
      return false; // No program to run.
    
    // Setup stdio stuff.
    
    int stdoutfd = -1,stderrfd = -1,stdinfd = -1;
    if((stdoutfd = open(out.chars(),O_WRONLY | O_CREAT | O_TRUNC,S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH)) < 0) {
      cerr << "Failed to open file '" << out << "'\n";
      return false;
    }
    fcntl(stdoutfd,F_SETFD,1); // Close this descriptor on exec.
    ONDEBUG(cerr << "Text file output fd:" << stdoutfd << "\n");
    if(redirectStderr)
      stderrfd = stdoutfd;
    
    if(useStdIn)
      stdinfd = SetupPipe(pstdin);
    
    // And run it.
    bool ret = Exec(args,stdinfd,stdoutfd,stderrfd);  
    return ret;
#else
    RavlIssueError("ChildOSProcessBodyC::Run() Not implemented. \n");
#endif
  }
  
  //////////////////////////////////
  //: Execute a program.
  // if infd,outfd or errfd are negative there not changed.
  
  bool ChildOSProcessBodyC::Exec(StringListC args,int infd,int outfd,int errfd) {  
    ONDEBUG(cerr << "ChildOSProcessBodyC::Exec(), Called. \n");
#if !RAVL_COMPILER_VISUALCPP
    if(args.IsEmpty()) {
      cerr << "ChildOSProcessBodyC::Exec(), No program to run. \n";
      return false; // No program to run.
    }
    if(running) {
      cerr << "ChildOSProcessBodyC::Exec(), Failed. Program already running. \n";
      return false;
    }
    // Make arg list, do it before vfork to avoid
    // problems that may be caused by multithreading.
    char **arglst = new char *[args.Size()+1];
    char **place = arglst;
    
    for(DLIterC<StringC> it(args);it;it++,place++)
      *place = const_cast<char *>(it.Data().chars());
    *place = 0; // Zero terminate list.
    
    // Fork off new process.
    if((pid = EXECFORK()) == 0) {
      // In child, setup a stdin and stdout.
      bool err = false;
      if(infd >= 0) {
	if(dup2(infd,STDIN_FILENO) >= 0)
	  fcntl(STDIN_FILENO,F_SETFD,0); // Make sure file is kept open across exec.
	else
	  err = true;
      }
      if(outfd >= 0 && !err) {
	if(dup2(outfd,STDOUT_FILENO) >= 0)
	  fcntl(STDOUT_FILENO,F_SETFD,0); // Make sure file is kept open across exec.
	else
	  err = true;
      }
      if(errfd >= 0 && !err) {
	if(dup2(errfd,STDERR_FILENO) >= 0)
	  fcntl(STDERR_FILENO,F_SETFD,0); // Make sure file is kept open across exec.
	else
	  err = true;
      }
      if(err) {
	perror("ChildOSProcessBodyC::Run(): Error setup execp ");
	_exit(-1);
      }
      execvp(*arglst,arglst); 
      // If no error this won't return.
      perror("ChildOSProcessBodyC::Run(): execvp failed ");
      cerr << "ChildOSProcessBodyC::Run(), Failed to run '" << args.First() << "' \n";
      _exit(-1);
    }
    // Close unneeded file descriptors.
    if(infd >= 0)  close(infd);
    if(outfd >= 0) close(outfd);
    if(errfd >= 0) close(errfd);
    
    // Cleanup args.
    
    delete [] arglst;
    if(pid < 0) { // Did fork fail ?
      cerr << "ChildOSProcessBodyC::Exec(), Failed to fork. \n";
      running = false;
      return false;
    }
    running = true;
    //cerr << "Started:" << pid << "\n";
    return true;
#else
    throw ExceptionC("ChildOSProcessBodyC::Exec() Not implemented. ");
#endif
  }
  
  //: Setup input stream.
  // Connect strm to one end, and return a file descriptor
  // for the other. 
  // If failed returns -1.
  
  int ChildOSProcessBodyC::SetupPipe(IStreamC &strm) {
#if !RAVL_COMPILER_VISUALCPP
    int fds[2];
    if(pipe(fds) != 0) { // 0-Read 1-Write
      cerr << "ChildOSProcessBodyC::SetupIPipe(), Failed to create pipe. \n";
      return -1;
    }
    strm = IStreamC(fds[0]);
    fcntl(fds[0],F_SETFD,1); // Close this descriptor on exec.
    fcntl(fds[1],F_SETFD,1); // Close this descriptor on exec.
    return fds[1];
#else
    throw ExceptionC("ChildOSProcessBodyC::SetupPipe(), Not implemented. ");
#endif
  }
  
  //: Setup output stream.
  // Connect strm to one end, and return a file descriptor
  // for the other. 
  // If failed returns -1.
  
  int ChildOSProcessBodyC::SetupPipe(OStreamC &strm) {
#if !RAVL_COMPILER_VISUALCPP
    int fds[2];
    if(pipe(fds) != 0) { // 0-Read 1-Write
      cerr << "ChildOSProcessBodyC::SetupOPipe(), Failed to create pipe. \n";
      return -1;
    }
    strm = OStreamC(fds[1]);
    fcntl(fds[0],F_SETFD,1); // Close this descriptor on exec.
    fcntl(fds[1],F_SETFD,1); // Close this descriptor on exec.
    return fds[0];
#else
    throw ExceptionC("ChildOSProcessBodyC::SetupPipe(), Not implemented. ");
#endif
  }
  
  //////////////////////////////////
  //: Call if you get the exit code from OSProcessC::WaitForChild()
  
  bool ChildOSProcessBodyC::GotExitCode(int code) {
#if !RAVL_COMPILER_VISUALCPP
    if(WIFEXITED(code)) { // Exited normally ?
      exitcode = WEXITSTATUS(code);
      exitok = true;
    }
    if(WIFSIGNALED(code)) { // Exited on signal ?
      exitcode = WTERMSIG(code);
      exitok = false;
    }
    running = false;
    return exitok;
#else
    throw ExceptionC("ChildOSProcessBodyC::GotExitCode(), Not implemented. ");
#endif
  }
  
  //////////////////////////////////
  //: Check exit status of child.

  bool ChildOSProcessBodyC::CheckExit(bool block) {
#if !RAVL_COMPILER_VISUALCPP
    int stat,id;
    int opt = WNOHANG;
#if RAVL_OS_LINUX || RAVL_OS_LINUX64
    opt |= __WALL;
#endif
    errno = 0;
    if((id = waitpid(pid,&stat,opt)) == 0) {
      if(errno == ECHILD) { // No such child ??
	exitok = false;
	exitcode = 0;
	return true;
      }
      return false; // OSProcess is still running.
    }
    if(id < 0) {
      if(errno != ECHILD)
	cerr << "ChildOSProcessBodyC::IsRunning(), waitpid failed. " << id << " errno=" << errno << "\n";
#if RAVL_LINIX_WAITPIDBUG
      // This is a bit of a hack to get around waitpid giving ECHILD when the child process
      // is still running.   It only happens on some machines, could this be a kernel bug ??
      
      FilenameC fn("/proc/");
      fn += StringC(pid);
      if(fn.Exists()) { // There's some hope.
	fn += "/stat";
	IStreamC in(fn);
	UIntT apid;
	in >> apid;
	if(in) { // Not having trouble reading the file ? If we are, assume its dead.
	  in.SkipTo(')'); // Skip to after the executable name
	  char state = 'z';
	  in >> state; // This is what we're after!
	  if(in) { // Still got a good stream ?
	    switch(state) {
	    case 'R': // Running.
	    case 'S': // Sleeping
	    case 'D': // Sound asleep.
	    case 'T': // Traced or stopped.
	    case 'W': // Paging.
	      return false; // Process is still alive! Well at least ish.
	    default:
	      cerr << "WARNING: Unknown process state '" << state << "' \n";
	    return false; // Assume its alive.
	    case 'Z': // Zombie
	      // Its really dead!
	      break;
	    }
	  }
	}
      }
#endif      
      exitok = false;
      exitcode = 0;
      return true;
    }
    GotExitCode(stat);
    return true;
#else
    throw ExceptionC("ChildOSProcessBodyC::CheckExit(), Not implemented. ");
#endif
  }
  
  //////////////////////////////////
  //: Wait for program to exit.
  
  bool ChildOSProcessBodyC::Wait() {
#if !RAVL_COMPILER_VISUALCPP
    int stat,id;
    id = waitpid(pid,&stat,0);
    if(id < 0) {
      perror("ChildOSProcessBodyC::Wait(), waitpid failed. ");
      return false;
    }
    running = false;
    GotExitCode(stat);
    return true;
#else
    throw ExceptionC("ChildOSProcessBodyC::Wait(), Not implemented. ");
#endif
  }
  
  //////////////////////////////////
  //: Test if child is still running.
  
  bool ChildOSProcessBodyC::IsRunning() {
    if(!running)
      return false;
    if(pid < 0)
      return false; // OSProcess never started.
    return !CheckExit();
  }
}
