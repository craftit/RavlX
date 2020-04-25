// This file is used in conjunction with RAVL, Recognition And Vision Library
// Copyright (C) 2004, OmniPerception Ltd.
// This code may be redistributed under the terms of the GNU
// General Public License (GPL). See the gpl.licence file for details or
// see http://www.gnu.org/copyleft/gpl.html
// file-header-ends-here

#ifndef RAVL_ClassifierFixed_HEADER
#define RAVL_ClassifierFixed_HEADER 1

#include "Ravl/PatternRec/Classifier.hh"

namespace RavlN
{
  //! userlevel = Develop
  //: Classifier with fixed outcome.
  // Useful for testing, and a place holder in degenerate cases.

  class ClassifierFixedBodyC
    : public ClassifierBodyC
  {
  public:
    ClassifierFixedBodyC(UIntT outcome = 0,UIntT nmaxLabels = 1);
    //: Default constructor

    ClassifierFixedBodyC(std::istream &Strm);
    //: Load from stream.

    ClassifierFixedBodyC(BinIStreamC &Strm);
    //: Load from binary stream.

    virtual bool Save(std::ostream &Out) const;
    //: Writes object to stream
    // returns always true (compatibility issue), on error an exception is thrown

    virtual bool Save(BinOStreamC &Out) const;
    //: Writes object to stream
    // returns always true (compatibility issue), on error an exception is thrown

    virtual UIntT Classify(const VectorC &Data) const;
    //: Classify vector 'data' and return class ID (0 or 1)

  protected:
    UIntT m_outcome;
  };

  //---------------------------------------------------------------------------

  //: Classifier with fixed outcome.
  // Useful for testing, and a place holder in degenerate cases.

  class ClassifierFixedC
   : public ClassifierC
  {
  public:
    ClassifierFixedC()
      {}
    //: Default constructor creates wrong object

    ClassifierFixedC(std::istream &Strm);
    //: Load from stream.

    ClassifierFixedC(BinIStreamC &Strm);
    //: Load from binary stream.

    ClassifierFixedC(const ClassifierC &Cls)
      : ClassifierC(Cls)
    {}
    //: Constructor

  protected:
    ClassifierFixedC(ClassifierFixedBodyC &Bod)
      : ClassifierC(Bod)
    {}
    //: Body constructor.
  
    ClassifierFixedC(ClassifierFixedBodyC *Bod)
      : ClassifierC(Bod)
    {}
    //: Body constructor.
  
    ClassifierFixedBodyC &Body()
    { return static_cast<ClassifierFixedBodyC &>(ClassifierC::Body()); }
    //: Access body.
  
    const ClassifierFixedBodyC &Body() const
    { return static_cast<const ClassifierFixedBodyC &>(ClassifierC::Body()); }
    //: Access body.

  };
  
}
#endif
