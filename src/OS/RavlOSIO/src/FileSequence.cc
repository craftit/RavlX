// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
/////////////////////////////////////////////////////////
//! rcsid="$Id$"
//! lib=RavlOSIO
//! file="Ravl/OS/IO/FileSequence.cc"

#include "Ravl/DP/FileSequence.hh"
#include "Ravl/DP/TypeInfo.hh"
#include "Ravl/OS/Directory.hh"
#include "Ravl/DLIter.hh"
#include "Ravl/StdMath.hh"
#include "Ravl/TypeName.hh"

#include <stdlib.h>
#include <ctype.h>

#define DPDEBUG 0
#if DPDEBUG
#define ONDEBUG(x) x
#else
#define ONDEBUG(x)
#endif

namespace RavlN {

  //: Default constructor.
  DPIFileSequenceBodyC::DPIFileSequenceBodyC()
    : loadType(0)
  {}

  //: Constructor.
  
  DPFileSequenceBaseBodyC::DPFileSequenceBaseBodyC(StringC fn,UIntT nstart,UIntT nend,IntT ndigits,bool nforLoad)
    : forLoad(nforLoad),
      digits(ndigits), // Number of digits of 0 padding. 
      start(nstart), // First in sequence.
      end(nend),   // Last in sequence.
      no(nstart),    // Current file no.
      templateFile(fn),
      subst("%d") // Number pattern   
  {
    if(!fn.IsEmpty()) {
      if(digits == -1)
	ProbeFormat(fn);
    }
  }
  
  //: Default constructor.
  // Sets file name 'noname'
  
  DPFileSequenceBaseBodyC::DPFileSequenceBaseBodyC()
    : forLoad(false),
      digits(-1),
      start((UIntT) -1),
      end((UIntT) -1),  
      no(0),
      templateFile("noname"),
      subst("%d"),
      ok(false),
      hasHoles(false)
  {}
  
  //: Seek to location in stream.
  // Returns false, if seek failed. (Maybe because its
  // not implemented.)
  // if an error occurred (Seek returned False) then stream
  // position will not be changed.
  
  bool DPFileSequenceBaseBodyC::Seek(UIntT off) {
    if(off < start || off > end) {
      ONDEBUG(cerr << "Seek failed: to:" << off << "   Start:" << start << "  End:" << end << "\n");
      return false;
    }
    no = off;
    return true;
  }
  
  //: Delta Seek, goto location relative to the current one.
  // The default behaviour of this functions is :
  // Do some error checking then:
  //   Seek((UIntT)((IntT) Tell() + off));
  // if an error occurred (DSeek returned False) then stream
  // position will not be changed.
  
  bool DPFileSequenceBaseBodyC::DSeek(IntT doff) {
    if(doff < 0) {
      if((UIntT)(-doff) > no)
	return false;
    }
    UIntT off = (UIntT)(no + doff);
    if(off < start || off > end)
      return false;
    no = off;
    return true;
  }
  
  //: Find current location in stream.
  // May return ((UIntT) (-1)) if not implemented.
  
  UIntT DPFileSequenceBaseBodyC::Tell() const { 
    return no; 
  }
  
  //: Find the total size of the stream.
  // May return ((UIntT) (-1)) if not implemented.
  // Assume it starts from 0.
  
  UIntT DPFileSequenceBaseBodyC::Size() const {
    return end + 1;
  }
  
  
  //: Get the filename of the current file.
  
  FilenameC DPFileSequenceBaseBodyC::Filename(IntT digs,IntT fno) const {
    StringC num(fno);
    if(digs > 0) { // Zero padding ?
      StringC zeroPad("",digs+1);
      for(int i = num.length();i < digs;i++)
	zeroPad += '0';
      num = zeroPad + num;
    }
    StringC ret(templateFile.Copy());
    ret.gsub(subst,num);
    ONDEBUG(cerr << "DPFileSequenceBaseBodyC::Filename(" << digs << "," << fno << ") = '" << ret << "' \n");
    return ret;
  }
  
