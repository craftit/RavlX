// This file is used in conjunction with RAVL, Recognition And Vision Library
// Copyright (C) 2004, OmniPerception Ltd.
// This code may be redistributed under the terms of the GNU
// General Public License (GPL). See the gpl.licence file for details or
// see http://www.gnu.org/copyleft/gpl.html
// file-header-ends-here
// $Id: DesignSvm.cc 9564 2006-09-28 13:19:30Z alex $
//! lib=RavlSVM

//! author="Alexey Kostin"

#include "Ravl/PatternRec/DesignSvm.hh"
#include "Ravl/XMLFactoryRegister.hh"

namespace RavlN
{

//---------------------------------------------------------------------------
// Default constructor.
DesignSvmBodyC::DesignSvmBodyC()
               : DesignClassifierSupervisedBodyC()
{
  callbackFunc = NULL;
  callbackData = NULL;
}

  
//---------------------------------------------------------------------------
//: Load from XML factory  
DesignSvmBodyC::DesignSvmBodyC(const XMLFactoryContextC & factory)
  : DesignClassifierSupervisedBodyC(factory)
{
  callbackFunc = NULL;
  callbackData = NULL;
}
  

//---------------------------------------------------------------------------
//: Load from stream.
DesignSvmBodyC::DesignSvmBodyC(istream &strm)
               :DesignClassifierSupervisedBodyC(strm)
{
}
//---------------------------------------------------------------------------
//: Load from binary stream.
DesignSvmBodyC::DesignSvmBodyC(BinIStreamC &strm)
               :DesignClassifierSupervisedBodyC(strm)
{
}
//---------------------------------------------------------------------------
//: Writes object to stream
bool DesignSvmBodyC::Save(ostream &out) const
{
  if(!DesignClassifierSupervisedBodyC::Save(out))
    return false;
  return true;
}
//---------------------------------------------------------------------------
//: Writes object to stream
bool DesignSvmBodyC::Save(BinOStreamC &out) const
{
  if(!DesignClassifierSupervisedBodyC::Save(out))
    return false;
  return true;
}
//---------------------------------------------------------------------------
// Destructor.
DesignSvmBodyC::~DesignSvmBodyC()
{
}
//---------------------------------------------------------------------------
Classifier2C DesignSvmBodyC::Apply(const SampleC<VectorC> &TrainingSetVectors,
                                   const SampleC<SByteT> &TrainingSetLabels,
                                   const SArray1dC<IndexC> &ObjectsToUse)
{
  throw ExceptionC("DesignSvmBodyC::Apply is abstract method");
  return Classifier2C();
}
//---------------------------------------------------------------------------
ClassifierC DesignSvmBodyC::Apply(const SampleC<VectorC> &TrainingSetVectors,
                                  const SampleC<UIntT> &TrainingSetLabels,
                                  const SArray1dC<IndexC> &ObjectsToUse)
{
  throw ExceptionC("DesignSvmBodyC::Apply is abstract method");
  return ClassifierC();
}
//---------------------------------------------------------------------------
Classifier2C DesignSvmBodyC::Apply(const SampleC<VectorC> &TrainingSetVectors,
                                   const SampleC<SByteT> &TrainingSetLabels)
{
  throw ExceptionC("DesignSvmBodyC::Apply is abstract method");
  return Classifier2C();
}
//---------------------------------------------------------------------------
ClassifierC DesignSvmBodyC::Apply(const SampleC<VectorC> &TrainingSetVectors,
                                  const SampleC<UIntT> &TrainingSetLabels)
{
  throw ExceptionC("DesignSvmBodyC::Apply is abstract method");
  return ClassifierC();
}
//---------------------------------------------------------------------------

// Reset designer to intial state
bool DesignSvmBodyC::Reset() {
  RavlAssertMsg(0,"DesignSvmBodyC::Reset(), Abstract method called. \n");
  return false;
}


//---------------------------------------------------------------------------
IntT DesignSvmBodyC::NumSupportVectors() const
{
  throw ExceptionC("DesignSvmBodyC::NumSupportVectors is abstract method");
  return 0;
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------


RavlN::XMLFactoryRegisterHandleConvertC<DesignSvmC, DesignClassifierSupervisedC> g_registerXMLFactoryDesignSvm("RavlN::DesignSvmC");

void linkDesignSvm()
{}

}

