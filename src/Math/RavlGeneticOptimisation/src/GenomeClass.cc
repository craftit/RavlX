// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2011, Charles Galambos
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//! lib=RavlGeneticOptimisation
//! author=Charles Galambos
//! docentry=Ravl.API.Math.Genetic.Optimisation

#include "Ravl/Genetic/GenomeClass.hh"
#include "Ravl/Genetic/Genome.hh"
#include "Ravl/Genetic/GenePalette.hh"
#include "Ravl/Genetic/GeneFactory.hh"

#include "Ravl/Random.hh"
#include "Ravl/SysLog.hh"
#include "Ravl/XMLFactoryRegister.hh"
#include "Ravl/TypeName.hh"
#include "Ravl/PointerManager.hh"
#include "Ravl/VirtualConstructor.hh"

#define DODEBUG 0
#if DODEBUG
#define ONDEBUG(x) x
#else
#define ONDEBUG(x)
#endif

namespace RavlN { namespace GeneticN {

  // -----------------------------------------------------------------

  //! Factory constructor
  GeneTypeNodeC::GeneTypeNodeC(const XMLFactoryContextC &factory)
   : GeneTypeC(factory)
  {
    XMLFactoryContextC childContext;
    if(factory.ChildContext("Components",childContext)) {
      for(RavlN::DLIterC<XMLTreeC> it(childContext.Children());it;it++) {
        GeneTypeC::ConstRefT geneType;
        if(!childContext.UseChildComponent(it->Name(),geneType)) {
          SysLog(SYSLOG_ERR,"Failed to load child component %s, at %s ",it->Name().data(),childContext.Path().data());
          throw RavlN::ExceptionBadConfigC("Failed to load component");
        }
        AddComponent(it->Name().data(),*geneType);
      }
    }
    XMLFactoryContextC attributeContext;
    if(factory.ChildContext("Attributes",attributeContext)) {
      // Get all the string attributes.
      for(RavlN::HashIterC<StringC,StringC> it(attributeContext.INode().XMLNode().Attributes());it;it++) {
        attributeContext.UseAttribute(it.Key());
        SetAttribute(it.Key(),it.Data());
      }
      // Anything more complex too.
      // FIXME: I've not tested retrieving values. There maybe RCWrapAbstractC / RCAbstract conversion issues.

      for(RavlN::DLIterC<XMLTreeC> it(attributeContext.Children());it;it++) {
        RCWrapAbstractC data;
        if(factory.UseChildComponent(it->Name(),data))
          SetAttribute(it->Name(),data);
      }
    }
  }

  // Constructor
  GeneTypeNodeC::GeneTypeNodeC(const std::string &name)
   : GeneTypeC(name)
  {}

  GeneTypeNodeC::GeneTypeNodeC(BinIStreamC &strm)
   : GeneTypeC(strm)
  {
    ByteT version = 0;
    strm >> version;
    if(version != 1)
      throw RavlN::ExceptionUnexpectedVersionInStreamC("GeneTypeFloatC");
    UInt32T size;
    strm >> size;
    std::string name;
    ONDEBUG(RavlDebug("Loading node '%s' elements=%u ",m_name.data(),size));

    for(UInt32T i = 0;i < size;i++) {
      strm >> name;
      ONDEBUG(RavlDebug("Loading '%s' ",name.data()));
      GeneTypeC::ConstRefT theGene;
      strm >> ObjIO(theGene);
      ONDEBUG(RavlDebug("Loading '%s' -> %s ",name.data(),RavlN::TypeName(typeid(*theGene))));
      m_componentTypes.Insert(name,theGene);
    }
    ONDEBUG(RavlDebug("Loading '%s' done. ",m_name.data()));
  }

  //! Load form a binary stream
  GeneTypeNodeC::GeneTypeNodeC(std::istream &strm)
   : GeneTypeC(strm)
  {
    RavlAssertMsg(0,"not implemented");
  }

  //! Save to binary stream
  bool GeneTypeNodeC::Save(BinOStreamC &strm) const
  {
    if(!GeneTypeC::Save(strm))
      return false;
    ByteT version = 1;
    strm << version;
    UInt32T size = m_componentTypes.Size();
    strm << size;
    for(RavlN::HashIterC<std::string,GeneTypeC::ConstRefT> it(m_componentTypes);it;it++) {
      strm << it.Key();
      strm << ObjIO(it.Data());
      RavlAssert(size-- > 0);
    }
    RavlAssert(size == 0);
    return true;
  }

