// This file is part of RAVL, Recognition And Vision Library
// Copyright (C) 2013, Charles Galambos
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVL_PROCESSPLAYLIST_HEADER
#define RAVL_PROCESSPLAYLIST_HEADER 1
/////////////////////////////////////////////////////////////
//! lib=RavlOSIO
//! userlevel=Normal
//! author="Charles Galambos"
//! date="8/2/2013"
//! docentry="Ravl.API.Core.Data Processing"

#include "Ravl/DP/PlayList.hh"
#include "Ravl/DP/StreamOp.hh"
#include "Ravl/XMLFactory.hh"

namespace RavlN {

  //: Go through a play list processing each file separately
  // The process
  class ProcessPlayListC
   : public RCBodyVC
  {
  public:
    //! Constructor
    ProcessPlayListC();

    //! Constructor
    ProcessPlayListC(const PlayListC &playList,const DPStreamOpC &process,const StringC &outputTemplate,bool verbose);


    //! Process list.
    bool ProcessList();

    //! Process a single file.
    bool Process(const SubSequenceSpecC &seqSpec);

    //! Handle to class
    typedef SmartPtrC<ProcessPlayListC> RefT;
  protected:
    StringC m_inputName;
    StringC m_outputName;
    DPStreamOpC m_process;
    PlayListC m_playList;
    StringC m_outputTemplate;
    bool m_verbose;
  };
}
#endif
