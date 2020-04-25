// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
///////////////////////////////////////
//! rcsid="$Id$"
//! lib=RavlCore
//
// Based on the idea's in C and C++ option library by George Matas and Radek Marik.
//! file="Ravl/Core/System/Option.cc"

#include "Ravl/Option.hh"
#include "Ravl/StringList.hh"
#include "Ravl/Index2d.hh"
#include "Ravl/TFVector.hh"

#include <stdlib.h>
#include <ctype.h>

#define OPTDEBUG 0

#if OPTDEBUG
#define ONDEBUG(x) x
#else
#define ONDEBUG(x)
#endif

// Set to 1 to support unnamed multiple arguments options.

#define SUPPORT_UNAMED_MULTIARG 1

namespace RavlN {
  //: Creates the option processing system
  // 'copy' == false ... options and pars. are removed from agrv, argc<br>
  // 'copy' == true  ... leave argv, argc untouched, work on a copy
  
  OptionC::OptionC(int argc, char ** argv,bool,OStreamC msgout)
    : leftOk(false),
      help(false),
      progName("-Unknown-"),
      sout(msgout),
      unnamed(0),
      doneUnnamed(false)
  {
    if(argc >= 1)
      progName = StringC(argv[0]);
    for(IntT i = 1;i < argc;i++)
      args.InsLast(argv[i]);
    Init();
  }
  
  //: Creates the option processing system
  // 'copy' == false ... options and pars. are removed from agrv, argc<br>
  // 'copy' == true  ... leave argv, argc untouched, work on a copy
  
  OptionC::OptionC(int argc, char ** argv, bool)
    : leftOk(false),
      help(false),
      progName("-Unknown-"),
      sout(std::cerr),
      unnamed(0),
      doneUnnamed(false)
  {
    if(argc >= 1)
      progName = StringC(argv[0]);
    for(IntT i = 1;i < argc;i++)
      args.InsLast(argv[i]);
    Init();
  }
  
  //: Creates the option processing system.
  
  OptionC::OptionC(DListC<StringC> opts,OStreamC msgout)
    : args(opts),
      leftOk(false),
      help(false),
      progName("-Unknown-"),
      sout(msgout),
      unnamed(0),
      doneUnnamed(false)
  {
    if(!args.IsEmpty()) {
      progName = opts.First();
      opts.DelFirst();
    }
    Init();
  }
  
  //: Sort out some standard options.
  
  void OptionC::Init() {
    allargs = args.Copy();
    help = Boolean("help","Print this usage information and exit. ");  
  }
  
  //: Add optload and optsave to the command line.
  // This adds the options to both load and save options to the command line.
  
  void OptionC::OfferLoadSave() {
    StringC filename = String("optload","","Load options from a file. ");
    optSave  = String("optsave","","Save options to a file. ");
    if(!filename.IsEmpty()) 
      LoadOptions(filename);
  }
  
  
  //: Returns true if the 'option' was on the command line. 
  
  bool OptionC::IsOnCommandLine(const char * option) const {
    StringC search(StringC("-") + option);
    for(DLIterC<StringC> it(const_cast<DListC<StringC> &>(allargs));it.IsElm();it.Next()) {
      if(it.Data() == search)
	return true;
  }
    return false;
  }
  
  //: Is argument a parameter ?
  /* defines which command line arguments are option specifiers and 
   * which are consider to be parameters 
   * PARAMETERs must not start with -, unless it is a single character 
   * '-' (allowing typical definition of pipe in/out) or the '-' is followed
   * by a digit (assuming a negative number 
   * Consequently, OPTIONS start with a '-' followed by at least one  letter
   */
  
  bool OptionC::IsParam(const StringC &arg) const {
    ONDEBUG(std::cerr << "Testing '" << arg << "' for being a paramiter. \n");
    if(arg.IsEmpty())
      return true;
    if (arg[0]!='-')  
      return true;
    if(arg.length() == 1 || isdigit(arg[1]))
      return true;
    return false;
  }
  
  //: has option been used already ?
  
  bool OptionC::IsUsed(const char *option) const {
    if(option == 0)
      return false;
    for(DLIterC<StringC> it(const_cast<DListC<StringC> &>(used));it.IsElm();it.Next()) {
      if(it.Data() == option)
	return true;
    }
    return false;
  }
  
  // Ends command line processing, if errors are detected their list
  // together with usage/help is printed out. 
  
