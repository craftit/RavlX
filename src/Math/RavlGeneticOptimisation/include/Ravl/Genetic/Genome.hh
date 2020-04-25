// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2011, Charles Galambos
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVL_GENETIC_GENOME_HH
#define RAVL_GENETIC_GENOME_HH 1
//! lib=RavlGeneticOptimisation
//! author=Charles Galambos
//! docentry=Ravl.API.Math.Genetic.Optimisation

#include "Ravl/STL.hh"
#include "Ravl/SmartPtr.hh"
#include "Ravl/BStack.hh"
#include "Ravl/Hash.hh"
#include "Ravl/DP/TypeConverter.hh"
#include "Ravl/OS/SysLog.hh"
#include "Ravl/TypeName.hh"
#include "Ravl/Genetic/GeneType.hh"
#include "Ravl/Genetic/GenePalette.hh"

namespace RavlN {
  class XMLFactoryContextC;
}

namespace RavlN { namespace GeneticN {

  //! Static part of the genome for an entity.

  class GenomeC
  : public RavlN::RCBodyVC
  {
  public:
    //! Default constructor
    GenomeC();

    //! Factory constructor
    GenomeC(const XMLFactoryContextC &factory);

    //! Construct a genome from a root gene.
    GenomeC(const GeneC &rootGene);

    //! Load form a binary stream
    GenomeC(BinIStreamC &strm);

    //! Load form a binary stream
    GenomeC(std::istream &strm);

    //! Save to binary stream
    virtual bool Save(BinOStreamC &strm) const;

    //! Save to binary stream
    virtual bool Save(std::ostream &strm) const;

    //! Save to binary stream
    virtual bool Save(RavlN::XMLOStreamC &strm) const;

    //! Set const.
    void SetConst(bool asConst)
    { m_const = asConst; }

    //! Test if genome is const.
    bool IsConst() const
    { return m_const; }

    //! Access the root gene.
    const GeneC &RootGene() const
    { return *m_genomeRoot; }

    //! Access number of gene's in the genome.
    size_t Size() const;

    //! Handle to genome
    typedef RavlN::SmartPtrC<GenomeC> RefT;

    //! Mutate this genome.
    //! Returns true if something changed.
    bool Mutate(GenePaletteC &pallete,float fraction,GenomeC::RefT &newGenome) const;

    //! Cross this genome with another
    void Cross(GenePaletteC &pallete,const GenomeC &other,GenomeC::RefT &newGenome) const;

    //! Update shared information.
    void UpdateShares(GeneFactoryC &factory) const;

    //! Age.
    UIntT Age() const
    { return m_age; }

    UIntT Generation() const
    { return m_generation; }

    //! Set age.
    void SetAge(UIntT value)
    { m_age = value; }

    //! Set the generation
    void SetGeneration(UIntT value)
    { m_generation = value; }

    //! Generate a hash value for the genome
    size_t Hash() const;

    //! Test if the genome is effective the same as another.
    bool IsEffectivelyEqual(const GenomeC &genome);

    //! Update running average score, return the latest value.
    float UpdateScore(float newScore,UIntT maxAge);

    //! Dump description in human readable form.
    void Dump(std::ostream &strm,UIntT indent = 0) const;

  protected:

    bool m_const;
    GeneC::RefT m_genomeRoot;
    UIntT m_age;
    UIntT m_generation;

    float m_averageScore;
    UIntT m_averageCount;
  };

  //! This actually tests if they are effectively equal which allows for some small
  //! user defined differences in some floating point numbers. This allows hash tables
  // to be created of similar genes.
  bool operator==(const GenomeC::RefT &g1,const GenomeC::RefT &g2);

}}

#endif
