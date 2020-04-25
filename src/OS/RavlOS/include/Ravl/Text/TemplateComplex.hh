// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVL_TEMPLATECMPLX_HEADER
#define RAVL_TEMPLATECMPLX_HEADER 1
/////////////////////////////////////////////////////
//! file="Ravl/OS/Text/TemplateComplex.hh"
//! lib=RavlOS
//! docentry="Ravl.API.OS.Text Processing"
//! author="Charles Galambos"
//! date="23/02/2000"

#include "Ravl/Text/TemplateFile.hh"
#include "Ravl/RefCounter.hh"
#include "Ravl/CallMethodRefs.hh"
#include "Ravl/Stack.hh"
#include "Ravl/HSet.hh"
#include <vector>

namespace RavlN {

  class TemplateComplexC;

  //! userlevel=Develop
  //: Complex template body.
  
  class TemplateComplexBodyC 
    : public RCBodyVC
  {
  public:
    TemplateComplexBodyC(const TextFileC &templFile);
    //: Constructor.
    
    TemplateComplexBodyC(const StringC &templFilename);
    //: Constructor.

    TemplateComplexBodyC(const TextFileC &templFile,
                         const CallFunc2C<const StringC &,StringC &,bool> &lookupMethod);
    //: Constructor.

    void SetupPresets(const HSetC<StringC> &nameset)
    { presets = nameset; }
    //: Setup preset variables.
    
    bool Scan(StringC &templ);
    //: Scan template file for preset variables.
    // The key, and values are added to the vars list.
    
    bool Scan(TextFileC &templ);
    //: Scan template file for preset variables.
    // The key, and values are added to the vars list.
    
    bool Build(StringC &fn);
    //: Start build of document.
    // Build to file 'fn'.
    
    bool BuildToStream(OStreamC &out);
    //: Start build of document, write results to given stream.
    
    bool BuildSub(const TextFileC &templ);
    //: Start build of document.
    
    virtual bool Lookup(const StringC &varname,StringC &buff);
    //: Lookup variable.
    // if found put value into 'buff' and return true.
    // otherwise return false.
    
    void InitCommands();
    //: Setup default commands.
    
    StringC Interpret(const StringC &str);
    //: Interpret string 'str' and return the result.

    bool DoInterpret(StringC &txt);
    //: Interpret string.
    
    bool DoSet(StringC &txt);
    //: Set variable.
    
    bool DoFor(StringC &txt);
    //: Look over a list.
    
    bool DoIf(StringC &txt);
    //: If condition.
    
    bool DoIfSet(StringC &txt);
    //: If a variable is set.
    
    bool DoElse(StringC &txt);
    //: If a variable is set.
    
    bool DoInclude(StringC &txt);
    //: Include another template file.
    
    bool DoSub(StringC &txt);
    //: Make a sub context.
    
    bool DoSubCopy(StringC &txt);
    //: Make a sub context, copy old context.
    
    bool DoSubst(StringC &txt);
    //: Substution
    
    bool DoDefine(StringC &txt);
    //: Make a sub context, copy old context.
    
    bool DoMacro(StringC &txt,StringC &macro);
    //: Make a sub context, copy old context.
    
    bool DoUpperCase(StringC &txt);
    //: Make all text upper case.
    
    bool DoLowerCase(StringC &txt);
    //: Make all text lower case.
    
    bool DoSilent(StringC &txt);
    //: Make all text lower case.
    // Redirect output to /dev/null
    
    bool DoErrPrint(StringC &txt);
    //: Print a message to stderr.
    // Put the text on the stderr.
    
    bool DoBefore(StringC &txt);
    //: Return text before a string.
    
    bool DoAfter(StringC &txt);
    //: Return text after a string.
    
    bool DoFirstN(StringC &txt);
    //: Return the first N charactors of a string.
    
    bool DoMarkFirstN(StringC &txt);
    //: Return the first N charactors of a string, if the string is truncated indicate it with a marker.
    
    bool HtmlSafe(StringC &txt);
    //: Make sure text will be taken literally in html.
    
    StringC MakeHtml(const StringC &name);
    //: Make string suitable for use in plain html.

    StringC MakeHtml(const SubStringC &name);
    //: Make string suitable for use in plain html.
    
    void SetVar(const StringC &var,const StringC &val)
    { vars.Top()[var] = val; }
    //: Set value of a variable.
    
    StringC GetVar(const StringC &var) { 
      if(!vars.Top().IsElm(var))
	return StringC(); // Nothing.
      return vars.Top()[var]; 
    }
    //: Get value of a variable.
    
    template<class ObjT>
    void SetupCommand(const StringC &cmd,ObjT &obj,bool (ObjT::* func)(StringC &)) {
      commands[cmd] = TriggerR(obj,func,StringC());
      //commands[cmd] = CallMethod1C<ObjT &,StringC &,bool>(obj,func,StringC());
    }
    //: Setup new command.
    // NB. Obj is used as a reference, it is assumed to be a derived instance of this class.
    
    void SetupLookup(const CallFunc2C<const StringC &,StringC &,bool> &lookupMethod);
    //: Setup method to lookup variable values.

    OStreamC &Output()
    { return output.Top(); }
    //: Access current output stream.
    
    bool CheckCondition(const SubStringC &cond);
    //: Check if a condition is met.
    
  protected:
    void SetupCommand(const StringC &cmd,bool (TemplateComplexBodyC::* func)(StringC &arg));
    //: Setup command func.
    
    
    TextFileC templFile;
    StringC templateDir; // For includes.
    StackC<OStreamC> output;
    StackC<RCHashC<StringC,StringC> > vars;
    HSetC<StringC> presets;
    HashC<SubStringC,CallFunc1C<StringC &,bool> > commands;
    HashC<StringC,TextFileC> component; // Components.
    std::vector<CallFunc2C<const StringC &,StringC &,bool> > m_lookupMethods;
    bool verbose;
    IntT incDepth; // Include depth. 
    bool lastIf; // Did the last if succeed ? Used for else.
  };
  
