// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVL_DPFILESEQUENCE_HEADER
#define RAVL_DPFILESEQUENCE_HEADER 1
/////////////////////////////////////////////////////////
//! docentry="Ravl.API.OS.Sequence.Numbered Files"
//! rcsid="$Id$"
//! file="Ravl/OS/IO/FileSequence.hh"
//! lib=RavlOSIO
//! author="Charles Galambos"
//! date="23/03/1999"

#include "Ravl/DP/SPort.hh"
#include "Ravl/OS/Filename.hh"
#include "Ravl/DP/FileFormat.hh"
#include "Ravl/DP/FileFormatDesc.hh"

namespace RavlN {

  //! userlevel=Develop
  //: File Sequence body.
  // Designed to be a component in an input/output stream that 
  // uses seperate files.
  
  class DPFileSequenceBaseBodyC 
    : public DPSeekCtrlBodyC
  {
  public:
    DPFileSequenceBaseBodyC(StringC fn,UIntT start = ((UIntT) -1),UIntT end = ((UIntT) -1),IntT digits = -1,bool forLoad = true);
    //: Constructor.
    // If 'fn' is an empty string, ProbeFormat must be called later.
    
    DPFileSequenceBaseBodyC();
    //: Default constructor.
    // Sets file name 'noname;
    
    virtual bool Seek(UIntT off);
    //: Seek to location in stream.
    // Returns false, if seek failed. (Maybe because its
    // not implemented.)
    // if an error occurered (Seek returned False) then stream
    // position will not be changed.
    
    virtual bool DSeek(IntT off);
    //: Delta Seek, goto location relative to the current one.
    // The default behavour of this functions is :
    // Do some error checking then:
    //   Seek((UIntT)((IntT) Tell() + off));
    // if an error occurered (DSeek returned False) then stream
    // position will not be changed.
    
    virtual UIntT Tell() const; 
    //: Find current location in stream.
    // May return ((UIntT) (-1)) if not implemented.
    
    virtual UIntT Size() const; 
    //: Find the total size of the stream.
    // May return ((UIntT) (-1)) if not implemented.
    
    virtual UIntT Start() const
    { return start; }
    //: Find the offset where the stream begins, normally zero.
    
    FilenameC Filename() const;
    //: Get the filename of the current file.
    
    FilenameC Filename(IntT digs,IntT fno) const;
    //: Get the filename of the current file.
    
    bool Next() { 
      if(no > end) 
	return false; 
      no++; 
      return no <= end; 
    }
    //: Goto next.
    
    inline FilenameC NextName()  { 
      FilenameC ret = Filename();
      Next();
      return ret;
    }
    //: Get filename, and increment counter.
    
    bool IsElm() const
    { return no <= end; }
    //: Check we're at a valid place in the sequence.
    
    UIntT FrameNo() const { return no; }
    //: Get current frame no.
    
    UIntT FrameStart() const { return start; }
    //: Get start frame no.
    
    UIntT FrameEnd() const { return end; }
    //: Get final frame no.
    // ((UIntT) -1) is given if not known.
    
    bool IsOk() const { return ok; }
    //: Is the file sequence setup ok ?
    
    bool ProbeFormat(FilenameC rootFn);
    //: See if we can find the format.
    // Returns false if fail.
    
    bool HasHoles() const
    { return hasHoles; }
    //: Does sequence have holes ?
    // This will only be detected during the playing of a sequence.
    
    void HasHoles(bool ahole) 
    { hasHoles = ahole; }
    //: Does sequence have holes ?
    
  protected:
    bool ProbeExample(FilenameC rootFn);
    //: See if we can find the format from an example
    // Returns false if fail.
    
    bool ProbeTemplate(FilenameC rootFn);
    //: Is the given filename a template for the sequence ?
    // Returns false if fail.
    
    bool ProbeImplicit(FilenameC rootFn);
    //: Is there a number between the filename and the extention ?
    // Returns false if fail.
    
    IntT ProbeDigits(StringC dir,StringC prefix,StringC postfix);
    //: See if we can guess the number of digits.
    // -1=Failed.
    
    bool ProbeRange(StringC rootFn);
    //: See if we can find the start of the sequence.
    // Returns false if fail.
    
    bool forLoad; // Sequence for loading ?
    IntT digits; // Number of digits of 0 padding. 
    UIntT start; // First in sequence.
    UIntT end;   // Last in sequence.
    UIntT no;    // Current file no.
    StringC templateFile;
    StringC subst; // Number pattern
    bool ok; // File sequence setup ok ?
    
    bool hasHoles; // Are there files missing from the sequence.
  };
  