  //! Save to text stream
  bool GeneTypeNodeC::Save(std::ostream &strm) const
  {
    RavlAssertMsg(0,"not implemented");
    return false;
  }

  //! List names of fields.
  void GeneTypeNodeC::ListFields(RavlN::CollectionC<Tuple2C<StringC,GeneTypeC::ConstRefT> > &col) const {
    if(!col.IsValid())
      col = RavlN::CollectionC<Tuple2C<StringC,GeneTypeC::ConstRefT> >(m_componentTypes.Size());
    for(RavlN::HashIterC<std::string,GeneTypeC::ConstRefT> it(m_componentTypes);it;it++) {
      col.Append(Tuple2C<StringC,GeneTypeC::ConstRefT>(it.Key(),it.Data()));
    }
  }

  //! Add a new entry to the gene

  void GeneTypeNodeC::AddComponent(const std::string &name,const GeneTypeC &geneType)
  {
    if(m_componentTypes.Insert(name,&geneType)) {
      RavlError("Member '%s' exists already in '%s' ",name.data(),Name().data());
      throw RavlN::ExceptionOperationFailedC("Member of given name exists already. ");
    }
  }

  //! Lookup component
  bool GeneTypeNodeC::LookupComponent(const std::string &name,GeneTypeC::ConstRefT &geneType) const
  {
    return m_componentTypes.Lookup(name,geneType);
  }

  //! Create randomise value
  void GeneTypeNodeC::Random(GenePaletteC &palette,GeneC::RefT &newValue) const
  {
    RavlAssert(newValue.IsValid());
    GeneNodeC &newNode = dynamic_cast<GeneNodeC &>(*newValue);
    for(RavlN::HashIterC<std::string,GeneTypeC::ConstRefT> it(m_componentTypes);it;it++) {
      GeneC::RefT gene;
      it.Data()->Random(palette,gene);
      newNode.SetComponent(it.Key(),*gene);
    }
  }

  //! Mutate a gene
  bool GeneTypeNodeC::Mutate(GenePaletteC &palette,
                             float fraction,
                             bool mustChange,
                             const GeneC &original,
                             RavlN::SmartPtrC<GeneC> &newValue) const
  {
    RavlAssert(newValue.IsValid());
    GeneNodeC &newNode = dynamic_cast<GeneNodeC &>(*newValue);
    const GeneNodeC &oldNode = dynamic_cast<const GeneNodeC &>(original);
    bool ret = false;
    ONDEBUG(RavlDebug("Mutating %zu components in '%s'. faction %f  ",(size_t) m_componentTypes.Size().V(),Name().data(),fraction));
    int mustChangeIndex = -1;
    if(m_componentTypes.Size() == 0)
      return false;
    if(mustChange) {
      mustChangeIndex = palette.RandomUInt32() % m_componentTypes.Size();
    }
    int index = 0;
    for(RavlN::HashIterC<std::string,GeneTypeC::ConstRefT> it(m_componentTypes);it;it++,index++) {
      GeneC::ConstRefT gene;
      if(!oldNode.GetComponent(it.Key(),gene)) {
        RavlError("Failed to find component '%s' in '%s' GeneType=%s ",
            it.Key().data(),
            Name().c_str(),
            RavlN::TypeName(typeid(*this)));
        oldNode.Dump(RavlSysLog(RavlN::SYSLOG_ERR) << "Old node:\n");
        RavlError("Aborting.");
        throw RavlN::ExceptionOperationFailedC("No component");
      }
      if(palette.Random1() > fraction && mustChangeIndex != index) {
        newNode.SetComponent(it.Key(),*gene);
        continue;
      }
      RavlN::SmartPtrC<GeneC> newComp;
      if(it.Data()->Mutate(palette,fraction,mustChangeIndex == index,*gene,newComp))
        ret = true;
      newNode.SetComponent(it.Key(),*newComp);
    }
    return ret;
  }

