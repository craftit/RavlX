// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2002, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVL_TIMECODE_HEADER 
#define RAVL_TIMECODE_HEADER 1
////////////////////////////////////////////////////////////////////////////
//! rcsid="$Id$"
//! date="14/05/2002"
//! lib=RavlCore
//! file="Ravl/Core/System/TimeCode.hh"
//! docentry="Ravl.API.Core.Misc"
//! userlevel=Normal
//! author="Kieron J Messer"

#include "Ravl/Assert.hh"
#include "Ravl/Types.hh"
#include "Ravl/Math.hh" 
#include "Ravl/StdMath.hh"

namespace RavlN {
#if RAVL_VISUALCPP_NAMESPACE_BUG && RAVL_HAVE_STDNAMESPACE
  using namespace std;
#endif
  
  class StringC;
  
  //! userlevel=Normal
  //: Time code.
  
  class TimeCodeC  
  {
  public:
    
    //:------------------
    // Constructors
    
    TimeCodeC(ByteT hr, ByteT min, ByteT sec, ByteT fr,RealT frameRate = 25.0);
    //: Construct timecode from 4 bytes
    
    TimeCodeC(ByteT * in,RealT frameRate = 25.0);
    //: Construct timecode from byte array
    
    TimeCodeC(IntT hr, IntT min, IntT sec, IntT fr,RealT frameRate = 25.0);
    //: Construct timecode from 4 ints
    
    TimeCodeC(const IntT nFrames,RealT frameRate = 25.0);
    //: Construct timecode from absolute frame count
    
    TimeCodeC(const char * string,RealT frameRate = 25.0);
    //: Construct from a valid string representation
    // expects format hh:mm:ss:ff 

    TimeCodeC(const StringC & string, RealT frameRate = 25.00) ; 
    //: Construct from a Ravl string representation 
    
    inline TimeCodeC()
      : m_liFrame(0),
	frameRate(25.0)
    {}
    //: Construct empty timecode

    static TimeCodeC Max(RealT FrameRate = 25.0)
    { return TimeCodeC(RavlConstN::maxInt, FrameRate); }
    //: Return max possible timecode

    
    //:-------------------
    //: Operators

    bool operator==(const TimeCodeC &in) const; 
    //: Checks for equals assignment
    
    bool operator!=(const TimeCodeC &in);
    //: Checks for not equals assignment
    
    bool operator<(const TimeCodeC &in) const;
    //: Checks if timecode smaller than input timecode
    
    bool operator>(const TimeCodeC &in) const;
    //: Checks if timecode is bigger than input
    
    bool operator>=(const TimeCodeC &in) const;
    //: Checks if timecode is bigger or equal to input
    
    bool operator<=(const TimeCodeC &in) const;
    //: Checks if timecode is smaller or equal to input
    
    TimeCodeC operator+(const TimeCodeC & in) const;
    //: Adds timecodes
    
    inline TimeCodeC operator+(const IntT & frames) const
      { return TimeCodeC(m_liFrame+frames, frameRate); }
    //: Adds no. of frames  to timecode
    
    TimeCodeC operator-(const TimeCodeC & in) const;
    //: Subtracts timecodes
    
    inline TimeCodeC operator-(const IntT & frames) const
      { return TimeCodeC(m_liFrame-frames, frameRate); }
    //: Subtracts no. of frames from timecode
    
    TimeCodeC & operator+=(const TimeCodeC & in);
    //: Adds input timecode to this timecode
    
    TimeCodeC & operator+=(IntT frame);
    //: Adds frame count to timecode
    
    TimeCodeC & operator-=(const TimeCodeC & in);
    //: Subtracts input timecode from this

    TimeCodeC & operator-=(IntT frame);
    //: Subtracts frame count from timecode
    
    TimeCodeC & operator++();
    //: Increments timecode by 1 frame
    
    friend std::ostream &operator<<(std::ostream &s, const TimeCodeC &out);
    //: Output stream for timecode
    
    friend std::istream &operator>>(std::istream &s, TimeCodeC &inds);
    //: Input stream for timecode

    friend BinOStreamC & operator << (BinOStreamC & s, const TimeCodeC & tc ); 
    //: Write time code to a binary stream in the form: frameNo frameRate
    
    friend BinIStreamC & operator >> (BinIStreamC & s, TimeCodeC & tc ); 
    //: Read time code in from binary stream in the form: frameNo frameRate
    
    //:----------------------
    //: Member Functions
    
    inline UIntT Hash() const
      { return (UIntT) m_liFrame; }
    //:: the hash key
    
    inline IntT bcd(ByteT in)
      { return (in >> 4) * 10  + (in & 0xf);}
    //: Routine to convert binary coded decimal to int
    
    StringC ToText() const;
    //: Return a string  representation of timecode
    
    inline IntT NumberOfFramesTo(const TimeCodeC &in) const 
      { return Abs(m_liFrame - in.m_liFrame); }    
    //: Count the number of frames to a timecode
    
    inline bool IsValid() const
      { return ((m_liFrame>=0) && !IsNan(frameRate) && (frameRate>0.0)); }
    //: Checks whether the timecode holds valid data
        
    inline IntT FrameCount() const
      {return m_liFrame;}
    //: Access frame count.

    bool ConvertTo(IntT &hr, IntT &min, IntT &sec, IntT &fr) const;
    //: Convert to hours, minutes, seconds, frame.
    
    bool ConvertFrom(IntT hr,IntT min,IntT sec,IntT fr);
    //: Convert from hours, minutes, seconds, frame.

    bool ConvertFrom  (const StringC& str);
    //: Convert from a string
    // expects format hh:mm:ss:ff 

    bool ConvertFrom  (const char * string) ;
    //: Convert from a string
    // expects format hh:mm:ss:ff 

    inline RealT FrameRate() const
      { return frameRate; }
    //: Access the frame rate

    inline int getFrameCount() const
      {return m_liFrame;}
    //: Access frame count.
    //!deprecated: Use <code>FrameCount()</code>

  protected:
    
    IntT m_liFrame ; 
    //: Absolute frame count of timecode
    
    RealT frameRate;
    //: Reference frame rate.

  };
  
  std::ostream &operator<<(std::ostream &s, const TimeCodeC &out);
  //: Write time code out to stream.
  
  std::istream &operator>>(std::istream &s, TimeCodeC &tc);
  //: Read time code in from stream.
  
  BinOStreamC & operator << (BinOStreamC & s, const TimeCodeC & tc ); 
  //: Write time code to a binary stream in the form: frameNo frameRate
  
  BinIStreamC & operator >> (BinIStreamC & s, TimeCodeC & tc ); 
  //: Read time code in from binary stream in the form: frameNo frameRate
    
} // end namespace RavlN

#endif
