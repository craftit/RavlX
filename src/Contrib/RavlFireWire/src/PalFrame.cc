// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2002, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
////////////////////////////////////////////////////////////////////////////
//! author="Kieron J Messer"
//! lib=RavlFireWire
//! date="24/9/2000"
//! rcsid="$Id$"
//! file="Ravl/Contrib/FireWire/PalFrame.cc"

#include "Ravl/Image/PalFrame.hh"
#include "Ravl/Assert.hh"
#include <stdio.h>
#include <cstring>

//: Default constructor

namespace RavlImageN {

  PalFrameC::PalFrameC()
    : decoded(720*576*3), 
      sound(7680), 
      palmap_ch1(2000), 
      palmap_ch2(2000), 
      palmap_2ch1(2000), 
      palmap_2ch2(2000), 
      ntscmap_ch1(2000), 
      ntscmap_ch2(2000), 
      ntscmap_2ch1(2000), 
      ntscmap_2ch2(2000)
  {
    data = new ByteT[144000];
    bw=0;
    decoder = dv_decoder_new(false, false, false);
    dv_init(false, false);
    decoder->quality = DV_QUALITY_BEST; // for know we will make it highest
    frameno=0;
  
    //: then we initialise some buffers
    for (int n = 0; n < 1944; ++n) {
      int sequence1 = ((n / 3) + 2 * (n % 3)) % 6;
      int sequence2 = sequence1 + 6;
      int block = 3 * (n % 3) + ((n % 54) / 18);
    
      block = 6 + block * 16;
      {
	register int byte = 8 + 2 * (n / 54);
	palmap_ch1[n] = sequence1 * 150 * 80 + block * 80 + byte;
	palmap_ch2[n] = sequence2 * 150 * 80 + block * 80 + byte;
	byte += (n / 54);
	palmap_2ch1[n] = sequence1 * 150 * 80 + block * 80 + byte;
	palmap_2ch2[n] = sequence2 * 150 * 80 + block * 80 + byte;
      }
    }
    for (int n = 0; n < 1620; ++n) {
      int sequence1 = ((n / 3) + 2 * (n % 3)) % 5;
      int sequence2 = sequence1 + 5;
      int block = 3 * (n % 3) + ((n % 45) / 15);
    
      block = 6 + block * 16;
      {
	register int byte = 8 + 2 * (n / 45);
	ntscmap_ch1[n] = sequence1 * 150 * 80 + block * 80 + byte;
	ntscmap_ch2[n] = sequence2 * 150 * 80 + block * 80 + byte;
	byte += (n / 45);
	ntscmap_2ch1[n] = sequence1 * 150 * 80 + block * 80 + byte;
	ntscmap_2ch2[n] = sequence2 * 150 * 80 + block * 80 + byte;
      }
    }
    for (int y = 0x700; y <= 0x7ff; ++y)
      compmap[y] = (y - 0x600) << 6;
    for (int y = 0x600; y <= 0x6ff; ++y)
      compmap[y] = (y - 0x500) << 5;
    for (int y = 0x500; y <= 0x5ff; ++y)
      compmap[y] = (y - 0x400) << 4;
    for (int y = 0x400; y <= 0x4ff; ++y)
      compmap[y] = (y - 0x300) << 3;
    for (int y = 0x300; y <= 0x3ff; ++y)
      compmap[y] = (y - 0x200) << 2;
    for (int y = 0x200; y <= 0x2ff; ++y)
      compmap[y] = (y - 0x100) << 1;
    for (int y = 0x000; y <= 0x1ff; ++y)
      compmap[y] = y;
    for (int y = 0x800; y <= 0xfff; ++y)
      compmap[y] = -1 - compmap[0xfff - y];  
  }

  //: Stream constructor

  PalFrameC::PalFrameC(istream &in)
  {
    RavlAssert(0); // Not implemented!
  }

  //: Destructor.
  
  PalFrameC::~PalFrameC() {
    delete [] data;
  }
  
  void 
  PalFrameC::setData(ByteT *dta) { 
    data = dta; 
    bw=144000;
    if(!init) {
      dv_parse_header(decoder, (uint8_t*)data);
      init=true;
      cout << "Audio is: " <<  decoder->audio->frequency / 1000.0 << "kHz" << endl;
      cout << decoder->audio->quantization << " bits quantization" << endl;
      cout << decoder->audio->num_channels << " channels" << endl;
    }
  }

