// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVL_OPTION_HEADER
#define RAVL_OPTION_HEADER 1
//////////////////////////////////////////////////////////////////////////
//! file="Ravl/Core/System/Option.hh"
//! author="Charles Galambos, Radek Marik, George Matas" 
//! lib=RavlCore
//! date="12/11/1993"
//! userlevel=Basic
//! example=exOption.cc
//! docentry="Ravl.API.Core.Misc"
//! rcsid="$Id$"
//
// Based on the ideas in C and C++ option library by George Matas and Radek Marik.

#include "Ravl/String.hh"
#include "Ravl/DList.hh"
#include "Ravl/StrStream.hh"
#include "Ravl/Assert.hh"

namespace RavlN {

  class Index2dC;
  template<class DataT,unsigned int N> class TFVectorC;
  
  //! userlevel=Basic
  //: The class OptionC is a simple parser of a command line. 
  // <p>This is a small object. <br>
  // The syntax of the command line tags uses the Unix convention of a single
  // dash followed by one or more alpha-numeric characters. </p>

  // <p><font color="#FF0000">Bug:</font> "Unnamed" arguments (i.e. arguments without a tag) <font color="#AA0000">MUST</font> be
  // processed last in the <i>code</i> for this class to work correctly.
  // On the other hand they can appear anywhere on the <i>command line</i>.
  
  class OptionC {
  public:
    OptionC(int argc, char ** argv, bool copy, OStreamC msgout);
    //: Creates the option processing system
    //!param: 'copy' == false - options and pars. are removed from argv, argc
    //!param: 'copy' == true  - leave argv, argc untouched, work on a copy
    //!param: msgout - Where to send messages about option processing.

    OptionC(int argc, char ** argv, bool copy = false);
    //: Creates the option processing system
    //!param: 'copy' == false - options and pars. are removed from argv, argc
    //!param: 'copy' == true  - leave argv, argc untouched, work on a copy

    OptionC(DListC<StringC> opts,OStreamC msgout = OStreamC(std::cerr));
    //: Creates the option processing system from a list of strings.
    // Note that the list has to mimic the "argv" list; hence it requires a dummy "main program" name as the first item
    //!param: msgout - Where to send messages about option processing.

    void OfferLoadSave(); 
    //: Add optload and optsave to the command line.
    // This adds the options to both load and save options to the command line.
    // When used, this should be called before any other options are processed.
    
    bool IsOnCommandLine(const char * option) const;
    // Returns true if the 'option' was on the command line. 
    
    inline void LeftOk()
      { leftOk = true; }
    //: Does not consider unprocessed options as an error. 
    
    bool Check(bool exitOnFail = true);
    //: Ends command line processing, 
    // if errors are detected their list together with usage/help is printed out. 
    
    inline DListC<StringC> Usage()
      { return usage; }
    //: Returns complete usage (NULL terminated). 
    
    void Comment(const char * commentText);
    //: Puts this line to the usage.
    
    void Comment(const StringC &commentText);
    //: Puts this line to the usage.
    
    template<class DataT>
    DataT Get(const char * name,const DataT &def,const char * comment,int nArgs = 1) {
      DataT ret;
      StringC value;
      RavlAssert(nArgs != 0);
      RavlAssert(name != 0);
      // Get argument text.
      if(nArgs == 1)
	value = GetOption1(name);
      else 
	value = GetOptionsTxt(name,nArgs);
      
      // Parse value.
      if(IsProcessed(value)) // If arg not found,
	ret = def;           // Use default value.
      else
	StrIStreamC(value) >> ret;
      
      // Build comment line.
      StrOStreamC comm;
      if(*name != 0)
	comm << '-' << name;
      else comm << "arg";
      comm << " (" << ret << ") [" << def << "] " << comment << " ";
      Comment(comm.String());
      return ret;
    }
    //: Get option with multipal args
    
    IntT Int(const char * name, int def,const char * comment);
    //: Gets integer.
        
    Index2dC Index2d(const char * name, int def1, int def2,const char * comment);
    //: Gets pair of IndexC types.
    
    RealT Real(const char * name, RealT def,const char * comment);
    //: Gets real value.
    
    TFVectorC<RealT,2> Point2d(const char * name,  RealT def1, RealT def2,const char * comment);
    //: Gets a pair of real values.
    
    char * CharArr(const char * name,const  char * def,const char * comment);
    //: Gets array of chars.
    // This returns a string allocated with 'new char [xx]' it is the callers
    // responability to free this string after use.
    
    StringC String(const char * name, const char *def, const char * comment);
    //: Gets a string.
    //N.B.: if "name" is the empty string, the default value is returned
    
