// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVL_DPSPORT_HEADER
#define RAVL_DPSPORT_HEADER 1
/////////////////////////////////////////////////////
//! docentry="Ravl.API.Core.Data Processing.Seekable Ports" 
//! rcsid="$Id$"
//! file="Ravl/Core/IO/SPort.hh"
//! lib=RavlIO
//! author="Charles Galambos"
//! date="27/10/1998"
//! userlevel=Default

#include "Ravl/DP/Port.hh"

namespace RavlN {
  
  typedef Int64T StreamPosT;
  //: Stream position.
  
  const StreamPosT streamPosUnknown = 0x7fffffffffffffffll;
  
  //:-
  
  //////////////////////////////////////////////////
  //! userlevel=Develop
  //: Stream position control body class.
  // This class provides an abstract base class which allows
  // the streams position to be queried and manipulated.

  class DPSeekCtrlBodyC 
    : virtual public DPEntityBodyC 
  {
  public:
    DPSeekCtrlBodyC()
    {}
    //: Default constructor.
    
    virtual bool Seek(UIntT posn);
    //: Seek to location in stream.
    // Returns false, if seek failed. (Maybe because its
    // not implemented.)
    // if an error occurred (Seek returned False) then stream
    // position will not be changed.
    
    virtual bool DSeek(IntT off);
    //: Delta Seek, goto location relative to the current one.
    // The default behaviour of this functions is :
    // Do some error checking then:
    //   Seek((UIntT)((IntT) Tell() + off));
    // if an error occurred (DSeek returned False) then stream
    // position will not be changed.
    
    virtual UIntT Tell() const; 
    //: Find current location in stream.
    // Defined as the index of the next object to be written or read.
    // May return ((UIntT) (-1)) if not implemented.
    
    
    virtual UIntT Size() const; 
    //: Find the total size of the stream. (assuming it starts from 0)
    // May return ((UIntT) (-1)) if not implemented.
    
    virtual UIntT Start() const; 
    //: Find the position where the stream begins, normally zero.
    // Defaults to 0
    
    virtual bool Seek64(StreamPosT posn);
    //: Seek to location in stream.
    // Returns false, if seek failed. (Maybe because its
    // not implemented.)
    // if an error occurred (Seek returned False) then stream
    // position will not be changed.
    
    virtual bool DSeek64(StreamPosT off);
    //: Delta Seek, goto location relative to the current one.
    // The default behaviour of this functions is :
    // Do some error checking then:
    //   Seek((UIntT)((IntT) Tell() + off));
    // if an error occurred (DSeek returned False) then stream
    // position will not be changed.
    
    virtual StreamPosT Tell64() const; 
    //: Find current location in stream.
    // Defined as the index of the next object to be written or read.
    // May return ((UIntT) (-1)) if not implemented.
    
    virtual StreamPosT Size64() const; 
    //: Find the total size of the stream. (assuming it starts from 0)
    // May return ((UIntT) (-1)) if not implemented.
    
    virtual StreamPosT Start64() const; 
    //: Find the position where the stream begins, normally zero.
    // Defaults to 0

  protected:
    bool RegisterSPortAttributes(AttributeCtrlBodyC &attrHandle);
    //: Register sport attributes.
    // Currently size and start.
    
    bool RegisterSPortAttributes(AttributeCtrlC &attrHandle);
    //: Register sport attributes.
    // Currently size and start.
    
  };
  
  //////////////////////////////////////////////////
  //! userlevel=Develop
  //: Stub Stream position control body class.
  // This is like the main body class, but doesn't issue errors
  // when unimplemented functions are called.
  
  class DPSeekCtrlStubBodyC 
    : public DPSeekCtrlBodyC  
  {
  public:
    DPSeekCtrlStubBodyC();
    //: Default constructor.
   
    DPSeekCtrlStubBodyC(const DPPortC &pb);
    //: Constructor.
   
    virtual bool Seek(UIntT posn);
    //: Seek to location in stream.
    // Returns false, if seek failed. (Maybe because its
    // not implemented.)
    // if an error occurred (Seek returned False) then stream
    // position will not be changed.
   
    virtual bool DSeek(IntT off);
    //: Delta Seek, goto location relative to the current one.
    // The default behaviour of this functions is :
    // Do some error checking then:
    //   Seek((UIntT)((IntT) Tell() + off));
    // if an error occurred (DSeek returned False) then stream
    // position will not be changed.
   
    virtual UIntT Tell() const; 
    //: Find current location in stream.
    // May return ((UIntT) (-1)) if not implemented.
   
    virtual UIntT Size() const; 
    //: Find the total size of the stream.  (assuming it starts from 0)
    // May return ((UIntT) (-1)) if not implemented.
   
  protected:
    DPPortC pb;
  };
  
