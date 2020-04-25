// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
/////////////////////////////////////////////////////////////////
//! rcsid="$Id$"
//! lib=CSPDriver
//! file="Ravl/Contrib/ClipStationPro/CSPControl.cc"

#include "Ravl/Image/CSPControl.hh"
#include "Ravl/DList.hh"
#include "Ravl/Hash.hh"
#include "Ravl/DP/AttributeValueTypes.hh"
   #include "Ravl/Assert.hh"
#include "Ravl/Assert.hh"
#include <string.h>
#include <stdlib.h>

#define DODEBUG 0
#if DODEBUG
#define ONDEBUG(x) x
#else
#define ONDEBUG(x)
#endif



namespace RavlImageN {
  
  static HashC<StringC,ClipStationProDeviceBodyC *> devices(2) ;
  //: A hash table of device bodies by devname 
  
  
  //: getCSPBody
  //: A helper function to get a pointer to a body either by checking the hash table 
  //: or by creating a new instance
  //: ----------------------------------------------------------------------------------------------
  ClipStationProDeviceBodyC * getCSPBody (const StringC & devName, ColourModeT mode)
{
  if (devices.IsElm( devName ) ) 
    return devices[devName] ; 
  else 
    return new ClipStationProDeviceBodyC(devName, mode) ; 
}
  

  
  // Handle Constructor
  //: ----------------------------------------------------------------------------------------------
  ClipStationProDeviceC::ClipStationProDeviceC (const StringC & devName, ColourModeT mode ) 
    : RCHandleC<ClipStationProDeviceBodyC> ( getCSPBody(devName, mode) )  
{}
  
  
  