  bool
  PalFrameC::isValid()
  {
    if(bw==144000) {
      if(!init) {
	dv_parse_header(decoder, (uint8_t *)data);
	init=true;
      }  
      return true;
    }
    return false;
  }


  void
  PalFrameC::writeDifBlock(int section_type, int dif_sequence, int dif_block, int length, ByteT * p)
  {
    switch (section_type) {
    case 0:   // 1 Header block
      if ((dif_sequence == 0) && (dif_block == 0)) bw = 0; //: found new frame
      memcpy(&data[dif_sequence * 150 * 80], p, 480);
      break;
    
    case 1:   // 2 Subcode blocks
      memcpy(&data[dif_sequence * 150 * 80 + (1 + dif_block) * 80], p, 480);
      break;
    
    case 2:   // 3 VAUX blocks
      memcpy(&data[dif_sequence * 150 * 80 + (3 + dif_block) * 80], p, 480);
      break;
    
    case 3:   // 9 Audio blocks interleaved with video
      memcpy(&data[dif_sequence * 150 * 80 + (6 + dif_block * 16) * 80], p, 480);
      break;
    
    case 4:   // 135 Video blocks interleaved with audio
      memcpy(&data[dif_sequence * 150 * 80 + (7 + (dif_block / 15) + dif_block) * 80], p, 480);
      break;
    
    default:   // we can�t handle any other data
      break;
    }
 
    bw += 480;
  }

  TimeCodeC
  PalFrameC::extractTimeCode() const
  {

    int seqCount =  12;
    int packNum = 0x13;
    /* process all DIF sequences */
  
    for (int i = 0; i < seqCount; ++i) {
    
      /* there are two DIF blocks in the subcode section */
    
      for (int j = 0; j < 2; ++j) {
      
	/* each block has 6 packets */
      
	for (int k = 0; k < 6; ++k) {
	
	  /* calculate address: 150 DIF blocks per sequence, 80 bytes
	     per DIF block, subcode blocks start at block 1, block and
	     packet have 3 bytes header, packet is 8 bytes long
	     (including header) */
	
	  ByteT *s = &data[i * 150 * 80 + 1 * 80 + j * 80 + 3 + k * 8 + 3];
	  // printf("ssyb %d: %2.2x %2.2x %2.2x %2.2x %2.2x\n",
	  // j * 6 + k, s[0], s[1], s[2], s[3], s[4]);
	  if (s[0] == packNum) {
	    int frame = s[1];
	    int sec = s[2];
	    int min = s[3];
	    int hour = s[4];
	    frame = (frame & 0xf) + 10 * ((frame >> 4) & 0x3);
	    sec = (sec & 0xf) + 10 * ((sec >> 4) & 0x7);
	    min = (min & 0xf) + 10 * ((min >> 4) & 0x7);
	    hour = (hour & 0xf) + 10 * ((hour >> 4) & 0x3);
	    TimeCodeC tc(hour, min, sec, frame);
	    return tc;
	  }
	}
      }
    }
    return TimeCodeC();
  }


  ImageC<ByteRGBValueC>
  PalFrameC::Image()
  {
    //: this little routin will decode them into rgb
    uint8_t *pixels[3];
    int pitches[3];

    pixels[0] = (uint8_t *)&decoded[0];
    pixels[1] = NULL;
    pixels[2] = NULL;
  
    pitches[0] = 720 * 3;
    pitches[1] = 0;
    pitches[2] = 0;
  
    if(!init) {
      dv_parse_header(decoder, (uint8_t*)data);
      init=true;
      cout << "Audio is: " <<  decoder->audio->frequency / 1000.0 << "kHz" << endl;
      cout << decoder->audio->quantization << " bits quantization" << endl;
      cout << decoder->audio->num_channels << " channels" << endl;
    }
  
    dv_decode_full_frame(decoder, (uint8_t*)data, e_dv_color_rgb, pixels, pitches);
    ImageC<ByteRGBValueC>im(576, 720, (ByteRGBValueC*)&pixels[0][0], false);
 
    frameno++;
    return im;
  }


  /** gets the audio properties of this frame
 
  get the sampling frequency and the number of samples in this particular DV frame (which can vary)
 
  \param info the AudioInfo record
  \return true, if audio properties could be determined */