  //////////////////////////////////////////////////
  //! userlevel=Normal
  //: Stream position control handle class.
  // This class provides an abstract base class which allows
  // the streams position to be queried and manipulated.
  
  class DPSeekCtrlC 
    : virtual public DPEntityC 
  {
  public:
    DPSeekCtrlC()
      : DPEntityC(true)
    {}
    //: Default constructor.
    
    DPSeekCtrlC(bool stubIt,const DPPortC &pb = DPPortC());
    //: Constructor.
    // If stubIt is true, a body the produces no
    // error message is used, otherwise its a noisy one.
    
    DPSeekCtrlC(const DPSeekCtrlC &oth)
      : DPEntityC(oth)
    {}
    //: Copy constructor.
    
    DPSeekCtrlC(const DPEntityC &oth)
      : DPEntityC(dynamic_cast<const DPSeekCtrlBodyC *>(BodyPtr(oth)))
    {}
    //: Base Constructor.
  protected:
    DPSeekCtrlC(DPSeekCtrlBodyC &bod)
      : DPEntityC(bod)
    {}
    
    //: Body constructor.
    inline DPSeekCtrlBodyC &Body()
    { return dynamic_cast<DPSeekCtrlBodyC &>(DPEntityC::Body()); }
    //: Access body class.
    
    inline const DPSeekCtrlBodyC &Body() const
    { return dynamic_cast<const DPSeekCtrlBodyC &>(DPEntityC::Body()); }  
    //: Access body class.

  public:    
    inline bool Seek(UIntT posn)
    { return Body().Seek(posn); }
    //: Seek to location in stream.
    // Returns false, if seek failed. (Maybe because its
    // not implemented.)
    // if an error occurred (Seek returned False) then stream
    // position will not be changed.
    
    inline bool DSeek(IntT off)
    { return Body().DSeek(off); }
    //: Delta Seek, goto location relative to the current one.
    // The default behaviour of this functions is :
    // Do some error checking then:
    //   Seek((UIntT)((IntT) Tell() + off));
    // if an error occurred (DSeek returned False) then stream
    // position will not be changed.
    
    inline UIntT Tell() const
    { return Body().Tell(); }
    //: Find current location in stream.
    // Defined as the index of the next object to be written or read.
    // May return ((UIntT) (-1)) if not implemented.
    
    inline UIntT Size() const
    { return Body().Size(); }
    //: Find the total size of the stream.  (assuming it starts from 0)
    // May return ((UIntT) (-1)) if not implemented.
    
    inline UIntT Start() const
    { return Body().Start(); }
    //: Find the position where the stream begins, normally zero.
    // Defaults to 0
    
    inline bool Seek64(StreamPosT posn)
    { return Body().Seek64(posn); }
    //: Seek to location in stream.
    // Returns false, if seek failed. (Maybe because its
    // not implemented.)
    // if an error occurred (Seek returned False) then stream
    // position will not be changed.
    
    inline bool DSeek64(StreamPosT off)
    { return Body().DSeek64(off); }
    //: Delta Seek, goto location relative to the current one.
    // The default behaviour of this functions is :
    // Do some error checking then:
    //   Seek((UIntT)((IntT) Tell() + off));
    // if an error occurred (DSeek returned False) then stream
    // position will not be changed.
    
    inline StreamPosT Tell64() const
    { return Body().Tell64(); }
    //: Find current location in stream.
    // Defined as the index of the next object to be written or read.
    // May return ((UIntT) (-1)) if not implemented.
    
    inline StreamPosT Size64() const
    { return Body().Size64(); }
    //: Find the total size of the stream.  (assuming it starts from 0)
    // May return ((UIntT) (-1)) if not implemented.
    
    inline StreamPosT Start64() const
    { return Body().Start64(); }
    //: Find the position where the stream begins, normally zero.
    // Defaults to 0
    
  };
  