  bool OptionC::Check(bool exitOnFail) {
    if(!leftOk) {
      for(DLIterC<StringC> it(args);it.IsElm();it.Next()) {
	if(!IsProcessed(it.Data()))
	  Error(StringC("Unused argument : ") + it.Data());
      }
    }
    if(help) {
      PrintUsage(); 
      if(exitOnFail)
	exit(1);
      return false;
    }
    if(errors.IsEmpty()) {
      if(!optSave.IsEmpty()) 
	SaveOptions(optSave);
      return true;
    }
    sout << "\n+++++++++++++++++++++++++++++++++++++++++++++++++++\n\n"
         << "The following errors occured while parsing options.\n\n";
    for(DLIterC<StringC> it(errors);it.IsElm();it.Next()) 
      sout << " " << it.Data() << std::endl;
    sout << "\n+++++++++++++++++++++++++++++++++++++++++++++++++++\n\n";
    PrintUsage();
    if(exitOnFail)
      exit(1);
    return false;
  }
  
  //: Save options to a file.
  
  bool OptionC::SaveOptions(StringC filename) {
    OStreamC out(filename);
    if(!out) {
      sout << "Can't open file '" << filename << "' to save options.\n";
      return false;
    }
    //out << "# Created ";
    out << *this << "\n";
    return true;
  }
  
  std::ostream & operator<<(std::ostream & out, const OptionC opt) {
    out << opt.progName << ' ';
    for(DLIterC<StringC> it(opt.allargs);it.IsElm();it.Next()) {
      if(it.Data() == "-optsave") {
	it.Next(); // Skip argument.
	continue;
      }
      if(it.Data().contains(' ')) {
	StringC tmp = it.Data().Copy();
	tmp.gsub(" ","\\ ");
	out << tmp << ' ';
      } else
	out << it.Data() << ' ';
    }
    return out;
  }

  //: Save options to a file.
  
  bool OptionC::LoadOptions(StringC filename) {
    IStreamC in(filename);
    if(!in) {
      sout << "Can't open file '" << filename << "' to load options. \n";
      return false;    
    }
    StringC line;
    do {
      readline(in,line);
    } while(in && (line.IsEmpty() || line[0] == '#')) ;
    DListC<StringC> newargs = StringListC(line,true);
    if(!newargs.IsEmpty())
      newargs.DelFirst();
    args += newargs;
#if 1
    sout << progName << ' ';
    for(DLIterC<StringC> it(args);it.IsElm();it.Next()) 
      sout << it.Data() << ' ';
    sout << "\n";
#endif
    Init();
    return true;
  }
  
  // Puts this line to the usage.

  void OptionC::Comment(const char * commentText)
  { usage.InsLast(StringC(commentText)); }
  
  
  // Puts this line to the usage.
  
  void OptionC::Comment(const StringC &commentText)
  { usage.InsLast(commentText); }
  
  
  // Gets pair of IndexC types.
  
  Index2dC OptionC::Index2d(const char * name, int def1, int def2,const char * comment) {
    IntT ret[2];
    DListC<StringC> value = GetOptions(name,2);
    DLIterC<StringC> it(value);
    if(!value.IsEmpty()) {
      RavlAssert(it);
      ret[0] = atoi(*it);
      it++; 
      RavlAssert(it);
      ret[1] = atoi(*it);
    } else {
      ret[0] = def1;
      ret[1] = def2;
    }
    StringC argName;
    if(*name == 0) argName = "arg";
    else argName = StringC('-') + name;
    Comment(argName + " (" + StringC(ret[0]) + ' ' + StringC(ret[1]) + ") [" + StringC(def1) + ' ' + StringC(def2) + "] " + comment);
    return Index2dC(ret[0],ret[1]);
  }
  
  // Gets a pair of real values.
  
  TFVectorC<RealT,2> OptionC::Point2d(const char * name,  RealT def1, RealT def2,const char * comment) {
    TFVectorC<RealT,2> ret;
    DListC<StringC> value = GetOptions(name,2);
    DLIterC<StringC> it(value);
    if(!value.IsEmpty()) {
      RavlAssert(it);
      ret[0] = atof(*it);
      it++;
      RavlAssert(it);
      ret[1] = atof(*it);
    } else {
      ret[0] = def1;
      ret[1] = def2;
    }
    StringC argName;
    if(*name == 0) argName = "arg";
    else argName = StringC('-') + name;
    Comment(argName + " (" + StringC(ret[0]) + ' ' + StringC(ret[1]) + ") [" + StringC(def1) + ' ' + StringC(def2) + "] " + comment);
    return ret;
  }
  
