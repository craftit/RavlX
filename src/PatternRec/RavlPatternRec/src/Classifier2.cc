// This file is used in conjunction with RAVL, Recognition And Vision Library
// Copyright (C) 2004, OmniPerception Ltd.
// This code may be redistributed under the terms of the GNU
// General Public License (GPL). See the gpl.licence file for details or
// see http://www.gnu.org/copyleft/gpl.html
// file-header-ends-here
//! lib=RavlPatternRec

#include "Ravl/PatternRec/Classifier2.hh"
#include "Ravl/DP/FileFormatStream.hh"
#include "Ravl/DP/FileFormatBinStream.hh"
#include "Ravl/DP/Converter.hh"
#include "Ravl/VirtualConstructor.hh"

namespace RavlN
{
using namespace RavlN;

//---------------------------------------------------------------------------
// Load from stream.
Classifier2BodyC::Classifier2BodyC(std::istream &Strm)
                 :ClassifierBodyC(Strm)
{
}
//---------------------------------------------------------------------------
// Load from binary stream.
Classifier2BodyC::Classifier2BodyC(BinIStreamC &Strm)
                 :ClassifierBodyC(Strm)
{
}
//---------------------------------------------------------------------------
// Destructor.
Classifier2BodyC::~Classifier2BodyC()
{
}
//---------------------------------------------------------------------------
// Writes object to stream
bool Classifier2BodyC::Save(std::ostream &Out) const
{
  if(!ClassifierBodyC::Save(Out))
    throw ExceptionOperationFailedC("Classifier2BodyC::Save:"
                                    "error in ClassifierBodyC::Save call.");
  return true;
}
//---------------------------------------------------------------------------
// Writes object to stream, can be loaded using constructor
bool Classifier2BodyC::Save(BinOStreamC &Out) const
{
  if(!ClassifierBodyC::Save(Out))
    throw ExceptionOperationFailedC("Classifier2BodyC::Save:"
                                    "error in ClassifierBodyC::Save call.");
  return true;
}
//---------------------------------------------------------------------------
// Classifier vector 'data' return the most likely label.
UIntT Classifier2BodyC::Classify(const VectorC &Data) const
{
  return Classify2(Data) > 0;
}
//---------------------------------------------------------------------------
// Classifier vector 'data' return the most likely label.
/*UIntT Classifier2BodyC::Classify(const RealT* Data) const
{
  return Classify2(Data) > 0;
}*/
//---------------------------------------------------------------------------
// Classifier vector 'Data' return value of descriminant function
RealT Classifier2BodyC::Classify2(const VectorC &Data) const
{
  throw ExceptionOperationFailedC("Classifier2BodyC::Classify2:"
                                  "This is an abstract method");
}
//---------------------------------------------------------------------------
// Classifier vector 'Data' return value of descriminant function
/*RealT Classifier2BodyC::Classify2(const RealT* Data) const
{
  throw ExceptionOperationFailedC("Classifier2BodyC::Classify2:"
                                  "This is an abstract method");
}*/
//---------------------------------------------------------------------------
// Get vector length of classifier
IntT Classifier2BodyC::GetDataSize() const
{
  throw ExceptionOperationFailedC("Classifier2BodyC::GetDataSize:"
                                  "This is an abstract method");
}

//- Virtual construction and data conversion --------------------------------
// --------------------------------------------------------------------------
ClassifierC Classifier2ToClassifier(const Classifier2C &func)
{ return func; }

DP_REGISTER_CONVERSION_NAMED(Classifier2ToClassifier ,1,
                             "RavlN::ClassifierC RavlN::Convert(const "
                             "RavlN::Classifier2ToClassifier &)");

static TypeNameC TypeClassifier2(typeid(Classifier2C), "RavlN::Classifier2C");

FileFormatStreamC<Classifier2C> FileFormatStream_Classifier2;
FileFormatBinStreamC<Classifier2C> FileFormatBinStream_Classifier2;

RAVL_INITVIRTUALCONSTRUCTOR_FULL(Classifier2BodyC, Classifier2C, ClassifierC);

//---------------------------------------------------------------------------

}
