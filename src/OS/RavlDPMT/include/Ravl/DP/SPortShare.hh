// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2003, OmniPerception Ltd.
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVL_SPORTSHARE_HEADER
#define RAVL_SPORTSHARE_HEADER 1
//! author="Charles Galambos"
//! docentry="Ravl.API.Core.Data Processing.Threads" 
//! rcsid="$Id$"
//! lib=RavlDPMT
//! date="18/9/2003"
//! file="Ravl/OS/DataProc/SPortShare.hh"

#include "Ravl/DP/SPort.hh"
#include "Ravl/DP/SPortAttach.hh"
#include "Ravl/DP/StreamOp.hh"
#include "Ravl/DP/Plug.hh"
#include "Ravl/Threads/Mutex.hh"
#include "Ravl/Trigger.hh"
#include "Ravl/DP/AttributeCtrlUpdateSignal.hh"
#include "Ravl/CallMethods.hh"

namespace RavlN {
  
  //! userlevel=Develop
  //: Share access to a port amounst several outputs.
  // This allows asynchronus/independant access to a seekable stream
  // by two or more threads.
  
  template<class DataT>
  class DPISPortShareBodyC
    : public DPIPlugBaseBodyC,
      public AttributeCtrlBodyC
  {
  public:
    explicit DPISPortShareBodyC()
      :	inputUpdateCount(1),
        clients(0)
    {
      this->MapBackChangedSignal("start");
      this->MapBackChangedSignal("size");
    }
    //: Default constructor.
    
    explicit DPISPortShareBodyC(const DPISPortC<DataT> &ip)
      : inputUpdateCount(1),
        input(ip),
        clients(0)
    {
      this->MapBackChangedSignal("start");
      this->MapBackChangedSignal("size");
    }
    //: Constructor.
    
    virtual bool ConnectPort(const DPIPortBaseC &port) {
      MutexLockC lock(access);
      inputUpdateCount++;
      input = SPort(DPIPortC<DataT>(const_cast<DPIPortBaseC &>(port)));
      lock.Unlock();
      ReparentAttributeCtrl(input); // Make sure changed signals are changed appropriately.
      return port.IsValid();
    }
    //: set port.
    
    bool ConnectPort(DPIPortC<DataT> &sp) {
      MutexLockC lock(access);
      inputUpdateCount++;
      input = sp;
      lock.Unlock();
      ReparentAttributeCtrl(sp); // Make sure changed signals are changed appropriately.
      return true;
    }
    //: Set port.
    
    virtual const type_info &InputType() const
    { return typeid(DataT); }
    //: Return type of port.
    
    MutexC &AccessLock()
    { return access; }
    //: Access lock.
    
    DPISPortC<DataT> &Input()
    { return input; }
    //: Access input.
    
    bool Get(StreamPosT frameNo,DataT &buf) {
      MutexLockC lock(access);
      return input.GetAt(frameNo,buf);
    }
    //: Get frameNo from stream.

    IntT GetArray(StreamPosT frameNo,SArray1dC<DataT> &buf) {
      MutexLockC lock(access);
      int n;
      if(!input.Seek64(frameNo)) {
//        cerr << "DPISPortShareBodyC(), Failed to seek to frame " << frameNo << ".\n";
        return 0;
      }
      n = input.GetArray(buf);
      return n;
    }
    //: Get an array of data from stream.

    bool IsGetReady(StreamPosT frameNo) const {
      MutexLockC lock(access);
      return (frameNo == input.Tell64()) && input.IsGetReady();
    }
    //: Is some data ready ?
    // true = yes.

    bool IsGetEOS(StreamPosT frameNo) const {
      MutexLockC lock(access);
      return (frameNo == input.Tell64()) && input.IsGetEOS();
    }
    //: Has the End Of Stream been reached ?
    // true = yes.

    UIntT Size() const {
      MutexLockC lock(access);
      return input.Size();
    }
    //: Find the total size of the stream. (assuming it starts from 0)
    // May return ((UIntT) (-1)) if not implemented.

    UIntT Start() const {
      MutexLockC lock(access);
      return input.Start();
    }
    //: Find the offset where the stream begins, normally zero.
    // Defaults to 0

    StreamPosT Size64() const
    {
      MutexLockC lock(access);
      return input.Size64();
    }
    //: Find the total size of the stream. (assuming it starts from 0)
    // May return ((UIntT) (-1)) if not implemented.

    StreamPosT Start64() const
    {
      MutexLockC lock(access);
      return input.Start64();
    }
    //: Find the offset where the stream begins, normally zero.
    // Defaults to 0

    //:----------------------------------------------------------
    // Attributes
    
    virtual bool GetAttr(const StringC &attrName,StringC &attrValue) {
      MutexLockC lock(access);
      return input.GetAttr(attrName,attrValue);
    }
    //: Get a attribute.
    // Returns false if the attribute name is unknown.
    // This is for handling attributes such as frame rate, and compression ratios.
    
    virtual bool SetAttr(const StringC &attrName,const StringC &attrValue) {
      MutexLockC lock(access);
      return input.SetAttr(attrName,attrValue);
    }
    //: Set a attribute.
    // Returns false if the attribute name is unknown.
    // This is for handling attributes such as frame rate, and compression ratios.
    
    virtual bool GetAttr(const StringC &attrName,IntT &attrValue) {
      MutexLockC lock(access);
      return input.GetAttr(attrName,attrValue);
    }
    //: Get a attribute.
    // Returns false if the attribute name is unknown.
    // This is for handling attributes such as frame rate, and compression ratios.
    
    virtual bool SetAttr(const StringC &attrName,const IntT &attrValue) {
      MutexLockC lock(access);
      return input.SetAttr(attrName,attrValue);
    }
    //: Set a attribute.
    // Returns false if the attribute name is unknown.
    // This is for handling attributes such as frame rate, and compression ratios.
    
    virtual bool GetAttr(const StringC &attrName,RealT &attrValue) {
      MutexLockC lock(access);
      return input.GetAttr(attrName,attrValue);
    }
    //: Get a attribute.
    // Returns false if the attribute name is unknown.
    // This is for handling attributes such as frame rate, and compression ratios.
    
    virtual bool SetAttr(const StringC &attrName,const RealT &attrValue) {
      MutexLockC lock(access);
      return input.SetAttr(attrName,attrValue);
    }
    //: Set a attribute.
    // Returns false if the attribute name is unknown.
    // This is for handling attributes such as frame rate, and compression ratios.
    
    virtual bool GetAttr(const StringC &attrName,bool &attrValue) {
      MutexLockC lock(access);
      return input.GetAttr(attrName,attrValue);
    }
    //: Get a attribute.
    // Returns false if the attribute name is unknown.
    // This is for handling attributes such as frame rate, and compression ratios.
    
    virtual bool SetAttr(const StringC &attrName,const bool &attrValue) {
      MutexLockC lock(access);
      return input.SetAttr(attrName,attrValue);
    }
    //: Set a attribute.
    // Returns false if the attribute name is unknown.
    // This is for handling attributes such as frame rate, and compression ratios.
    
    virtual bool GetAttrList(DListC<StringC> &list) const {
      MutexLockC lock(access);
      return input.GetAttrList(list);
    }
    //: Get list of attributes available.
    // This method will ADD all available attribute names to 'list'.
    
    virtual bool GetAttrTypes(DListC<AttributeTypeC> &list) const {
      MutexLockC lock(access);
      return input.GetAttrTypes(list);
    }
    //: Get a list of available attribute types.
    
#if 0
    virtual AttributeTypeC GetAttrType(const StringC &attrName) const {
      MutexLockC lock(access);
      return input.GetAttrType(attrName);
    }
    //: Get type of a particular attribute.
    // Returns an invalid handle if attribute is unknown.
#endif
    
    virtual IntT RegisterChangedSignal(const StringC &attrName,const TriggerC &trig) {
      MutexLockC lock(access);
      return AttributeCtrlBodyC::RegisterChangedSignal(attrName,trig);
    }
    
    //: Register a value changed signal.
    // Note: This method may not be implemented for all AttributeCtrl's.
    // Returns an id for the trigger, or -1 if operation fails.
    
    virtual bool RemoveChangedSignal(IntT id) {
      MutexLockC lock(access);
      return AttributeCtrlBodyC::RemoveChangedSignal(id);
    }
    //: Remove a changed signal.
    // Note: This method may not be implemented for all AttributeCtrl's.
    
    virtual bool RegisterAttribute(const AttributeTypeC &attr) {
      MutexLockC lock(access);
      return AttributeCtrlBodyC::RegisterAttribute(attr);      
    }
    //: Register a new attribute type.
    
    virtual AttributeCtrlC ParentCtrl() const
    { return AttributeCtrlC(input); }
    //: Get Parent attribute control.
    
    UIntT Clients() const
    { return clients; }
    //: Access number of clients using the SPort.
    
    void RegisterClient() {
      MutexLockC lock(access);
      clients++;
    }
    //: Regiser a client
    
    void DeregisterClient() {
      MutexLockC lock(access);
      clients--;
      if(clients == 0 && triggerCountZero.IsValid()) {
        lock.Unlock();
        triggerCountZero.Invoke();
      }
    }
    //: Unregiser a client
    
    TriggerC &TriggerCountZero()
    { return triggerCountZero; }
    //: Access trigger called when client count drops to zero.
    
    UIntT InputUpdateCount() const
    { return inputUpdateCount; }
    //: Count of the number of times the input has changed.
    // used to flag an update in stream size and start is needed.
    
  protected:
    volatile UIntT inputUpdateCount;
    DPISPortC<DataT> input;
    UIntT clients; // Number of clients currently using the port.
    MutexC access;
    TriggerC triggerCountZero; // Called when client count drops to zero.
  };
  
