// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2002, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//! rcsid="$Id$"
//! lib=RavlCore
//! file="Ravl/Core/System/VirtualConstructor.cc"

#include "Ravl/VirtualConstructor.hh"
#include "Ravl/TypeName.hh"
#include "Ravl/BinStream.hh"

#define DODEBUG 0
#if DODEBUG
#define ONDEBUG(x) x
#else
#define ONDEBUG(x)
#endif

namespace RavlN {

  static HashC<const char *,const char *> &AliasTable() {
    static HashC<const char *,const char *> tab;
    return tab;
  }

  //: Access virtual constructor table.
  
  static HashC<const char *,VirtualConstructorC> &Table() {
    static HashC<const char *,VirtualConstructorC> tab;
    return tab;
  }
  
  bool CreateVirtualConstructorAlias(const char *newName,const char *oldName) {
    AliasTable()[oldName] = newName;
    VirtualConstructorC *oldVC = Table().Lookup(oldName);
    if(oldVC == 0) {
      // Type being aliases hasn't been registered yet. We'll take care of it later.
      return false;
    }
    if(Table().Lookup(newName) != 0) {
      std::cerr << "Failed to construct virtual constructor alias '" << newName << "' for '" << oldName << "', name in use. \n";
      RavlAssert(0);
      return false;
    }
    Table()[newName] = *oldVC;
    return true;
  }

  //: Default constructor.
  
  VirtualConstructorBodyC::VirtualConstructorBodyC(const std::type_info &info,const char *typesname)
  {
    ONDEBUG(std::cerr << "VirtualConstructorBodyC::VirtualConstructorBodyC(), Registering '" << info.name() << "' as '" <<typesname << "' \n");
    VirtualConstructorC vc(*this);
    Table()[typesname] = vc;
    // Take care of any aliases.
    const char *aliasName = 0;
    if(AliasTable().Lookup(typesname,aliasName)) {
      if(Table().Lookup(aliasName) != 0) {
        std::cerr << "Failed to construct virtual constructor alias '" << aliasName << "' for '" << typesname << "', name in use. \n";
        RavlAssert(0);
      } else {
        Table()[aliasName] = vc;
      }
    }
    AddTypeName(info,typesname);
  }

  //: Construct from an std::istream.
  // Dummy method.
  
  VirtualConstructorBodyC::VirtualConstructorBodyC(std::istream &in)
    : RCBodyVC(in)
  {}
  
  //: Construct from an std::istream.
  // Dummy method
  
  VirtualConstructorBodyC::VirtualConstructorBodyC(BinIStreamC &in) 
    : RCBodyVC(in)
  {}
  
  //: Load an object of this type from an std::istream
  
  RCBodyVC *VirtualConstructorBodyC::Load(std::istream &in) const { 
    RavlAssertMsg(0,"VirtualConstructorBodyC::Load(), Abstract method called. ");
    return 0; 
  }
  
  //: Load an object of this type from a BinIStreamC
  
  RCBodyVC *VirtualConstructorBodyC::Load(BinIStreamC &in) const {
    RavlAssertMsg(0,"VirtualConstructorBodyC::Load(), Abstract method called. ");
    return 0; 
  }

  
  RCBodyVC *VCLoad(std::istream &s) {
    StringC name;
    s >> name;
    VirtualConstructorC vc;
    if(!Table().Lookup(name.chars(),vc)) {
      std::cerr << "WARNING: Failed to find virtual constructor for type '" << name.chars() << "' \n";
      RavlAssert(0);
      throw ExceptionOperationFailedC((StringC("Failed to find virtual constructor to decode stream for ") + name).data(), true);
    }
    return vc.Load(s);
  }
  
  RCBodyVC *VCLoad(BinIStreamC &s) {
    StringC name;
    s >> name;
    VirtualConstructorC vc;
    if(!Table().Lookup(name.chars(),vc)) {
      std::cerr << "WARNING: Failed to find virtual constructor for type '" << name.chars() << "' \n";
      RavlAssert(0);
      throw ExceptionOperationFailedC((StringC("Failed to find virtual constructor to decode stream for ") + name).data(), true);
    }
    return vc.Load(s);
  }
  
  // The following is included for debugging only.
  
  RAVL_INITVIRTUALCONSTRUCTOR(VirtualConstructorBodyC)
}
