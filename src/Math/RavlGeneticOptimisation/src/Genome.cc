// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2011, Charles Galambos
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//! lib=RavlGeneticOptimisation
//! author=Charles Galambos
//! docentry=Ravl.API.Math.Genetic.Optimisation

#include "Ravl/Genetic/Genome.hh"
#include "Ravl/Genetic/GeneFactory.hh"
#include "Ravl/Genetic/GenomeConst.hh"
#include "Ravl/Random.hh"
#include "Ravl/OS/SysLog.hh"
#include "Ravl/BListIter.hh"
#include "Ravl/XMLFactoryRegister.hh"
#include "Ravl/TypeName.hh"
#include "Ravl/PointerManager.hh"
#include "Ravl/VirtualConstructor.hh"
#include "Ravl/DP/FileFormatBinStream.hh"

#define DODEBUG 0
#if DODEBUG
#define ONDEBUG(x) x
#else
#define ONDEBUG(x)
#endif

namespace RavlN { namespace GeneticN {

  //! Default constructor

  GenomeC::GenomeC()
  : m_const(false),
    m_age(0),
    m_generation(0),
    m_averageScore(0),
    m_averageCount(0)
  {}

  GenomeC::GenomeC(const GeneC &rootGene)
   : m_const(false),
     m_genomeRoot(&rootGene),
     m_age(0),
     m_generation(0),
     m_averageScore(0),
     m_averageCount(0)
  {
  }

  //! Factory constructor
  GenomeC::GenomeC(const XMLFactoryContextC &factory)
   : m_const(factory.AttributeBool("const",false)),
     m_age(0),
     m_generation(0),
     m_averageScore(0),
     m_averageCount(0)
  {
    factory.UseComponent("Gene",m_genomeRoot);
  }


  //! Load form a binary stream
  GenomeC::GenomeC(BinIStreamC &strm)
   : RCBodyVC(strm),
     m_const(false),
     m_age(0),
     m_generation(0),
     m_averageScore(0),
     m_averageCount(0)
  {
    ByteT version = 0;
    strm >> version;
    if(version != 1)
      throw RavlN::ExceptionUnexpectedVersionInStreamC("GenomeC");
    strm >> ObjIO(m_genomeRoot) >> m_age >> m_generation >> m_averageScore >> m_averageCount;
  }

  //! Load form a binary stream
  GenomeC::GenomeC(std::istream &strm)
   : RCBodyVC(strm),
     m_const(false),
     m_age(0),
     m_generation(0),
     m_averageScore(0),
     m_averageCount(0)
  {
    RavlAssertMsg(0,"not implemented");
  }

  //! Save to binary stream
  bool GenomeC::Save(BinOStreamC &strm) const
  {
    if(!RCBodyVC::Save(strm))
      return false;
    ByteT version = 1;
    strm << version;
    strm << ObjIO(m_genomeRoot) << m_age << m_generation << m_averageScore << m_averageCount;
    return true;
  }


  //! Save to binary stream
  bool GenomeC::Save(std::ostream &strm) const
  {
    if(m_genomeRoot.IsValid())
      m_genomeRoot->Save(strm);
    return false;
  }

  //! Save to binary stream
  bool GenomeC::Save(RavlN::XMLOStreamC &strm) const {
    strm << RavlN::XMLStartTag("Genome") << XMLAttribute("typename",RavlN::TypeName(typeid(*this))) << XMLContent;
     //      << XMLIndent <<  "Some data... ";
    if(m_genomeRoot.IsValid()) {
      m_genomeRoot->Save(strm);
      strm << XMLIndentDown << XMLEndTag << " ";
    }
    strm << XMLIndentDown << XMLEndTag << " ";
    return true;
  }

  class GeneCounterC
   : public GeneVisitorC
  {
  public:
    GeneCounterC()
     : m_counter(0)
    {}

    // Examine a gene.
    virtual bool Examine(const GeneC &gene) {
      m_counter++;
      return true;
    }

    size_t Count() const
    { return m_counter; }

  protected:
    size_t m_counter;
  };

