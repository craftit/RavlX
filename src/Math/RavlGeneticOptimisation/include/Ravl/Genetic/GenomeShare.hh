// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2011, Charles Galambos
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVL_GENETIC_GENOMESHARE_HH
#define RAVL_GENETIC_GENOMESHARE_HH 1
//! lib=RavlGeneticOptimisation
//! author=Charles Galambos
//! docentry=Ravl.API.Math.Genetic.Optimisation

#include "Ravl/Genetic/GenomeClass.hh"

namespace RavlN { namespace GeneticN {

  class GeneClassShareC;

  //! Type for class that is shared from various points in the genome.

  class GeneTypeClassShareC
   : public GeneTypeClassC
  {
  public:
    //! Factory constructor
    GeneTypeClassShareC(const XMLFactoryContextC &factory);

    //! Constructor
    //! If mustConnect is true, then the resulting gene will connect to
    //! the best match regardless how distant it is.
    GeneTypeClassShareC(const std::type_info &classType, bool mustConnect = false,float maxStrength = -1);

    //! Load form a binary stream
    GeneTypeClassShareC(BinIStreamC &strm);

    //! Load form a binary stream
    GeneTypeClassShareC(std::istream &strm);

    //! Save to binary stream
    virtual bool Save(BinOStreamC &strm) const;

    //! Save to binary stream
    virtual bool Save(std::ostream &strm) const;

    //! Dump description in human readable form.
    virtual void Dump(std::ostream &strm,UIntT indent = 0) const;

    //! Create randomise value
    virtual void Random(GenePaletteC &palette,GeneC::RefT &newValue) const;

    //! Mutate a gene
    virtual bool Mutate(GenePaletteC &palette,float fraction,bool mustChange,const GeneC &original,RavlN::SmartPtrC<GeneC> &newValue) const;

    //! Mutate a gene
    virtual void Cross(GenePaletteC &palette,const GeneC &original1,const GeneC &original2,RavlN::SmartPtrC<GeneC> &newValue) const;

    //! Update share information.
    void UpdateShare(GeneFactoryC &factory,std::vector<GeneClassShareC *> &shares) const;

    //! Must connection be made ?
    bool IsMustConnect() const
    { return m_mustConnect; }

    //! Access maximum allowed strength.
    float MaxStrength() const
    { return m_maxStrength; }

    // Reference to this gene.
    typedef RavlN::SmartPtrC<GeneTypeClassShareC > RefT;

    // Const reference to this gene.
    typedef RavlN::SmartPtrC<const GeneTypeClassShareC > ConstRefT;

  protected:
    bool m_mustConnect;
    float m_maxStrength;
  };

  // Class that's shared amongst multiple points in the genome.

  class GeneClassShareC
   : public GeneClassC
  {
  public:
    //! Factory constructor
    GeneClassShareC(const XMLFactoryContextC &factory);

    //! Constructor.
    GeneClassShareC(const GeneTypeClassShareC &geneType,const RavlN::TFVectorC<float,2> &position,float strength = 1.0);

    //! Constructor.
    GeneClassShareC(const GeneTypeClassShareC &geneType);

    //! Load form a binary stream
    GeneClassShareC(BinIStreamC &strm);

    //! Load form a binary stream
    GeneClassShareC(std::istream &strm);

    //! Save to binary stream
    virtual bool Save(BinOStreamC &strm) const;

    //! Save to binary stream
    virtual bool Save(std::ostream &strm) const;

    //! Save to binary stream
    virtual bool Save(RavlN::XMLOStreamC &strm) const;

    //! Generate an instance of the class.
    virtual void Generate(const GeneFactoryC &context,RCWrapAbstractC &handle) const;

    //! Position in connection resolution space.
    const RavlN::TFVectorC<float,2> &Position() const
    { return m_position; }

    //! Position in connection resolution space.
    void SetPosition(const RavlN::TFVectorC<float,2> &newPos)
    { m_position = newPos; }

    //! Strength of node
    float Strength() const
    { return m_strength; }

    //! Set strength
    void SetStrength(float val)
    { m_strength = val; }

    //! Generate a hash value for the gene
    virtual size_t Hash() const;

    //! Test is value is effectively equal to this within tolerances specified in the type.
    virtual bool IsEffectivelyEqual(const GeneC &other) const;

    // Reference to this gene.
    typedef RavlN::SmartPtrC<GeneClassShareC> RefT;

    // Const reference to this gene.
    typedef RavlN::SmartPtrC<const GeneClassShareC> ConstRefT;

  protected:
    RavlN::TFVectorC<float,2> m_position;
    float m_strength;
  };



}}

#endif
