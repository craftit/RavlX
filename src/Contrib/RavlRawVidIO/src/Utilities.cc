// This file is part of RAVL, Recognition And Vision Library
// Copyright (C) 2005-12, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#include "Ravl/Image/Utilities.hh"
#include "Ravl/Types.hh"
#include  "Ravl/BiHash.hh"
#include "Ravl/RCHash.hh"
#include "Ravl/Tuple2.hh"
#include  <iomanip>
#include <stdint.h>
//! rcsid="$Id: fixSource.cfg,v 1.6 2005/03/02 09:28:41 craftit Exp $"
//! lib=RavlRawVidIO

#define DODEBUG 0
#if DODEBUG
#define ONDEBUG(x) x
#else
#define ONDEBUG(x)
#endif

namespace RavlImageN {
/*
static const UIntT numVideoOffsets = 2 ;
//: how many video offsets into buffer - 2 fields
static const UIntT numAudioOffsets = 8 ;
//: how many audio offsets into buffer - 8 interleaved stereo pairs.

typedef TFVectorC<SizeT, numAudioOffsets> AudioOffsetsT ;
typedef TFVectorC<SizeT, numVideoOffsets> VideoOffsetsT ;
//: Types to hold audio and video offsets.
*/
typedef BiHashC<IntT,StringC> EnumNameTableT ;
//: A structure to hold string / sdk value paris.
//: This may be helpfull for reading in and out of enumerators by string rather than value.

static RCHashC<StringC,EnumNameTableT> enumNameTable ;
//: The lookup table for enumerator strings and sdk values.
//: Each enumerator type has its own lookup table.

//! userlevel=Develop
//: An internal class that handles the Registering of enum names
//: The constructor registers the enum names and is invoked through static initialisation
class RegisterEnumNamesC
{ public: RegisterEnumNamesC(void) ; };


// Register the enums here - Each Enum defined in Types.hh should be registered.
//: -----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
RegisterEnumNamesC::RegisterEnumNamesC(void)
{

  //: CardT
  RegisterNameM(UNKNOWN_CARD) ;
  RegisterNameM(CENTAURUS) ;
  RegisterNameM(SDBOARD) ;

  //: SyncT
  RegisterNameM(UNKNOWN_SYNC) ;
  RegisterNameM(SYNC_INTERNAL) ;
  RegisterNameM(SYNC_EXTERNAL) ;
  RegisterNameM(SYNC_AUTO);

  //: VideoModeT
  //RegisterNameM (MISC) ;
  RegisterNameM(UNKNOWN_VIDEOMODE) ;
  RegisterNameM (SMPTE274_25I) ;
  RegisterNameM (SMPTE274_25P);
  RegisterNameM (SMPTE274_24P) ;
  RegisterNameM (SMPTE274_24sF) ;
  RegisterNameM (PAL) ;

  //: ByteFormatT
  RegisterNameM(UNKNOWN_BYTEFORMAT) ;
  RegisterNameM (BITS_10_DVS) ;
  RegisterNameM (BITS_8) ;

  //: ColourModeT
  RegisterNameM(UNKNOWN_COLOURMODE) ;
  RegisterNameM (YUV422) ;
  RegisterNameM (RGB_RGB) ;

  //: DMA ModeT
  RegisterNameM(UNKNOWN_DMAMODE) ;
  RegisterNameM (FIFO_DMA) ;
  RegisterNameM (FIFO_NODMA) ;

  //: AudioInputT
  RegisterNameM(UNKNOWN_AUDIOINPUT) ;
  RegisterNameM (AIV);
  RegisterNameM (AES) ;
  RegisterNameM (NOAUDIO) ;

  //: AudioBitsT
  RegisterNameM(UNKNOWN_AUDIOBITS) ;
  RegisterNameM (AUDIO_32BIT) ;
  RegisterNameM (AUDIO_16BIT) ;

  //: AudioChannels
  RegisterNameM(UNKNOWN_AUDIOCHANNELS) ;
  RegisterNameM (AUDIO_CHANNELS_8) ;
  RegisterNameM(AUDIO_CHANNELS_4) ;
  RegisterNameM(AUDIO_NOAUDIO) ;
  //RegisterNameM (AVAUDIO_CH8) ;

  //: AVModeT
  RegisterNameM(UNKNOWN_AVMODE) ;
  RegisterNameM(AVAUDIO_CH8) ;

  //: AudioFreqT
  RegisterNameM(UNKNOWN_AUDIOFREQ) ;
  RegisterNameM ( AUDIO_FREQ_48000) ;
};


static RegisterEnumNamesC  initRegister ;
//: invoke a static instance so constructor gets called, and enums get hashed !




//: This associates an enumerator value with its string
//: The type of enumerator indexed using the typename parameter.
//: Use the RegisterNameM macro for a shorthanded version.
//: ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void RegisterName (  const StringC & typeName, const IntT & enumerator, const  StringC & name )
{ enumNameTable[typeName].Insert( enumerator, name ) ; }


//: This looks up the name of the enumerator from the value.
//: ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
StringC LookupEnumName( const StringC & typeName, const IntT & enumerator )
{
  StringC ret ;
  EnumNameTableT * table = enumNameTable.Lookup( typeName ) ;
  if ( ! table ) { cerr << "\nLookupEnumName::Unknown enum name lookup, name: " << typeName ; return  ret ; }
  if ( ! table->LookupFwd(enumerator, ret) ) { cerr << "\nLookupEnumName::Name not known for type " << typeName << ", and value " << enumerator ; };
  return ret ;
}


//: This looks up the name of the enumerator from the value.
//: --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
IntT LookupEnum (const StringC & typeName, const StringC & name )
{
  ONDEBUG ( cerr << "\nIntT LookupEnum (const StringC & name )  : " << ", " << name ) ;
  IntT ret = 0;
  EnumNameTableT  * table = enumNameTable.Lookup( typeName) ;
  if ( ! table ) { cerr << "\nLookupEnum::Unknown enum type name lookup, name: " << typeName ; return ret ; }
  if ( ! table->LookupBkw (name, ret) ) cerr << "\nLookupEnum::Value not found for name " << name << ", of type " << typeName ;
  return ret ;
}



//: Dump the enum table to stream - debugging
//: ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void DumpNameTable ( ostream & stream )
{
  for ( HashIterC<StringC,EnumNameTableT> typeIter ( enumNameTable ) ; typeIter ; typeIter++ ) {
    stream << "\n---- " << setw(30) << typeIter.Key().chars() ;
    for ( BiHashIterC<IntT,StringC> tabIter ( typeIter.Data() ) ; tabIter ; tabIter ++ )
      stream << "\n\t\t\t--" << setw(20) << tabIter.Data().chars() << "\t" << tabIter.Key() ;
  }
}

//==========================================================================//

bool GetDimensions(const VideoModeT video_mode, UIntT& x, UIntT& y)
{
  bool ok = true;

  switch(video_mode) {
  case PAL:
    x = 576;
    y = 720;
    break;
  case SMPTE274_25I:
  case SMPTE274_25P:
  case SMPTE274_24P:
  case SMPTE274_24sF:
    x = 1080;
    y = 1920;
    break;
  default:
    ok = false;
    x = 0;
    y = 0;
    break;
  }

  return ok;
}

//--------------------------------------------------------------------------//

VideoModeT IdToVideoMode(UIntT id)
{
  // Video Mode
  VideoModeT mode = UNKNOWN_VIDEOMODE;
  switch(id) {
  case 0:
    mode = UNKNOWN_VIDEOMODE;
    break;
  case 1:
    mode = SMPTE274_25I;
    break;
  case 2:
    mode = SMPTE274_25P;
    break;
  case 3:
    mode = SMPTE274_24P;
    break;
  case 4:
    mode = SMPTE274_24sF;
    break;
  case 5:
    mode = PAL;
    break;
  default:
    RavlIssueError("Unknown Video Mode Id in IdToVideoMode().") ;
    break;
  }

  return mode;
}

//--------------------------------------------------------------------------//

uint8_t VideoModeToId(const VideoModeT mode)
{
  // Video Mode
  uint8_t mode_id = 0;
  switch(mode) {
  case UNKNOWN_VIDEOMODE:
    mode_id = 0;
    break;
  case SMPTE274_25I:
    mode_id = 1;
    break;
  case SMPTE274_25P:
    mode_id = 2;
    break;
  case SMPTE274_24P:
    mode_id = 3;
    break;
  case SMPTE274_24sF:
    mode_id = 4;
    break;
  case PAL:
    mode_id = 5;
    break;
  default:
    RavlIssueError("Unknown Video Mode in VideoModeToId().") ;
    break;
  }

  return mode_id;
}

//--------------------------------------------------------------------------//
/*
ByteFormatT IdToByteFormat(uint8_t id)
{
  // Video Mode
  ByteFormatT format = UNKNOWN_BYTEFORMAT;
  switch(id) {
  case 0:
    format = UNKNOWN_BYTEFORMAT;
    break;
  case 1:
    format = BITS_10_DVS;
    break;
  case 2:
    format = BITS_8;
    break;
  default:
    RavlIssueError("Unknown Byte Format Id in IdToByteFormat().") ;
  }

  return format;
}
*/

//---------------------------------------------------------------------------//

ByteFormatT IdToByteFormat(int id)
{
  // Video Mode
  ByteFormatT format = UNKNOWN_BYTEFORMAT;
  switch(id) {
  case 0:
    format = UNKNOWN_BYTEFORMAT;
    break;
  case 1:
    format = BITS_10_DVS;
    break;
  case 2:
    format = BITS_8;
    break;
  default:
    RavlIssueError("Unknown Byte Format Id in IdToByteFormat().") ;
    break;
  }
  return format;
}



//--------------------------------------------------------------------------//

uint8_t ByteFormatToId(const ByteFormatT format)
{
  uint8_t format_id = 0;
  switch(format) {
  case UNKNOWN_BYTEFORMAT:
    format_id = 0;
    break;
  case BITS_10_DVS:
    format_id = 1;
    break;
  case BITS_8:
    format_id = 2;
    break;
  default:
    RavlIssueError("Unknown Byte Format in ByteFormatToId().") ;
    break;
  }

  return format_id;

}

//--------------------------------------------------------------------------//

ColourModeT IdToColourMode(UIntT id)
{
  ColourModeT mode = UNKNOWN_COLOURMODE;
  switch(id) {
  case 0:
    mode = UNKNOWN_COLOURMODE;
    break;
  case 1:
    mode = YUV422;
    break;
  case 2:
    mode = RGB_RGB;
    break;
  default:
    RavlIssueError("Unknown Colour Mode Id in IdToColourMode().") ;
    break;
  }

  return mode;
}

//--------------------------------------------------------------------------//

uint8_t ColourModeToId(const ColourModeT mode)
{
  uint8_t mode_id = 0;
  switch(mode) {
  case UNKNOWN_COLOURMODE:
    mode_id = 0;
    break;
  case YUV422:
    mode_id = 1;
    break;
  case RGB_RGB:
    mode_id = 2;
    break;
  default:
    RavlIssueError("Unknown Colour Mode in ColourModeToId().") ;
    break;
  }

  return mode_id;
}

}; // DVSN ;
