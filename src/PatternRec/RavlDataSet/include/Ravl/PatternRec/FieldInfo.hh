// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVL_FieldInfo_HEADER
#define RAVL_FieldInfo_HEADER 1
///////////////////////////////////////////////////////////
//! rcsid="$Id$"
//! lib=RavlPatternRec
//! docentry="Ravl.API.Pattern Recognition.Feature Selection"
//! author="Kieron Messer"
//! file="Ravl/PatternRec/DataSet/FieldInfo.hh"
#include "Ravl/RCBodyV.hh"
#include "Ravl/RCHandleV.hh"
#include "Ravl/String.hh"

namespace RavlN {

  //! userlevel=Develop
  //: Informantion about a a data field

  class FieldInfoBodyC: public RCBodyVC {
    public:
      FieldInfoBodyC() {
      }
      //: Constructor.

      FieldInfoBodyC(const StringC & name);

      FieldInfoBodyC(std::istream &strm);
      //: Load from stream.

      FieldInfoBodyC(BinIStreamC &strm);
      //: Load from binary stream.

      virtual bool Save(std::ostream &out) const;
      //: Writes object to stream, can be loaded using constructor

      virtual bool Save(BinOStreamC &out) const;
      //: Writes object to stream, can be loaded using constructor

      const StringC & Name() const {
        return m_name;
      }
      //: Access name of field

      void Name(const StringC & name) {
        m_name = name;
      }
      //: Set name of field

    protected:
      StringC m_name; //!< The name of the field

  };

  //! userlevel=Normal
  //: Generic feature selector.

  class FieldInfoC: public RCHandleVC<FieldInfoBodyC> {
    public:
      FieldInfoC() {
      }
      //: Default constructor.
      // Creates an invalid handle.

      FieldInfoC(const StringC & name)
          : RCHandleVC<FieldInfoBodyC>(*new FieldInfoBodyC(name)) {
      }
      //: Default constructor.

      FieldInfoC(std::istream &strm);
      //: Load from stream.

      FieldInfoC(BinIStreamC &strm);
      //: Load from binary stream.

    protected:
      FieldInfoC(FieldInfoBodyC &bod)
          : RCHandleVC<FieldInfoBodyC>(bod) {
      }
      //: Body constructor.

      FieldInfoC(FieldInfoBodyC *bod)
          : RCHandleVC<FieldInfoBodyC>(bod) {
      }
      //: Body ptr constructor.

      FieldInfoBodyC &Body() {
        return RCHandleC<FieldInfoBodyC>::Body();
      }
      //: Access body.

      const FieldInfoBodyC &Body() const {
        return RCHandleC<FieldInfoBodyC>::Body();
      }
      //: Access body.

    public:
      const StringC & Name() const {
        return Body().Name();
      }
      //: Access name of field

      void Name(const StringC & name) {
        Body().Name(name);
      }
      //: Set name of field

  };

  inline std::istream &operator>>(std::istream &strm, FieldInfoC &obj) {
    obj = FieldInfoC(strm);
    return strm;
  }
  //: Load from a stream.
  // Uses virtual constructor.

  inline std::ostream &operator<<(std::ostream &out, const FieldInfoC &obj) {
    obj.Save(out);
    return out;
  }
  //: Save to a stream.
  // Uses virtual constructor.

  inline BinIStreamC &operator>>(BinIStreamC &strm, FieldInfoC &obj) {
    obj = FieldInfoC(strm);
    return strm;
  }
  //: Load from a binary stream.
  // Uses virtual constructor.

  inline BinOStreamC &operator<<(BinOStreamC &out, const FieldInfoC &obj) {
    obj.Save(out);
    return out;
  }
//: Save to a stream.
// Uses virtual constructor.

}

#endif
