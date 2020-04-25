// This file is part of RAVL, Recognition And Vision Library
// Copyright (C) 2009, OmniPerception Ltd.
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVL_SAMPLESTREAMFROMSAMPLE_HEADER
#define RAVL_SAMPLESTREAMFROMSAMPLE_HEADER 1
//! author="Robert Crida"
//! docentry="Ravl.API.Pattern Recognition.Data Set"
//! rcsid="$Id: SampleStreamVector.hh 7324 2009-10-21 16:23:20Z craftit $"
//! lib=RavlPatternRec
//! file="Ravl/PatternRec/DataSet/SampleStreamFromSample.hh"

#include "Ravl/PatternRec/SampleStream.hh"
#include "Ravl/PatternRec/Sample.hh"

namespace RavlN {

  //: Sample stream implementation that reads from data stored in a SampleC<DataT>
  // Uses a SampleIterC<DataT> to iterate over the sample.
  template<typename DataT>
  class SampleStreamFromSampleBodyC
    : public DPISPortBodyC<DataT>
  {
  public:
    SampleStreamFromSampleBodyC(const SampleC<DataT> &sample) :
      m_sample(sample),
      m_sampleIter(sample)
    {}
    //: Constructor from sample of items

    virtual bool Seek(UIntT off) {
      return m_sampleIter.GotoNth(off);
    }
    //: Seek to location in stream.
    // Returns false, if seek failed. (Maybe because its
    // not implemented.)
    // if an error occurered (Seek returned False) then stream
    // position will not be changed.

    virtual UIntT Tell() const  {
      return m_sampleIter.Index().V();
    }
    //: Find current location in stream.
    // Defined as the index of the next object to be written or read.
    // May return ((UIntT) (-1)) if not implemented.


    virtual UIntT Size() const{
      return m_sample.Size();
    }
    //: Find the total size of the stream. (assuming it starts from 0)
    // May return ((UIntT) (-1)) if not implemented.

    virtual DataT Get() {
      if (!m_sampleIter.IsElm()) {
        throw DataNotReadyC();
      }
      DataT data = *m_sampleIter;
      m_sampleIter.Next();
      return data;
    }
    //: Get next piece of data.

  protected:
    SampleC<DataT> m_sample;
    SampleIterC<DataT> m_sampleIter;
  };

  //! userlevel=Normal
  //: Stream of samples backed by a SampleC
  template<typename DataT>
  class SampleStreamFromSampleC
    : public SampleStreamC<DataT>
  {
  public:
    SampleStreamFromSampleC()
      : DPEntityC(true)
    {}
    //: Default constructor.
    // Creates an invalid handle

    SampleStreamFromSampleC(const SampleC<DataT> &sample)
      : DPEntityC(new SampleStreamFromSampleBodyC<DataT>(sample))
    {}
    //: Base constructor.

  protected:
    SampleStreamFromSampleC(SampleStreamFromSampleBodyC<DataT> &bod)
      : DPEntityC(bod)
    {}
    //: Body constructor.

    inline SampleStreamFromSampleBodyC<DataT> &Body()
    { return dynamic_cast<SampleStreamFromSampleBodyC<DataT> &>(DPEntityC::Body()); }
    //: Access body class.

    inline const SampleStreamFromSampleBodyC<DataT> &Body() const
    { return dynamic_cast<const SampleStreamFromSampleBodyC<DataT> &>(DPEntityC::Body()); }
    //: Access body class.

  public:
    bool GetAt(StreamPosT off,DataT &buffer)
    { return Body().GetAt(off,buffer); }
    //: Get data from the given offset.
    //!param: off - Offset to retrieve data from.
    //!param: buffer - buffer to store retrieved data in.
    //!return: true if data retrieved successfully.
    // Note: The position next get in stream after this operation is not guaranteed.
  };
}


#endif