  //: Get the filename of the current file.
  
  FilenameC DPFileSequenceBaseBodyC::Filename() const { 
    return Filename(digits,no); 
  }
  
  
  //: See if we can find the format.
  
  bool DPFileSequenceBaseBodyC::ProbeFormat(FilenameC rootFn) {
    templateFile = rootFn;
    ONDEBUG(cerr << "DPFileSequenceBaseBodyC::ProbeFormat(), Called for '" << rootFn << "'\n");
    
    if(forLoad) {
      if(!ProbeExample(rootFn))    // Is it an example ?
	if(!ProbeTemplate(rootFn))     // Is it a template filename ?
	  if(!ProbeImplicit(rootFn)) // Is it an implicit number before extention ?
	    return false;
    } else {
      if(!ProbeTemplate(rootFn))     // Is it a template filename ?
	if(!ProbeExample(rootFn))    // Is it an example ?
	  if(!ProbeImplicit(rootFn)) // Is it an implicit number before extention ?
	    return false;
    }
    
    // Sort out sequence range.
    if(forLoad) {
      ONDEBUG(cerr << "DPFileSequenceBaseBodyC::ProbeFormat(): Pre-Range Start: " << start << " End:" << end << " \n");
      if(!ProbeRange(rootFn))
	return false; // Failed to find start of sequence.
      ONDEBUG(cerr << "DPFileSequenceBaseBodyC::ProbeFormat(): Range, Start: " << start << " End:" << end << " \n");
    } else {
      if(start == ((UIntT) -1))
	start = 0;
    }
    
    // Sort out inital position ...
    if(no == ((UIntT) -1))
      no = start;
    
    ONDEBUG(cerr << "DPFileSequenceBaseBodyC::ProbeFormat(), File Sequence found for '" << rootFn << "'. Digits:" << digits << "\n");
    return true;
  }
  
  //: Is there a number between the filename and the extension ?
  // Returns false if fail.
  
  bool DPFileSequenceBaseBodyC::ProbeImplicit(FilenameC rootFn) {
    ONDEBUG(cerr << "DPFileSequenceBaseBodyC::ProbeImplicit('" << rootFn << "') \n");
    // Try and find a sequence.
    FilenameC nameComp = rootFn.NameComponent();
    FilenameC pathComp = rootFn.PathComponent();
    if(pathComp == "")
      pathComp = StringC('.');
    StringC prefix; 
    StringC postfix;
    // Look for extension.
    // Build template filename.
    IntT extStart = nameComp.index('.');
    if(extStart >= 0) { // Found a '.' ?
      postfix = nameComp.from(extStart);
      prefix = nameComp.before(extStart);
      ONDEBUG(cerr << "DPFileSequenceBaseBodyC::ProbeImplicit(), "<< StringC(extStart) << " Pre:'" << prefix <<"'  Post:'" << postfix << "'\n");
      templateFile = pathComp + RavlN::filenameSeperator + prefix + "%d" + postfix;
    } else
      templateFile = rootFn + "%d";
    // Find number of digits...
    if(digits == -1) 
      digits = ProbeDigits(rootFn.PathComponent(),prefix,postfix);
    // Only worked if we found the number of digits..
    ONDEBUG(cerr << "DPFileSequenceBaseBodyC::ProbeImplicit(), Digits=" << digits << "  0=Failed. \n");
    return digits >= 0;
  }
  
  //: Is the given filename a template for the sequence ?
  // Returns false if fail.
  
