// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVLGUI_PLAYCONTROL_HEADER
#define RAVLGUI_PLAYCONTROL_HEADER 1
//////////////////////////////////////////////
//! rcsid="$Id$"
//! file="Ravl/Applications/VPlay/GUIPlayControl.hh"
//! author="Charles Galambos"
//! date="23/03/1999"
//! docentry="Ravl.Applications.Video"
//! lib=RavlVPlay

#include "Ravl/GUI/LBox.hh"
#include "Ravl/DP/PlayControl.hh"
#include "Ravl/Threads/Mutex.hh"
#include "Ravl/Threads/Thread.hh"
#include "Ravl/Threads/Signal1.hh"
#include "Ravl/Threads/ThreadEvent.hh"
#include "Ravl/GUI/Slider.hh"
#include "Ravl/GUI/TextEntry.hh"
#include "Ravl/GUI/ToggleButton.hh"

namespace RavlGUIN {
  
  //! userlevel = Develop
  //: Play button control values.
  
  enum PlayControlButtonT
  {
    PCB_Rewind = 0, // Jump to the beginning.
    PCB_End, // Jump to the end.
    PCB_Backx2, // Play backwards at a faster rate.
    PCB_Back, // Play backwards.
    PCB_JumpBack, // Jump one frame backwards.
    PCB_Stop, // Stop.
    PCB_JumpForward, // Jump one frame forwards.
    PCB_Play, // Play forwards.
    PCB_Playx2, // Play forwards at a faster rate.
    PCB_Slider // Slider.
  };

  class PlayControlC;

  ////////////////////////////
  //! userlevel=Develop
  //: Play Control GUI interface body.
  
  class PlayControlBodyC 
    : public LBoxBodyC
  {
  public:
    PlayControlBodyC(const DPPlayControlC &ctrl,bool simpleControls = false,bool extendedControls = true);
    //: Constructor.
    
    PlayControlBodyC(bool simpleControls = false,bool extendedControls = true);
    //: Default constructor.
    
    ~PlayControlBodyC();
    //: Destructor.

    bool CommonCreate(GtkWidget *_widget=NULL);

    virtual bool Create();
    //: Create widget.
    
    virtual bool Create(GtkWidget *_widget);
    //: Create with a widget supplied from elsewhere.

    bool Rewind();
    //: Rewind to beginning.
    
    bool TheEnd();
    //: Goto end.
    
    bool Backx2();
    //: Backward x2 speed
    
    bool Back();
    //: Backward x1 speed
    
    bool JBkw();
    //: Jog a frame backward
    
    bool Stop();
    //: Stop
    
    bool JFwd();
    //: Jog a frame forward.
    
    bool Play();
    //: Forward x1 speed
    
    bool Playx2();
    //: Forward x2 speed
    
    void SetControl(const DPPlayControlC &ctrl);
    //: Setup control.
    
    void AddStream(DPPlayControlC &strm);
    //: Add stream to control list.
    // Thread safe
    
    void GUIAddStream(DPPlayControlC &strm);
    //: Add stream to control list.
    // If no master stream exists  the new stream
    // will become it.
    
    bool DelStream(DPPlayControlC &strm);
    //: Remove stream from control list.
    // If stream is the master, it will be removed
    // and replace with the first slave.
    
    bool SetSkip(StringC &val);
    //: Set the frame skip value.

    bool ShowExtended(bool &doit);
    //: Show/Hide extended controls.
    
    bool SetSubStart(StringC &text);
    //: Set start of sub-sequence
    
    bool SetSubEnd(StringC &text);
    //: Set end of sub-sequence
    
    bool SetRepeatMode(StringC &text);
    //: Set sub-sequence mode.
    
    bool HandleKeyPress(GdkEventKey * keyEvent);

    Signal1C<IntT> &SigUpdateFrameNo()
    { return sigUpdateFrameNo; }
    //: Access update frame number signal.
    // Note this is NOT called every frame, only about 5 to 10 times
    // a second.

    Signal2C<bool, PlayControlButtonT> & SigInteractive()
    { return m_sigInteractive; }
    //: Access to the interactive signal
    // this signal is set true whenever the user interactively uses the play control
    // eg. for removing governer constraints when user drags slider...

    bool Shutdown();
    //: Shutdown play control.

    /*bool testskipfunc() {
       GrabFocus();
       return true;
    }*/

  protected:
    virtual void Destroy();
    //: Undo all references.
    
    void InitGUI();
    //: Setup gui.
    
    void AddStream(DPPlayControlC &strm, bool bIsGUIThread);
    
    bool SliderUpdate();
    //: Update the GUI slider.
    
    bool SliderCallback(RealT &num);
    //: Position updates from slider.
   
    bool CBInteractiveMode(bool & state);
    //: Callback for when we are in 'interactive mode', ie user touching controls.


    void Speed(IntT i);
    void Seek(UIntT pos);
    void Pause();
    void Continue();
    void Jog(IntT i);
    
    MutexC access;
    DPPlayControlC pc; // Master sequence.
    DListC<DPPlayControlC> pcs; // Slave sequences.
    ThreadC sliderUpdate;
    SliderC frameSlider;
    TextEntryC textSkip,textStart,textEnd;  
    LBoxC extraControls;
    CheckButtonC enableextras;
    bool doneAdd;
    bool created;
    IntT baseSpeed; // what speed was last set.
    IntT skip;
    IntT textBoxSelected;
    
    Signal1C<IntT> sigUpdateFrameNo; // Signal frame number update.
    Signal2C<bool, PlayControlButtonT> m_sigInteractive; // Signal that the GUI is being used interactively
    bool simpleControls;
    bool extendedControls;
    friend class PlayControlC;
    UIntT lastUpdateFrameNo; //: Frame number of last update.
    //ThreadEventC m_freeze; // flag to freeze the slider from updating.
  };
  
