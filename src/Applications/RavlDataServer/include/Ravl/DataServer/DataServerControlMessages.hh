// This file is part of RAVL, Recognition And Vision Library.
// Copyright (C) 2009, OmniPerception Ltd
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVL_DATASERVERCONTROLMESSAGES_H
#define	RAVL_DATASERVERCONTROLMESSAGES_H
//! lib=RavlDataServer

namespace RavlN
{

  enum DataServerControlMessageIdT
  {
    DATASERVERCONTROL_ADDNODE        = 800,
    DATASERVERCONTROL_REMOVENODE     = 801,
    DATASERVERCONTROL_QUERYNODESPACE = 802,

    DATASERVERCONTROL_ADDNODE_RESULT        = 900,
    DATASERVERCONTROL_REMOVENODE_RESULT     = 901,
    DATASERVERCONTROL_NODECLOSED            = 902,
    DATASERVERCONTROL_NODEREMOVED           = 903,
    DATASERVERCONTROL_QUERYNODESPACE_RESULT = 904,
  };
  
}

#endif