  //! Mutate a gene
  void GeneTypeNodeC::Cross(GenePaletteC &palette,const GeneC &original1,const GeneC &original2,RavlN::SmartPtrC<GeneC> &newValue) const
  {
    GeneNodeC &newNode = dynamic_cast<GeneNodeC &>(*newValue);
    const GeneNodeC &oldNode1 = dynamic_cast<const GeneNodeC &>(original1);
    const GeneNodeC &oldNode2 = dynamic_cast<const GeneNodeC &>(original2);
    for(RavlN::HashIterC<std::string,GeneTypeC::ConstRefT> it(m_componentTypes);it;it++) {
      GeneC::ConstRefT gene1;
      if(!oldNode1.GetComponent(it.Key(),gene1)) {
        // This can happen went crossing with hand built genome's
        GeneC::RefT newGene;
        it.Data()->Random(palette,newGene);
        gene1 = newGene.BodyPtr();
      }
      GeneC::ConstRefT gene2;
      if(!oldNode2.GetComponent(it.Key(),gene2)) {
        // This can happen went crossing with hand built genome's
        GeneC::RefT newGene;
        it.Data()->Random(palette,newGene);
        gene2 = newGene.BodyPtr();
      }
      RavlAssert(gene1.IsValid());
      RavlAssert(gene2.IsValid());
      unsigned option = palette.RandomUInt32() % 8;
      switch(option)
      {
        default:
        case 6:
        case 7:
        {
          RavlN::SmartPtrC<GeneC> newComp;
          if(&gene1->Type() == &gene2->Type()) {
            gene1->Cross(palette,*gene2,newComp);
            RavlAssert(newComp.IsValid());
            newNode.SetComponent(it.Key(),*newComp);
          } else {
            if(option & 0x1)
              newNode.SetComponent(it.Key(),*gene1);
            else
              newNode.SetComponent(it.Key(),*gene2);
          }
        } break;
        case 0:
        case 4:
        case 2:
          newNode.SetComponent(it.Key(),*gene1);
          break;
        case 3:
        case 1:
        case 5:
          newNode.SetComponent(it.Key(),*gene2);
          break;
      }
    }
  }


  XMLFactoryRegisterConvertC<GeneTypeNodeC,GeneTypeC> g_registerGeneTypeNode("RavlN::GeneticN::GeneTypeNodeC");
  RAVL_INITVIRTUALCONSTRUCTOR_NAMED(GeneTypeNodeC,"RavlN::GeneticN::GeneTypeNodeC");
  static RavlN::TypeNameC g_typePtrGeneTypeNode(typeid(GeneTypeNodeC::RefT),"RavlN::SmartPtrC<RavlN::GeneticN::GeneTypeNodeC>");

  // -----------------------------------------------------------------

  GeneNodeC::GeneNodeC(const GeneTypeNodeC &geneType)
   : GeneC(geneType)
  {}

  //! Factory constructor
  GeneNodeC::GeneNodeC(const XMLFactoryContextC &factory)
   : GeneC(factory)
  {
    XMLFactoryContextC childContext;
    if(factory.ChildContext("Components",childContext)) {
      for(RavlN::DLIterC<XMLTreeC> it(childContext.Children());it;it++) {
        GeneC::ConstRefT gene;
        if(!childContext.UseChildComponent(it->Name(),gene)) {
          SysLog(SYSLOG_ERR,"Failed to load child component %s, at %s ",it->Name().data(),childContext.Path().data());
          throw RavlN::ExceptionBadConfigC("Failed to load component");
        }
        // FIXME:- Check type exists for field
        m_components.Insert(it->Name().data(),gene);
      }
    }
  }

  //! Load form a binary stream
  GeneNodeC::GeneNodeC(BinIStreamC &strm)
   : GeneC(strm)
  {
    ByteT version = 0;
    strm >> version;
    if(version != 1)
      throw RavlN::ExceptionUnexpectedVersionInStreamC("GeneC");
    UInt32T size;
    strm >> size;
    std::string name;
    for(UInt32T i = 0;i < size;i++) {
      strm >> name;
      GeneC::ConstRefT theGene;
      strm >> ObjIO(theGene);
      m_components.Insert(name,theGene);
    }
  }

