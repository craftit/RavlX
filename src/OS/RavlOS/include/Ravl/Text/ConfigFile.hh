// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVL_CONFIGFILE_HEADER
#define RAVL_CONFIGFILE_HEADER 1
///////////////////////////////////////////////////////////////////////
//! file="Ravl/OS/Text/ConfigFile.hh"
//! userlevel=Advanced
//! author="Charles Galambos"
//! lib=RavlOS
//! rcsid="$Id$"
//! date="14/05/1998"
//! example=exConfigFile.cc
//! docentry="Ravl.API.OS.Configuration Files"

#include "Ravl/Hash.hh"
#include "Ravl/RCHandleV.hh"
#include "Ravl/StringList.hh"
#include "Ravl/Text/TextFragment.hh"
#include "Ravl/RCAbstract.hh"
#include "Ravl/HashIter.hh"
#include "Ravl/BinStream.hh"


namespace RavlN {

  class TextCursorC;
  class TextFragmentC;
  class TextFileC;
  class ConfigFileC;
  class XMLIStreamC;
  class XMLOStreamC;
  
  typedef HashIterC<StringC,StringC> ConfigFileIterVarC;
  //: Iterate variables.
  
  //! userlevel=Develop
  //: Config file body.
  
  class ConfigFileBodyC 
    : public RCBodyVC
  {
  public:
    ConfigFileBodyC();
    //: Default constructor.
    
    ConfigFileBodyC(TextFileC &af,const StringC &name);
    //: Sub-section. constructor.

    ConfigFileBodyC(XMLIStreamC &is);
    //: Read config from an XML stream.
    
    bool Load(StringC fn,bool doCheck = true);
    //: Load a def file.
    
    bool Load(TextFileC &af,bool doCheck = true);
    //: Read from a buffer.
    
    bool Load(XMLIStreamC &strm,bool doCheck = true);
    //: Read from XML stream.

    bool Load(BinIStreamC & strm) ; 
    //: Load from stream 

    bool Save(std::ostream &strm) const;
    //: Save config to text
    // Not supported.

    bool Save(XMLOStreamC &strm) const;
    //: Save config to XML stream.
    // Not supported.
    
    bool Save(BinOStreamC & strm) const ; 
    //: Save to Binary Stream
    
    void Empty(void) ; 
    //: Empty the configuration. 
    
    const StringC &Name() const { return name; }
    //: Get name of configuration.
    // In sections it will be the name of the current section,
    // otherwise it will be set to the name of the file.
    
    virtual bool CheckTag(StringC tag);
    //: Check if a tag is valid.
    // Returns false by default.
    
    StringC Value(const StringC &tag) { return tab[tag]; }
    //: Get value of tag.
    
    StringC Value(const StringC &tag) const { StringC ret; tab.Lookup(tag,ret); return ret; }
    //: Get value of tag.
    // Will return an empty string if value is not set.
    
    UIntT Order(const StringC &tag);
    //: Get the order no for tagged object.
    // This gets number of elements read before 'tag' in the
    // config file.
    
    StringC &operator[](const StringC &tag) { return tab[tag]; }
    //: Get value of tag.
    
    StringC operator[](const StringC &tag) const { StringC ret; tab.Lookup(tag,ret); return ret; }
    //: Get value of tag.
    
    bool IsDefined(const StringC &tag) { return !tab[tag].IsEmpty(); }
    //: Is tag defined ?
    
    ConfigFileC  Section(const StringC &tag);
    //: Look for sub-section.
    // If none found an invalid handle is returned.
    
    ConfigFileIterVarC IterVars()
    { return HashIterC<StringC,StringC>(tab); }
    //: Iterate variables.
    
    DListC<StringC> ListSections() const;
    //: Make a list of sections.
    
    void Dump(ostream &out,int depth = 0);
    //: Dump contents of file to 'out'.
    
  protected:
    void SetVar(const StringC &str,const StringC &val,const TextFragmentC &frag) { 
      tab[str] = val; 
      frags[str] = frag;
      order[str] = ordCount++;
    }
    //: Set a new variable.
    
    void AddVar(const StringC &tag,const StringC &data,const TextFragmentC &nf) {
      tab[tag] += data;
      if(!frags.IsElm(tag)) {
	frags[tag] = nf;
	order[tag] = ordCount++;
      }
    }
    //: Add text to a variable.
    
    void AddSection(const StringC &tag,ConfigFileC &cf,const TextFragmentC &nf);
    //: Add section.

    void AddSection(const StringC &tag,ConfigFileC &cf);
    //: Add section.
    
    StringC name; // Name of config. Used 
    HashC<StringC,StringC> tab;
    HashC<StringC,UIntT> order; // Order elements are found in.
    HashC<StringC,TextFragmentC> frags;
    HashC<StringC,RCAbstractC> sec; // Sub sections.
    UIntT ordCount;
    friend class ConfigFileC;
  };
  
  //! userlevel=Normal
  //: Config file.
  
