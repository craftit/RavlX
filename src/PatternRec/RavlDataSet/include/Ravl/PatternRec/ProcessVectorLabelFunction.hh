// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2002, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVL_PROCESSVECTORLABELFUNCTION_HEADER
#define RAVL_PROCESSVECTORLABELFUNCTION_HEADER 1
///////////////////////////////////////////////////////////
//! rcsid="$Id: ProcessVectorLabelFunction.hh 5240 2005-12-06 17:16:50Z kieron $"
//! author="Kieron Messer"
//! docentry="Ravl.API.Pattern Recognition.Data Set" 
//! date="8/12/2009"
//! lib=RavlPatternRec
//! file="Ravl/PatternRec/DataSet.hh"

#include "Ravl/DP/Process.hh"
#include "Ravl/PatternRec/Function.hh"
#include "Ravl/Tuple2.hh"

namespace RavlN {
  
  //! userlevel=Develop
  //: A process which applies a function to a vector
  
  class ProcessVectorLabelFunctionBodyC
    : public DPProcessBodyC<Tuple2C<VectorC, UIntT>, Tuple2C<VectorC, UIntT> >
  {
  public:
    ProcessVectorLabelFunctionBodyC(const FunctionC & function)
      : m_function(function)
    {}
    //: Constructor.

    ProcessVectorLabelFunctionBodyC(std::istream &in) 
      : DPProcessBodyC<Tuple2C<VectorC, UIntT>, Tuple2C<VectorC, UIntT> >(in)
    {
      in >> m_function;
    }
    //: Stream constructor.
    
    ProcessVectorLabelFunctionBodyC(BinIStreamC &in) 
      : DPProcessBodyC<Tuple2C<VectorC, UIntT>, Tuple2C<VectorC, UIntT> >(in)
    {
      in >> m_function;
    }
    //: Stream constructor.
    
    virtual bool Save(std::ostream &out) const
    {
      out << m_function;
      return DPProcessBodyC<Tuple2C<VectorC, UIntT>, Tuple2C<VectorC, UIntT> >::Save(out);
    }
    //: Save to std::ostream.
    
    virtual bool Save(BinOStreamC &out) const
    {
      out << m_function;
      return DPProcessBodyC<Tuple2C<VectorC, UIntT>, Tuple2C<VectorC, UIntT> >::Save(out);
    }
    //: Save to binary stream.  
    
    virtual Tuple2C<VectorC, UIntT> Apply(const Tuple2C<VectorC, UIntT> &d1) {
      VectorC vec = m_function.Apply(d1.Data1());
      return Tuple2C<VectorC, UIntT>(vec, d1.Data2());
    }
    //: Do operation.
    
  protected:
    FunctionC m_function;
    
  };

  //! userlevel=Normal
  //: Process which slices vector dependingin supplied index range.
  
  class ProcessVectorLabelFunctionC
    : public DPProcessC<Tuple2C<VectorC, UIntT>, Tuple2C<VectorC, UIntT> >
  {
  public:
    ProcessVectorLabelFunctionC(const FunctionC & function)
      : DPProcessC<Tuple2C<VectorC, UIntT>, Tuple2C<VectorC, UIntT> >(*new ProcessVectorLabelFunctionBodyC(function))
    {}
    //: Default constructor.
    
    ProcessVectorLabelFunctionC(std::istream &in)
      : DPProcessC<Tuple2C<VectorC, UIntT>, Tuple2C<VectorC, UIntT> >(*new ProcessVectorLabelFunctionBodyC(in))
    {}
    //: Stream constructor.
    
    ProcessVectorLabelFunctionC(BinIStreamC &in)
      : DPProcessC<Tuple2C<VectorC, UIntT>, Tuple2C<VectorC, UIntT> >(*new ProcessVectorLabelFunctionBodyC(in))
    {}
    //: Binary stream constructor.
    
  protected:
    ProcessVectorLabelFunctionC(ProcessVectorLabelFunctionBodyC & bod)
      : DPProcessC<Tuple2C<VectorC, UIntT>, Tuple2C<VectorC, UIntT> >(bod)
    {}
    //: Body constructor.
    
    
  };
  
  
}

#endif
