// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2002, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
////////////////////////////////////////////////////////
//! rcsid="$Id$"
//! lib=RavlFireWire
//! file="Ravl/Contrib/FireWire/doDvGrab.cc"
//! docentry="Ravl.API.Images.Video.Video IO.DV;Ravl.Applications.Video"
//! userlevel=Normal

//: Grabs video sequences + audio from a Sony DV VCR
//
// <p>This program reads video + audio from a DV tape and dumps it to a .dv file (unless the -ppm option is used).  Type "doDvGrab -help" for more help.</p>
// <p><font color=red>Warning</font>: the tape player needs to pre-roll the tape by a few (~3) seconds, so do not try to grab right from the beginning of the tape.</p>

#include "Ravl/Option.hh"
#include "Ravl/OS/Filename.hh"
#include "Ravl/EntryPnt.hh"
#include "Ravl/Image/DvDevice.hh"
#include "Ravl/DP/FileFormatIO.hh"


#include "Ravl/OS/Date.hh"
using namespace RavlImageN;



int doDvGrab(int argc, char **argv)
{  
  OptionC   opt(argc,argv);
   opt.Comment("This program reads video + audio from a DV tape and dumps it to a .dv file (unless the -ppm option is used)");
   opt.Comment("\n**Warnings**:\n (1) The tape player needs to pre-roll the tape by a few (~3) seconds, so do not try to grab right from the beginning of the tape.\n(2) To avoid dropped frames, it is highly recommended that the output is written to a file system on the local machine\n\n");
   FilenameC     OutFile = opt.String("o", "grab.dv", "output file");
   StringC       PpmImage = opt.String("ppm", "00:00:10:00", "Grab a single frame at given timecode");
   StringC       tcStart = opt.String("start", "00:00:10:00", "start timecode");
   StringC       tcEnd   = opt.String("end", "00:00:00:00", "end timecode");
   UIntT         Step    = opt.Int("step", 0, "if set then we grab every step frames to ppm stream");
   bool          Audio   = opt.Boolean("audio", false, "just grab the audio to a wav file");
   //opt.Compulsory("o");
   opt.Check();
   


   DvDeviceC dev;
   
  //: Just grab a single image
   if(opt.IsOnCommandLine("ppm")) {
     ImageC<ByteRGBValueC>im = dev.GrabFrame((TimeCodeC)PpmImage);
     Save(PpmImage, im, "", true);
   }
   //: Lets grab a whole sequence
   else {
     if(!dev.IsPlaying()) {
       dev.Pause();
     }
     //: grab streaming data
     if(Audio) {
       dev.GrabWav(OutFile, (TimeCodeC)tcStart, (TimeCodeC)tcEnd);
     } 
     else {
       if(opt.IsOnCommandLine("step")) {
	 dev.GrabImageSequence(OutFile, (TimeCodeC)tcStart, (TimeCodeC)tcEnd, Step);
       } else {
	 dev.GrabSequence(OutFile, (TimeCodeC)tcStart, (TimeCodeC)tcEnd);
       } 
      dev.Pause();
     }
   }
   dev.Stop() ; 
   return 0;
}

//: This puts a wrapper around the main program that catches
//: exceptions and turns them into readable error messages.


RAVL_ENTRY_POINT(doDvGrab);
