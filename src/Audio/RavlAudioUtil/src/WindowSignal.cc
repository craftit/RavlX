// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2003, OmniPerception Ltd.
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//! lib=RavlAudioUtil
//! author="Charles Galambos"
//! docentry="Ravl.Audio.Processing"
//! file="Ravl/Audio/Util/WindowSignal.cc"

#include "Ravl/Audio/WindowSignal.hh"
#include "Ravl/Math.hh"
#include "Ravl/StdConst.hh"
#include "Ravl/SysLog.hh"
#include "Ravl/XMLFactoryRegister.hh"

namespace RavlAudioN {
  
  static struct {
    const char *m_name;
    RAWindowSignalT m_windowType;
  } g_RAWindowSignalNames [] =
  {
    {"None",RAWNone},
    {"Ramp",RAWRamp },
    {"Hanning",RAWHanning},
    {"Hamming",RAWHamming},
    {"Blackman",RAWBlackman},
    {"Custom",RAWCustom},
    {0,RAWCustom},
  };

  RAWindowSignalT String2WindowType(const StringC &windowTypeName)
  {
    for(int i = 0;g_RAWindowSignalNames[i].m_name != 0;i++) {
      if(windowTypeName == g_RAWindowSignalNames[i].m_name) {
        return g_RAWindowSignalNames[i].m_windowType;
      }
    }
    RavlError("Unknown window type '%s' ",windowTypeName.c_str());
    throw RavlN::ExceptionBadConfigC("Unknown window type '%s' ",windowTypeName.c_str());
    return RAWCustom;
  }
  //: Convert string to window type
  // Throw an exception if none found.


  //: Constructor.
  
  WindowSignalBaseC::WindowSignalBaseC(RAWindowSignalT nwinType,UIntT nsize,UIntT frameSeperation)
    : winType(nwinType),
      winSize(nsize),
      frameSep(frameSeperation)
  {}

  //: XML factory constructor.
  
  WindowSignalBaseC::WindowSignalBaseC(const XMLFactoryContextC &factory)
   : winType(String2WindowType(factory.AttributeString("windowType","Ramp"))),
     winSize(factory.AttributeUInt("windowSize",512)),
     frameSep(factory.AttributeUInt("frameStep",256))
  {}

  //: Generate the filter.
  
  bool WindowSignalBaseC::Generate(SArray1dC<RealT> &filter) {
    switch(winType){
    case RAWNone:
      filter = SArray1dC<RealT>(winSize);
      filter.Fill(1);
      break;
    case RAWRamp:    return GenerateRamp(filter);
    case RAWHanning: return GenerateHanning(filter);
    case RAWHamming: return GenerateHamming(filter);
    case RAWBlackman: return GenerateBlackman(filter);
    default:
      RavlAssertMsg(0,"WindowSignalBaseC::Generate(), Unknown filter type.");
      throw RavlN::ExceptionBadConfigC("Unknown filter type");
    }
    return true;
  }
  
  //: Generate the filter.

  bool WindowSignalBaseC::Generate(SArray1dC<float> &filter) {
    SArray1dC<RealT> rfilter;
    if(!Generate(rfilter))
      return false;
    SArray1dC<float> ffilter(rfilter.Size());
    for(unsigned i = 0;i < rfilter.Size();i++)
      ffilter[i] = rfilter[i];
    filter = ffilter;
    return true;
  }

  //: Generate a saw tooth ramp
  
  bool WindowSignalBaseC::GenerateRamp(SArray1dC<RealT> &filter) {
    filter = SArray1dC<RealT>(winSize);
    IntT halfSize = (winSize/2);
    RealT frac = ((winSize)/2);
    for(IntT i = 0;i < halfSize;i++) {
      RealT val = ((RealT)i+1) / frac;
      filter[i] =val ;
      filter[(winSize-1) -i] =val ;
    }
    return true;
  }
  
  bool WindowSignalBaseC::GenerateHanning(SArray1dC<RealT> &filter) {
    filter = SArray1dC<RealT>(winSize);    
    for(UIntT i = 0;i < winSize;i++)
      filter[i] = (1 - Cos(RavlConstN::pi * 2 * ((RealT) i / (winSize-1)))) / 2;
    return true;
  }
  
  bool WindowSignalBaseC::GenerateHamming(SArray1dC<RealT> &filter) {
    filter = SArray1dC<RealT>(winSize);    
    for(UIntT i = 0;i < winSize;i++)
      filter[i] = 0.54 - 0.46 * Cos(RavlConstN::pi * 2 * ((RealT) i / (winSize-1)));
    return true;
  }
  
  bool WindowSignalBaseC::GenerateBlackman(SArray1dC<RealT> &filter) {
    filter = SArray1dC<RealT>(winSize);    
    for(UIntT i = 0;i < winSize;i++) {
      RealT a = (RealT) i / (winSize-1);
      filter[i] = 0.42 - 0.5 * Cos(RavlConstN::pi * 2 * a) + 0.08 * Cos(RavlConstN::pi * 4 * a);
    }
    return true;
  }

  // ------------------------------------------------------------------------------

  //: Default constructor.
  WindowSignalFloatC::WindowSignalFloatC()
   : DPEntityC(true)
  {}

  //: Construct window function
  WindowSignalFloatC::WindowSignalFloatC(RAWindowSignalT sigType,UIntT size,UIntT frameSeperation)
   : DPEntityC(new WindowSignalBodyC<float,float,float>(sigType,size,frameSeperation))
  {}

  //: Construct from an xml file.
  WindowSignalFloatC::WindowSignalFloatC(const XMLFactoryContextC &factory)
   : DPEntityC(new WindowSignalBodyC<float,float,float>(factory))
  {}

  void LinkWindowSignal()
  {}

  static RavlN::XMLFactoryRegisterHandleConvertC<WindowSignalFloatC,DPIStreamOpC<float,SArray1dC<float> > > g_registerWindowSignalFloat("RavlAudioN::WindowSignalFloatC");

}
