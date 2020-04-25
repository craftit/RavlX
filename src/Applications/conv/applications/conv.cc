// This file is part of RAVL, Recognition And Vision Library
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
///////////////////////////////////////////////////////////
//! rcsid="$Id$"
//! file="Ravl/Applications/FileConverter/conv.cc"
//! docentry="Ravl.Applications.Video"
//! author="Charles Galambos"
//! userlevel=Normal

//: File/Sequence convertion.
//
//This program converts between Ravl fileformats. To list the
//known file formats use the '-lf' option, to see a list of known
//conversions use '-lc', and see a list of known classes use '-lt.'

#include "Ravl/Option.hh"
#include "Ravl/DP/FileFormatIO.hh"
#include "Ravl/DP/SequenceIO.hh"
#include "Ravl/DP/TypeInfo.hh"
#include "Ravl/TypeName.hh"
#include "Ravl/EntryPnt.hh"
#include "Ravl/DP/FileFormat.hh"
#include "Ravl/HashIter.hh"
#include "Ravl/EntryPnt.hh"
#include "Ravl/Exception.hh"
#include "Ravl/OS/Filename.hh"
#include "Ravl/Graph.hh"
#include "Ravl/RCWrap.hh"
#include "Ravl/DP/SPort.hh"
#include "Ravl/DP/FileFormatRegistry.hh"
#include "Ravl/DP/TypeConverter.hh"
#include "Ravl/DP/PrintIOInfo.hh"

//#include "Ravl/Image/ImgIO.hh"
//#include "Ravl/Image/VidIO.hh"
//#include "Ravl/DP/GeomIO.hh"

using namespace RavlN;

int DoIdent(FilenameC fn); // Identify file.

