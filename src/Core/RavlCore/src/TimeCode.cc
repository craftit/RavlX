// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2002, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//! rcsid="$Id$"
//! lib=RavlCore
//! file="Ravl/Core/System/TimeCode.cc"

#include "Ravl/TimeCode.hh"
#include "Ravl/String.hh"
#include "Ravl/StringList.hh"
#include "Ravl/BinStream.hh"
#include <stdlib.h>
#include <stdio.h>

namespace RavlN {

  //:  Construct timecode from 4 bytes 
  TimeCodeC::TimeCodeC(ByteT hr, ByteT min, ByteT sec, ByteT fr,RealT nFrameRate) 
    : frameRate(nFrameRate)
  { ConvertFrom(bcd(hr),bcd(min),bcd(sec),bcd(fr));  }
  

  //:  Construct timecode from byte array 
  TimeCodeC::TimeCodeC(ByteT *in,RealT nFrameRate)     
    : frameRate(nFrameRate)
  { ConvertFrom(bcd(in[3]),bcd(in[2]),bcd(in[1]),bcd(in[0])); }

  
  //:  Construct timecode from 4 ints 
  TimeCodeC::TimeCodeC(IntT hour, IntT min, IntT sec, IntT frame,RealT nFrameRate) 
    : frameRate(nFrameRate)
  { ConvertFrom(hour,min,sec,frame); }

  
  //: Construct timecode from absolute frame count
  TimeCodeC::TimeCodeC(const IntT frameNum,RealT nFrameRate) 
    : frameRate(nFrameRate)
  { m_liFrame = frameNum; }

  
  //: Construct timecode from string 
  TimeCodeC::TimeCodeC(const char * p,RealT nFrameRate) 
    : m_liFrame(0),
      frameRate(nFrameRate)
  { ConvertFrom(p); }
  

  //: Construct timecode from Ravl string
  TimeCodeC::TimeCodeC(const StringC & str, RealT nFrameRate )
    : m_liFrame(0),
      frameRate (nFrameRate)
  { ConvertFrom(str); }
  
  
  //: Convert to hours, minutes, seconds, frame
  bool TimeCodeC::ConvertTo(IntT &hour, IntT &minute, IntT &second, IntT &frame) const {
    if (frameRate == 0.0) return false;
    ldiv_t hr = ldiv(m_liFrame,(IntT) (frameRate * 3600.0) );
    //hour = Floor( 

    hour = (IntT) hr.quot;
    ldiv_t mn = ldiv(hr.rem, (IntT) (frameRate * 60.0));
    minute = (IntT)mn.quot;
    ldiv_t sc = ldiv(mn.rem, (IntT) frameRate);
    second = (IntT)sc.quot;
    frame = (IntT)sc.rem;
    return true;
  }

  
  //: Convert from hours, minutes, seconds, frame.
  bool TimeCodeC::ConvertFrom(IntT hr,IntT min,IntT sec,IntT fr) {
    m_liFrame = (IntT) (hr * (frameRate * 3600.0));
    m_liFrame +=(IntT) (min * (frameRate * 60.0));
    m_liFrame +=(IntT) (sec * frameRate);
    m_liFrame +=(IntT) fr;
    return true;
  }


  //: Convert from string 
  bool TimeCodeC::ConvertFrom  (const StringC& str) {
    StringListC items(str, ":");
    if (items.Size() != 4) return false;
    IntT hour = items.PopFirst().IntValue();
    IntT min = items.PopFirst().IntValue();
    IntT sec = items.PopFirst().IntValue();
    IntT frame = items.PopFirst().IntValue();
    if ((hour<24) && (hour>=0) && (min<60)  && (min>=0)
	&& (sec<60) && (sec>=0) && (frame<(IntT) frameRate) && (frame>=0)) 
      return ConvertFrom(hour,min,sec,frame) ;

    // Invalid value given.
    frameRate = RavlConstN::nanReal;
    return false;
  }

  //: Convert from string 
  bool TimeCodeC::ConvertFrom  (const char * p) {
    return ConvertFrom(StringC(p));
  }

