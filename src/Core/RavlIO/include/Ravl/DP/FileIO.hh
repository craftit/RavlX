// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVLDPFILEIO_HEADER
#define RAVLDPFILEIO_HEADER 1
//////////////////////////////////////////////////////////
//! docentry="Ravl.API.Core.Data Processing.IO" 
//! example=exDataProc.cc
//! file="Ravl/Core/IO/FileIO.hh"
//! lib=RavlIO
//! author="Charles Galambos"
//! date="04/07/1998"
//! rcsid="$Id$"
//! userlevel=Default

#include "Ravl/DP/SPort.hh"
#include "Ravl/String.hh"
#include "Ravl/TypeName.hh"
#include "Ravl/Stream.hh"

namespace RavlN {
  ////////////////////////////////////////////
  //! userlevel=Develop
  //: Save objects to a file.
  // Object must have a stream output function.
  
  template<class DataT>
  class DPOFileBodyC 
    : public DPOPortBodyC<DataT>
  {
  public:
    DPOFileBodyC() {}
    //: Default constructor.
    
    DPOFileBodyC(const StringC &nfname,bool useHeader=false)
      : out(nfname)
    {
#if RAVL_CHECK
      if(!out.good()) 
	cerr << "DPOFileBodyC<DataT>::DPOFileBodyC<DataT>(StringC,bool), Failed to open file '" << nfname << "'.\n";
#endif
      if(useHeader) 
	out << TypeName(typeid(DataT)) << endl;
    }
    //: Construct from a filename.
    
    inline DPOFileBodyC(OStreamC &strmout,bool useHeader=false)
      : out(strmout)
    {
#if RAVL_CHECK
      if(!out.good()) 
	cerr << "DPOFileBodyC<DataT>::DPOFileBodyC<DataT>(OStreamC,bool), Passed bad output stream. \n";
#endif
      if(useHeader) 
	out << TypeName(typeid(DataT)) << endl;
#if RAVL_CHECK
      if(!out.good()) 
	cerr << "DPOFileBodyC<DataT>::DPOFileBodyC<DataT>(OStreamC,bool), Bad stream after writting header! \n";
#endif
    }
    //: Stream constructor.
    
    virtual bool Put(const DataT &dat) { 
#if RAVL_CHECK
      if(!out.good()) {
	cerr << "DPOFileBodyC<DataT>::Put(), Failed because of bad output stream (before write!). \n";
	return false;
      }
#endif
      out << dat << endl; 
#if RAVL_CHECK
      if(!out.good()) 
	cerr << "DPOFileBodyC<DataT>::Put(), Failed because of bad output stream. \n";
#endif
      return out.good(); 
    }
    //: Put data.
    
    virtual IntT PutArray(const SArray1dC<DataT> &data) {
      if(!out.good()) 
	return data.Size();
      for(SArray1dIterC<DataT> it(data);it;it++) {
	out << *it << endl;
	if(!out.good()) {
#if RAVL_CHECK
	  cerr << "DPOFileBodyC<DataT>::PutArray(), Endded early because of bad output stream. \n";	
#endif
	  return it.Index().V();
	}
      }
      return data.Size();
    }
    //: Put an array of data to stream.
    // returns the number of elements processed.
    
    virtual bool IsPutReady() const 
    { return out.good(); }
    //: Is port ready for data ?
    
    virtual bool Save(std::ostream &sout) const  { 
      sout << out.Name(); 
      return true; 
    }
    //: Save to std::ostream.
    
  private:
    OStreamC out;
  };

  /////////////////////////////////////
  //! userlevel=Develop
  //: Load objects from a file.
  // Object must have a stream input function
  // and a default constructor.
  
