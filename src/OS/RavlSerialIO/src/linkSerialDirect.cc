// This file is part of RAVL, Recognition And Vision Library
// Copyright (C) 2010, OmniPerception Ltd.
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
/*
 * linkSerialDirect.cc
 *
 *  Created on: 23 Sep 2010
 *      Author: Alexey Kostin
 */
//! lib=RavlSerialIO

namespace RavlN {
  extern void InitSerialDirectFormat();

  void LinkSerialDirect() {
    InitSerialDirectFormat();
  }

}