  ////////////////////////////
  //! userlevel=Normal
  //: Play Control GUI interface.
  // This class implements a video player object for inclusino in GUI applications.  See <a href = "../../../Application/VPlay.html">VPlay</a> for an example of use.
  
  class PlayControlC 
    : public LBoxC
  {
  public:
    PlayControlC(const DPPlayControlC &ctrl,bool simpleControls = false,bool extendedControls = true)
      : LBoxC(*new PlayControlBodyC(ctrl,simpleControls,extendedControls))
    {}
    //: Constructor.
    
    PlayControlC(bool simpleControls,bool extendedControls = true)
      : LBoxC(*new PlayControlBodyC(simpleControls,extendedControls))
    {}
    //: Constructor.
    
    PlayControlC()
    {}
    //: Default constructor.
    // Creates an invalid handle.
    
  protected:
    PlayControlC(PlayControlBodyC &bod)
      : LBoxC(bod)
    {}
    //: Body constructor.
    
    PlayControlBodyC &Body()
    { return static_cast<PlayControlBodyC &>(WidgetC::Body()); }
    //: Access body.
    
    const PlayControlBodyC &Body() const
    { return dynamic_cast<const PlayControlBodyC &>(WidgetC::Body()); }
    //: Access body.
    
    bool SliderUpdate()
    { return Body().SliderUpdate(); }
    //: Update the GUI slider.
    
    bool SliderCallback(RealT &num)
    { return Body().SliderCallback(num); }
    //: Position updates from slider.
  
  public:  

    bool Rewind()
    { return Body().Rewind(); }
    //: Rewind to beginning.
    
    bool TheEnd()
    { return Body().TheEnd(); }
    //: Goto end.
    
    bool Backx2()
    { return Body().Backx2(); }
    //: Backward x2 speed
    
    bool Back()
    { return Body().Back(); }
    //: Backward x1 speed
    
    bool JBkw()
    { return Body().JBkw(); }
    //: Jog a frame backward
    
    bool Stop()
    { return Body().Stop(); }
    //: Stop
    
    bool JFwd()
    { return Body().JFwd(); }
    //: Jog a frame forward.
    
    bool Play()
    { return Body().Play(); }
    //: Forward x1 speed
    
    bool Playx2()
    { return Body().Playx2(); }
    //: Forward x2 speed
    
    void SetControl(const DPPlayControlC &ctrl)
    { Body().SetControl(ctrl); }
    //: Setup control.
    
    void AddStream(DPPlayControlC &strm)
    { Body().AddStream(strm); }
    //: Add stream to control list.
    // If no master stream exists  the new stream
    // will become it.
    void GUIAddStream(DPPlayControlC &strm)
    { Body().GUIAddStream(strm); }
    
    bool DelStream(DPPlayControlC &strm)
    { return Body().DelStream(strm); }
    //: Remove stream from control list.
    // If stream is the master, it will be removed
    // and replace with the first slave.
    // Returns true is stream successfully removed.
    // (Can only fail if 'strm' is not in control.)
    // Thread safe.
    
    Signal1C<IntT> &SigUpdateFrameNo()
    { return Body().SigUpdateFrameNo(); }
    //: Access update frame number signal.
    // Note this is NOT called every frame, only about 5 to 10 times
    // a second.

    Signal2C<bool, PlayControlButtonT> & SigInteractive()
    { return Body().SigInteractive(); }
    //: Access to the interactive signal
    // this signal is set true whenever the user interactively uses the play control
    // eg. for removing governer constraints when user drags slider...

    bool Shutdown()
    { return Body().Shutdown(); }
    //: Shutdown play control.
   
    void setSkip(StringC &val) {
       Body().SetSkip(val);
    }
    //: Set the frame skip value.
    
 
    void setsubStart(StringC &val) {
       Body().SetSubStart(val);
    }

    void setsubEnd(StringC &val) {
       Body().SetSubEnd(val);
    }

    TextEntryC textskip() {
        return Body().textSkip;
    }

    TextEntryC textstart() {
        return Body().textStart;
    }

    TextEntryC textend() {
        return Body().textEnd;
    }

    IntT getTextBoxSelected() {
        return Body().textBoxSelected;
    }
    void setTextBoxSelected(IntT in) {
        Body().textBoxSelected = in;
    }

    //: Access body.
    
    friend class PlayControlBodyC;
  };
}

#endif