  bool 
  PalFrameC::GetAudioInfo(AudioInfo &info) const
  {
    int af_size;
    int smp;
    int flag;
    Pack pack50;

    /* Check whether this frame has a valid AAUX source packet
       (header == 0x50). If so, get the audio samples count. If not,
       skip this audio data. */

    if (GetAAUXPack(0x50, pack50) == true) {

      /* get size, sampling type and the 50/60 flag. The number of
	 audio samples is dependend on all of these. */

      af_size = pack50.data[1] & 0x3f;
      smp = (pack50.data[4] >> 3) & 0x07;
      flag = pack50.data[3] & 0x20;

      if (flag == 0) { // 60 frames (NTSC)
	info.frames = 60;
	switch (smp) {
	case 0:
	  info.frequency = 48000;
	  info.samples = 1580 + af_size;
	  break;
	case 1:
	  info.frequency = 44100;
	  info.samples = 1452 + af_size;
	  break;
	case 2:
	  info.frequency = 32000;
	  info.samples = 1053 + af_size;
	  break;
	}
      } else { // 50 frames (PAL)
	info.frames = 50;
	switch (smp) {
	case 0:
	  info.frequency = 48000;
	  info.samples = 1896 + af_size;
	  break;
	case 1:
	  info.frequency = 44100;
	  info.samples = 0; // I don't know
	  break;
	case 2:
	  info.frequency = 32000;
	  info.samples = 1264 + af_size;
	  break;
	}
      }
      return true;
    } else {
      return false;
    }
  }

  Array1dC<char>
  PalFrameC::Sound()
  {
    AudioInfo info;
  
    char *s = (char*)&sound[0];

    if (GetAudioInfo(info) == true) {
     
      switch (info.frequency) {
      case 32000:
      
	/* This is 4 channel audio */
      
	if (IsPAL()) {
	  short* p = (short*)&sound[0];
	  for (int n = 0; n < info.samples; ++n) {
	    register int r
	      = ((unsigned char*)data)[palmap_2ch1[n] + 1]; // LSB
	    *p++ = compmap[(((unsigned char*)data)[palmap_2ch1[n]] << 4)
			   + (r >> 4)];   // MSB
	    *p++ = compmap[(((unsigned char*)data)[palmap_2ch1[n] + 1] << 4)
			   + (r & 0x0f)];
	  }
	} else {
	  short* p = (short*)&sound[0];
	  for (int n = 0; n < info.samples; ++n) {
	    register int r
	      = ((unsigned char*)data)[ntscmap_2ch1[n] + 1]; // LSB
	    *p++ = compmap[(((unsigned char*)data)[ntscmap_2ch1[n]] << 4)
			   + (r >> 4)];   // MSB
	    *p++ = compmap[(((unsigned char*)data)[ntscmap_2ch1[n] + 1] << 4)
			   + (r & 0x0f)];
	  }
	}
	break;
      
      case 44100:
      case 48000:
      
	/* this can be optimized significantly */
      
	if (IsPAL()) {
	  for (int n = 0; n < info.samples; ++n) {
	    *s++ = ((char*)data)[palmap_ch1[n] + 1]; /* LSB */
	    *s++ = ((char*)data)[palmap_ch1[n]];     /* MSB */
	    *s++ = ((char*)data)[palmap_ch2[n] + 1]; /* LSB */
	    *s++ = ((char*)data)[palmap_ch2[n]];     /* MSB */
	  }
	} else {
	  for (int n = 0; n < info.samples; ++n) {
	    *s++ = ((char*)data)[ntscmap_ch1[n] + 1]; /* LSB */
	    *s++ = ((char*)data)[ntscmap_ch1[n]];     /* MSB */
	    *s++ = ((char*)data)[ntscmap_ch2[n] + 1]; /* LSB */
	    *s++ = ((char*)data)[ntscmap_ch2[n]];     /* MSB */
	  }
	}
	break;
      
	/* we can't handle any other format in the moment */
      
      default:
	info.samples = 0;
	break;
      }
    }
  
    return sound;
  }

  bool
  PalFrameC::IsPAL() const
  {
    unsigned char dsf = data[3] & 0x80;
    return dsf == 0 ? false : true;
  }


  //: Output your class members

  ostream & 
  operator<<(ostream & s, const PalFrameC & out)
  {
    RavlAssert(0); // Not implemented!
    return s;
  }


