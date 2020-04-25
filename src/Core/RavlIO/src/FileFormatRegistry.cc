// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
///////////////////////////////////////////////////////
//! lib=RavlIO
//! file="Ravl/Core/IO/FileFormatRegistry.cc"

#include "Ravl/DP/FileFormatRegistry.hh"

#include "Ravl/TypeName.hh"
#include "Ravl/StringList.hh"
#include "Ravl/HSet.hh"
#include "Ravl/DP/FileFormatDesc.hh"
#include "Ravl/MTLocks.hh"
#include "Ravl/SysLog.hh"
#include "Ravl/StdMath.hh"

#define DODEBUG 0
#if DODEBUG
#define ONDEBUG(x) x
#else
#define ONDEBUG(x)
#endif

namespace RavlN {
 
  //: Default constructor.
  
  FileFormatRegistryBodyC::FileFormatRegistryBodyC() 
#if RAVL_USE_IO_AUTO_TYPECONVERTER
    : typeConverter(SystemTypeConverter())
#endif
  {}
  
#if RAVL_USE_IO_AUTO_TYPECONVERTER
  //: Constructor.
  
  FileFormatRegistryBodyC::FileFormatRegistryBodyC(const TypeConverterC &aTypeConveter)
    : typeConverter(aTypeConveter)
  {}
#endif
  
  //: Add a new format to the registery.
  
  bool FileFormatRegistryBodyC::Insert(FileFormatBaseC &ff) {
    MTWriteLockC writeLock(6);
    if(ff.Name() != "") {
      FormatByName()[ff.Name()].InsLast(ff);
      ONDEBUG(std::cout << "Registering file format '" << ff.Name() << "'  : " << ff.Description() << "\n");
    }
    Formats().InsLast(ff);
    return true;
  }
  
  //: Remove an old format from the registry.
  
  bool FileFormatRegistryBodyC::Remove(FileFormatBaseC &ff) {
    RavlAlwaysAssert(0); // Not implemented.
    return false;
  }
  
  /////////////////////////////////////
  //: Find a format.
  
  FileFormatBaseC FileFormatRegistryBodyC::FindFormat(bool forLoad,
						      const StringC &name,
						      const std::type_info &objtype,
						      bool useIndirect
						      )
  {
    MTReadLockC readLock(6);
    if(name == "") { // Request for default format ?
      for(DLIterC<FileFormatBaseC> it(Formats());
	  it.IsElm();
	  it.Next()) {
	if(it.Data().DefaultType() == objtype)
	  return it.Data();
      }
      cerr << "No default file format for: '" << TypeName(objtype) << "'" << endl;
      return FileFormatBaseC(); // No default type found.
    }
    if(!FormatByName().IsElm(name)) {
      cerr << "Unrecognised file format: '" << name << "' "<< endl;
      return FileFormatBaseC(); // Unknown format.
    }
    for(DLIterC<FileFormatBaseC> it(FormatByName()[name]);
	it.IsElm();
	it.Next()) {
      if(forLoad) {
	if(it.Data().ProbeLoad(objtype) != typeid(void))
	  return it.Data();
      } else {
	if(it.Data().ProbeSave(objtype) != typeid(void))
	  return it.Data();
      }
    }
    StringC ls;
    if(forLoad)
      ls = "load";
    else ls = "save";
    cerr << "Don't know how to " << ls << " '" << TypeName(objtype) << "' in format '" << name << "'" << endl;
    return FileFormatBaseC();
  }
  
  /////////////////////////////////////
  //: Find the format of the file.
  
