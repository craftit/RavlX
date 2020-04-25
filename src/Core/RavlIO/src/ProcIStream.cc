
#include "Ravl/DP/ProcIStream.hh"
#include "Ravl/DP/Converter.hh"
#include "Ravl/Complex.hh"

namespace RavlN {

  static DPIStreamOpC<float,float> DPProcIStreamFloat(const DPProcessC<float,float> &bod)
  { return DPProcIStreamC<float,float>(bod); }

  DP_REGISTER_CONVERSION(DPProcIStreamFloat,1);

  static DPIStreamOpC<float,SArray1dC<float> > DPProcIStreamFloat2SArrayFloat(const DPProcessC<float,SArray1dC<float> > &bod)
  { return DPProcIStreamC<float,SArray1dC<float> >(bod); }

  DP_REGISTER_CONVERSION(DPProcIStreamFloat2SArrayFloat,1);

  static DPIStreamOpC<SArray1dC<float>,SArray1dC<float> > DPProcIStreamSArrayFloat(const DPProcessC<SArray1dC<float> ,SArray1dC<float> > &bod)
  { return DPProcIStreamC<SArray1dC<float>,SArray1dC<float> >(bod); }

  DP_REGISTER_CONVERSION(DPProcIStreamSArrayFloat,1);

  static DPIStreamOpC<SArray1dC<RealT>,SArray1dC<RealT> > DPProcIStreamSArrayReal(const DPProcessC<SArray1dC<RealT> ,SArray1dC<RealT> > &bod)
  { return DPProcIStreamC<SArray1dC<RealT>,SArray1dC<RealT> >(bod); }

  DP_REGISTER_CONVERSION(DPProcIStreamSArrayReal,1);

  static DPIStreamOpC<SArray1dC<ComplexC>,SArray1dC<RealT> > DPProcIStreamSArrayComplexSArrayReal(const DPProcessC<SArray1dC<ComplexC> ,SArray1dC<RealT> > &bod)
  { return DPProcIStreamC<SArray1dC<ComplexC>,SArray1dC<RealT> >(bod); }

  DP_REGISTER_CONVERSION(DPProcIStreamSArrayComplexSArrayReal,1);

  void LinkProcIStream()
  {}
}