  //! userlevel=Develop
  //: Share access to a port amounst several outputs.
  // This allows asynchronus/independant access to a seekable stream
  // by two or more threads.
  
  template<class DataT>
  class DPISPortShareC
    : public DPIPlugBaseC,
      public AttributeCtrlC 
  {
  public:
    DPISPortShareC()
      : DPEntityC(true)
    {}
    //: Default constructor.
    // Creates an invalid handle.
    
    explicit DPISPortShareC(const DPISPortC<DataT> &ip)
      : DPEntityC(*new DPISPortShareBodyC<DataT>(ip))
    {}
    //: Constructor.
    
    explicit DPISPortShareC(bool)
      : DPEntityC(*new DPISPortShareBodyC<DataT>())
    {}
    //: Constructor.
    
    const DPISPortShareC<DataT> &operator=(const DPISPortShareC<DataT> &other) {
      DPEntityC::operator=(other);
      return *this;
    }
    //: Make sure assigment gets done right.
    
  protected:
    DPISPortShareC(DPISPortShareBodyC<DataT> &bod)
      : DPEntityC(bod)
    {}
    //: Body constructor.
    
    DPISPortShareBodyC<DataT> &Body()
    { return dynamic_cast<DPISPortShareBodyC<DataT> &>(DPEntityC::Body()); }
    //: Access body.
    
    const DPISPortShareBodyC<DataT> &Body() const
    { return dynamic_cast<const DPISPortShareBodyC<DataT> &>(DPEntityC::Body()); }
    //: Access body.
    
  public:
    MutexC &AccessLock()
    { return Body().AccessLock(); }
    //: Access lock.
    
    DPISPortC<DataT> &Input()
    { return Body().Input(); }
    //: Access input.
    
    bool Get(StreamPosT frameNo,DataT &buf)
    { return Body().Get(frameNo,buf); }
    //: Get frameNo from stream.
    
    IntT GetArray(StreamPosT frameNo,SArray1dC<DataT> &buf)
    { return Body().GetArray(frameNo,buf); }
    //: Get an array of data from stream.
    
    bool IsGetReady(StreamPosT frameNo) const
    { return Body().IsGetReady(frameNo); }
    //: Is some data ready ?
    // true = yes.
    
    bool IsGetEOS(StreamPosT frameNo) const
    { return Body().IsGetEOS(frameNo); }
    //: Has the End Of Stream been reached ?
    // true = yes.        
    
    UIntT Size() const
    { return Body().Size(); }
    //: Find the total size of the stream. (assuming it starts from 0)
    // May return ((UIntT) (-1)) if not implemented.

    UIntT Start() const
    { return Body().Start(); }
    //: Find the offset where the stream begins, normally zero.
    // Defaults to 0

    StreamPosT Size64() const
    { return Body().Size64(); }
    //: Find the total size of the stream. (assuming it starts from 0)
    // May return ((UIntT) (-1)) if not implemented.

    StreamPosT Start64() const
    { return Body().Start64(); }
    //: Find the offset where the stream begins, normally zero.
    // Defaults to 0

    operator DPIPortC<DataT> () const;
    //: Convert into a DPIPortC
    
    operator DPISPortC<DataT> () const;
    //: Convert into a DPISPortC
    
    DPIPortC<DataT> Port() const;
    //: Explicitly create a port.
    
    UIntT Clients() const
    { return Body().Clients(); }
    //: Access number of clients using the SPort.
    
    void RegisterClient()
    { Body().RegisterClient(); }
    //: Register a client using this port.
    
    void DeregisterClient()
    { Body().DeregisterClient(); }
    //: Deregister a client using this port
    
    TriggerC &TriggerCountZero()
    { return Body().TriggerCountZero(); }
    //: Access trigger called when client count drops to zero.
    
    UIntT InputUpdateCount() const
    { return Body().InputUpdateCount(); }
    //: Count of the number of times the input has changed.
    // used to flag an update in stream size and start is needed.
    
  }; 
  
