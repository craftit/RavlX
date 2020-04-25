// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVLLOGIC_BINDSET_HEADER
#define RAVLLOGIC_BINDSET_HEADER 1
//////////////////////////////////////////////////////////
//! rcsid="$Id$"
//! docentry="Ravl.API.Logic"
//! author="Charles Galambos"
//! lib=RavlLogic
//! file="Ravl/Logic/Base/BindSet.hh"

#include "Ravl/Logic/Literal.hh"
#include "Ravl/Hash.hh"
#include "Ravl/HSet.hh"

namespace RavlLogicN {
  using namespace RavlN;
#if RAVL_VISUALCPP_NAMESPACE_BUG
  using RavlN::HashElemC;
  using RavlN::RCBodyC;
  using RavlN::HashC;
  using RavlN::UIntT;
  using RavlN::RCHandleC;
  using RavlN::HashIterC;
  using RavlN::StringC;
#endif

  class BindC;
  class BindSetC;
  
  typedef HashElemC<LiteralC,BindC>* BindMarkT;
  
  //! userlevel=Advanced
  //: Information about a single binding.
  
  class BindC {
  public:
    BindC()
    {}

    BindC(const LiteralC &val,BindMarkT nnext)
      : value(val),
	next(nnext)
    {}
    
    LiteralC &Value()
    { return value; }
    //: Access value.

    const LiteralC &Value() const
    { return value; }
    //: Access value.
    
    BindMarkT &Next()
    { return next; }
    //: access next ptr.

    const BindMarkT &Next() const
    { return next; }
    //: access next ptr.
    
  protected:
    LiteralC value;
    BindMarkT next;
  };

  inline std::ostream &operator<<(std::ostream &s,const BindC &bind) { 
    s << bind.Value();
    return s;
  }
  //! userlevel=Advanced
  //: output stream 
  
  inline std::istream &operator>>(std::istream &s,BindC &bind) {
    RavlAssertMsg(0,"operator<<(std::istream &s,BindC &binds), Not implemented. ");
    return s;
  }
  //! userlevel=Advanced
  //: input stream 

  inline BinOStreamC &operator<<(BinOStreamC &s,const BindC &bind);
  //! userlevel=Advanced
  //: output stream 
  
  BinIStreamC &operator>>(BinIStreamC &s,BindC &bind);
  //! userlevel=Advanced
  //: input stream 
  
  BinOStreamC &operator<<(BinOStreamC &out,const HashElemC<LiteralC,BindC> &obj);
  //! userlevel=Develop
  //: Write hash element out to stream.
  
  BinIStreamC &operator>>(BinIStreamC &in,HashElemC<LiteralC,BindC> &obj);
  //! userlevel=Develop
  //: Read hash element from stream.
  
  typedef HashElemC<LiteralC,BindC>* BindMarkT;
  
  //! userlevel=Normal
  //: Set of bindings.
  // Note: This is a small object.
  
  class BindSetBodyC 
    : public RCBodyC,
      protected HashC<LiteralC,BindC> 
  {
  public:
    BindSetBodyC()
      : top(0)
    {}
    //: Default constructor.
    
    BindSetBodyC(std::istream &strm);
    //: Construct from a binary stream.
    
    BindSetBodyC(BinIStreamC &strm);
    //: Construct from a binary stream.
    
    bool Save(std::ostream &out) const;
    //: Save to stream 'out'.
    
    bool Save(BinOStreamC &out) const;
    //: Save to binary stream 'out'.
    
    BindSetBodyC(const BindSetC &oth);
    //: Copy constructor.
    // Any bind marks in oth will NOT be valid for
    // the new BindSetC.
    
    BindSetBodyC(const HashC<LiteralC,LiteralC> &tab);
    //: Construct from a table of mappings.
    
    BindSetBodyC &Copy() const
    { return *new BindSetBodyC(*this); }
    //: Make a copy of this bind set.
    
    bool IsBound(const LiteralC &var) const 
    { return Lookup(var) != 0; }
    //: Is variable bound ?
    
    bool Resolve(const LiteralC &var,LiteralC &x) const {
      const BindC *lu = Lookup(var);
      if(lu == 0)
	return false;
      x = lu->Value();
      return true;
    }
    //: Resolve value 

    LiteralC Resolve(const LiteralC &var) const {
      const BindC *lu = Lookup(var);
      if(lu != 0)
	return lu->Value();
      return var;
    }
    //: Resolve a binding.
    // return the value 'var' is bound to, or
    // if its a free variable 'var' itself.
    
    LiteralC operator[](const LiteralC &var) const
    { return Resolve(var); }
    //: Resolve a binding.
    
    bool Bind(const LiteralC &var,const LiteralC &val);
    //: Attempty to bind a value to var.
    // Will fail if var is already bound.
    
    BindMarkT Mark() const
    { return top; }
    //: Mark the current set of bindings.
    
    void Undo(BindMarkT bm);
    //: Undo bindings to marked place.
    
    void Undo(LiteralC var);
    //: Undo bindings done after and including var.
    
    UIntT Size() const
    { return HashC<LiteralC,BindC>::Size(); }
    //: Get the number of binds in set.
    
    void Empty();
    //: Remove all bindings from set.
    
    StringC Name() const;
    //: Bind set as string.

  protected:
    BindMarkT top;
    
    friend class BindSetC;
  };