  //! Access number of gene's in the genome.
  size_t  GenomeC::Size() const {
    GeneCounterC counter;
    m_genomeRoot->Visit(counter);
    return counter.Count();
  }

  //! Mutate this genome.
  bool GenomeC::Mutate(GenePaletteC &pallete,float fraction,GenomeC::RefT &newGenome) const
  {
    GeneC::RefT newRootGene;
    bool ret = false;
    if(fraction <= 1e-6) {
      ret = m_genomeRoot->Mutate(pallete,fraction,true,newRootGene);
    } else {
      int tryNo = 5;
      float localFraction = fraction;
      while(!ret && tryNo-- > 0) {
        ret = m_genomeRoot->Mutate(pallete,localFraction,true,newRootGene);
        localFraction *= 1.5; // Increase rate bit and try again.
      }
#if 0
      if(tryNo < 18)
        RavlDebug("Trys %d ",tryNo);
#endif

#if 0
      if(!ret) {
        RavlWarning("Failed to mutate gene, mutation rate %f is set too low ?",fraction);
      }
#endif
    }
    newGenome = new GenomeC(*newRootGene);
    newGenome->SetAge(m_age+1);
    return ret;
  }

  //! Cross this genome with another
  void GenomeC::Cross(GenePaletteC &pallete,const GenomeC &other,GenomeC::RefT &newGenome) const
  {
    GeneC::RefT newRootGene;
    m_genomeRoot->Cross(pallete,other.RootGene(),newRootGene);
    newGenome = new GenomeC(*newRootGene);
    newGenome->SetAge(RavlN::Max(m_age,other.Age()));
  }

  //! Update running average score, return the latest value.
  float GenomeC::UpdateScore(float newScore,UIntT maxAge) {
    if(m_averageCount == 0) {
      m_averageCount = 1;
      m_averageScore = newScore;
      return newScore;
    }
    if(maxAge > 1000) {
      m_averageScore += newScore;
      return m_averageScore;
    }
    m_averageScore = (m_averageScore * static_cast<float>(m_averageCount) + newScore) / static_cast<float>(m_averageCount + 1);
    if(m_averageCount < maxAge)
      m_averageCount++;
    return m_averageScore;
  }

  //! Generate a hash value for the genome
  size_t GenomeC::Hash() const {
    if(!m_genomeRoot.IsValid())
      return 0;
    return m_genomeRoot->Hash();
  }

  //! Test if the genome is effective the same as another.

  bool GenomeC::IsEffectivelyEqual(const GenomeC &genome) {
    RavlAssert(m_genomeRoot.IsValid());
    return m_genomeRoot->IsEffectivelyEqual(RootGene());
  }

  //! Dump description in human readable form.
  void GenomeC::Dump(std::ostream &strm,UIntT indent) const
  {
    if(m_genomeRoot.IsValid()) {
      m_genomeRoot->Dump(strm,indent+1);
    } else {
      strm << "NIL";
    }
  }


  //! This actually tests if they are effectively equal which allows for some small
  //! user defined differences in some floating point numbers. This allows hash tables
  // to be created of similar genes.
  bool operator==(const GenomeC::RefT &g1,const GenomeC::RefT &g2) {
    if(g1.BodyPtr() == g2.BodyPtr())
      return true;
    if(!g1.IsValid() || !g2.IsValid())
      return false;
    return g1->RootGene().IsEffectivelyEqual(g2->RootGene());
  }

  RAVL_INITVIRTUALCONSTRUCTOR_NAMED(GenomeC,"RavlN::GeneticN::GenomeC");

  XMLFactoryRegisterC<GenomeC> g_registerGenome("RavlN::GeneticN::GenomeC");
  static RavlN::TypeNameC g_typePtrGenome(typeid(GenomeC::RefT),"RavlN::SmartPtrC<RavlN::GeneticN::GenomeC>");
  static FileFormatBinStreamC<RavlN::SmartPtrC<RavlN::GeneticN::GenomeC> > g_registerGenomeBinStream;



}}

