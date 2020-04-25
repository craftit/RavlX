
#include "Ravl/DP/ProcOStream.hh"
#include "Ravl/DP/Converter.hh"

namespace RavlN {

  static DPOStreamOpC<float,float> DPProcOStreamFloat(const DPProcessC<float,float> &bod)
  { return DPProcOStreamC<float,float>(bod); }

  DP_REGISTER_CONVERSION(DPProcOStreamFloat,1);

  static DPOStreamOpC<float,SArray1dC<float> > DPProcOStreamFloat2SArrayFloat(const DPProcessC<float,SArray1dC<float> > &bod)
  { return DPProcOStreamC<float,SArray1dC<float> >(bod); }

  DP_REGISTER_CONVERSION(DPProcOStreamFloat2SArrayFloat,1);

  static DPOStreamOpC<SArray1dC<float>,SArray1dC<float> > DPProcOStreamSArrayFloat(const DPProcessC<SArray1dC<float> ,SArray1dC<float> > &bod)
  { return DPProcOStreamC<SArray1dC<float>,SArray1dC<float> >(bod); }

  DP_REGISTER_CONVERSION(DPProcOStreamSArrayFloat,1);

  static DPOStreamOpC<SArray1dC<RealT>,SArray1dC<RealT> > DPProcOStreamSArrayReal(const DPProcessC<SArray1dC<RealT> ,SArray1dC<RealT> > &bod)
  { return DPProcOStreamC<SArray1dC<RealT>,SArray1dC<RealT> >(bod); }

  DP_REGISTER_CONVERSION(DPProcOStreamSArrayReal,1);

  void LinkProcOStream()
  {}
}
