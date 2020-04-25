// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2004, OmniPerception Ltd.
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVL_NETREQUESTMANAGER_HEADER
#define RAVL_NETREQUESTMANAGER_HEADER 1
//! rcsid="$Id$"
//! lib=RavlNet
//! author="Charles Galambos"
//! docentry="Ravl.API.OS.Network"
//! file="Ravl/OS/Network/NetRequestManager.hh"

#include "Ravl/Threads/ThreadEvent.hh"
#include "Ravl/Threads/Mutex.hh"
#include "Ravl/RCWrap.hh"
#include "Ravl/Hash.hh"

namespace RavlN {
  
  
  //! userlevel=Develop
  //: Request handing data.
  
  class NetRequestDataBodyC 
    : public RCBodyC
  {
  public:
    NetRequestDataBodyC()
    {}
    //: Default constructor.
    
    ThreadEventC &Event()
    { return event; }
    //: Access event.
    
    RCWrapAbstractC &Data()
    { return data; }
    //: Access data.
    
  protected:
    ThreadEventC event;
    RCWrapAbstractC data;
  };
  
  //! userlevel=Advanced
  //: Request handing data.
  
  class NetRequestDataC 
    : public RCHandleC<NetRequestDataBodyC>
  {
  public:
    NetRequestDataC()
    {}
    //: Default constructor
    // Creates an invalid handle.
    
    NetRequestDataC(bool)
      : RCHandleC<NetRequestDataBodyC>(*new NetRequestDataBodyC())
    {}
    //: Constructor.
    
    ThreadEventC &Event()
    { return Body().Event(); }
    //: Access event.
    
    RCWrapAbstractC &Data()
    { return Body().Data(); }
    //: Access data.
  };
  
  //! userlevel=Advanced
  //: Helper class for managing requests for info over NetEndPoint's
  // This is designed to be used via inheritance or as a member
  // variable. <br>
  // Small object.
  
  class NetRequestManagerC 
  {
  public:
    NetRequestManagerC(bool throwExceptionOnFail);
    //: Constructor.
    //!param: throwExceptionOnFail - If false WaitForReq(..) will just return false if request fails, else it will throw an ExceptionOperationFailedC
    
    ~NetRequestManagerC();
    //: Destructor.
    
    bool CreateReq(UIntT &id);
    //: Create new request.
    
    bool DeliverReqAbstract(UIntT id,const RCWrapAbstractC &data);
    //: Deliver data to waiting thread.
    
    template<class Data1T>
    bool DeliverReq(UIntT id,const Data1T &data1) 
    { return DeliverReqAbstract(id,RCWrap(data1)); }
    //: Deliver a reply with 1 arg

    template<class Data1T,class Data2T>
    bool DeliverReq(UIntT id,const Data1T &data1,const Data2T &data2) 
    { return DeliverReqAbstract(id,RCWrap(Tuple2C<Data1T,Data2T>(data1,data2))); }
    //: Deliver a reply with 2 args
    
    bool WaitForReq(UIntT id,RCWrapAbstractC &data);
    //: Wait for reply to request to arrive.
    // Returns false on time out.
    
    template<class DataT>
    bool WaitForReq(UIntT id,DataT &data) {
      RCWrapAbstractC awrap;
      if(!WaitForReq(id,awrap))
	return false;
      RCWrapC<DataT> wrap(awrap,true);
      // The following can be used to identify problems with types.
      //if(!wrap.IsValid()) { cerr << "Type mismatch data:" << TypeName(awrap.DataType()) << " and arg:" << TypeName(typeid(DataT)) << "\n"; }
      RavlAssert(wrap.IsValid()); // Check the types are what we expect.
      data = wrap.Data();
      return true;
    }
    // Wait for a reply to a request.
    
    template<class Data1T,class Data2T>
    bool WaitForReq(UIntT id,Data1T &data1,Data2T &data2) {
      RCWrapAbstractC awrap;
      if(!WaitForReq(id,awrap))
	return false;
      RCWrapC<Tuple2C<Data1T,Data2T> > wrap(awrap,true);
      RavlAssert(wrap.IsValid());      
      data1 = wrap.Data().Data1();
      data2 = wrap.Data().Data2();
      return true;
    }
    // Wait for a reply to a request.
    
    bool ConnectionClosed();
    //: Called if the connection is closed.
    // This will wake all waiting threads an report an error.
    
    void SetConnectionOk(bool value)
    { connectionOk = value; }
    //: Set connection ok flag
    //!param: value - 
  protected:
    MutexC reqAccess;
    UIntT reqIdAlloc;
    HashC<UIntT,NetRequestDataC> id2reqResults;
    bool connectionOk;
    bool throwExceptionOnFail;
  };


}


#endif