  //: Check return value of Low level sv functions
  //: ----------------------------------------------------------------------------------------------
  bool ClipStationProDeviceBodyC::CheckError ( int retVal, const char * errorMessage, ostream & output ) const 
{
  if ( retVal == SV_OK ) 
    return true ; 
  else {
    output << "\n " << errorMessage << "\n" ; 
    sv_errorprint(dev,retVal) ; 
    return false ; }
}



//: Constructor 
//: ----------------------------------------------------------------------------------------------
ClipStationProDeviceBodyC::ClipStationProDeviceBodyC(const StringC &devName, ColourModeT mode)
  : deviceName(devName),
    dev(0),
    inputFifo(0),
    outputFifo(0),
    rect(0,575,0,719),
    useDMA(true),
    doInput(true),
    fifoMode(true),
    sharedFifo(false),
    captureAudio(true), // Capture audio ?
    captureVideo(true), // Capture video ?
    frameBufferSize(0), // 0 = use as much mem as available 
    timecode_from_getframe((int)0, 25.0), 
    alignDMA (8), // 8 Byte alignment for SDStation Card 
    colourMode(mode) 
{
  // register device in global hash
  if (devices.Insert(deviceName,this) ) {
    cerr << "\n Error: Body already exists for device " << deviceName ; 
    throw ExceptionOperationFailedC("Failed to open ClipStation device (body) ") ; }
  
  // open the device 
  dev = sv_open((char *) deviceName.chars());
  if(dev == 0) {
    cerr << "\n Error: Failed to open ClipStation device '" << deviceName << "'.";
    cerr << "\n        Either someone else is using the device, or the hardware is not installed. \n";
    throw ExceptionOperationFailedC("Failed to open a ClipStation device. ");
  }
  
  if (! Init() ) {
    cerr << "Failed to initialize settings " ; 
    throw ExceptionOperationFailedC("Failed to initialize settings" ) ; 
  }
}
  




//: Destructor 
  //: ----------------------------------------------------------------------------------------------
ClipStationProDeviceBodyC::~ClipStationProDeviceBodyC() {
  ONDEBUG(cerr << "CSP Closing device.\n");
  if(inputFifo != 0) {
    sv_fifo_free(dev,inputFifo);
    inputFifo = 0;
  }
  if(dev != 0) {
      sv_close(dev);
      dev = 0;
  }
  dev = 0;
    // remove from global hash
  devices.Del(deviceName) ;
}


//: Build the attributes for output mode 
//: ----------------------------------------------------------------------------------------------
bool ClipStationProDeviceBodyC::BuildAttributesOut  (AttributeCtrlBodyC & attrCtrl) 
{
  DListC<StringC> modes ; 
  modes.InsLast("BLACK"); modes.InsLast("COLOURBAR"); modes.InsLast("LIVE") ; // this is crude 
  attrCtrl.RegisterAttribute( AttributeTypeEnumC ("mode", "This sets various modes on the card, see CSPModeT for details" ,true, true, modes, "COLOURBAR" ));
  return true ; 
}



//: Build the attributes for input mode 
//: ----------------------------------------------------------------------------------------------
bool ClipStationProDeviceBodyC::BuildAttributesIn  (AttributeCtrlBodyC & attrCtrl) 
{

  attrCtrl.RegisterAttribute
    ( AttributeTypeStringC("timecode", "The timecode of the last frame captured", true, false ,"", 255 )) ; 

  attrCtrl.RegisterAttribute ( AttributeTypeNumC<IntT>("FrameBufferSize", 
						       "The size of the framebuffer (number of buffers (0=as many buffers as possible))",
						       true , false , 1, -1)  );
return true ; 
}



//: GetAttrIn
//: ----------------------------------------------------------------------------------------------
bool ClipStationProDeviceBodyC::GetAttrIn(const StringC &attrName,  StringC &attrValue) const 

{
  RWLockHoldC holdLock ( rwLock, RWLOCK_READONLY ) ; 
  if(attrName == "FrameBufferSize") {
    attrValue = StringC(frameBufferSize);
    return true;
  }
    else if (attrName == "timecode") {
      attrValue = timecode_from_getframe.ToText();
      return true ; 
    }
  return false;
}



//: SetAttrIn
//: ----------------------------------------------------------------------------------------------
bool ClipStationProDeviceBodyC::SetAttrIn(const StringC &attrName,const StringC &attrValue) 
{ return false ; }





//: Get a stream attribute.
// Returns false if the attribute name is unknown.
// This is for handling stream attributes such as frame rate, and compression ratios.  
//: ----------------------- -----------------------------------------------------------------------
bool ClipStationProDeviceBodyC::GetAttrOut(const StringC &attrName, StringC &attrValue) const 
{
  if(attrName == "FrameBufferSize") 
    {
      attrValue = frameBufferSize ; 
      return true ; 
    }
  return false ; 
  //    if(nsize == frameBufferSize) return true;
}






//: Set a stream attribute.
// Returns false if the attribute name is unknown.
// This is for handling stream attributes such as frame rate, and compression ratios.
//: ----------------------------------------------------------------------------------------------
bool ClipStationProDeviceBodyC::SetAttrOut(const StringC &attrName,const StringC &attrValue) {

  {
    RWLockHoldC hold (rwLock,RWLOCK_WRITE) ;
    // framebuffer size 
    // -----------------------------
  if(attrName == "FrameBufferSize") {
    int nsize = attrValue.IntValue();
    if(nsize == frameBufferSize) return true;
    if(!useDMA) return true; // Only works in DMA mode.
    cerr << "Setting frame buffer size=" << nsize << "\n" ;
    CheckError( sv_fifo_free(dev,inputFifo), "Failed to free fifo", cerr) ;
    CheckError ( sv_fifo_init(dev,&inputFifo,1,1,useDMA,0,nsize), "Failed to init fifo", cerr ) ;;
    CheckError ( sv_fifo_start(dev,inputFifo), "Failed to start fifo", cerr) ;
    CheckError (  sv_fifo_configstatus(dev,inputFifo,&fifo_config), "Failed to get fifo config", cerr ) ; 
    frameBufferSize = nsize;
    return true;
  } 
  } // release lock
  
  
 // raster modes (colourbars etc) 
  // -------------------------------
    if (attrName == "LIVE" && attrValue == "1") 
      { 
	SetMode(LIVE) ; 
	return true ; 
      } else 
	if (attrName == "COLOURBAR" && attrValue == "1") 
	  {
	    SetMode(COLOURBAR) ; 
	    return true ; 
	    } else 
	      if (attrName == "BLACK" && attrValue == "1") 
		{
		  SetMode(BLACK) ; 
		  return true ; 
		} else 
		  if (attrName == "DELAY" && attrValue == "1") 
		    {
		      SetMode(DELAY) ; 
		      return true ; 
		    } 
		  else 
		    return false ; 

}



//: Get a generic frame ( buffer of chars ) 
//---------------------------------------------------------------------------------------------------------------------------------------
DMABufferC<char> ClipStationProDeviceBodyC::GetFrameGeneric() const { 

 RWLockHoldC hold (rwLock,RWLOCK_WRITE) ; 
  
  // get the hardware buffer 
  sv_fifo_buffer *svbuf ;
  if (! CheckError (sv_fifo_getbuffer(dev,inputFifo,&svbuf,0,0), "Failed to get frame buffer :") )
    return DMABufferC<char>();
  
  // make a ravl buffer 
  int dmaBufferSize = fifo_config.vbuffersize + fifo_config.abuffersize;
  cout << "\n dmaBufferSize = " << dmaBufferSize ; 
  DMABufferC<char> buf = GetDMABufferGeneric() ; // this buffer gets deleted when finished with. 

  // arrange transfer using dma
  if(useDMA) {
    svbuf->dma.addr = (char *)  buf.ReferenceElm();
    svbuf->dma.size = dmaBufferSize; } 
  // otherwise do it with memcpy
  else {
    memcpy(buf.ReferenceElm(),svbuf->video[0].addr,(svbuf->video[0].size + svbuf->video[1].size)); }
    
  // give buffer back to card 
  if (! CheckError ( sv_fifo_putbuffer(dev,inputFifo,svbuf,0), "ERROR ClipStationProDeviceBodyC: Failed to put frame " ) )
    return DMABufferC<char>();


  // store timecode in case it's requested by CSPGetAttr(...)
  // this returns it backwards!: "ff:ss:mm:hh"
  char str_backwards[16];
  sv_tc2asc(dev, svbuf->timecode.vitc_tc, str_backwards, 16);

  // convert to Ravl timecode object
  timecode_from_getframe = TimeCodeC(atoi(&str_backwards[9]),
				     atoi(&str_backwards[6]),
				     atoi(&str_backwards[3]),
				     atoi(&str_backwards[0]));
    
  return buf;
}



