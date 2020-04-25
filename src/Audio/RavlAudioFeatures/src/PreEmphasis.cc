

#include "Ravl/Audio/PreEmphasis.hh"
#include "Ravl/XMLFactoryRegister.hh"

namespace RavlAudioN {

  //! Default constructor.
  PreEmphasisFloatC::PreEmphasisFloatC()
   : DPEntityC(true)
  {}

  //! Factory constructor.
  PreEmphasisFloatC::PreEmphasisFloatC(const XMLFactoryContextC &factory)
   : DPEntityC(*new PreEmphasisBodyC<RealT,RealT>(factory.AttributeReal("alpha",0.97)))
  {}

  static RavlN::XMLFactoryRegisterHandleConvertC<PreEmphasisFloatC,DPIStreamOpC<float,float> > g_registerVectorDelta012("RavlAudioN::PreEmphasisFloatC");

  void LinkPreEmphasis()
  {}

}
