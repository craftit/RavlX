// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2004, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef _RAVL_PARPORT_HH_ 
#define _RAVL_PARPORT_HH_
#include "Ravl/RefCounter.hh"
#include <linux/parport.h> 
#include "Ravl/OS/Filename.hh"
//! rcsid="$Id$"
//! file="Ravl/Contrib/ParallelPortPPDev/Parport.hh"

/////////////////////////////////////////////////////
//! docentry="Ravl.API.OS.Hardware"
//! lib=PPDev
//! userlevel=Normal
//! author="Lee Gregory"
//! date="11/05/2003"

namespace RavlN {


  class ParallelPortPPDevC ;


  //! userlevel=Develop
  // --------------------------------------------------------------------------------------------------------- // 
  // ----------------------------------------- ParallelPortPPDevBodyC ------------------------------------------------ //
  // --------------------------------------------------------------------------------------------------------- // 
  //: PPDev Parallel port body 
  // This class provides access to the parallel port <p> 
  // The port is treated as 3 individual ports, data port, status port, and control port <p> 
  // Bidirectional modes allow data port to work both ways. <p> 
  // 'frobbing' is not yet supported, neither are select, poll type operations.
  //!classbugs:  It may be necessary to disable the port when hardware writes directly to the port, othwise port may be DAMAGED !


  class ParallelPortPPDevBodyC  : public RCBodyC
{
 public: 

 enum PPDevModeT { 
    COMPAT = IEEE1284_MODE_COMPAT   ,  // SPP mode 
    NIBBLE = IEEE1284_MODE_BYTE  ,
    BYTE = IEEE1284_MODE_BYTE ,    
    ECP = IEEE1284_MODE_ECP,    
    EPP = IEEE1284_MODE_EPP ,
    BECP = IEEE1284_MODE_BECP ,
    ECPRLE = IEEE1284_MODE_ECPRLE,
    ECPSWE = IEEE1284_MODE_EPPSWE  ,
    EPPSL = IEEE1284_MODE_EPPSL ,
    EPPSWE = IEEE1284_MODE_EPPSWE,
    DEVICEID = IEEE1284_DEVICEID ,
    EXT = IEEE1284_EXT_LINK ,
    NONE
  };
  //: Enumerators for parallel port modes. Compat (SPP) is the default. 


  //typedef ParallelPortPPDevC::ParallelPortPPDevModeT ParallelPortPPDevModeT ;  
  
  //:-----------------------------------------------------------
  //: Setup Methods

  ParallelPortPPDevBodyC(const FilenameC & device) ; 
  //: Construct a parport device 

  bool Initialize (void) ; 
  //: Initialize the device - returns true on success
  //: This opens the device, claims it, and sets to COMPAT mode by default  
  
  bool Claim (bool exclusive ) ;  
  //: Claim access to the serial port - returns true on sucess 
  // exclusive prevents other drivers from using the port and may not succeed

  bool Yield ( void ) ;  
  //: Yield control of the port 
  // Other drivers may use the port, when finished, it is immediately reclaimed
  // returns true on success

  bool Release (void) ; 
  //: Releases control of the port. 
  // returns true on success 

  ParallelPortPPDevBodyC::PPDevModeT  Negotiate(void) ;
    //: Perform IEEE1284 Negotiation into a particular mode. 

  bool SetMode(ParallelPortPPDevBodyC::PPDevModeT mode ) ; 
  //: Set the IEEE1284 mode  to use for Read and Write calls. 

  ParallelPortPPDevBodyC::PPDevModeT  GetMode (void ) const ;
    //:  Get the IEEE1284 port mode 
  
  RealT Timeout(void) const ; 
  //: Access to the timeout  for read and write calls 

 bool Timeout(const RealT & timeout) ; 
  //: Set the timout for read and write calls 


  //: ---------------------------------------------------------------
  //: Read / Write Methods   

  bool Write (ByteT data) ; 
  //: Write to data port ; 

  ByteT  Read (void) ; 
  //: Read from data port 
  //: Beware, when driving this port with hardware, port drivers may need to be disabled
  