  //: Get one Frame of video. - old depricated, only works when card is in yuv modes
  //: ----------------------------------------------------------------------------------------------
DMABufferC<ByteYUV422ValueC> ClipStationProDeviceBodyC::GetFrame() const {
  RavlAlwaysAssertMsg ( colourMode == YUV, "Error GetFrame called for YUV, but card not in YUV mode") ;
  RWLockHoldC hold (rwLock,RWLOCK_WRITE) ; 
  
  // get the hardware buffer 
  sv_fifo_buffer *svbuf ;
  if (! CheckError (sv_fifo_getbuffer(dev,inputFifo,&svbuf,0,0), "Failed to get frame buffer :") )
    return DMABufferC<ByteYUV422ValueC>();
  
  // make a ravl buffer 
  // int audioBufferSize = fifo_config.abuffersize ;
  int dmaBufferSize = fifo_config.vbuffersize + fifo_config.abuffersize;
  //BufferC<ByteYUV422ValueC> buf(dmaBufferSize / sizeof(ByteYUV422ValueC) ) ;// ,fifo_config.dmaalignment);
  DMABufferC<ByteYUV422ValueC> buf = GetDMABuffer() ; 

  // arrange transfer using dma
  if(useDMA) {
    svbuf->dma.addr = (char *)  buf.ReferenceElm();
    svbuf->dma.size = dmaBufferSize; } 
  // otherwise do it with memcpy
  else {
    memcpy(buf.ReferenceElm(),svbuf->video[0].addr,(svbuf->video[0].size + svbuf->video[1].size)); }
    
  // give buffer back to card 
  if (! CheckError ( sv_fifo_putbuffer(dev,inputFifo,svbuf,0), "ERROR ClipStationProDeviceBodyC: Failed to put frame " ) )
    return DMABufferC<ByteYUV422ValueC>();


  // store timecode in case it's requested by CSPGetAttr(...)
  // this returns it backwards!: "ff:ss:mm:hh"
  char str_backwards[16];
  sv_tc2asc(dev, svbuf->timecode.vitc_tc, str_backwards, 16);

  // convert to Ravl timecode object
  timecode_from_getframe = TimeCodeC(atoi(&str_backwards[9]),
				     atoi(&str_backwards[6]),
				     atoi(&str_backwards[3]),
				     atoi(&str_backwards[0]));
  
  
#if DODEBUG
  sv_fifo_info info;
  sv_fifo_status(dev,inputFifo,&info);
  
  cerr <<"Dropped: " << info.dropped << " TimeCode1 ltc:" << svbuf->timecode.ltc_tc << " vitc:" << svbuf->timecode.vitc_tc << " vitc2:" << svbuf->timecode.vitc_tc2 <<  " tick:"<< svbuf->timecode.vtr_tick << " vtr:" << svbuf->timecode.vtr_tc << " Lock:" << svbuf->timecode.vtr_lockcount << " \n";
#endif
  
  
  return buf;
}


bool ClipStationProDeviceBodyC::PutFrame( const DMABufferC<char> & buffer) const { 
RWLockHoldC hold (rwLock, RWLOCK_WRITE) ;
  if (useDMA) 
    {
      sv_fifo_buffer * bufferOutput = NULL ; 
      //IntT dmaBufferSize = fifo_config.vbuffersize + fifo_config.abuffersize ; // how much data should we transfer  
      CheckError( sv_fifo_getbuffer(dev, outputFifo, & bufferOutput, NULL, 0 ), "\n Error getting buffer "  ) ; 
      bufferOutput->dma.addr = (char*) buffer.ReferenceElm() ; // arrange transfer 
      bufferOutput->dma.size = buffer.Size() * sizeof(char)  ; 
      CheckError( sv_fifo_putbuffer(dev, outputFifo, bufferOutput, 0 ), "\n Error: Failed to release buffer" ) ; // return buffer and do transfer 
      return true;
    }
  else // NO DMA 
    {
      cerr << "non dma not supported " ; 
      return true ; 
    }
}




  
//: Put one frame of video onto the device output.   - old method uses yuv 
//: ----------------------------------------------------------------------------------------------
bool ClipStationProDeviceBodyC::PutFrame(const DMABufferC<ByteYUV422ValueC> & buffer) const{ 
  RWLockHoldC hold (rwLock, RWLOCK_WRITE) ;
  
  RavlAlwaysAssertMsg ( colourMode == YUV, "Error PutFrame called for YUV, but card not in YUV mode") ;
  if (useDMA) 
    {
      sv_fifo_buffer * bufferOutput = NULL ; 
      //IntT dmaBufferSize = fifo_config.vbuffersize + fifo_config.abuffersize ; // how much data should we transfer  
      CheckError( sv_fifo_getbuffer(dev, outputFifo, & bufferOutput, NULL, 0 ), "\n Error getting buffer "  ) ; 
      bufferOutput->dma.addr = (char*) buffer.ReferenceElm() ; // arrange transfer 
      bufferOutput->dma.size = buffer.Size() * sizeof(ByteYUV422ValueC)  ; 
      CheckError( sv_fifo_putbuffer(dev, outputFifo, bufferOutput, 0 ), "\n Error: Failed to release buffer" ) ; // return buffer and do transfer 
      return true;
    }
  else // NO DMA 
    {
      cerr << "non dma not supported " ; 
      return true ; 
    }	
}



//: Switch the card into one of its defalt modes 
//: ----------------------------------------------------------------------------------------------
bool ClipStationProDeviceBodyC::SetMode(CSPModeT mode) 
{
  RWLockHoldC hold (rwLock, RWLOCK_WRITE) ;
  
  // stop the fifo's flush them out 
  int flags = SV_FIFO_FLAG_FLUSH ; 
  sv_fifo_wait ( dev, inputFifo) ; 
  sv_fifo_wait ( dev, outputFifo) ; 
  sv_fifo_stop ( dev, inputFifo , flags ) ; 
  sv_fifo_stop ( dev, outputFifo, flags ) ; 

  IntT ret = false ; 
  switch (mode) {
    

  case BLACK: 
    ret = CheckError( sv_black(dev), "Error: Failed to set video raster to black mode" ) ;
    break ; 
    
  case COLOURBAR:
    ret = CheckError( sv_colorbar(dev), "Error: Failed to set video raster to colour bar mode" ) ;
    break ; 

  case LIVE: 
    ret = CheckError( sv_showinput(dev, SV_SHOWINPUT_DEFAULT, 0), "Error: Failed to set video raster to live mode") ; 
    break ; 

  case DELAY: 
    ret = CheckError( sv_showinput(dev, SV_SHOWINPUT_FRAMEBUFFERED, 0), "Error: Failed to set video raster to delay mode") ; 
    break ; 
    
  default:
    ret = false ; 
    cerr << "\n\nError: Unknown mode " << mode ; 
    break ;  
  };
  
  // restart the fifos 
  ret = sv_fifo_start(dev,inputFifo); // Start it going...
  ret = sv_fifo_start(dev,outputFifo); 

  return ret ; 
}





//: Setup video modes to a suitable default.
//: ----------------------------------------------------------------------------------------------
bool ClipStationProDeviceBodyC::Init() {
  RWLockHoldC hold (rwLock,RWLOCK_WRITE) ;

  //: Set PAL 
  if ( colourMode == YUV ) {
	  cerr << "\nYUV mode" ; 
	  if( !CheckError( sv_videomode(dev,SV_MODE_PAL | SV_MODE_COLOR_YUV422 ), " Failed to set mode to PAL. Error" ) )
    		return false; }
  else if ( colourMode == RGB ) {
  	cerr << "\nRGB mode" ; 
   	  if( !CheckError( sv_videomode(dev,SV_MODE_PAL | SV_MODE_COLOR_RGB_RGB ), " Failed to set mode to PAL. Error" ) )
    		return false; }
  else CheckError(false, "Unknown video mode") ; 
		
  //: Set SYNC - Should this be internal or external  ? 
  //if( !CheckError( sv_sync(dev,SV_SYNC_INTERNAL), "Failed to set sync" ) ) 
  //  return false;
    
  //: Set SYNC - Should this be internal or external  ? 
  if( !CheckError( sv_sync(dev,SV_SYNC_EXTERNAL), "Failed to set sync" ) ) 
    return false;

  //: Set TimeCode Line 
  if( !CheckError(sv_option(dev,SV_OPTION_VITCLINE,19), "Failed to set vitc line." ) )
    return false ; 

  //: Set SDI
  if( !CheckError( sv_option(dev,SV_OPTION_INPUTPORT,SV_INPORT_SDI), "Failed to set input to SDI" ) )
    return false;
  
  //: Preset Video 
  if( !CheckError( sv_preset(dev,SV_PRESET_VIDEO), "Failed to preset video. Error." )) 
    return false;
    
    //: Set the cards into ccir601 
    if ( !CheckError( sv_matrix(dev,SV_MATRIX_CCIR601,0), "Failed to set rgb conversion matrixt to ccir601") ) 
    return false ; 
    

  /*
    // Audio...
    int audioChannels,audioBits,audioSize;
    if((ret = sv_option(dev, SV_OPTION_AUDIOINPUT,SV_AUDIOINPUT_AIV )) != SV_OK) {
      cerr << "Failed to set audio input." << ret << "\n";
    }
    if((ret = sv_option(dev, SV_OPTION_AUDIOCHANNELS,1 )) != SV_OK) {
      cerr << "Failed to set audio channels." << ret << "\n";
    }
    if((ret = sv_query(dev, SV_QUERY_AUDIOCHANNELS, 0, &audioChannels)) != SV_OK) {
      cerr << "Failed to query audio channels." << ret << "\n";
    }
    if((ret = sv_query(dev, SV_QUERY_AUDIOBITS, 0, &audioBits)) != SV_OK) {
      cerr << "Failed to query audio bits." << ret << "\n";
    }
    if((ret = sv_query(dev, SV_QUERY_AUDIOSIZE, 0, &audioSize)) != SV_OK) {
      cerr << "Failed to query audio soize." << ret << "\n";
    }
    cerr << "Audio Channels=" << audioChannels << " Bits=" << audioBits << " Size=" << audioSize << "\n";
  */
    
  if(fifoMode) {

    // start the input fifo 
    IntT inputFlags = 0 ;
    CheckError( sv_fifo_init(dev,&inputFifo, true, sharedFifo, useDMA, inputFlags, frameBufferSize), "Failed to initialize input fifo" ) ; 
    CheckError( sv_fifo_start(dev,inputFifo), "Failed to start input fifo " ) ;
    
    // start the output fifo
    IntT outputFlags = 0 ; 
    CheckError( sv_fifo_init(dev,&outputFifo, false, sharedFifo, useDMA, outputFlags, frameBufferSize), "Failed to initialize output fifo") ;
    CheckError( sv_fifo_start( dev, outputFifo ), "Failed to start output fifo" ) ;

    if ( !CheckError( sv_fifo_configstatus(dev,inputFifo,&fifo_config), "Failed to get fif status. Error" ) )
      return false ;
    //	ONDEBUG(cerr << "Field offset=" << fifo_config.field1offset << " " << fifo_config.field2offset << "\n");
  } 
  else 
    {
      fifo_config.dmaalignment = 1;
    }
  
  return true;
}
}
