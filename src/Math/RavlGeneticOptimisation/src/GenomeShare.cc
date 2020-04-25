// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2011, Charles Galambos
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//! lib=RavlGeneticOptimisation
//! author=Charles Galambos
//! docentry=Ravl.API.Math.Genetic.Optimisation

#include "Ravl/Genetic/GenomeShare.hh"
#include "Ravl/Genetic/Genome.hh"
#include "Ravl/Random.hh"
#include "Ravl/OS/SysLog.hh"
#include "Ravl/XMLFactoryRegister.hh"
#include "Ravl/TypeName.hh"
#include "Ravl/PointerManager.hh"
#include "Ravl/VirtualConstructor.hh"
#include "Ravl/HSet.hh"
#include "Ravl/Genetic/GeneFactory.hh"

#include <algorithm>

#define DODEBUG 0
#if DODEBUG
#define ONDEBUG(x) x
#else
#define ONDEBUG(x)
#endif

namespace RavlN { namespace GeneticN {

  static float g_maxStrength = 100.0;
  static float g_spaceSize = 100.0;

  //! Factory constructor
  GeneTypeClassShareC::GeneTypeClassShareC(const XMLFactoryContextC &factory)
   : GeneTypeClassC(factory),
     m_mustConnect(factory.AttributeBool("mustConnect",false)),
     m_maxStrength(factory.AttributeReal("maxStrength",g_maxStrength))
  {
  }

  //! Constructor
  GeneTypeClassShareC::GeneTypeClassShareC(const std::type_info &classShareType,bool mustConnect,float maxStrength)
   : GeneTypeClassC(classShareType),
     m_mustConnect(mustConnect),
     m_maxStrength(maxStrength)
  {
    if(m_maxStrength <= 0)
      m_maxStrength = g_maxStrength;
  }

  //! Load form a binary stream
  GeneTypeClassShareC::GeneTypeClassShareC(BinIStreamC &strm)
   : GeneTypeClassC(strm),
     m_mustConnect(false),
     m_maxStrength(g_maxStrength)
  {
    ByteT version = 0;
    strm >> version;
    if(version < 1 || version > 2)
      throw RavlN::ExceptionUnexpectedVersionInStreamC("GeneTypeClassShareC");
    strm >> m_mustConnect;
    if(version > 1)
      strm >> m_maxStrength;
  }

  //! Load form a binary stream
  GeneTypeClassShareC::GeneTypeClassShareC(std::istream &strm)
   : GeneTypeClassC(strm),
     m_mustConnect(false),
     m_maxStrength(0)
  {
    RavlAssertMsg(0,"not implemented");
  }

  //! Save to binary stream
  bool GeneTypeClassShareC::Save(BinOStreamC &strm) const
  {
    if(!GeneTypeClassC::Save(strm))
      return false;
    ByteT version = 2;
    strm << version ;
    strm << m_mustConnect << m_maxStrength;
    return true;
  }

  //! Save to binary stream
  bool GeneTypeClassShareC::Save(std::ostream &strm) const
  {
    RavlAssertMsg(0,"not implemented");
    return false;
  }

  //! Dump description in human readable form.
  void GeneTypeClassShareC::Dump(std::ostream &strm,UIntT indent) const {
    GeneTypeClassC::Dump(strm,indent);
    strm << " MustConnect=" << m_mustConnect;
  }


  //! Create randomise value
  void GeneTypeClassShareC::Random(GenePaletteC &palette,GeneC::RefT &newValue) const {
    if(!newValue.IsValid())
      newValue = new GeneClassShareC(*this);

    GeneTypeClassC::Random(palette,newValue);

    GeneClassShareC *gcs = dynamic_cast<GeneClassShareC *>(newValue.BodyPtr());
    RavlAssert(gcs != 0);
    TFVectorC<float,2> newPos;
    for(unsigned i = 0;i < 2;i++)
      newPos[i] = static_cast<float>(palette.Random1() * g_spaceSize);
    gcs->SetPosition(newPos);
    gcs->SetStrength(static_cast<float>(palette.Random1() * m_maxStrength));
  }

