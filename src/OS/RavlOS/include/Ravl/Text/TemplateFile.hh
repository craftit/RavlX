// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVLTEMPLATEFILE_HEADER
#define RAVLTEMPLATEFILE_HEADER 1
///////////////////////////////////////////////////////////////
//! rcsid="$Id$"
//! example=exTemplateFile.cc
//! file="Ravl/OS/Text/TemplateFile.hh"
//! lib=RavlOS
//! author="Charles Galambos"
//! date="04/03/99"
//! docentry="Ravl.API.OS.Text Processing"

#include "Ravl/Text/TextFile.hh"
#include "Ravl/Text/TextCursor.hh"
#include "Ravl/Stream.hh"
#include "Ravl/RCHash.hh"

namespace RavlN {
  
  //! userlevel=Normal
  //: Subsitute variables in a template file.
  // This is a small object. <p>
  // Variables in the input are of the form $(tag) where
  // tag is used to determine what should be placed in
  // the output file. If you wish to avoid a substitution use
  // $$(tag) in the template file, it becomes $(tag) in the 
  // output file
  
  class TemplateFileC {
  public:
    TemplateFileC(const StringC &templName,const OStreamC &anOut);
    //: Constructor.
    
    TemplateFileC(const TextFileC &templFile,const OStreamC &anOut);
    //: Constructor.
    
    TemplateFileC(const TextFileC &templFile,const OStreamC &anOut,const RCHashC<StringC,StringC> &tab);
    //: Constructor.
    
    bool Build(bool userWillHandleExceptions = false);
    //: Build a template output file based on those variables
    //: setup with 'SetVar'. 
    // This function is NOT to be used with 'First','Next' and 'IsElm'.
    // Returns true if completed without error, otherwise false.
    
    bool IsElm() const { return at.IsElm(); }
    //: Still more to do ?
    // NB. Unlike other iterators First() must be called before
    // IsElm() returns a valid result.
    
    bool First();
    //: Start reading template file.
    // If returns false, everything is finished already.
    
    StringC &Next();
    //: Goto next insertion point.
    // returns name of insertion point.
    // Empty string if done. <p>
    
    StringC &Data() { return point; }
    //: Returns name of current insertion point.
    
    OStreamC &Output() { return out; }
    //: Where is the output going ?
    
    void SetVar(const StringC &id,const StringC &value) 
      { vars[id.Copy()] = value.Copy(); }
    //: Set the value of a variable.
    
    void ErrorLabel();
    //: Print an error message about the label.
    
  protected:
    StringC ClipVar(TextCursorC &at);
    //: Clip out variable.
    
    OStreamC out;
    RCHashC<StringC,StringC> vars;
    TextCursorC at;
    StringC point;
  };
}

#endif
