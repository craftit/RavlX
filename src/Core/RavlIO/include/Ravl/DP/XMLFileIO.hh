// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVL_XMLFILEIO_HEADER
#define RAVL_XMLFILEIO_HEADER 1
//////////////////////////////////////////////////////////
//! docentry="Ravl.API.Core.Data Processing.IO" 
//! example=exDataProc.cc
//! lib=RavlIO
//! author="Charles Galambos"
//! date="21/10/2002"
//! rcsid="$Id$"
//! file="Ravl/Core/IO/XMLFileIO.hh"
//! userlevel=Default

#include "Ravl/DP/Port.hh"
#include "Ravl/String.hh"
#include "Ravl/TypeName.hh"
#include "Ravl/XMLStream.hh"

namespace RavlN {
  
  /////////////////////////////////////////////
  //! userlevel=Develop
  //: Save objects in binary to a file.
  // Object must have a binary stream output function.
  
  template<class DataT>
  class DPOXMLFileBodyC 
    : public DPOPortBodyC<DataT>
  {
  public:
    DPOXMLFileBodyC() 
    {}
    //: Default constructor.
    
    DPOXMLFileBodyC(const StringC &nfname,bool useHeader=false)
      : out(nfname),
        version(0)
    {
#ifdef RAVL_CHECK
      if(!out.good()) 
	cerr << "DPOXMLFileBodyC<DataT>::DPOXMLFileBodyC<DataT>(StringC,bool), Failed to open file.\n";
#endif
      if(useHeader) {
	out.WriteHeader();
	RCHashC<StringC,StringC> attr;
	attr["class"] = TypeName(typeid(DataT));
	out.WritePI("RAVL",attr);
      }
    }
    //: Construct from a filename.
    
    inline DPOXMLFileBodyC(XMLOStreamC &strmout,bool useHeader=false)
      : out(strmout),
      version(0)
    {
#ifdef RAVL_CHECK
      if(!out.good()) 
	cerr << "DPOXMLFileBodyC<DataT>::DPOXMLFileBodyC<DataT>(OStreamC,bool), Passed bad output stream. \n";
#endif
      if(useHeader) {
	out.WriteHeader();
	RCHashC<StringC,StringC> attr;
	attr["class"] = TypeName(typeid(DataT));
	out.WritePI("RAVL",attr);
#ifdef RAVL_CHECK
	if(!out.good()) 
	  cerr << "DPOXMLFileBodyC<DataT>::DPOXMLFileBodyC<DataT>(XMLOStreamC,bool), Bad stream after writting header! \n";
#endif
      }
    }
    //: Stream constructor.
    
    virtual bool Put(const DataT &dat) { 
#ifdef RAVL_CHECK
      if(!out.good()) {
	cerr << "DPOXMLFileBodyC<DataT>::Put(), Failed because of bad output stream (before write!). \n";
	return false;
      }
#endif
      out << dat;
#ifdef RAVL_CHECK
      if(!out.good()) 
	cerr << "DPOXMLFileBodyC<DataT>::Put(), Failed because of bad output stream. \n";
#endif
      return out.good(); 
    }
    //: Put data.
    
    virtual IntT PutArray(const SArray1dC<DataT> &data) {
      if(!out.good()) 
	return data.Size();
      for(SArray1dIterC<DataT> it(data);it;it++) {
	out << *it;
	if(!out.good()) {
#ifdef RAVL_CHECK
	  cerr << "DPOXMLFileBodyC<DataT>::PutArray(), Endded early because of bad output stream. \n";	
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
    
    virtual bool Save(std::ostream &sout) const 
      { sout << out.Name(); return true; }
    //: Save to std::ostream.
    
  private:
    XMLOStreamC out;
    UIntT version;
  };

  /////////////////////////////////////
  //! userlevel=Develop
  //: Load objects in binary from a file.
  // Object must have a binary stream input function
  // and a default constructor.
  
  template<class DataT>
  class DPIXMLFileBodyC 
    : public DPIPortBodyC<DataT>
  {
  public:
    DPIXMLFileBodyC() {}
    //: Default constructor.
    
    DPIXMLFileBodyC(const StringC &nfname,bool useHeader = false)
      : in(nfname),
      version(0)
    {
      if(useHeader) {
	if(!in.ReadHeader()) {
	  cerr << "DPIXMLFileC ERROR: File has no XML header. \n";
	  return ;
	} 
	StringC strmName;
	RCHashC<StringC,StringC> attrs;
	in.ReadTag(strmName,attrs);
	if(strmName != "RAVL") {
	  cerr << "DPIXMLFileC ERROR: Not a RAVL stream. \n";
	  return ;
	}
	StringC classname = attrs["class"];
	if(classname != TypeName(typeid(DataT))) 
	  cerr << "DPIXMLFileC ERROR: Bad file type: " << classname << " Expected:" << TypeName(typeid(DataT)) << " \n";
      }
    }
    //: Construct from a filename.
    
    inline DPIXMLFileBodyC(XMLIStreamC &strmin,bool useHeader = false)
      : in(strmin),
        version(0)
    {
      if(useHeader) {
	if(!in.ReadHeader()) {
	  cerr << "DPIXMLFileC ERROR: File has no XML header. \n";
	  return ;
	} 
	StringC strmName;
	RCHashC<StringC,StringC> attrs;
	in.ReadTag(strmName,attrs);
	if(strmName != "RAVL") {
	  cerr << "DPIXMLFileC ERROR: Not a RAVL stream. \n";
	  return ;
	}
	StringC classname = attrs["class"];
	if(classname != TypeName(typeid(DataT))) 
	  cerr << "DPIXMLFileC ERROR: Bad file type: " << classname << " Expected:" << TypeName(typeid(DataT)) << " \n";
      }
    }
    //: Stream constructor.
    
    virtual bool IsGetEOS() const 
      { return (!in.good() || in.eof()); }
    //: Is valid data ?
  
    virtual DataT Get() { DataT ret; in >> ret; return ret; }
    //: Get next piece of data.
    
    virtual bool Get(DataT &buff) { 
      if(in.IsEndOfStream())
	return false;
      in >> buff;
      return true; 
    }
    //: Get next piece of data.
    
    virtual IntT GetArray(SArray1dC<DataT> &data) {
      for(SArray1dIterC<DataT> it(data);it;it++) {
	in >> *it;
	if(!in.good()) {
#ifdef RAVL_CHECK
	  cerr << "DPIXMLFileBodyC<DataT>::GetArray(), Ended early because of bad input stream. \n";	
#endif
	  return it.Index().V();
	}
      }
      return data.Size();
    }
    //: Get multiple pieces of input data.
    
    virtual bool Save(std::ostream &out) const 
      { out << in.Name(); return true; }
    //: Save to std::ostream.
    
  private:
    XMLIStreamC in;
    UIntT version;
  };
  
  ///////////////////////////////
  //! userlevel=Normal
  //: XMLary file output stream.
  // Object must have a binary stream output function.
  
  template<class DataT>
  class DPOXMLFileC 
    : public DPOPortC<DataT> 
  {
  public:
    inline DPOXMLFileC() {}
    //: Default constructor.
    
    inline DPOXMLFileC(XMLOStreamC &strm,bool useHeader=false)
      : DPEntityC(*new DPOXMLFileBodyC<DataT>(strm,useHeader))
      {}
    //: Stream constructor.
    
    inline DPOXMLFileC(const StringC &fname,bool useHeader=false) 
      : DPEntityC(*new DPOXMLFileBodyC<DataT>(fname,useHeader))
      {}
    
    //: Filename constructor.  
  };
  
  //////////////////////////////////
  //! userlevel=Normal
  //: XMLary file input stream.
  // Object must have a binary stream input function
  // and a default constructor.
  
  template<class DataT>
  class DPIXMLFileC 
    : public DPIPortC<DataT> 
  {
  public:
    inline DPIXMLFileC() {}
    //: Default constructor.
    
    inline DPIXMLFileC(XMLIStreamC &strm,bool useHeader=false)
      : DPEntityC(*new DPIXMLFileBodyC<DataT>(strm,useHeader))
      {}
    //: Stream constructor.
    
    inline DPIXMLFileC(const StringC &afname,bool useHeader=false)
      : DPEntityC(*new DPIXMLFileBodyC<DataT>(afname,useHeader))
      {}
    //: Filename constructor.  
  };
}


#endif