  bool DPFileSequenceBaseBodyC::ProbeTemplate(FilenameC rootFn) {
    ONDEBUG(cerr << "DPFileSequenceBaseBodyC::ProbeTemplate('" << rootFn << "') \n");
    int at = 0;
    int done = 0;
    StringC prefix;
    
    //: Extract template prefix, postfix and number of digits.
    for(;(UIntT) at < rootFn.length();at++) {
      if((at = rootFn.index('%')) < 0)
	return false; // No template character.
      at++;
      if((UIntT) at >= rootFn.length())
	return false; // Not found.
      if(rootFn[at] != '%')  // Stuffed ?
	break;
      prefix += rootFn.at(done,at - done);
      at++;
      done = at;
    }
    if((UIntT) at >= rootFn.length()) {
      ONDEBUG(cerr << "DPFileSequenceBaseBodyC::ProbeTemplate(), Got to end without find subst char. \n");
      return false; // Got to end without finding template.
    }
    prefix += rootFn.at(done,(at - done)-1);
    //: Do we have a digit spec ?
    int ds = at;
    for(;(UIntT) at < rootFn.length();at++) {
      if(!isdigit(rootFn[at]))
	break;
    }
    int nodigits = -1;
    if(at > ds) // Found digit spec, this overides given one.
      nodigits = atoi(StringC(rootFn.at(ds,at-ds)));
    
    //: Do we have a closing 'd' ?
    if(rootFn[at] != 'd') {
      ONDEBUG(cerr << "DPFileSequenceBaseBodyC::ProbeTemplate(), Not closing 'd' ! \n");
      return false; // Doesn't look like a template !
    }
    
    //: Post fix
    StringC postfix = rootFn.after(at);
    
    ONDEBUG(cerr << "DPFileSequenceBaseBodyC::ProbeTemplate(), Template found. Pre:'" << prefix << "' Post:" << postfix << "'. \n");
    
    if(digits == -1 && nodigits < 0) {
      FilenameC tmp(prefix);
      digits = ProbeDigits(tmp.PathComponent(),tmp.NameComponent(),postfix);
      if(digits == -1) {
	ONDEBUG(cerr << "DPFileSequenceBaseBodyC::ProbeTemplate(), Couldn't stablish no of digits.\n");
	return false; // Failed to find sequence.
      }
    }
    
    if(nodigits >= 0)
      digits = nodigits;
    
    //: Found, so sort out information needed for filename !  
    templateFile = prefix + "%d" + postfix;
    subst = "%d";
    
    ONDEBUG(cerr << "DPFileSequenceBaseBodyC::ProbeTemplate(), Found. \n");
    return true;
  }
  
  //: See if we can find the format from an example
  // Returns false if fail.
  
  bool DPFileSequenceBaseBodyC::ProbeExample(FilenameC rootFn) {
    ONDEBUG(cerr << "DPFileSequenceBaseBodyC::ProbeExample('" << rootFn << "') \n");
    if(forLoad) {
      if(!rootFn.Exists()) // Does the root file name exist ?
	return false;
    }
    int i;
    // Find the last digit...
    bool digits_set = false;
    if(digits == -1)
      digits_set = true;
    for(i = rootFn.length()-1;i >= 0;i--) {
      if(isdigit(rootFn[i]))
	break;
    }
    if(i < 0) { // No digits ?? 
      ONDEBUG(cerr << "No digits found. Not an example. \n");
      return false;// Then its not an example.
    }
    
    int lastd = i;
    // Find the first digit...
    for(;i >= 0;i--) {
      if(!isdigit(rootFn[i]))
	break;
    }
    i++;
    if(rootFn[i] == '0' && (lastd != i)) // Got a leading 0 ? 
      digits = (lastd - i)+1; // Calc required number of digits...
    else
      digits = 0;
    IntT tmpStart = atoi(rootFn.at(i,digits).chars());
    if(start == ((UIntT) -1))
      start = tmpStart;
    if(no == ((UIntT) -1))
      no = tmpStart;
    templateFile = rootFn.before(i) + "%d" + rootFn.after(lastd);
    // Well lets try an experiment.
    if(forLoad) {
      if(!Filename(digits,no+1).Exists()) {
	if(digits_set)
	  digits = -1; // Scrub failed hypothesis.
	ONDEBUG(cerr << "Failed to load example. Rejecting. \n");
	return false;
      }
    }
    ONDEBUG(cerr << "DPFileSequenceBaseBodyC::ProbeExample(), File Sequence found for '" << rootFn << "'. Template='" << templateFile << "' Digits=" << digits << " \n");
    if(start == ((UIntT) -1)) {
      // Test if the example is the start..
      if(!forLoad || !Filename(digits,no-1).Exists()) 
	start = no;  // Yep it was.
    }
    ONDEBUG(cerr << "DPFileSequenceBaseBodyC::ProbeExample(), Found. \n");
    return true; // It worked !
  }
  
