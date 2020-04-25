// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2002, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//! rcsid="$Id$"
//! lib=RavlFireWire
//! file="Ravl/Contrib/FireWire/WavFile.cc"

#include"Ravl/Image/WavFile.hh"

namespace RavlImageN {

  WavFileC::WavFileC(FilenameC & fn, UIntT bitsPerSample, UIntT sampleRate, UIntT numChannels)
    : fname(fn), 
      BitsPerSample(bitsPerSample), 
      SampleRate(sampleRate), 
      NumChannels(numChannels)
  {
  
    BytesWritten=0;
    ofs = OStreamC(fn, true); // output binary stream

    UIntT data_length, data_length_without_hdr, byteRate;
  
    data_length_without_hdr = 0; // we will fill it in later
    data_length = data_length_without_hdr+36; // header size  // 36 = (8+16+12)
  
    byteRate = NumChannels * SampleRate; // this is wrong, should need to * by 2
    unsigned short blockAlign = NumChannels * (BitsPerSample/8);
  
    // building the header (http://ccrma-www.stanford.edu/CCRMA/Courses/422/projects/WaveFormat)
    ofs << "RIFF";
    ofs.write((char *)&data_length, 4);
    ofs << "WAVE";
    ofs << "fmt ";
    unsigned long PCMLength =16; // 16 for PCM
    ofs.write((char *)&PCMLength, 4);
    unsigned short PCMFormat = 1; // normaly WAVE_FORMAT_PCM = 1
    ofs.write((char *)&PCMFormat, 2);
    ofs.write((char *)&NumChannels, 2);
    ofs.write((char *)&SampleRate, 4);
    ofs.write((char *)&byteRate, 4);
    ofs.write((char *)&blockAlign, 2);
    ofs.write((char *)&BitsPerSample, 2);
    ofs << "data";
    ofs.write((char *)&data_length_without_hdr, 4);
    
    // now we can write data
  }


  void WavFileC::write(const Array1dC<char> & data) {
    ofs.write(&data[0], 7680);
    BytesWritten += 7680;
  }

  void WavFileC::Close() {
    UIntT tot = BytesWritten+36;
    ofs.Seek(4);
    ofs.write((char*)&tot, 4);
    ofs.Seek(40);
    ofs.write((char *)&BytesWritten, 4);
  }


} // end namespace
