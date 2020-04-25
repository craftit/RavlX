// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
/////////////////////////////////////////////////////
//! rcsid="$Id$"
//! lib=RavlOS
//! file="Ravl/OS/Text/TemplateComplex.cc"

#include "Ravl/Text/TemplateComplex.hh"
#include "Ravl/StrStream.hh"
#include "Ravl/StringList.hh"
#include "Ravl/SubStringList.hh"
#include "Ravl/OS/Filename.hh"
#include "Ravl/OS/SysLog.hh"

#define DODEBUG 0
#if DODEBUG
#define ONDEBUG(x) x
#else
#define ONDEBUG(x)
#endif

namespace RavlN {
  
  //: Constructor.
  
  TemplateComplexBodyC::TemplateComplexBodyC(const TextFileC &ntemplFile)
    : templFile(ntemplFile)
  {
    templateDir = FilenameC(ntemplFile.Filename()).PathComponent();
    vars.Push(RCHashC<StringC,StringC>(true) ); // Push base variables.
    InitCommands();
  }

  TemplateComplexBodyC::TemplateComplexBodyC(const TextFileC &ntemplFile,
                                             const CallFunc2C<const StringC &,StringC &,bool> &lookupMethod)
    : templFile(ntemplFile)
  {
    m_lookupMethods.push_back(lookupMethod);
    templateDir = FilenameC(ntemplFile.Filename()).PathComponent();
    vars.Push(RCHashC<StringC,StringC>(true) ); // Push base variables.
    InitCommands();
  }

  //: Constructor.
  
  TemplateComplexBodyC::TemplateComplexBodyC(const StringC &fname)
    : templFile(fname)
  {
    templateDir = FilenameC(fname).PathComponent();
    vars.Push(RCHashC<StringC,StringC>(true) ); // Push base variables.
    InitCommands();
  }

  //: Start build of document, write results to given stream.
  
  bool TemplateComplexBodyC::BuildToStream(OStreamC &out) {
    output.Push(out);
    incDepth = 0;
    bool ret = BuildSub(templFile);
    output.DelTop();
    return ret;
  }
  
  //: Start build of document.
  
  bool TemplateComplexBodyC::Build(StringC &fn) {
    //if(verbose)
    ONDEBUG(RavlSysLog(SYSLOG_DEBUG) << "Building file:'" << fn <<"'");
    FilenameC dirComp = FilenameC(fn).PathComponent();
    SetVar("thisFilename",FilenameC(fn).NameComponent());
    if(dirComp != "") { // Extra directory specified ?
      if(!dirComp.IsDirectory()) {
	if(!dirComp.MakeDir()) {
	  RavlSysLog(SYSLOG_ERR) << "Failed to make directory '" << dirComp  <<"' for file '" << fn << "'";
	  return false;
	}
      }
    }
    OStreamC outs(fn);
    if(!outs.good()) {
      RavlSysLog(SYSLOG_ERR) << "Failed to open output file '" << fn << "'";
      return false;
    }
    return BuildToStream(outs);
  }
  
  //: Setup command func.
  
  void TemplateComplexBodyC::SetupCommand(const StringC &cmd,bool (TemplateComplexBodyC::* func)(StringC &arg)) {
    StringC empty;
    //commands[cmd] = CallMethod1C<TemplateComplexBodyC &,StringC &,bool>(*this,func,empty);
    commands[cmd] = TriggerR(*this,func,empty);
  }
  
  //: Setup default commands.
  