  //! Mutate a gene
  bool GeneTypeClassShareC::Mutate(GenePaletteC &palette,float fraction,bool mustChange,const GeneC &original,RavlN::SmartPtrC<GeneC> &newValue) const {
    if(fraction <= 0 && !mustChange) {
      newValue = &original;
      return false;
    }
    if(!newValue.IsValid()) {
      newValue = new GeneClassShareC(*this);
    }
    //RavlDebug("Mutating %s ",RavlN::TypeName(typeid(original)));
    GeneClassShareC &newNode = dynamic_cast<GeneClassShareC &>(*newValue);
#if DODEBUG
    if(dynamic_cast<const GeneClassShareC *>(&original) == 0) {
      RavlError("Original not a GeneClassShareC. Type:%s ",TypeName(typeid(original)));
      RavlAssert(0);
    }
#endif
    const GeneClassShareC &oldNode = dynamic_cast<const GeneClassShareC &>(original);
    if(fraction < 0.3) {
      // Wander
      float delta = static_cast<float>(palette.Random1() - 0.5) * m_maxStrength * fraction;
      float strength = oldNode.Strength() + delta;
      if(strength < 0 || strength > m_maxStrength)
        strength = oldNode.Strength() - delta;
      if(strength < 0) strength = 0;
      if(strength > m_maxStrength) strength = m_maxStrength;
      newNode.SetStrength(strength);

      TFVectorC<float,2> newPos;
      for(unsigned i = 0;i < 2;i++) {
        float delta = static_cast<float>(palette.Random1() - 0.5) * g_spaceSize * fraction;
        newPos[i] = oldNode.Position()[i] + delta;
        if(newPos[i] < 0 || newPos[i] > g_spaceSize)
          newPos[i] = oldNode.Position()[i] - delta;
        if(newPos[i] < 0)
          newPos[i] = 0;
        if(newPos[i] > g_spaceSize)
          newPos[i] = g_spaceSize;
      }
      newNode.SetPosition(newPos);
    } else {
      // Blend with random
      newNode.SetStrength(static_cast<float>(palette.Random1() * m_maxStrength * fraction + oldNode.Strength()  * (1.0-fraction)));
      TFVectorC<float,2> newPos;
      for(unsigned i = 0;i < 2;i++)
        newPos[i] = static_cast<float>(palette.Random1() * g_spaceSize * fraction) + oldNode.Position()[i] * (1.0 -fraction);
      newNode.SetPosition(newPos);
    }

    GeneTypeClassC::Mutate(palette,fraction,mustChange,original,newValue);

    return true;
  }

  //! Mutate a gene
  void GeneTypeClassShareC::Cross(GenePaletteC &palette,const GeneC &original1,const GeneC &original2,RavlN::SmartPtrC<GeneC> &newValue) const {
    if(!newValue.IsValid())
      newValue = new GeneClassShareC(*this);
    GeneTypeClassC::Cross(palette,original1,original2,newValue);
  }


  static bool CompareShardGenes(const GeneClassShareC *g1,const GeneClassShareC *g2) {
    return g1->Strength() > g2->Strength();
  }

  //! Update share information.
  void GeneTypeClassShareC::UpdateShare(GeneFactoryC &factory,std::vector<GeneClassShareC *> &shares) const
  {
    // Order instances from strongest to weakest
    std::sort(shares.begin(),shares.end(),CompareShardGenes);
    for(unsigned i = 0;i < shares.size();i++) {
      const RavlN::TFVectorC<float,2> &start = shares[i]->Position();
      float threshold = Sqr(shares[i]->Strength());
      float bestDist = Sqr(g_spaceSize)* 2;
      GeneClassShareC *fallBack = 0;
      bool found = false;
      ONDEBUG(RavlDebug(" %u Start:%s Strength:%f ",i,RavlN::StringOf(shares[i]->Position()).data(),shares[i]->Strength()));
      GeneC::RefT tmp;
      for(unsigned j = i+1;j < shares.size();j++) {
        if(factory.LookupOverride(*shares[j],tmp))
          continue;
        // Ignore if overridden already, it will have been done by a stronger share.
        float distance = start.SqrEuclidDistance(shares[j]->Position());
        ONDEBUG(RavlDebug(" i=%u j=%u from %zu  Distance:%f ",i,j,(size_t) shares.size(),RavlN::Sqrt(distance)));
        if(distance < bestDist || fallBack == 0) {
          fallBack = shares[j];
          bestDist = distance;
        }
        if(distance < threshold) {
          RavlAssert(!(factory.LookupOverride(*shares[i],tmp) && tmp.BodyPtr() == shares[j]));
          factory.InsertOverride(*shares[j],*shares[i]);
          ONDEBUG(RavlDebug("Set override. "));
          found = true;
        }
      }
      // If must connect, connect to closest node.
      if(!found && m_mustConnect && fallBack != 0) {
        ONDEBUG(RavlDebug("Using fallback connect. %u of %zu BestDist:%f ",i,shares.size(),Sqrt(bestDist)));
        RavlAssert(!(factory.LookupOverride(*shares[i],tmp) && tmp.BodyPtr() == fallBack));
        factory.InsertOverride(*fallBack,*shares[i]);
      }

      ONDEBUG(if(found) {RavlDebug("Got connection. %u of %zu BestDist:%f Strength:%f ",i,shares.size(),Sqrt(bestDist),Sqrt(threshold));});
    }
  }


