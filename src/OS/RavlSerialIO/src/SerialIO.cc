// This file is part of RAVL, Recognition And Vision Library
// Copyright (C) 2001-12, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
/////////////////////////////////////////////////////////////
//! rcsid="$Id$"
//! lib=RavlSerialIO
//! file="Ravl/OS/Misc/SerialIO.cc"


#include  "Ravl/OS/SerialIO.hh"
#include  "Ravl/SmartPtr.hh"
#include  "Ravl/DP/FileFormatIO.hh"

namespace RavlN {
#if RAVL_VISUALCPP_NAMESPACE_BUG
  using namespace RavlN;
#endif
  
  SerialCtrlC::SerialCtrlC()
  {}

  SerialCtrlC::SerialCtrlC(const char *dev, const char * perm,bool nonBlocking) {
    Open(dev,perm,nonBlocking);
  }
  
  //: Open device.
  bool SerialCtrlC::Open(const char *dev, const char * perm,bool nonBlocking) {
    StringC devFile;
    if(dev[0] == '@') { //new style of addressing
      devFile = dev;
    } else {
      devFile = StringC("@SP_DIRECT:") + dev;
    }
    //FIXME need to delete perm option from initial string,
    //but for now adding new perm option at the end should override anything which is already in dev
    devFile += StringC(";perm=") + perm;

    SmartPtrC<SerialAbstractC> port;
    if(!Load(devFile, port))
      return false;

    *this = SerialCtrlC(port.BodyPtr());

    return true;
  }
  
  
  //////// OSerialC ///////////////////////////////////////////////////////////////

  //: Open a output serial stream.
#if (!RAVL_OS_WIN32)
  OSerialC::OSerialC(const char *dev, bool buffered)
    : SerialCtrlC(dev, "WRONLY")
  {
    ((OStreamC &)(*this)) = OStreamC(Fd(),true,buffered);
  }
#endif
  
  //////// ISerialC ///////////////////////////////////////////////////////////////


  //: Open an input serial stream.
#if (!RAVL_OS_WIN32)
  ISerialC::ISerialC (const char *dev, bool buffered)
    : SerialCtrlC(dev, "RDONLY")
  {
    ((IStreamC &)(*this)) = IStreamC(Fd(),true,buffered);
  }
#endif
  //////////////////////////////////////////////////////////

  //: Open an input serial stream.
#if (!RAVL_OS_WIN32)
  IOSerialC::IOSerialC (const char *dev, bool buffered)
  : SerialCtrlC(dev, "RDWR")
  {
    ((IStreamC &)(*this)) = IStreamC(Fd(),true,buffered);
    ((OStreamC &)(*this)) = OStreamC(Fd(),true,buffered);
  }
#endif

  bool IOSerialC::Close()
  {
    ((IStreamC &)(*this)) = IStreamC();
    ((OStreamC &)(*this)) = OStreamC();
    SerialCtrlC::Close();
    return true;
  }


}


