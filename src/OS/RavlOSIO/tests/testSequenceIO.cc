// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//! rcsid="$Id$"
//! lib=RavlOSIO
//! file="Ravl/OS/IO/testSequenceIO.cc"
//! author="Charles Galambos"
//! docentry="Ravl.API.OS.Sequence"
//! userlevel=Develop

#include "Ravl/DP/SPort.hh"
#include "Ravl/DP/FileFormat.hh"
#include "Ravl/DP/SequenceIO.hh"
#include "Ravl/DP/Converter.hh"

using namespace RavlN;

//////////////// DUMMY SEQUENCE ///////////////////////////////////////////

class DPISPortTestBodyC 
  : public DPISPortBodyC<UIntT>
{
public:
  
  DPISPortTestBodyC(UIntT nsize = 10)
    : place(0),
      size(nsize)
  {}
  //: Default

  virtual UIntT Get() {
    return place++;
  }
  
  virtual bool Get(UIntT &buff) { 
    if(place >= size)
      return false;
    buff = place++;
    return true;
  }
  
  virtual bool IsGetReady() const
  { return place >= size; }
  //: Is some data ready ?
  // true = yes.
  // Defaults to !IsGetEOS().
  
  virtual bool IsGetEOS() const 
  { return place >= size; }
  //: Has the End Of Stream been reached ?
  // true = yes.

  virtual bool Seek(UIntT off) { 
    if(off >= size)
      return false;
    place = off; 
    return true;
  }
  //: Seek to location in stream.
  // Returns false, if seek failed. (Maybe because its
  // not implemented.)
  // if an error occurered (Seek returned False) then stream
  // position will not be changed.
  
  virtual bool DSeek(IntT off) { 
    if(off < 0 && ((UIntT) ((int) off * -1)) > place)
      return false;
    UIntT nplace = place + off; 
    if(nplace > size)
      return false;
    place = nplace;
    return true;
  }
  //: Delta Seek, goto location relative to the current one.
  // The default behavour of this functions is :
  // Do some error checking then:
  //   Seek((UIntT)((IntT) Tell() + off));
  // if an error occurered (DSeek returned False) then stream
  // position will not be changed.
  
  virtual UIntT Tell() const
  { return place; }
  //: Find current location in stream.
  // May return ((UIntT) (-1)) if not implemented.
  
  virtual UIntT Size() const
  { return size; }
  
  //: Find the total size of the stream.  (assuming it starts from 0)
  // May return ((UIntT) (-1)) if not implemented.

protected:
  UIntT place;
  UIntT size;
};

class DPISPortTestC 
  : public DPISPortC<UIntT>
{
public:
  DPISPortTestC()
    : DPEntityC(true)
  {}
  //: Default constructor.
  // creates an invalid handle.
  
  DPISPortTestC(UIntT nsize)
    : DPEntityC(*new DPISPortTestBodyC(nsize))
  {}
  //: Size constructor.
  
};


//////////////// FORMAT FOR DUMMY SEQUENCE ///////////////////////////////////////////

//! userlevel=Develop
//: SPortTest File format information.

class FileFormatSPortTestBodyC 
  : public FileFormatBodyC 
{
public:
  FileFormatSPortTestBodyC()
  {}
  //: Constructor.
  
  virtual const type_info &ProbeLoad(IStreamC &in,const type_info &obj_type) const
  { return typeid(UIntT); }
  //: Is stream in std stream format ?
  
  virtual const type_info &ProbeLoad(const StringC &filename,IStreamC &in,const type_info &obj_type) const {
    if(filename.length() < 1)
      return typeid(void);
    if(filename[0] != '@')
      return typeid(void);
    if(ExtractDevice(filename) != "TESTSPORT")
      return typeid(void);
    return typeid(UIntT);
  }
  //: Probe for load.
  
  virtual const type_info &ProbeSave(const StringC &filename,const type_info &obj_type,bool forceFormat) const {
    return typeid(void);
  }
  //: Probe for Save.
  
  virtual DPIPortBaseC CreateInput(IStreamC &in,const type_info &obj_type) const {
    return DPISPortTestC(10);
  }
  //: Create a input port for loading.
  // This creates a plain SPortTest stream.
  // Will create an Invalid port if not supported.
  
  virtual DPOPortBaseC CreateOutput(OStreamC &out,const type_info &obj_type) const {
    return DPOPortBaseC();      
  }
  //: Create a output port for saving.
  // This creates a plain SPortTest stream.
  // Will create an Invalid port if not supported.
  
  virtual DPIPortBaseC CreateInput(const StringC &filename,const type_info &obj_type) const {
    return DPISPortTestC(10);
  }
  //: Create a input port for loading from file 'filename'.
  // Will create an Invalid port if not supported. <p>
  
  virtual DPOPortBaseC CreateOutput(const StringC &filename,const type_info &obj_type) const {
    return DPOPortBaseC();      
  }
  //: Create a output port for saving to file 'filename'..
  // Will create an Invalid port if not supported. <p>
  
  virtual const type_info &DefaultType() const
  { return typeid(UIntT); }
  //: Get prefered IO type.
  
  virtual IntT Priority() const { return 1; }
  //: Find the priority of the format. the higher the better.
  // Default is zero, this is better than the default (streams.)
  
  virtual bool IsStream() const { return true; }
  //: Test if format is a fully streamable.
  // check if you can read/write more than object object.
  
protected:
};