  ////////////////////////////////
  // Gets array of chars.
  
  char * OptionC::CharArr(const char * name,const char * def,const char * comment) {
    StringC value;
    StringC ret;
    value = GetOption1(name);
    if(IsProcessed(value))
      ret = def;
    else
      ret = value;
    if(*name == 0)
      Comment(StringC("arg ") + "(" + ret + ") [" + def + "] " + comment);
    else
      Comment(StringC('-') + name + " (" + ret + ") [" + def + "] " + comment);
    char *place,*end,*str = new char [ret.length()+1];
    const char *oth = ret.chars();
    end = &(str[ret.length()+1]);
    for(place = str;place != end;place++,oth++)
      *place = *oth;
    return str;
  }

  ////////////////////////////////
  // Gets toggle (boolean value).
  
  bool OptionC::Boolean(const char * name, bool def,const char * comment) {
    bool ret(def);
    if(GetOption0(name))
      ret = !def;
    StringC rs = ret ? "true" : "false";
    StringC ds = def ? "true" : "false";
    Comment(StringC('-') + name + " (" + rs + ") [" + ds + "] " + comment);  
    return ret;
  }
  
  ////////////////////////////////////
  // Gets a list of strings following the option 'name'. 
  
  DListC<StringC> OptionC::List(const char * name, const char * def, const char * comment ) {
    DListC<StringC> ret = GetOptions(name,-1);
    // Note: we can't use default values for anonymous options.
    if (!IsOnCommandLine(name) && *name != 0) ret = StringListC(def);
    StrOStreamC os;  
    if(*name == 0) os << "arg";
    else os << '-' << name;
    os << " (";
    for(DLIterC<StringC> it(ret);it;it++) {
      os << *it;
      if(!it.IsLast())
	os << ' ';
    }
    os << ") [" << def << "] " << comment;
    Comment(os.String());
    return ret;
  }
  
  
  /////////////////////////////////
  // xor_opt defines a set of mutually exclusive options 
  // options are specified in xor_opt without '-', eg. 'alpha beta x'  
  
  void OptionC::DependXor(const char * xor_opt) {
    
    StringListC sl(xor_opt);
    IntT grps = 0;
    StringC msg,msg2;
    for(DLIterC<StringC> it(sl);it.IsElm();it.Next()) {
      if(IsOnCommandLine(*it)) {
	if(!msg.IsEmpty())
	  msg += " and ";
	msg += StringC('-') + it.Data();
	grps++;
      }
      if(!msg2.IsEmpty())
	msg2 += " and ";
      msg2 += StringC('-') + it.Data();
    }
    Depend(msg2 + " are mutually exclusive");
    if(grps > 1)
      Error(msg + " are mutually exclusive.");
  }
  
  /////////////////////////////////
  // option "depend" can be specified only if a given condition 'enableCond'
  // is met
  
  void OptionC::If(int enableCond,const char * depend,const char * comment) {
    if(!enableCond) {
      if(IsOnCommandLine(depend)) 
	Error(StringC('-') + depend + " can only be used if " + comment);
    }
    Depend(StringC('-') + depend + " may only be used if " + comment);
  }
  
  // options (several of them) in string "depend" can be specified only if a
  // given condition 'enableCond' is met
  
  void OptionC::MultIf(int enableCond,const char * depend,const char * comment) {
    if(!enableCond) {
      StringListC sl(depend);
      for(DLIterC<StringC> it(sl);it.IsElm();it.Next()) {
	if(IsOnCommandLine(it.Data()))
	  Error(StringC(it.Data()) + " can only be used if " + comment);
      }
    }
    Depend(StringC(depend) + " may only be used if " + comment);
  }
  
  // option 'depend' can be specified only if option 'cond' has value  
  // 'enableVal'. Current value of 'depend' is passed explicitly in val
  
  void OptionC::DependIf(const char * cond, int enableVal, int val,const char * depend) {
    Depend("DependIf, currently is not supported. \n");
  }
  
  // Emits error message if 'name' was not found on the command line 
  
  void OptionC::Compulsory(const char * name) {
    RavlAssert(name != 0);
    if(!IsOnCommandLine(name))
      Error(StringC('-') + name + " not found. This option is compulsory");
    Depend(StringC('-') + name + " is compulsory.");
  }
  
