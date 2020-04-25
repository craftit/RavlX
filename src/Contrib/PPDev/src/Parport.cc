// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2004, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#include "Ravl/OS/Parport.hh"
#include <sys/ioctl.h>
#include <linux/parport.h> 
#include <linux/ppdev.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
//! rcsid="$Id$"
//! lib=PPDev
//! file="Ravl/Contrib/ParallelPortPPDev/Parport.cc"


namespace RavlN {


  //: Constructor 
  // ------------------------------------------------------------------------------------
  ParallelPortPPDevBodyC::ParallelPortPPDevBodyC(const FilenameC & device) 
    : fd(0), filename(device), initialized(false)  
{}
  
  
  //: Error 
  // ------------------------------------------------------------------------------------
  void ParallelPortPPDevBodyC::Error ( const StringC & message ) const
{  perror ("\n"+message) ; }

  
  //: Initialize 
   // -----------------------------------------------------------------------------------
  bool ParallelPortPPDevBodyC::Initialize (void) 
{
  initialized = true ;
  // check device exists
  if ( ! filename.Exists() )
    {    
      cerr << "\nError: file " << filename << " does not exist" ; 
      initialized = false ; 
      return initialized ; 
    }
  
  // open it
  fd = open ( filename, O_RDWR ) ; 
  if ( fd == -1 )  {
    Error("Error: Opening File: " + filename  ); 
    initialized = false ; return initialized ; }
  
  // Claim the port.  Not exclusive 
  if ( ! Claim(false) ) 
    { initialized = false ; return initialized ; }

  // Set the mode to COMPAT (SPP) 
  if ( ! SetMode ( ParallelPortPPDevBodyC::COMPAT  ) )
    { initialized = false ; return initialized ; } 
  
  return initialized ; 
}; 


  //: Claim  
  // ------------------------------------------------------------------------------------
bool  ParallelPortPPDevBodyC::Claim (bool exclusive) 
{
  if ( exclusive ) {
    // make exclusive access to port 
    if ( ioctl(fd, PPEXCL) ) {
      Error ("Error: Could not get exclusivity") ; 
      return false ; }
  }
  
  // claim the port 
  if ( ioctl(fd, PPCLAIM) ) { 
    Error("Error: Could not claim port. ") ; 
    return false ; } 
  
  return true ; 
};



//: Yield
// ------------------------------------------------------------------------------------
bool  ParallelPortPPDevBodyC::Yield (void) 
{
  if ( ioctl(fd, PPYIELD) ) {
    Error ("Error: Could not yield") ; 
    return false ; 
  }
  return true ; 
}


//: Release
// ------------------------------------------------------------------------------------
bool  ParallelPortPPDevBodyC::Release (void) 
{
  if ( ioctl(fd, PPRELEASE) ) {
    Error ("Error: Could not release") ; 
    return false ; 
  }
  return true ; 
}



//: Release
// ------------------------------------------------------------------------------------
ParallelPortPPDevBodyC::PPDevModeT  ParallelPortPPDevBodyC::Negotiate (void) 
{
  ParallelPortPPDevBodyC::PPDevModeT mode ; 
  if ( ioctl ( fd, PPNEGOT, &mode ) ) {
    Error ("Error: Could not negotiate a port mode") ; 
    return ParallelPortPPDevBodyC::NONE ; 
  }
 cout << "\n\n mode negotiated is " << mode ; 
 return mode  ; 
}; 


//: Set Mode 
// ------------------------------------------------------------------------------------
bool ParallelPortPPDevBodyC::SetMode (ParallelPortPPDevBodyC::PPDevModeT mode) 
{
  if (ioctl(fd,PPSETMODE,&mode) )
    { Error ("Error: Could not set mode") ; return false ; } 
  return true ; 
}


//: Get Mode 
// ------------------------------------------------------------------------------------
ParallelPortPPDevBodyC::PPDevModeT ParallelPortPPDevBodyC::GetMode (void) const
{
  ParallelPortPPDevBodyC::PPDevModeT mode (ParallelPortPPDevBodyC::NONE) ; 
  if (ioctl(fd,PPGETMODE, &mode) ) 
    { Error ("Error: Could not get mode") ; return mode ; } 
  return mode ;  
}

// ------------------------------------------------------------------------------------
bool ParallelPortPPDevBodyC::Write (ByteT data) 
{
  if (ioctl ( fd, PPWDATA, &data)) {
    Error("Error: Could not write data ") ; 
    return false ; }
  return true ; 
}


// Read from Data port 
// ------------------------------------------------------------------------------------
ByteT  ParallelPortPPDevBodyC::Read (void) 
{
  ByteT ret = 0;
  if ( ioctl( fd,PPRDATA, &ret) ) {
    Error("Error: Could not read data") ; 
    return ret ;}  
  return ret ;
  }


// Read Status Bits 
// ------------------------------------------------------------------------------------
ByteT ParallelPortPPDevBodyC::ReadStatus(void) 
{
  ByteT ret = 0 ; 
  if ( ioctl ( fd, PPRSTATUS, &ret ) ) {
    Error ("Error: Could not read status " ) ; 
   return ret ; }
   return ret ; 
}



// Write to data port 
// ------------------------------------------------------------------------------------
bool ParallelPortPPDevBodyC::WriteCtrl( ByteT data) 
{  
  if (ioctl ( fd, PPWCONTROL, &data)) {
    Error("Error: Failed to write to control port") ; return false ; }
  return true ; 
}


// Set timeout for read and write calls. 
//: ------------------------------------------------------------------------------------
bool ParallelPortPPDevBodyC::Timeout (const RealT & timeout ) 
{ 
  // convert real value to timeval 
  timeval time ; 
  unsigned long int 
    sec = (unsigned long int) timeout , 
    usec = (unsigned long int) ((timeout - sec) * 1000000) ; 
  time.tv_sec = sec; 
  time.tv_usec = usec ; 
 
  //  set the timeout 
  if ( ioctl (fd, PPSETTIME, &time ) ) {
   Error ("Error: Failed set IO timeout") ; 
   return false ; 
  }
  return true;
}



// Query timeout 
//: ------------------------------------------------------------------------------------
RealT ParallelPortPPDevBodyC::Timeout(void) const
{
timeval time ; 
 if ( ioctl (fd, PPGETTIME, &time ) ) {
   Error ("Error: Failed to get IO timeout") ; 
   return -1 ; 
 }
 RealT sec = time.tv_sec ; 
 RealT usec = time.tv_usec ; 
 RealT ret = sec + usec/1000000 ; 
   return ret ;
}


//: Driver On 
//: ------------------------------------------------------------------------------------
bool ParallelPortPPDevBodyC::DriverOn (bool status) 
{
  bool notstat = !status ; // 1 = off 
  if ( ioctl(fd, PPDATADIR, &notstat )) 
    { Error ("Errror: Failed to enable port driver") ; return false  ; }
  return true ; 
}


//: ClearIRQCount
//: ------------------------------------------------------------------------------------
IntT ParallelPortPPDevBodyC::ClearIRQCount (void) 
{
IntT count = -1; 
 if ( ioctl(fd, PPCLRIRQ, &count) ) 
   Error ("Error: Could not clear/get IRQ count ")  ;
 return count ;
}
  

//: TriggerResponse
//: ------------------------------------------------------------------------------------
bool ParallelPortPPDevBodyC::TriggerResponse ( const ByteT & value ) 
{
  if ( ioctl( fd, PPWCTLONIRQ, &value) )
    { Error ("Error: Could not set Trigger Response") ; return false ; } 
  return true ; 
}


};
