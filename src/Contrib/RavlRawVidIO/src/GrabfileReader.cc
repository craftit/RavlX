// This file is part of RAVL, Recognition And Vision Library
// Copyright (C) 2001-12, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#include "Ravl/Image/GrabfileReader.hh"
#include "Ravl/Assert.hh"
//! lib=RavlRawVidIO

#include <fstream>
#include <arpa/inet.h>

namespace RavlImageN {

//--------------------------------------------------------------------------//

const bool GrabfileReaderC::IsGrabfile(const char* const filename)
{
  std::ifstream infile(filename, std::ios::binary | std::ios::in);

  const bool match = IsGrabfile(infile);

  infile.close();

  return match;
}

//--------------------------------------------------------------------------//

const bool GrabfileReaderC::IsGrabfile(std::ifstream& infile)
{
  char file_id[4] = {0x00, 0x00, 0x00, 0x00};
  infile.read(file_id, 4);

  const bool match = (
                      GrabfileCommonN::GrabfileFOURCC[0] == file_id[0] &&
                      GrabfileCommonN::GrabfileFOURCC[1] == file_id[1] &&
                      GrabfileCommonN::GrabfileFOURCC[2] == file_id[2] &&
                      GrabfileCommonN::GrabfileFOURCC[3] == file_id[3]
                      );

  return match;
}

//--------------------------------------------------------------------------//

const int GrabfileReaderC::FileVersion(const char* const filename)
{
  uint32_t version = 0;

  std::ifstream infile(filename, std::ios::binary | std::ios::in);

  if(IsGrabfile(infile)) {
    version = FileVersion(infile);
  }

  infile.close();

  return version;
}

//--------------------------------------------------------------------------//

const int GrabfileReaderC::FileVersion(ifstream& infile)
{
  uint32_t version = 0;

  // Read the version
  if(infile.good()) {
    infile.read(reinterpret_cast<char*>(&version), 4);
  }

  return ntohl(version);
}

} // End DVSN namespace
