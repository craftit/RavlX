// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2003, OmniPerception Ltd.
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVL_BLACKBOARD_HEADER
#define RAVL_BLACKBOARD_HEADER 1
////////////////////////////////////////////////////////////
//! author="Charles Galambos"
//! rcsid="$Id$"
//! lib=RavlDPMT
//! file="Ravl/OS/DataProc/Blackboard.hh"

#include "Ravl/RefCounter.hh"
#include "Ravl/RCHandleV.hh"
#include "Ravl/Hash.hh"
#include "Ravl/RCWrap.hh"
#include "Ravl/String.hh"
#include "Ravl/Tuple2.hh"
#include "Ravl/Threads/RWLock.hh"

namespace RavlN {
  
  //! userlevel=Develop
  //: Blackboard body.
  // This container allows arbitrary data to be associated with
  // a string key. This class supports loading and saving even where the type
  // of objects is unknown to the program by storing it as a binary stream. 
  
  class BlackboardBodyC
    : public RCBodyVC
  {
  public:
    BlackboardBodyC();
    //: Default constructor.
    
    BlackboardBodyC(istream &in);
    //: Stream constructor.
    
    BlackboardBodyC(BinIStreamC &in);
    //: Stream constructor.
    
    virtual bool Save (ostream &out) const;
    //: Writes object to stream, can be loaded using constructor
    
    virtual bool Save (BinOStreamC &out) const;
    //: Writes object to stream, can be loaded using constructor
    
    template<typename DataT>
    bool Get(const StringC &tag,DataT &value) { 
      RCWrapAbstractC handle;
      RWLockHoldC lock(rwlock,true);
      if(!entries.Lookup(tag,handle))
	return false; // Not found.
      handle = Convert(handle,typeid(value));
      if(!handle.IsValid())
	return false; // Can't convert.
      RCWrapC<DataT> rcw(handle,true);
      RavlAssert(rcw.IsValid()); // Should always work after conversion.
      value = rcw.Data();
      return true;
    }
    //: Lookup entry in blackboard
    
    template<typename DataT>
    bool Put(const StringC &tag,const DataT &value) { 
      RWLockHoldC lock(rwlock,false);
      return entries.Update(tag,RCWrapC<DataT>(value)); 
    }
    //: Add entry to table.
    
    bool Remove(const StringC &tag) {
      RWLockHoldC lock(rwlock,false);
      return entries.Del(tag);
    }
    //: Remove entry from blackboard.
    
    
  protected:
    RCWrapAbstractC Convert(RCWrapAbstractC &handle,const type_info &to);
    //: Attempt conversion to requested type.

    RWLockC rwlock;
    HashC<StringC,RCWrapAbstractC> entries;
    HashC<StringC,Tuple2C<StringC,SArray1dC<char> > > unknown;
  };
  
  //! userlevel=Develop
  //: Blackboard.
  // This container allows arbitary data to be associated with
  // a string key.  Also supports IO event where the type
  // of objects is unknown to the program. Though the object is
  // not accessable.
  
  class BlackboardC
    : public RCHandleVC<BlackboardBodyC>
  {
  public:
    BlackboardC()
    {}
    //: Default constructor.
    // Creates an invalid handle.
    
    BlackboardC(bool)
      : RCHandleVC<BlackboardBodyC>(*new BlackboardBodyC())
    {}
    //: Constructor.
    
    BlackboardC(istream &strm);
    //: Load from stream.
    
    BlackboardC(BinIStreamC &strm);
    //: Load from binary stream.
    
  protected:
    BlackboardC(BlackboardBodyC &bod)
      : RCHandleVC<BlackboardBodyC>(bod)
    {}
    //: Body constructor.
    
    BlackboardBodyC &Body()
    { return RCHandleVC<BlackboardBodyC>::Body(); }
    //: Access body.
    
    const BlackboardBodyC &Body() const
    { return RCHandleVC<BlackboardBodyC>::Body(); }
    //: Access body.
    
  public:
    template<typename DataT>
    bool Get(const StringC &tag,DataT &value) 
    { return Body().Get(tag,value); }
    //: Lookup entry in blackboard
    // Get an item from the blackboard. The type conversion
    // mechanism will be used if types don't match exactly
    
    template<typename DataT>
    bool Put(const StringC &tag,const DataT &value)
    { return Body().Put(tag,value); }
    //: Lookup entry to blackboard
    // Put a piece of data into the blackboard.
    
    bool Remove(const StringC &tag) 
    { return Body().Remove(tag); }
    //: Remove entry from blackboard.
    // Remove data with the given tag from the blackboard.
    
  };
  
  inline
  ostream &operator<<(ostream &strm,const BlackboardC &bb) {
    RavlAssert(bb.IsValid());
    bb.Save(strm);
    return strm;
  }
  //: Save a blackboard to a text stream.
  
  inline
  istream &operator>>(istream &strm,BlackboardC &bb) {
    bb = BlackboardC(strm);
    return strm;
  }
  //: Load a blackboard from a text stream.
  
  inline
  BinOStreamC &operator<<(BinOStreamC &strm,const BlackboardC &bb) {
    RavlAssert(bb.IsValid());
    bb.Save(strm);
    return strm;
  }
  //: Save a blackboard to a binary stream.
  
  inline
  BinIStreamC &operator>>(BinIStreamC &strm,BlackboardC &bb) {
    bb = BlackboardC(strm);
    return strm;
  }
  //: Load a blackboard from a binary stream.
  
}


#endif
