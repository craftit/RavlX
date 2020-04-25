// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//////////////////////////////////////////////////////////////
//! lib=RavlOSIO
//! file="Ravl/OS/IO/SequenceIO.cc"
//! author="Charles Galambos"
//! docentry="Ravl.OS.Sequence"
//! userlevel=Normal

#include "Ravl/DP/SequenceIO.hh"
#include "Ravl/DP/FileSequence.hh"
#include "Ravl/DP/FileFormatIO.hh"
#include "Ravl/DP/FileFormat.hh"
#include "Ravl/DP/FileFormatDesc.hh"
#include "Ravl/DP/SPortAttach.hh"
#include "Ravl/DP/FileFormatRegistry.hh"
#include "Ravl/SysLog.hh"

#define DPDEBUG 0
#if DPDEBUG
#include "Ravl/TypeName.hh"
#define ONDEBUG(x) x
#else
#define ONDEBUG(x)
#endif

namespace RavlN {
  
  extern URLMapperFuncT urlMapper;

  //: Open input stream base.
  
  bool OpenISequenceBase(DPIPortBaseC &ip,DPSeekCtrlC &sc,const StringC &fn,const StringC &fileformat,const type_info &obj_type,bool verbose) {   
    FileFormatDescC  fmtInfo;
    FilenameC ifilename;
    
    if(urlMapper != 0)
      ifilename = urlMapper(fn);
    else ifilename = fn;
    if(ifilename.IsEmpty()) {
      RavlError("Can't open an empty filename. ");
      return false;
    }
    DPIFileSequenceC fileSeq;
    sc.Invalidate();
    IStreamC inStream;
    if(ifilename.Exists() || (ifilename == "-") || (ifilename[0] == '@')) { // Does the requested file exists ?
      // If requested file exists, it might be a single file sequence...
      if(!SystemFileFormatRegistry().FindInputFormat(fmtInfo,ifilename,inStream,fileformat,obj_type,verbose)) {
	ONDEBUG(cerr << "OpenISequenceBase(), Failed to find format for '" << fn << "' \n");
	return false; // Failed to find format.
      }
      ONDEBUG(cerr << "OpenISequenceBase(), Format found for '" << fn << "' = '" << fmtInfo.Format().Name() << "' IsStream:" << fmtInfo.Format().IsStream() << "\n");
      if(fmtInfo.Format().IsStream() || fn == "-") { // Is stream already ?
	if(verbose)
	  fmtInfo.DumpConv(cerr);
	ip = fmtInfo.CreateInput(ifilename,inStream,sc);
	ONDEBUG(cerr << "OpenISequenceBase(), Stream is a sequence. sc=" << sc.IsValid() << " \n");
	return ip.IsValid();
      }
      // Not a single file, so try a sequence.
      fileSeq = DPIFileSequenceC(StringC("")); 
      if(!fileSeq.ProbeFormat(ifilename)) { 
	// If its not a sequence then just return stream 'as is'.
	ip = fmtInfo.CreateInput(ifilename,inStream);
	if(!ip.IsValid()) 
	  return false;
	sc = DPSeekCtrlC(true); // Put in a dummy seek control.
	ONDEBUG(cerr << "OpenISequenceBase(), Stream not a sequence. \n");
	return true;
      }
    } else {
      // If requested file does not exists, it MUST be a file sequence, or a mistake.
      ONDEBUG(cerr << "OpenISequenceBase(), File '" << ifilename << "' doesn't exist. Exist=" << ifilename.Exists() << "\n");
      fileSeq = DPIFileSequenceC(ifilename);
      if(!fileSeq.Filename().Exists()) {
	if(verbose) 
	  cerr << "Can't find file sequence of with base name '" << ifilename << "' \n";
	return false; // Obviously failed to find sequence pattern.
      }
      if(!SystemFileFormatRegistry().FindInputFormat(fmtInfo,fileSeq.Filename(),inStream,fileformat,obj_type,verbose))
	return false; // Failed to find format.
    }
    ONDEBUG(cerr << "OpenISequenceBase(), Building input stream.... \n");
    
    DPIPortBaseC ipipe(fileSeq.Setup(fmtInfo));
    // Try a file sequence.
    if(!ipipe.IsValid()) {
      cerr << "OpenISequenceBase(), Failed to setup file sequence... \n";
      return false;
    }
    if(verbose)
      fmtInfo.DumpConv(cerr);
    ip = fmtInfo.BuildInputConv(ipipe); 
    sc = fileSeq;   // So caller can attach seekable port into stream.
    ONDEBUG(cerr << "OpenISequenceBase(), Building stream done. Final type:" << TypeName(ip.InputType()) << " \n");
    return ip.IsValid(); // Did if all work ?
  }
  
  //: Open output stream base.
  
  bool OpenOSequenceBase(DPOPortBaseC &op,DPSeekCtrlC &sc,const StringC &fn,const StringC &fileformat,const type_info &obj_type,bool verbose) { 
    sc.Invalidate();
    FilenameC filename;
    if(urlMapper != 0)
      filename = urlMapper(fn);
    else filename = fn;

    FileFormatDescC  fmtInfo;
    if(!SystemFileFormatRegistry().FindOutputFormat(fmtInfo,filename,fileformat,obj_type,verbose)) {
      if(verbose)
	cerr << "Can't find output format for '" << fn <<"' \n";
      ONDEBUG(cerr << "OpenOSequenceBase(), Failed to find format for '" << fn << "' \n");
      return false; // Failed to find format.
    }
    if(fmtInfo.Format().IsStream() || fn == "-" || fn[0] == '@') { // Is stream already ?
      op = fmtInfo.CreateOutput(filename,sc);
      if(!op.IsValid()) {
	if(verbose)
	  cerr << "Failed to create output pipe for '" << fn <<"' in format '" << fmtInfo.Format().Name() << "' \n";
	return false;
      }
      return true;
    }
    // Use a file sequence.
    DPOFileSequenceC fileSeq(filename);
    DPOPortBaseC opipe(fileSeq.Setup(fmtInfo));
    if(!opipe.IsValid()) {
      cerr << "OpenOSequenceBase(), Failed to setup file sequence... \n";
      return false;
    }
    if(verbose)
      fmtInfo.DumpConv(cerr);
    op = fmtInfo.BuildOutputConv(opipe); 
    sc = fileSeq;   // So caller can attach seekable port into stream.
    ONDEBUG(cerr << "OpenOSequenceBase(), Building stream done. Final type:" << TypeName(op.OutputType()) << " \n");
    return op.IsValid();
  }
}
  

