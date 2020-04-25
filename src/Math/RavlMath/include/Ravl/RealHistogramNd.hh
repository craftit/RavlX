// This file is part of RAVL, Recognition And Vision Library
// Copyright (C) 2001-12, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVL_HISTOGRAMND_HH
#define RAVL_HISTOGRAMND_HH 1
//! lib=RavlMath

#include "Ravl/RealRange1d.hh"
#include "Ravl/SArray1d.hh"
#include "Ravl/IndexNd.hh"
#include "Ravl/Vector.hh"

namespace RavlN {

  //! Base class for nd histograms.

  class RealHistogramBaseNdC {
  public:
    RealHistogramBaseNdC();
    //: Default constructor

    RealHistogramBaseNdC(const SArray1dC<RealRangeC> &ranges,const SArray1dC<size_t> &binSizes);
    //: Constructor

    RealHistogramBaseNdC(BinIStreamC &strm);
    //: Binary stream constructor

    bool Save(BinOStreamC &strm) const;
    //: Write histogram to binary stream

    const SArray1dC<RealRangeC> &InputRange() const
    { return m_inputLimits; }
    //: Input range.

    const SArray1dC<size_t> &GridSize() const
    { return m_gridSize; }
    //: Access the grid size.

    VectorC Scale() const;
    //: Scaling.

    VectorC Offset() const;
    //: Offset used in hash table.

    IndexNdC Bin(const VectorC &v) const;
    //: Access bin vector falls in.

    VectorC MidBin(const IndexNdC &bin) const;
    //: Get the middle of given bin.

  protected:
    void Init();

    //! Compute index of bin
    //! Returns false if vote has been clipped to bounds
    bool ComputeIndex(const VectorC &v,size_t &bin) const;

    //! Compute index of bin
    //! Returns false if vote has been clipped to bounds
    bool ComputeIndexAndRemainders(const VectorC &v,size_t &bin,SArray1dC<RealT> &offsets) const;

    SArray1dC<RealRangeC> m_inputLimits;
    SArray1dC<size_t> m_gridSize;
    SArray1dC<size_t> m_gridScale;
    size_t m_dataSize;
  };



  template<typename AccumT>
  class RealHistogramNdC
   : public RealHistogramBaseNdC
  {
  public:
    RealHistogramNdC()
    {}
    //: Default constructor

    RealHistogramNdC(const SArray1dC<RealRangeC> &ranges,const SArray1dC<size_t> &binSizes)
     : RealHistogramBaseNdC(ranges,binSizes),
       m_data(m_dataSize)
    { Reset(); }
    //: Constructor

    RealHistogramNdC(BinIStreamC &strm)
     : RealHistogramBaseNdC(strm)
    {
      ByteT version = 0;
      strm >> version;
      if(version != 1)
        throw ExceptionUnexpectedVersionInStreamC("RealHistogramNdC<>");
      strm >> m_data;
    }
    //: Binary stream constructor

    bool Save(BinOStreamC &strm) const {
      if(!RealHistogramBaseNdC::Save(strm))
        return false;
      ByteT version = 1;
      strm << version << m_data;
      return true;
    }
    //: Write histogram to binary stream

    void Reset()
    {
      AccumT zero;
      SetZero(zero);
      m_data.Fill(zero);
    }
    //: Reset counters in histogram to zero.

    void Vote(const VectorC &v)
    {
      size_t index;
      ComputeIndex(v,index);
      m_data[index]++;
    }
    //: Vote for value.
    // Note, this will not check that the value is within the histogram.
    // In check mode this will cause an error, in optimised it will corrupt
    // memory.

    template<typename WeighT>
    void Vote(const VectorC &v,const WeighT &votes)
    {
      size_t index;
      ComputeIndex(v,index);
      m_data[index] += votes;
    }
    //: Vote "votes" times for value.
    // Note, this will not check that the value is within the histogram.
    // In check mode this will cause an error, in optimised it will corrupt
    // memory.

    template<typename WeighT>
    bool CheckVote(const VectorC &v, WeighT &votes)
    {
      size_t index;
      if(!ComputeIndex(v,index))
        return false;
      m_data[index] += votes;
      return true;
    }
    //: Vote "votes" times for value.
    // Returns false if value is out of range.

    AccumT &AccessBin(const VectorC &v)
    {
      size_t ind;
      ComputeIndex(v,ind);
      return m_data[ind];
    }
    //: Access a bin containing the specified value.

    const AccumT &AccessBin(const VectorC &v) const
    {
      size_t ind;
      ComputeIndex(v,ind);
      return m_data[ind];
    }
    //: Access a bin containing the specified value.

    bool Interpolate(const VectorC &v,AccumT &value) const
    {
      SArray1dC<RealT> offsets;
      size_t indexAt;
      bool ret = ComputeIndexAndRemainders(v,indexAt,offsets);
      InternalInterpolate(static_cast<int>(m_gridSize.Size()-1),&m_data[indexAt],offsets,value);
      return ret;
    }
    //: Interpolate a value.
    //: Returns false if is outside the input bounds.

    SArray1dC<AccumT> &RawBins()
    { return m_data; }
    //: Access raw bin data

    const SArray1dC<AccumT> &RawBins() const
    { return m_data; }
    //: Access raw bin data

  protected:
    //! Internal accumulation routine
    void InternalInterpolate(int index,const AccumT *at,const SArray1dC<RealT> &position,AccumT &result) const {
      AccumT v1,v2;
      if(index == 0) {
        RavlAssert(m_gridScale[0] == 1);
        v1 = at[0];
        v2 = at[1];
      } else {
        InternalInterpolate(index-1,at,position,v1);
        InternalInterpolate(index-1,at + m_gridScale[index],position,v2);
      }
      result = (v1 * (1.0 - position[index])) + (v2 * position[index]);
    }
    //: Interpolation routine.

    SArray1dC<AccumT> m_data;
  };

  template<typename AccumT>
  BinOStreamC &operator<<(BinOStreamC &strm,const RealHistogramNdC<AccumT> &hist)
  {

    return strm;
  }

  template<typename AccumT>
  BinIStreamC &operator>>(BinIStreamC &strm,RealHistogramNdC<AccumT> &hist)
  {

    return strm;
  }
}


#endif