  void TemplateComplexBodyC::InitCommands() {
    commands["FilePattern"] = CallFunc1C<StringC &,bool>(false); // Not need here, ignore it.
    commands["FileObject"] = CallFunc1C<StringC &,bool>(false);  // Not need here, ignore it.
    commands["//"] = CallFunc1C<StringC &,bool>(false);          // Comment, ignore.
    SetupCommand("set",&TemplateComplexBodyC::DoSet);
    SetupCommand("for",&TemplateComplexBodyC::DoFor);
    SetupCommand("if",&TemplateComplexBodyC::DoIf);
    SetupCommand("ifset",&TemplateComplexBodyC::DoIfSet);
    SetupCommand("else",&TemplateComplexBodyC::DoElse);
    SetupCommand("include",&TemplateComplexBodyC::DoInclude);
    SetupCommand("sub",&TemplateComplexBodyC::DoSub);
    SetupCommand("subc",&TemplateComplexBodyC::DoSubCopy);
    SetupCommand("subst",&TemplateComplexBodyC::DoSubst);
    SetupCommand("define",&TemplateComplexBodyC::DoDefine);
    SetupCommand("htmlsafe",&TemplateComplexBodyC::HtmlSafe);
    SetupCommand("uppercase",&TemplateComplexBodyC::DoUpperCase);
    SetupCommand("lowercase",&TemplateComplexBodyC::DoLowerCase);
    SetupCommand("silent",&TemplateComplexBodyC::DoSilent);
    SetupCommand("print",&TemplateComplexBodyC::DoErrPrint);
    SetupCommand("before",&TemplateComplexBodyC::DoBefore);
    SetupCommand("after",&TemplateComplexBodyC::DoAfter);
    SetupCommand("truncate",&TemplateComplexBodyC::DoFirstN);
    SetupCommand("marktruncate",&TemplateComplexBodyC::DoMarkFirstN);
    SetupCommand("interpret",&TemplateComplexBodyC::DoInterpret);
  }
  
  //: Print a message to stderr.
  // Put the text on the stderr.
  
  bool TemplateComplexBodyC::DoErrPrint(StringC &txt) {
    //RavlSysLog(SYSLOG_DEBUG) << "TemplateComplexBodyC::DoErrPrint(), Called '" << txt <<"' ";
    RavlSysLog(SYSLOG_DEBUG) << Interpret(txt);
    return true;
  }
  
  //: Get initialise information from template file.
  
  bool TemplateComplexBodyC::Scan(StringC &fn) {
    TextFileC templ(fn);
    return Scan(templ);
  }

  bool TemplateComplexBodyC::Scan(TextFileC &templ) {  
    OStreamC toutput("/dev/null");
    TemplateFileC master(templ ,toutput);
    while(master.IsElm()) {
      StringC &ip = master.Next();
      int at = ip.index(':');
      if(at < 0)
	continue;
      StringC var = ip.before(at);
      //RavlSysLog(SYSLOG_DEBUG) << "Got data:'" << ip <<"'  Var: '" << var << "' ";
      if(!presets.IsMember(var))
	continue;
      StringC value = ip.after(at);
      vars.Top()[var] = value;    
    }  
    return true;
  }

  //: Interpret string 'str' and return the result.
  
  StringC TemplateComplexBodyC::Interpret(const StringC &str) {
    StringC ret;
    try {
      StrOStreamC outStr;
      output.Push(outStr);
      TextFileC subTextBuff(str,true,true);
      if(!BuildSub(subTextBuff))
        RavlSysLog(SYSLOG_WARNING) << "WARNING: BuildSub of '" << str << "' in template '" << templFile.Filename() << "' failed.";
      ret = outStr.String();
      ONDEBUG(RavlSysLog(SYSLOG_DEBUG) << "TemplateComplexBodyC::Interpret(), '" << str << "' -> '" << ret << "'");
      output.DelTop();
    } catch(...) {
      // Help a bit with debugging...
      RavlSysLog(SYSLOG_ERR) << "Failed to interpret '" << str << "' ";
      throw ;
    }
    return ret;
  }
  
  //: Interpret string.
  bool TemplateComplexBodyC::DoInterpret(StringC &txt) {
    StringC result = Interpret(txt);
    TextFileC subTextBuff(result,true,true);
    return BuildSub(subTextBuff);
  }
  
  //: Set variable.
  
