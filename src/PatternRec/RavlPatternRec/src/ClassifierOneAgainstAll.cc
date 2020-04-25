// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2003, OmniPerception Ltd.
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//! rcsid="$Id: ClassifierOneAgainstAll.cc 4777 2005-01-18 14:28:18Z ees1wc $"
//! lib=RavlPatternRec
//! author="Kieron Messer"
//! file="Ravl/PatternRec/Classify/ClassifierOneAgainstAll.cc"

#include "Ravl/PatternRec/ClassifierOneAgainstAll.hh"
#include "Ravl/SArray1dIter3.hh"
#include "Ravl/VirtualConstructor.hh"
#include "Ravl/BinStream.hh"
#include "Ravl/DP/FileFormatStream.hh"
#include "Ravl/DP/FileFormatBinStream.hh"
#include "Ravl/TypeName.hh"
#include "Ravl/config.h"

#if RAVL_COMPILER_MIPSPRO 
#include "Ravl/VirtualConstructor.hh"
#pragma instantiate RavlN::ClassifierOneAgainstAllBodyC* RavlN::VCLoad(RavlN::BinIStreamC&,RavlN::ClassifierOneAgainstAllBodyC*)
#pragma instantiate RavlN::ClassifierOneAgainstAllBodyC* RavlN::VCLoad(std::istream &,RavlN::ClassifierOneAgainstAllBodyC*)
#endif 

namespace RavlN {

  //: Default constructor.
  
  ClassifierOneAgainstAllBodyC::ClassifierOneAgainstAllBodyC()
  {}
  
  //: Default constructor.
  
  ClassifierOneAgainstAllBodyC::ClassifierOneAgainstAllBodyC(const RCHashC<UIntT, ClassifierC> & classifiers)
    : m_classifiers(classifiers)
  {     
  }
  
  //: Load from stream.
  
  ClassifierOneAgainstAllBodyC::ClassifierOneAgainstAllBodyC(std::istream &strm) 
    : ClassifierBodyC(strm)
  { strm >> m_classifiers; }
  
  //: Load from binary stream.
  
  ClassifierOneAgainstAllBodyC::ClassifierOneAgainstAllBodyC(BinIStreamC &strm) 
    : ClassifierBodyC(strm)
  { strm >> m_classifiers; }
  
  //: Writes object to stream.
  
  bool ClassifierOneAgainstAllBodyC::Save (std::ostream &out) const {
    if(!ClassifierBodyC::Save(out))
      return false;
    out << ' ' << m_classifiers << ' ';
    return true;    
  }
  
  //: Writes object to binary stream.
  
  bool ClassifierOneAgainstAllBodyC::Save (BinOStreamC &out) const {
    if(!ClassifierBodyC::Save(out))
      return false;
    out << m_classifiers;
    return true;
  }
  
  //: Classify sample 
  UIntT ClassifierOneAgainstAllBodyC::Classify(const VectorC &data) const {

    // do some magic
    DListC<UIntT>zeroLabel;
    for(HashIterC<UIntT, ClassifierC>it(m_classifiers);it;it++) {
      UIntT label = it.Data().Classify(data); // this will either be a 0 or 1
      // OK we have recognised the single class
      if(label == 0) {
        zeroLabel.InsLast(it.Key());
      }      
    }

    // eek not quite sure what to do. we have not modelled this bit of space
    if(zeroLabel.Size() == 0) {
      return 9999;
    }

    // ok we think it is more than one class, so it is a bit dodgy
    if(zeroLabel.Size() > 1) {
      return 9999;
    }
    
    return zeroLabel.First(); // only one class was found so all is good
    
  }
  
  ///////////////////////////////////////////////////////////
  
  
  RAVL_INITVIRTUALCONSTRUCTOR_FULL(ClassifierOneAgainstAllBodyC,ClassifierOneAgainstAllC,ClassifierC);



}
