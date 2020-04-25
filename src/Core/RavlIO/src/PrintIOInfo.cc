// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2005-12, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//! lib=RavlIO

#include "Ravl/DP/PrintIOInfo.hh"
#include "Ravl/DP/FileFormatRegistry.hh"
#include "Ravl/DP/TypeConverter.hh"
#include "Ravl/DP/FileFormat.hh"
#include "Ravl/TypeName.hh"
#include "Ravl/MTLocks.hh"

namespace RavlN {
  
  static StringC MakePad(IntT strLen,IntT tabs) 
  {
    StringC ret;
    for(int i = tabs - strLen/8;i > 0;i--)
      ret += '\t';
    return ret;  
  }

  // List all know formats.

  static bool ListFmts_LessThanOrEqual(const FileFormatBaseC &f1,const FileFormatBaseC &f2) 
  {
    if(f1.Name() == f2.Name())
      return StringC(TypeName(f1.DefaultType())) <= StringC(TypeName(f2.DefaultType()));
    return f1.Name() <= f2.Name(); 
  }
  
  void PrintIOFormats(std::ostream &os) {
    os << "File Formats:\n";
    os << "Seq Pri\tFormat\t\tClass\t\t\t\tDescription\n";
    MTReadLockC readLock(0);
    DListC<FileFormatBaseC> fmts = SystemFileFormatRegistry().Formats().Copy();
    readLock.Unlock();
    fmts.MergeSort(ListFmts_LessThanOrEqual);
    for(DLIterC<FileFormatBaseC> it(fmts);
        it.IsElm();it.Next()) {
      StringC typeName = TypeName(it.Data().DefaultType());
      StringC auxInfo = StringC((int) it.Data().IsStream()) + "   " + StringC(it.Data().Priority());
      os << " " << auxInfo << 
        MakePad(auxInfo.length() + 1,1) << " " << it.Data().Name() << 
        MakePad(it.Data().Name().length()+1,2) << " " << typeName << 
        MakePad(typeName.length()+1,4) << " " << it.Data().Description() << "\n";
    }
  }

  // List all know conversions.
  
  void PrintIOConversions(std::ostream &os) {
    os << "Type conversions:\n";
    MTReadLockC cacheLock(5);
    for(GraphEdgeIterC<StringC,DPConverterBaseC> it(SystemTypeConverter().Graph());
        it.IsElm();it.Next()) {
      StringC name1 = TypeName(it.Data().ArgType(0)); 
      os << " " << name1 << MakePad(name1.Size()+1,4) << " -> " << TypeName(it.Data().Output()) << " (cost=" << it.Data().Cost() << ") \n";
    }
  }

  // List all know class types.

  void PrintIOClassTypes(std::ostream &os) {
    os << "Classes:\n";
    MTReadLockC cacheLock(0);
    for(HashIterC<const char *,DPTypeInfoC> it(DPTypeInfoBodyC::Types());it.IsElm();it.Next()) 
      os << " " << TypeName(it.Key()) << "\n";
  }

  
}