  bool TemplateComplexBodyC::DoSet(StringC &data) {
    ONDEBUG(RavlSysLog(SYSLOG_DEBUG) << "Set '" << data << "'");
    int eqSign = data.index('=');
    if(eqSign < 0) {
      RavlSysLog(SYSLOG_ERR) << "Malformed set '" << data << "'";
      return false;
    }
    StringC varName = data.before(eqSign).TopAndTail();
    if(varName == "") {
      RavlSysLog(SYSLOG_ERR) << "ERROR: Empty variable name in '" << data << "'";
      return false;
    }
    
    StringC value = data.after(eqSign).TopAndTail();
    ONDEBUG(RavlSysLog(SYSLOG_DEBUG) << "Value='" << value << "'");
    if(value != "") 
      vars.Top()[varName] = Interpret(value);
    else
      vars.Top().Del(varName); // Remove variable.
    return true;
  }
  
  //: Look over a list.
  
  bool TemplateComplexBodyC::DoFor(StringC &data) {
    int startList = data.index(':');
    if(startList < 0) {
      RavlSysLog(SYSLOG_ERR) << "Malformed 'for' in template. '" << data << "' no list ";
      return false;
    }
    int startTmpl = data.index(':',startList+1);
    if(startTmpl < 0) {
      RavlSysLog(SYSLOG_ERR) << "Malformed 'for' in template. '" << data << "' no template ";
      return false;
    }
    
    StringC sep = data.before(startList);
    SubStringC list = data.at(startList+1,(startTmpl - startList)-1).TopAndTail();
    
    //RavlSysLog(SYSLOG_DEBUG) << "DoFor: Sep='" << sep << "' List='" << list << "'";
    
    if(list.length() < 1)  // Nothing in list ?
      return true;
    if(list.firstchar() == '$') {
      //ONDEBUG(RavlSysLog(SYSLOG_DEBUG) << "Subst param1 '" << param1  << "' -> '" << obj.Var(param1) << "'");
      StringC val = list.after(0);
      if(!Lookup(val,val)) {
        RavlSysLog(SYSLOG_ERR) << "Unknown variable in for. '" << val << "'";
	return false;
      }
      list = val;
    }
    SubStringListC strList(list,sep);
    SubStringC subtempltxt = data.after(startTmpl);
    for(DLIterC<SubStringC> it(strList);it.IsElm();it.Next()) {
      StringC tb = subtempltxt;
      tb.gsub("%arg%",*it);
      //RavlSysLog(SYSLOG_DEBUG) << "For value : '" << *it << "' Res:'" << tb <<"'";
      TextFileC subTextBuff(tb,true,true);
      BuildSub(subTextBuff);
    }
    return true;
  }
  
  //: Check if a condition is met.
  
  bool TemplateComplexBodyC::CheckCondition(const SubStringC &ccond) {
    SubStringC &cond = const_cast<SubStringC &>(ccond);
    
    // Treat list like a set of minterms.
    
    if(cond.contains('|')) { // Any or's ?
      SubStringListC sl(cond,"|");
      for(DLIterC<SubStringC> it(sl);it.IsElm();it.Next()) {
	if(CheckCondition(it.Data()))
	  return true;
      }
      return false;
    }
    
    if(cond.contains('&')) { // And ands ?
      SubStringListC sl(cond,"&");
      for(DLIterC<SubStringC> it(sl);it.IsElm();it.Next()) {
	if(!CheckCondition(it.Data()))
	  return false;
      }
      return true;
    }
    
    // Find condition.
    
    SubStringC param1;
    SubStringC param2;
    
    IntT at = cond.index("!=");
    bool ifnot = true;
    if(at >= 0) {
      param1 = cond.before(at);
      param2 = cond.after(at+1);
    } else { // Equals ?
      at = cond.index('=');
      if(at < 0) {
        RavlSysLog(SYSLOG_ERR) << "Unrecognised condition '" << cond << "' ";
	return false;
      }    
      param1 = cond.before(at);
      param2 = cond.after(at);
      ifnot = false;
    }
    
    // Sort out parameters.
    
    param1=param1.TopAndTail();
    param2=param2.TopAndTail();  
    
    if(param1.length() > 0) {
      if(param1.firstchar() == '$') {
	param1 = param1.after(0);
	//ONDEBUG(RavlSysLog(SYSLOG_DEBUG) << "Subst param1 '" << param1  << "' -> '" << obj.Var(param1) << "'");
	StringC val = param1;
	if(!Lookup(val,val)) {
	  RavlSysLog(SYSLOG_ERR) << "Unknown variable in if. '" << param1 << "' ";
	  return false;
	}
	param1 = val;
      }
    }
    
    if(param2.length() > 0) {
      if(param2.firstchar() == '$') {
	param2 = param2.after(0);
	StringC val = param2;
	//ONDEBUG(ONDEBUG(RavlSysLog(SYSLOG_DEBUG) << "Subst param2 '" << param2  << "' -> '" << obj.Var(param2) << "'");
	if(!Lookup(val,val)) {
	  RavlSysLog(SYSLOG_ERR) << "Unknown variable in if. '" << param1 << "'";
	  return false;
	}
	param2 = val;
      }
    }
    
    // Do test.
    
    if(!ifnot) 
      return (param1 == param2);   
    return (param1 != param2);
  }
  
