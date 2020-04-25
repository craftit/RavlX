// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2002, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
////////////////////////////////////////////////////////////////
//! author="Charles Galambos"
//! date="24/7/2002"
//! rcsid="$Id$"
//! file="Ravl/Image/Processing/Tracking/PointTrack.cc"
//! lib=RavlImageProc

#include "Ravl/Image/PointTrack.hh"
#include "Ravl/Stream.hh"
#include "Ravl/BinStream.hh"
#include "Ravl/Exception.hh"

namespace RavlImageN {
  
  //: Read from binary stream.
  
  PointTrackC::PointTrackC(BinIStreamC &strm) {
    ByteT version = 0;
    strm >> version;
    if(version != 1)
      throw ExceptionOperationFailedC("Unexpected version number in stream.");
    strm >> id >> at >> conf;
  }
  
  //: Write to binary stream.
  
  bool PointTrackC::Save(BinOStreamC &strm) const {
    ByteT version = 1;
    strm << version << id << at << conf;
    return true;
  }
  
  //: Save to std::ostream.
  
  std::ostream &operator<<(std::ostream &strm,const PointTrackC &pt) {
    strm << pt.ID() << ' ' << pt.Location() << ' ' << pt.Confidence() << "\n";
    return strm;
  }
  
  //: Load from std::istream.
  
  std::istream &operator>>(std::istream &strm,PointTrackC &pt) {
    strm >> pt.id >> pt.at >> pt.conf;
    return strm;
  }

  //: Save to binary stream
  
  BinOStreamC &operator<<(BinOStreamC &strm,const PointTrackC &pt) {
    pt.Save(strm);
    return strm;
  }
  
  //: Load from binary stream
  
  BinIStreamC &operator>>(BinIStreamC &strm,PointTrackC &pt) {
    pt = PointTrackC(strm);
    return strm;
  }
  
}
