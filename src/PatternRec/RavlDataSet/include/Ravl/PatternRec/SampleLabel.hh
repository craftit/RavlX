// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVL_SAMPLELABEL_HEADER
#define RAVL_SAMPLELABEL_HEADER 1
//! rcsid="$Id$"
//! author="Kieron Messer"
//! docentry="Ravl.API.Pattern Recognition.Data Set"
//! lib=RavlPatternRec
//! file="Ravl/PatternRec/DataSet/SampleLabel.hh"

#include "Ravl/PatternRec/SampleDiscrete.hh"
#include "Ravl/PatternRec/FieldInfo.hh"

namespace RavlN {
  class VectorC;

  //! userlevel=Normal
  //: Sample of labels.

  class SampleLabelC
     : public SampleDiscreteC<UIntT>
  {
    public:
      SampleLabelC(SizeT maxSize = 10, const FieldInfoC & fieldInfo = FieldInfoC("label"))
          : SampleDiscreteC<UIntT>(maxSize), m_fieldInfo(fieldInfo) {
      }
      //: Create a sample of data with a maximum size

      SampleLabelC(const SArray1dC<UIntT> & dat, const FieldInfoC & fieldInfo = FieldInfoC("label"))
          : SampleDiscreteC<UIntT>(dat), m_fieldInfo(fieldInfo) {
      }
      //: Create a sample of data from an array

      SampleLabelC(const SampleC<UIntT> &sample, const FieldInfoC & fieldInfo = FieldInfoC("label"))
          : SampleDiscreteC<UIntT>(sample), m_fieldInfo(fieldInfo) {
      }
      //: Construct from base class.

      SampleLabelC Copy (void) const
      { return SampleLabelC(static_cast<DArray1dC<UIntT> >(*this).Copy(),FieldInfo()); }
      //: Make a copy of this object

      UIntT MaxValue() const;
      //: Find the value of the largest label.

      SArray1dC<UIntT> LabelSums() const;
      //: Return the number of samples in each class

      SampleC<VectorC> SampleVector(RealT inClass = 1, RealT outClass = 0, IntT maxLabel = -1) const;
      //: Convert a sample of labels to vectors
      // Where the label index is set to 'inClass' and the rest to 'outClass'.

      const FieldInfoC & FieldInfo() const {
        return m_fieldInfo;
      }
      //: Access field info

      bool SetFieldInfo(const FieldInfoC & fieldInfo) {
        m_fieldInfo = fieldInfo;
        return true;
      }
      //: Set the field info

      bool SetClassName(UIntT label, const StringC & className);
      //: Map a label to a class name

      bool GetClassName(UIntT label, StringC & className) const;
      //: Get a class name

      const RCHashC<UIntT, StringC> & Label2ClassNames() const {
        return m_label2name;
      }
      //: Get the map of label to class names

    protected:
      FieldInfoC m_fieldInfo;
      //: Information about the data being held

      RCHashC<UIntT, StringC>m_label2name;
      //: Information on class

  };

}

#endif
