// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2003, OmniPerception Ltd.
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVLAUDIO_SEGMENT_HEADER
#define RAVLAUDIO_SEGMENT_HEADER 1
////////////////////////////////////////////////////////////////////////
//! author="Charles Galambos"
//! docentry="Ravl.API.Audio.Feature Extraction"
//! rcsid="$Id$"
//! lib=RavlAudioUtil
//! file="Ravl/Audio/Util/Segment.hh"

#include "Ravl/String.hh"
#include "Ravl/IndexRange1d.hh"

namespace RavlAudioN {
  using namespace RavlN;
  
  //! userlevel=Normal
  //: Labeled Segment.
  // Range is in samples.
  
  class SegmentC {
  public:
    SegmentC();
    //: Default constructor.

    SegmentC(const IndexRangeC &sampleRange,const StringC &lab)
      : range(sampleRange),
	label(lab)
    {}
    //: Constructor.
    
    SegmentC(IntT start,IntT end,const StringC &lab)
      : range(start,end),
	label(lab)
    {}
    //: Constructor.
    
    IndexRangeC &SampleRange()
    { return range; }
    //: Access range.
    
    const IndexRangeC &SampleRange() const
    { return range; }
    //: Access range.
    
    StringC &Label()
    { return label; }
    //: Access label
    
    const StringC &Label() const
    { return label; }
    //: Access label
    
  protected:    
    IndexRangeC range;
    StringC label;
  };
}


#endif
