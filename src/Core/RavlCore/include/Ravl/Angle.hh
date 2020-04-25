// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVL_ANGLE_HEADER
#define RAVL_ANGLE_HEADER 1
////////////////////////////////////////////////////////
//! file="Ravl/Core/Math/Angle.hh"
//! lib=RavlCore
//! userlevel=Normal
//! author="Charles Galambos"
//! docentry="Ravl.API.Math"
//! rcsid="$Id$"
//! date="09/02/1998"

#include "Ravl/StdMath.hh"
#include "Ravl/StdConst.hh"
#include "Ravl/Types.hh"

namespace RavlN {

  
  //: This class is designed to deal with angles in radians.
  // It keeps the values normalised and provides arithmetic operations. <p>
  // A SMALL OBJECT
  
  class AngleC {
  public:
    inline AngleC(RealT anglerad = 0,RealT maxval = RavlConstN::pi * 2)
      : angle(anglerad),
	m_max(maxval)
    { Normalise(); }
    //: Construct from value in radians.
    // maxval is angle to wrap around at. 
    //  for directed lines this should be 2*pi.
    //  for undirected lines is should be pi.
    
    void Restrict(RealT newMax) {
      m_max = newMax;
      Normalise();
    }
    //: Restrict angle to values between 0 and newMax.
    
    inline void Normalise()
    { angle -= Floor(angle/m_max) * m_max; }
    //: Normalise the angle to values between 0 and max.

    inline RealT Normalise(RealT value) const
    { return value - Floor(value/m_max) * m_max; }
    //: Normalise the angle to values between 0 and max.
    // Returns the normalised angle.
    
    inline AngleC operator- (const AngleC &val) const
    { return AngleC(angle - val.angle,m_max); }
    //: Subtract angles.
    
    inline AngleC operator+ (const AngleC &val) const
    { return AngleC(angle + val.angle,m_max); }
    //: Add angles.
    
    inline const AngleC &operator-= (const AngleC &val) {
      angle -= val.angle;
      Normalise();
      return *this;
    }
    //: Subtract angles.
    
    inline const AngleC &operator+= (const AngleC &val)  {
      angle += val.angle;
      Normalise();
      return *this;
    }
    //: Add angles.

    inline bool IsBetween(RealT angle1,RealT angle2) const {
      RealT diff1 = Normalise(angle2  - angle1);
      RealT diff2 = Normalise(Value() - angle1);
      return (diff1 < diff2);
    }
    //: Test if this angle lies between angle1 and angle2.
    
    inline bool IsBetween(const AngleC &angle1,const AngleC &angle2) const 
    { return IsBetween(angle1.Value(),angle2.Value()); }
    //: Test if this angle lies between angle1 and angle2.
    
    inline RealT Diff(const AngleC &val) const;
    //: Find the difference between two angles.
    // it returns values in the rangle +/- max/2.
    
    inline RealT MaxAngle() const { return m_max; };
    //: Get maximum angle.
    
    inline RealT Value() const { return angle; }
    //: Get value of angle.
    
    inline RealT Sin() const { return RavlN::Sin(angle); }
    //: Get sin of angle.
    
    inline RealT Cos() const { return RavlN::Cos(angle); }
    //: Get cos of angle.
    
    void Dump(std::ostream &out);
    //: Dump to stream.
  protected:
    RealT angle;
    RealT m_max;
  };
  
  
  inline 
  RealT AngleC::Diff(const AngleC &val) const {
    RealT ret = angle - val.angle;
    RealT maxb2 = m_max / 2;
    if(ret > maxb2)
      ret -= m_max;
    else {
      if(ret < -maxb2)
	ret += m_max;
    }
    return ret;
  }
  
}


#endif
