// This file is part of RAVL, Recognition And Vision Library
// Copyright (C) 2012, Charles Galambos
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVL_GENETIC_GENETYPECLASSDIRECTCALL_HH
#define RAVL_GENETIC_GENETYPECLASSDIRECTCALL_HH 1
//! lib=RavlGeneticOptimisation
//! author=Charles Galambos
//! docentry=Ravl.API.Math.Genetic.Optimisation

#include "Ravl/Genetic/GenomeClass.hh"
#include "Ravl/RCWrap.hh"
#include "Ravl/XMLFactory.hh"
#include "Ravl/Calls.hh"

namespace RavlN { namespace GeneticN {

  //! Generate a class via a function pointer

  class GeneTypeClassDirectCallC
  : public GeneTypeClassC
  {
  public:

    //! Factory constructor
    GeneTypeClassDirectCallC(const XMLFactoryContextC &factory);

    //! Constructor
    GeneTypeClassDirectCallC(const std::type_info &classType,
                             const CallFunc2C<const GeneFactoryC &,RCWrapAbstractC &> &generator);

    //! Load form a binary stream
    GeneTypeClassDirectCallC(BinIStreamC &strm);

    //! Load form a binary stream
    GeneTypeClassDirectCallC(std::istream &strm);

    //! Save to binary stream
    virtual bool Save(BinOStreamC &strm) const;

    //! Save to binary stream
    virtual bool Save(std::ostream &strm) const;

    //! Set function to use to generate
    void SetGenerator(const CallFunc2C<const GeneFactoryC &,RCWrapAbstractC &> &generator)
    { m_generator = generator; }

    //! Reference to this gene.
    typedef RavlN::SmartPtrC<GeneTypeClassDirectCallC > RefT;

    //! Const reference to this gene.
    typedef RavlN::SmartPtrC<const GeneTypeClassDirectCallC > ConstRefT;

  protected:
    //! Method for generating the class from a GeneFactoryC which has a
    //! current context of a 'GeneClassC'
    virtual void Generate(const GeneFactoryC &context,RCWrapAbstractC &handle) const;

    mutable CallFunc2C<const GeneFactoryC &,RCWrapAbstractC &> m_generator;
  };

}}
#endif
