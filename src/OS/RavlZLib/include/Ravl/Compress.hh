// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2005, OmniPerception Ltd.
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVL_COMPRESS_HEADER
#define RAVL_COMPRESS_HEADER 1
//! docentry="Ravl.API.OS.Compression"
//! author="Charles Galambos"
//! lib=RavlZLib

#include "Ravl/SArray1d.hh"
#include "Ravl/String.hh"

namespace RavlN {
  
  bool CompressZLib(const SArray1dC<char> &data,SArray1dC<char> &buff,IntT level = 9);
  //: zlib wrapper, Compress data into 'buff'
  //!param: data - Data to be compressed.
  //!param: buff - Compressed data.
  
  bool DecompressZLib(const SArray1dC<char> &buff,SArray1dC<char> &data,UIntT originalSize);
  //: zlib wrapper, Decompress buff into data. 
  //!param: buff - Compressed data.
  //!param: data - Uncompressed data.
  //!param: originalSize - must be the original size in bytes of the data or greater.

  bool CompressZLib(const StringC &data,StringC &buff,IntT level = 9);
  //: zlib wrapper, Compress data into 'buff'
  //!param: data - Data to be compressed.
  //!param: buff - Compressed data.

  bool DecompressZLib(const StringC &buff,StringC &data,UIntT originalSize);
  //: zlib wrapper, Decompress buff into data.
  //!param: buff - Compressed data.
  //!param: data - Uncompressed data.
  //!param: originalSize - must be the original size in bytes of the data or greater.

  
}


#endif