  //! userlevel=Normal
  //: File Sequence.
  
  class DPFileSequenceBaseC 
    : public DPSeekCtrlC
  {
  public:
    DPFileSequenceBaseC(StringC fn,UIntT start = ((UIntT) -1),UIntT end = ((UIntT) -1),IntT digits = -1,bool forLoad = true)
      : DPEntityC(*new DPFileSequenceBaseC(fn,start,end,digits,forLoad))
    {}
    //: Constructor.
    // If 'fn' is an empty string, ProbeFormat must be called later.
    
    DPFileSequenceBaseC()
      : DPEntityC(true)
    {}
    //: Default constructor.
    // Creates an invalid handle.
    
  protected:
    DPFileSequenceBaseBodyC &Body() 
    { return dynamic_cast<DPFileSequenceBaseBodyC &>(DPEntityC::Body()); }
    //: Access body.
    
    const DPFileSequenceBaseBodyC &Body() const
    { return dynamic_cast<const DPFileSequenceBaseBodyC &>(DPEntityC::Body()); }
    //: Access body.
    
  public:
    bool ProbeFormat(FilenameC rootFn)
    { return Body().ProbeFormat(rootFn); }
    //: See if we can find the format.
    // Returns false if fail.
    
    inline FilenameC Filename() const
    { return Body().Filename(); }
    //: Get the filename of the current file.
    
    inline UIntT FrameNo() const 
    { return Body().FrameNo(); }
    //: Get current frame no.
    
    inline UIntT FrameStart() const 
    { return Body().FrameStart(); }
    //: Get start frame no.
    
    inline UIntT FrameEnd() const 
    { return Body().FrameEnd(); }
    //: Get final frame no.
    // ((UIntT) -1) is given if not known.
    
    inline FilenameC NextName() 
    { return Body().NextName(); }
    //: Get next filename, then increment counter.
    
    bool IsElm() const
    { return Body().IsElm(); }
    //: Check we're at a valid place in the sequence.
    
    bool IsOk() const 
    { return Body().IsOk(); }
    //: Is the file sequence setup ok ?

    bool HasHoles() const
    { return Body().HasHoles(); }
    //: Does sequence have holes ?
    // This will only be detected during the playing of a sequence.
    
    void HasHoles(bool ahole)
    { Body().HasHoles(ahole); }
    //: Set flag indicating if a sequence has holes.
    
  };
  
  ///////////////////////////////////////////////////////////
  
  //! userlevel=Develop
  //: Input File Sequence
  
  class DPIFileSequenceBodyC
    : public DPFileSequenceBaseBodyC
  {
  public:
    DPIFileSequenceBodyC();
    //: Default constructor.
    
    DPIFileSequenceBodyC(StringC fn,UIntT start = ((UIntT) -1),UIntT end = ((UIntT) -1),IntT digits = -1);
    //: Constructor.
    
    DPIPortBaseC Setup(FileFormatDescC &desc);
    //: Setup for a specific file type.
    
    static bool AuxFunction(DPIPipeBaseC &auxFun,DPEntityC &hold);
    //: Auxilary processing function for pipe.
    
    FileFormatBaseC &Format() { return format; }
    //: Access files format.
    
    const type_info &LoadType() const { return *loadType; }
    //: Type of class to load.
    
  protected:
    FileFormatBaseC format;   // Format of input files.
    const type_info *loadType;// Type to load.
  };
  
  //! userlevel=Normal
  //: Input File Sequence
  // The filename can now be one of the following, where XXX is the
  // frame number in a multi-file sequence.
  // (filename)XXX.(suffix)
  // (filename).(suffix)     - The numbers will be inserted in before .(suffix)
  // or a string with '%d' put where digits should be inserted.
  