  FileFormatBaseC FileFormatRegistryBodyC::FindFormatFile(bool forLoad,
							  const StringC &filename,
							  const std::type_info &objtype,
							  StringC formName,
							  bool useIndirect
							  )
  {
    MTReadLockC readLock(6);
    if(formName == "") {
      for(DLIterC<FileFormatBaseC> it(Formats());
	  it.IsElm();
	  it.Next()) {
	if(objtype == typeid(void)) {
	  if(forLoad) {
	    if(it.Data().ProbeLoad(filename,it.Data().DefaultType()) == typeid(void))
	      continue;
	  } else {
	    if(it.Data().ProbeSave(filename,it.Data().DefaultType(),false) == typeid(void))
	      continue;
	  }
	} else {
	  if(forLoad) {
	    if(it.Data().ProbeLoad(filename,objtype) == typeid(void))
	      continue;
	  } else {
	    if(it.Data().ProbeSave(filename,objtype,false) == typeid(void))
	      continue;
	  }
	}
	return it.Data();
      }
      if(objtype != typeid(void)) // If a type is specified and we couldn't convert to it.
	cerr << "Unknown file format: '" << filename << "' (Requested type: '" << TypeName(objtype.name()) << ")" << endl;
      return FileFormatBaseC(); // No default type found.
    }
    if(!FormatByName().IsElm(formName)) {
      cerr << "Unrecognised file format: '" << formName << "'" << endl;
      return FileFormatBaseC(); // Unknown format.
    }
    for(DLIterC<FileFormatBaseC> it(FormatByName()[formName]);
	it.IsElm();
	it.Next()) {
      if(objtype == typeid(void)) {
	if(forLoad) {
	  if(it.Data().ProbeLoad(filename,it.Data().DefaultType()) == typeid(void))
	    continue;
	} else {
	  if(it.Data().ProbeSave(filename,it.Data().DefaultType(),true) == typeid(void))
	    continue;
	}
      } else {
	if(forLoad) {
	  if(it.Data().ProbeLoad(filename,objtype) == typeid(void))
	    continue;
	} else {
	  if(it.Data().ProbeSave(filename,objtype,true) == typeid(void))
	    continue;
	}
      }
    return it.Data();
    }
    cerr << "Can't read '" << filename << "' from '" << formName << "' into '" << TypeName(objtype) << "'" << endl;  
    return FileFormatBaseC();
  }
  
  ///////////////////////////////////
  //: Find the format of the file.
  
  FileFormatBaseC FileFormatRegistryBodyC::FindFormat(IStreamC &in,
						      const std::type_info &objtype,
						      StringC formName,
						      bool useIndirect
						      )
  {
    MTReadLockC readLock(6);
    if(formName == "") {
      for(DLIterC<FileFormatBaseC> it(Formats());
	  it.IsElm();
	  it.Next()) {
	ONDEBUG(cout << "FindFormat() " << it.Data().Name() << " : " << TypeName(it.Data().DefaultType()) << endl);
	if(objtype == typeid(void)) {
	  if(it.Data().ProbeLoad(in,it.Data().DefaultType()) == typeid(void))
	    continue;
	} else {
	  if(it.Data().ProbeLoad(in,objtype) == typeid(void))
	    continue;
	}
	return it.Data();
      }
      std::cerr << "No file format for: 'IStreamC' to " << TypeName(objtype) << endl;
      return FileFormatBaseC(); // No default type found.
    }
    if(!FormatByName().IsElm(formName)) {
      //std::cerr << "Unrecognised file format: " << formName << endl;
      RavlError("Unrecognised format '%s' ",formName.c_str());
      return FileFormatBaseC(); // Unknown format.
    }
    for(DLIterC<FileFormatBaseC> it(FormatByName()[formName]);
	it.IsElm();
	it.Next()) {
      if(objtype == typeid(void)) {
	if(it.Data().ProbeLoad(in,it.Data().DefaultType()) == typeid(void))
	continue;
      } else {
	if(it.Data().ProbeLoad(in,objtype) == typeid(void))
	  continue;
      }
      return it.Data();
    }
    //std::cerr << "Can't read stream in format " << formName << " into " << TypeName(objtype) <<endl;
    RavlError("Can't read stream in format '%s' into '%s' ",formName.c_str(),TypeName(objtype));
    return FileFormatBaseC();
  }
  
  static bool ParseFmts(StringC &fmts,
			HSetC<StringC> &ignoreFmts,
			HSetC<StringC> &acceptFmts)
  {
    StringListC aLst(fmts,", \n\t\0");
    for(DLIterC<StringC> it(aLst);it.IsElm();it.Next()) {
      if(it.Data()[0] == '!') {
	ignoreFmts += it.Data().after(0);
	ONDEBUG(cerr << "Ignoreing: " << it.Data().after(0) << "\n");
      } else 
	acceptFmts += it.Data();
    } 
    return acceptFmts.IsEmpty();
  }
  
  //: Search for input format.
  