  //: See if we can guess the number of digits.
  // -1=Failed.
  
  IntT DPFileSequenceBaseBodyC::ProbeDigits(StringC dirName,StringC prefix,StringC postfix) {
    if(dirName == "")
      dirName = ".";
    ONDEBUG(cerr << "DPFileSequenceBaseBodyC::ProbeDigits(), Called for dir '" << dirName << "'\n");
    if(!forLoad) {
      if(end != ((UIntT) -1)) 
	return (IntT) log10((double) end) + 1; // Number of digits in final number.
      return 0;
    }
    DirectoryC dir(dirName);
    IntT len = -1;
    bool dif = false;
    DListC<StringC> files = dir.List(prefix,postfix);
    for(DLIterC<StringC> it(files);it.IsElm();it.Next()) {
      // Monitor length;
      IntT nlen = it.Data().length(); 
      if(nlen != len) { // Are there matching files with different lengths ?
	if(len != -1) {
	  dif = true;
	  break;
	}
	len = nlen;
      }
    }
    if(len == -1) {
      ONDEBUG(cerr << "DPFileSequenceBaseBodyC::ProbeDigits() No files found. Pre:'" << prefix << "' Post:'" << postfix << "'\n");
      return -1;
    }
    if(dif) {
      ONDEBUG(cerr << "DPFileSequenceBaseBodyC::ProbeDigits(), Failed. No matching files. \n");
      return 0;
    }
    IntT dig = len - (prefix.length() + postfix.length());
    if(dig == 0) {
      ONDEBUG(cerr << "DPFileSequenceBaseBodyC::ProbeDigits(), Failed. No files with digits found. \n");
      return -1;
    }
    ONDEBUG(cerr << "DPFileSequenceBaseBodyC::ProbeDigits() Found, Len:" << len <<" Pre:" << prefix.length() << " Post:" << postfix.length() << "\n");
    return dig;
  }
  
  //: See if we can find the length of the sequence.
  // Returns false if fail.
  