  //! Load form a binary stream
  GeneNodeC::GeneNodeC(std::istream &strm)
   : GeneC(strm)
  {
    RavlAssertMsg(0,"not implemented");
  }

  //! Save to binary stream
  bool GeneNodeC::Save(BinOStreamC &strm) const
  {
    if(!GeneC::Save(strm))
      return false;
    ByteT version = 1;
    strm << version;
    UInt32T size = m_components.Size();
    strm << size;
    for(RavlN::HashIterC<std::string,GeneC::ConstRefT> it(m_components);it;it++) {
      strm << it.Key();
      strm << ObjIO(it.Data());
    }

    return true;
  }


  //! Save to binary stream
  bool GeneNodeC::Save(std::ostream &strm) const
  {
    strm << " Node: " << Name().data() << "\n";
    for(RavlN::HashIterC<std::string,GeneC::ConstRefT> it(m_components);it;it++) {
      strm << " " << it.Key() << " = " << it.Data()->Name() << "\n{\n";
      it.Data()->Save(strm);
      strm << "}\n";
    }
    return false;
  }

  //! Save to binary stream
  bool GeneNodeC::Save(RavlN::XMLOStreamC &strm) const
  {
    GeneC::Save(strm);
    for(RavlN::HashIterC<std::string,GeneC::ConstRefT> it(m_components);it;it++) {
      it.Data()->Save(strm);
      strm << XMLAttribute("cname",it.Key());
      strm << XMLIndentDown << XMLEndTag ;
    }
    return true;
  }

  //! Lookup a component.
  bool GeneNodeC::Lookup(const std::string &name, GeneC::ConstRefT &component) const {
    bool ret= m_components.Lookup(name,component);
    ONDEBUG(RavlDebug("Lookup %s.  Succeeded=%d ",name.data(),ret));
    return ret;
  }

  //! Add a new entry to the gene
  void GeneNodeC::AddComponent(const std::string &name,const GeneC &newEntry,const GeneTypeC &geneType)
  {
    RavlAssert(&newEntry != 0);
    RavlAssert(&geneType != 0);
    //! Add a new entry to the gene
    GeneTypeC::ConstRefT fieldType;
    if(!m_type->LookupComponent(name,fieldType)) {
      ONDEBUG(RavlDebug("Adding new field %s ",name.data()));
      m_type->AddComponent(name,geneType);
    }
    m_components.Insert(name,&newEntry);
  }

  //! Get Component.
  bool GeneNodeC::GetComponent(const std::string &name,GeneC::ConstRefT &gene) const
  { return m_components.Lookup(name,gene); }

  //! Set component
  void GeneNodeC::SetComponent(const std::string &name,const GeneC &newGene)
  { m_components.Insert(name,&newGene); }


  //! Visit all gene's in tree.
  void GeneNodeC::Visit(GeneVisitorC &visitor) const {
    GeneC::Visit(visitor);
    for(RavlN::HashIterC<std::string,GeneC::ConstRefT> it(m_components);it;it++) {
      RavlAssert(it.Data().IsValid());
      it.Data()->Visit(visitor);
    }
  }

  //! Generate a hash value for the gene
  size_t GeneNodeC::Hash() const
  {
    size_t value = RavlN::StdHash((void *) m_type.BodyPtr());
    for(RavlN::HashIterC<std::string,GeneC::ConstRefT> it(m_components);it;it++) {
      value += RavlN::StdHash(it.Key());
      value += it.Data().Hash();
    }
    return value;
  }

  //! Test is value is effectively equal to this within tolerances specified in the type.
  bool GeneNodeC::IsEffectivelyEqual(const GeneC &other) const
  {
    if(*m_type != other.Type())
      return false;
    const GeneNodeC* node = dynamic_cast<const GeneNodeC*>(&other);
    if(node == 0)
      return false;
    if(node->m_components.Size() != m_components.Size())
      return false;
    for(RavlN::HashIterC<std::string,GeneC::ConstRefT> it(m_components);it;it++) {
      const GeneC::ConstRefT *cref = node->m_components.Lookup(it.Key());
      if(cref == 0) return false;
      if(!(*cref)->IsEffectivelyEqual(*it.Data()))
        return false;
    }
    return true;
  }

