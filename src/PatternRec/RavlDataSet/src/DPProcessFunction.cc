
#include "Ravl/PatternRec/DPProcessFunction.hh"
#include "Ravl/DP/Converter.hh"

namespace RavlN {

  DPProcessFunctionBodyC::DPProcessFunctionBodyC()
  {}
  //: Constructor

  DPProcessFunctionBodyC::DPProcessFunctionBodyC(const FunctionC &func)
   : m_func(func)
  {}
  //: Constructor

  VectorC DPProcessFunctionBodyC::Apply(const VectorC &avec) {
    RavlAssert(m_func.IsValid());
    return m_func.Apply(avec);
  }
  //: Apply operation.

  static DPProcessFunctionC Function2Process(const FunctionC &func)
  { return DPProcessFunctionC(func); }

  DP_REGISTER_CONVERSION(Function2Process,1);


  // -------------------------------------------------------


  DPProcessFunction1BodyC::DPProcessFunction1BodyC()
  {}
  //: Constructor

  DPProcessFunction1BodyC::DPProcessFunction1BodyC(const Function1C &func)
   : m_func(func)
  {}
  //: Constructor

  RealT DPProcessFunction1BodyC::Apply(const VectorC &avec) {
    RavlAssert(m_func.IsValid());
    return m_func.Apply1(avec);
  }
  //: Apply operation.

  static DPProcessFunction1C Function12Process(const Function1C &func)
  { return DPProcessFunction1C(func); }

  DP_REGISTER_CONVERSION(Function12Process,1);

  void LinkDPProcessFunction()
  {}
}
