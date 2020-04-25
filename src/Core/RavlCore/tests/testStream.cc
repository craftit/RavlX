// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//! rcsid="$Id$"
//! lib=RavlCore
//! file="Ravl/Core/Base/testStream.cc"
//! date="23/04/1998"
//! author="Charles Galambos"
//! docentry="Ravl.API.Core.IO.Streams"
//! userlevel=Develop

// Do a quick test of stream functionality.  We don't
// have any OS file functions so unfortunatly we can't
// clear up after ourselves very well.


#include "Ravl/Types.hh"
#include "Ravl/BinStream.hh"
#include "Ravl/BitStream.hh"
#include "Ravl/StrStream.hh"
#include "Ravl/TFVector.hh"
#include "Ravl/String.hh"

#include "Ravl/fdstreambuf.hh"
#include "Ravl/fdstream.hh"
#include "Ravl/FuncStream.hh"
#include "Ravl/Calls.hh"
#include "Ravl/Math.hh"
#include "Ravl/UnitTest.hh"
#include <string.h>

#if RAVL_HAVE_UNISTD_H
#include <unistd.h>
#endif
#if RAVL_HAVE_PROCESS_H
#include <process.h>
#endif

using namespace RavlN;

int SimpleTest();
int VectorTest();
int StringTest();
int StrStreamTest();
int testRawFD(); /* NB. This is only useful on some platforms. */
int testFDStream();
int testFDStream2();
int testBitStream();
int testFuncStream();

#if RAVL_OS_WIN32
StringC testFile = "C:/WINDOWS/Temp/testStream" + StringC((IntT) getpid());
#else
StringC testFile = "/tmp/testStream" + StringC((IntT) getpid());
#endif

int main() {
  std::cerr << "Starting test... \n";
  // NOTE: test may fail if file exits and belongs to someone else!!
  RAVL_RUN_TEST(SimpleTest());
  RAVL_RUN_TEST(VectorTest());
  RAVL_RUN_TEST(StringTest());
  RAVL_RUN_TEST(StrStreamTest());
  RAVL_RUN_TEST(testFDStream());
  RAVL_RUN_TEST(testFDStream2());
#if RAVL_HAVE_INTFILEDESCRIPTORS
  RAVL_RUN_TEST(testRawFD());
#endif
  RAVL_RUN_TEST(testBitStream());
  RAVL_RUN_TEST(testFuncStream());
  std::cerr << "Test passed. \n";
  return 0;
}

int VectorTest() {
  //std::cerr << "VectorTest(), Called. \n";
  TFVectorC<ByteT,3> val;
  
  val[0] = 1;
  val[1] = 2;
  val[2] = 3;
  {
    BinOStreamC os(testFile);
    if(!os.Stream()) {
      std::cerr << "Failed to open output. \n";
      return __LINE__;
    }
    
    os << val;
  }
  
  val[0] = 4;
  val[1] = 4;
  val[2] = 4;
  {
    BinIStreamC is(testFile);
    if(!is.Stream()) {
      std::cerr << "Failed to open output. \n";
      return __LINE__;
    }
    is >> val;
    if(val[0] != 1 || val[1] != 2 || val[2] != 3) {
      std::cerr << "Test failed. \n";
      return __LINE__; 
    }
  }
  //std::cerr << "VectorTest(), Done. \n";
  return 0;
  
}

int StringTest() {
  StringC strings[] = {"Hello", "this", "is", "a", "test"};
  {
    OStreamC os(testFile);
    if(!os) {
      std::cerr << "Failed to open output. \n";
      return __LINE__;
    }
    for (int i=0; i<5; i++) {
      os << i << " " << strings[i] << " ";
    }
    if(!os) {
      std::cerr << "Failed output stream bad after write. \n";
      return __LINE__;
    }
  }
  
  {
    IStreamC is(testFile);
    if(!is) {
      std::cerr << "Failed to open output. \n";
      return __LINE__;
    }
    for (int i=0; i<5; i++) {
      IntT ii;
      StringC str;
      is >> ii >> str;
      if (ii != i || str != strings[i]) {
	std::cerr << "Test failed on string " << i << " = " << ii << ", '" << str << "'\n";
	return __LINE__; 
      }
      else {
	//std::cerr << str << " ";
      }
    }
    //std::cerr << "\n";
  }
  //std::cerr << "StringTest(), Done. \n";
  return 0;
  
}


int SimpleTest() {
  
  ByteT val;
  RealT rval = 0.5;
  FloatT fval = 0.7f;
  val = 1;
  {
    BinOStreamC os(testFile);
    if(!os.Stream()) {
      std::cerr << "Failed to open output. \n";
      return __LINE__;
    }
    os << val << rval << fval;
  }
  val = 2;
  RealT nval;
  FloatT nfval;
  {
    BinIStreamC is(testFile);
    if(!is.Stream()) {
      std::cerr << "Failed to open output. \n";
      return __LINE__;
    }
    is >> val >> nval >> nfval;
    if(val != 1) {
      std::cerr << "Test failed. " << ((int) val) << "\n";
      return __LINE__;
    }
  }
  if(Abs(nval - rval) > 0.00000001)  return __LINE__;
  if(Abs(nfval - fval) > 0.00000001) return __LINE__;
  //std::cerr << "SimpleTest(), Done. \n";
  return 0;
}