  bool FileFormatRegistryBodyC::FindInputFormat(FileFormatDescC &fmtInfo,
						StringC filename,
						IStreamC &in,
						StringC format,
						const std::type_info &obj_type,
						bool verbose
						)
  {
    MTReadLockC readLock(6);

    ONDEBUG(cerr << "FindInputFormat(), Fn:'" << filename << "' Format:'" << format << "'  Loading into type : " << TypeName(obj_type) << "  Verb:" << verbose << "\n");

    if(filename.length() == 0) {
      cerr<< "FileFormatRegistryBodyC::FindInputFormat(), Filename too short. \n";
      return false; 
    }
    
    // Open file to start probe..
    
    in = IStreamC(filename,true); // Open in binary mode, particularly for windows.
    if(in.IsEndOfStream() && filename[0] != '@') {
      cerr << "Can't open '" << filename << "', empty or invalid file.\n";
      return false;
    }
    if(!in.good() && filename[0] != '@') {
      cerr << "Can't open file '" << filename << "'\n";
      return false;
    }
    FileFormatBaseC bestFormat;
    FileFormatBaseC aUsableFormat; // Remember a usable format even if conversion fails for error reporting.
    const std::type_info *usableType = 0;
    DListC<DPConverterBaseC> bestConverterList;
    RealT bestConversionCost = 100000;
    const std::type_info *bestFormatType = 0;
    HSetC<StringC> ignoreFmts,acceptFmts;
    bool acceptAll = ParseFmts(format,ignoreFmts,acceptFmts);
    
    RavlAssert(in.good() || filename[0] == '@');
    
    // Should look for best ??
    for(DLIterC<FileFormatBaseC> it(Formats());
        it.IsElm();
        it.Next()) {
      // Format specified ?
      if(!ignoreFmts.IsEmpty() && ignoreFmts.IsMember(it.Data().Name()))
        continue;
      if(!acceptAll) {
        if(!acceptFmts.IsMember(it.Data().Name()))
          continue;
      }
      // Look for all loaders that can load from stream.
      const std::type_info *ti;
      if(filename != "-")
        ti = &it.Data().ProbeLoad(filename,in,obj_type); // Use file probe.
      else
        ti = &it.Data().ProbeLoad(in,obj_type); // Use stream probe...
      ONDEBUG(cerr << "IProbe '" << it.Data().Name() << "' '" << TypeName(it.Data().DefaultType()) << "' = '" << TypeName(*ti) << "' priority(" << it.Data().Priority() << ")\n");
#if RAVL_CHECK
      if(!in.good() && filename[0] != '@') {
        cerr << "FindInputFormat(), IProbe left bad stream '" << it.Data().Name() << "' '" << TypeName(it.Data().DefaultType()) << "'  = " << TypeName(*ti) << "\n";
      }
#endif
      RavlAssert(in.good() || filename[0] == '@');
      //ONDEBUG(cerr << "Tell:" << in.Tell() << ") \n");
      if(*ti == typeid(void))
        continue; // Nope.
      aUsableFormat = *it; // Remember a workable format in case we need it for error reporting later.
      usableType = ti;
      if(*ti == obj_type || obj_type == typeid(void)) {
        // Found direct load, store it if it's the highest priority!
        if (bestConversionCost > 0 || (it.Data().Priority() > bestFormat.Priority()))
        {
          ONDEBUG(cerr << "IProbe storing '" << it.Data().Name() << "' '" << TypeName(it.Data().DefaultType()) << "'  = " << TypeName(*ti) << " priority(" << it.Data().Priority() << ")\n");
          bestFormat = it.Data();
          bestConverterList = DListC<DPConverterBaseC>();
          bestConversionCost = 0;
          bestFormatType = ti;
        }
        continue;
      }

#if RAVL_USE_IO_AUTO_TYPECONVERTER
      if (bestConversionCost == 0)
        continue;

      RealT conversionCost = -1;
      DListC<DPConverterBaseC> converterList(typeConverter.FindConversion(*ti,obj_type,  conversionCost));
      if (converterList.Size() == 0)
        continue;

      if (conversionCost < bestConversionCost || (conversionCost == bestConversionCost && bestFormat.IsValid() && it.Data().Priority() > bestFormat.Priority()))
      {
        bestFormat = it.Data();
        bestConverterList = converterList;
        bestConversionCost = conversionCost;
        bestFormatType = ti;
      }
#endif
    }

    if(!bestFormat.IsValid()) {
      ONDEBUG(cerr << "FindInputFormat(StringC), Can't identify stream. \n");
      if(verbose ONDEBUG(|| 1)) {
        if(aUsableFormat.IsValid()) {
          std::cerr << "Identified file format as " << aUsableFormat.Name() << " but could not convert result from " << RavlN::TypeName(*usableType) << " to requested type " << RavlN::TypeName(obj_type) << "\n";
        } else
          cerr << "Can't find format for '" << filename <<"'\n";
      }
      return false;
    }
    if(verbose ONDEBUG(|| 1)) {
      std::cerr << "Loading object '" << TypeName(obj_type) << "' in format '" << bestFormat.Name() << "' from file '" << filename << "' Steps:" << bestConverterList.Size()<< "\n";
      for(DLIterC<DPConverterBaseC> it(bestConverterList);it.IsElm();it.Next())
        std::cerr << " Via:" <<TypeName(it.Data().ArgType(0)) << " -> " << TypeName(it.Data().Output()) << std::endl;
    }
    fmtInfo = FileFormatDescC(bestFormat,bestConverterList,*bestFormatType,true);
    return true;
  }
  
