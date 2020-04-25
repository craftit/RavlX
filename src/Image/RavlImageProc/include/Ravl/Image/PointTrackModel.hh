// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2002, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVLIMAGE_POINTTRACKMODEL_HEADER
#define RAVLIMAGE_POINTTRACKMODEL_HEADER 1
/////////////////////////////////////////////////////
//! rcsid="$Id$"
//! author="Charles Galambos"
//! date="24/7/2002"
//! docentry="Ravl.API.Images.Tracking"
//! file="Ravl/Image/Processing/Tracking/PointTrackModel.hh"
//! lib=RavlImageProc

#include "Ravl/Array2d.hh"
#include "Ravl/Point2d.hh"
#include "Ravl/Vector2d.hh"
#include "Ravl/Image/PointTrack.hh"

namespace RavlImageN {
  using namespace RavlN;
  
  class PointTrackModelC;
  
  std::ostream &operator<<(std::ostream &strm,const PointTrackModelC &pt);
  //: Save to std::ostream.
  
  std::istream &operator>>(std::istream &strm,PointTrackModelC &pt);
  //: Load from std::istream.

  //:-
  
  //! userlevel=Develop
  //: Tracking information on a single point.
  
  class PointTrackModelBodyC : public RCBodyC
  {
  public:
    PointTrackModelBodyC(UIntT nid,const Index2dC &nat,IntT frameNo,const Array2dC<ByteT> &ntempl,RealT nremoveThreshold)
      : id(nid),
	at(nat),
	vel(0,0),
	frame(frameNo),
	matchScore(0),
	templ(ntempl),
	live(true),
	removeThreshold(nremoveThreshold)
    {}
    //: Create new track.
    
    UIntT ID() const
    { return id; }
    //: Access track ID.
    
    const Point2dC &Location() const
    { return at; }
    //: Access last location that object was seen.
    
    Point2dC EstimateLocation(IntT frameNo)
    { return at + vel * (frameNo - frame); }
    //: Estimate location in given frame.
    // Simple estimate on next position based on the points velocity.
    
    IntT Frame() const
    { return frame; }
    //: Access frame number object was last seen.
    
    void Update(const Point2dC &nat,UIntT frameNo,RealT nmatchScore) { 
      vel = (nat - at) / (RealT)(frameNo - frame);
      at = nat;
      frame = frameNo;
      matchScore = nmatchScore;
    }
    //: Update a track.
    
    Array2dC<ByteT> &Template()
    { return templ; }
    //: Access template.
    
    const Vector2dC &Velocity() const
    { return vel; }
    //: Estimated velocity of point.
    
    RealT MatchScore() const
    { return matchScore; }
    //: Get the last match score.
    
    RealT Confidence() const;
    //: Get confidence measurement between 0 and 1 for the last frame

    bool IsLive() const
    { return live; }
    //: Is this a live track ?
    
    void SetLive(bool nlive)
    { live = nlive; }
    //: Set live flag for track.
    // Note: If the point tracker find a a track model with this flag set to false
    // it will remove it from the track list. This can be used to remove tracks without
    // editing the list directly.
    
  protected:
    UIntT id;      // Point ID.
    Point2dC at;  // Point at which it was last seen.
    Vector2dC vel; // Difference between previous two positions.
    UIntT frame;   // Frame number.
    RealT matchScore; // Last match score.
    Array2dC<ByteT> templ; // Template of feature.
    bool live;   // Is this a live track ?
    RealT removeThreshold;
  };
  
  class PointTrackModelC : public RCHandleC<PointTrackModelBodyC>
  {
  public:
    PointTrackModelC()
    {}
    //: Default constructor.
    // Creates an invalid handle.

    PointTrackModelC(UIntT nid,const Index2dC &nat,IntT frameNo,const Array2dC<ByteT> &ntempl,RealT nremoveThreshold)
      : RCHandleC<PointTrackModelBodyC>(*new PointTrackModelBodyC(nid,nat,frameNo,ntempl,nremoveThreshold))
    {}
    //: Create new track.
    
  protected:
    PointTrackModelBodyC &Body()
    { return RCHandleC<PointTrackModelBodyC>::Body(); }
    //: Access body.
    
    const PointTrackModelBodyC &Body() const
    { return RCHandleC<PointTrackModelBodyC>::Body(); }
    //: Access body.
    
  public:

    UIntT ID() const
    { return Body().ID(); }
    //: Access track ID.
    
    const Point2dC &Location() const
    { return Body().Location(); }
    //: Access last location that object was seen.
    
    Point2dC EstimateLocation(IntT frameNo)
    { return Body().EstimateLocation(frameNo); }
    //: Estimate location in given frame.
    // Simple estimate on next position based on the points velocity.
    
    IntT Frame() const
    { return Body().Frame(); }
    //: Access frame number object was last seen.
    
    void Update(const Point2dC &nat,UIntT frameNo,IntT nmatchScore) 
    { Body().Update(nat,frameNo,nmatchScore); }
    //: Update a track.
    
    Array2dC<ByteT> &Template()
    { return Body().Template(); }
    //: Access template.
    
    const Vector2dC &Velocity() const
    { return Body().Velocity(); }
    //: Estimated velocity of point.
    
    RealT MatchScore() const
    { return Body().MatchScore(); }
    //: Get the last match score.
    
    RealT Confidence() const
    { return Body().Confidence(); }
    //: Get confidence measurement between 0 and 1 for the last frame

    bool IsLive() const
    { return Body().IsLive(); }
    //: Is this a live track ?
    
    void SetLive(bool nlive)
    { Body().SetLive(nlive); }
    //: Set live flag for track.
    // Note: If the point tracker find a a track model with this flag set to false
    // it will remove it from the track list. This can be used to remove tracks without
    // editing the list directly.
    
  };

}


#endif
