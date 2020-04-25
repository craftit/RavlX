// This file is part of CxxDoc, The RAVL C++ Documentation tool 
// Copyright (C) 2004, University of Surrey
// This code may be redistributed under the terms of the GNU General 
// Public License (GPL). See the gpl.licence file for details or
// see http://www.gnu.org/copyleft/gpl.html
// file-header-ends-here
///////////////////////////////////////////////////////
//! rcsid="$Id$"
//! author="Lee Gregory"
//! docentry="Ravl.API.Source Tools"
//! lib=RavlSourceTools

#include "Ravl/Option.hh"
#include "Ravl/EntryPnt.hh"
#include "Ravl/Text/TextFile.hh"
#include "Ravl/Text/TextCursor.hh"
#include "Ravl/SourceTools/DefsFile.hh"
#include "Ravl/SourceTools/SourceCodeManager.hh"
#include "Ravl/SourceTools/SourceFile.hh"
#include "Ravl/CallMethods.hh"
//#include "Ravl/Calls.hh"
#include "Ravl/OS/Filename.hh"
#include   "Ravl/Text/ConfigFile.hh"

using namespace RavlN;


//: Simple class to handle moving html and eht doc entries.
//: ------------------------------------------------------------------------------
class DocRepC 
{
public:

  
  inline DocRepC(const StringC &o,const StringC &n,bool v,bool dr)
    : original(o), newun(n), verbose(v), dryRun(dr) {}
  //: Constructor.

  bool Process(StringC &dir,DefsMkFileC &where);
  //: Process a directory.
  
  inline bool IsVerbose() const { return verbose; }
  //: In verbose mode ?
  
protected:

  StringC original; 		// pattern to replace
  StringC newun;		// pattern to replace with
  bool verbose;			// verbose ?
  bool dryRun;			// dry run ?
};


//: ------------------------------------------------------------------------------
// MAIN
//: ------------------------------------------------------------------------------
int renameDocs(int nargs,char **argv) {
 StringC usage ;
usage += "\n\n This program is designed to move html and eht files in the qmake system. " ; 
usage += "\nAs well as moving the files, the references to these files are updated in the corresponding defs.mk \n" ;
  OptionC option(nargs,argv,true);
  FilenameC fn = option.CharArr("i",".","Input filename. ");
  //bool rec = option.Boolean("r",true,"recurse into subdirectories. ");
  bool all = option.Boolean("a",false,"Go into inactive directories as well. ");
  bool verb = option.Boolean("v",false,"Verbose mode.");
  bool dryRun = option.Boolean("d",false,"Do a dry run.");
  StringC oValue = option.CharArr("s","","Search pattern");
  StringC nValue = option.CharArr("r","","Replace with");
  option.Comment(usage) ; 
  option.Compulsory("s") ; 
  option.Compulsory("r") ;   
  option.Check();
  
  SourceCodeManagerC chkit(fn);
  //TriggerC trig = Trigger( &ProccessDir, StringC(""), DefsMkFileC("")  ) ; 
DocRepC rep ( oValue, nValue, verb, dryRun ) ; 
TriggerC trig = Trigger ( rep, &DocRepC::Process, StringC(""), DefsMkFileC("") ) ;
 chkit.ForAllDirs(trig,all); 
  return 0;
}



//: ------------------------------------------------------------------------------
//: Process a directory.
//: ------------------------------------------------------------------------------
bool DocRepC::Process(StringC &dir,DefsMkFileC &where) 
{
  if(IsVerbose())
    std::cerr << "\nProcessing:\t" << dir ;

// load this defs file as text so we can edit it
TextFileC text ; 
if ( ! text.Load( where.Name() ) ) 
{ std::cerr << "\nCould not load defs file " ;  } 

// get a list of doc files that need processing
  StringListC hdrs ; 
	hdrs += (where.Value("EHT"));
	hdrs += (where.Value("HTML") ) ; 


// check length of filename is reasonable  
  for(DLIterC<StringC> it(hdrs);it.IsElm();it.Next()) {
    if(it.Data().length() <= 3) {
      std::cerr << "WARNING: Short file name detected :'" << it.Data() << "'\n";
      continue; // Far too short !
    }

//: get absolute path to filename 
    FilenameC oldFile(dir + '/' + it.Data());

//: check file exists. 
    if(!oldFile.Exists()) {
      std::cerr <<"\nWARNING: No access to file:\t" << oldFile << "\n";
      continue; // No access to file.
    }

//: change any docentry tags in the html too.
TextFileC htmlFile ;
if ( ! htmlFile.Load(oldFile) ) { std::cerr << "\nWARNING: Could not load file\t" << oldFile  ; }
else
 // iterate each line and only do a replace on lines with a docentry tag !
for ( DLIterC<TextFileLineC> iter ( htmlFile.Lines() ) ; iter ; iter ++ )
	if ( iter.Data().Text().contains("docentry") ) { // does it contain a docentry line ?
		cerr << "\nUpdating docentry tag:\t" << oldFile ; 
		if (iter.Data().Text().gsub(original,newun)) htmlFile.SetModified()  ; 
		cerr << iter.Data().Text() ; 
}

// save if modified 
//cerr << "\n just as a test \n\n\n" ; htmlFile.Dump(std::cerr) ; 
if ( ! dryRun ) 
if (htmlFile.IsModified()) { 
	if ( ! htmlFile.Save() ) { std::cerr << "\nWARNING: Could not update file:\t" << oldFile ; }}

//: Apply substition to get new file name. 
FilenameC newFile (oldFile.Copy() ) ; 
newFile.gsub(original, newun) ; 


//: do we have a new name for this doc file, if so, rename it and update the defs.mk
if ( oldFile != newFile ) {
	cerr  << "\nUpdating:\t" << oldFile.NameComponent() << " ->\t " << newFile.NameComponent() ; 
	//: rename file.
	if ( ! dryRun ) {
		if ( ! oldFile.CopyTo(newFile)  ) { std::cerr << "\nWARNING: Could not copy file: " << oldFile << " to " << newFile ; }
		else if ( ! oldFile.Remove() ) { std::cerr << "\nWARNING: Could not remove old file: " << oldFile ; }
	} 
	else std::cerr << "\t(dryRun)" ;



	//: update defs.mk 
	text.GlobalSubst( oldFile.NameComponent(), newFile.NameComponent() ) ; 
	}
} // end of loop for all html and eht files. 

//: Update the defs file, if we have modified it.
    if(text.IsModified()) {
	cerr << "\nUpdating:\t" << where.Name() ; 
      if(!dryRun) {
	if(!text.Save())
	  std::cerr << "\nWARNING Failed to save file '" << where.Name()<< "' \n";  
	} else std::cerr << "\t(dryRun ) " ; }

  return true;
}



RAVL_ENTRY_POINT(renameDocs);