  //! userlevel=Normal
  //: Complex template body.
  
  class TemplateComplexC 
    : public RCHandleC<TemplateComplexBodyC>
  {
  public:
    TemplateComplexC()
    {}
    //: Default constructor.
    // Creates an invalid handle.
    
    TemplateComplexC(const TextFileC &templFile)
      : RCHandleC<TemplateComplexBodyC>(*new TemplateComplexBodyC(templFile))
    {}
    //: Constructor.

    TemplateComplexC(const TextFileC &templFile,
                     const CallFunc2C<const StringC &,StringC &,bool> &lookupMethod)
      : RCHandleC<TemplateComplexBodyC>(*new TemplateComplexBodyC(templFile,lookupMethod))
    {}
    //: Constructor.

    TemplateComplexC(const StringC &templFile)
      : RCHandleC<TemplateComplexBodyC>(*new TemplateComplexBodyC(templFile))
    {}
    //: Constructor.
    
  protected:
    TemplateComplexC(TemplateComplexBodyC &bod)
      : RCHandleC<TemplateComplexBodyC>(bod)
    {}
    //: Body constructor.
    
    TemplateComplexBodyC &Body() 
    { return RCHandleC<TemplateComplexBodyC>::Body(); }
    //: Access body.
    
    const TemplateComplexBodyC &Body() const
    { return RCHandleC<TemplateComplexBodyC>::Body(); }
    //: Access body.
    
  public:
    
    bool Build(StringC &fn)
    { return Body().Build(fn); }
    //: Start build of document.
    
    bool BuildToStream(OStreamC &out)
    { return Body().BuildToStream(out); }
    //: Start build of document, write results to given stream.
    
    void SetupPresets(const HSetC<StringC> &nameset)
    { Body().SetupPresets(nameset); }
    //: Setup presets.
    
    void SetVar(const StringC &var,const StringC &val)
    { Body().SetVar(var,val); }
    //: Set value of variable.
    
    StringC GetVar(const StringC &var)
    { return Body().GetVar(var); }
    //: Get value of a variable.
    
    StringC Interpret(const StringC &str)
    { return Body().Interpret(str); }
    //: Interpret string 'str' and return the result.    
  };


}
#endif