  //! Dump description in human readable form.
  void GeneNodeC::Dump(std::ostream &strm,UIntT indent) const
  {
    GeneC::Dump(strm,indent);
    strm << "\n";
    for(RavlN::HashIterC<std::string,GeneC::ConstRefT> it(m_components);it;it++) {
      strm << RavlN::Indent(indent+2) << "'" << it.Key() << "' ->\n";
      it.Data()->Dump(strm,indent+4);
      strm << "\n";
    }
  }


  XMLFactoryRegisterConvertC<GeneNodeC,GeneC> g_registerGeneNode("RavlN::GeneticN::GeneNodeC");
  RAVL_INITVIRTUALCONSTRUCTOR_NAMED(GeneNodeC,"RavlN::GeneticN::GeneNodeC");
  static RavlN::TypeNameC g_typePtrGeneNode(typeid(GeneNodeC::RefT),"RavlN::SmartPtrC<RavlN::GeneticN::GeneNodeC>");

  // ---------------------------------------------------------------------

  //! Factory constructor
  GeneTypeClassC::GeneTypeClassC(const XMLFactoryContextC &factory,bool checkType)
   : GeneTypeNodeC(factory),
     m_typeName(factory.AttributeString("className","").data())
  {
    if(checkType) {
      m_typeInfo = &RTypeInfo(m_typeName.data());
      if(*m_typeInfo == typeid(void)) {
        RavlError("Type '%s' unknown ",m_typeName.data());
        throw RavlN::ExceptionBadConfigC("Unknown type");
      }
      if(!SystemTypeConverter().CanConvert(typeid(GeneFactoryC),*m_typeInfo)) {
        RavlError("Don't know how to create class '%s' ",m_typeName.data());
        throw RavlN::ExceptionBadConfigC("Asked to create unknown class. ");
      }
    }
  }

  //! Constructor
  GeneTypeClassC::GeneTypeClassC(const std::type_info &classType)
   : GeneTypeNodeC(RavlN::TypeName(classType)),
     m_typeName(RavlN::TypeName(classType)),
     m_typeInfo(&classType)
  {
  }

  //! Load form a binary stream
  GeneTypeClassC::GeneTypeClassC(BinIStreamC &strm)
   : GeneTypeNodeC(strm)
  {
    ByteT version = 0;
    strm >> version;
    if(version != 1)
      throw RavlN::ExceptionUnexpectedVersionInStreamC("GeneC");
    strm >> m_typeName;
    m_typeInfo = &RTypeInfo(m_typeName.data());
    if(*m_typeInfo == typeid(void)) {
      RavlError("Type '%s' unknown ",m_typeName.data());
      throw RavlN::ExceptionBadConfigC("Unknown type");
    }
  }

  //! Load form a binary stream
  GeneTypeClassC::GeneTypeClassC(std::istream &strm)
   : GeneTypeNodeC(strm),
     m_typeInfo(&typeid(void))
  {
    RavlAssertMsg(0,"not implemented");
  }

  //! Save to binary stream
  bool GeneTypeClassC::Save(BinOStreamC &strm) const
  {
    if(!GeneTypeNodeC::Save(strm))
      return false;
    ByteT version = 1;
    strm << version;
    strm << m_typeName;

    return true;
  }

  //! Save to binary stream
  bool GeneTypeClassC::Save(std::ostream &strm) const
  {
    RavlAssertMsg(0,"not implemented");
    return false;
  }

  //! Dump description in human readable form.
  void GeneTypeClassC::Dump(std::ostream &strm,UIntT indent) const {
    GeneTypeNodeC::Dump(strm,indent);
    strm << RavlN::Indent(indent) << "Typename=" << m_typeName;
  }

  //! Create randomise value
  void GeneTypeClassC::Random(GenePaletteC &palette,GeneC::RefT &newValue) const
  {
    if(!newValue.IsValid())
      newValue = new GeneClassC(*this);
    GeneTypeNodeC::Random(palette,newValue);
  }