  ///////////////////////////////
  //: Create an input pipe.
  
  DPIPortBaseC FileFormatRegistryBodyC::CreateInput(StringC filename,
						    StringC format,
						    const std::type_info &obj_type,
						    bool verbose
						    )
  {
    FileFormatDescC fmtInfo;
    IStreamC inStream;
    if(!FindInputFormat(fmtInfo,filename,inStream,format,obj_type,verbose)) {
      if(verbose ONDEBUG(|| 1)) 
	cerr << "CreateInput(), Can't find format for file '" << filename << "'. \n"; 
      return DPIPortBaseC();
    }
    DPIPortBaseC ret = fmtInfo.CreateInput(filename,inStream);
    if(!ret.IsValid())
      cerr << "CreateInput(), Failed to create input stream for '" << filename << "'\n";
    return ret;
  }
  
  //: Search for output format.
  
  bool FileFormatRegistryBodyC::FindOutputFormat(FileFormatDescC &fmtInfo,
						 StringC filename,
						 StringC format,
						 const std::type_info &obj_type,
						 bool verbose
						 ) {
    //ONDEBUG(cerr << "FindOutputFormat(), Fn:'" << filename << "' Format:'" << format << "'  Type : " << TypeName(obj_type) << "  Verb:" << verbose << "\n");
    ONDEBUG(RavlDebug("FindOutputFormat Filename='%s' Format='%s' Type='%s' Verbose=%d",filename.c_str(),format.c_str(),TypeName(obj_type),verbose));
    MTReadLockC readLock(6);
    // Find format thats least effort to convert to.
    RealT minCost = 100000;
    FileFormatBaseC minForm;
    DListC<DPConverterBaseC> bestConv;
    const std::type_info *bestout = 0;
    IntT bestPri = -100;
    
    HSetC<StringC> ignoreFmts,acceptFmts;
    bool acceptAll = ParseFmts(format,ignoreFmts,acceptFmts);
    const std::type_info *testType = &obj_type;
    bool foundAnyFmt = false;
    for(DLIterC<FileFormatBaseC> it(Formats());
	it.IsElm();
	it.Next()) {
      if(ignoreFmts.IsMember(it.Data().Name()))
	continue;
      if(!acceptAll) {
	if(!acceptFmts.IsMember(it.Data().Name()))
	continue;
      }
      foundAnyFmt = true;
      if(obj_type == typeid(void))
	testType = &(it.Data().DefaultType());
      const std::type_info &ti = it.Data().ProbeSave(filename,*testType,!acceptAll); // it.Data().DefaultType()
      //ONDEBUG(cerr << "OProbe '" << it.Data().Name() << "' '" << TypeName(it.Data().DefaultType()) << "'  = " << TypeName(ti) << "\n");
      ONDEBUG(RavlDebug("OProbe Format:%s  Default type:%s Suggest:%s  ",it.Data().Name().c_str(),TypeName(it.Data().DefaultType()),TypeName(ti)));
      if(ti == typeid(void))
	continue;
      if(ti == obj_type || obj_type == typeid(void)) {
	if(minCost > 0 || it.Data().Priority() > bestPri) {
	  ONDEBUG(RavlDebug("Conversion for %s priority %d (Best so far:%d) Cost:%f ",it.Data().Name().c_str(),it.Data().Priority(),bestPri,minCost));
	  minForm = it.Data();
	  bestConv = DListC<DPConverterBaseC>();
	  minCost = 0;
	  bestPri = it.Data().Priority();
	  bestout = &ti;
	}
	continue;
      }
#if RAVL_USE_IO_AUTO_TYPECONVERTER
      if(minCost == 0) {
	continue; // Already found a format needing no conversion !
      }
      RealT acost = 1000;
      DListC<DPConverterBaseC> aConv(typeConverter.FindConversion(obj_type,ti,acost));
      RavlAssert(!IsNan(acost));
      if(aConv.Size() == 0)
	continue; // Can't convert !
      ONDEBUG(RavlDebug("Conversion cost %f (Best so far:%f) ",acost,minCost));
      if(acost < minCost || (acost == minCost && it.Data().Priority() > bestPri)) {
	bestConv = aConv;
	minForm = it.Data();
	minCost = acost;
	bestout = &ti;
	bestPri = it.Data().Priority();
      }
#endif
    }
    
    if(!foundAnyFmt && !format.IsEmpty()) {
      RavlInfo("No format found matching '%s' ",format.c_str());
    }
    if(!minForm.IsValid()) {
      if(verbose ONDEBUG(|| 1))
	RavlWarning("Don't know how to save to file '%s' in format '%s' from type '%s' ",filename.c_str(),format.c_str(),TypeName(obj_type));
      return false;
    }  
    if(verbose ONDEBUG(|| 1)) {
      //std::cerr << "Saving object '" << TypeName(obj_type) << "' in format '" << minForm.Name() << "' with type '" << TypeName(*bestout) << "' to file '" << filename << "' Steps:" << bestConv.Size() << " Priority:" << bestPri <<  " \n";
      RavlInfo("Saving object '%s' in format '%s' with type '%s' to file '%s' in %d steps with priority:%d cost:%f ",
          TypeName(obj_type),minForm.Name().c_str(),TypeName(*bestout),filename.c_str(),(int) bestConv.Size().V(),bestPri,minCost);
      for(RavlN::DLIterC<DPConverterBaseC> it(bestConv);it.IsElm();it++) {
        if(!it.IsLast())
          RavlInfo(" Via %s ",RavlN::TypeName(it->Output()));
      }
    }
    
    fmtInfo = FileFormatDescC(minForm,bestConv,*bestout,false);
    return true;
  }
  
