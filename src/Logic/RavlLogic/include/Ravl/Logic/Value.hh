// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2003, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVLLOGIC_VALUE_HEADER
#define RAVLLOGIC_VALUE_HEADER 1
///////////////////////////////////////////////////////////////
//! file="Ravl/Logic/Base/Value.hh"
//! lib=RavlLogic
//! userlevel=Normal
//! date="03/07/1996"
//! rcsid="$Id$"
//! author="Charles Galambos"
//! docentry="Ravl.API.Logic"

#include "Ravl/Logic/Literal.hh"
#include "Ravl/Logic/Tuple.hh"
#include "Ravl/StrStream.hh"

namespace RavlLogicN {
  
  //! userlevel=Develop
  //: Abstract value.

  class ValueBaseBodyC 
    : public LiteralBodyC 
  {
  public:
    inline ValueBaseBodyC() 
      : LiteralBodyC() 
    {}
    // Default constructor.
    
    virtual ~ValueBaseBodyC() {}
    // Destructor.
    
    ValueBaseBodyC(std::istream &strm);
    //: Construct from a binary stream.
    
    ValueBaseBodyC(BinIStreamC &strm);
    //: Construct from a binary stream.
    
    virtual bool Save(std::ostream &out) const;
    //: Save to binary stream 'out'.

    virtual bool Save(BinOStreamC &out) const;
    //: Save to binary stream 'out'.

    virtual bool Unify(const LiteralC &Other,BindSetC &SF) const;
    // Unify with other values ?
  };

  //! userlevel=Advanced
  //: Abstract value.
  
  class ValueBaseC 
    : public LiteralC 
  {
  public:
    inline ValueBaseC() 
      : LiteralC() 
    {}
    //: Default constructor.
    // Creates an invalid handle.    

    ValueBaseC(const LiteralC &base)
      : LiteralC(dynamic_cast<const ValueBaseBodyC *>(BodyPtr(base)))
    {}
    //: Constructor.
    // Will create an invalid handle if types don't match.
    
    ValueBaseC(std::istream &strm);
    //: Load from stream.
    
    ValueBaseC(BinIStreamC &strm);
    //: Load from binary stream.
    
  protected:
    inline ValueBaseC(ValueBaseBodyC &bod)
      : LiteralC(bod)
    {}
    //: Body constructor.

    inline ValueBaseC(const ValueBaseBodyC *bod)
      : LiteralC(bod)
    {}
    //: Body constructor.
    
    ValueBaseBodyC &Body()
    { return static_cast<ValueBaseBodyC &>(LiteralC::Body()); }
    //: Access body.
    
    const ValueBaseBodyC &Body() const
    { return static_cast<const ValueBaseBodyC &>(LiteralC::Body()); }
    //: Access body.
    
  public:
    
  };

  //:----------------------------------------------------------------
  //! userlevel=Develop
  //: Value template
  
  template<class DataT>
  class ValueBodyC 
    : public ValueBaseBodyC
  {
  public:
    ValueBodyC(const DataT &ndata)
      : data(ndata)
    {}
    //: Constructor.

    ValueBodyC()
    {}
    //: Default constructor.
    
    ValueBodyC(std::istream &strm)
      : ValueBaseBodyC(strm)
    { strm >> data; }
    //: Construct from a binary stream.
    
    ValueBodyC(BinIStreamC &strm)
      : ValueBaseBodyC(strm)
    { strm >> data; }
    //: Construct from a binary stream.
    
    virtual bool Save(std::ostream &out) const {
      if(!ValueBaseBodyC::Save(out)) return false;
      out << ' ' << data;
      return true;
    }
    //: Save to binary stream 'out'.

    virtual bool Save(BinOStreamC &out) const{
      if(!ValueBaseBodyC::Save(out)) return false;
      out << data;
      return true;
    }
    //: Save to binary stream 'out'.
    
    virtual bool IsEqual(const LiteralC &oth) const;
    //: Is this equal to another value.
    
    virtual UIntT Hash() const 
    { return StdHash(data); }
    //: Hash this value.
    
    virtual StringC Name() const {
      StringC nm = StringOf(data);
      nm.gsub("\n","_");
      nm.gsub(" ","_");
      return nm;
    }
    //: Name for value.
    
    virtual void Dump(std::ostream &out) 
    { out << data; }
    //: Dump in human readable format.
    
    DataT &Data()
    { return data; }
    //: Access data.
    
    const DataT &Data() const
    { return data; }
    //: Access data.
    
  protected:
    DataT data;
  };

  //:----------------------------------------------------------------
  //! userlevel=Normal
  //: Value template

  template<class DataT>
  class ValueC 
    : public ValueBaseC
  {
  public:
    ValueC()
    {}
    //: Default constructor.
    // Creates an invalid handle.
    
    ValueC(const DataT &ndata)
      : ValueBaseC(*new ValueBodyC<DataT>(ndata))
    {}
    //: Constructor.
    
    ValueC(const LiteralC &base)
      : ValueBaseC(dynamic_cast<const ValueBodyC<DataT> *>(BodyPtr(base)))
    {}
    //: Constructor.
    // Will create an invalid handle if types don't match.

    ValueC(std::istream &strm)
      : ValueBaseC(RAVL_VIRTUALCONSTRUCTOR(strm,ValueBodyC<DataT>))
    {}
    //: Construct from a binary stream.
    
    ValueC(BinIStreamC &strm)
      : ValueBaseC(RAVL_VIRTUALCONSTRUCTOR(strm,ValueBodyC<DataT>))
    {}
    //: Construct from a binary stream.
    
  protected:
    ValueC(ValueBodyC<DataT> &bod)
      : ValueBaseC(bod)
    {}
    //: Body constructor.
    
    ValueC(const ValueBodyC<DataT> *bod)
      : ValueBaseC(bod)
    {}
    //: Body constructor.
    
    ValueBodyC<DataT> &Body()
    { return static_cast<ValueBodyC<DataT> &>(LiteralC::Body()); }
    //: Access body.
    
    const ValueBodyC<DataT> &Body() const
    { return static_cast<const ValueBodyC<DataT> &>(LiteralC::Body()); }
    //: Access body.
    
  public:
    DataT &Data()
    { return Body().Data(); }
    //: Access data.
    
    const DataT &Data() const
    { return Body().Data(); }
    //: Access data.
    
  };
  
  template<class DataT>
  bool ValueBodyC<DataT>::IsEqual(const LiteralC &oth) const {
    ValueC<DataT> isval(oth);
    if(!isval.IsValid()) return false;
    return data == isval.Data();
  }
  
  template<class DataT>
  bool ValueOf(const LiteralC &oth,DataT &value) {
    if(!oth.IsValid())
      return false;
    const ValueBodyC<DataT> *bod = dynamic_cast<const ValueBodyC<DataT> *>(&oth.Body());
    if(bod == 0)
      return false;
    value = bod->Data();
    return true;
  }
  //: Extract the value from a literal. Returns true if types
  //: match and value was extracted ok, false otherwise

  template<class DataT>
  bool ValueOfArg(const LiteralC &oth,UIntT argNo,DataT &value) {
    return ValueOf(TupleArg(oth,argNo),value);
  }
  //: Extract the value from a argument of a tuple. Returns true if types
  //: match and value was extracted ok, false otherwise
  
          
}  
#endif