  //:---------------------------------------------------------------------------------------------
  
  //! userlevel=Develop
  //: Share access to a port amoungst several outputs.
  // This allows asynchronus/independant access to a seekable stream
  // by two or more threads.
  
  template<class DataT>
  class DPISPortShareClientBodyC
    : public DPStreamOpBodyC,
      public DPISPortBodyC<DataT>
  {
  public:
    DPISPortShareClientBodyC()
      : offset(0),
        start(0),
        size(streamPosUnknown),
        lastUpdateCount(0)
    {
      this->MapBackChangedSignal("start");
      this->MapBackChangedSignal("size");
    }
    //: Default constructor.
    
    DPISPortShareClientBodyC(const DPISPortShareC<DataT> &sharedPort)
      : offset(0),
        input(sharedPort),
        lastUpdateCount(0)
    {
      input.RegisterClient();
      
      this->MapBackChangedSignal("start");
      this->MapBackChangedSignal("size");
      
      // Listen out for changes to cached stream parameters
      attrCtrlUpdateStart.Connect(sharedPort,"start",TriggerR(*this,&DPISPortShareClientBodyC<DataT>::CBStartChanged));
      attrCtrlUpdateSize.Connect(sharedPort,"size",TriggerR(*this,&DPISPortShareClientBodyC<DataT>::CBSizeChanged));
      
      // Cache stream parameters.
      offset = input.Start64();
      size = input.Size64();
      start =  input.Start64();
      lastUpdateCount = input.InputUpdateCount();
    }
    //: Constructor.
    
    ~DPISPortShareClientBodyC() {
      if(input.IsValid())
      input.DeregisterClient();
    }
    //: Destructor

    virtual bool Save(ostream &out) const 
    { return DPISPortBodyC<DataT>::Save(out); }
    //: Save to ostream.
    //:----------------------------------------------------------
    // Get controls
    
    virtual bool Seek(UIntT off) {
      return Seek64(off);
    }
    //: Seek to location in stream.
    // Returns false, if seek failed. (Maybe because its
    // not implemented.)
    // if an error occurered (Seek returned False) then stream
    // position will not be changed.
    
    virtual bool DSeek(IntT off) {
      return DSeek64(off);
    }
    //: Delta Seek, goto location relative to the current one.
    // The default behavour of this functions is :
    // Do some error checking then:
    //   Seek((UIntT)((IntT) Tell() + off));
    // if an error occurered (DSeek returned False) then stream
    // position will not be changed.
    
    virtual UIntT Tell() const 
    {
      StreamPosT value = Tell64();
      return (value == streamPosUnknown ? static_cast<UIntT>(-1) : static_cast<UIntT>(value));
    }
    //: Find current location in stream.
    // Defined as the index of the next object to be written or read.
    // May return ((UIntT) (-1)) if not implemented.
    
    virtual UIntT Size() const
    {
      StreamPosT value = Size64();
      return (value == streamPosUnknown ? static_cast<UIntT>(-1) : static_cast<UIntT>(value));
    }
    //: Find the total size of the stream. (assuming it starts from 0)
    // May return ((UIntT) (-1)) if not implemented.
    
    virtual UIntT Start() const
    { 
      StreamPosT value = Start64();
      return (value == streamPosUnknown ? static_cast<UIntT>(-1) : static_cast<UIntT>(value));
    }
    //: Find the offset where the stream begins, normally zero.
    // Defaults to 0
    
    //:----------------------------------------------------------
    // Get controls
    
    virtual bool IsGetReady() const 
    { return input.IsGetReady(offset); }
    //: Is some data ready ?
    // true = yes.
    
    virtual bool IsGetEOS() const 
    { return input.IsGetEOS(offset); }
    //: Has the End Of Stream been reached ?
    // true = yes.
    
    virtual DataT Get()  {
      DataT ret;
      //cerr << "DPISPortShareClientBodyC()::Get() Offset=" << offset << "\n";
      if(!input.Get(offset,ret))
        throw DataNotReadyC("Get failed");
      offset++;
      return ret;
    }
    //: Get next piece of data.
    
    virtual bool Get(DataT &buff) { 
      //cerr << "DPISPortShareClientBodyC()::Get(DataT&) Offset=" << offset << "\n";
      if(input.Get(offset,buff)) {
        offset++;
        return true;
      }
      return false;
    }
    //: Try and get next piece of data.
    
    virtual bool GetAt(StreamPosT off,DataT &buffer) {
      if(input.Get(off,buffer)) {
        offset = off + 1;
        return true;
      }
      return false;
    }
    //: Get data from the given offset.
    //!param: off - Offset to retrieve data from.
    //!param: buffer - buffer to store retrieved data in.
    //!return: true if data retrieved successfully.
    // Note: The position next get in stream after this operation is not garanteed.
    
    virtual IntT GetArray(SArray1dC<DataT> &data) {
      IntT n = input.GetArray(offset,data);
      offset += n;
      return n;
    }
    //: Get an array of data from stream.
    
    virtual bool Seek64(StreamPosT off)
    {
//      cerr << "DPISPortShareClientBodyC()::Seek64() Off=" << off << " Start=" << start << " Size=" << size << "\n";
      if (off < start || off >= size || off == streamPosUnknown)
        return false;
      offset = off;
      return true;
    }
    //: Seek to location in stream.
    // Returns false, if seek failed. (Maybe because its
    // not implemented.)
    // if an error occurered (Seek returned False) then stream
    // position will not be changed.

    virtual bool DSeek64(StreamPosT off)
    {
//      cerr << "DPISPortShareClientBodyC()::DSeek() At=" << offset << " Off=" << off << " Start=" << start << " Size=" << size << "\n";
      if (off == streamPosUnknown)
        return false;
      StreamPosT newOff = offset + off;
      if(off < 0) {
        if(newOff < start)
          return false;
      } else {
        if(newOff >= size)
          return false;
      }
      offset = newOff;
      return true;
    }
    //: Delta Seek, goto location relative to the current one.
    // The default behavour of this functions is :
    // Do some error checking then:
    //   Seek((UIntT)((IntT) Tell() + off));
    // if an error occurered (DSeek returned False) then stream
    // position will not be changed.

    virtual StreamPosT Tell64() const
    {
      return offset;
    }
    //: Find current location in stream.
    // Defined as the index of the next object to be written or read.
    // May return ((UIntT) (-1)) if not implemented.

    virtual StreamPosT Size64() const
    {
      // Check the cache is upto date before using it
      // InputUpdateCount() doesn't do any locking so its quick.
      if(lastUpdateCount == input.InputUpdateCount())
        return size;
      return input.Size64();
    }
    //: Find the total size of the stream.  (assuming it starts from 0)
    // May return ((UIntT) (-1)) if not implemented.

    virtual StreamPosT Start64() const
    {
      // Check the cache is upto date before using it
      // InputUpdateCount() doesn't do any locking so its quick.
      if(lastUpdateCount == input.InputUpdateCount())
        return start;
      return input.Start64();
    }
    //: Find the offset where the stream begins, normally zero.
    // Defaults to 0

    virtual bool Discard() {
      offset++;
      return true;
    }
    //: Discard the next input datum.
    
    virtual AttributeCtrlC ParentCtrl() const
    { return AttributeCtrlC(input); }
    //: Get Parent attribute control.
    
  protected:
    bool CBStartChanged() { 
      // Check cache is current
      if(lastUpdateCount != input.InputUpdateCount()) {
        lastUpdateCount = input.InputUpdateCount();
        start = input.Start64();
        size = input.Size64();
      } else
        start = input.Start64();
      //cerr << "DPISPortShareClientBodyC::CBStartChanged, Called. Start="<< start << "\n";
      return true; 
    }
    //: Callback on input start changing.
    
    bool CBSizeChanged() {
      // Check cache is current
      if(lastUpdateCount != input.InputUpdateCount()) {
        lastUpdateCount = input.InputUpdateCount();
        start = input.Start64();
        size = input.Size64();
      } else
        size = input.Size64();
      //cerr << "DPISPortShareClientBodyC::CBSizeChanged, Called. Size=" << size << "\n";
      return true;
    }
    //: Callback on input size changing.
    
    StreamPosT start;
    StreamPosT size;
    StreamPosT offset;
    DPISPortShareC<DataT> input;
    UIntT lastUpdateCount;
    
    AttributeCtrlUpdateSignalC attrCtrlUpdateStart;
    AttributeCtrlUpdateSignalC attrCtrlUpdateSize;
  };
  