  class DPIFileSequenceC
    : public DPFileSequenceBaseC
  {
  public:
    DPIFileSequenceC()
      : DPEntityC(false)
    {}
    //: Default constructor.
    // This creates an invalid handle.
    
    DPIFileSequenceC(StringC fn,UIntT start = ((UIntT) -1),UIntT end = ((UIntT) -1),IntT digits = -1)
      : DPEntityC(*new DPIFileSequenceBodyC(fn,start,end,digits))
    {}
    //: Constructor.
    
    DPIFileSequenceC(const DPEntityC &obj)
      : DPEntityC(dynamic_cast<const DPIFileSequenceBodyC *>(BodyPtr(obj)))
    {}
    //: Create form an DPEntityC handle.
    
    DPIFileSequenceC(const DPIFileSequenceC &bod)
      : DPEntityC(bod)
    {}
    //: Body constructor.
  protected:
    
    DPIFileSequenceBodyC &Body() 
    { return dynamic_cast<DPIFileSequenceBodyC &>(DPEntityC::Body()); }
    //: Access body.
    
    const DPIFileSequenceBodyC &Body() const
    { return dynamic_cast<const DPIFileSequenceBodyC &>(DPEntityC::Body()); }
    //: Access body.
    
  public:
    
    inline DPIPortBaseC Setup(FileFormatDescC &desc)
    { return Body().Setup(desc); }
    //: Setup for a specific file type.
    
    inline FileFormatBaseC &Format() 
    { return Body().Format(); }
    //: Access files format.
    
    inline const type_info &LoadType() const 
    { return Body().LoadType(); }
    //: Type of class to load.
    
  };
  
  ///////////////////////////////////////////////////////////
  
  //! userlevel=Develop
  //: Input File Sequence
  
  class DPOFileSequenceBodyC
    : public DPFileSequenceBaseBodyC
  {
  public:
    DPOFileSequenceBodyC();
    //: Default constructor.
    
    DPOFileSequenceBodyC(StringC fn,UIntT start = ((UIntT) -1),UIntT end = ((UIntT) -1),IntT digits = -1);
    //: Constructor.
  
    DPOPortBaseC Setup(FileFormatDescC &desc);
    //: Setup for a specific file type.
    
    static bool AuxFunction(DPOPipeBaseC &auxFun,DPEntityC &hold);
    //: Auxiliary processing function for pipe.
    
    FileFormatBaseC &Format() { return format; }
    //: Access files format.
    
    const type_info &SaveType() const { return *saveType; }
    //: Type of class to load.
    
  protected:
    FileFormatBaseC format;   // Format of input files.
    const type_info *saveType;// Type to load.
  };
  
  //! userlevel=Normal
  //: Input File Sequence
  
  class DPOFileSequenceC
    : public DPFileSequenceBaseC
  {
  public:
    DPOFileSequenceC()
      : DPEntityC(false)
    {}
    //: Default constructor.
    // This creates an invalid handle.
    
    DPOFileSequenceC(StringC fn,UIntT start = ((UIntT) -1),UIntT end = ((UIntT) -1),IntT digits = -1)
      : DPEntityC(*new DPOFileSequenceBodyC(fn,start,end,digits))
    {}
    //: Constructor.
    
    DPOFileSequenceC(const DPEntityC &obj)
      : DPEntityC(dynamic_cast<const DPOFileSequenceBodyC *>(BodyPtr(obj)))
    {}
    //: Create form an DPEntityC handle.
    
    DPOFileSequenceC(const DPOFileSequenceC &bod)
      : DPEntityC(bod)
    {}
    //: Body constructor.
  protected:
    
    DPOFileSequenceBodyC &Body() 
    { return dynamic_cast<DPOFileSequenceBodyC &>(DPEntityC::Body()); }
    //: Access body.
    
    const DPOFileSequenceBodyC &Body() const
    { return dynamic_cast<const DPOFileSequenceBodyC &>(DPEntityC::Body()); }
    //: Access body.
    
  public:
    
    inline DPOPortBaseC Setup(FileFormatDescC &desc)
    { return Body().Setup(desc); }
    //: Setup for a specific file type.
    
    inline FileFormatBaseC &Format() 
    { return Body().Format(); }
    //: Access files format.
    
    inline const type_info &SaveType() const 
    { return Body().SaveType(); }
    //: Type of class to load.
    
  };
}

#endif
