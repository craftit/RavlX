// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2006-12, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVL_LOGPROB_HEADER
#define RAVL_LOGPROB_HEADER 1
//! author="Jack Longton"
//! docentry="Ravl.API.Math"
//! date="10/07/2006"
//! userlevel=Normal
//! file="Ravl/Core/Math/LogProb.hh"
//! lib=RavlCore

#include "Ravl/Types.hh"
#include "Ravl/Assert.hh"
#include "Ravl/Math.hh"

namespace RavlN{
  
  //: Store probability as log value.
  // This avoids problems with underflow allowing for the representation of extremely small probabilities

  class LogValueC
  {
    public:
    LogValueC()
	:v(-99999999)
    {}
    //: Constructs with a probability near zero.
    
    LogValueC(double prob) {
      if(prob == 0) v = -99999999;
      else v = RavlN::Log(prob);
    }
    //: Construct with given probability.
    
    LogValueC(RealT prob, bool)
      : v(prob)
    {}
    //: Construct from log value.
    
    LogValueC Copy() const 
    { return LogValueC(v,true); }
    //: Copy this value.
    
    void SetZero()
    { v = -99999999; }
    //: Set this value to zero.
    
    LogValueC operator*(const LogValueC & prob) const 
    { return LogValueC(v+prob.v,true);}
    
    LogValueC operator/(const LogValueC & prob) const 
    { return LogValueC(v-prob.v,true);} 
    
    LogValueC operator+(const LogValueC & prob) const 
    { return LogValueC(v + RavlN::Log(1+Exp(prob.v-v)),true); }
    
    LogValueC operator-(const LogValueC & prob) const 
    { return LogValueC(v + RavlN::Log(1+Exp(-prob.v-v)),true); }
    
    LogValueC operator*=(const LogValueC & prob) 
    { v += prob.v; return LogValueC(v,true); }
    
    LogValueC operator/=(const LogValueC & prob)
    { v -= prob.v; return LogValueC(v,true); } 
     
    LogValueC operator+=(const LogValueC & prob) 
    { v += RavlN::Log(1+Exp(prob.v-v)); return LogValueC(v,true); }
    
    LogValueC operator-=(const LogValueC & prob) 
    { v -= RavlN::Log(1+Exp(-prob.v-v)); return LogValueC(v,true); }
    
    bool operator >(const LogValueC & prob) const 
    { return prob.v < v; }
    
    bool operator < (const LogValueC & prob) const 
    { return prob.v > v; }
    
    bool operator >=(const LogValueC & prob) const 
    { return prob.v <= v; }
    
    bool operator <= (const LogValueC & prob) const 
    { return prob.v >= v; }
    
    bool operator == (const LogValueC & prob) const 
    { return prob.v == v; }
    //: Equals operator.
    
    RealT Value() const
    { return Exp(v); }
    //: Return the value 
    
    RealT Log() const
    { return v; }
    //: Return the log of the value
    
  protected:
    RealT v; // Log of value.
  };

  std::ostream &operator<<(std::ostream &out,const LogValueC &x);  
  std::istream &operator>>(std::istream &in,LogValueC &x);
  
  BinOStreamC &operator<<(BinOStreamC &out,const LogValueC &x);
  BinIStreamC &operator>>(BinIStreamC &in,LogValueC &x);
}

#endif
