// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2010, OmniPerception Ltd.
// This code may be redistributed under the terms of the MIT
// License. See http://www.opensource.org/licenses/mit-license.html
// file-header-ends-here
//////////////////////////////////////////////////////////////////
//! author = "Alex Kostin"
//! lib=RavlSerialIO

#ifndef VISUAL_CPP

#include "Ravl/OS/SerialDFormat.hh"
#include "Ravl/OS/SerialAbstractPort.hh"
#include "Ravl/TypeName.hh"
#include "Ravl/StringList.hh"
#include "Ravl/OS/SerialDirect.hh"

#define DPDEBUG 0
#if DPDEBUG
#define ONDEBUG(x) x
#else
#define ONDEBUG(x)
#endif

namespace RavlN
{
  
  FileFormatSerialDirectBodyC::FileFormatSerialDirectBodyC()
    : FileFormatBodyC("serialDirect", "direct serial port connection (@SP_DIRECT)")
  {}
  
  
  
  const type_info &FileFormatSerialDirectBodyC::ProbeLoad(IStreamC &in, const type_info &obj_type) const
  {
    ONDEBUG(cerr << "FileFormatSerialDirectBodyC::ProbeLoad not a SerialDirect input" << endl;)
    return typeid(void); 
  }
  
  
  const type_info &FileFormatSerialDirectBodyC::ProbeLoad(const StringC &filename, IStreamC &in, const type_info &obj_type) const
  {
    ONDEBUG(cerr << "FileFormatSerialDirectBodyC::ProbeLoad filename(" << filename << ") type(" << obj_type.name() << ")" << endl);
    // Look for the device symbol
    if(filename.length() == 0)
      return typeid(void);
    if(filename[0] != '@')
      return typeid(void);
    
    // Check it's a serial port device
    StringC deviceId = ExtractDevice(filename);
    if (deviceId != "SP_DIRECT")
      return typeid(void);
    
    return typeid(SmartPtrC<SerialAbstractC>);
  }
  
  
  
  const type_info &FileFormatSerialDirectBodyC::ProbeSave(const StringC &filename, const type_info &obj_type, bool forceFormat ) const
  {
    ONDEBUG(cerr << "FileFormatSerialDirectBodyC::ProbeSave is unsupported" << endl;)
    return typeid(void);   
  }
  
  DPIPortBaseC FileFormatSerialDirectBodyC::CreateInput(IStreamC &in, const type_info &obj_type) const
  { 
    ONDEBUG(cerr << "FileFormatSerialDirectBodyC::CreateInput(IStreamC) is unsupported" << endl);
    return DPIPortBaseC();
  }
  
  DPOPortBaseC FileFormatSerialDirectBodyC::CreateOutput(OStreamC &out, const type_info &obj_type) const
  {
    ONDEBUG(cerr << "FileFormatSerialDirectBodyC::CreateOutput(OStreamC) is unsupported" << endl);
    return DPOPortBaseC();  
  }
  
  DPIPortBaseC FileFormatSerialDirectBodyC::CreateInput(const StringC &filename, const type_info &obj_type) const
  {
    ONDEBUG(cerr << "FileFormatSerialDirectBodyC::CreateInput filename(" << filename << ") type(" << obj_type.name() << ")" << endl);

    if(filename.length() == 0)
      return DPIPortBaseC();
    if(filename[0] != '@')
      return DPIPortBaseC();
    
    StringC deviceId = ExtractDevice(filename);
    if (deviceId != "SP_DIRECT")
      return DPIPortBaseC();
    
    try {
      // Parse parameters
      StringC params = ExtractParams(filename);
      RavlN::DListC<StringC> paramList = RavlN::StringListC(params,";");
      RavlN::DLIterC<StringC> it(paramList);

      if(!it) {
        throw "Name of the port is not specified";
      }

      StringC portName = *it;
      it++;
      IntT speed = 9600;
      StringC permission = "RDWR";
      IntT stop_bits = 1;
      SerialAbstractC::ParityT parity = SerialAbstractC::SERIAL_PARITY_NONE;
      IntT data_bits = 8;
      for(;it; it++) {
        RavlN::DListC<StringC> pair = RavlN::StringListC(*it, "=");
        if(pair.Size() != 2)
          throw "wrong parameter option";

        if(pair.First().CaseCmp("speed") == 0) {
          speed = pair.Last().IntValue();
        } else if(pair.First().CaseCmp("perm") == 0) {
          permission = pair.Last();
        } else if(pair.First().CaseCmp("stop_bits") == 0) {
          stop_bits = pair.Last().IntValue();
        } else if(pair.First().CaseCmp("parity") == 0) {
          if(pair.Last().CaseCmp("ODD") == 0) {
            parity = SerialAbstractC::SERIAL_PARITY_ODD;
          } else if(pair.Last().CaseCmp("EVEN") == 0) {
            parity = SerialAbstractC::SERIAL_PARITY_EVEN;
          } else if(pair.Last().CaseCmp("SET") == 0) {
            parity = SerialAbstractC::SERIAL_PARITY_SET;
          } else if(pair.Last().CaseCmp("NONE") == 0) {
            parity = SerialAbstractC::SERIAL_PARITY_NONE;
          }
        } else if(pair.First().CaseCmp("data_bits") == 0) {
          data_bits = pair.Last().IntValue();
        }
      }

      SmartPtrC<SerialAbstractC> portPtr = new SerialDirectC(portName.data(), permission.data(), true);
      if(!portPtr->Setup(speed, speed, stop_bits, parity, data_bits))
        throw "Wrong parameters for port setup";

      SerialFormatAuxDPIPortC rv(portPtr);
      return rv;
      /*DListC<SmartPtrC<SerialAbstractC> > portList;
      portList.InsLast(portPtr);
      return portList;*/
    } catch(const char *msg) {
      std::cerr << "FileFormatSerialDirectBodyC::CreateInput:" << msg << "\n";
    }
    return DPIPortBaseC();
  }
  
  
  
  DPOPortBaseC FileFormatSerialDirectBodyC::CreateOutput(const StringC &filename, const type_info &obj_type) const
  {
    ONDEBUG(cerr << "FileFormatSerialDirectBodyC::CreateOutput(const StringC) unsupported" << endl);
    return DPOPortBaseC();  
  }
  
  
  const type_info &FileFormatSerialDirectBodyC::DefaultType() const
  {
    return typeid(SmartPtrC<SerialAbstractC>);
  }
  
  
  static FileFormatSerialDirectC Init;
  
  void InitSerialDirectFormat()
  {}
}
#endif
