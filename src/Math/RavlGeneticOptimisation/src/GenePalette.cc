// This file is part of RAVL, Recognition And Vision Library
// Copyright (C) 2011, Charles Galambos
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//! lib=RavlGeneticOptimisation

#include "Ravl/Genetic/GenePalette.hh"
#include "Ravl/Genetic/GeneType.hh"
#include "Ravl/XMLFactoryRegister.hh"
#include "Ravl/Random.hh"
#include "Ravl/TypeName.hh"
#include "Ravl/SysLog.hh"

#define DODEBUG	0
#if DODEBUG
#define ONDEBUG(x) x
#else
#define ONDEBUG(x)
#endif

namespace RavlN { namespace GeneticN {

  //! Default constructor
  GeneTypeProxyMapC::GeneTypeProxyMapC()
  {}

  //! factory constructor
  GeneTypeProxyMapC::GeneTypeProxyMapC(const XMLFactoryContextC &factory)
  {
    factory.UseComponentGroup("Map",*this,&GeneTypeProxyMapC::AddProxyRef);
  }

  //! Load from text stream.
  GeneTypeProxyMapC::GeneTypeProxyMapC(std::istream &strm)
   : RCBodyVC(strm)
  {
    RavlAssertMsg(0,"not implemented");
  }

  //! Copy proxy map.
  RavlN::RCBodyVC &GeneTypeProxyMapC::Copy() const {
    return *new GeneTypeProxyMapC(*this);
  }

  //! Save to binary stream
  bool GeneTypeProxyMapC::Save(BinOStreamC &strm) const
  {
    RCBodyVC::Save(strm);
    RavlAssertMsg(0,"not implemented");
    return true;
  }

  //! Save to text stream
  bool GeneTypeProxyMapC::Save(std::ostream &strm) const {
    RCBodyVC::Save(strm);
    RavlAssertMsg(0,"not implemented");
    return true;
  }

  //! Add a new proxy to the map.
  void GeneTypeProxyMapC::AddProxy(const StringC &value,const GeneTypeC &geneType)
  {
    ONDEBUG(RavlInfo("Adding proxy '%s' -> '%s' ",value.data(),RavlN::TypeName(typeid(geneType))));
    m_values[value] = &geneType;
  }

  //! Add a new proxy to the map via reference.
  //! Used for loading from FactoryXML.

  void GeneTypeProxyMapC::AddProxyRef(const StringC &value,const GeneTypeC::RefT &geneType)
  {
    ONDEBUG(RavlInfo("Adding proxy '%s' -> '%s' ",value.data(),RavlN::TypeName(typeid(*geneType))));
    m_values[value] = geneType;
  }

  //! Lookup a value.
  bool GeneTypeProxyMapC::Lookup(const StringC &key,SmartPtrC<const GeneTypeC> &val) const
  {
    return m_values.Lookup(key,val);
  }

  //! Test if there is a proxy map.

  bool GeneTypeProxyMapC::HasProxy(const StringC &key) const
  {
    return m_values.IsElm(key);
  }

  static XMLFactoryRegisterC<GeneTypeProxyMapC> g_registerGeneTypeProxyMap("RavlN::GeneticN::GeneTypeProxyMapC");

  // --------------------------------------------------------------------------------

  static GeneTypeProxyMapC::RefT g_emptyMap = new GeneTypeProxyMapC();

  //! factory constructor
  GenePaletteC::GenePaletteC(const XMLFactoryContextC &factory)
  {
    StringC randomSeed = factory.AttributeString("seed","global");
    if(randomSeed == "global") {
      SeedFromGlobalRandomGenerator();
    } else {
      m_random.Seed(randomSeed.chars(),randomSeed.Size());
    }
    GeneTypeProxyMapC::RefT proxyMap;
    if(factory.UseChildComponent("ProxyMap",proxyMap,true,typeid(GeneTypeProxyMapC))) {
      ONDEBUG(RavlInfo("Using proxymap with %u entries ",(unsigned) proxyMap->Size()));
      PushProxyMap(*proxyMap);
    } else {
      m_proxyMap.Push(*g_emptyMap);
    }
  }