  bool 
  PalFrameC::GetSSYBPack(int packNum, Pack &pack) const
  {
    /* number of DIF sequences is different for PAL and NTSC */

    int seqCount = IsPAL() ? 12 : 10;

    /* process all DIF sequences */

    for (int i = 0; i < seqCount; ++i) {

      /* there are two DIF blocks in the subcode section */

      for (int j = 0; j < 2; ++j) {

	/* each block has 6 packets */

	for (int k = 0; k < 6; ++k) {

	  /* calculate address: 150 DIF blocks per sequence, 80 bytes
	     per DIF block, subcode blocks start at block 1, block and
	     packet have 3 bytes header, packet is 8 bytes long
	     (including header) */

	  const unsigned char *s = &data[i * 150 * 80 + 1 * 80 + j * 80 + 3 + k * 8 + 3];
	  // printf("ssyb %d: %2.2x %2.2x %2.2x %2.2x %2.2x\n",
	  // j * 6 + k, s[0], s[1], s[2], s[3], s[4]);
	  if (s[0] == packNum) {
	    pack.data[0] = s[0];
	    pack.data[1] = s[1];
	    pack.data[2] = s[2];
	    pack.data[3] = s[3];
	    pack.data[4] = s[4];
	    return true;
	  }
	}
      }
    }
    return false;
  }


  /** gets a video auxiliary data packet
 
  Every DIF block in the video auxiliary data section contains 15
  video auxiliary data packets, for a total of 45 VAUX packets. As
  the position of a VAUX packet is fixed, we could directly look it
  up, but I choose to walk through all data as with the other
  GetXXXX routines.
 
  \param packNum the VAUX package id to return
  \param pack a reference to the variable where the result is stored
  \return true for success, false if no pack could be found */

  bool 
  PalFrameC::GetVAUXPack(int packNum, Pack &pack) const
  {
    /* number of DIF sequences is different for PAL and NTSC */

    int seqCount = IsPAL() ? 12 : 10;

    /* process all DIF sequences */

    for (int i = 0; i < seqCount; ++i) {

      /* there are three DIF blocks in the VAUX section */

      for (int j = 0; j < 3; ++j) {

	/* each block has 15 packets */

	for (int k = 0; k < 15; ++k) {

	  /* calculate address: 150 DIF blocks per sequence, 80 bytes
	     per DIF block, vaux blocks start at block 3, block has 3
	     bytes header, packets have no header and are 5 bytes
	     long. */

	  const unsigned char *s = &data[i * 150 * 80 + 3 * 80 + j * 80 + 3 + k * 5];
	  // printf("vaux %d: %2.2x %2.2x %2.2x %2.2x %2.2x\n",
	  // j * 15 + k, s[0],  s[1],  s[2],  s[3],  s[4]);
	  if (s[0] == packNum) {
	    pack.data[0] = s[0];
	    pack.data[1] = s[1];
	    pack.data[2] = s[2];
	    pack.data[3] = s[3];
	    pack.data[4] = s[4];
	    return true;
	  }
	}
      }
    }
    return false;
  }


  /** gets an audio auxiliary data packet
 
  Every DIF block in the audio section contains 5 bytes audio
  auxiliary data and 72 bytes of audio data.  The function searches
  through all DIF blocks although AAUX packets are only allowed in
  certain defined DIF blocks.
 
  \param packNum the AAUX package id to return
  \param pack a reference to the variable where the result is stored
  \return true for success, false if no pack could be found */

  bool 
  PalFrameC::GetAAUXPack(int packNum, Pack &pack) const
  {
    /* number of DIF sequences is different for PAL and NTSC */

    int seqCount = IsPAL() ? 12 : 10;

    /* process all DIF sequences */

    for (int i = 0; i < seqCount; ++i) {

      /* there are nine audio DIF blocks */

      for (int j = 0; j < 9; ++j) {

	/* calculate address: 150 DIF blocks per sequence, 80 bytes
	   per DIF block, audio blocks start at every 16th beginning
	   with block 6, block has 3 bytes header, followed by one
	   packet. */

	const unsigned char *s = &data[i * 150 * 80 + 6 * 80 + j * 16 * 80 + 3];
	if (s[0] == packNum) {
	  // printf("aaux %d: %2.2x %2.2x %2.2x %2.2x %2.2x\n",
	  // j, s[0], s[1], s[2], s[3], s[4]);
	  pack.data[0] = s[0];
	  pack.data[1] = s[1];
	  pack.data[2] = s[2];
	  pack.data[3] = s[3];
	  pack.data[4] = s[4];
	  return true;
	}
      }
    }
    return false;
  }
 
} // end of namespace