  ///////////////////////////////
  //: Create an output pipe.

  DPOPortBaseC FileFormatRegistryBodyC::CreateOutput(StringC filename,
						     StringC format,
						     const std::type_info &obj_type,
						     bool verbose
						     ) {
    FileFormatDescC fmtInfo;
    if(!FindOutputFormat(fmtInfo,filename,format,obj_type,verbose))
      return DPOPortBaseC();
    DPOPortBaseC ret = fmtInfo.CreateOutput(filename); 
    if(!ret.IsValid())
      cerr << "CreateInput(), Failed to create output stream for '" << filename << "'\n";
    return ret;
  }
  
  ///////////////////////////////
  //: Create an input pipe.
  
  DPIPortBaseC FileFormatRegistryBodyC::CreateInput(IStreamC &in,
						    StringC format,
						    const std::type_info &obj_type,
						    bool verbose
						    )
  {
    MTReadLockC readLock(6);
    FileFormatBaseC bestFormat;

    // Should look for best ??
    DListC<DPConverterBaseC> bestConverterList;
    const std::type_info *bestFormatType = 0;
    RealT bestConversionCost = 100000;
    
    for(DLIterC<FileFormatBaseC> it(Formats());
	it.IsElm();
	it.Next()) {
      if(!format.IsEmpty()) {
	if(format != it.Data().Name())
	  continue;
      }
      // Look for all loaders that can load from stream.

      const std::type_info *reqType = 0;
      if(obj_type != typeid(void))
	reqType = &obj_type;
      else
	reqType = &it.Data().DefaultType();
      const std::type_info &ti = it.Data().ProbeLoad(in,*reqType);
      
      if(ti == typeid(void))
	continue; // Can't load give input.
      if(ti == obj_type || obj_type == typeid (void)) 
      { // Found direct load, store it if it's the highest priority!
        if (bestConversionCost > 0 || (it.Data().Priority() > bestFormat.Priority()))
        {  ONDEBUG(cerr << "IProbe storing '" << it.Data().Name() << "' '" << TypeName(it.Data().DefaultType()) << "'  = " << TypeName(ti) << " priority(" << it.Data().Priority() << ")\n");
           bestFormat = it.Data();
           bestConverterList = DListC<DPConverterBaseC>();
           bestConversionCost = 0;
           bestFormatType = &ti;
        }
        continue;
      }

#if RAVL_USE_IO_AUTO_TYPECONVERTER
      if (bestConversionCost == 0)
        continue;

      // Can we convert to requested type ?
      RealT conversionCost = -1;
      DListC<DPConverterBaseC> converterList(typeConverter.FindConversion(ti, obj_type, conversionCost));
      if (converterList.Size() == 0)
        continue;

      if (conversionCost < bestConversionCost || (conversionCost == bestConversionCost && bestFormat.IsValid() && it.Data().Priority() > bestFormat.Priority()))
      {
        bestFormat = it.Data();
        bestConverterList = converterList;
        bestConversionCost = conversionCost;
        bestFormatType = &ti;
      }
#endif
    }
    if(!bestFormat.IsValid()) {
      std::istream &istrm = in.is(); // Use via reference to avoid clang warnings.
      RavlError("CreateInput(StreamC), Can't load stream in format '%s' for type '%s',  stream name '%s' buff:%s ",
          format.c_str(),TypeName(obj_type),in.Name().c_str(),TypeName(typeid(istrm)));
      if(verbose ONDEBUG(|| 1)) 
        RavlWarning("CreateInput(StreamC), Can't identify format. ");
      return DPIPortBaseC();
    }
    if(verbose ONDEBUG(|| 1)) {
      //cerr << "Loading object '" << TypeName(obj_type) << "' in format '" << bestFormat.Name() << "' from stream in " << bestConverterList.Size() << " steps.\n";
      RavlInfo("Loading object '%s' in format '%s' from stream in %d steps.",TypeName(obj_type),bestFormat.Name().c_str(),(int) bestConverterList.Size());
    }
    
    //: Build conversion stream.
    DPIPortBaseC inp = bestFormat.CreateInput(in,*bestFormatType);
    if(!inp.IsValid()) {
      RavlError("Internal error: Failed to open input stream in format '%s' ",bestFormat.Name().c_str());
      //cerr << "Internal error: Failed to open input stream in format '" << bestFormat.Name() << "' \n" ;
      RavlAssert(0);
      return DPIPortBaseC();
    }
    for(DLIterC<DPConverterBaseC> it(bestConverterList);it.IsElm();it.Next()) {
      inp = it.Data().CreateIStream(inp);
      RavlAssert(inp.IsValid());
    }
    return inp;  
  }
  