 bool DriverOn (bool) ; 
  //: Set State of Low Level Port Drivers - IO direction
  // This is usefull when the peripherial wishes to drive the signals, and may prevent port damage. 
  // Used in conjunction with Read and Write

  ByteT ReadCtrl (void) ; 
  //: Read the control lines ( strobe, autofd, select, init )  
  // This reads from the register containing the last values set, it does not read from hardware directly

  bool WriteCtrl (ByteT data) ; 
  //: Write to the control lines ( strobe, autofd, select, init ) 
  // OUTPUT             BIT                VALUE 
  // STROBE              1                     1
  // AUTOFD             2                     2 
  // INIT                     3                      4 
  // SELECT                4                     8

  void FrobCtrl (void) 
    { RavlAssert(0) ; }
  //!bug:  Not implemented 

  ByteT ReadStatus (void) ; 
  //: Read from the status port 

  IntT ClearIRQCount (void) ; 
  //: Return the interrupt count and reset counter to zero
  // In the unlikely event that an error occurs, -1 is returned

  bool TriggerResponse ( const ByteT & value ) ; 
  //: Set a trigger response
  // When an interrupt is received, the control lines are set with value
  // The purpose of this is speed. There is no context switch from kernel to user space.
  

  // -----------------------------------------------
  //: Misc functions 
  
  inline const IntT &  FD (void) const { return fd ; } 
  //:  Access the file descriptor 

  inline const FilenameC & Filename (void) const { return filename ; } 
  //: Access the file name 

  inline const bool & IsInit(void) const { return initialized ; } 
  //: Access the initialized flag

 protected: 

  //: -----------------------------------------------
  //: Internal Methods 
  

  void Error (const StringC & message) const ; 
  //: Helper function to handle library errors 
  /*
  int FromMode ( ParallelPortPPDevBodyC::PPDevModeT mode ) ;  
  //:  Convert from ParallelPortPPDevModeT to ppdev mode

  ParallelPortPPDevBodyC::PPDevModeT ToMode ( int ) ; 
  //: Convert from ppdev mode to ParallelPortPPDevModeT 
*/

    protected: 
  
  IntT fd ; 
  //: The file descriptor for the parport
  
  FilenameC filename ; 
  //: The name of the device ; 

  bool initialized ; 
  //: has the device been initialized ok  
  
}; 



//! userlevel=Normal

  // --------------------------------------------------------------------------------------------------------- // 
  // ----------------------------------------- ParallelPortPPDevC ----------------------------------------------------- //
  // --------------------------------------------------------------------------------------------------------- // 
  //: PPDev Parallel port handle 
  // This class provides access to the parallel port <p> 
  // The port is treated as 3 individual ports, data port, status port, and control port <p> 
  // Bidirectional modes allow data port to work both ways. <p> 
  // 'frobbing' is not yet supported, neither are select, poll type operations.
  //!classbugs:  It may be necessary to disable the port when hardware writes directly to the port, othwise port may be DAMAGED !
 

  class ParallelPortPPDevC  : public RCHandleC<ParallelPortPPDevBodyC> 
{
 public: 
  




  //:-----------------------------------------------------------
  //: Setup Methods

 //inline ParallelPortPPDevC(void) 
 // : RCHandleC<ParallelPortPPDevBodyC> () {}
 //: Create Invalid handle 
 
  inline ParallelPortPPDevC(const FilenameC & device) 
    : RCHandleC<ParallelPortPPDevBodyC> ( * new ParallelPortPPDevBodyC(device) ) {} 
  //: Construct a parport device 
  
  inline bool Initialize (void) 
    { return Body().Initialize() ; }  
  //: Initialize the device - returns true on success
  //: This opens the device, claims it, and sets to COMPAT mode by default  
  
  inline bool Claim (bool exclusive ) 
    { return Body().Claim(exclusive) ; }
  //: Claim access to the serial port - returns true on sucess 
  // exclusive prevents other drivers from using the port and may not succeed

