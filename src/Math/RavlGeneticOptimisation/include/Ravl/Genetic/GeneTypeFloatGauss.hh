// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2011, Charles Galambos
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVL_GENETIC_GeneTypeFloatGauss_HH
#define RAVL_GENETIC_GeneTypeFloatGauss_HH 1
//! lib=RavlGeneticOptimisation
//! author=Charles Galambos
//! docentry=Ravl.API.Math.Genetic.Optimisation

#include "Ravl/Genetic/GenomeConst.hh"

namespace RavlN { namespace GeneticN {


  //! Float value generated with a Gaussian distribution

  class GeneTypeFloatGaussC
    : public GeneTypeFloatC
  {
  public:
    enum FoldModeT { FoldNone,FoldUp,FoldDown };

    //! Factory constructor
    GeneTypeFloatGaussC(const XMLFactoryContextC &factory);

    //! Constructor
    //! width is in standard deviations.
    GeneTypeFloatGaussC(const std::string &name,float min,float max,float width,float offset,FoldModeT foldMode);

    //! Load form a binary stream
    GeneTypeFloatGaussC(BinIStreamC &strm);

    //! Load form a binary stream
    GeneTypeFloatGaussC(std::istream &strm);

    //! Save to binary stream
    virtual bool Save(BinOStreamC &strm) const;

    //! Save to binary stream
    virtual bool Save(std::ostream &strm) const;

    //! Generate a new value
    virtual void RandomValue(GenePaletteC &palette,float &value) const;

    //! Access width
    float Width() const
    { return m_width; }

    //! Access offset
    float Offset() const
    { return m_offset; }

    //! Access fold mode.
    FoldModeT FoldMode() const
    { return m_foldMode; }

    // Reference to this gene.
    typedef RavlN::SmartPtrC<GeneTypeFloatGaussC> RefT;

    // Const reference to this gene.
    typedef RavlN::SmartPtrC<const GeneTypeFloatGaussC> ConstRefT;

  protected:
    float m_width ;
    float m_offset;
    FoldModeT m_foldMode;
  };
}}

#endif
