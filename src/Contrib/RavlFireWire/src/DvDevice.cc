// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2002, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
////////////////////////////////////////////////////////////////////////////
//! author="Kieron J Messer"
//! lib=RavlFireWire
//! date="23/9/100"
//! rcsid="$Id$"
//! file="Ravl/Contrib/FireWire/DvDevice.cc"

//#define _LARGEFILE_SOURCE
//#define _FILE_OFFSET_BITS 64

#define DODEBUG 0
#if DODEBUG
#define ONDEBUG(x) x
#else 
#define ONDEBUG(x) 
#endif 

#include "Ravl/Image/DvDevice.hh"
#include "Ravl/Image/ImgIO.hh"
#include "Ravl/DP/FileFormatIO.hh"
#include "Ravl/Image/Deinterlace.hh"
#include "Ravl/Assert.hh"
#include "Ravl/OS/Date.hh"
//#include <netinet/in.h> 


namespace RavlImageN {

  int raw_iso_handler(raw1394handle_t handle, int channel, size_t length, quadlet_t *data)
  {
    PalFrameC *frame = (PalFrameC*)raw1394_get_userdata(handle); //: get user data
    if (length > 16) {
      unsigned char *p = (unsigned char*) & data[3];
      int section_type = p[0] >> 5;   /* section type is in bits 5 - 7 */
      int dif_sequence = p[1] >> 4;   /* dif sequence number is in bits 4 - 7 */
      int dif_block = p[2];
      frame->writeDifBlock(section_type, dif_sequence, dif_block, length, p);
    }
    return 0;
  }
  
  
  DvDeviceC::DvDeviceC()
{
  ONDEBUG (cerr << "\nDvDeviceC::DvDeviceC()" ) ; 
  device = -1;
  
#ifdef RAW1394_V_0_8
  handle = raw1394_get_handle();
#else
  handle = raw1394_new_handle();
#endif
  
  //: Check the status 
  if (!handle) {
    if (!errno)
      RavlAlwaysAssertMsg(0,"\nNot Compatible! \n")
    else 
      RavlAlwaysAssertMsg(0,"\nCouldn't get 1394 handle, Is ieee1394, driver, and raw1394 loaded?\n") ; 
  }

  if (raw1394_set_port(handle, 0) < 0) {
    raw1394_destroy_handle(handle);
    RavlAlwaysAssertMsg(0, "Couldn't set port" ) ; 
    exit(1);
  }
  
  for (IntT i=0; i < raw1394_get_nodecount(handle); ++i) {
    if (rom1394_get_directory(handle, i, &rom_dir) < 0) {
      RavlIssueWarning("Error reading config rom directory for node " + i);
      raw1394_destroy_handle(handle);
      exit(1); 
    }
    
    if ( (rom1394_get_node_type(&rom_dir) == ROM1394_NODE_TYPE_AVC) &&
	 avc1394_check_subunit_type(handle, i, AVC1394_SUBUNIT_TYPE_VCR)) {
      device = i;
      break;
    }}
  
  
  if (device == -1)
    {
      RavlIssueError("Could not find any AV/C devices on the 1394 bus.\n");
      raw1394_destroy_handle(handle);
      exit(1);
    }
  
  // lets set the ISO receiver
  raw1394_set_iso_handler(handle, 63, raw_iso_handler);
  raw1394_set_userdata(handle, (PalFrameC*)&frame);
}




//: OK a few check state routines
bool DvDeviceC::IsPlaying() const
{
  ONDEBUG (cerr << "\nDvDeviceC::IsPlaying() const" ); 
  if(avc1394_vcr_is_playing(handle, device)) return true;
  return false;
}


//: Is the device recording. 
bool DvDeviceC::IsRecording() const
{
  ONDEBUG (cerr << "\nDvDeviceC::IsRecording() const" ); 
  if(avc1394_vcr_is_recording(handle, device)) return true;
  return false;
}


//: Is the device playing.
void DvDeviceC::Play() const
{
  ONDEBUG (cerr << "\nDvDeviceC::Play() const" ); 
  avc1394_vcr_play(handle, device);
  return;
}

//: Go to next frame ; 
void DvDeviceC::NextFrame() const
{
  ONDEBUG (cerr << "\nDvDeviceC::NextFrame() const" ); 
  avc1394_vcr_next(handle, device);
  return;
}


//: Trick play 
//: seek at given speed ; 
void DvDeviceC::TrickPlay(const int speed) const
{
  ONDEBUG (cerr << "\nDvDeviceC::TrickPlay(const int speed) const :: speed = " << speed ); 
  avc1394_vcr_trick_play(handle, device, speed);
  return;
}


void
DvDeviceC::Pause() const
{
 ONDEBUG (cerr << "\nDvDeviceC::Pause() const" ); 
 avc1394_vcr_pause(handle, device);
  return;
}



void
DvDeviceC::Stop() const
{
 ONDEBUG (cerr << "\nDvDeviceC::Stop() const" ); 
 avc1394_vcr_stop(handle, device);
  return;
}

void
DvDeviceC::Rewind() const
{
  ONDEBUG (cerr << "\nDvDeviceC::Rewind() const" ); 
  avc1394_vcr_rewind(handle, device);
  return;
}

void
DvDeviceC::ForwardWind() const
{
  ONDEBUG (cerr << "\nDvDeviceC::ForwardWind() const" ); 
  avc1394_vcr_forward(handle, device);
  return;
}

       
TimeCodeC DvDeviceC::GetTimeCode() const
{
  char *tc =  avc1394_vcr_get_timecode(handle, device);
  TimeCodeC myTc(tc);
  ONDEBUG (cerr << "\nDvDeviceC::GetTimeCode() const \t Timecode:" << myTc.ToText()  ); 
  return myTc;
}






 
 bool DvDeviceC::GotoTimeCodeFwd (const TimeCodeC & tcTarget) const 
{
  ONDEBUG (cerr << "\nDvDeviceC::GotoTimeCode(const TimeCodeC &tcTarget) const" ); 
  bool forward = true ;            // seek direction 
  IntT framesToGo = 0 ;            // number of frames to target. 
  IntT speed = 0 ;                 // current speed, 15 is forward and rewind. 
  Pause() ; 
  RealT sleepTime = 0.1 ; 
  Sleep(sleepTime) ; 
  
  // find number of frames to target, and direction. 
  framesToGo = (tcTarget.getFrameCount() - GetTimeCode().getFrameCount()) ; 

  cout << "\n\n Status " << Status() ; 
  if ( framesToGo >= 0 ) forward = true ; else forward = false ; 
  framesToGo = Abs(framesToGo) ;
  ONDEBUG ( cerr << "\nFrames to target = " << framesToGo << "\t direction:" << forward ) ;   
  ONDEBUG ( cerr << "\nCurrent Timecode is " << GetTimeCode().ToText())  ;
  if ( ! forward ) cerr << "\n Timecode seems to have passed ...." ; 

  
  // now seek to timecode 
  while (true) {
    ONDEBUG ( cerr << "\n\nFrames to go is " << framesToGo << "\t direction:" << forward << "\t timecode: " << GetTimeCode().ToText()  ) ;  
    
    // get offset data again. 
    framesToGo = (tcTarget.getFrameCount() - GetTimeCode().getFrameCount()) ; 
    if ( framesToGo >= 0 ) forward = true ; else forward = false ; 
    framesToGo = Abs(framesToGo) ; 
  

    // We are there ! 
    if ( framesToGo == 0 ) {
      ONDEBUG ( cerr << "\n\n\n\nNo more frames to go !" ) ; 
      Pause() ;
      return true ; // we are where we need to be. 
    }

    // if we hit end of tape without finding it, return false 
    if ( Status() =="Winding stopped" ) {
      cerr << "\nEnd of tape encountered, timecode not found" << Status() ; 
      return false ; }

    // We are >1000  frames away still 
    if ( framesToGo >= 1000 ) {
      const IntT thisSpeed = 14 ;
      ONDEBUG ( cerr << "\n Target is >1000 frames away" ) ; 
      if ( speed != thisSpeed ) {
	speed = thisSpeed ; 
	TrickPlay(thisSpeed) ;
      }
      Sleep(sleepTime) ; 
      continue ; 
    }
    // between 250 and 1000 frames 
    else if (( framesToGo >= 250 ) && framesToGo < 1000 ) {
      const IntT thisSpeed = 12 ; 
      ONDEBUG ( cerr << "\n Target is 250-1000 frames away" ) ; 
      if ( speed != thisSpeed ) {
	speed = thisSpeed ; 
	TrickPlay(thisSpeed) ; 
      }
      Sleep(sleepTime) ; 
      continue ; 
    }
    // between 50 and 250 frames ; 
    else if (( framesToGo >=50 ) && framesToGo < 250 ) {
      const IntT thisSpeed = 8 ; 
      ONDEBUG ( cerr << "\n Target is 5-250 frames away" ) ; 
      if ( speed != thisSpeed ) {
	speed = thisSpeed ; 
	TrickPlay(thisSpeed) ; 
      }
      Sleep(sleepTime) ; 
      continue ; 
    }
  // between 10 and 50 frames ; 
    else if (( framesToGo >=10 ) && framesToGo < 50 ) {
      const IntT thisSpeed = 6 ; 
      ONDEBUG ( cerr << "\n Target is 10-50 frames away" ) ; 
      if ( speed != thisSpeed ) {
	speed = thisSpeed ; 
	TrickPlay(thisSpeed) ; 
      }
      Sleep(sleepTime) ; 
      continue ; 
    }
    // less that 10 frames
    else {
      const IntT thisSpeed = 1 ; 
      ONDEBUG ( cerr << "\n Target is <10 frames away" ) ; 
      if ( speed != thisSpeed ) {
	speed = thisSpeed ; 
	TrickPlay(thisSpeed) ;
      }
      Sleep(sleepTime) ;
      continue ; 
    }
    
  }
  ONDEBUG ( cerr << "\n End of timecode seek, current tc is " << GetTimeCode().ToText() ) ; 
  ONDEBUG ( cerr << "\n\ncurrent timecode is " << GetTimeCode().ToText() ) ; 
  return false ; 
}






StringC DvDeviceC::Status() const 
{
  quadlet_t  quad =avc1394_vcr_status(handle, device);
  StringC status(avc1394_vcr_decode_status(quad)); 
  ONDEBUG (cerr << "\nDvDeviceC::Status() const \t status=\"" << status << "\""  );
  return status;
}


TimeCodeC DvDeviceC::GrabFrame() 
{
  ONDEBUG (cerr << "\nDvDeviceC::grabFrame() " ); 
  int channel=63;
  //: start ISO receive
  if (raw1394_start_iso_rcv(handle, channel) < 0)
    RavlIssueError("raw1394 - couldn't start iso receive") ; 
  
  bool done=false;
  TimeCodeC ret;
  while(!done) {    
    raw1394_loop_iterate(handle);
    if(frame.isValid()) {
      done=true;
      ret= frame.extractTimeCode();
      done = true;
    }
  }
  raw1394_stop_iso_rcv(handle, channel);
  
  return ret;
}


ImageC<ByteRGBValueC>
DvDeviceC::GrabImage() 
{
  ONDEBUG (cerr << "\nDvDeviceC::grabImage() " ); 
  int channel=63;

  //: start ISO receive
  if (raw1394_start_iso_rcv(handle, channel) < 0) {
    cerr << "raw1394 - couldn't start iso receive" << endl;
    exit( -1);
  }

  bool done=false;
  while(!done) {    
    raw1394_loop_iterate(handle);
    if(frame.isValid()) {
      //TimeCodeC tcFrame = frame.extractTimeCode();
      //cout << "grabbed frame: " << tcFrame << endl;
      done = true;
    }
  }
  raw1394_stop_iso_rcv(handle, channel);

  return frame.Image();
}

ImageC<ByteRGBValueC>
DvDeviceC::GrabFrame(const TimeCodeC & tcGrab) 
{
  ONDEBUG (cerr << "\nDvDeviceC::grabFrame(const TimeCodeC & tcGrab) " ); 
  int channel=63;
  

  // rewind tape
  Stop() ; 
  Rewind() ; 
  Sleep(0.5) ;   
  while ( Status() != "Winding stopped" )
    Sleep(0.5) ; 
  
  // Search fwd for timecode 
  GotoTimeCodeFwd(tcGrab);

  //: start ISO receive
  if (raw1394_start_iso_rcv(handle, channel) < 0) {
    cerr << "raw1394 - couldn't start iso receive" << endl;
    exit( -1);
  }

  bool done=false;
  while(!done) {    
    raw1394_loop_iterate(handle);
    if(frame.isValid()) {
      // TimeCodeC tcFrame = frame.extractTimeCode();
      //cout << "grabbed frame: " << tcFrame << endl;
      done = true;
    }
  }
  raw1394_stop_iso_rcv(handle, channel);

  return frame.Image();
}


bool
DvDeviceC::GrabSequence(const char * filename, const TimeCodeC & tcStart, const TimeCodeC & tcEnd) 
{
  ONDEBUG (cerr << "\nDvDeviceC::grabSequence(const char * filename, const TimeCodeC & tcStart, const TimeCodeC & tcEnd) " ); 
  //: goto the first timecode
  TimeCodeC zero(0,0,0,0);
  TimeCodeC offset(0,0,3,0); // three seconds
  TimeCodeC realStart = tcStart-offset;
  if(realStart<zero)realStart=zero;
  bool dropped = false;
  //: first goto timecode
  Stop() ; 
  Rewind() ; 
  Sleep(0.5) ;   
  while ( Status() != "Winding stopped" )
    Sleep(0.5) ; 
  GotoTimeCodeFwd(realStart);
  //: open the file for the video data
  
  //int fd = open (filename, O_LARGEFILE, "wb") ; 
  //FILE *fp = fdopen(fd,"w");

  FILE *fp = fopen(filename, "wb");
  int channel = 63;

  
  //: start ISO receive
  if (raw1394_start_iso_rcv(handle, channel) < 0) {
    cerr << "raw1394 - couldn't start iso receive" << endl;
    exit( -1);
  }
  
  int frames=0;
  bool done=false;
  //: OK lets go for it
  Play(); // lets start play
  TimeCodeC tcNext(tcStart);
  bool capture=false;
  while(!done) {    
    raw1394_loop_iterate(handle);
    
    if(frame.isValid()) {
      TimeCodeC tcFrame = frame.extractTimeCode();
      
      if(!capture) if(tcFrame>=tcStart) capture=true; //: look to start capturing
      
      if(capture) {
	if(tcFrame<tcNext) {
	  // cerr << "double frame..not saving" << endl;
	} else {
	  if(tcFrame>tcNext) {
	    cerr << "dropped frame" << tcNext << endl;
	    dropped = true;
	  } 
	  
	  fwrite(frame.getData(), 144000, sizeof(ByteT), fp);
	  frames++;
	  //frame.Decode();
	  if(tcFrame >= tcEnd) done=true;
	}
      }
      tcNext = tcFrame;
      tcNext+=1;
    }
    
  }
  
  fclose(fp);
  raw1394_stop_iso_rcv(handle, channel);
  //  cout << "grabbed frames: " << frames << endl;
  
  return dropped;
  
}

bool DvDeviceC::GrabWav(const char * filename, const TimeCodeC & tcStart, const TimeCodeC & tcEnd) 
{
  ONDEBUG (cerr << "\nDvDeviceC::grabWav(const char * filename, const TimeCodeC & tcStart, const TimeCodeC & tcEnd) " ); 
  //: goto the first timecode
  TimeCodeC zero(0,0,0,0);
  TimeCodeC offset(0,0,3,0); // three seconds
  TimeCodeC realStart = tcStart-offset;
  if(realStart<zero)realStart=zero;
  bool dropped = false;
  //: first goto timecode
  // rewind tape
  Stop() ; 
  Rewind() ; 
  Sleep(0.5) ;   
  while ( Status() != "Winding stopped" )
    Sleep(0.5) ; 
  GotoTimeCodeFwd(realStart);
  
//: open the file for the video data
  int channel = 63;
  WavFileC audio;

  //: start ISO receive
  if (raw1394_start_iso_rcv(handle, channel) < 0) {
    cerr << "raw1394 - couldn't start iso receive" << endl;
    exit( -1);
  }
  

  int frames=0;
  bool done=false;
  //: OK lets go for it
  Play(); // lets start play
  TimeCodeC tcNext(tcStart);
  bool capture=false;
  bool firstframe=true;
  while(!done) {    
    raw1394_loop_iterate(handle);
    
    if(frame.isValid()) {
      TimeCodeC tcFrame = frame.extractTimeCode();
      
      if(firstframe) {
	// lets set audio properties
	UIntT bitsPerSample = frame.Decoder()->audio->quantization;
	UIntT sampleFrequency = frame.Decoder()->audio->frequency;
	UIntT numChannels =  frame.Decoder()->audio->num_channels;
	cout << sampleFrequency << " Hz" << endl;
	cout << bitsPerSample << " bits" << endl;
	cout << numChannels << " channels" << endl;
	FilenameC fn(filename);
	audio = WavFileC(fn, bitsPerSample, sampleFrequency, numChannels); 
	firstframe=false;
      }

      if(!capture) if(tcFrame>=tcStart) capture=true; //: look to start capturing
      
      if(capture) {

	if(tcFrame<tcNext) {
	  // cerr << "double frame..not saving" << endl;
	} else {
	  if(tcFrame>tcNext) {
	    cerr << "dropped frame" << tcNext << endl;
	    dropped = true;
	  } 
	  
	  audio.write(frame.Sound());
	  frames++;
	  //frame.Decode();
	  if(tcFrame >= tcEnd) done=true;
	}
      }
      tcNext = tcFrame;
      tcNext+=1;
    }
    
  }
  
  audio.Close();
  raw1394_stop_iso_rcv(handle, channel);
  //  cout << "grabbed frames: " << frames << endl;
  
  return dropped;
  
}


bool
DvDeviceC::GrabImageSequence(const StringC & prefix, const TimeCodeC & tcStart, const TimeCodeC & tcEnd, UIntT nFrames) 
{
  ONDEBUG (cerr << "\nDvDeviceC::grabImageSequence(const StringC & prefix, const TimeCodeC & tcStart, const TimeCodeC & tcEnd, UIntT nFrames) " ); 
  //: goto the first timecode
  TimeCodeC zero(0,0,0,0);
  TimeCodeC offset(0,0,3,0); // three seconds
  TimeCodeC realStart = tcStart-offset;
  if(realStart<zero)realStart=zero;
  bool dropped = false;
  //: first goto timecode
  // rewind tape
  Stop() ; 
  Rewind() ; 
  Sleep(0.5) ;   
  while ( Status() != "Winding stopped" )
    Sleep(0.5) ; 
  GotoTimeCodeFwd(realStart);
  //: open the file for the video data
  int channel = 63;

  
  //: start ISO receive
  if (raw1394_start_iso_rcv(handle, channel) < 0) {
    cerr << "raw1394 - couldn't start iso receive" << endl;
    exit( -1);
  }
  
  bool done=false;
  //: OK lets go for it
  Play(); // lets start play


  TimeCodeC tcNext(tcStart);
  TimeCodeC tcStep(nFrames, 25.0);

  while(!done) {    
    raw1394_loop_iterate(handle);
    
    if(frame.isValid()) {
      TimeCodeC tcFrame = frame.extractTimeCode();
      
      if(tcFrame==tcNext) {
	StringC filename = prefix + tcFrame.ToText() + ".ppm";
	cout << "saved: " << filename << endl << flush;
	ImageC<ByteRGBValueC>im = frame.Image().Copy();
	im = DeinterlaceSubsample(im);
	Save(filename, im);
	tcNext += tcStep;
	if(tcNext>tcEnd) done = true;
      }
    }
    
  }
  
  raw1394_stop_iso_rcv(handle, channel);
  
  return dropped;  
}



} // end namespace
 