  bool DPFileSequenceBaseBodyC::ProbeRange(StringC rootFn) {
    // Check the obvious starting points...
    if(start == ((UIntT)-1)) {
      if(Filename(digits,0).Exists()) 
	start = 0;
      else {
	if(Filename(digits,1).Exists()) 
	  start = 1;
      }
    }
    if(start != ((UIntT)-1) && (end != ((UIntT)-1)))
      return true; // Done !
    
    // Well that wasn't enought, so try something clever.
    // Get a directory listing...
    
    FilenameC tf(templateFile);
    DirectoryC dir = tf.PathComponent();
    if(dir.IsEmpty())
      dir = StringC(".");
    FilenameC baseName = tf.NameComponent();
    StringC prefix = baseName.before(subst);
    StringC postfix = baseName.after(subst);
    DListC<StringC> files = dir.List(prefix,postfix);  
    
    ONDEBUG(cerr << "DPFileSequenceBaseBodyC::ProbeRange(),  Matching files:" << files.Size() << "\n");
    // Got anything ?
    if(files.IsEmpty())
      return (start != ((UIntT)-1)); // If we found the start, good enough.
    
    // if we know the start, try next simplist thing.
    // just add the number of files found to the start index,
    // to give the end.
    // NB. This may not work if we got stray files, which have
    // the same prefix and postfix.
    
    if(start != ((UIntT)-1)) {
      UIntT len = files.Size();
      int tend = start + len;
      // if this is right tend-1 should exist, but tend not.
      if(Filename(digits,tend-1).Exists() && !Filename(digits,tend).Exists()) {
	end = tend-1;
	return true; // Done !!
      }
    }
    
    ONDEBUG(cerr << "DPFileSequenceBaseBodyC::ProbeRange(),  Filter \n");
    
    // Ok, next try extracting the largest and smallest number directly
    // Only use files matching exactly postfix + number + prefix.
    
    const int spre = prefix.Size();
    const int spost = postfix.Size();
    int min = ((1<<((sizeof(int)*8)-2))-1) * 2+1;
    int max = 0;
    for(DLIterC<StringC> it(files);it.IsElm();it.Next()) {
      // Check num is only digits.
      const int len = it.Data().length();
      const int numend = (len - spost);
      if(digits > 0) {
	if(len != (spre + digits + spost))
	  continue; // Only accept names with the right number of digits.
      }
      ONDEBUG(cerr << "DPFileSequenceBaseBodyC::ProbeRange(),  Checking file: '" << it.Data() << "' \n");
      bool ok = true;
      for(int i = spre;i < numend;i++) {
	if(!isdigit(it.Data()[i])) {
	  ok = false;  // Reject
	  break;
	}
      }
      if(!ok)
	continue;
      int num = atoi(StringC(it.Data().at(spre,numend - spre)));
      ONDEBUG(cerr << "DPFileSequenceBaseBodyC::ProbeRange(),  Num: " << num << " \n");
      if(num > max)
	max = num;
      if(num < min)
	min = num;
    }
    ONDEBUG(cerr << "DPFileSequenceBaseBodyC::ProbeRange(),  Min:" <<  min << " Max:" << max << "\n");
    
    if(start == ((UIntT)-1))
      start = min;
    if(end == ((UIntT)-1))
      end = max;
    
    return (start != ((UIntT)-1)); // If we found the start, good enough.
  }
  
  ////////////// DPIFileSequenceBodyC //////////////////////////////////////////////////


  //: Constructor.
  
  DPIFileSequenceBodyC::DPIFileSequenceBodyC(StringC fn,
					     UIntT start,
					     UIntT end,
					     IntT digits
					     )
    : DPFileSequenceBaseBodyC(fn,start,end,digits,true),
      loadType(&typeid(void))
  {}

  //: Setup for a specific file type.
  
  DPIPortBaseC DPIFileSequenceBodyC::Setup(FileFormatDescC &desc) {
    format = desc.Format();
    loadType = &desc.SourceType();
    DPTypeInfoC ti = TypeInfo(desc.SourceType());
    if(!ti.IsValid()) {
      cerr << "DPIFileSequenceBodyC::DPIFileSequenceBodyC(), Failed to find type information.... \n";
      return DPIPortBaseC(); // Failed to find typeinformation...
    }
    DPIFileSequenceC fseq(*this);
    DPIPipeBaseC fin(ti.CreateIPipe(fseq));
    RavlAssert(fin.IsValid());
    fin.SetStreamStatus(true,true);
    fin.SetAuxOperation(AuxFunction);
    return fin.Port();
  }
  
  //: Auxilary processing function for pipe.
  