  //: If true.
  
  bool TemplateComplexBodyC::DoIf(StringC &data) {
    ONDEBUG(RavlSysLog(SYSLOG_DEBUG) << "If '" << data << "'");
    
    int templStart = data.index(':');
    if(templStart < 0) {
      RavlSysLog(SYSLOG_ERR) << "Malformed 'if' in template. '" << data << "' ignoring ";
      return false;
    }
    SubStringC condition = data.before(templStart);
    if(!CheckCondition(condition)) {
      lastIf = false;
      return true;
    }
    
    // Test passed.
    
    StringC subtempltxt = data.after(templStart);
    TextFileC subTextBuff(subtempltxt,true,true);
    BuildSub(subTextBuff);
    lastIf = true;
    return true;
  }
  
  //: If variable set
  
  bool TemplateComplexBodyC::DoIfSet(StringC &data) {
    ONDEBUG(RavlSysLog(SYSLOG_DEBUG) << "IfSet '" << data << "'");
    int templStart = data.index(':');
    if(templStart < 0) {
      RavlSysLog(SYSLOG_ERR) << "Malformed 'ifset' in template. '" << data << "' ignoring ";
      return false;
    }
    SubStringC condition = data.before(templStart);
    StringC tmp;
    switch(condition.firstchar()) 
      {
      case 0:
        RavlSysLog(SYSLOG_ERR) << "Malformed 'ifset' in template, no condition. '" << data << "' ignoring ";
	return false; 
      case '!': 
	{
	  StringC x = condition.after(0);
	  if(Lookup(x,tmp)) {
	    lastIf = false;
	    return true;
	  }
	  if(commands.IsElm(x)) {
	    lastIf = false;
	    return true;
	  }
	}
	break;
      default: 
	{
	  StringC x = condition;
	  if(!Lookup(x,tmp)) {
	    if(!commands.IsElm(x)) {
	      lastIf = false;
	      return true;
	    }
	  }
	}
	break;
      }
    // Test passed.
    
    StringC subtempltxt = data.after(templStart);
    TextFileC subTextBuff(subtempltxt,true,true);
    BuildSub(subTextBuff);
    lastIf = true;
    return true;
  }
  
  //: 
  
  bool TemplateComplexBodyC::DoElse(StringC &data) {
    ONDEBUG(RavlSysLog(SYSLOG_DEBUG) << "Else '" << data << "'");
    if(lastIf)
      return true;
    TextFileC subTextBuff(data,true,true);
    BuildSub(subTextBuff);  
    return true;
  }
  
  //: Include file.
  
