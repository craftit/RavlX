// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//! rcsid="$Id$"
//! lib=CSPDriver
//! file="Ravl/Contrib/ClipStationPro/exCSPControl.cc"
//! userlevel=Normal
//! author="Lee Gregory"
//! docentry="Ravl.API.Images.Video.Video IO.ClipStationPro"
#include "Ravl/OS/Date.hh"
#include "Ravl/EntryPnt.hh"
#include "Ravl/Image/CSPControl.hh"


using namespace RavlN ; 
using namespace RavlImageN ; 

int mainFunc (int argc, char** argv) 
{

	// setup the device 
StringC device = "PCI,card:0" ; 	
ClipStationProDeviceC dev ( device) ; 

	// generate the colourbars for 5 seconds 
 cerr << "\n Generating Colourbars " ; 
dev.SetMode(COLOURBAR) ; 
sleep(5) ; 

	// grab some frames and play them out. 
 cerr << "\n display 125 frames " ; 
 for (IntT i = 0 ; i < 125 ; i ++ ) 
   {
      dev.PutFrame( dev.GetFrame() )  ; 
   } 

 // generate the colourbars for 5
 cerr << "\n Generating Colourbars " ; 
 dev.SetMode(COLOURBAR) ; 
 sleep(5) ; 

 // set raster to black   
 cerr << "\n Black Mode" ; 
 dev.SetMode(BLACK) ; 
 sleep(5) ; 
   
 // set raster to live mode  
 cerr << "\n Live Mode " ; 
 dev.SetMode(LIVE) ; 
 sleep(5) ; 


// finaly grab some more frames 
 cerr << "\n display 125 frames " ; 
 for (IntT i = 0 ; i < 125 ; i ++ ) 
   {
      dev.PutFrame( dev.GetFrame() )  ; 
   } 

 
// display colourbars    
 cerr << "\n Generating Colourbars " ; 
 dev.SetMode(COLOURBAR) ; 
 sleep(5) ; 


return 0 ; 
}


RAVL_ENTRY_POINT(mainFunc) ;  
