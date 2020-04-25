// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVL_COMPOSE_HEADER
#define RAVL_COMPOSE_HEADER 1
////////////////////////////////////////////////////
//! rcsid="$Id$"
//! userlevel=Normal
//! author="Charles Galambos"
//! docentry="Ravl.API.Core.Data Processing"
//! example=exDataProc.cc
//! lib=RavlIO
//! file="Ravl/Core/IO/Compose.hh"

#include "Ravl/DP/ProcCompose.hh"
#include "Ravl/DP/ProcIStream.hh"
#include "Ravl/DP/ProcOStream.hh"
#include "Ravl/DP/FuncP2Proc.hh"
#include "Ravl/DP/IOConnect.hh"
#include "Ravl/DP/Port.hh"
#include "Ravl/DP/StreamOp.hh"
#include "Ravl/DP/Method2Proc.hh"
#include "Ravl/DP/Method2Proc21.hh"
#include "Ravl/DP/Func2Proc21.hh"
#include "Ravl/DP/Method2Proc31.hh"
#include "Ravl/DP/Func2Proc31.hh"
#include "Ravl/DP/StreamOp21.hh"
#include "Ravl/DP/StreamOp31.hh"
#include "Ravl/DP/IOPort.hh"
#include "Ravl/DP/MethodIO.hh"
#include "Ravl/DP/Func2Stream.hh"

namespace RavlN {
  
  template<class InT,class MidT,class OutT>
  DPIOPortC<InT,OutT> operator>>(const DPIOPortC<InT,MidT> &in,DPIStreamOpC<MidT,OutT> dat)  {
    dat.Input() = in;
    return DPIOPortJoin(dat,in);
  }
  
}

#endif
