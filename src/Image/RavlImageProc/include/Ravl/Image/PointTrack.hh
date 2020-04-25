// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2002, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVLIMAGE_POINTTRACK_HEADER
#define RAVLIMAGE_POINTTRACK_HEADER 1
//////////////////////////////////////////////////////
//! userlevel=Normal
//! author="Charles Galambos"
//! date="24/7/2002"
//! rcsid="$Id$"
//! docentry="Ravl.API.Images.Tracking"
//! file="Ravl/Image/Processing/Tracking/PointTrack.hh"
//! lib=RavlImageProc

#include "Ravl/Point2d.hh"
#include "Ravl/Types.hh"

namespace RavlImageN {
  using namespace RavlN;
  
  class PointTrackC;
  
  std::ostream &operator<<(std::ostream &strm,const PointTrackC &pt);
  //: Save to std::ostream.
  
  std::istream &operator>>(std::istream &strm,PointTrackC &pt);
  //: Load from std::istream.
  
  //:-
  
  //! userlevel=Normal
  //: Information on a single point track.
  
  class PointTrackC {
  public:
    PointTrackC()
    {}
    //: Default constructor.
    // Leave the track in an undefined state.

    PointTrackC(UIntT nid,const Point2dC &nat,RealT confidence)
      : id(nid),
	at(nat),
	conf(confidence)
    {}
    //: Constructor.
    
    PointTrackC(BinIStreamC &strm);
    //: Read from binary stream.
    
    bool Save(BinOStreamC &strm) const;
    //: Write to binary stream.
    
    UIntT ID() const
    { return id; }
    //: ID for track.
    
    const Point2dC &Location() const
    { return at; }
    //: Location of track.
    
    RealT Confidence() const
    { return conf; }
    //: Confidence in track
    // 0=No information was found on the track in this frame.
    // 1=Ideal match found.
    
  protected:
    UIntT id;
    Point2dC at;
    RealT conf;
    
    friend std::istream &operator>>(std::istream &strm,PointTrackC &pt);
  };
  
  BinOStreamC &operator<<(BinOStreamC &strm,const PointTrackC &pt);
  //: Save to binary stream
  
  BinIStreamC &operator>>(BinIStreamC &strm,PointTrackC &pt);
  //: Load from binary stream
  
  
}

#endif
