// This file is part of RAVL, Recognition And Vision Library
// Copyright (C) 2010, OmniPerception Ltd.
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVL_BUFFERSTLVECTOR_HEADER
#define RAVL_BUFFERSTLVECTOR_HEADER 1
//! lib=RavlCore
//! docentry="Ravl.API.Core.Misc"
//! file="Ravl/Core/System/Base64.hh"

#include "Ravl/Buffer.hh"
#include <vector>

namespace RavlN {

  //: Buffer from an stl vector.
  template<typename DataT>
  class BufferSTLVectorBodyC
   : public BufferBodyC<DataT>
  {
  public:
    //! Constructor
    BufferSTLVectorBodyC(std::vector<DataT> &vec,bool swapContents = false)
    {
      if(swapContents)
        std::swap(vec,m_vec);
      else
        m_vec = vec;
      if(m_vec.size() > 0)
        this->buff = &(m_vec[0]);
      this->sz = vec.size();
    }

    //! Constructor
    BufferSTLVectorBodyC(const std::vector<DataT> &vec)
    {
      m_vec = vec;
      if(m_vec.size() > 0)
        this->buff = &(m_vec[0]);
      this->sz = vec.size();
    }

  protected:
    std::vector<DataT> m_vec;
  };

  //: Buffer from an stl vector.
  template<typename DataT>
  class BufferSTLVectorC
  : public BufferC<DataT>
  {
  public:
    //! Constructor
    BufferSTLVectorC(std::vector<DataT> &vec,bool swapContents = false)
     : BufferC<DataT>(new BufferSTLVectorBodyC<DataT>(vec,swapContents))
    {}

    //! Constructor
    BufferSTLVectorC(const std::vector<DataT> &vec)
     : BufferC<DataT>(new BufferSTLVectorBodyC<DataT>(vec))
    {}

  };

}

#endif