  class ConfigFileC 
    : public RCHandleVC<ConfigFileBodyC>
  {
  public:
    ConfigFileC() 
    {}
    //: Default constructor.
    // Creates an invalid handle.
    
    explicit ConfigFileC(bool) 
      : RCHandleVC<ConfigFileBodyC>(*new ConfigFileBodyC())
    {}
    //: Constructor.
    
    ConfigFileC(const StringC &fn,bool doCheck = true)
      : RCHandleVC<ConfigFileBodyC>(*new ConfigFileBodyC())
    { Load(fn,doCheck); }
    //: Filename.
    
    ConfigFileC(const RCAbstractC &ah) 
      : RCHandleVC<ConfigFileBodyC>(ah)
    {}
    //: Abstract constructor.
    // Note: If ah is not a handle for a config file an
    // invalid handle will be created. As with the default
    // constructor this can be checked for with the IsValid()
    // method
    
    ConfigFileC(XMLIStreamC &is)
      : RCHandleVC<ConfigFileBodyC>(*new ConfigFileBodyC(is))
    {}
    //: Read config from an XML stream.
    
  protected:
    ConfigFileC(ConfigFileBodyC &bod) 
      : RCHandleVC<ConfigFileBodyC>(bod)
    {}
    //: Body constructor.
    
    ConfigFileC(TextFileC &af,const StringC &name)
      : RCHandleVC<ConfigFileBodyC>(*new ConfigFileBodyC(af,name))
    {}
    //: Sub-section. constructor.
    
    void Empty(void) 
    { Body().Empty() ; } 
    //: Empty the configuration 
    
    void SetVar(const StringC &str,const StringC &val,const TextFragmentC &frag)    
    { Body().SetVar(str,val,frag); }
    //: Set a new variable.
    
    void AddVar(const StringC &tag,const StringC &data,const TextFragmentC &nf)    
    { Body().AddVar(tag,data,nf); }
    //: Add text to a variable.
    
    void AddSection(const StringC &tag,ConfigFileC &cf,const TextFragmentC &nf)    
    { Body().AddSection(tag,cf,nf); }
    //: Add section.
    
    void AddSection(const StringC &tag,ConfigFileC &cf)    
    { Body().AddSection(tag,cf); }
    //: Add section.
    
  public:
    
    bool Load(const StringC &fn,bool doCheck = true) { 
      if(!IsValid())
	(*this) = ConfigFileC(true);
      return Body().Load(fn,doCheck); 
    }
    //: Load a def file.
    // If this is an invalid handle a new ConfigFileC will
    // be created. <p>
    // **** Derived classes MUST overload this function to
    // ensure the correct class is created. ****
    
    bool Load(TextFileC &af,bool doCheck = true)
    { return Body().Load(af,doCheck); }
    //: Read from an existing text buffer.
    
    bool Load(XMLIStreamC &strm,bool doCheck = true) {
      if(!IsValid())
	(*this) = ConfigFileC(true);
      return Body().Load(strm,doCheck); 
    }
    //: Read from XML stream.

    bool Load(BinIStreamC & strm) 
    { return Body().Load(strm) ; } 
    //: Load from a binary stream
    
    bool Save(BinOStreamC & strm) const 
    { return Body().Save(strm) ; } 
    //: Save to a binary stream
    
    bool Save(XMLOStreamC &strm) const
    { return Body().Save(strm); }
    //: Save config to XML stream.
    // Not supported

    bool Save(std::ostream &strm) const
    { return Body().Save(strm); }
    //: Save config to text stream.
    // Not supported

    const StringC &Name() const
    { return Body().Name(); }
    //: Get name of configuration.
    // In sections it will be the name of the current section,
    // otherwise it will be set to the name of the file.
    
    bool CheckTag(const StringC &tag)
    { return Body().CheckTag(tag); }
    //: Check if a tag is valid.
    
    StringC Value(const StringC &tag)
    { return Body().Value(tag); }
    //: Get value of tag.
    // Will return an empty string if value is not set.

    StringC Value(const StringC &tag) const
    { return Body().Value(tag); }
    //: Get value of tag.
    // Will return an empty string if value is not set.

    UIntT Order(const StringC &tag)
    { return Body().Order(tag); }
    //: Get the order no for tagged object.
    
    StringC &operator[](const StringC &tag) 
    { return Body().operator[](tag); }
    //:  Get value of tag.
    // Will return an empty string if value is not set.
    
    StringC operator[](const StringC &tag) const
    { return Body().operator[](tag); }
    //:  Get value of tag.
    // Will return an empty string if value is not set.
    
    bool IsDefined(StringC tag)
    { return Body().IsDefined(tag); }
    //: Is tag defined ?
    
    ConfigFileC  Section(const StringC &tag)
    { return Body().Section(tag); }
    //: Look for sub-section.
    // If none found an invalid handle is returned.
    
    ConfigFileIterVarC IterVars()
    { return Body().IterVars(); }
    //: Iterate variables.
    
    DListC<StringC> ListSections() const
    { return Body().ListSections(); }
    //: Make a list of sections.
    
    void Dump(ostream &out,int depth = 0)
    { Body().Dump(out,depth); }
    //: Dump contents of file to 'out' stream.
    
    friend class ConfigFileBodyC;
    
  };
  
}

#endif
