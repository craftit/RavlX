// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2003, OmniPerception Ltd.
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//! lib=RavlAudioIO
//! file="Ravl/Audio/IO/AudioSample.cc"

#include "Ravl/Vector.hh"
#include "Ravl/DP/FileFormatStream.hh"
#include "Ravl/DP/FileFormatBinStream.hh"
#include "Ravl/DP/Converter.hh"
#include "Ravl/Types.hh"
#include "Ravl/Audio/Types.hh"

namespace RavlAudioN {

  void InitAudioSample()
  {
  }

  //: register mono 16  bit signed type 
  static TypeNameC type1(typeid(SampleElemC < 1, Int16T>), "RavlAudioN::SampleElemC<1,Int16T>");

  //: register stereo 16 bit signed type 
  static TypeNameC type2(typeid(SampleElemC < 2, Int16T>), "RavlAudioN::SampleElemC<2,Int16T>");

  //: register mono 8 bit unsigned type 
  static TypeNameC type3(typeid(SampleElemC < 1, UByteT>), "RavlAudioN::SampleElemC<1,UByteT>");

  //: register stereo 8 bit unsigned type 
  static TypeNameC type4(typeid(SampleElemC < 2, UByteT>), "RavlAudioN::SampleElemC<2,UByteT>");

  //: register mono Real 
  static TypeNameC type5(typeid(SampleElemC < 1, RealT>), "RavlAudioN::SampleElemC<1,RealT>");

  //: register stereo Real 
  static TypeNameC type6(typeid(SampleElemC < 2, RealT>), "RavlAudioN::SampleElemC<2,RealT>");

  //: register mono Real
  static TypeNameC type7(typeid(SampleElemC < 1, float>), "RavlAudioN::SampleElemC<1,float>");

  //: register stereo Real
  static TypeNameC type8(typeid(SampleElemC < 2, float>), "RavlAudioN::SampleElemC<2,float>");

  //: some normalising values 
  const RealT normInt16 = 32678 - 1;
  const RealT normUByte = 128;

  //: convert stereo sample into mono 
  // By default this will return only the first (usually left) channel

  static SampleElemC < 1, RealT > Sample2ToSample1(const SampleElemC < 2, RealT> & sample)
  {
    SampleElemC < 1, RealT> newSamp;
    newSamp.channel[0] = sample.channel[0];
    return newSamp;
  }


  //: convert mono sample into stereo  (RealT) 
  // The mono channel will be copied to both stereo channels 

  static SampleElemC < 2, RealT> Sample1ToSample2(const SampleElemC < 1, RealT> & sample)
  {
    SampleElemC < 2, RealT> newSample;
    newSample.channel[0] = newSample.channel[1] = sample.channel[0];
    return newSample;
  }

  //: Convert Samples of Int16T to RealT

  static SampleElemC < 1, float> Sample1Int16ToFloat(const SampleElemC < 1, Int16T> & sample)
  {
    SampleElemC < 1, float> newSample;
    newSample.channel[0] = sample.channel[0] / normInt16;
    return newSample;
  }

  //: Convert Samples of Int16T to RealT

  static SampleElemC < 1, float> FloatToSampleFloat(const float & sample)
  {
    SampleElemC < 1, float> newSample;
    newSample.channel[0] = sample;
    return newSample;
  }

  //: Convert Samples of Int16T to RealT

  static SampleElemC < 2, float> Sample2Int16ToFloat(const SampleElemC < 2, Int16T> & sample)
  {
    SampleElemC < 2, float> newSample;
    newSample.channel[0] = sample.channel[0] / normInt16;
    newSample.channel[1] = sample.channel[1] / normInt16;
    return newSample;
  }

  //: Convert Samples of Int16T to RealT 

  static SampleElemC < 1, RealT> Sample1Int16ToReal(const SampleElemC < 1, Int16T> & sample)
  {
    SampleElemC < 1, RealT> newSample;
    newSample.channel[0] = sample.channel[0] / normInt16;
    return newSample;
  }

  //: Convert Samples of Int16T to RealT

