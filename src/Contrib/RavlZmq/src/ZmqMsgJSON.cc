// This file is part of RAVL, Recognition And Vision Library
// Copyright (C) 2011, Charles Galambos
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//! lib=RavlZmq

#include "Ravl/Zmq/MsgJSON.hh"
#include "Ravl/Zmq/MsgBuffer.hh"
#include "Ravl/DP/Converter.hh"

namespace RavlN { namespace ZmqN {

  //! Create a message buffer of a given size.
  MsgJSONC::MsgJSONC(size_t size)
    : SArray1dC<char>(MsgBufferC(size),size)
  {}

  SArray1dC<char> MsgJSON2SArray1d(const MsgJSONC &msg)
  { return msg; }

  DP_REGISTER_CONVERSION(MsgJSON2SArray1d,1.0);

  void LinkMsgJSON()
  {}
}}