  DPOPortBaseC FileFormatRegistryBodyC::CreateOutput(OStreamC &to,
						     StringC format,
						     const std::type_info &obj_type,
						     bool verbose
						     )
  {
    if(obj_type == typeid(void)) {
      ONDEBUG(cerr << "CreateOutput(OStreamC), Asked to output void. \n");
      return DPOPortBaseC();
    }
    MTReadLockC readLock(6);
  // Find format thats least effort to convert to.
    RealT minCost = 100000;
    FileFormatBaseC minForm;
    DListC<DPConverterBaseC> bestConv;
    IntT bestPri = 0;
    const std::type_info *bestout = 0;
    bool forceFormat = (format != "");
    for(DLIterC<FileFormatBaseC> it(Formats());
	it.IsElm();
	it.Next()) {
      if(forceFormat) {
	if(format != it.Data().Name())
	  continue;
      }
      const std::type_info &ti = it.Data().ProbeSave(obj_type);//it.Data().DefaultType());
      if(ti == typeid(void))
	continue;
      if(ti == obj_type) {
	if(minCost > 0 || it.Data().Priority() > bestPri) {
	  minForm = it.Data();
	  bestConv = DListC<DPConverterBaseC>();
	  minCost = 0;
	  bestPri = it.Data().Priority();
	  bestout = &ti;
	}
      }
#if RAVL_USE_IO_AUTO_TYPECONVERTER
      if(minCost == 0)
	continue; // Conversion less format already found.
      RealT acost = -1;
      DListC<DPConverterBaseC> aConv(typeConverter.FindConversion(obj_type,ti,acost));
      if(aConv.Size() == 0)
	continue; // Can't convert !
      if(acost < minCost || (acost == minCost && it.Data().Priority() > bestPri)) {
	bestConv = aConv;
	minForm = it.Data();
	minCost = acost;
	bestPri = it.Data().Priority();
	bestout = &ti;
      }
#endif
    }
    
    if(!minForm.IsValid()) {
      if(verbose ONDEBUG(|| 1)) {
        //cerr << "WARNING: Don't know how to save to stream in format '" << format << "' from type '" << TypeName(obj_type) << "' \n";
	RavlWarning("Don't know how to save to stream in format '%s' from type '%s' ",format.c_str(),TypeName(obj_type));
      }
      return DPOPortBaseC();
    }  
    if(verbose ONDEBUG(|| 1)) {
      //cerr << "Saving object '" << TypeName(obj_type) << "' in format '" << minForm.Name() << "' to stream \n";
      RavlInfo("Saving object '%s' in format '%s' to stream ",TypeName(obj_type),minForm.Name().c_str());
    }
    
    //: Build conversion stream.
    
    DPOPortBaseC outp = minForm.CreateOutput(to,*bestout);
    if(!outp.IsValid()) {
      DPEntityBodyC &minFormBody = static_cast<DPEntityC &>(minForm).Body(); // Avoid clang warnings.
      RavlError("Internal error: Failed to open output stream in format '%s' (%s) type '%s' ",
          minForm.Name().c_str(),
          minForm.Description().c_str(),
          TypeName(typeid(minFormBody)));
      RavlAssert(0);
      return DPOPortBaseC();
    }
    for(DLIterC<DPConverterBaseC> it2(bestConv);it2.IsElm();it2.Next()) {
      outp = it2.Data().CreateOStream(outp);
      RavlAssert(outp.IsValid());
    }
    return outp;  
  }