  static SampleElemC < 2, RealT> Sample2Int16ToReal(const SampleElemC < 2, Int16T> & sample)
  {
    SampleElemC < 2, RealT> newSample;
    newSample.channel[0] = sample.channel[0] / normInt16;
    newSample.channel[1] = sample.channel[1] / normInt16;
    return newSample;
  }


  //: Convert Samples of RealT to Int16T

  static SampleElemC < 1, Int16T> Sample1RealToInt16(const SampleElemC < 1, RealT> & sample)
  {
    SampleElemC < 1, Int16T> newSample;
    newSample.channel[0] = (Int16T) (sample.channel[0] * normInt16);
    return newSample;
  }

  //: Convert Samples of RealT to Int16T

  static SampleElemC < 2, Int16T> Sample2RealToInt16(const SampleElemC < 2, RealT> & sample)
  {
    SampleElemC < 2, Int16T> newSample;
    newSample.channel [0] = (Int16T) (sample.channel[0] * normInt16);
    newSample.channel [1] = (Int16T) (sample.channel[1] * normInt16);
    return newSample;
  }

  //: Convert Samples of RealT to Int16T

  static SampleElemC < 1, Int16T> Sample1FloatToInt16(const SampleElemC < 1, float> & sample)
  {
    SampleElemC < 1, Int16T> newSample;
    newSample.channel[0] = (Int16T) (sample.channel[0] * normInt16);
    return newSample;
  }

  //: Convert Samples of RealT to Int16T

  static SampleElemC < 2, Int16T> Sample2FloatToInt16(const SampleElemC < 2, float> & sample)
  {
    SampleElemC < 2, Int16T> newSample;
    newSample.channel [0] = (Int16T) (sample.channel[0] * normInt16);
    newSample.channel [1] = (Int16T) (sample.channel[1] * normInt16);
    return newSample;
  }

  //: Convert Samples of UByte to Real

  static SampleElemC < 1, RealT> Sample1UByteToReal(const SampleElemC < 1, UByteT> & sample)
  {
    SampleElemC < 1, RealT> newSample;
    newSample.channel[0] = (sample.channel[0] - normUByte) / (normUByte - 1);
    return newSample;
  }

  //: Convert Samples of UByte to Real

  static SampleElemC < 2, RealT> Sample2UByteToReal(const SampleElemC < 2, UByteT> & sample)
  {
    SampleElemC < 2, RealT> newSample;
    newSample.channel[0] = (sample.channel[0] - normUByte) / (normUByte - 1);
    newSample.channel[1] = (sample.channel[1] - normUByte) / (normUByte - 1);
    return newSample;
  }

  //: Convert Samples of Real to UByte

  static SampleElemC < 1, UByteT> Sample1RealToUByte(const SampleElemC < 1, RealT> & sample)
  {
    SampleElemC < 1, UByteT> newSample;
    newSample.channel[0] = (UByteT) (sample.channel[0] * (normUByte - 1));
    return newSample;
  }

  //: Convert Samples of Real to UByte

  static SampleElemC < 2, UByteT> Sample2RealToUByte(const SampleElemC < 2, RealT> & sample)
  {
    SampleElemC < 2, UByteT> newSample;
    newSample.channel[0] = (UByteT) (sample.channel[0] * (normUByte - 1));
    newSample.channel[1] = (UByteT) (sample.channel[1] * (normUByte - 1));
    return newSample;
  }

  // This could be optimised for converting blocks of data. Later

  static Int16T Sample2Int16ToInt16(const SampleElemC < 2, Int16T> &samp)
  { return(Int16T) (((IntT) samp.channel[0] + (IntT) samp.channel[1]) / 2); }

  static Int16T Sample1Int16ToInt16(const SampleElemC < 1, Int16T> &samp)
  { return samp.channel[0]; }

  static SampleElemC < 1, Int16T> Int16ToSample1Int16(const Int16T &samp)
  {
    SampleElemC < 1, Int16T> ret;
    ret.channel[0] = samp;
    return ret;
  }

  static float SampleFloatToFloat(const SampleElemC < 1, float> &samp)
  { return samp.channel[0]; }

