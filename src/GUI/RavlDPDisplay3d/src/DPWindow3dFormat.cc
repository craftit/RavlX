// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2008, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
/////////////////////////////////////////////////////////
//! rcsid="$Id$"
//! lib=RavlDPDisplay3d
//! file="Ravl/GUI/DPDisplay/DPWindow3dFormat.cc"

#include "Ravl/GUI/DPWindow3dOPort.hh"
#include "Ravl/Threads/RWLock.hh"
#include "Ravl/TypeName.hh"
#include "Ravl/Hash.hh"
#include "Ravl/GUI/DPWindow3dFormat.hh"

#define DODEBUG 0
#if DODEBUG
#define ONDEBUG(x) x
#else
#define ONDEBUG(x)
#endif

namespace RavlGUIN {
  
  static HashC<StringC,DPWindow3dC> g_windows; // List of windows open.
  static RWLockC g_windowsLock;
  
  
  //: Default constructor.
  
  DPWindow3dFormatBodyC::DPWindow3dFormatBodyC()
    : FileFormatBodyC("gui3d","Display 3d data in a window. (@GL)")
  {}
  
  //: Probe for Save.
  
  const type_info &DPWindow3dFormatBodyC::ProbeSave(const StringC &filename,const type_info &obj_type,bool forceFormat) const {
    ONDEBUG(cerr << "DPWindow3dFormatBodyC::ProbeSave(), Called. Filename=" << filename << " obj_type=" << TypeName(obj_type) << " ForceFormat=" << forceFormat << "\n");
    if(forceFormat)
      return typeid(DObject3DC);
    if(filename.IsEmpty())
      return typeid(void);
    if(filename[0] != '@')
      return typeid(void);
    StringC device = ExtractDevice(filename);
    if(device != "GL" && device != "GLA" && device != "GLR")
      return typeid(void);
    ONDEBUG(cerr << "DPWindow3dFormatBodyC::ProbeSave(), Target accepted. \n"); 
    return typeid(DObject3DC);
  }
  
  //: Create a output port for saving.
  // Will create an Invalid port if not supported.
  
  DPOPortBaseC DPWindow3dFormatBodyC::CreateOutput(const StringC &filename,const type_info &obj_type) const {
    ONDEBUG(cerr << "DPWindow3dFormatBodyC::CreateOutput(), Called. Filename=" << filename << " obj_type=" << TypeName(obj_type) << " \n");
    if(obj_type != typeid(DObject3DC))
      return DPOPortBaseC();
    StringC winName = ExtractParams(filename);
    StringC device = ExtractDevice(filename);
    
    // See if the window already exists.
    DPWindow3dC win;
    RWLockHoldC hold(g_windowsLock,RWLOCK_WRITE);
    if(!g_windows.Lookup(winName,win)) {
      win = DPWindow3dC(winName);
      g_windows[winName] = win;
    }
    hold.Unlock();
    
    // Setup a new port to the window and return it.
    DPWindow3dOPortC port;
    bool accum = false;
    bool resetWin = false;
    if(device == "GLR")
      resetWin = true;
    else if(device == "GLA") 
      accum = true;
    else {
      RavlAssert(device == "GL");
    }
    return DPWindow3dOPortC(win,accum,resetWin);
  }
  
  DPWindow3dFormatC initDPWindow3dFormat;
  
  void InitDPWindow3dFormat()
  {}
  
 
}