  //: Access the system's format registry.
  
  FileFormatRegistryC &SystemFileFormatRegistry() {
    static FileFormatRegistryC fr(true);
    return fr;
  }



  /////////////////////////////////
  //! userlevel=Advanced
  //: Load to an abstract object handle.
  // NB. an instace of TypeInfoInstC must exists for the contained class if this
  // is to work.
  
  bool FileFormatRegistryBodyC::Load(const StringC &filename,RCWrapAbstractC &obj,StringC fileformat,bool verbose) {
    DPIPortBaseC port = CreateInput(filename,fileformat,typeid(void),verbose);
    ONDEBUG(cerr << "FileFormatRegistryBodyC::Load(const StringC &,const RCWrapAbstractC &), Called. \n");
    if(!port.IsValid()) 
      return false;
    DPTypeInfoC lt = DPTypeInfoBodyC::Types()[port.InputType().name()];
    if(!lt.IsValid()) {
      std::cerr << "Load(RCWrapAbstractC), No TypeInfoC class for type '" << TypeName(port.InputType()) << "', load failed. \n";
      return false;
    }
    obj = lt.Get(port);
    return true;
  }
  
  /////////////////////////////////
  //! userlevel=Advanced
  //: Save an abstract object handle.
  // NB. an instace of TypeInfoInstC must exists for the contained class if this
  // is to work.
  
  bool FileFormatRegistryBodyC::Save(const StringC &filename,const RCWrapAbstractC &obj,StringC fileformat,bool verbose) {
    if(!obj.IsValid()) {
      if(verbose ONDEBUG(|| 1))
	cerr << "Save(RCWrapAbstractC), Asked to save invalid handle to file '" + filename + "'\n";
      return false;
    }
    ONDEBUG(cerr << "FileFormatRegistryBodyC::Save(const StringC &,const RCWrapAbstractC &), Called. Type=" << TypeName(obj.DataType()) << "\n");
    DPOPortBaseC port = CreateOutput(filename,fileformat,obj.DataType(),verbose);
    if(!port.IsValid()) 
      return false; // Just plain don't know now to save.
    DPTypeInfoC lt = DPTypeInfoBodyC::Types()[obj.DataType().name()];
    if(!lt.IsValid()) {
      cerr << "Save(RCWrapAbstractC), No TypeInfoC class for type '" << TypeName(obj.DataType()) << "', save failed. \n";
      return false;
    }
    return lt.Put(port,obj);
  }
  
