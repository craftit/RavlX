// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2003, University of Surrey
// This code may be redistributed under the terms of the GNU
// General Public License (GPL). See the gpl.licence file for details or
// see http://www.gnu.org/copyleft/gpl.html
// Demultiplexer based upon parts of mpeg2dec, which can be found at
// http://libmpeg2.sourceforge.net/
// file-header-ends-here
#ifndef RAVL_MPEG2DEMUX_HEADER
#define RAVL_MPEG2DEMUX_HEADER 1
//////////////////////////////////////////////////////////////////
//! lib=RavlLibMPEG2
//! author = "Warren Moore"
//! file="Ravl/Contrib/MPEG/LibMPEG2/MPEG2Demux.hh"

#include "Ravl/DP/StreamOp.hh"
#include "Ravl/SArray1d.hh"
#include "Ravl/DList.hh"
#include "Ravl/DP/SPort.hh"

extern "C"
{
  typedef unsigned int uint32_t;
  typedef unsigned char uint8_t;
  
  #include <mpeg2dec/mpeg2.h>
}

namespace RavlImageN
{

  using namespace RavlN;

  class MPEG2DemuxBodyC :
    public DPIStreamOpBodyC< ByteT, ByteT >
  {
  public:
    MPEG2DemuxBodyC(ByteT track);
    //: Constructor.
    
    virtual ~MPEG2DemuxBodyC();
    //: Destructor.
    
    virtual ByteT Get()
    {
      ByteT data;
      if(!Get(data))
        throw DataNotReadyC("Failed to get next byte.");
      return data;
    }
    //: Get next byte.
    
    virtual bool Get(ByteT &data);
    //: Get next byte.
    
    virtual IntT GetArray(SArray1dC<ByteT> &data);
    //: Get an array of bytes from stream.
    // returns the number of elements processed

    virtual bool IsGetEOS() const;
    //: Is it the EOS

    virtual bool GetAttr(const StringC &attrName, StringC &attrValue);
    //: Get a stream attribute.
    // Returns false if the attribute name is unknown.

    virtual bool Reset();
    //: Reset the demultiplexer
    
  private:
    bool ReadInput();
    //: Read data from the input stream
  
    void AppendOutput(ByteT *start, ByteT *end);
    //: Append demultiplexed data to the output buffer
    
    bool Demultiplex();
    //: Demultiplex the input buffer
    
    void BuildAttributes();
    //: Register stream attributes

  private:
    ByteT m_track;                                                          // Selected track to extract
    IntT m_state;                                                           // Current demultiplexer state
    IntT m_stateBytes;                                                      // Current number of bytes in buffer
    ByteT m_headBuf[264];                                                   // Demultiplexer head buffer
    
    SArray1dC< ByteT > m_dataIn;                                            // Input data buffer
    DListC< SArray1dC< ByteT > > m_dataOut;                                 // Output data buffer
    ByteT *m_bufStart, *m_bufEnd;                                           // Input data buffer pointers
    
    SArray1dC< ByteT > m_dataCurrent;                                       // Current GetArray output buffer
    SizeT m_dataCount;                                                      // Output data count
  };

  class MPEG2DemuxC :
    public DPIStreamOpC< ByteT, ByteT >
  {
  public:
    MPEG2DemuxC() :
      DPEntityC(true)
    {}
    //: Default constructor.
    // Creates an invalid handle.

    MPEG2DemuxC(ByteT track) :
      DPEntityC(*new MPEG2DemuxBodyC(track))
    {}
    //: Constructor.

    bool Reset()
    { return Body().Reset(); }
    //: Reset the demultiplexer

  protected:
    MPEG2DemuxBodyC &Body()
    { return static_cast<MPEG2DemuxBodyC &>(DPIStreamOpC< ByteT, ByteT >::Body()); }
    //: Access body.

    const MPEG2DemuxBodyC &Body() const
    { return static_cast<const MPEG2DemuxBodyC &>(DPIStreamOpC< ByteT, ByteT >::Body()); }
    //: Access body.
  };

}

#endif // RAVL_MPEG2DEMUX_HEADER
