// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2003, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
/////////////////////////////////////////////////////////
//! rcsid="$Id$"
//! lib=RavlDPGraph
//! file="Ravl/Contrib/Plotting/DPGraph/DPGraphWindowFormat.cc"

#include "Ravl/Plot/DPGraphWindowFormat.hh"
#include "Ravl/Plot/DPGraphWindowOPort.hh"
#include "Ravl/Threads/RWLock.hh"
#include "Ravl/TypeName.hh"
#include "Ravl/Hash.hh"

#define DODEBUG 0
#if DODEBUG
#define ONDEBUG(x) x
#else
#define ONDEBUG(x)
#endif

namespace RavlPlotN {
  
  void InitDPGraphWindowFormat()
  {}
  
  static TypeNameC type1(typeid(Array1dC<RealT>),"Array1dC<RealT>");
  
  static HashC<StringC,DPGraphWindowC> windows; // List of windows open.
  static RWLockC windowsLock;
  
  
  //: Default constructor.
  
  DPGraphWindowFormatBodyC::DPGraphWindowFormatBodyC()
    : FileFormatBodyC("gui","Plot data in a window. (@GRAPH) ")
  {}
    
  //: Probe for Save.
  
  const type_info &DPGraphWindowFormatBodyC::ProbeSave(const StringC &filename,const type_info &obj_type,bool forceFormat) const {
    ONDEBUG(cerr << "DPGraphWindowFormatBodyC::ProbeSave(), Called. Filename=" << filename << " obj_type=" << TypeName(obj_type) << " ForceFormat=" << forceFormat << "\n");
    if(forceFormat)
      return typeid(Array1dC<Point2dC>);
    if(filename.IsEmpty() || filename[0] != '@')
      return typeid(void);
    StringC device = ExtractDevice(filename);
    if(device != "GRAPH" && device != "GRAPHA")
      return typeid(void);
    if(obj_type == typeid(Array1dC<RealT>))
      return typeid(Array1dC<RealT>);
    return typeid(Array1dC<Point2dC>);
  }
  
  //: Create a output port for saving.
  // Will create an Invalid port if not supported.
  
  DPOPortBaseC DPGraphWindowFormatBodyC::CreateOutput(const StringC &filename,const type_info &obj_type) const {
    ONDEBUG(cerr << "DPGraphWindowFormatBodyC::CreateOutput(), Called. Filename=" << filename << " obj_type=" << TypeName(obj_type) << " \n");
    if(obj_type != typeid(Array1dC<RealT>) && obj_type != typeid(Array1dC<Point2dC>))
      return DPOPortBaseC();
    
    StringC winName = ExtractParams(filename);
    StringC device = ExtractDevice(filename);
    
    // See if the window already exists.
    DPGraphWindowC win;
    RWLockHoldC hold(windowsLock,RWLOCK_WRITE);
    if(!windows.Lookup(winName,win)) {
      win = DPGraphWindowC(winName);
      windows[winName] = win;
    }
    hold.Unlock();
    
    // Setup a new port to the window and return it.
    bool accum = false;
    if(device == "GRAPHA") 
      accum = true;
    if(obj_type == typeid(Array1dC<RealT>))
      return DPGraphWindowOPortC<Array1dC<RealT> >(win,device,accum);
    return DPGraphWindowOPortC<Array1dC<Point2dC> >(win,device,accum);
  }
  
  DPGraphWindowFormatC initDPGraphWindowFormat;
 
}