/////////////////////////////
//! userlevel=Advanced
//: Create an instance of a SPortTest File Format.

class FileFormatSPortTestC : public FileFormatC<UIntT > {
public:
  FileFormatSPortTestC()
    : FileFormatC<UIntT >(*new FileFormatSPortTestBodyC())
  {}
};

FileFormatSPortTestC testFormat;

///////////////////////////////////////////////////////////////////////////

Int16T UInt2Int16(const UIntT &v)
{ return (Int16T) v; } 

namespace RavlN {
  DP_REGISTER_CONVERSION(UInt2Int16,2);
}

///////////////////////////////////////////////////////////////////////////

int TestPortConv();
int TestISequence();
int TestConvISequence();

int main(int nargs,char **argv) {
  int ln;
  if((ln = TestPortConv()) != 0) {
    cerr << "ERROR at line " << ln << "\n";
    return 1;
  }
  if((ln = TestISequence()) != 0) {
    cerr << "ERROR at line " << ln << "\n";
    return 1;
  }
  if((ln = TestConvISequence()) != 0) {
    cerr << "ERROR at line " << ln << "\n";
    return 1;
  }
  cerr << "Test passed. \n";
  return 0;
}


int TestPortConv() {
  DPISPortTestC ap(100);
  DPIPortBaseC ipBase = ap;
  if(!ipBase.IsValid()) return __LINE__;
  DPISPortC<UIntT> in1(ipBase);
  if(!in1.IsValid()) return __LINE__;
  DPSeekCtrlC in2(ipBase);
  if(!in2.IsValid()) return __LINE__;
  
  StringC fn("hello");
  DPSeekCtrlC sc;
  DPIPortBaseC xip = ((FileFormatBaseC &)testFormat).CreateInput(fn,typeid(UIntT));
  if(!xip.IsValid())
    return __LINE__;
  sc = DPSeekCtrlC(xip); // This may or maynot work...
  if(!sc.IsValid()) return __LINE__;
  cerr << "int TestPortConv(), Passed. \n";
  return 0;
}

int TestISequence() {
  DPISPortC<UIntT> inp;
  if(!OpenISequence(inp,"@TESTSPORT","",true))
    return __LINE__;
  for(UIntT i = 0;i < 10;i++) {
    UIntT t = inp.Tell();
    UIntT v = inp.Get();
    //cerr << "V=" << v << " " << t << "\n";
    if(t != v) return __LINE__;
    if(v != i) return __LINE__;
  }
  UIntT x;
  if(inp.Get(x))
    return __LINE__;
  return 0;
}

int TestConvISequence() {
  DPISPortC<Int16T> inp;
  if(!OpenISequence(inp,"@TESTSPORT","",true))
    return __LINE__;
  for(UIntT i = 0;i < 10;i++) {
    UIntT t = inp.Tell();
    UIntT v = inp.Get();
    //cerr << "V=" << v << " t=" << t << "\n";
    if(t != v) return __LINE__;
    if(v != i) return __LINE__;
  }
  Int16T x;
  if(inp.Get(x))
    return __LINE__;
  return 0;
}
