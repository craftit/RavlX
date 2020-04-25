// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2011, Charles Galambos
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVL_GENETIC_GeneTypeFloatExp_HH
#define RAVL_GENETIC_GeneTypeFloatExp_HH 1
//! lib=RavlGeneticOptimisation
//! author=Charles Galambos
//! docentry=Ravl.API.Math.Genetic.Optimisation

#include "Ravl/Genetic/GenomeConst.hh"

namespace RavlN { namespace GeneticN {


  //! Float value generated with a exponential distribution

  class GeneTypeFloatExpC
    : public GeneTypeFloatC
  {
  public:

    //! Factory constructor
    GeneTypeFloatExpC(const XMLFactoryContextC &factory);

    //! Constructor
    //! width is in standard deviations.
    GeneTypeFloatExpC(const std::string &name,float min,float max,float inc);

    //! Load form a binary stream
    GeneTypeFloatExpC(BinIStreamC &strm);

    //! Load form a binary stream
    GeneTypeFloatExpC(std::istream &strm);

    //! Save to binary stream
    virtual bool Save(BinOStreamC &strm) const;

    //! Save to binary stream
    virtual bool Save(std::ostream &strm) const;

    //! Generate a new value
    virtual void RandomValue(GenePaletteC &palette,float &value) const;

    //! Mutate a gene
    virtual bool Mutate(GenePaletteC &palette,float fraction,bool mustChange,const GeneC &original,RavlN::SmartPtrC<GeneC> &newValue) const;

    // Reference to this gene.
    typedef RavlN::SmartPtrC<GeneTypeFloatExpC> RefT;

    // Const reference to this gene.
    typedef RavlN::SmartPtrC<const GeneTypeFloatExpC> ConstRefT;

  protected:
  };
}}

#endif
