// This file is part of RAVL, Recognition And Vision Library.
// Copyright (C) 2009, OmniPerception Ltd
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//! lib=RavlDataServer


namespace RavlN {
  extern void LinkDataServer();
  extern void LinkDataServerControlServer();

  void MustLinkRavlDataServer() {
    LinkDataServer();
    LinkDataServerControlServer();
  }

}
