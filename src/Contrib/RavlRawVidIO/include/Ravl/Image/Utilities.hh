// This file is part of RAVL, Recognition And Vision Library
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#include "Ravl/String.hh"
#include "Ravl/Types.hh"
#include "Ravl/TFVector.hh"
#include <stdint.h>
//! rcsid="$Id: fixSource.cfg,v 1.6 2005/03/02 09:28:41 craftit Exp $"
//! lib=RavlRawVidIO
//! userlevel=Develop
//! author="Lee Gregory"
#ifndef RAVL_DVS_TYPES
#define RAVL_DVS_TYPES 1
namespace RavlImageN {

#define RegisterNameM(x) RegisterName (typeid(x).name(), x, #x)
//: A Macro to register an enumerator with its corresponding string.

#define LookupNameM(x) LookupEnumName (typeid(x).name(),x)
//: A Macro to lookup an enumerator from its string.

//#define LookupEnumM(x) LookupEnum(typeid(x).name(),x)
//: A Macro to lookup enumerator values - not used anymore .
using namespace RavlN ;

static const UIntT numVideoOffsets = 2 ;
//: how many video offsets into buffer - 2 fields
static const UIntT numAudioOffsets = 8 ;
//: how many audio offsets into buffer - 8 interleaved stereo pairs.

typedef TFVectorC<SizeT, numAudioOffsets> AudioOffsetsT ;
typedef TFVectorC<SizeT, numVideoOffsets> VideoOffsetsT ;
//: Types to hold audio and video offsets.

//using namespace RavlN ;
//! userlevel=Develop

void RegisterName ( const StringC  & typeName, const IntT & enumerator, const StringC & name ) ;
//: Enumerator helper function
//: A function used to register the name of an enumerator for pretty printing of enumerators.
// This function is best used with macro RegisterNameM

StringC LookupEnumName( const StringC & typeName, const IntT & enumerator) ;
//: Enumerator helper function.
//: This looks up the string name for the given enumerator.
// This function is best used with the macro LookupNameM

//IntT LookupEnum (const StringC & typeName, const StringC & name ) ;
//: Enumerator helper function
// This looks up the enumerator value from its name(string)
// Best used with LookupEnumM() macro

void DumpNameTable ( ostream & stream ) ;
//: Enumerator helper function
//: Dump the enum lookup table to stream - debugging

//==========================================================================//

enum DVSPassModeT { BLACK, COLOURBAR, LIVE, DELAY }  ;
//! userlevel=Normal
//: some diagnostic modes
//!param BLACK - Sets video raster to black
//!param COLOURBAR - Sets video raster to generate colourbars
//!param LIVE - Sets video raster to live mode (ouput mirros input)
//!param DELAY - Sets video raster output to mirror video input but with a 1 frame delay

enum CardT { UNKNOWN_CARD=-1, CENTAURUS , SDBOARD } ;
//: The model of card detected

enum InSyncT { UNKNOWN_SYNC=-1, SYNC_INTERNAL, SYNC_EXTERNAL, SYNC_AUTO } ;
//: The input sync mode

//enum OutSyncT { SYNCOUT_INTERNAL, SYNCOUT_EXTERNAL, SYNCOUT_AUTO} ;
//: The output sync mode

enum VideoModeT { UNKNOWN_VIDEOMODE=-1, SMPTE274_25I, SMPTE274_25P, SMPTE274_24P, SMPTE274_24sF, PAL} ;
//: Some video modes (SDI)

enum ByteFormatT { UNKNOWN_BYTEFORMAT=-1, BITS_10_DVS, BITS_8 } ;
//: The byte format for the buffers.

enum ColourModeT { UNKNOWN_COLOURMODE=-1,YUV422, RGB_RGB } ;
//: The colour format for the buffers.

enum DMAModeT { UNKNOWN_DMAMODE=-1, FIFO_DMA, FIFO_NODMA } ;   // needs registering
//: The dma mode for the frame transers

enum AudioInputT { UNKNOWN_AUDIOINPUT=-1, AIV, AES  , NOAUDIO} ;
//: The audio input source

enum AudioBitsT { UNKNOWN_AUDIOBITS=-1, AUDIO_32BIT, AUDIO_16BIT} ;
//: Audio bits ;

enum AudioChannelsT { UNKNOWN_AUDIOCHANNELS=-1, AUDIO_CHANNELS_8, AUDIO_CHANNELS_4, AUDIO_NOAUDIO } ;
//: AudioChannels  ( in buffer )

enum AVAudioModeT { UNKNOWN_AVMODE=-1, AVAUDIO_CH8 } ;
//: Audion Channels i think !

enum AudioFreqT { UNKNOWN_AUDIOFREQ=-1, AUDIO_FREQ_48000 } ;
//: Audio Frequency

enum DVSMatrixT { UNKNOWN_MATRIX=-1, CCIR601, SMPTE274 } ;
//: Colour Conversion matrices

enum LinkModeT { UNKNOWN_LINKMODE=-1, SINGLE_LINK, DUAL_LINK } ;
//: Use single or dual channel mode

//==========================================================================//

bool GetDimensions(const VideoModeT video_mode, UIntT& x, UIntT& y);
//: Get the dimensions of a certain video mode.
//!param: video_mode - The video mode to find the dimensions of.
//!param: x - Return reference for the horizontal dimension.
//!param: y - Return reference for the vertical dimension.
//!return: true if the video mode was recognise, false otherwise.

VideoModeT IdToVideoMode(UIntT id);
//: Convert the videomode id to the correct enum.
//!param: id - The id to convert.
//!return: The videomode corresponding to the id or UNKNOWN_VIDEOMODE otherwise.

uint8_t VideoModeToId(const VideoModeT mode);
//: Convert the videomode enum to an int.

//ByteFormatT IdToByteFormat(uint8_t id);
ByteFormatT IdToByteFormat(int id);
//: Convert the byteformat id to the correct enum.
//!param: id - The id to convert.
//!return: The byteformat corresponding to the id or UNKNOWN_BYTEFORMAT otherwise.

uint8_t ByteFormatToId(const ByteFormatT format);
//: Convert the byteformat enum to an int.

ColourModeT IdToColourMode(UIntT id);
//: Convert the colourmode id to the correct enum.
//!param: id - The id to convert.
//!return: The colourmode corresponding to the id or UNKNOWN_COLOURMODE otherwise.

uint8_t ColourModeToId(const ColourModeT mode);
//: Convert the coourmode enum to an int.

};
#endif
