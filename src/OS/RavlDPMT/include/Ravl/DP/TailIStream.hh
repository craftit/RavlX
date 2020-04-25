// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2008, OmniPerception Ltd.
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVL_TAILISTREAM_HEADER
#define RAVL_TAILISTREAM_HEADER 1
/////////////////////////////////////////////////////////////////////
//! file="Ravl/OS/DataProc/TailIStream.hh"
//! lib=RavlDPMT
//! userlevel=Normal
//! author="Charles Galambos"
//! date="14/02/2008"
//! docentry="Ravl.API.Core.Data Processing" 

#include "Ravl/DP/SPortAttach.hh"
#include "Ravl/Threads/Semaphore.hh"

namespace RavlN {  
  
  //! userlevel=Develop
  //: Base class for tailing seekable stream
  
  class TailStreamBaseC 
    : virtual public DPEntityBodyC
  {
  public:
    typedef bool (*ProcessUpdateT)(TailStreamBaseC *);
    
    //: Constructor.
    TailStreamBaseC();
    
    //: Destructor.
    virtual ~TailStreamBaseC();
    
    //: Process an update.
    bool HandleUpdate();
    
    //: Stop tailing the stream.
    bool Terminate();
    
  protected:
    //: Initailse 
    void Init();
    
    Int64T m_currentSize;
    SemaphoreC m_sizeUpdate;
    volatile bool m_terminate;
    IntT m_updateSigId;
  };
  
  //! userlevel=Develop
  //: Tail a seekable stream that is occasionally being appended too.
  // This class changed the behaviour when reading entries off the end of a
  // seekable stream. Instead of reporting no entry, it will wait for an image
  // to be appended to the end of the stream.
  
  template<class DataT>
  class DPTailIStreamBodyC 
    : public DPISPortAttachBodyC<DataT>,
      public TailStreamBaseC
  {
  public:
    //: Constructor
    DPTailIStreamBodyC(const DPIPortC<DataT> &aport)
      : DPISPortAttachBodyC<DataT>(aport),
        TailStreamBaseC()
    { Init(); }
    
    //: Destructor.
    virtual ~DPTailIStreamBodyC()
    {
      if(m_updateSigId >= 0)
        this->AttributeCtrlC::RemoveChangedSignal(m_updateSigId);
    }
    
    virtual DataT Get() { 
      do {
        try {
          return this->port.Get(); 
        } catch(DataNotReadyC &) {
          // Just throw on exception ?
          if(m_terminate)
            throw ;
        }
        this->m_sizeUpdate.Wait(5.0);        
      } while(!m_terminate);
      throw DataNotReadyC();
    }
    //: Get next piece of data.
    
    virtual bool Get(DataT &buff) {  
      do {
        if(this->port.Get(buff)) 
          return true;
        // Wait for size update, and
        // check for new data or terminate flag from time to time.
        if(!m_terminate)
          this->m_sizeUpdate.Wait(5.0);
      } while(!m_terminate);
      return false;
    }
    //: Try and get next piece of data.
    
  protected:
    
  };

  //! userlevel=Normal
  //: Tail a seekable stream that is occasionally being appended too.
  // This class changed the behaviour when reading entries off the end of a
  // seekable stream. Instead of reporting no entry, it will wait for an image
  // to be appended to the end of the stream.

  template<class DataT>
  class DPTailIStreamC 
    : public DPISPortAttachBodyC<DataT>
  {
  public:
    //! Default constructor
    DPTailIStreamC()
      : DPEntityC(true)
    {}
    
    //! Constructor.
    DPTailIStreamC(const DPIPortC<DataT> &aport)
      : DPEntityC(*new DPTailIStreamBodyC<DataT>(aport))
    {}
    
  protected:
    //! Access body.
    DPTailIStreamBodyC<DataT> &Body()
    { return dynamic_cast<DPTailIStreamBodyC<DataT> &>(DPEntityC::Body()); }
    
    //! Access body.
    const DPTailIStreamBodyC<DataT> &Body() const
    { return dynamic_cast<const DPTailIStreamBodyC<DataT> &>(DPEntityC::Body()); }
    
  public:
    
    //: Stop tailing the stream.
    bool Terminate()
    { return Body().Terminate(); }
    
  };  
  
}


#endif