int StrStreamTest() {
  {
    // Test write of a simple string.
    StrOStreamC sos;
    StringC hello("hello");
    sos << hello;
    if(sos.String() != hello)
      return __LINE__;
  }
  {
    StrOStreamC sos;
    StringC hello("hello");
    sos << hello << ' ' << hello << "zyz" << std::flush << "x";
    sos << hello.before('o');
    sos << hello.after('h');
    OStreamC *tmp = new OStreamC(sos);
    delete tmp;
    StringC got = sos.String();
    //std::cerr << "'" << got << "'\n";
    if(got != "hello hellozyzxhellello") {
      //std::cerr << " Got:'" << got << "'\n";
      return __LINE__;
    }
  }
  {
    StrOStreamC sos;
    StringC hello("Greetings");
    SubStringC rol = hello.from(0);
    sos << rol;
    StringC ret = sos.String();
    //std::cerr << "'" << ret << "'\n";
    
  }  
  {
    IStreamC strm;
    {
      StringC tst("Hello\nThere");
      StrIStreamC strStream(tst);
      strm = strStream;
      // Just to be sure the original stream is gone.
      strStream = StrIStreamC();
    }
    StringC instr;
    IStreamC strm2 = strm;
    if(!strm2)
      return __LINE__;
    strm >> instr;
    if(instr != "Hello") return __LINE__;
    strm >> instr;
    if(instr != "There") return __LINE__;
    strm >> instr;
    if(strm2)
      return __LINE__;
  }
  {
    StringC tst("Hello\nThere");
    StrIStreamC strStream(tst);

    std::streampos pos = strStream.Tell();

    StringC word;
    strStream >> word;
    if(word != "Hello") return __LINE__;

    strStream.Seek(pos);
    StringC word2;
    strStream >> word2;
    if(word2 != "Hello") return __LINE__;

  }

  return 0;
}

int testFDStream() {
#if RAVL_COMPILER_GCC
  int fds[2];
  pipe(fds);
  char let = 'a';
  //std::cerr << "Writing data... \n";
  
  ofdstream os(fds[1]);
  if(!os)  return __LINE__;
  os << let << let;
  os.flush();
  
  //std::cerr << "Read data... \n";
  
  char rlet1 = 0,rlet2 = 0;
  
  ifdstream is(fds[0]);
  if(!is)  return __LINE__;
  //std::cerr << "Reading byte1 \n";
  is >> rlet1;
  //std::cerr << "Reading byte1 done. Let=" << (int) rlet1 <<" \n";
  if(!is)  return __LINE__;  
  is >> rlet2;
  if(!is)  return __LINE__;  
  
  //std::cerr << "Let=" << (int) let << " RLet1=" << (int) rlet1 << " RLet2=" << (int) rlet2 << "\n";
  
  if(let != rlet1) return __LINE__;
  if(let != rlet2) return __LINE__;
#endif  
  return 0;
}

int testFDStream2()
{
#if RAVL_COMPILER_GCC

  int fds[2];
  pipe(fds);

  OStreamC os(fds[1],true,false);
  IStreamC is(fds[0],true,false);

  const char *someText = "Hello how are you?\n";
  for(int i = 0;i < 10000;i++) {
    os << someText;
    os.os().flush();

    StringC buff;
    RavlN::readline(is,buff,'\n',false);
    if(buff != someText) {
      return __LINE__;
    }
  }

#if 0
  // This blocks, hard to tell if its a deadlock from file io or a bug.
  // really need threads to test this properly.
  const int buffSize = 1024;
  char buff[buffSize];
  for(int i = 0;i < buffSize;i++)
    buff[i] = 'a' + i % 26;
  buff[buffSize-1] = 0;

  os << buff;
  os.os().flush();

  char rbuff[buffSize];
  is >> rbuff;
  for(int i = 0;i < buffSize-1;i++)
    if(rbuff[i] != ('a' + i % 26)) return __LINE__;
#endif
#endif
  return 0;
}

