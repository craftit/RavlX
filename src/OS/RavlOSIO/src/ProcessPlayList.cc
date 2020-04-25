#include "Ravl/DP/ProcessPlayList.hh"
#include "Ravl/XMLFactory.hh"
#include "Ravl/OS/Filename.hh"
#include "Ravl/DP/SequenceIO.hh"
#include "Ravl/DP/Plug.hh"
#include "Ravl/DP/TypeInfo.hh"
#include "Ravl/StrStream.hh"

namespace RavlN {

  //! Constructor
  ProcessPlayListC::ProcessPlayListC()
   : m_verbose(false)
  {}

  //! Constructor
  ProcessPlayListC::ProcessPlayListC(const PlayListC &playList,
                                     const DPStreamOpC &process,
                                     const StringC &outputTemplate,
                                     bool verbose)
   : m_inputName("In1"),
     m_outputName("Out1"),
     m_process(process),
     m_playList(playList),
     m_outputTemplate(outputTemplate),
     m_verbose(verbose)
  {}


  //! Process list.
  bool ProcessPlayListC::ProcessList()
  {

    SArray1dC<EditSpecC> playList = m_playList.Edits();

    for(unsigned i = 0;i < playList.Size();i++) {
      Process(playList[i]);
    }

    return true;
  }

  //! Process a single file.
  bool ProcessPlayListC::Process(const SubSequenceSpecC &seqSpec) {

    if(!m_process.IsValid()) {
      RavlError("No process provided.");
      return false;
    }

    FilenameC fn = seqSpec.Filename();
    FilenameC pc = fn.PathComponent();
    FilenameC nc = fn.NameComponent();
    FilenameC bc = fn.BaseNameComponent();

    FilenameC outputName = m_outputTemplate;
    outputName.gsub("%p",pc);
    outputName.gsub("%n",nc);
    outputName.gsub("%b",bc);
    outputName.gsub("%f",fn);

    DPIPortBaseC isource;
    DPIPlugBaseC iplug;

    if(!m_process.GetIPlug(m_inputName,iplug)) {
      RavlError("Failed to find input '%s' ",m_inputName.c_str());
      for(DLIterC<DPIPlugBaseC> it(m_process.IPlugs());it;it++) {
        RavlDebug(" IPlug '%s' ",it->EntityName().c_str());
      }
      return false;
    }
    RavlAssert(iplug.IsValid());

    DPSeekCtrlC seekControl;
    if(!OpenISequenceBase(isource,seekControl,seqSpec.Filename(),seqSpec.FileFormat(),iplug.InputType(),m_verbose)) {
      RavlError("Failed to open input '%s'",seqSpec.Filename().c_str());
      return false;
    }
    RavlAssert(isource.IsValid());
    if(!iplug.ConnectPort(isource)) {
      RavlError("Failed to connect port.");
      return false;
    }

    DPIPortBaseC pullPort;
    if(!m_process.GetIPort(m_outputName,pullPort)) {
      RavlError("Failed to get output");
      return false;
    }

    DPOPortBaseC pushPort;

    DPSeekCtrlC seekControlO;
    if(!OpenOSequenceBase(pushPort,seekControl,outputName,"",pullPort.InputType(),m_verbose)) {
      RavlError("Failed to open output '%s' for type '%s' ",outputName.c_str(),RavlN::TypeName(pullPort.InputType()));
      return false;
    }

    DPTypeInfoC theType = TypeInfo(pullPort.InputType());
    if(!theType.IsValid()) {
      RavlError("Type '%s' unknown, can't complete process.");
      return false;
    }

    IndexC ff = seqSpec.FirstFrame();
    if(ff != 0) {
      bool done= false;
      if(seekControl.IsValid())
        done = seekControl.Seek64(ff.V());
      if(!done) {
        // Do it the hard way.
        for(IndexC i =0;i < ff;i++)
          pullPort.Discard();
      }
    }

    Int64T count = 0;
    if(seqSpec.LastFrame() == RavlConstN::maxInt) {
      RavlDebug("Processing whole sequence");
      while(1) {
        UIntT n = theType.Move(pullPort,pushPort,32);
        if(n == 0)
          break;
        count += n;
      }
    } else {
      RavlDebug("Processing sub sequence");
      // FIXME:- THe sequence length of the output and input need not match.
      count = theType.Move(pullPort,pushPort,seqSpec.Range().Size());
    }
    if(m_verbose) {
      RavlInfo("Processed %s frames.",RavlN::StringOf(count).c_str());
    }

    return true;
  }

}
