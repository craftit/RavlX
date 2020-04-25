// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2002, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVL_PROCESSVECTORFUNCTION_HEADER
#define RAVL_PROCESSVECTORFUNCTION_HEADER 1
///////////////////////////////////////////////////////////
//! rcsid="$Id: ProcessVectorFunction.hh 5240 2005-12-06 17:16:50Z kieron $"
//! author="Kieron Messer"
//! docentry="Ravl.API.Pattern Recognition.Data Set" 
//! date="8/12/2009"
//! lib=RavlPatternRec
//! file="Ravl/PatternRec/DataSet.hh"

#include "Ravl/DP/Process.hh"
#include "Ravl/PatternRec/Function.hh"

namespace RavlN {
  
  //! userlevel=Develop
  //: A process which applies a function to a vector
  
  class ProcessVectorFunctionBodyC
    : public DPProcessBodyC<VectorC, VectorC>
  {
  public:
    ProcessVectorFunctionBodyC(const FunctionC & function)
      : m_function(function)
    {}
    //: Constructor.

    ProcessVectorFunctionBodyC(std::istream &in) 
      : DPProcessBodyC<VectorC, VectorC>(in)
    {
      in >> m_function;
    }
    //: Stream constructor.
    
    ProcessVectorFunctionBodyC(BinIStreamC &in) 
      : DPProcessBodyC<VectorC, VectorC>(in)
    {
      in >> m_function;
    }
    //: Stream constructor.
    
    virtual bool Save(std::ostream &out) const
    {
      if(!DPProcessBodyC<VectorC, VectorC>::Save(out))
        return false;
      out << m_function;
      return true;
    }
    //: Save to std::ostream.
    
    virtual bool Save(BinOStreamC &out) const
    {
      if(!DPProcessBodyC<VectorC, VectorC>::Save(out))
        return false;
      out << m_function;
      return true;
    }
    //: Save to binary stream.  
    
    virtual VectorC Apply(const VectorC &d1) {
      return m_function.Apply(d1);
    }
    //: Do operation.
    
  protected:
    FunctionC m_function;
    
  };

  //! userlevel=Normal
  //: Process which slices vector dependingin supplied index range.
  
  class ProcessVectorFunctionC
    : public DPProcessC<VectorC, VectorC>
  {
  public:
    ProcessVectorFunctionC(const FunctionC & function)
      : DPProcessC<VectorC, VectorC>(*new ProcessVectorFunctionBodyC(function))
    {}
    //: Default constructor.
    
    ProcessVectorFunctionC(std::istream &in)
      : DPProcessC<VectorC, VectorC>(*new ProcessVectorFunctionBodyC(in))
    {}
    //: Stream constructor.
    
    ProcessVectorFunctionC(BinIStreamC &in)
      : DPProcessC<VectorC, VectorC>(*new ProcessVectorFunctionBodyC(in))
    {}
    //: Binary stream constructor.
    
  protected:
    ProcessVectorFunctionC(ProcessVectorFunctionBodyC & bod)
      : DPProcessC<VectorC, VectorC>(bod)
    {}
    //: Body constructor.
    
    
  };
  
  
}

#endif