  //////////////////////////////////////////////////
  //! userlevel=Develop
  //: Seekable port input body.
  // Note: changes in stream position attribute are not automatically reported.
  
  template<class DataT>
  class DPISPortBodyC
    : public DPIPortBodyC<DataT>, 
      virtual public DPSeekCtrlBodyC
  {
  public:
    DPISPortBodyC()
    { DPSeekCtrlBodyC::RegisterSPortAttributes(*this); }
    //: Default constructor.
    
    virtual bool Save(std::ostream &out) const 
    { return DPIPortBodyC<DataT>::Save(out); }
    //: Save to std::ostream.
    
    virtual bool GetAt(StreamPosT posn,DataT &buffer) {
      if(!Seek64(posn)) return false;
      return this->Get(buffer);
    }
    //: Get data from the given stream position.
    //!param: posn - Absolute position to retrieve data from (not relative to start point)
    //!param: buffer - buffer to store retrieved data in.
    //!return: true if data retrieved successfully.
    // Note: The position next get in stream after this operation is not garanteed.
    
    virtual bool GetAttr(const StringC &attrName,IntT &attrValue) {
      if(attrName == "size") {
	attrValue = Size();
	return true;
      }
      if(attrName == "start") {
	attrValue = Start();
	return true;
      }
      if(attrName == "position") {
	attrValue = Tell();
	return true;
      }
      return DPPortBodyC::GetAttr(attrName,attrValue);
    }
    
    virtual bool GetAttr(const StringC &attrName,RealT &attrValue) {
      if(attrName == "size") {
	attrValue = static_cast<RealT>(Size64());
	return true;
      }
      if(attrName == "start") {
	attrValue = static_cast<RealT>(Start64());
	return true;
      }
      if(attrName == "position") {
	attrValue = static_cast<RealT>(Tell64());
	return true;
      }
      return DPPortBodyC::GetAttr(attrName,attrValue);
    }
    
    virtual bool GetAttr(const StringC &attrName,StringC &attrValue) {
      if(attrName == "size") {
	attrValue = StringC(Size64());
	return true;
      }
      if(attrName == "start") {
	attrValue = StringC(Start64());
	return true;
      }
      if(attrName == "position") {
	attrValue = StringC(Tell64());
	return true;
      }
      return DPPortBodyC::GetAttr(attrName,attrValue);
    }
    //: Get a stream attribute.
    // Returns false if the attribute name is unknown.
    
    virtual bool GetAttr(const StringC &attrName,bool &attrValue) 
    { return DPPortBodyC::GetAttr(attrName,attrValue); }
    //: Get a stream attribute.
    // Returns false if the attribute name is unknown.
    
  };
  
  //////////////////////////////////////////////////
  //! userlevel=Develop
  //: Seekable port ouput body.
  
  template<class DataT>
  class DPOSPortBodyC
    : public DPOPortBodyC<DataT>, 
      virtual public DPSeekCtrlBodyC
  {
  public:
    DPOSPortBodyC()
    { DPSeekCtrlBodyC::RegisterSPortAttributes(*this); }
    //: Default constructor.
    
    virtual bool Save(std::ostream &out) const 
    { return DPOPortBodyC<DataT>::Save(out); }
    //: Save to std::ostream.
    
    virtual bool PutAt(StreamPosT posn,const DataT &buffer) {
      if(!Seek64(posn)) return false;
      return this->Put(buffer);
    }
    //: Put data at the given stream position.
    //!param: posn - Absolute position to store data at (not relative to start point)
    //!param: buffer - buffer for stored data
    //!return: true if data stored successfully.
    // Note: The position next put in stream after this operation is not garanteed.
    
    virtual bool GetAttr(const StringC &attrName,IntT &attrValue) {
      if(attrName == "size") {
	attrValue = Size();
	return true;
      }
      if(attrName == "start") {
	attrValue = Start();
	return true;
      }
      if(attrName == "position") {
	attrValue = Tell();
	return true;
      }
      return DPPortBodyC::GetAttr(attrName,attrValue);
    }
    
    virtual bool GetAttr(const StringC &attrName,RealT &attrValue) {
      if(attrName == "size") {
	attrValue = static_cast<RealT>(Size64());
	return true;
      }
      if(attrName == "start") {
	attrValue = static_cast<RealT>(Start64());
	return true;
      }
      if(attrName == "position") {
	attrValue = static_cast<RealT>(Tell64());
	return true;
      }
      return DPPortBodyC::GetAttr(attrName,attrValue);
    }
    
    virtual bool GetAttr(const StringC &attrName,StringC &attrValue) {
      if(attrName == "size") {
	attrValue = StringC(Size64());
	return true;
      }
      if(attrName == "start") {
	attrValue = StringC(Start64());
	return true;
      }
      if(attrName == "position") {
	attrValue = StringC(Tell64());
	return true;
      }
      return DPPortBodyC::GetAttr(attrName,attrValue);
    }
    //: Get a stream attribute.
    // Returns false if the attribute name is unknown.
    
    virtual bool GetAttr(const StringC &attrName,bool &attrValue) 
    { return DPPortBodyC::GetAttr(attrName,attrValue); }
    //: Get a stream attribute.
    // Returns false if the attribute name is unknown.
    
  };