  //! userlevel=Advanced
  //: Share access to a port amounst several outputs.
  // This allows asynchronus/independant access to a seekable stream
  // by two or more threads.
  
  template<class DataT>
  class DPISPortShareClientC
    : public DPStreamOpC,
      public DPISPortC<DataT>
  {
  public:
    DPISPortShareClientC()
    {}
    //: Default constructor.

    explicit DPISPortShareClientC(bool)
      : DPEntityC(*new DPISPortShareClientBodyC<DataT>())
    {}
    //: Constructor
    
    explicit DPISPortShareClientC(const DPISPortShareC<DataT> &sharedPort)
      : DPEntityC(*new DPISPortShareClientBodyC<DataT>(sharedPort))
    {}
    //: Constructor.
    
  protected:
    DPISPortShareClientC(DPISPortShareClientBodyC<DataT> &bod)
      : DPEntityC(bod)
    {}
    //: Body constructor.
    
    DPISPortShareClientBodyC<DataT> &Body()
    { return dynamic_cast<DPISPortShareClientBodyC<DataT> &>(DPEntityC::Body()); }
    //: Access body.

    const DPISPortShareClientBodyC<DataT> &Body() const
    { return dynamic_cast<const DPISPortShareClientBodyC<DataT> &>(DPEntityC::Body()); }
    //: Access body.
    
  public:
    
  };
  
  
  template<class DataT>
  DPISPortShareC<DataT>::operator DPIPortC<DataT> () const 
  { return DPISPortShareClientC<DataT>(*this); }
  
  template<class DataT>
  DPISPortShareC<DataT>::operator DPISPortC<DataT> () const
  { return DPISPortShareClientC<DataT>(*this); }  
  
  template<class DataT>
  DPIPortC<DataT> DPISPortShareC<DataT>::Port() const
  { return DPISPortShareClientC<DataT>(*this); }

}

#endif
