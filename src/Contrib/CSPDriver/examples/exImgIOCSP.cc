// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//! rcsid="$Id$"
//! lib=CSPDriver
//! file="Ravl/Contrib/ClipStationPro/exImgIOCSP.cc"
//! userlevel=Normal
//! author="Lee Gregory"
//! docentry="Ravl.API.Images.Video.Video IO.ClipStationPro"

#include "Ravl/Image/ImgIOCSP.hh" 
#include "Ravl/DP/SequenceIO.hh"
#include "Ravl/Option.hh"
#include "Ravl/OS/Date.hh"
#include "Ravl/Image/DrawFrame.hh"
#include "Ravl/Random.hh"


using namespace RavlN ; 
using namespace RavlImageN ; 


int main (int argc, char ** argv ) 
{

  // Get Some options 
  OptionC opts(argc,argv) ; 
  StringC device = opts.String("d", "@CSP:PCI,card:0", "The pci device id (PCI,card:0) or (PCI,card:1) ") ; 
  opts.Check() ; 


  // open the device as a port for output - Save function can be used instead for single images  
  DPOPortC< ImageC<ByteYUV422ValueC> > output ; 
  if ( ! OpenOSequence( output, device, "", true ) ) 
    { cerr << "failed to open device " << device << " for output" ; exit(1) ; } 


  // open the device as a port for input - Load function can be used instead for single images
  DPIPortC< ImageC<ByteYUV422ValueC> > input ; 
  if ( !OpenISequence( input, device, "", true) )
    { cerr << "failed to open device: " << device << " for input "  ; exit(1) ; }

    
/*
  cout << "\n\n Attributes are " ; 	
  DListC<AttributeTypeC> attrOut, attrIn ; 
  output.GetAttrTypes(attrOut) ; 
  input.GetAttrTypes(attrIn) ; 
  cout << "\n\n" << attrOut << "\n\n" << attrIn ; 
*/

  //DListC<StringC> strOut, strIn ; 
  //output.GetAttrList(strOut) ; 
  //input.GetAttrList(strIn) ; 
  //cout << "\n\n" << strOut << "\n\n" << strIn ; 

	
  // This is how we set various diagnostic modes on the output 
  // ----------------------------------------------
  
/*	 	
	// colourbars 
  cout << "\n\n\n colourbars mode " ;	
  output.SetAttr("COLOURBAR","1") ;  
  Sleep(5) ; 

  // Black 
  cout << "\n\n\n set raster black " ;
  output.SetAttr("BLACK","1") ; 
  Sleep(5) ; 

	// Live
  cout << "\n\n\n Set raster to live mode " ; 
  output.SetAttr("LIVE", "1") ; 
  Sleep(5) ; 
  */
  
  // This is a small example of how to grab images, with timecodes if available,
  // Edit them and output them to the grabber card.
  // We produce a randomly moving rectanble on the grabber output ; 

  Index2dC pos (300,300) ;                              // starting point for the rectangle  
  ByteYUV422ValueC colour (0,0) ;                       // colour of the rectangle 
  for ( IntT x = 0 ; x < 1000 ; x ++ ) 
	{
	  ImageC<ByteYUV422ValueC> img = input.Get() ;  // grab the image 
	  TimeCodeC tc = input.GetAttr("timecode") ;    // grab the timecode 
	  cout << "\n" << tc.ToText() ;                 // output the timecode 
	  
	  RavlN::Save( "test.ppm", img) ; 
	  /*
	  // randomly perturb the position of the rectangle
	  pos += Index2dC( Random1() * 10 - 5.0 , Random1()*10 - 5.0 ) ;
	  if ( ! pos.IsInside( img.Rectangle() ) ) // move it back to center if it falls off the edge
	    pos = Index2dC(300,300) ; 

	  // Draw the rectnagle on the image and display it back to the o/p. 
	  IndexRange2dC rect ( pos, 30 ) ; 
	  DrawFrame ( img, colour, rect, true ) ; 
	  output.Put(img) ; 				
	*/
	}

// Notes 
// Putting ravl images to the output of the grabber card in this way is expensive, because the images have to be copied into a suitable 
// dma buffer prior to output. If more speed is required, then see the CSPControlC class, which allows access to the raw frame buffers. 
return 0 ;
}



