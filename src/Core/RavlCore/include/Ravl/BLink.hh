// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVL_BLINK_HEADER 
#define RAVL_BLINK_HEADER 1
///////////////////////////////////////////////////////////////////////////
//! rcsid="$Id$"
//! lib=RavlCore
//! author="Charles Galambos"
//! docentry="Ravl.API.Core.Branch"
//! author="Charles Galambos"
//! file="Ravl/Core/Container/Branch/BLink.hh"

#include "Ravl/Types.hh"
#include "Ravl/RefCounter.hh"
#include "Ravl/SmartPtr.hh"

namespace RavlN {
   
  //! userlevel=Develop
  //: Link and data.
  
  template<class DataT>
  class BLinkBodyC 
    : public RCBodyC
  {
  public:
    BLinkBodyC()
    {}
    //: Default constructor.
    
    BLinkBodyC(const DataT &dat)
      : data(dat)
    {}
    //: Constructor.
    
    BLinkBodyC(const DataT &dat,const SmartPtrC<BLinkBodyC<DataT> > &nnext)
      : data(dat),
	next(nnext)
    {}
    //: Constructor.
    
    DataT &Data()
    { return data; }
    //: Access data.
    
    const DataT &Data() const
    { return data; }
    //: Access data.
    
    SmartPtrC<BLinkBodyC<DataT> > &Next()
    { return next; }
    //: Access to next element in list.
    
    const SmartPtrC<BLinkBodyC<DataT> > &Next() const
    { return next; }
    //: Access to next element in list.
    
  protected:
    DataT data;
    SmartPtrC<BLinkBodyC<DataT> > next;
  };
  
  //! userlevel=Advanced
  //: Link in list.
  
  template<class DataT>
  class BLinkC
    : public SmartPtrC<BLinkBodyC<DataT> >
  {
  public:
    BLinkC()
    {}
    //: Default constructor.
    
    BLinkC(const DataT &dat)
      : SmartPtrC<BLinkBodyC<DataT> >(*new BLinkBodyC<DataT>(dat))
    {}
    //: Construct from data.
    
    BLinkC(const DataT &dat,const SmartPtrC<BLinkBodyC<DataT> > &nnext)
      : SmartPtrC<BLinkBodyC<DataT> >(*new BLinkBodyC<DataT>(dat,nnext))
    {}
    //: Construct from data, and next in list
    
    BLinkC(RCHandleC<BLinkBodyC<DataT> > &handle)
      : SmartPtrC<BLinkBodyC<DataT> >(handle)
    {}
    //: Construct from a base handle.
    
  protected:
    BLinkC(BLinkBodyC<DataT> &body)
      : SmartPtrC<BLinkBodyC<DataT> >(body)
    {}
    //: Body constructor.
    
    BLinkBodyC<DataT> &Body()
    { return RCHandleC<BLinkBodyC<DataT> >::Body(); }
    //: Access body.

    const BLinkBodyC<DataT> &Body() const
    { return RCHandleC<BLinkBodyC<DataT> >::Body(); }
    //: Access body.
    
  public:
    DataT &Data()
    { return Body().Data(); }
    //: Access data.
    
    const DataT &Data() const
    { return Body().Data(); }
    //: Access data.
    
    SmartPtrC<BLinkBodyC<DataT> > &Next()
    { return Body().Next(); }
    //: Access next in list.
    
    const SmartPtrC<BLinkBodyC<DataT> > &Next() const
    { return Body().Next(); }
    //: Access next in list.
    
  };
  
  
}

#endif