  inline bool Yield ( void ) 
    { return Body().Yield() ; }  
  //: Yield control of the port 
  // Other drivers may use the port, when finished, it is immediately reclaimed
  // returns true on success

  inline bool Release (void) 
    { return Body().Release() ; } 
  //: Releases control of the port. 
  // returns true on success 

  inline ParallelPortPPDevBodyC::PPDevModeT  Negotiate(void) 
    { return Body().Negotiate() ; } 
  //: Perform IEEE1284 Negotiation into a particular mode. 
  

  inline bool SetMode(ParallelPortPPDevBodyC::PPDevModeT mode ) 
    { return Body().SetMode(mode) ; }
  //: Set the IEEE1284 mode  to use for Read and Write calls. 
  
  inline ParallelPortPPDevBodyC::PPDevModeT  GetMode (void ) const 
    { return Body().GetMode() ; } 
  //:  Get the IEEE1284 port mode 
  
  inline RealT Timeout(void) const 
    { return Body().Timeout() ; }
  //: Access to the timeout  for read and write calls 
  
  inline bool Timeout(const RealT & timeout) 
  { return Body().Timeout( timeout) ; } 
  //: Set the timout for read and write calls 
  
  
  //: ---------------------------------------------------------------
  //: Read / Write Methods   
  
  inline bool Write (ByteT data) 
    { return Body().Write( data ) ; } 
  //: Write to data port ; 
  
  inline ByteT  Read (void) 
    { return Body().Read() ; } 
  //: Read from data port 
  //: Beware, when driving this port with hardware, port drivers may need to be disabled
  
  inline  bool DriverOn (bool state) 
    { return Body().DriverOn(state) ; } 
  //: Set State of Low Level Port Drivers - IO direction
  // This is usefull when the peripherial wishes to drive the signals, and may prevent port damage. 
  // Used in conjunction with Read and Write
  
  inline ByteT ReadCtrl (void) 
    { return Body().ReadCtrl() ; } 
  //: Read the control lines ( strobe, autofd, select, init )  
  // This reads from the register containing the last values set, it does not read from hardware directly

  inline bool WriteCtrl (ByteT data) 
    { return Body().WriteCtrl(data) ; }  
  //: Write to the control lines ( strobe, autofd, select, init ) 
  // OUTPUT             BIT                VALUE 
  // STROBE              1                     1
  // AUTOFD             2                     2 
  // INIT                     3                      4 
  // SELECT                4                     8


  inline ByteT ReadStatus (void) 
  { return Body().ReadStatus() ; } 
  //: Read from the status port 

  inline IntT ClearIRQCount (void) 
    { return Body().ClearIRQCount() ; }  
  //: Return the interrupt count and reset counter to zero
  // In the unlikely event that an error occurs, -1 is returned

  bool TriggerResponse ( const ByteT & value ) ; 
  //: Set a trigger response
  // When an interrupt is received, the control lines are set with value
  // The purpose of this is speed. There is no context switch from kernel to user space.
  

  // -----------------------------------------------
  //: Misc functions 
  
  inline const IntT &  FD (void) const { return fd ; } 
  //:  Access the file descriptor 

  inline const FilenameC & Filename (void) const { return filename ; } 
  //: Access the file name 

  inline const bool & IsInit(void) const { return initialized ; } 
  //: Access the initialized flag

 protected: 

  //: -----------------------------------------------
  //: Internal Methods 
  

  void Error (const StringC & message) const ; 
  //: Helper function to handle library errors 
  /*
  int FromMode ( ParallelPortPPDevBodyC::PPDevModeT mode ) ;  
  //:  Convert from ParallelPortPPDevModeT to ppdev mode

  ParallelPortPPDevBodyC::PPDevModeT ToMode ( int ) ; 
  //: Convert from ppdev mode to ParallelPortPPDevModeT 
*/

    protected: 
  
  IntT fd ; 
  //: The file descriptor for the parport
  
  FilenameC filename ; 
  //: The name of the device ; 

  bool initialized ; 
  //: has the device been initialized ok  

  

 



};















};

#endif 