  // Defines the number of compulsory arguments. It
  // must be used after all options have been called!
  
  void OptionC::CompulsoryArgs(int num) {
    if(num == 1) {
      Depend("1 argument is compulsory.");
      if(unnamed < num) 
	Error("1 argument is compulsory.");
    } else {
      Depend(StringC(num) + " args are compulsory.");
      if(unnamed < num) 
	Error(StringC(num) + " arguments are compulsory.");
    }
  }
  
  //: Get option with zero args.
  
  bool OptionC::GetOption0(const char *name) {
    RavlAssert(name != 0);
    if(IsUsed(name))
      Error(StringC("Internal program error: Option ") + name + " has been defined more than once. ");
    used.InsLast(name);
    StringC srch = StringC('-') + name;
    for(DLIterC<StringC> it(args);it.IsElm();it.Next()) {
      if(it.Data() != srch) 
      continue;
      MarkProcessed(it.Data());
      ONDEBUG(std::cerr << "OptionC::GetOption0(), Looking for :" << srch << " -> found." << endl);
      return true;
  } 
    ONDEBUG(std::cerr << "OptionC::GetOption0(), Looking for :" << srch << " -> Not found." << endl);
    return false;
  }
  
  //: Get option with one arg.
  
  StringC OptionC::GetOption1(const char *name) {
    StringC ret;
    MarkProcessed(ret); // Mark as unfound by default.
    ONDEBUG(std::cerr << "OptionC::GetOption1(), Called looking for '"<< name <<"'\n");
    RavlAssert(name != 0);
    if(*name == 0) { // Unnamed arg ?
      doneUnnamed = true;
      for(DLIterC<StringC> it(args);it.IsElm();it.Next()) {
	if(IsProcessed(it.Data()))
	  continue;
	if(!IsParam(it.Data()))
	  continue;
	ret = it.Data();
	MarkProcessed(it.Data());
	unnamed++;
	return ret;
      }
      return ret;
    }
    if(IsUsed(name))
      Error(StringC("Internal program error: Option ") + name + " has been defined more than once. ");
    if(doneUnnamed)
      Error(StringC("OptionC, Internal error: Found argument ") + name + " after an unamed one. All unamed arguments must be processed last.  ");
    used.InsLast(name);
    StringC srch = StringC('-') + name;
    for(DLIterC<StringC> it(args);it.IsElm();it.Next()) {
      if(it.Data() != srch) 
	continue;
      MarkProcessed(it.Data());
      it.Next();
      bool ok = true;
      if(it.IsElm()) {
	if(IsProcessed(it.Data()))
	  ok = false;
	else {
	  if(!IsParam(it.Data()))
	    ok = false;
	}
      } else
	ok = false;
      if(!ok) {
	Error(StringC("No value given for option '-") + name + "'");
	return StringC();
      }
      ret = it.Data();
      MarkProcessed(it.Data());
      break;
  }
    return ret;
  }
  
  //: Get option with multiple but fixed no of args.
  
  DListC<StringC> OptionC::GetOptions(const char *name,int nargs) {
    DListC<StringC> ret;
    ONDEBUG(std::cerr << "OptionC::GetOptions(), Called looking for "<< name <<" with " << nargs << " args. \n");
    
    RavlAssert(name != 0);
    
#if !SUPPORT_UNAMED_MULTIARG
    if(*name == 0) { // Unnamed option ?
      Error(StringC("Program error: Unnamed multiargument options not supported. \n"));
      return ret;
    }
#endif
    
    if(name != 0) {
      if(doneUnnamed)
	Error(StringC("OptionC, Internal error: Found argument ") + name + " after an unnamed one. All unnamed arguments must be processed last.  ");
      if(IsUsed(name))
	Error(StringC("Program error: Option ") + name + " is has already been used. ");
      used.InsLast(name);
    } else
      doneUnnamed = true;
    
    StringC srch('-');
    if(name != 0)
      srch += name;
    else {
      ONDEBUG(std::cerr << "GetOptions(), Searching for unnamed arg of length " << nargs << "\n");
    }
    
    for(DLIterC<StringC> it(args);it.IsElm();it.Next()) {
#if SUPPORT_UNAMED_MULTIARG
      if(*name == 0) {
	if(IsProcessed(it.Data()))
	  continue;
	ONDEBUG(std::cerr << "GetOptions(), Start of unnamed args : '" << it.Data() << "\n");
      } else 
#endif      
	{
	  if(it.Data() != srch) 
	    continue;
	  MarkProcessed(it.Data());
	  it.Next();
	}
      for(int i = 0;i < nargs || (nargs < 0);i++) {
	bool ok = true;
	if(it.IsElm()) {
	  if(IsProcessed(it.Data()))
	    ok = false;
	  else {
	    if(!IsParam(it.Data()))
	      ok = false;
	  }
	} else 
	  ok = false;
	if(!ok) {
	  if(nargs < 0) // Dealing with fixed number of args ?
	    break;
	  Error(StringC("Not enough arguments for -") + name + " read " + StringC(i) + " expected " + StringC(nargs));
	  return DListC<StringC>();
	}
	ret.InsLast(it.Data());
	MarkProcessed(it.Data());
	it.Next();
      }
      break;
    }
    return ret;
  }
  