  XMLFactoryRegisterConvertC<GeneTypeClassShareC,GeneTypeNodeC> g_registerGeneTypeClassShare("RavlN::GeneticN::GeneTypeClassShareC");
  RAVL_INITVIRTUALCONSTRUCTOR_NAMED(GeneTypeClassShareC,"RavlN::GeneticN::GeneTypeClassShareC");
  static RavlN::TypeNameC g_typePtrGeneTypeClassShare(typeid(GeneTypeClassShareC::RefT),"RavlN::SmartPtrC<RavlN::GeneticN::GeneTypeClassShareC>");

  // ---------------------------------------------------------------------

  //! Factory constructor
  GeneClassShareC::GeneClassShareC(const XMLFactoryContextC &factory)
   : GeneClassC(factory),
     m_strength(static_cast<float>(factory.AttributeReal("strength",20.0)))
  {
    RavlN::TFVectorC<float,2>  defaultValue;
    defaultValue[0] = 50.0;
    defaultValue[1] = 50.0;
    factory.Attribute("position",m_position, defaultValue);
    // FIXME:- Check its a ClassShare type.
  }

  GeneClassShareC::GeneClassShareC(const GeneTypeClassShareC &geneType)
  : GeneClassC(geneType),
    m_strength(1.0)
  {
    m_position[0] = 50.0;
    m_position[1] = 50.0;
  }

  GeneClassShareC::GeneClassShareC(const GeneTypeClassShareC &geneType,
                                   const RavlN::TFVectorC<float,2> &position,
                                   float strength)
   : GeneClassC(geneType),
     m_position(position),
     m_strength(strength)
  {}

  //! Load form a binary stream
  GeneClassShareC::GeneClassShareC(BinIStreamC &strm)
   : GeneClassC(strm),
     m_strength(0)
  {
    ByteT version = 0;
    strm >> version;
    if(version != 1)
      throw RavlN::ExceptionUnexpectedVersionInStreamC("GeneClassShareC");
    strm >> m_position >> m_strength;
  }

  //! Load form a binary stream
  GeneClassShareC::GeneClassShareC(std::istream &strm)
   : GeneClassC(strm),
     m_strength(0)
  {
    RavlAssertMsg(0,"not implemented");
  }

  //! Save to binary stream
  bool GeneClassShareC::Save(BinOStreamC &strm) const
  {
    if(!GeneClassC::Save(strm))
      return false;
    ByteT version = 1;
    strm << version;
    strm << m_position << m_strength;

    return true;
  }

  //! Save to binary stream
  bool GeneClassShareC::Save(std::ostream &strm) const
  {
    GeneClassC::Save(strm);
    strm << " Position=" <<  m_position << " Strength=" << m_strength << " ";
    return false;
  }

  //! Save to binary stream
  bool GeneClassShareC::Save(RavlN::XMLOStreamC &strm) const
  {
    GeneClassC::Save(strm);
    strm << XMLStartTag("Share") << XMLAttribute("position",m_position) << XMLAttribute("strength",m_strength) << XMLEndTag;
    return true;
  }

