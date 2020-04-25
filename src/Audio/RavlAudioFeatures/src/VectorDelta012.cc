// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2003, OmniPerception Ltd.
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//! rcsid="$Id$"
//! lib=RavlAudioFeatures
//! file="Ravl/Audio/Features/VectorDelta012.cc"

#include "Ravl/Audio/VectorDelta012.hh"
#include "Ravl/FixedQueue.hh"
#include "Ravl/SArray1dIter2.hh"
#include "Ravl/XMLFactoryRegister.hh"

#define DODEBUG 0
#if DODEBUG
#define ONDEBUG(x) x
#else
#define ONDEBUG(x)
#endif

namespace RavlAudioN {
  
  //: XML factory constructor

  VectorDelta012C::VectorDelta012C(const XMLFactoryContextC &context)
    : DPProcessBodyC<VectorC,VectorC>(),
      samples(5),
      delta1(3),
      features(context.AttributeUInt("features",0))
  {
    if(features > 0)
      Init(features);
  }

  //: Constructor
  
  VectorDelta012C::VectorDelta012C(UIntT fawFeatures,const VectorC &mean)
    : samples(5),
      delta1(3),
      features(fawFeatures)
  {
    Init(fawFeatures,mean);
  }

  void VectorDelta012C::Init(UIntT numFeatures,const VectorC &mean) {
    VectorC zero(numFeatures);
    zero.Fill(0);
    if(mean.IsValid()) {
      samples.InsLast(mean);
      samples.InsLast(mean);
      samples.InsLast(mean);
      samples.InsLast(mean);
      samples.InsLast(mean);
    } else {
      samples.InsLast(zero);
      samples.InsLast(zero);
      samples.InsLast(zero);
      samples.InsLast(zero);
      samples.InsLast(zero);
    }
    delta1.InsLast(zero);
    delta1.InsLast(zero);
    delta1.InsLast(zero);
  }

  //: Compute feature vector.
  
  VectorC VectorDelta012C::Apply(const VectorC &rawFeatures) {
    // Need to initialise ?
    if(features == 0) {
      features = rawFeatures.Size();
      Init(features);
    }

    VectorC ret(features * 3);
    ret.Fill(-1);
    samples.DelFirst();
    samples.InsLast(rawFeatures);
    FixedQueueRevIterC<VectorC> qit(samples);
    VectorC dp2 = *qit;
    qit++;
    qit++;
    VectorC base = *qit;
    qit++;
    qit++;
    VectorC diff = (dp2 - *qit)/4;
    
    delta1.DelFirst();
    delta1.InsLast(diff);
    
    FixedQueueRevIterC<VectorC> dit(delta1);
    VectorC dm = *dit;
    dit++;
    VectorC dc = *dit;
    dit++;
    VectorC dp = *dit;
    VectorC d2 = (dm - dp)/2;
    
    for(SArray1dIter2C<RealT,RealT> it(base,ret);it;it++)
      it.Data2() = it.Data1();
    for(SArray1dIter2C<RealT,RealT> it(dc,ret.From(features));it;it++)
      it.Data2() = it.Data1();
    for(SArray1dIter2C<RealT,RealT> it(d2,ret.From(features * 2));it;it++)
      it.Data2() = it.Data1();
    
    return ret;
  }

  void LinkVectorDelta12()
  {}

  static RavlN::XMLFactoryRegisterConvertC<VectorDelta012C,DPProcessBodyC<VectorC,VectorC> > g_registerVectorDelta012("RavlAudioN::VectorDelta012C");
}
