// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVL_SEQUENCEIO_HEADERS
#define RAVL_SEQUENCEIO_HEADERS 1
//////////////////////////////////////////////////////////////
//! docentry="Ravl.API.OS.Sequence"
//! rcsid="$Id$"
//! userlevel=Normal
//! file="Ravl/OS/IO/SequenceIO.hh"
//! lib=RavlOSIO
//! author="Charles Galambos"
//! date="08/04/1999"

#include "Ravl/DP/SPort.hh"
#include "Ravl/DP/SPortAttach.hh"
#include "Ravl/String.hh"

namespace RavlN
{
  
  bool OpenISequenceBase(DPIPortBaseC &ip,DPSeekCtrlC &sc,const StringC &filename,const StringC &fileformat,const type_info &obj_type,bool verbose = false);
  //! userlevel=Develop
  //: Open input stream base.
  // Returns true on success.
  
  
  bool OpenOSequenceBase(DPOPortBaseC &op,DPSeekCtrlC &sc,const StringC &filename,const StringC &fileformat,const type_info &obj_type,bool verbose = false);
  //! userlevel=Develop
  //: Open output stream base.
  // Returns true on success.

  
  template<class DataT>
  bool OpenOSequence(DPOSPortC<DataT> &op,const StringC &filename,const StringC &fileformat = "",bool verbose = false)
  { 
    DPOPortBaseC anOp;
    DPSeekCtrlC sc;
    if(!OpenOSequenceBase(anOp,sc,filename,fileformat,typeid(DataT),verbose))
      return false;
    RavlAssert(anOp.IsValid());
    // Check return'd port is a sport !
    op = DPOSPortC<DataT>(anOp);
    if(!op.IsValid()) {
      if(sc.IsValid())
	op = DPOSPortAttachC<DataT>((DPOPortC<DataT> &)anOp,sc);
      else
	op = DPOSPortAttachC<DataT>((DPOPortC<DataT> &)anOp);
    }
    return true;
  }
  //! userlevel=Normal
  //: Open a seekable output stream.
  // - Not all streams are seekable, the seek/tell functions are not garanteed to work
  // for all formats. <p>
  // If the fileformat argument is used, it must be one of the known types
  // returned by "conv -lf".  If the fileformat is a zero length string (the default), the filename extension is used.
  // Returns true on success.
  
  template<class DataT>
  bool OpenISequence(DPISPortC<DataT> &ip,const StringC &filename,const StringC &fileformat = "",bool verbose = false)
  { 
    DPIPortBaseC anIp;
    DPSeekCtrlC sc;
    if(!OpenISequenceBase(anIp,sc,filename,fileformat,typeid(DataT),verbose))
      return false;
    RavlAssert(anIp.IsValid());
    // Check return'd port is a sport !
    ip = DPISPortC<DataT>(anIp);
    //cerr << "Attaching... \n";
    if(!ip.IsValid()) {
      DPIPortC<DataT> newPort(anIp);
      RavlAssert(newPort.IsValid());
      if(sc.IsValid())
	ip = DPISPortAttachC<DataT>(newPort,sc);
      else
	ip = DPISPortAttachC<DataT>(newPort,anIp);
    }
    //cerr << "Attach done. \n";
    return true;
  }
  //! userlevel=Normal
  //: Open a seekable input stream.
  // Note: Not all streams are seekable, the seek/tell functions are not garanteed to work
  // for all formats. <p>
  // If the fileformat argument is used, it must be one of the known types
  // returned by "conv -lf".  If the fileformat is a zero length string (the default), all formats are considered.
  // Returns true on success.

  
  template<class DataT>
  bool OpenOSequence(DPOPortC<DataT> &op,const StringC &filename,const StringC &fileformat = "",bool verbose = false)
  { 
    DPOPortBaseC anOp;
    DPSeekCtrlC sc;
    if(!OpenOSequenceBase(anOp,sc,filename,fileformat,typeid(DataT),verbose))
      return false;
    RavlAssert(anOp.IsValid());
    op = DPOPortC<DataT>(anOp); // This makes sure type checking gets done.
    return op.IsValid();
  }
  //! userlevel=Normal
  //: Open a normal output stream
  // If the fileformat argument is used, it must be one of the known types
  // returned by "conv -lf".  If the fileformat is a zero length string (the
  // default), the filename extension is used.
  // Returns true on success.
  
  template<class DataT>
  bool OpenISequence(DPIPortC<DataT> &ip,const StringC &filename,const StringC &fileformat = "",bool verbose = false)
  { 
    DPIPortBaseC anIp;
    DPSeekCtrlC sc;
    if(!OpenISequenceBase(anIp,sc,filename,fileformat,typeid(DataT),verbose))
      return false;
    RavlAssert(anIp.IsValid());
    ip = DPIPortC<DataT>(anIp);  // This makes sure type checking gets done.
    return ip.IsValid();
  }
  //! userlevel=Normal
  //: Open a normal input stream
  // If the fileformat argument is used, it must be one of the known types
  // returned by "conv -lf".  If the fileformat is a zero length string (the default), all formats are considered.
  // Returns true on success.
}


#endif