  static RealT SampleRealToReal(const SampleElemC < 1, RealT> &samp)
  { return samp.channel[0]; }

  DP_REGISTER_CONVERSION_NAMED(Sample2ToSample1, 2, "SampleElemC<1,RealT > Sample2ToSample1 ( const SampleElemC<2,RealT>  & sample) ");
  DP_REGISTER_CONVERSION_NAMED(Sample1ToSample2, 1, "SampleElemC<2,RealT> Sample1ToSample2 ( const SampleElemC<1,RealT> & sample ) ");
  DP_REGISTER_CONVERSION_NAMED(Sample1Int16ToFloat, 1, "SampleElemC<1,float> Sample1Int16ToFloat ( const SampleElemC<1,Int16T> & sample ) ");
  DP_REGISTER_CONVERSION_NAMED(Sample2Int16ToFloat, 1, "SampleElemC<2,float> Sample2Int16ToFloat ( const SampleElemC<2,Int16T> & sample ) ");
  DP_REGISTER_CONVERSION_NAMED(Sample1Int16ToReal, 1, "SampleElemC<1,RealT> Sample1Int16ToReal ( const SampleElemC<1,Int16T> & sample ) ");
  DP_REGISTER_CONVERSION_NAMED(Sample2Int16ToReal, 1, "SampleElemC<2,RealT> Sample2Int16ToReal ( const SampleElemC<2,Int16T> & sample ) ");
  DP_REGISTER_CONVERSION_NAMED(Sample1RealToInt16, 2, "SampleElemC<1,Int16T> Sample1RealToInt16 ( const SampleElemC<1,RealT> & sample ) ");
  DP_REGISTER_CONVERSION_NAMED(Sample2RealToInt16, 2, "SampleElemC<2,Int16T> Sample2RealToInt16 ( const SampleElemC<2,RealT> & sample ) ");
  DP_REGISTER_CONVERSION_NAMED(Sample1FloatToInt16, 2, "SampleElemC<1,Int16T> Sample1FloatToInt16 ( const SampleElemC<1,float> & sample ) ");
  DP_REGISTER_CONVERSION_NAMED(Sample2FloatToInt16, 2, "SampleElemC<2,Int16T> Sample2FloatToInt16 ( const SampleElemC<2,float> & sample ) ");
  DP_REGISTER_CONVERSION_NAMED(Sample1UByteToReal, 1, "SampleElemC<1,RealT> Sample1UByteToReal (const SampleElemC<1,UByteT> & sample ) ");
  DP_REGISTER_CONVERSION_NAMED(Sample2UByteToReal, 1, "SampleElemC<2,RealT> Sample1UByteToReal (const SampleElemC<2,UByteT> & sample ) ");
  DP_REGISTER_CONVERSION_NAMED(Sample1RealToUByte, 2, "SampleElemC<1,UByteT> Sample1RealToUByte (const SampleElemC<1,RealT> & sample ) ");
  DP_REGISTER_CONVERSION_NAMED(Sample2RealToUByte, 2, "SampleElemC<2,UByteT> Sample1RealToUByte (const SampleElemC<2,RealT> & sample ) ");

  DP_REGISTER_CONVERSION(Int16ToSample1Int16, 1);

  DP_REGISTER_CONVERSION(SampleFloatToFloat,1);
  DP_REGISTER_CONVERSION(FloatToSampleFloat,1);

  DP_REGISTER_CONVERSION(SampleRealToReal,1);

  DP_REGISTER_CONVERSION(Sample2Int16ToInt16, 2);
  DP_REGISTER_CONVERSION(Sample1Int16ToInt16, 1);

  // register ascii output
  FileFormatStreamC <SampleElemC < 2, Int16T> > FileFormatStream_SampleElem_2_Int16T;
  FileFormatStreamC <SampleElemC < 1, Int16T> > FileFormatStream_SampleElem_1_Int16T;

  // register binary output
  FileFormatBinStreamC <SampleElemC < 2, Int16T> > FileFormatBinStream_SampleElem_2_Int16T;
  FileFormatBinStreamC <SampleElemC < 1, Int16T> > FileFormatBinStream_SampleElem_1_Int16T;

}