  //! Generate an instance of the ClassShare.
  void GeneClassShareC::Generate(const GeneFactoryC &context,RCWrapAbstractC &handle) const
  {
    GeneC::RefT override;
    if(context.LookupOverride(*this,override)) {
      ONDEBUG(RavlDebug("Shared class %s being overridden. ",Name().data()));
      // We don't want to build recursive structures, so check stack
      // for the overridden class before calling.
      if(!context.CheckStackFor(*override)) {
        override->Generate(context,handle);
        return ;
      }
    }
    RCAbstractC abstractHandle;
    if(context.Lookup((const void *) this,abstractHandle)) {
      ONDEBUG(RavlDebug("Found instance in context from %s ",Name().data()));
      handle = abstractHandle;
      return ;
    }
    GeneClassC::Generate(context,handle);
    context.Insert((const void *) this,handle.Abstract());
    ONDEBUG(RavlDebug("Putting instance into context from %s ",Name().data()));
  }

  //! Generate a hash value for the gene
  size_t GeneClassShareC::Hash() const {
    size_t ret = GeneClassC::Hash();
    // They have to be exactly the same for the moment.
    ret += StdHash(m_position[0]) + (StdHash(m_position[1]) << 2) + (StdHash(m_strength) << 3);
    return ret;
  }

  //! Test is value is effectively equal to this within tolerances specified in the type.
  bool GeneClassShareC::IsEffectivelyEqual(const GeneC &other) const {
    if(!GeneClassC::IsEffectivelyEqual(other))
      return false;
    const GeneClassShareC *gcs = dynamic_cast<const GeneClassShareC *>(&other);
    if(gcs == 0) return false;
    // They have to be exactly the same for the moment.
    if(gcs->m_position != m_position) return false;
    if(gcs->m_strength != m_strength) return false;
    return true;
  }


  XMLFactoryRegisterConvertC<GeneClassShareC,GeneNodeC> g_registerGeneClassShare("RavlN::GeneticN::GeneClassShareC");
  RAVL_INITVIRTUALCONSTRUCTOR_NAMED(GeneClassShareC,"RavlN::GeneticN::GeneClassShareC");
  static RavlN::TypeNameC g_typePtrGeneClassShare(typeid(GeneClassShareC::RefT),"RavlN::SmartPtrC<RavlN::GeneticN::GeneClassShareC>");

  // ---------------------------------------------------------------------


  size_t StdHash(const GeneTypeC *val)
  { return RavlN::StdHash((const void *)val); }

  size_t StdHash(GeneClassShareC *val)
  { return RavlN::StdHash((const void *)val); }

  class ListSharesC
   : public GeneVisitorC
  {
  public:
     // Examine a gene.
     virtual bool Examine(const GeneC &gene) {
       const GeneClassShareC *theShare = dynamic_cast<const GeneClassShareC *>(&gene);
       if(theShare == 0)
         return true;
       RavlN::HSetC<GeneClassShareC *> &geneList = m_shares[&theShare->Type()];
       geneList += const_cast<GeneClassShareC *>(theShare);
       return true;
     }

     RavlN::HashC<const GeneTypeC *,RavlN::HSetC<GeneClassShareC *> > &Shares()
     { return m_shares; }

  protected:
     RavlN::HashC<const GeneTypeC *,RavlN::HSetC<GeneClassShareC *> > m_shares;
  };

  //! Update shared information.
  void GenomeC::UpdateShares(GeneFactoryC &factory) const {
    ListSharesC theShares;
    m_genomeRoot->Visit(theShares);
    for(RavlN::HashIterC<const GeneTypeC *,RavlN::HSetC<GeneClassShareC *> > it(theShares.Shares());it;it++) {
      const GeneTypeClassShareC *gts = dynamic_cast<const GeneTypeClassShareC *>(it.Key());
      if(gts == 0) {
        RavlError("Type not a share!");
        RavlAssert(0);
        continue;
      }
      std::vector<GeneClassShareC *> classes;
      classes.reserve(it.Data().Size());
      for(HSetIterC<GeneClassShareC *> git(it.Data());git;git++)
        classes.push_back(*git);
      gts->UpdateShare(factory,classes);
    }
  }


  void LinkGenomeShare()
  {}

}}