  bool DPIFileSequenceBodyC::AuxFunction(DPIPipeBaseC &auxFun,DPEntityC &hold) {
    DPIFileSequenceC fs(hold);
    //ONDEBUG(cerr << "DPIFileSequenceBodyC::AuxFunction(), Called. File:'" << fs.Filename() << "'  Fmt:" << fs.Format().Name() << "\n");
    RavlAssert(fs.IsValid());
    
    // Check for end of stream...
    DPIPortBaseC ipb;
    do {
      if(!fs.IsElm()) {
	auxFun.SetStreamStatus(false,false);
	auxFun.SetInput(DPIPortBaseC());
	return true; // Its normal to get to the end of a sequence.
      }
      FilenameC nextName = fs.NextName();
      if(!nextName.Exists()) {
	if(!fs.HasHoles()) {
	  cerr << "WARNING: File sequence is incomplete, missing '" << nextName << "'. \n";
	  cerr << " Further missing file on this sequence will be silently skipped. \n";
	  fs.HasHoles(true);
	}
	continue;
      }
      ipb = fs.Format().CreateInput(nextName,fs.LoadType());
      if(!ipb.IsValid()) {
	cerr << "DPIFileSequenceBodyC::AuxFunction(), WARNING: Failed to create input '" << nextName << "' of type " << TypeName(fs.LoadType()) << " \n";
	return false;
      }
    } while(0);
    
    //ONDEBUG(cerr << " TN:" << TypeName(typeid(auxFun.DPEntityC::Body())) << " CI:" << TypeName(typeid(ipb.DPEntityC::Body())) << "\n");
    bool ret = auxFun.SetInput(ipb);
    //ONDEBUG(cerr << "DPIFileSequenceBodyC::AuxFunction(), Done.. \n");
    return ret;
  }
  
  //// DPOFileSequenceBodyC /////////////////////////////////////////////////////////////////////////////////

  //: Default constructor.

  DPOFileSequenceBodyC::DPOFileSequenceBodyC()
    : saveType(0)
  {}

  //: Constructor.
  
  DPOFileSequenceBodyC::DPOFileSequenceBodyC(StringC fn,
					     UIntT start,
					     UIntT end,
					     IntT digits
					     )
    : DPFileSequenceBaseBodyC(fn,start,end,digits,false),
      saveType(&typeid(void))
  {}

  //: Setup for a specific file type.
  
  DPOPortBaseC DPOFileSequenceBodyC::Setup(FileFormatDescC &desc) {
    format = desc.Format();
    saveType = &desc.SourceType();
    DPTypeInfoC ti = TypeInfo(desc.SourceType());
    if(!ti.IsValid()) {
      cerr << "DPOFileSequenceBodyC::DPOFileSequenceBodyC(), Failed to find type information.... \n";
      return DPOPortBaseC(); // Failed to find typeinformation...
    }
    DPOFileSequenceC fseq(*this);
    DPOPipeBaseC fin(ti.CreateOPipe(fseq));
    RavlAssert(fin.IsValid());
    fin.SetStreamStatus(true,true);
    fin.SetAuxOperation(AuxFunction);
    return fin.Port();
  }
  
  //: Auxilary processing function for pipe.
  
  bool DPOFileSequenceBodyC::AuxFunction(DPOPipeBaseC &auxFun,DPEntityC &hold) {
    DPOFileSequenceC fs(hold);
    //ONDEBUG(cerr << "DPOFileSequenceBodyC::AuxFunction(), Called. File:'" << fs.Filename() << "'  Fmt:" << fs.Format().Name() << "\n");
    RavlAssert(fs.IsValid());
    FilenameC nextName;
    
    // Check for end of stream...
    if(!fs.IsElm()) {
      auxFun.SetStreamStatus(false,false);
      auxFun.SetOutput(DPOPortBaseC());
      return true; // Its normal to get to the end of a sequence.
    }
    nextName = fs.NextName();
    
    DPOPortBaseC ipb = fs.Format().CreateOutput(nextName,fs.SaveType());
    if(!ipb.IsValid()) {
      cerr << "DPOFileSequenceBodyC::AuxFunction(), Failed to create output '" << nextName << "' of type " << TypeName(fs.SaveType()) << " \n";
      return false; 
    }
    //ONDEBUG(cerr << " TN:" << TypeName(typeid(auxFun.DPEntityC::Body())) << " CI:" << TypeName(typeid(ipb.DPEntityC::Body())) << "\n");
    bool ret = auxFun.SetOutput(ipb);
    //ONDEBUG(cerr << "DPOFileSequenceBodyC::AuxFunction(), Done.. \n");
    return ret;
  }
  
}
