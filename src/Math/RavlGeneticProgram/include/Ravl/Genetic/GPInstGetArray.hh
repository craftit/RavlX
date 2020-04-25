// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2011, Charles Galambos
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVL_GENETIC_GPInstGetArray_HH
#define RAVL_GENETIC_GPInstGetArray_HH 1
//! lib=RavlGeneticProgram
//! author=Charles Galambos
//! docentry=Ravl.API.Math.Genetic.Programming

#include "Ravl/Genetic/GPInstruction.hh"
#include "Ravl/Genetic/GPVariable.hh"
#include "Ravl/Genetic/GenomeConst.hh"
#include "Ravl/Array1d.hh"
#include "Ravl/Genetic/GeneFactory.hh"

namespace RavlN { namespace GeneticN {

  //! Get a value from an array

  template<typename DataT>
  class GPInstGetArrayC
   : public GPInstructionC
  {
  public:
    //! Factory constructor
    GPInstGetArrayC(const GeneFactoryC &factory)
    {
      static GeneTypeIntC::RefT rangeSizeType = new GeneTypeIntC("arraySizes",2,100);
      factory.Get("index",m_index,*GPVariableC<IntT>::GeneType());
      factory.Get("array",m_array,*GPVariableC<RavlN::Array1dC<DataT> >::GeneType());
      factory.Get("result",m_result,*GPVariableC<DataT>::GeneType());
    }

    //! Constructor
    GPInstGetArrayC()
    {}

    //! Access gene type for instructions
    static GeneTypeC::RefT &GeneType() {
      static GeneTypeC::RefT x = new GeneTypeClassC(typeid(typename GPInstGetArrayC<DataT>::RefT));
      return x;
    }

    //! Execute instruction
    virtual bool Execute(GPExecutionContextC &context) {
      if(!m_index->IsSet())
        m_index->Set(0);
      if(!m_array->IsSet())
        return false;
      IndexC index = m_index->Data();
      RavlN::Array1dC<DataT> &array = m_array->Data();
      RavlAssert(!array.Range().IsEmpty());
      IndexC useIndex = array.Range().Clip(index);
      m_result->Set(array[useIndex]);
      return true;
    }

    //! Handle to this class.
    typedef RavlN::SmartPtrC<GPInstGetArrayC<DataT> > RefT;

  protected:
    typename GPVariableC<IntT>::RefT m_index;
    typename GPVariableC<RavlN::Array1dC<DataT> >::RefT m_array;
    typename GPVariableC<DataT>::RefT m_result;
  };


}}

#endif