  //: Get option with multiple but fixed no of args.
  //: Concatinate into one string.
  
  StringC OptionC::GetOptionsTxt(const char *name,int nargs) {
    DListC<StringC> val = GetOptions(name,nargs);
    StringC ret;
    if(val.IsEmpty()) {
      if(!IsOnCommandLine(name)) {
	MarkProcessed(ret); // Mark as unfound.
	return ret;
      }
    }
    bool isFirst(true);
    for(DLIterC<StringC> it(val);it.IsElm();it.Next()) {
      if(isFirst) {
	isFirst = false;
      } else 
	ret += ' ';
      ret += it.Data();
    }
    return ret;
  }
  
  //: Print usage to stderr.
  
  void OptionC::PrintUsage() {
    sout << "Usage: " << progName << " [options] \n";
    for(DLIterC<StringC> it(usage);it.IsElm();it.Next())
      sout << "   " << it.Data() << std::endl;
    if(!depend.IsEmpty()) {
      sout << "\nDependencies:\n";
      for(DLIterC<StringC> it(depend);it.IsElm();it.Next())
	sout << "   " << it.Data() << std::endl;
    }
  }
  
  // Have to do strings seperately, to allow spaces in them.
  
  StringC OptionC::String(const char * name,const char *def,const char * comment) {
    StringC ret;
    StringC value;
    
    // Get argument text.
    value = GetOption1(name);
    
    // Parse value.
    if(!IsProcessed(value))
      ret = value;
    else {
      if(def != 0) {
	if(*def != 0)
	  ret = def; // Use default value.
      }
    }
    
    // Build comment line.
    StrOStreamC comm;
    if(*name != 0)
      comm << '-' << name;
    else comm << "arg";
    comm << " (" << ret << ") [" << def << "] " << comment << " ";
    Comment(comm.String());
    return ret;
  }
  
#if 1
  //ndef VISUAL_CPP
  
  IntT OptionC::Int(const char * name, int def,const char * comment)
  { return Get(name,def,comment); }
  
  
  RealT OptionC::Real(const char * name, RealT def,const char * comment)
  { return Get(name,def,comment); }
  
  
  
  unsigned char OptionC::UnsignedChar(const char * name, unsigned char def,const char * comment)
  { return Get(name,def,comment); }

#else

   template<class DataT>
   DataT OptionC::GetOption(OptionC &opt,const char * name,const DataT &def,const char * comment,int nArgs = 1) {
     DataT ret;
     StringC value;
     RavlAssert(nArgs != 0);
     
     // Get argument text.
     if(nArgs == 1)
       value = opt.GetOption1(name);
     else 
       value = opt.GetOptionsTxt(name,nArgs);
     
     // Parse value.
     if(!opt.IsProcessed(value)) // If arg not found,
       StrIStreamC(value) >> ret;
     else
       ret = def; // Use default value.
     
     // Build comment line.
     StrOStreamC comm;
     if(*name != 0)
       comm << '-' << name;
     else comm << "arg";
     comm << " (" << ret << ") [" << def << "] " << comment << " ";
     opt.Comment(comm.String());
     return ret;
   }
  
  
  IntT OptionC::Int(const char * name, int def,const char * comment)
  { return GetOption(*this,name,def,comment); }
  
  
  
  RealT OptionC::Real(const char * name, RealT def,const char * comment)
  { return GetOption(*this,name,def,comment); }
  
  
  unsigned char OptionC::UnsignedChar(const char * name, unsigned char def,const char * comment)
  { return GetOption(*this,name,def,comment); 
#endif

}


