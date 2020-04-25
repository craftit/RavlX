// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2002, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//! rcsid="$Id$"
//! lib=RavlDV
//! file="Ravl/Contrib/DV/DvDecode.cc"

#include"Ravl/Image/DvDecode.hh"
#include"Ravl/Image/Deinterlace.hh"

namespace RavlImageN {

  //: Constructor 
  //: ------------------------------------------------------------------------------------------------------------------------------------------------------
  DvDecodeBodyC::DvDecodeBodyC (bool deint)
    : init(false), 
      deinterlace(deint)
  {
    decoded = (uint8_t*)malloc(576*720*3);
    decoder = dv_decoder_new(true, true, true);
    dv_init(true, true);
    decoder->quality = DV_QUALITY_BEST; // for know we will make it highest

    // allocate  audio buffers  
    for ( IntT i = 0 ; i < maxAudioChannels ; ++i ) 
      audio_buffers[i] = (int16_t *) malloc (sizeof (int16_t) * 2 * maxAudioSamples ) ;   
  }
  

  //: Initialize 
  //: ------------------------------------------------------------------------------------------------------------------------------------------------------
  void DvDecodeBodyC::Initialize (ByteT * data ) 
{
  dv_parse_header(decoder, (uint8_t*)data);
  // get the info
    samplerate = decoder->audio->frequency ; 
    samplesThisFrame    = decoder->audio->samples_this_frame ; 
    //IntT frames       = decoder->audio->aaux_as.pc3.system ; 
    //IntT channels   = decoder->audio->num_channels ;
    samplebits = decoder->audio->quantization ; 
  init=true;
}

  



  //: Destructor.
  //: ------------------------------------------------------------------------------------------------------------------------------------------------------
  DvDecodeBodyC::~DvDecodeBodyC() {
    free(decoded);    
    // free audio buffers 
    for ( IntT i = 0 ; i < maxAudioChannels ; ++i ) 
      free ( audio_buffers[i] ) ;
}
  

  //: Apply 
  //: ------------------------------------------------------------------------------------------------------------------------------------------------------
  AVFrameC DvDecodeBodyC::Apply(ByteT *data) {
    uint8_t *pixels[3];
    int pitches[3];
  
    pixels[0] = decoded;
    pixels[1] = NULL;
    pixels[2] = NULL;

    pitches[0] = 720 * 3;
    pitches[1] = 0;
    pitches[2] = 0;
    
    if(!init) 
      Initialize (( uint8_t * ) data );
    
    dv_decode_full_frame(decoder, data, e_dv_color_rgb, pixels, pitches);
    
    
    // Audio 
    // ------    
    // extract 
    dv_decode_full_audio( decoder, data, ( int16_t ** ) audio_buffers )  ; 
    
    // fill array with samples 
    typedef SampleElemC<2,Int16T> SampleT; 
    SArray1dC<SampleT>  array (samplesThisFrame) ; 
    for ( IntT n = 0; n < samplesThisFrame; ++n )
      {
	SampleT sample ; 
	sample.channel[0] = audio_buffers[0][n] ; 
	sample.channel[1] = audio_buffers[1][n] ; 
	array[n] = sample ; 
      }
    

   

    ImageC<ByteRGBValueC> im(576, 720, (ByteRGBValueC*)&pixels[0][0], false);
    if(deinterlace) im = DeinterlaceSubsample(im);
    return AVFrameC( AudioFrameC( array, samplerate) , im ) ;
  }





















 


} // end namespace RavlImageN

