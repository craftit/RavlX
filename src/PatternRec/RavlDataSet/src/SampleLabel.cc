// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//! lib=RavlPatternRec
//! file="Ravl/PatternRec/DataSet/SampleLabel.cc"
//! rcsid="$Id$"

#include "Ravl/PatternRec/SampleLabel.hh"
#include "Ravl/PatternRec/SampleIter.hh"
#include "Ravl/Vector.hh"
#include "Ravl/SysLog.hh"

namespace RavlN {

  //: Find the value of the largest label.
  
  UIntT SampleLabelC::MaxValue() const {
    UIntT lv = 0;
    for(DArray1dIterC<UIntT> it(*this);it;it++) 
      if(*it > lv)
	lv = *it;
    return lv;
  }

  //: Return the number of samples in each class
  SArray1dC<UIntT> SampleLabelC::LabelSums() const {
    UIntT noClasses = MaxValue()+1;
    SArray1dC<UIntT>arr(noClasses);
    arr.Fill(0);
    for(DArray1dIterC<UIntT> it(*this);it;it++) arr[*it]++;
    return arr;
  }
    

  //: Convert a sample of labels to vectors
  // Where the label index is set to 'inClass' and the rest to 'outClass'.
  
  SampleC<VectorC> SampleLabelC::SampleVector(RealT inClass, RealT outClass, IntT maxLabel) const
  {
    if (maxLabel < 0) {
      for (SampleIterC<UIntT> it(*this); it; it++) {
        if ((IntT)*it > maxLabel) {
          maxLabel = (UIntT) *it;
        }
      }
    }

    SampleC<VectorC> ret(Size());
    for (SampleIterC<UIntT> it(*this); it; it++) {
      VectorC vec(maxLabel + 1);
      vec.Fill(outClass);
      vec[*it] = inClass;
      ret.Append(vec);
    }
    return ret;

  }

  bool SampleLabelC::SetClassName(UIntT label, const StringC & className)
  {
    if(m_label2name.IsElm(label)) {
      m_label2name[label] = className;
      return true;
    }
    return m_label2name.Insert(label, className);
  }
  //: Map a label to a class name

  bool SampleLabelC::GetClassName(UIntT label, StringC & className) const
  {
    return m_label2name.Lookup(label, className);
  }
//: Get a class anem


  
}
