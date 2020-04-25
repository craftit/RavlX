// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2011, Charles Galambos
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVL_GENETIC_GPVARIABLE_HH
#define RAVL_GENETIC_GPVARIABLE_HH 1
//! lib=RavlGeneticProgram
//! author=Charles Galambos
//! docentry=Ravl.API.Math.Genetic.Programming

#include "Ravl/SmartPtr.hh"
#include "Ravl/Genetic/GenomeShare.hh"
#include "Ravl/TypeName.hh"

namespace RavlN { namespace GeneticN {

  class GeneFactoryC;

  //! A variable in a program

  class GPVariableBaseC
   : public RCBodyVC
  {
  public:
    //! Factory constructor
    GPVariableBaseC(const XMLFactoryContextC &factory);

    //! Factory constructor
    GPVariableBaseC(const GeneFactoryC &factory);

    //! Constructor
    GPVariableBaseC();

    //! Has variable been set?
    bool IsSet() const
    { return m_isSet; }

    //! Handle to this class.
    typedef RavlN::SmartPtrC<GPVariableBaseC> RefT;

  protected:
    std::string m_name;
    bool m_isSet;
  };

  //! Template for variable containing arbitrary type.

  template<typename DataT>
  class GPVariableC
   : public GPVariableBaseC
  {
  public:
    //! Default constructor.
    GPVariableC()
    {}

    //! Factory constructor
    GPVariableC(const GeneFactoryC &factory)
     : GPVariableBaseC(factory)
    {}

    //! Set value.
    void Set(const DataT &val) {
      m_data = val;
      m_isSet = true;
    }

    //! Access data.
    DataT &Data()
    { return m_data; }

    //! Const access to data.
    const DataT &Data() const
    { return m_data; }

    //! Handle to this class.
    typedef RavlN::SmartPtrC<GPVariableC<DataT> > RefT;

    //! Get the gene type.
    static GeneTypeC::RefT &GeneType() {
      static GeneTypeC::RefT x = new GeneTypeClassShareC(typeid(typename GPVariableC<DataT>::RefT),true);
      return x;
    }

  protected:
    DataT m_data;
  };

}}

#endif