  bool TemplateComplexBodyC::DoInclude(StringC &data) {
    ONDEBUG(RavlSysLog(SYSLOG_DEBUG) << "Include '" << data << "'");
    if(incDepth > 100) {
      RavlSysLog(SYSLOG_ERR) << "ERROR: Include depth to great, " << incDepth << " Ignoring. ";
      return false;
    }
    data = Interpret(data);
    ONDEBUG(RavlSysLog(SYSLOG_DEBUG) << "Include after interpret '" << data << "'");
    TextFileC subTextBuff;
    TextFileC *stb = component.Lookup(data);
    if(stb == 0) {
      FilenameC fn(data);
      if(templateDir != "") {
	if(fn.firstchar() != RavlN::filenameSeperator) {
	  if(fn.firstchar() == '@')
	    fn = fn.after('@');
	  else
	    fn = templateDir + RavlN::filenameSeperator + fn;
	}
      }
      if(!fn.Exists()) {
        RavlSysLog(SYSLOG_ERR) << "Can't find include file '" << data << "' ";
	return false;
      }
      ONDEBUG(RavlSysLog(SYSLOG_DEBUG) << "Including '" << fn << "' ");
      subTextBuff = TextFileC(fn);
      component[data] = subTextBuff;
    } else
      subTextBuff = *stb;
    incDepth++;
    BuildSub(subTextBuff);
    incDepth--;
    return true;
  }
  
  //: Make a sub context.
  
  bool TemplateComplexBodyC::DoSub(StringC &txt) {
    ONDEBUG(RavlSysLog(SYSLOG_DEBUG) << "Sub '" << txt << "'");
    
    vars.Push(RCHashC<StringC,StringC>(true) ); // Push base variables.
    TextFileC subTextBuff(txt,true,true);
    BuildSub(subTextBuff);
    vars.DelTop(); // Restore old set.
    return true;
  }
  
  //: Make a sub context, copy old context.
  
  bool TemplateComplexBodyC::DoSubCopy(StringC &txt) {
    ONDEBUG(RavlSysLog(SYSLOG_DEBUG) << "SubCopy '" << txt << "'");
    
    // Push copy of vars.
    vars.Push(RCHashC<StringC,StringC>(vars.Top().Copy()));
    TextFileC subTextBuff(txt,true,true);
    BuildSub(subTextBuff);
    vars.DelTop(); // Restore old set.  
    return true;
  }
  
  //: Substitution
  
  bool TemplateComplexBodyC::DoSubst(StringC &txt) {
   
    int arg1s, arg2s, before;

    arg1s=-1;
    do // Find the first non-escaped :
    {  arg1s  = txt.index(':',arg1s+1);
       if(arg1s < 0) {
         RavlSysLog(SYSLOG_ERR) << "Malformed 'subst' in template. '" << txt << "' ignoring ";
         return false;
       }
       before = arg1s-1;
       while (before >= 0)
       {  if (txt.matches("\\",before,false))
             before--;
          else
             break;
       }
    } while ((arg1s-before)%2 == 0);
    // An even result implies \:, \\\:, etc. i.e. : is escaped so look for the next

    arg2s=arg1s;
    do // Find the next non-escaped :
    {  arg2s = txt.index(':',arg2s+1);
       if(arg2s < 0) {
         RavlSysLog(SYSLOG_ERR) << "Malformed 'subst' in template. '" << txt << "' ignoring ";
         return false;
       }
       before = arg2s-1;
       while (before > arg1s) {
         if (txt.matches("\\",before,false))
             before--;
          else
             break;
       }
    } while ((arg2s-before)%2 == 0);
    // An odd result implies c:, c\\:, etc. i.e. : is not escaped so is the separator

    // Extract arguments dropping escape (\) characters  
    StringC arg1 = txt.before(arg1s);
    arg1.gsub("\\:",":");  // We know this is not going to wrongly change \\:
                           // as we will not have a \\: as that will have been
                           // regarded as the : terminating the argument
    arg1.gsub("\\\\","\\");// Collapse \\ into \

    StringC arg2 = txt.at(arg1s+1,(arg2s-arg1s)-1);
    arg2.gsub("\\:",":");  // Again we know we have no \\: to break
    arg2.gsub("\\\\","\\");

    StringC value = txt.after(arg2s);
    // We do not process the original text for \-escape sequences. They are not
    // required to get $(subst to work, and such processing would cause problems
    // with nesting of commands (the user would need to double-up (then triple,
    // etc) any \ characters). This is offset against the slightly inconsistent
    // design where 2 parameters have one form of processing, the third another
    // form. However, the selected path is probably the more straightforward of
    // the two possibilities.
    
    ONDEBUG(RavlSysLog(SYSLOG_ERR) << "DoSubst '" << arg1 << "' '" << arg2 << "' '" << value << "' ");
    
    StringC resultStr = Interpret(value);
    resultStr.gsub(arg1,arg2);
    Output() << resultStr;
    return true;
  }