  template<class DataT>
  class DPIFileBodyC 
    : public DPISPortBodyC<DataT>
  {
  public:
    DPIFileBodyC() 
      : dataStart(0),
	off(0)
    {}
    //: Default constructor.
    
    DPIFileBodyC(const StringC &nfname,bool useHeader = false)
      : in(nfname),
	dataStart(0),
	off(0)
    {
#if RAVL_CHECK
      if(!in.good()) 
	cerr << "DPOFileBodyC<DataT>::DPOFileBodyC<DataT>(StringC,bool), WARNING: Failed to open file '" << nfname << "'.\n";
#endif
      if(useHeader) {
	StringC classname;
	in >> classname; // Skip class name.
	if(classname != TypeName(typeid(DataT))) 
	  cerr << "DPIFileC ERROR: Bad file type: " << classname << " Expected:" << TypeName(typeid(DataT)) << " \n";
      }
      dataStart = in.Tell();
    }
    //: Construct from a filename.
    
    inline DPIFileBodyC(IStreamC &strmin,bool useHeader = false)
      : in(strmin),
	dataStart(0),
	off(0)
    {
#if RAVL_CHECK
      if(!in.good()) 
	cerr << "DPIFileBodyC<DataT>::DPIFileBodyC<DataT>(OStreamC,bool), WARNING: Passed bad input stream. \n";
#endif
      if(useHeader) {
	StringC classname;
	in >> classname; // Skip class name.
	if(classname != TypeName(typeid(DataT))) 
	  cerr << "DPIFileC ERROR: Bad file type. " << classname << " Expected:" << TypeName(typeid(DataT)) << " \n";
      }
      dataStart = in.Tell();
    }
    //: Stream constructor.
    
    virtual bool IsGetEOS() const 
    { return (in.eof()) || !in.good(); }
    //: Is valid data ?
    
    virtual DataT Get() { 
      if(in.IsEndOfStream())
	throw DataNotReadyC("DPIFileBodyC::Get(), End of input stream. ");
      DataT ret; 
      in >> ret;
      off++;
      if(!in.good())
	throw DataNotReadyC("DPIFileBodyC::Get(), Bad input stream. ");    
      return ret; 
    }
    //: Get next piece of data.
    
    virtual bool Get(DataT &buff) { 
      if(in.IsEndOfStream())
	return false;
      in >> buff;
      off++;
      return in.good();
    }
    //: Get next piece of data.
    
    virtual IntT GetArray(SArray1dC<DataT> &data) {
      if(!in.good()) 
	return data.Size();
      for(SArray1dIterC<DataT> it(data);it;it++) {
	in >> *it;
	off++;
	if(!in.good()) {
#if RAVL_CHECK
	  cerr << "DPIFileBodyC<DataT>::GetArray(), Ended early because of bad input stream. \n";	
#endif
	  return it.Index().V();
	}
      }
      return data.Size();
    }
    //: Get multiple pieces of input data.
    // returns the number of elements processed.
    
    virtual bool Save(std::ostream &out) const { 
      out << in.Name(); 
      return true; 
    }
    //: Save to std::ostream.
    
    virtual bool Seek(UIntT newOff) {
      if(newOff == 0) {
	in.Seek(dataStart);
	off = 0;
	return true;
      }
      return false;
    }
    //: Seek to position in stream.
    // Currently will only seek to begining of stream.
    
    virtual UIntT Tell() const
    { return off; }
    //: Get offset in stream.
    
    virtual UIntT Size() const
    { return (UIntT) (-1); }
    //: Get size of stream. 
  
  private:
    IStreamC in;
    streampos dataStart;
    UIntT off;
  };
  
  ///////////////////////////////
  //! userlevel=Normal
  //: File output stream.
  // Object must have a stream output function.
  
  template<class DataT>
  class DPOFileC 
    : public DPOPortC<DataT>
  {
  public:
    inline DPOFileC() {}
    //: Default constructor.
    
    inline DPOFileC(OStreamC &strm,bool useHeader=false)
      : DPEntityC(*new DPOFileBodyC<DataT>(strm,useHeader))
    {}
    //: Stream constructor.
    
    inline DPOFileC(const StringC &fname,bool useHeader=false) 
      : DPEntityC(*new DPOFileBodyC<DataT>(fname,useHeader))
    {}
    
    //: Filename constructor.  
  };
  
  //////////////////////////////////
  //! userlevel=Normal
  //: File input stream.
  // Object must have a stream input function
  // and a default constructor.
  
  template<class DataT>
  class DPIFileC 
    : public DPISPortC<DataT> 
  {
  public:
    inline DPIFileC() {}
    //: Default constructor.
    
    inline DPIFileC(IStreamC &strm,bool useHeader=false)
      : DPEntityC(*new DPIFileBodyC<DataT>(strm,useHeader))
    {}
    //: Stream constructor.
    
    inline DPIFileC(const StringC &afname,bool useHeader=false)
      : DPEntityC(*new DPIFileBodyC<DataT>(afname,useHeader))
    {}
    //: Filename constructor.  
  }; 
}
#endif
