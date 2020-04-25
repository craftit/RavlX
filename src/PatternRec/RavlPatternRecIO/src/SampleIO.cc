// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2003, OmniPerception Ltd.
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//! rcsid="$Id$"
//! lib=RavlPatternRecIO
//! file="Ravl/PatternRec/IO/SampleIO.cc"

#include "Ravl/Vector.hh"
#include "Ravl/PatternRec/Sample.hh"
#include "Ravl/DP/FileFormatStream.hh"
#include "Ravl/DP/FileFormatBinStream.hh"
#include "Ravl/DP/Converter.hh"
#include "Ravl/Types.hh"
#include "Ravl/PatternRec/SampleVector.hh"
#include "Ravl/PatternRec/SampleLabel.hh"

namespace RavlN {
  void InitRavlSampleIO()
  {}
  
  static TypeNameC type1(typeid(SampleC<UIntT>),"RavlN::SampleC<UIntT>");
  static TypeNameC type2(typeid(SampleC<VectorC>),"RavlN::SampleC<VectorC>");
  static TypeNameC type3(typeid(SampleVectorC),"RavlN::SampleVectorC");
  static TypeNameC type4(typeid(SampleLabelC),"RavlN::SampleLabelC");
  static TypeNameC type5(typeid(SArray1dC<VectorC>),"RavlN::SArray1dC<VectorC>");
  
  static FileFormatStreamC<SampleC<UIntT> > FileFormatStream_SampelC_UIntT;
  static FileFormatStreamC<SampleC<VectorC> > FileFormatStream_SampelC_VectorC;
  
  static FileFormatBinStreamC<SampleC<UIntT> > FileFormatBinStream_SampelC_UIntT;
  static FileFormatBinStreamC<SampleC<VectorC> > FileFormatBinStream_SampelC_VectorC;
  
  SampleC<VectorC> SampleVectorClassToTemplate(const SampleVectorC &sv)
  { return const_cast<SampleC<VectorC> &>((SampleC<VectorC> &)sv); }
  
  SampleC<VectorC> SArray1dC_Vector2Sample_Vector(const SArray1dC<VectorC> &sv)
  { return SampleC<VectorC>(sv); }
  
  SampleVectorC SampleVectorTemplateToClass(const SampleC<VectorC> &sv)
  { return SampleVectorC(sv); }
  
  SampleC<UIntT> SampleLabelClassToTemplate(const SampleLabelC &sv)
  { return const_cast<SampleC<UIntT> &>((SampleC<UIntT> &)sv); }

  SampleLabelC SampleLabelTemplateToClass(const SampleC<UIntT> &sv)
  { return SampleLabelC(sv); }
  
  DP_REGISTER_CONVERSION_NAMED(SampleVectorClassToTemplate,1,"RavlN::SampleC<VectorC> RavlN::Convert(const SampleVectorC &)");
  DP_REGISTER_CONVERSION_NAMED(SArray1dC_Vector2Sample_Vector,1,"RavlN::SampleC<VectorC> RavlN::Convert(const SArray1dC<VectorC> &)");
  DP_REGISTER_CONVERSION_NAMED(SampleVectorTemplateToClass,1,"RavlN::SampleVectorC RavlN::Convert(const SampleC<VectorC> &)");
  DP_REGISTER_CONVERSION_NAMED(SampleLabelClassToTemplate,1,"RavlN::SampleC<UIntT> RavlN::Convert(const SampleLabelC &)");
  DP_REGISTER_CONVERSION_NAMED(SampleLabelTemplateToClass,1,"RavlN::SampleLabelC RavlN::Convert(const SampleC<UIntT> &)");
  
}