  //: 
  
  bool TemplateComplexBodyC::DoDefine(StringC &data) {
    int templStart = data.index(':');
    if(templStart < 0) {
      RavlSysLog(SYSLOG_ERR) << "Malformed 'define' in template. '" << data << "' ignoring ";
      return false;
    }
    StringC macroNm = data.before(templStart); // Get name of macro.
    StringC macroVal = data.after(templStart); // Get string for macro.
    ONDEBUG(RavlSysLog(SYSLOG_DEBUG) << "DoDefine '" << macroNm << "' Val:'" << macroVal << "'");
    StringC empty;    
    //commands[macroNm] = CallMethod2C<TemplateComplexBodyC &,StringC &,StringC &,bool>(*this,&TemplateComplexBodyC::DoMacro,empty,macroVal);
    commands[macroNm] = TriggerR(*this,&TemplateComplexBodyC::DoMacro,empty,macroVal);
    return true;
  }
  
  //: 
  
  bool TemplateComplexBodyC::DoMacro(StringC &arg,StringC &macro) {
    ONDEBUG(RavlSysLog(SYSLOG_DEBUG) << "DoMacro '" << macro << "' Arg:'" << arg << "' ");
    if(incDepth > 100) {
      RavlSysLog(SYSLOG_ERR) << "ERROR: Recurse depth to great, " << incDepth << " Ignoring. ";
      return false;
    }
    
    StringC submacro = macro.Copy();
    submacro.gsub("%arg%",arg);
    TextFileC subTextBuff(submacro,true,true);
    incDepth++;
    BuildSub(subTextBuff);
    incDepth--;
    return true;
  }
  
  
  //: Make all text upper case.
  
  bool TemplateComplexBodyC::DoUpperCase(StringC &txt) {
    StrOStreamC outStr; 
    output.Push(outStr);
    TextFileC subTextBuff(txt,true,true);
    if(!BuildSub(subTextBuff))
      RavlSysLog(SYSLOG_WARNING) << "BuildSub of '" << txt << "' in template '" << templFile.Filename() << "' failed. ";
    output.DelTop();
    Output() << upcase(outStr.String());
    return true;
  }
  
  //: Make all text lower case.
  
  bool TemplateComplexBodyC::DoLowerCase(StringC &txt) {
    StrOStreamC outStr;    
    output.Push(outStr);
    TextFileC subTextBuff(txt,true,true);
    if(!BuildSub(subTextBuff))
      RavlSysLog(SYSLOG_WARNING) << "BuildSub of '" << txt << "' in template '" << templFile.Filename() << "' failed. ";
    output.DelTop();
    Output() << downcase(outStr.String());
    return true;
  }
  
  //: Make all text lower case.
  // Redirect output to /dev/null

  bool TemplateComplexBodyC::DoSilent(StringC &txt) {
    OStreamC outStr("/dev/null");
    output.Push(outStr);
    TextFileC subTextBuff(txt,true,true);
    if(!BuildSub(subTextBuff))
      RavlSysLog(SYSLOG_WARNING) << "BuildSub of '" << txt << "' in template '" << templFile.Filename() << "' failed. ";
    output.DelTop();
    return true;
  }