#if RAVL_HAVE_INTFILEDESCRIPTORS
int testRawFD() {
  int fds[2];
  pipe(fds);
  
  char let1 = 'a';
  char let2 = 'b';
  //std::cerr << "Writting data... \n";
  
#if 0
  write(fds[1],&let,1);
  write(fds[1],&let,1);
#else
  OStreamC os(fds[1],true,false);
  if(!os)  return __LINE__;
  os << let1 << let2;
  os.os().flush();
#endif
  //std::cerr << "Read data... \n";
  
  char rlet1 = 0,rlet2 = 0;
  
#if 0
  FILE *inf = fdopen(fds[0],"r");
  fread(&rlet1,1,1,inf);
  fread(&rlet2,1,1,inf);
  fclose(inf);
#elif 0
  read(fds[0],&rlet1,1);
  read(fds[0],&rlet2,1);
#else
  IStreamC is(fds[0],true,false);
  if(!is)  return __LINE__;
  //std::cerr << "Reading byte1 \n";
  is >> rlet1;
  //std::cerr << "Reading byte1 done. Let=" << (int) rlet1 <<" \n";
  if(!is)  return __LINE__;  
  is >> rlet2;
  if(!is)  return __LINE__;  
#endif
  //std::cerr << "Let 1=" << (int) let1 << " 2=" << (int) let2 << " RLet1=" << (int) rlet1 << " RLet2=" << (int) rlet2 << "\n";
  
  if(let1 != rlet1) return __LINE__;
  if(let2 != rlet2) return __LINE__;
  return 0;
}
#endif

int testBitStream() {
  //std::cerr << "testBitStream(), Called. \n";
  StrOStreamC ostr;
  int i;
  {
    BitOStreamC ob(ostr);
    ob.WriteBit(true);
    ob.WriteBit(false);
    ob.WriteUByte(0x56);
    ob.WriteUInt(0x12345678);
    ob.WriteBit(true);
    for(i = 1;i < 8;i++)
      ob.WriteUByte(i,i);
    for(i = 1;i < 32;i++)
      ob.WriteUInt(i,i);
    ob.Flush();
  }
  
  StrIStreamC istr(ostr.String());
  BitIStreamC bi(istr);
  if(!bi.ReadBit()) return __LINE__;
  if(bi.ReadBit()) return __LINE__;
  if(bi.ReadUByte() != 0x56) return __LINE__;
  if(bi.ReadUInt() != 0x12345678) return __LINE__;
  if(!bi.ReadBit()) return __LINE__;
  for(i = 1;i < 8;i++)
    if(bi.ReadUByte(i) != i) return __LINE__;
  for(i = 1;i < 32;i++)
    if(bi.ReadUInt(i) != (UIntT) i) return __LINE__;
  
  // Gordon's test.
  
  StrOStreamC ostr2;
  {
    BitOStreamC test(ostr2);
    test.WriteUInt(777);
    test.WriteUInt(9,5);
    for(int i = 0;i < 3;i++) {
      test.WriteBit(0);
      test.WriteBit(0);
      test.WriteBit(1);
      test.WriteBit(0);
      test.WriteBit(1);
      test.WriteBit(1);
    }
    test.WriteUByte(156);
    test.WriteUByte(4);
    test.WriteUInt(555,12);
    test.Flush();
  }
  {
    StrIStreamC istr2(ostr2.String());
    BitIStreamC in(istr2);
    if(in.ReadUInt() != 777) return __LINE__;
    if(in.ReadUInt(5) != 9) return __LINE__;
    
    for(int i = 0;i < 3;i++) {
      if(in.ReadBit() != 0) return __LINE__;
      if(in.ReadBit() != 0) return __LINE__;
      if(in.ReadBit() != 1) return __LINE__;
      if(in.ReadBit() != 0) return __LINE__;
      if(in.ReadBit() != 1) return __LINE__;
      if(in.ReadBit() != 1) return __LINE__;
    }
    if(in.ReadUByte() != 156) return __LINE__;
    if(in.ReadUByte() != 4) return __LINE__;
    if(in.ReadUInt(12) != 555) return __LINE__;
  }
  
  return 0;
}

SizeT writeCount = 0;
bool WriteMethod(const char *data,SizeT len) {
  //std::cerr << "Write:" << len << "\n";
  writeCount += len;
  return true;
}

SizeT ReadMethod(char *data,SizeT bufferSize) {
  RavlAssert(data != 0);
  const char *tmp = "Hello\n";
  UIntT nr = RavlN::Min(bufferSize,(SizeT) 6);
  memcpy(data,tmp,nr);
  return nr;
}

int testFuncStream() {
  //std::cerr << "testFuncStream(). \n";
  {
    FuncOStreamBufC oStrmBuf(RavlN::Trigger(&::WriteMethod,(const char *)0,0));
    std::ostream ostrm(&oStrmBuf);
    ostrm << "Hello.\n" << std::flush;
  }
  //std::cerr << "Data:" << writeCount << "\n";
  if(writeCount != 7) return __LINE__;
  
  {
    FuncIStreamBufC iStrmBuf(RavlN::Trigger(&::ReadMethod,(char *)0,0));
    std::istream istrmOfInifinitGreetings(&iStrmBuf);
    StringC str;
    istrmOfInifinitGreetings >> str;
    
    //std::cerr<< " Read:'" << str << "' \n";
  }
  return 0;
}