  //////////////////////////////////////////////////
  //! userlevel=Normal
  //: Seekable port input handle.
  // Note: changes in stream position attribute are not automaticly 
  // reported.
  
  template<class DataT>
  class DPISPortC 
    : public DPIPortC<DataT>, 
      public DPSeekCtrlC
  {
  public:
    DPISPortC()
      : DPEntityC(true)
    {}
    //: Default constructor.
    
    DPISPortC(const DPISPortC<DataT> &oth)
      : DPEntityC(oth)
    {}
    //: Copy constructor.
    
    DPISPortC(const DPIPortBaseC &oth)
      : DPEntityC(dynamic_cast<const DPISPortBodyC<DataT> *>(BodyPtr(oth)))
    {}
    //: Base constructor.
    
    const DPISPortC<DataT> &operator= (const DPISPortC<DataT> &obj) {
      DPEntityC::operator=(obj);
      return *this;
    }
    //: Assigment.  
    
    bool GetAt(StreamPosT posn,DataT &buffer) 
    { return Body().GetAt(posn,buffer); }
    //: Get data from the given stream position.
    //!param: posn - Absolute position to retrieve data from (not relative to start point)
    //!param: buffer - buffer to store retrieved data in.
    //!return: true if data retrieved successfully.
    // Note: The position next get in stream after this operation is not garanteed.
    
  protected:
    DPISPortBodyC<DataT> &Body()
    { return dynamic_cast<DPISPortBodyC<DataT> &>(DPEntityC::Body()); }
    //: Access body.
    
    const DPISPortBodyC<DataT> &Body() const
    { return dynamic_cast<const DPISPortBodyC<DataT> &>(DPEntityC::Body()); }
    //: Access body.
  };
  
  //////////////////////////////////////////////////
  //! userlevel=Normal
  //: Seekable port ouput handle.
  
  template<class DataT>
  class DPOSPortC 
    : public DPOPortC<DataT>, 
      public DPSeekCtrlC
  {
  public:
    DPOSPortC()
      : DPEntityC(true)
    {}
    //: Default constructor.
    
    DPOSPortC(const DPOSPortC<DataT> &oth)
      : DPEntityC(oth)
    {}
    //: Copy constructor.
    
    DPOSPortC(const DPOPortBaseC &oth)
      : DPEntityC(dynamic_cast<const DPOSPortBodyC<DataT> *>(BodyPtr(oth)))
    {}
    //: Base constructor.
    
    const DPOSPortC<DataT> &operator= (const DPOSPortC<DataT> &obj) {
      DPEntityC::operator=(obj);
      return *this;
    }
    //: Assigment.
    
    bool PutAt(StreamPosT posn,const DataT &buffer) 
    { return Body().PutAt(posn,buffer); }
    //: Put data at the given stream position.
    //!param: posn - Absolute position to store data at (not relative to start point)
    //!param: buffer - buffer for stored data
    //!return: true if data stored successfully.
    // Note: The position next put in stream after this operation is not garanteed.
    
  protected:
    DPOSPortBodyC<DataT> &Body()
    { return dynamic_cast<DPOSPortBodyC<DataT> &>(DPEntityC::Body()); }
    //: Access body.
    
    const DPOSPortBodyC<DataT> &Body() const
    { return dynamic_cast<const DPOSPortBodyC<DataT> &>(DPEntityC::Body()); }
    //: Access body.
  };
}

#endif
