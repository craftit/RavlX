// This file is part of RAVL, Recognition And Vision Library
// Copyright (C) 2017, Charles Galambos
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVL_GENETIC_POPULATIONSTATE_HH
#define RAVL_GENETIC_POPULATIONSTATE_HH 1
//! lib=RavlGeneticOptimisation
//! author=Charles Galambos
//! docentry=Ravl.API.Math.Genetic.Optimisation

#include "Ravl/STL.hh"
#include "Ravl/SmartPtr.hh"
#include "Ravl/Genetic/Genome.hh"
#include "Ravl/Genetic/EvaluateFitness.hh"

namespace RavlN {
  class XMLFactoryContextC;
}

namespace RavlN { namespace GeneticN {

  //! State of optimiser

  class PopulationStateC
    : public RavlN::RCBodyVC
  {
  public:
    //! Default constructor
    PopulationStateC();

    //! Setup
    PopulationStateC(
        int gen,
        SArray1dC<RavlN::Tuple2C<float,GenomeC::RefT> > &seeds,
        SArray1dC<RavlN::Tuple2C<float,GenomeC::RefT> > &population
        );

    //! Load form a binary stream
    PopulationStateC(BinIStreamC &strm);

    //! Load form a binary stream
    PopulationStateC(std::istream &strm);

    //! Save to binary stream
    virtual bool Save(BinOStreamC &strm) const;

    //! Save to binary stream
    virtual bool Save(std::ostream &strm) const;

    //! Access current generation
    int Generation() const
    { return m_generation; }

    //! Access seeds
    const SArray1dC<RavlN::Tuple2C<float,GenomeC::RefT> > &Seeds() const
    { return m_seeds; }

    //! Access population
    const SArray1dC<RavlN::Tuple2C<float,GenomeC::RefT> > &Population() const
    { return m_population; }

    //! Set the fitness function
    void SetFitnessFunction(const EvaluateFitnessC::RefT &ff)
    { m_fitnessFunction = ff; }

    //! Access fitness function
    const EvaluateFitnessC::RefT &FitnessFunction() const
    { return m_fitnessFunction; }

    typedef RavlN::SmartPtrC<PopulationStateC> RefT;

  protected:
    int m_generation;
    SArray1dC<RavlN::Tuple2C<float,GenomeC::RefT> > m_seeds;
    SArray1dC<RavlN::Tuple2C<float,GenomeC::RefT> > m_population;
    EvaluateFitnessC::RefT m_fitnessFunction;
  };

}}

#endif
