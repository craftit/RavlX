// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVL_AVERAGE_HEADER
#define RAVL_AVERAGE_HEADER 1
/////////////////////////////////////////////////////
//! docentry="Ravl.API.Math.Averages"
//! userlevel=Normal
//! author="Charles Galambos"
//! lib=RavlCore
//! file="Ravl/Core/Base/Average.hh"

#include "Ravl/Types.hh"

namespace RavlN {
  
  template<class DataT>
  inline DataT Average(const DataT &a,const DataT &b)
  { return (a+b)/2.0; }
  //: Take average of 2 numbers.
  // This is useful to deal with types limited value ranges. as it can be overridden.
  
  template<class DataT>
  inline DataT Average(const DataT &a,const DataT &b,const DataT &c)
  { return (a+b+c)/3.0; }
  //: Take average of 3 numbers.
  // This is useful to deal with types limited value ranges. as it can be overridden.
  
  template<class DataT>
  inline DataT Average(const DataT &a,const DataT &b,const DataT &c,const DataT &d)
  { return (a+b+c+d)/4.0; }
  //: Take average of 4 numbers.
  // This is useful to deal with types limited value ranges. as it can be overridden.
  
  inline ByteT Average(const ByteT &a,const ByteT &b)
  { return (ByteT)((IntT)(a+b)/2); }
  //: Take average of 2 numbers.
  // This is useful to deal with types limited value ranges. e.g.
  
  inline ByteT Average(const ByteT &a,const ByteT &b,const ByteT &c)
  { return (ByteT)((IntT)(a+b+c)/3); }
  //: Take average of 3 numbers.
  
  inline ByteT Average(const ByteT &a,const ByteT &b,const ByteT &c,const ByteT &d)
  { return (ByteT)((IntT)(a+b+c+d)/4); }
  //: Take average of 4 numbers.
  
  inline SByteT Average(const SByteT &a,const SByteT &b)
  { return (SByteT)((IntT)(a+b)/2); }
  //: Take average of 2 numbers.
  
  inline SByteT Average(const SByteT &a,const SByteT &b,const SByteT &c)
  { return (SByteT)((IntT)(a+b+c)/3); }
  //: Take average of 3 numbers.
  
  inline SByteT Average(const SByteT &a,const SByteT &b,const SByteT &c,const SByteT &d)
  { return (SByteT)((IntT)(a+b+c+d)/4); }
  //: Take average of 4 numbers.
  
  inline Int16T Average(const Int16T &a,const Int16T &b)
  { return (Int16T)((IntT)(a+b)/2); }
  //: Take average of 2 numbers.
  
  inline Int16T Average(const Int16T &a,const Int16T &b,const Int16T &c)
  { return (Int16T)((IntT)(a+b+c)/3); }
  //: Take average of 3 numbers.
  
  inline Int16T Average(const Int16T &a,const Int16T &b,const Int16T &c,const Int16T &d)
  { return (Int16T)((IntT)(a+b+c+d)/4); }
  //: Take average of 4 numbers.
  
  inline UInt16T Average(const UInt16T &a,const UInt16T &b)
  { return (UInt16T)((IntT)(a+b)/2); }
  //: Take average of 2 numbers.
  
  inline UInt16T Average(const UInt16T &a,const UInt16T &b,const UInt16T &c)
  { return (UInt16T)((IntT)(a+b+c)/3); }
  //: Take average of 3 numbers.
  
  inline UInt16T Average(const UInt16T &a,const UInt16T &b,const UInt16T &c,const UInt16T &d)
  { return (UInt16T)((IntT)(a+b+c+d)/4); }
  //: Take average of 4 numbers.
  
}
#endif
