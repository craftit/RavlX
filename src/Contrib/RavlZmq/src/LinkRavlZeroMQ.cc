// This file is part of RAVL, Recognition And Vision Library
// Copyright (C) 2011, Charles Galambos
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//! lib=RavlZmq

namespace RavlN { namespace ZmqN {

  extern void LinkContext();
  extern void LinkSocket();
  extern void LinkMsgJSON();

  void LinkRavlZeroMQ() {
    LinkContext();
    LinkSocket();
    LinkMsgJSON();
  }
}}