  //: Load to an abstract object handle.
  // NB. an instace of TypeInfoInstC must exists for the contained class if this
  // is to work.
  
  bool FileFormatRegistryBodyC::Load(IStreamC &strm,RCWrapAbstractC &obj,StringC fileformat,bool verbose) {
    ONDEBUG(cerr << "FileFormatRegistryBodyC::Load(IStreamC &,const RCWrapAbstractC &), Called. \n");
    DPIPortBaseC port = CreateInput(strm,fileformat,typeid(void),verbose);
    if(!port.IsValid()) 
      return false;
    DPTypeInfoC lt = DPTypeInfoBodyC::Types()[port.InputType().name()];
    if(!lt.IsValid()) {
      cerr << "Load(RCWrapAbstractC), No TypeInfoC class for type '" << TypeName(port.InputType()) << "', load failed. \n";
      return false;
    }
    obj = lt.Get(port);
    return true;
  }
  
  //: Save an abstract object handle.
  // NB. an instace of TypeInfoInstC must exists for the contained class if this
  // is to work.
  
  bool FileFormatRegistryBodyC::Save(OStreamC &strm,const RCWrapAbstractC &obj,StringC fileformat,bool verbose) {
    if(!obj.IsValid()) {
      if(verbose ONDEBUG(|| 1))
	cerr << "Save(RCWrapAbstractC), Asked to save invalid handle to stream\n";
      return false;
    }
    ONDEBUG(cerr << "FileFormatRegistryBodyC::Save(OStreamC &,const RCWrapAbstractC &), Called. Type=" << TypeName(obj.DataType()) << "\n");
    DPOPortBaseC port = CreateOutput(strm,fileformat,obj.DataType(),verbose);
    if(!port.IsValid()) 
      return false; // Just plain don't know now to save.
    DPTypeInfoC lt = DPTypeInfoBodyC::Types()[obj.DataType().name()];
    if(!lt.IsValid()) {
      cerr << "Save(RCWrapAbstractC), No TypeInfoC class for type '" << TypeName(obj.DataType()) << "', save failed. \n";
      return false;
    }
    return lt.Put(port,obj);
  }
  
  /////////////////////////////////
  //! userlevel=Advanced
  //: List all file formats that support the given type.
  // If typespec is void then all types are listed.
  
  DListC<FileFormatBaseC> FileFormatRegistryBodyC::ListFormats(bool forLoad,const StringC &fileFormat,const std::type_info &typespec) {
    DListC<FileFormatBaseC> ret;
    MTReadLockC readLock(6);
    bool forceFormat = (fileFormat != "");
    for(DLIterC<FileFormatBaseC> it(SystemFileFormatRegistry().Formats());
	it.IsElm();
	it.Next()) {
      if(forceFormat) {
	if(it.Data().Name() != fileFormat)
	  continue;
      }
      if(forLoad) {
	IStreamC tmp;
      if(it.Data().ProbeLoad("",tmp,typespec) != typeid(void))
	ret.InsLast(it.Data());
      } else {
	if(it.Data().ProbeSave("",typespec,forceFormat)  != typeid(void))
	  ret.InsLast(it.Data());
      }
    }
    return ret;
  }

  //////////////////////////////////
  //: Identify a file.
  // If file format is unrecognised FileFormatBaseC will be an invalid handle.
  
  FileFormatBaseC FileFormatRegistryBodyC::Identify(const StringC &afile)  {
    FileFormatBaseC ret;
    ret = FindFormatFile(true,afile); // Find a load format.
    if(!ret.IsValid()) // No, then try save.
      ret = FindFormatFile(false,afile);
    return ret;
  }
  
  
  //: Identify a stream.
  // If file format is unrecognised the returned FileFormatBaseC will be an invalid handle.
  
  FileFormatBaseC FileFormatRegistryBodyC::Identify(IStreamC &istrm) 
  { return FindFormat(istrm); }
  
}
