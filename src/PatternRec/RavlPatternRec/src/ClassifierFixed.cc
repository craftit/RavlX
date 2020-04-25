// This file is used in conjunction with RAVL, Recognition And Vision Library
// Copyright (C) 2004, OmniPerception Ltd.
// This code may be redistributed under the terms of the GNU
// General Public License (GPL). See the gpl.licence file for details or
// see http://www.gnu.org/copyleft/gpl.html
// file-header-ends-here
//! lib=RavlPatternRec

#include "Ravl/PatternRec/ClassifierFixed.hh"
#include "Ravl/DP/FileFormatStream.hh"
#include "Ravl/DP/FileFormatBinStream.hh"
#include "Ravl/DP/Converter.hh"
#include "Ravl/VirtualConstructor.hh"

namespace RavlN
{
  ClassifierFixedBodyC::ClassifierFixedBodyC(UIntT outcome,UIntT nmaxLabels)
   : ClassifierBodyC(nmaxLabels),
     m_outcome(outcome)
  {}


  // Load from stream.
  ClassifierFixedBodyC::ClassifierFixedBodyC(std::istream &strm)
    : ClassifierBodyC(strm),
      m_outcome(0)
  {
    RavlIssueError("not implemented");
  }

  // Load from binary stream.
  ClassifierFixedBodyC::ClassifierFixedBodyC(BinIStreamC &strm)
    : ClassifierBodyC(strm),
      m_outcome(0)
  {
    ByteT version = 0;
    strm >> version;
    if(version != 1)
      throw RavlN::ExceptionUnexpectedVersionInStreamC("ClassifierFixedC");
    strm >> m_outcome;
  }

  // Writes object to stream
  bool ClassifierFixedBodyC::Save(std::ostream &out) const
  {
    ClassifierBodyC::Save(out);
    RavlIssueError("not implemented");
    return true;
  }

  // Writes object to stream, can be loaded using constructor
  bool ClassifierFixedBodyC::Save(BinOStreamC &out) const
  {
    ClassifierBodyC::Save(out);
    ByteT version = 1;
    out << version << m_outcome;
    return true;
  }

  // Classifier vector 'data' return the most likely label.
  UIntT ClassifierFixedBodyC::Classify(const VectorC &Data) const
  {
    return m_outcome;
  }

  void LinkClassifierFixed()
  {}

  ClassifierC ClassifierFixedToClassifier(const ClassifierFixedC &func)
  { return func; }

  DP_REGISTER_CONVERSION_NAMED(ClassifierFixedToClassifier ,1,
                               "RavlN::ClassifierC RavlN::Convert(const "
                               "RavlN::ClassifierFixedToClassifier &)");

  static TypeNameC TypeClassifierFixed(typeid(ClassifierFixedC), "RavlN::ClassifierFixedC");

  FileFormatStreamC<ClassifierFixedC> FileFormatStream_ClassifierFixed;
  FileFormatBinStreamC<ClassifierFixedC> FileFormatBinStream_ClassifierFixed;

  RAVL_INITVIRTUALCONSTRUCTOR_FULL(ClassifierFixedBodyC, ClassifierFixedC, ClassifierC);


}