  //: Return the first N charactors of a string.
  
  bool TemplateComplexBodyC::DoFirstN(StringC &txt) {
    int arg1s = txt.index(':');
    if(arg1s < 0) {
      RavlSysLog(SYSLOG_ERR) << "Malformed 'truncate' in template. '" << txt << "' ignoring ";
      return false;
    }
    SubStringC arg1 = txt.before(arg1s);
    StringC value = txt.after(arg1s);
    IntT n = arg1.IntValue();
    
    StrOStreamC outStr;    
    output.Push(outStr);
    TextFileC subTextBuff(value,true,true);
    if(!BuildSub(subTextBuff))
      RavlSysLog(SYSLOG_WARNING) << "BuildSub of '" << value << "' in template '" << templFile.Filename() << "' failed. ";
    output.DelTop();
    StringC outs = outStr.String(); 
    if(outs.Size() > (UIntT) n)
      Output() << outs(0,n);
    else
      Output() << outs;
    return true;
  }
  
  //: Return the first N charactors of a string, if the string is truncated indicate it with a marker.
  
  bool TemplateComplexBodyC::DoMarkFirstN(StringC &txt) {
    int arg1s = txt.index(':');
    if(arg1s < 0) {
      RavlSysLog(SYSLOG_ERR) << "Malformed 'marktruncate' in template. '" << txt << "' ignoring ";
      return false;
    }
    int arg2s = txt.index(':',arg1s+1);
    if(arg2s < 0) {
      RavlSysLog(SYSLOG_ERR) << "Malformed 'marktruncate' in template. '" << txt << "' ignoring ";
      return false;
    }
    SubStringC arg1 = txt.before(arg1s);
    SubStringC arg2 = txt.at(arg1s+1,(arg2s-arg1s)-1);
    StringC value = txt.after(arg2s);
    
    IntT n = arg1.IntValue();
    if(n < (IntT) arg2.Size()) {
      RavlSysLog(SYSLOG_WARNING) << "Marker larger than truncation size, expanding to marker size. '" << txt << "'. ";
      n = arg2.Size();
    }
    StrOStreamC outStr;    
    output.Push(outStr);
    TextFileC subTextBuff(value,true,true);
    if(!BuildSub(subTextBuff))
      RavlSysLog(SYSLOG_WARNING) << "WARNING: BuildSub of '" << value << "' in template '" << templFile.Filename() << "' failed. ";
    output.DelTop();
    StringC outs = outStr.String(); 
    if(outs.Size() > (UIntT) n) {
      Output() << outs(0,n-arg2.Size()) << arg2;
    } else
      Output() << outs;
    return true;
  }
  
  //: Make string suitable for use in plain html.
  
  StringC TemplateComplexBodyC::MakeHtml(const SubStringC &name) {
    ONDEBUG(RavlSysLog(SYSLOG_DEBUG) << "TemplateComplexBodyC::MakeHtml(SubStringC&) called, '" << name << "' ");
    StringC ret = StringC(name);
    ret.gsub("&","&amp;");
    ret.gsub("<","&lt;");
    ret.gsub(">","&gt;");    
    return ret;
  }

  StringC TemplateComplexBodyC::MakeHtml(const StringC &str) {
    StringC ret = str.Copy();
    ret.gsub("&","&amp;");
    ret.gsub("<","&lt;");
    ret.gsub(">","&gt;");    
    return ret;
}
  
  //: Make sure text will be taken literally in html.
  
  bool TemplateComplexBodyC::HtmlSafe(StringC &value) {
    ONDEBUG(RavlSysLog(SYSLOG_DEBUG) << "HtmlSafe '" << value << "'");
    if(value == "") 
      return true;
    
    StrOStreamC outStr;    
    output.Push(outStr);
    TextFileC subTextBuff(value,true,true);
    
    if(!BuildSub(subTextBuff))
      RavlSysLog(SYSLOG_WARNING) << "BuildSub of '" << value << "' in template '" << templFile.Filename() << "' failed. ";
    output.DelTop();
    StringC tmp = outStr.String();
    output.Top() << MakeHtml(tmp);
    return true;
  }