  //: Checks for equals assignment
  bool TimeCodeC::operator==(const TimeCodeC &in) const {
    if (frameRate == in.FrameRate())
      return (m_liFrame == in.m_liFrame);
    else
      return (m_liFrame*in.FrameRate() == in.m_liFrame*frameRate);
  }
  
  
  //: Checks for not equals assignment
  bool TimeCodeC::operator!=(const TimeCodeC &in) {
    if (frameRate == in.FrameRate())
      return (m_liFrame != in.m_liFrame);
    else
      return (m_liFrame*in.FrameRate() != in.m_liFrame*frameRate);
  }


  //: Checks if timecode smaller than input timecode
  bool TimeCodeC::operator<(const TimeCodeC &in) const {
    if (frameRate == in.FrameRate())
      return (m_liFrame < in.m_liFrame);
    else
      return (m_liFrame*in.FrameRate() < in.m_liFrame*frameRate);
  }
  
  
  //: Checks if timecode is bigger than input
  bool TimeCodeC::operator>(const TimeCodeC &in) const {
    if (frameRate == in.FrameRate())
      return (m_liFrame > in.m_liFrame);
    else
      return (m_liFrame*in.FrameRate() > in.m_liFrame*frameRate);
  }
  
  
  //: Checks if timecode is bigger or equal to input
  bool TimeCodeC::operator<=(const TimeCodeC &in) const {
    if (frameRate == in.FrameRate())
      return (m_liFrame <= in.m_liFrame);
    else
      return (m_liFrame*in.FrameRate() <= in.m_liFrame*frameRate);
  }
  
  
  //: Checks if timecode is smaller or equal to input
  bool TimeCodeC::operator>=(const TimeCodeC &in) const {
    if (frameRate == in.FrameRate())
      return (m_liFrame >= in.m_liFrame);
    else
      return (m_liFrame*in.FrameRate() >= in.m_liFrame*frameRate);
  }
  
  
  //: Adds timecodes
  TimeCodeC TimeCodeC::operator+(const TimeCodeC & in) const {
    IntT newFrameCount = m_liFrame + in.m_liFrame;
    return TimeCodeC(newFrameCount, frameRate);
  }

  
  //: Subtracts timecodes
  TimeCodeC TimeCodeC::operator-(const TimeCodeC & in) const {
    IntT newFrameCount = m_liFrame - in.m_liFrame;
    return TimeCodeC(newFrameCount, frameRate);
  }


  //: Adds input timecode to this timecode
  TimeCodeC &TimeCodeC::operator+=(const TimeCodeC & in) {
    m_liFrame += in.m_liFrame;
    return *this;
  }
  
  
  //: Adds frame count to timecode  
  TimeCodeC &TimeCodeC::operator+=(IntT in) {
    m_liFrame += in;
    return *this;
  }


  //: Subtracts input timecode from this
  TimeCodeC &TimeCodeC::operator-=(const TimeCodeC & in) {
    m_liFrame -= in.m_liFrame;
    return *this;
  }
  
  
  //: Subtracts frame count from timecode
  TimeCodeC &TimeCodeC::operator-=(IntT in) {
    m_liFrame -= in;
    return *this;
  }


  //: Increments timecode by 1 frame
  TimeCodeC &TimeCodeC::operator++() {
    ++m_liFrame;
    return *this;
  }
    
  //: Return a string  representation of timecode
  StringC TimeCodeC::ToText() const {
    IntT hour,minute,second,frame;
    if (ConvertTo(hour,minute,second,frame)) {
	  StringC ret;
      ret.form("%.2d:%.2d:%.2d:%.2d", hour, minute, second, frame);
      return ret;
    }
    else return StringC("--:--:--:--");
  }

  
  //: I/O operators 
  //: Write time code out to stream.
  std::ostream &operator<<(std::ostream & s, const TimeCodeC & tc) {
    s << tc.m_liFrame << " " << tc.frameRate ;
    return s;
  }


  //: Read time code in from stream.
  std::istream &operator>>(std::istream & s, TimeCodeC & tc) {
    s >> tc.m_liFrame >> tc.frameRate ; 
    return s;
  }

  
  //: Write time code to a binary stream 
  BinOStreamC & operator << (BinOStreamC & s, const TimeCodeC & tc ) {
    s << tc.m_liFrame << tc.frameRate ; 
    return s ; 
  }


  //: Read time code in from binary stream 
  BinIStreamC & operator >> (BinIStreamC & s, TimeCodeC & tc ) {
    s >> tc.m_liFrame >> tc.frameRate ; 
    return s ; 
  }


} // end namespace RavlN
 