  //! Mutate a gene
  bool GeneTypeClassC::Mutate(GenePaletteC &palette,
                              float fraction,
                              bool mustChange,
                              const GeneC &original,
                              RavlN::SmartPtrC<GeneC> &newValue) const
  {
    if(fraction <= 0 && !mustChange) {
      newValue = &original;
      return false;
    }
    if(!newValue.IsValid())
      newValue = new GeneClassC(*this);
    return GeneTypeNodeC::Mutate(palette,fraction,mustChange,original,newValue);
  }

  //! Mutate a gene
  void GeneTypeClassC::Cross(GenePaletteC &palette,
                             const GeneC &original1,
                             const GeneC &original2,
                             RavlN::SmartPtrC<GeneC> &newValue) const
  {
    if(!newValue.IsValid())
      newValue = new GeneClassC(*this);
    return GeneTypeNodeC::Cross(palette,original1,original2,newValue);
  }

  //! Method for generating the class.
  void GeneTypeClassC::Generate(const GeneFactoryC &childContext,RCWrapAbstractC &handle) const
  {
    RavlN::RCWrapC<GeneFactoryC> fgwrap(childContext);
    ONDEBUG(RavlDebug("Creating class '%s' ",TypeName().data()));
    handle = SystemTypeConverter().DoConversion(fgwrap.Abstract(),fgwrap.DataType(),TypeInfo());
    if(!handle.IsValid()) {
      RavlError("Failed to create class '%s' ",TypeName().data());
    }
    RavlAssert(handle.IsValid());
  }

  //! Generate a gene type for
  const GeneTypeC &CreateGeneType(const std::type_info &ti)
  { return *new GeneTypeClassC(ti); }

  XMLFactoryRegisterConvertC<GeneTypeClassC,GeneTypeNodeC> g_registerGeneTypeClass("RavlN::GeneticN::GeneTypeClassC");
  RAVL_INITVIRTUALCONSTRUCTOR_NAMED(GeneTypeClassC,"RavlN::GeneticN::GeneTypeClassC");
  static RavlN::TypeNameC g_typePtrGeneTypeClass(typeid(GeneTypeClassC::RefT),"RavlN::SmartPtrC<RavlN::GeneticN::GeneTypeClassC>");

  // ---------------------------------------------------------------------

  //! Factory constructor
  GeneClassC::GeneClassC(const XMLFactoryContextC &geneType)
   : GeneNodeC(geneType)
  {
    // FIXME:- Check its a class type.
  }

  GeneClassC::GeneClassC(const GeneTypeClassC &geneType)
   : GeneNodeC(geneType)
  {}

  //! Load form a binary stream
  GeneClassC::GeneClassC(BinIStreamC &strm)
   : GeneNodeC(strm)
  {
    ByteT version = 0;
    strm >> version;
    if(version != 1)
      throw RavlN::ExceptionUnexpectedVersionInStreamC("GeneClassC");
  }

  //! Load form a binary stream
  GeneClassC::GeneClassC(std::istream &strm)
   : GeneNodeC(strm)
  {
    RavlAssertMsg(0,"not implemented");
  }

  //! Save to binary stream
  bool GeneClassC::Save(BinOStreamC &strm) const
  {
    if(!GeneNodeC::Save(strm))
      return false;
    ByteT version = 1;
    strm << version;

    return true;
  }

  //! Save to binary stream
  bool GeneClassC::Save(std::ostream &strm) const
  {
    GeneNodeC::Save(strm);
    return true;
  }

  //! Save to binary stream
  bool GeneClassC::Save(RavlN::XMLOStreamC &strm) const
  {
    GeneNodeC::Save(strm);
    return true;
  }


  //! Generate an instance of the class.
  void GeneClassC::Generate(const GeneFactoryC &context,RCWrapAbstractC &handle) const
  {
    GeneFactoryC childContext(context,*this);
    ClassType().Generate(childContext,handle);
  }

  XMLFactoryRegisterConvertC<GeneClassC,GeneNodeC> g_registerGeneClass("RavlN::GeneticN::GeneClassC");
  RAVL_INITVIRTUALCONSTRUCTOR_NAMED(GeneClassC,"RavlN::GeneticN::GeneClassC");
  static RavlN::TypeNameC g_typePtrGeneClass(typeid(GeneClassC::RefT),"RavlN::SmartPtrC<RavlN::GeneticN::GeneClassC>");



}}
