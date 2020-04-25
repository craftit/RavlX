// This file is part of CxxDoc, The RAVL C++ Documentation tool 
// Copyright (C) 2004, University of Surrey
// This code may be redistributed under the terms of the GNU General 
// Public License (GPL). See the gpl.licence file for details or
// see http://www.gnu.org/copyleft/gpl.html
// file-header-ends-here
///////////////////////////////////////////////////////
//! rcsid="$Id$"
//! author="Charles Galambos"
//! docentry="Ravl.API.Source Tools"
//! lib=RavlSourceTools
//! file="Ravl/SourceTools/CodeManager/docSubst.cc"

#include "Ravl/Option.hh"
#include "Ravl/EntryPnt.hh"
#include "Ravl/Text/TextFile.hh"
#include "Ravl/Text/TextCursor.hh"
#include "Ravl/SourceTools/DefsFile.hh"
#include "Ravl/SourceTools/SourceCodeManager.hh"
#include "Ravl/SourceTools/SourceFile.hh"
#include "Ravl/CallMethods.hh"
#include "Ravl/OS/Filename.hh"


using namespace RavlN;

class SubstsC
{
public:
  SubstsC()
  {}
  //: Default constructor.
  
  SubstsC(const StringC &l,const StringC &o,const StringC &n,bool v,bool dr, bool headers=true, bool mains=false, bool tests=false, bool examples=false)
    : label(l),
      original(o),
      newun(n),
      verbose(v),
      dryRun(dr),
      doHeaders(headers),
      doMains(mains),
      doTests(tests),
      doExamples(examples)
  {}
  //: Constructor.
  
  bool Process(StringC &dir,DefsMkFileC &where);
  //: Process a directory.
  
  bool IsVerbose() const
  { return verbose; }
  //: In verbose mode ?

  IntT  DoSubst(TextFileC &hdrfile);
  //: Subsitute value.
  // returns number of substitued lines
  
protected:
  StringC label;
  StringC original;
  StringC newun;
  bool verbose;
  bool dryRun;
  bool doHeaders ; 
  bool doMains ;
  bool doTests ;
  bool doExamples ;
};

//: Process a directory.

bool SubstsC::Process(StringC &dir,DefsMkFileC &where) {
  if(IsVerbose())
    std::cerr << "Processing: " << dir << "\n";

  StringListC hdrs ; 
  if ( doHeaders )
    hdrs += (where.Value("HEADERS"));
  if (doMains)
    hdrs += (where.Value("MAINS") ) ; 
  if (doTests) 
    hdrs += (where.Value("TESTEXES") ) ; 
  if (doExamples) 
    hdrs += (where.Value("EXAMPLES") ) ;
  
  
  for(DLIterC<StringC> it(hdrs);it.IsElm();it.Next()) {
    if(it.Data().length() <= 3) {
      std::cerr << "WARNING: Short header file name detected :'" << it.Data() << "'\n";
      continue; // Far too short !
    }

    if( (!it.Data().matches(".hh",it.Data().length()-3) ) &&  ( !it.Data().matches(".cc", it.Data().length()-3)) ) 
      {
        //cerr << "WARNING: Non C++ header file ignored:'" + it.Data() + "'\n";
        continue; // C++ Files only.
      }
   
    FilenameC file(dir + '/' + it.Data());
    //cerr << file << " ";
    //cerr << " (" << file << ")";
    if(!file.Exists()) {
      std::cerr <<"WARNING: No access to file:" << file << "\n";
      continue; // No access to file.
    }
    TextFileC hdrfile(file); 
    hdrfile.SetReadonly(false);
    
    if(!hdrfile.IsLoaded()) {
      std::cerr << "ERROR: Failed to load file '" << file << "'\n";
      continue;
    }
    
    // Update variable
    IntT lines =  DoSubst(hdrfile); 
    
    // Update repository
    
    if(hdrfile.IsModified()) {
      if(!dryRun)
	cerr << "Updating " << file << "\t" << lines << " occurences" <<endl;
      else
	cerr << "Updating " << file << " (dryrun)" << "\t" << lines << " occurences"<< std::endl;
      if(!dryRun) {
	if(!hdrfile.Save())
	  std::cerr << "Failed to save file '" << file << "' \n"; 
      }
    }
  }
  return true;
}

//: Subsitute value.
/*
  bool SubstsC::DoSubst(TextFileC &buff) {
  // FIXME - Var could turn out to be a prefix of something else ??
  IntT atline = buff.FindLineMatch("//! " + label);
  if(atline < 0)
  return false; // Not found...
  if(buff[atline].gsub(original,newun) > 0)
  buff.SetModified();
  return true;
  }
*/
/*
  IntT SubstsC::DoSubst(TextFileC &buff) {
  int count = 0 ; 
  // FIXME - Var could turn out to be a prefix of something else ??
  IntT atline = 0;
  while (true){
  atline = buff.FindLineMatch("//! " + label, atline+1 ) ;
  std::cerr << "\n atline :" << atline ;
  if ( atline < 0 ) break ;
  if(buff[atline].gsub(original,newun) > 0)
  {  buff.SetModified()  ; ++ count ; }
  }
  return count ; 
  }
*/


//: Do the substitution in the text file
IntT SubstsC::DoSubst(TextFileC &buff) 
{
  int count = 0 ; 
  // FIXME - Var could turn out to be a prefix of something else ??
  const StringC expression( "//! " + label) ; 
  for ( DLIterC<TextFileLineC> it (buff.Lines() ) ; it ; it ++ ) {
    IntT index = it.Data().Text().index(expression);
    
    if ( index < 0 ) 
      continue ; // move on to next line if not found.
    // otherwise replace it. 
    
    if ( it.Data().Text().gsub(original, newun) > 0 ){ 
      buff.SetModified() ; 
      ++ count ; 
    } 
  }
  return count ;
}



int doHeaders(int nargs,char **argv) {
  OptionC option(nargs,argv,true);
  FilenameC fn = option.CharArr("i",".","Input filename. ");
  //bool rec = option.Boolean("r",true,"recurse into subdirectories. ");
  bool all = option.Boolean("a",false,"Go into inactive directories as well. ");
  bool verb = option.Boolean("v",false,"Verbose mode.");
  bool dryRun = option.Boolean("d",false,"Do a dry run.");
  bool doHeaders = option.Boolean("h", true, "Process Headers") ;
  bool doMains = option.Boolean("m", false, "Process Mains" ) ;  
  bool doTests = option.Boolean("t", false, "Process test programs") ; 
  bool doExamples = option.Boolean("e", false, "Process examples") ;

  StringC label = option.CharArr("l","docentry","Label to change.");
  StringC oValue = option.CharArr("lo","","Original value. ");
  StringC nValue = option.CharArr("ln","","New value ");
  option.Check();
  
  SubstsC op(label,oValue,nValue,verb,dryRun,doHeaders, doMains, doTests, doExamples);
  
  SourceCodeManagerC chkit(fn);
  if(verb)
    chkit.SetVerbose(true);
  
  chkit.ForAllDirs(Trigger(op,&SubstsC::Process,StringC(""),DefsMkFileC("")),all); 
  
  return 0;
}

RAVL_ENTRY_POINT(doHeaders);



