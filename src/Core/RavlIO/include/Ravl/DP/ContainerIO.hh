// This file is part of RAVL, Recognition And Vision Library
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVL_CONTAINERIO_HEADER
#define RAVL_CONTAINERIO_HEADER 1
///////////////////////////////////////////////////////
//! lib=RavlIO
//! author="Charles Galambos"
//! date="07/07/1998"
//! docentry="Ravl.API.Core.Data Processing.IO"
//! file="Ravl/Core/IO/ContainerIO.hh"

#include "Ravl/DP/SPort.hh"
#include "Ravl/DList.hh"
#include "Ravl/DLIter.hh"

namespace RavlN {
  //////////////////////////////
  //! userlevel=Develop
  //: Input list body.

  template<class ContainerT>
  class DPIContainerBodyC
    : public DPISPortBodyC<typename ContainerT::ElementT>
  {
  public:
    DPIContainerBodyC(const ContainerT &dat)
      : m_container(dat),
        m_iter(const_cast<ContainerT &>(dat))
    {}
    //: Constructor.

    virtual bool Seek(UIntT offset) {
      if(0 == offset) {
        m_iter = m_container;
        return true;
      }
      return false;
    }
    //: Support at least going back to the first element.

    virtual UIntT Size() const {
      return m_container.Size();
    }
    //: How many elements are there?

    virtual bool IsGetReady() const
    { return m_iter; }
    //: Is some data ready ?

    virtual bool IsGetEOS() const
    { return m_iter; }
    //: Has the End Of Stream been reached ?
    // true = yes.

    virtual typename ContainerT::ElementT Get() {
      const typename ContainerT::ElementT &dat = m_iter.Data();
      m_iter++;
      return dat;
    }
    //: Get next piece of data.

    virtual IntT GetArray(SArray1dC<typename ContainerT::ElementT> &data) {
      for(SArray1dIterC<typename ContainerT::ElementT> it(data);it;it++) {
        if(!m_iter)
          return it.Index().V();
        *it = *m_iter;
        m_iter++;
      }
      return data.Size();
    }
    //: Get an array of data from stream.
    // returns the number of elements processed.

    virtual bool Get(typename ContainerT::ElementT &buff) {
      if(!m_iter)
        return false;
      buff = *m_iter;
      m_iter++;
      return true;
    }
    //: Try and get next piece of data.

  protected:
    ContainerT m_container;
    typename ContainerT::IteratorT m_iter;
  };

  ///////////////////////////////////////////////////////////////////////////////
  //! userlevel=Develop
  //: Output list body.
  // Append useing the '+=' operator.

  template<class ContainerT>
  class DPOContainerBodyC
    : public DPOPortBodyC<typename ContainerT::ElementT>
  {
  public:
    DPOContainerBodyC(ContainerT &dat)
      : m_container(dat)
    {}
    //: Default constructor.

    virtual bool Put(const typename ContainerT::ElementT &dat) {
      m_container += dat;
      return true;
    }
    //: Put data.

    virtual IntT PutArray(const SArray1dC<typename ContainerT::ElementT> &data) {
      for(SArray1dIterC<typename ContainerT::ElementT> it(data);it;it++)
        m_container += *it;
      return data.Size();
    }
    //: Put data into container.

    ContainerT &Container()
    { return m_container; }
    //: Access list.

  protected:
    ContainerT m_container;
  };

  ///////////////////////////////////////////////////////////////////////////////
  //! userlevel=Develop
  //: Output list body.
  // Append using the '+=' operator.

  template<class ContainerT>
  class DPOContainerOverwriteBodyC
    : public DPOSPortBodyC<typename ContainerT::ElementT>
  {
  public:
    DPOContainerOverwriteBodyC(ContainerT &dat)
     : m_container(dat),
       m_iter(dat)
    {}
    //: Default constructor.

    virtual bool Seek(UIntT offset) {
      if(0 == offset) {
        m_iter = m_container;
        return true;
      }
      return false;
    }
    //: Support going back to the first element

    virtual bool Put(const typename ContainerT::ElementT &dat) {
      if(!m_iter)
        return false;
      *m_iter = dat;
      m_iter++;
      return true;
    }
    //: Put data.

    virtual IntT PutArray(const SArray1dC<typename ContainerT::ElementT> &data) {
      for(SArray1dIterC<typename ContainerT::ElementT> it(data);it;it++) {
        if(!m_iter)
          return it.Index().V();
        *m_iter = *it;
        m_iter++;
      }
      return data.Size();
    }
    //: Put data into container.

    ContainerT &Container() { return m_container; }
    //: Access list.

    virtual bool IsPutReady() const
    { return m_iter; }
    //: Is port ready for data ?

  protected:
    ContainerT m_container;
    typename ContainerT::IteratorT m_iter;
  };

  /////////////////////////////////
  //! userlevel=Normal
  //: Input from list.

  template<class ContainerT>
  class DPIContainerC
    : public DPISPortC<typename ContainerT::ElementT>
  {
  public:
    DPIContainerC(const ContainerT &dat)
      : DPEntityC(*new DPIContainerBodyC<ContainerT>(dat))
    {}
    //: Constructor.
  };

  ////////////////////////////////////
  //! userlevel=Normal
  //: Output to a container.
  // This class with use the '+=' operator to append items to the
  // container. <p>
  // The container must define 'ElementT'

  template<class ContainerT>
  class DPOContainerC
    : public DPOPortC<typename ContainerT::ElementT>
  {
  public:
    DPOContainerC(ContainerT &dat)
      : DPEntityC(*new DPOContainerBodyC<ContainerT>(dat))
    {}
    //: Constructor.

    inline ContainerT &Container()
    { return dynamic_cast<DPOContainerBodyC<ContainerT> &>(DPEntityC::Body()).Container(); }
    //: Access list.
  };

  ////////////////////////////////////
  //! userlevel=Normal
  //: Output to a container.
  // This class with use the '+=' operator to append items to the
  // container. <p>
  // The container must define 'ElementT'

  template<class ContainerT>
  class DPOContainerOverwriteC
    : public DPOSPortC<typename ContainerT::ElementT>
  {
  public:
    DPOContainerOverwriteC(ContainerT &dat)
      : DPEntityC(*new DPOContainerOverwriteBodyC<ContainerT>(dat))
    {}
    //: Constructor.

    inline ContainerT &Container()
    { return dynamic_cast<DPOContainerBodyC<ContainerT> &>(DPEntityC::Body()).Container(); }
    //: Access list.
  };

  ////////////////////////////////////////
  //: Some helper functions.

  template<class ContainerT>
  DPOPortC<typename ContainerT::ElementT> DPOContainer(ContainerT &dat)
  { return DPOContainerC<ContainerT>(dat); }

  //: Write out stream to container, appending to contents.

  template<class ContainerT>
  DPOSPortC<typename ContainerT::ElementT> DPOContainerOverwrite(ContainerT &dat)
  { return DPOContainerOverwriteC<ContainerT>(dat); }

  //: Write out stream to container, overwriting existing contents.

  template<class ContainerT>
  DPISPortC<typename ContainerT::ElementT> DPIContainer(const ContainerT &dat)
  { return DPIContainerC<ContainerT>(dat); }

  //: Use container as source for stream.

  template<class DataT>
  DPOContainerC<DListC<DataT> > DPOList(DListC<DataT> &dat)
  { return DPOContainerC<DListC<DataT> >(dat); }

  template<class DataT>
  DPIContainerC<DListC<DataT> > DPIList(const DListC<DataT> &dat)
  { return DPIContainerC<DListC<DataT> >(dat); }

}

#endif