    unsigned char UnsignedChar(const char * name, unsigned char def,const char * comment);
    //: Gets an unsigned char.
    
    bool Boolean(const char * name, const char * comment)
    { return Boolean(name, false, comment); }
    //: True / false if option is present / absent
        
    bool Boolean(const char * name, bool def,const char * comment);
    //: Gets toggle (boolean value).
    //!deprecated: the "def" argument causes endless confusion to the users; use previous Boolean() method,.
        
    DListC<StringC> List(const char * name, const char * def, const char * comment );
    //: Gets a list of strings following the option 'name'. 
    //!param: name - name of option
    //!param: def - default value as it would appear on the command line. Spaces separate entries in the list. If entries contain spaces quotes may be used to separate the entries.
    //!param: commit - Comment describing option.
    
    // This function can used only if the header file 'DList.hh' is 
    // called before this header file.
    
    inline DListC<StringC> List(const char * name,const char * comment )
    { return List(name, "", comment); }
    //!deprecated: has no default list; use previous List() method,.
    
    //!section:  Dependency definition functions.
    /* -------------------------------- */
    
    void DependXor(const char * xor_opt);
    //: xor_opt defines a set of mutually exclusive options 
    // Options are specified in xor_opt without '-', as a single string, eg. "alpha beta x"  
    
    void If(int enableCond, const char * depend,const  char * comment);
    //: Option "depend" can be specified only if a given condition 'enableCond' is met
    
    void MultIf(int enableCond,const char * depend,const char * comment);
    //: Options (several of them) in string "depend" can be specified only if a
    //: given condition 'enableCond' is met
    
    void DependIf(const char * cond, int enableVal, int val, const char * depend);
    // Option 'depend' can be specified only if option 'cond' has value  
    // 'enableVal'. Current value of 'depend' is passed explicitly in val
  
    void Compulsory(const char * name);
    //: Emits error message if 'name' was not found on the command line 
    
    void CompulsoryArgs(int num);
    //: Defines the number of compulsory arguments. 
    // Use it to specify compulsory arguments that don't have a tag<br>
    // It must be used after all options have been called!
    
    void PrintUsage();
    //: Print usage to sout.
    
    bool SaveOptions(StringC filename);
    //: Save options to a file.
    // If OptionC is constructed on argv and argc, the effect is to save the command line
    
    bool LoadOptions(StringC filename);
    //: Load options from a file.
    // The format is as it would be on a command line.<br>
    // This MUST be called before any options are processed for
    // it to have any effect.

    friend std::ostream & operator<<(std::ostream & s, const OptionC opt);
    
#if !RAVL_COMPILER_VISUALCPP
  protected:
#endif
    StringC GetOption1(const char *name);
    //: Get option with one arg.
    
    StringC GetOptionsTxt(const char *name,int nargs);
    //: Get option with multiple but fixed no of args.
    //: Concatinate into one string.
    
  protected:
    bool IsParam(const StringC &arg) const;
    //: Is argument a paramiter.
    
    inline bool IsProcessed(const StringC &str) const
      { return (str.length() == 1 && str[0] == 0); }
    //: Has arg been processed.
    
    inline void MarkProcessed(StringC &str) const { 
      str = StringC(" ",1,1);
      str[0] = 0;
    }
    //: Has arg been processed.
    
    bool IsUsed(const char *option) const;
    //: has option been used already ?
    // Note this only works in debug mode!
    
    bool GetOption0(const char *name);
    //: Get option with one arg.
    
    
    DListC<StringC> GetOptions(const char *name,int nargs);
    //: Get option with multiple but fixed no of args.
    
    void Init();
    //: Do sort out some standard options.
    
    inline void Error(const StringC &text) { errors.InsLast(text); }
    //: Add an error.
    
    inline void Depend(const StringC &text) { depend.InsLast(text); }
    //: Add an depend.
    
    DListC<StringC> args;   // List of arguments left.
    DListC<StringC> allargs;// List of all arguments.
    DListC<StringC> used;   // List of used args.
    
    DListC<StringC> usage;  // Usage text.
    DListC<StringC> depend; // Dependancies text.
    DListC<StringC> errors; // List of error messages.
    bool leftOk;        // True if unused args are ok.
    bool help;
    StringC progName;       // Name of program
    StringC optSave;        // File to save options to.
    OStreamC sout;          // Where to send messages about option processing.
    IntT unnamed;           // Number of unnamed args found.
    bool doneUnnamed;        // Have we seen an unnamed arg ?
  };
  
  //////////////////////////////////////////////
  
  //template<class DataT>
  //DataT OptionC::Get(const char * name,const DataT &def,const char * comment,int nArgs)
  
  std::ostream & operator<<(std::ostream & s, const OptionC opt); 

}

#endif
