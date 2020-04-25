// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2004, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVL_MEMIO_HEADER
#define RAVL_MEMIO_HEADER 1
/////////////////////////////////////////////////////////////
//! file="Ravl/Core/IO/MemIO.hh"
//! lib=RavlIO
//! date="03/12/2004"
//! author="Charles Galambos"
//! docentry="Ravl.API.Core.IO"
//! rcsid="$Id$"
//! userlevel=Normal

#include "Ravl/IO.hh"
#include "Ravl/BufStream.hh"
#include "Ravl/StrStream.hh"

// Declare DODEBUG in the .cc module to allow sufficient trace granularity
#if DODEBUG
#define ONDEBUG_MEMIO(x) x
#else
#define ONDEBUG_MEMIO(x)
#endif

namespace RavlN {
  
  template<class DataT>
  bool MemLoad(const SArray1dC<char> &buffer,DataT &obj,StringC fileformat = "",bool verbose = false) {
    BufIStreamC bufStrm(buffer);
    DPIPortC<DataT> in(RavlN::BaseLoad(bufStrm,fileformat,typeid(DataT),verbose ONDEBUG_MEMIO(|| 1)));
    if(!in.IsValid()) {
      if(verbose ONDEBUG_MEMIO(|| 1))
	cerr << "RavlN::Load(), Failed to load object from stream.\n";
      return false;
    }
    return in.Get(obj);
  }
  //! userlevel=Normal
  //: Load a single object from a stream
  //!param: buffer - buffer from which to load object.
  //!param: obj - name of C++ object
  //!param: fileformat - format of object file. If fileformat string is empty, the file format is detected automatically.  To get a list of file formats, type <code>conv -lf</code>
  //!param: verbose -  if true, a message is printed to stdout describing the filename, the format used, the object type being loaded and any status information. 
  // More information <a href="../Tree/Ravl.API.Core.IO.html#LoadSave">here</a>
  
  template<class DataT>
  bool MemSave(SArray1dC<char> &buffer,const DataT &obj,StringC fileformat = "",bool verbose = false) {
    BufOStreamC bufStrm;
    DPOPortC<DataT> out(BaseSave(bufStrm,fileformat,typeid(DataT),verbose ONDEBUG_MEMIO(|| 1)));
    if(!out.IsValid()) {
      if(verbose ONDEBUG_MEMIO(|| 1))
	cerr << "RavlN::Save(), Failed to save object to stream\n";
      return false; // Failed to find format.
    }
    if(!out.Put(obj)) {
      if(verbose ONDEBUG_MEMIO(|| 1))
	cerr << "WARNING: Save to stream failed. \n";
      return false;
    }
    bufStrm.Close();
    buffer = bufStrm.Data();
    return true;
  }
  //: Save a single object to a stream
  //!param: buffer - Where to save the data to.
  //!param: obj - name of C++ object
  //!param: fileformat - format of object file. If fileformat string is empty, the file format is detected automatically.  To get a list of file formats, type <code>conv -lf</code>
  //!param: verbose -  if true, a message is printed to stdout describing the filename, the format used, the object type being loaded and any status information. 
  // More information <a href="../Tree/Ravl.API.Core.IO.html#LoadSave">here</a>

  
  template<class DataT>
  bool MemLoad(const StringC &buffer,DataT &obj,StringC fileformat = "",bool verbose = false) {
    StrIStreamC bufStrm(buffer);
    DPIPortC<DataT> in(RavlN::BaseLoad(bufStrm,fileformat,typeid(DataT),verbose ONDEBUG_MEMIO(|| 1)));
    if(!in.IsValid()) {
      if(verbose ONDEBUG_MEMIO(|| 1))
	cerr << "RavlN::Load(), Failed to load object from stream.\n";
      return false;
    }
    return in.Get(obj);
  }
  //! userlevel=Normal
  //: Load a single object from a stream
  //!param: buffer - buffer from which to load object.
  //!param: obj - name of C++ object
  //!param: fileformat - format of object file. If fileformat string is empty, the file format is detected automatically.  To get a list of file formats, type <code>conv -lf</code>
  //!param: verbose -  if true, a message is printed to stdout describing the filename, the format used, the object type being loaded and any status information. 
  // More information <a href="../Tree/Ravl.API.Core.IO.html#LoadSave">here</a>
  
  template<class DataT>
  bool MemSave(StringC &buffer,const DataT &obj,StringC fileformat = "",bool verbose = false) {
    StrOStreamC bufStrm;
    DPOPortC<DataT> out(BaseSave(bufStrm,fileformat,typeid(DataT),verbose ONDEBUG_MEMIO(|| 1)));
    if(!out.IsValid()) {
      if(verbose ONDEBUG_MEMIO(|| 1))
	cerr << "RavlN::Save(), Failed to save object to stream\n";
      return false; // Failed to find format.
    }
    if(!out.Put(obj)) {
      if(verbose ONDEBUG_MEMIO(|| 1))
	cerr << "WARNING: Save to stream failed. \n";
      return false;
    }
    bufStrm.Close();
    buffer = bufStrm.String();
    return true;
  }
  //: Save a single object to a stream
  //!param: buffer - Where to save the data to.
  //!param: obj - name of C++ object
  //!param: fileformat - format of object file. If fileformat string is empty, the file format is detected automatically.  To get a list of file formats, type <code>conv -lf</code>
  //!param: verbose -  if true, a message is printed to stdout describing the filename, the format used, the object type being loaded and any status information. 
  // More information <a href="../Tree/Ravl.API.Core.IO.html#LoadSave">here</a>
}


#endif