  //! userlevel=Normal
  //: Set of bindings.
  
  class BindSetC 
    : public RCHandleC<BindSetBodyC>
  {
  public:
    BindSetC()
    {}
    //: Default constructor.
    // Creates an invalid handle.
    
    explicit BindSetC(bool)
      : RCHandleC<BindSetBodyC>(*new BindSetBodyC())
    {}
    //: Constructor.
    // Creates an empty bind set.
    
    BindSetC(const HashC<LiteralC,LiteralC> &tab)
      : RCHandleC<BindSetBodyC>(*new BindSetBodyC(tab))
    {}
    //: Construct from a table of mappings.
    
    BindSetC(std::istream &strm)
      : RCHandleC<BindSetBodyC>(*new BindSetBodyC(strm))
    {}
    //: Construct from a stream.
    
    BindSetC(BinIStreamC &strm)
      : RCHandleC<BindSetBodyC>(*new BindSetBodyC(strm))
    {}
    //: Construct from a binary stream.
    
  protected:
    BindSetC(BindSetBodyC &body)
      : RCHandleC<BindSetBodyC>(body)
    {}
    //: Body constructor.
    
    BindSetBodyC &Body()
    { return RCHandleC<BindSetBodyC>::Body(); }
    //: Access body.
    
    const BindSetBodyC &Body() const
    { return RCHandleC<BindSetBodyC>::Body(); }
    //: Access body.
    
  public:
    bool Save(std::ostream &out) const
    { return Body().Save(out); }
    //: Save to stream 'out'.
    
    bool Save(BinOStreamC &out) const
    { return Body().Save(out); }
    //: Save to binary stream 'out'.
    
    BindSetC Copy() const 
    { return BindSetC(Body().Copy()); }
    //: Make a copy of some bindings.
    
    bool IsBound(const LiteralC &var) const 
    { return Body().IsBound(var); }
    //: Is variable bound ?
    
    bool Resolve(const LiteralC &var,LiteralC &x) const
    { return Body().Resolve(var,x); }
    //: Resolve value 
    // Lookup 'var' in set, if found assign its value to 'x' and
    // return true, otherwise return false.
    
    LiteralC Resolve(const LiteralC &var) const
    { return Body().Resolve(var); }
    //: Resolve a binding.
    // return the value 'var' is bound to, or
    // if its a free variable 'var' itself.
    
    LiteralC operator[](const LiteralC &var) const
    { return Resolve(var); }
    //: Resolve a binding.
    
    bool Bind(const LiteralC &var,const LiteralC &val)
    { return Body().Bind(var,val); }
    //: Attempty to bind a value to var.
    // Will fail if var is already bound.

    BindMarkT Mark() const
    { return Body().Mark(); }
    //: Mark the current set of bindings.
    
    void Undo(BindMarkT bm)
    { Body().Undo(bm); }
    //: Undo bindings to marked place.
    
    void Undo(LiteralC var)
    { Body().Undo(var); }
    //: Undo bindings done after and including var.
    
    UIntT Size() const
    { return Body().Size(); }
    //: Get the number of binds in set.
    
    void Empty()
    { Body().Empty(); }
    //: Remove all bindings from set.
    
    HashIterC<LiteralC,BindC> Iter()
    { return HashC<LiteralC,BindC>(Body()); }
    //: Iterate through binds.

    StringC Name() const
    { return Body().Name(); }
    //: Bind set as string.
  };
  
  std::ostream &operator<<(std::ostream &s,const BindSetC &binds);
  //: output stream 
  
  std::istream &operator<<(std::istream &s,BindSetC &binds);
  //: input stream 
  
  inline BinIStreamC &operator>>(BinIStreamC &strm,BindSetC &obj) {
    obj = BindSetC(strm);
    return strm;
  }
  //: Load from a binary stream.
  // Uses virtual constructor.
  
  inline BinOStreamC &operator<<(BinOStreamC &out,const BindSetC &obj) {
    RavlAssert(obj.IsValid());
    obj.Save(out);
    return out;
  }
  //: Save to a stream.
  // Uses virtual constructor.
  
}

#endif