  //! Default constructor.
  //! Seed from the global random number source.
  GenePaletteC::GenePaletteC()
  {
    SeedFromGlobalRandomGenerator();
    m_proxyMap.Push(*g_emptyMap);
  }

  //! Holds information used when mutating, crossing or generating genes.

  GenePaletteC::GenePaletteC(UInt32T seed)
   : m_random(seed)
  {
    m_proxyMap.Push(*g_emptyMap);
  }

  //! Copy constructor
  GenePaletteC::GenePaletteC(GenePaletteC &other)
   : m_proxyMap(other.m_proxyMap)
  {
    const unsigned keyLen = 8;
    UInt32T init_key[keyLen];
    for(unsigned i = 0;i < keyLen;i++)
      init_key[i] = other.m_random.UInt();
    m_random.Seed(init_key,keyLen);
  }

  //! Copy constructor
  GenePaletteC::GenePaletteC(const GenePaletteC &other)
   : m_proxyMap(other.m_proxyMap)
  {
    SeedFromGlobalRandomGenerator();
  }

  //! Load from binary stream.
  GenePaletteC::GenePaletteC(BinIStreamC &strm)
   : RCBodyVC(strm)
  {
    RavlAssertMsg(0,"not implemented");
  }

  //! Load from text stream.
  GenePaletteC::GenePaletteC(std::istream &strm)
   : RCBodyVC(strm)
  {
    RavlAssertMsg(0,"not implemented");
  }

  //! Generate a seed from the global random number source.
  void GenePaletteC::SeedFromGlobalRandomGenerator() {
    const unsigned keyLen = 8;
    UInt32T init_key[keyLen];
    for(unsigned i = 0;i < keyLen;i++)
      init_key[i] = (unsigned) RavlN::RandomInt();
    m_random.Seed(init_key,keyLen);
  }

  //! Make a copy of the palette.
  RavlN::RCBodyVC &GenePaletteC::Copy() const
  { return *new GenePaletteC(*this); }

  //! Save to binary stream
  bool GenePaletteC::Save(BinOStreamC &strm) const
  {
    RCBodyVC::Save(strm);
    RavlAssertMsg(0,"not implemented");
    return true;
  }

  //! Save to text stream
  bool GenePaletteC::Save(std::ostream &strm) const {
    RCBodyVC::Save(strm);
    RavlAssertMsg(0,"not implemented");
    return true;
  }


  //! Generate a random value between 0 and 1.
  RealT GenePaletteC::RandomDouble()
  {
    return m_random.Double();
  }

  //! Generate a random integer.
  UInt32T GenePaletteC::RandomUInt32()
  {
    return m_random.UInt();
  }

  //! Generate an integer with a gaussian distribution.
  float GenePaletteC::RandomGauss()
  {
    return m_guass.Generate(m_random);
  }

  //! Add a new proxy to the map.
  void GenePaletteC::AddProxy(const StringC &value,const GeneTypeC &geneType)
  {
    GeneTypeProxyMapC::RefT newProxy = new GeneTypeProxyMapC(*m_proxyMap.Top());
    newProxy->AddProxy(value,geneType);
    m_proxyMap.Push(newProxy);
  }

  //! Push new proxy map on the stack.
  void GenePaletteC::PushProxyMap(const GeneTypeProxyMapC &newMap)
  {
    m_proxyMap.Push(&newMap);
  }

  //! Pop old map off the stack.
  void GenePaletteC::PopProxyMap()
  {
    m_proxyMap.DelTop();
  }

  static XMLFactoryRegisterC<GenePaletteC> g_registerGenePalette("RavlN::GeneticN::GenePaletteC");

}}
