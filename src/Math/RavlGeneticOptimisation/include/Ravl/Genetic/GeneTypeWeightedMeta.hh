// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2011, Charles Galambos
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVL_GENETIC_GENETYPEWEIGHTEDMETA_HH
#define RAVL_GENETIC_GENETYPEWEIGHTEDMETA_HH 1
//! lib=RavlGeneticOptimisation
//! author=Charles Galambos
//! docentry=Ravl.API.Math.Genetic.Optimisation

#include "Ravl/Genetic/GenomeMeta.hh"
#include <map>

namespace RavlN { namespace GeneticN {

  //! Gene type which is an enumeration of its possible types.
  //! Where each is given an equal chance of occurring.

  class GeneTypeWeightedMetaC
   : public GeneTypeMetaC
  {
  public:
    //! Factory constructor
    GeneTypeWeightedMetaC(const XMLFactoryContextC &factory);

    //! Constructor
    GeneTypeWeightedMetaC(const std::string &name,
                          const std::vector<GeneTypeC::ConstRefT> &types,
                          const std::vector<float> &weights);

    //! Constructor
    GeneTypeWeightedMetaC(const std::string &name,
                          const std::vector<GeneTypeC::ConstRefT> &types);

    //! Constructor
    GeneTypeWeightedMetaC(const std::string &name,
                          const GeneTypeC &type1,float weight1,
                          const GeneTypeC &type2,float weight2);

    //! Constructor
    GeneTypeWeightedMetaC(const std::string &name,
                          const GeneTypeC &type1,float weight1,
                          const GeneTypeC &type2,float weight2,
                          const GeneTypeC &type3,float weight3);

    //! Constructor
    GeneTypeWeightedMetaC(const std::string &name);

    //! Load form a binary stream
    GeneTypeWeightedMetaC(BinIStreamC &strm);

    //! Load form a binary stream
    GeneTypeWeightedMetaC(std::istream &strm);

    //! Save to binary stream
    virtual bool Save(BinOStreamC &strm) const;

    //! Save to binary stream
    virtual bool Save(std::ostream &strm) const;

    //! Create randomise value
    virtual void Random(GenePaletteC &palette,GeneC::RefT &newValue) const;

    //! Access list of types
    std::vector<float> &Weights()
    { return m_weights; }

    //! Access list of types
    const std::vector<float> &Weights() const
    { return m_weights; }

    //! Add type to list
    virtual void AddType(const GeneTypeC &geneType,float weight = -1.0);

    // Reference to this gene.
    typedef RavlN::SmartPtrC<GeneTypeWeightedMetaC> RefT;

    // Const reference to this gene.
    typedef RavlN::SmartPtrC<const GeneTypeWeightedMetaC> ConstRefT;

  protected:
    //! Generate map again
    void Remap();

    //! Entry in the timeline.
    typedef std::map<float,GeneTypeC::ConstRefT>::value_type EntryT;

    std::vector<float> m_weights;
    float m_totalWeights;
    std::map<float,GeneTypeC::ConstRefT> m_typeMap;
  };

}}

#endif
