// This file is part of RAVL, Recognition And Vision Library
// Copyright (C) 2011, Charles Galambos
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVL_GENETIC_GENEPALETTE_HH
#define RAVL_GENETIC_GENEPALETTE_HH 1
//! lib=RavlGeneticOptimisation

#include "Ravl/RandomMersenneTwister.hh"
#include "Ravl/RandomGauss.hh"
#include "Ravl/Stack.hh"
#include "Ravl/RCHash.hh"
#include "Ravl/String.hh"
#include "Ravl/SmartPtr.hh"
#include "Ravl/Genetic/GeneType.hh"
#include "Ravl/RCWrap.hh"

namespace RavlN {
  class XMLFactoryContextC;
}

namespace RavlN { namespace GeneticN {

  class GeneTypeC;

  //! Set of gene proxy values.

  class GeneTypeProxyMapC
   : public RCBodyVC
  {
  public:
    //! Default constructor
    GeneTypeProxyMapC();

    //! factory constructor
    GeneTypeProxyMapC(const XMLFactoryContextC &factory);

    //! Load from binary stream.
    GeneTypeProxyMapC(BinIStreamC &strm);

    //! Load from text stream.
    GeneTypeProxyMapC(std::istream &strm);

    //! Copy proxy map.
    RavlN::RCBodyVC &Copy() const;

    //! Save to binary stream
    bool Save(BinOStreamC &strm) const;

    //! Save to text stream
    bool Save(std::ostream &strm) const;

    //! Add a new proxy to the map.
    void AddProxy(const StringC &value,const GeneTypeC &geneType);

    //! Add a new proxy to the map via reference.
    //! Used for loading from FactoryXML.
    void AddProxyRef(const StringC &value,const RavlN::SmartPtrC<GeneTypeC> &geneType);

    //! Lookup a value.
    bool Lookup(const StringC &key,SmartPtrC<const GeneTypeC> &val) const;

    //! Test if there is a key in the proxy map.
    bool HasProxy(const StringC &key) const;

    //! Get number of entries in map.
    size_t Size() const
    { return m_values.Size(); }

    //! Handle to set.
    typedef SmartPtrC<GeneTypeProxyMapC> RefT;
  protected:
    HashC<StringC,RavlN::SmartPtrC<const GeneTypeC> > m_values;
  };

  //! Holds information used when mutating, crossing or generating genes.

  class GenePaletteC
    : public RCBodyVC
  {
  public:
    //! Default constructor.
    //! Seed from the global random number source.
    GenePaletteC();

    //! Construct from a seed.
    GenePaletteC(UInt32T seed);

    //! factory constructor
    GenePaletteC(const XMLFactoryContextC &factory);

    //! Copy constructor
    //! This generates a new random seed based on the other palette,
    //! and so modifies it in the process.
    GenePaletteC(GenePaletteC &other);

    //! Copy constructor
    //! This seeds the random palette from the global random number source.
    GenePaletteC(const GenePaletteC &other);

    //! Load from binary stream.
    GenePaletteC(BinIStreamC &strm);

    //! Load from text stream.
    GenePaletteC(std::istream &strm);

    //! Make a copy of the palette.
    virtual RavlN::RCBodyVC &Copy() const;

    //! Save to binary stream
    virtual bool Save(BinOStreamC &strm) const;

    //! Save to text stream
    virtual bool Save(std::ostream &strm) const;

    //! Generate a random value between 0 and 1.
    RealT RandomDouble();

    //! Generate a random value between 0 and 1.
    RealT Random1()
    { return RandomDouble(); }

    //! Generate a random value between 0 and 1.
    float RandomFloat()
    { return RandomDouble(); }

    //! Generate a random integer.
    UInt32T RandomUInt32();

    //! Generate an integer with a Gaussian distribution.
    float RandomGauss();

    //! Add a new proxy to the map.
    void AddProxy(const StringC &value,const GeneTypeC &geneType);

    //! Push new proxy map on the stack.
    void PushProxyMap(const GeneTypeProxyMapC &newMap);

    //! Pop old map off the stack.
    void PopProxyMap();

    //! Access the current map
    const GeneTypeProxyMapC &ProxyMap() const
    { return *m_proxyMap.Top(); }

    template<typename DataT>
    void SetParameter(const StringC &name,const DataT &value)
    { m_parameters[name] = ToRCAbstract(value); }

    template<typename DataT>
    bool GetParameter(const StringC &name,DataT &value) const
    {
      const RCAbstractC *val = m_parameters.Lookup(name);
      if(val == 0)
        return false;
      FromRCAbstract(*val,value);
      return true;
    }

    //! Handle to palette.
    typedef SmartPtrC<GenePaletteC> RefT;
  protected:
    //! Generate a seed from the global random number source.
    void SeedFromGlobalRandomGenerator();

    RandomMersenneTwisterC m_random;
    RandomGaussC m_guass;
    StackC<GeneTypeProxyMapC::RefT > m_proxyMap;
    HashC<StringC,RCAbstractC> m_parameters; //!< Setup constants for optimisation
  };

}}

#endif