int FileConv(int argc,char **argv)
{
  // Parse options.

  OptionC   option(argc,argv);
  StringC   inType  = option.String("it","","Input file type. ");
  StringC   outType = option.String("ot","","Output file type. ");
  StringC   classType = option.String("ct","void","Intermediate class type. (Default: use most efficient route).  E.g.\n"
    "      conv -s 000_1_3.avi @X -v -ct \"ImageC<ByteRGBValueC>\"");
  bool  seq     = option.Boolean("s",false,"Convert a sequence of objects. (The default will convert only 1) ");
  bool  verb    = option.Boolean("v",false,"Verbose. ");
  bool  test    = option.Boolean("t",false,"Test if conversion is possible. (Sequence only) ");
  bool  listForm = option.Boolean("lf",false,"List all known file formats.");
  bool  listConv = option.Boolean("lc",false,"List all known conversions.");
  bool  listTypes = option.Boolean("lt",false,"List all known classes.");
  bool  ident    = option.Boolean("i",false,"Identify file. ");
  bool use32BitMode = option.Boolean("32",false,"Force the use 32 bit mode. ");
  bool use64BitMode = option.Boolean("64",false,"Force the use 64 bit mode. ");
  bool usecompression = option.Boolean("usecomp",false,"Use compression with -comp arg.");
  IntT      noFrames = option.Int("len",-1,"Length of sequence. ");
  IntT      NoFrames = option.Int("nf",-1,"No. of frames in sequence.  -1 -> do all frames");
  IntT      startFrame = option.Int("sf",-1,"Where to start in sequence.-1=Default. ");
  IntT      everyNth = option.Int("nth",1,"Frequency of frames to copy. 1=Every frame. ");
  IntT  compression = option.Int("comp",31,"Level of compression 1 - 31 with 1(high) 31(low).");
  FilenameC inFile  = option.String("","in.pgm","Input filename");
  FilenameC outFile = option.String("","out.pgm","Output filename");

  option.If(seq,"len","a sequence is being processed. (-s) ");
  option.If(seq,"st","a sequence is being processed. (-s) ");
  option.If(seq,"nth","a sequence is being processed. (-s) ");
  option.DependXor("len nf");
  option.DependXor("32 64");
  option.Comment("NOTE: The automatic conversion path in this program isn't as clever as it could be, you may need to specify an intermediate type with the -ct for the program to work. ");
  option.Check();
  if (option.IsOnCommandLine("nf")) noFrames = NoFrames;

  // Do stuff for info options.

  if(listForm)
    PrintIOFormats(cout);
  if(listConv)
    PrintIOConversions(cout);
  if(listTypes)
    PrintIOClassTypes(cout);
  if(listConv || listForm || listTypes)
    return 0; // If asked for format info, leave now.

  if(use32BitMode)
    SetCompatibilityMode32Bit(true);
  if(use64BitMode)
    SetCompatibilityMode32Bit(false);
  if(verb) {
    if(DefaultToCompatibilityMode32Bit()) {
      std::cout << "In 32 bit binary file mode. \n";
    } else {
      std::cout << "In 64 bit binary file mode. \n";
    }
  }
  // Identify what type a file is.

  if(ident)
    return DoIdent(inFile);

  // Do Copy.

  RCWrapAbstractC dataHandle;

  // Find intermidiate type.  typeid(void) == Any.

  const type_info &iotype = RTypeInfo(classType);
  if(iotype == typeid(void) && classType != "void") {
    cerr << "ERROR: Class '" << classType << "' not known. \n";
    return 1;
  }

  if(!seq) {
    // ---------------- Convert single file. ------------------

    // Open IO.
    DPIPortBaseC ip = BaseLoad(inFile,inType,iotype,verb);
    if(!ip.IsValid()) {
      cerr << "ERROR: Failed to open file for load '" << inFile << "' \n";
      return 1;
    }
    DPOPortBaseC op = BaseSave(outFile,outType,ip.InputType(),verb);
    if(!op.IsValid()) {
      cerr << "ERROR: Failed to open file for save '" << outFile << "' \n";
      return 1;
    }
    // Sort out intermediate type.
    DPTypeInfoC typeInfo(TypeInfo(ip.InputType()));
    if(!typeInfo.IsValid()) {
      cerr << "ERROR: Don't know how to copy type '" << TypeName(iotype) << "' \n";
      return 1;
    }
    if(test) {
      cerr << "Conversion possible. \n";
      return 0;
    }
    // Do the actual copy.
    dataHandle = typeInfo.Get(ip);
    if(!dataHandle.IsValid()) {
      cerr << "ERROR: Read failed. \n";
      return 1;
    }
    if(!typeInfo.Put(op,dataHandle)) {
      cerr << "ERROR: Write failed. \n";
      return 1;
    }
  } else {
    // ------------------ Convert sequence. ---------------------
    // Setup IO.

    DPIPortBaseC ip;
    DPSeekCtrlC isc; // Input seek control.
    if(!OpenISequenceBase(ip,isc,inFile,inType,iotype,verb)) {
      cerr << "ERROR: Failed to open input sequence '" << inFile << "' \n";
      return 1;
    }
    if(startFrame >= 0) {
      if(!isc.IsValid())
	cerr << "WARNING: Input stream not seekable. \n";
      else {
	if(!isc.Seek(startFrame))
	  cerr << "WARNING: Input stream seek failed. \n";
      }
    }
    DPOPortBaseC op;
    DPSeekCtrlC osc; // Output seek control.
    if(!OpenOSequenceBase(op,osc,outFile,outType,ip.InputType(),verb)) {
      cerr << "ERROR: Failed to open output sequence '" << outFile << "' \n";
      return 1;
    }
    if(compression < 1 || compression > 31 ) {
        cerr << "Error compression has to be in the range 1 to 31." << "\n";
        return 1;
    }
    if(usecompression == true) {
       op.SetAttr("compression",compression);
    }
    // Get type to use in copy.

    DPTypeInfoC typeInfo(TypeInfo(ip.InputType()));
    if(!typeInfo.IsValid()) {
      cerr << "ERROR: Don't know how to copy type '" << TypeName(iotype) << "' \n";
      return 1;
    }
    if(test) {
      cerr << "Conversion possible. \n";
      return 0;
    }
    // If in verbose mode tell the user a bit about whats going on.
    if(verb) {
      if(isc.IsValid()) {
	unsigned int size = isc.Size();
	cerr << "Sequence length : ";
	if(size == ((UIntT) -1))
	  cerr << "Unknown";
	else
	  cerr << size;
	cerr << "\n";
      }
      cerr << "Starting conversion:" << flush;
    }
    // Copy sequence.
    int i = 0;
    if(everyNth < 1)  everyNth = 1;
    try {
      for(;noFrames != 0;) {
	if(noFrames > 0)
	  noFrames--;
	if(verb)
	  cerr << "." << flush;
	if(!ip.IsGetReady())
	  break;
	dataHandle = typeInfo.Get(ip);
	if(!dataHandle.IsValid())
	  break;
	if((i++ % everyNth) == 0) { // Do we want every frame ?
	  if(!typeInfo.Put(op,dataHandle)) {
	    cerr << "WARNING: Write failed. Terminating copy. \n";
	    return 1;
	  }
	}
      }
      if(verb)
	cerr << "\n";
    } catch(DataNotReadyC &dnr) {
      if(verb)
	cerr << "\n";
      if(noFrames >= 0)
	cerr << "WARNING: Premature end of input. \n";
    }
  }
  // Done.
  if(verb)
    cerr << "Done.\n";
  return 0;
}

RAVL_ENTRY_POINT(FileConv);

// Identify file.

int DoIdent(FilenameC fn)
{
  if(!fn.Exists()) {
    cerr << "Can't find file '" << fn << "' \n";
    return 1;
  }
  if(!fn.IsReadable()) {
    cerr << "Can't read file '" << fn << "' \n";
    return 1;
  }
  FileFormatBaseC ff(Identify(fn));
  if(!ff.IsValid()) {
    cerr << "Failed to identify file '" << fn << "'\n";
    return 1;;
  }
  cout << fn << " = " << ff.Name() << ",  Default RAVL load type:" << TypeName(ff.DefaultType()) << "\n";
  return 0;
}