  //: Return text before a string.
  
  bool TemplateComplexBodyC::DoBefore(StringC &txt) {
    int arg1s = txt.index(':');
    if(arg1s < 0) {
      RavlSysLog(SYSLOG_ERR) << "Malformed 'before' in template '" << txt << "' ignoring ";
      return false;
    }
    SubStringC arg1 = txt.before(arg1s);
    StringC value = txt.after(arg1s);
    StringC resultStr = Interpret(value);
    if(resultStr.contains(arg1))
      Output() << resultStr.before(arg1);
    else
      Output() << resultStr;
    return true;
  }
  
  //: Return text after a string.
  
  bool TemplateComplexBodyC::DoAfter(StringC &txt) {
    int arg1s = txt.index(':');
    if(arg1s < 0) {
      RavlSysLog(SYSLOG_ERR) << "Malformed 'after' in template '" << txt << "' ignoring ";
      return false;
    }
    SubStringC arg1 = txt.before(arg1s);
    StringC value = txt.after(arg1s);
    StringC resultStr = Interpret(value);
    if(resultStr.contains(arg1))
      Output() << resultStr.after(arg1);
    else
      Output() << resultStr;
    return true;
  }
  
  //: Lookup variable.
  // if found put value into 'buff' and return true.
  // otherwise return false.
  
  bool TemplateComplexBodyC::Lookup(const StringC &varname,StringC &buff) {
    StringC *valPtr = vars.Top().Lookup(varname);
    if(valPtr == 0) {
      for(unsigned i = 0;i < m_lookupMethods.size();i++) {
        RavlAssert(m_lookupMethods[i].IsValid());
        StringC tmp(varname);
        if(m_lookupMethods[i].Call(tmp,buff)) {
          // Its up to the method to cache the value if apporiate.
          return true;
        }
      }
      return false;
    }
    buff = *valPtr;
    return true;
  }

  //: Setup method to lookup variable values.

  void TemplateComplexBodyC::SetupLookup(const CallFunc2C<const StringC &,StringC &,bool> &lookupMethod)
  {
    m_lookupMethods.push_back(lookupMethod);
  }

  //: Start build of document.
  
  bool TemplateComplexBodyC::BuildSub(const TextFileC &subtempl) {
    TemplateFileC tf(subtempl ,output.Top());
    bool ret = true;
    while(tf.IsElm()) {
      StringC &ip = tf.Next();
      if(ip.IsEmpty())
	break;
      int at = ip.index(':');
      //RavlSysLog(SYSLOG_DEBUG) << "Value:'" << ip <<"'";
      if(at == -1) {
	StringC buff;
	if(Lookup(ip,buff)) {
	  ONDEBUG(RavlSysLog(SYSLOG_DEBUG) << "TemplateComplexBodyC::BuildSub(), Adding '" << buff << "' ");
	  output.Top() << buff;
	  continue;
	}
	RavlSysLog(SYSLOG_ERR) << "Unknown variable: '" << ip <<"' in template '" << templFile.Filename() << "' ";
        ret = false;
	continue;
      }
      
      // Prefixed command.
      CallFunc1C<StringC &,bool> &com = commands[ip.before(at)];
      if(!com.IsValid()) {
        RavlSysLog(SYSLOG_ERR) << "Unknown command: '" << ip.before(at) << "' in template '" << templFile.Filename() << "' ";
	continue;
      }
      StringC arg(ip.after(at));
      ret &= com.Call(arg);
    }
    
    //ONDEBUG(RavlSysLog(SYSLOG_DEBUG) << "DocumentBodyC::BuildSub(), Completed for  '" << obj.Name() << "' ");
    return ret;
  }
  
}
