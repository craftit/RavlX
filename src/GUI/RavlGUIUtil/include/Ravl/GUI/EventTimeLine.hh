// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2006, OmniPerception Ltd.
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVL_EVENTTIMELINE_HEADER
#define RAVL_EVENTTIMELINE_HEADER 1
//! author="Charles Galambos"
//! rcsid="$Id$"
//! lib=RavlGUIUtil
//! docentry="Ravl.API.Graphics.GTK.Util"

#include "Ravl/GUI/RawCanvas.hh"
#include "Ravl/RealRange1d.hh"
#include "Ravl/GUI/MouseEvent.hh"
#include "Ravl/Tuple2.hh"

namespace RavlGUIN {
  
  using namespace RavlImageN;
  class EventTimeLineC;
  
  //! userlevel=Develop
  //: Event time line.
  
  class EventTimeLineBodyC 
    : public RawCanvasBodyC 
  {
  public:
    EventTimeLineBodyC();
    //: Default constructor.
    
    EventTimeLineBodyC(const RealRangeC &rng,const DListC<Tuple2C<IntT,RealRangeC> > &events);
    //: Constructor.
    //!param: rng - Range of times to display.
    //!param: events - List of events.

    EventTimeLineBodyC(IntT srow,IntT scol,const RealRangeC &rng,const DListC<Tuple2C<IntT,RealRangeC> > &events);
    //: Constructor.
    //!param: rng - Range of times to display.
    //!param: events - List of events.
    
    EventTimeLineBodyC(const RealRangeC &rng);
    //: Constructor.
    //!param: rng - Range of times to display.
    //!param: events - List of events.

    EventTimeLineBodyC(IntT srow,IntT scol,const RealRangeC &rng);
    //: Constructor.
    //!param: rng - Range of times to display.
    //!param: events - List of events.
    
    ~EventTimeLineBodyC();
    //: Destructor.
    
    virtual bool Create()
    { return CommonCreate(); }
    //: Create the widget.
    
    virtual bool Create(GtkWidget *_widget)
    { return CommonCreate(_widget); }
    //: Create the widget.
    
    bool SetDisplayRange(RealRangeC &rng, bool redraw=true);
    //: Set range of times to display
    
    bool GUISetDisplayRange(RealRangeC &rng, bool redraw=true);
    //: Set range of times to display
    
    bool GUISetLocalSegment(const RealRangeC &segRange, bool redraw=true);
    //: Set the local segment.
    // Segments with a negative size will not be displayed
    
    inline Signal1C<RealT> &SigTimeSelected()
    { return timeSelected; }
    //: Frame selected signal
    
    bool Goto(RealT &time, bool redraw=true);
    //: Centre on a specific time.
    
    bool GUIGoto(RealT &time, bool redraw=true);
    //: Centre on a specific time.
    
    bool SetMarker(RealT time, bool redraw=true);
    //: Set marker position.
    
    bool GUISetMarker(RealT time, bool redraw=true);
    //: Set marker position.

    bool SetMarkerSpan(RealT span, bool redraw=true);
    //: Set the span of the marker 

    bool GUISetMarkerSpan(RealT span, bool redraw=true);
    //: Set the span
    
    bool SetEvents(DListC<Tuple2C<IntT,RealRangeC> > &events, bool redraw);
    //: Set event list.
    
    bool GUISetEvents(DListC<Tuple2C<IntT,RealRangeC> > &events, bool redraw);
    //: Set event list.

    bool SetActiveSegments(DListC<RealRangeC> & segments, bool redraw);
    //: Set list of inactive segments

    bool GUISetActiveSegments(DListC<RealRangeC> & segments, bool redraw);
    //: Set a list of inactive segments. 
    
    bool SetDisplaySpan(RealT &size, bool redraw);
    //: Set the length of time to display.
    
    bool GUISetDisplaySpan(RealT &size, bool redraw);
    //: Set the length of time to display.

    inline const RealRangeC & LocalSegment() const
    { return m_localSegment; }
    //: Access local segment.

    inline const RealRangeC & DisplayRange() const
    { return displayRange; }
    //: Access display range

    inline const RealT & At() const
    { return atMarker; }
    //: Access at position.

    inline const DListC<Tuple2C<IntT,RealRangeC> > & Events() const
    { return events; }
    //: Access to the events
    
  protected:
    bool CommonCreate(GtkWidget *_widget = NULL);
    //: Common GUI creation

    bool EventConfigure(GdkEvent* &event);
    //: Configure.
    
    bool EventExpose(GdkEvent* &event);
    //: Expose of area.
    
    bool EventMousePress(MouseEventC &mousePress);
    //: Event mouse press.
    
    bool GUIDraw();
    //: Draw widget on screen.

  protected:

    IndexRange2dC displayArea; // Area to use for displaying images.
    RealRangeC displayRange; // Range of times to display.
    DListC<Tuple2C<IntT,RealRangeC> > events;
    DListC<RealRangeC > m_activeSegments;
    RealRangeC m_localSegment; // Local segment.
    Signal1C<RealT> timeSelected; // Frame selected signal
    RealT atMarker; // Marker for where you are in the sequence.
    RealT m_atSpan;
    UIntT updateId;
    GdkGC *markerGc;
    GdkGC *segmentGc;
    
    const static RealT m_arrowWidth;
    const static RealT m_arrowBorder;
    
    friend class EventTimeLineC;
  };



  //! userlevel=Normal
  //: Event time line.
  
  class EventTimeLineC 
    : public RawCanvasC 
  {
  public:
    EventTimeLineC()
    {}
    //: Default constructor.
    // Creates an invalid handle.

    EventTimeLineC(const RealRangeC &rng,const DListC<Tuple2C<IntT,RealRangeC> > &events) 
      : RawCanvasC(*new EventTimeLineBodyC(rng,events))
    {}
    //: Constructor.
    //!param: rng - Range of times to display.
    //!param: events - List of events.

    EventTimeLineC(const RealRangeC &rng) 
      : RawCanvasC(*new EventTimeLineBodyC(rng))
    {}
    //: Constructor.
    //!param: rng - Range of times to display.
    //!param: events - List of events.
    
    EventTimeLineC(IntT srow,IntT scol,const RealRangeC &rng ,const DListC<Tuple2C<IntT,RealRangeC> > &events) 
      : RawCanvasC(*new EventTimeLineBodyC(srow,scol,rng,events))
    {}
    //: Constructor.
    //!param: rng - Range of times to display.
    //!param: events - List of events.
    
    EventTimeLineC(IntT srow,IntT scol,const RealRangeC &rng) 
      : RawCanvasC(*new EventTimeLineBodyC(srow,scol,rng))
    {}
    //: Constructor.
    //!param: rng - Range of times to display.
    //!param: events - List of events.
    
    
  protected:
    EventTimeLineC(EventTimeLineBodyC &bod)
      : RawCanvasC(bod)
    {}
    
    EventTimeLineBodyC& Body()
    { return static_cast<EventTimeLineBodyC&>(WidgetC::Body()); }
    //: Access body.

    const EventTimeLineBodyC& Body() const
    { return static_cast<const EventTimeLineBodyC&>(WidgetC::Body()); }
    //: Access body.
    
  public:

    bool Goto(RealT &time, bool redraw=true)
    { return Body().Goto(time, redraw); }
    //: Centre on a specific time.
    
    bool GUIGoto(RealT &time, bool redraw=true)
    { return Body().GUIGoto(time, redraw); }
    //: Centre on a specific time.
    
    bool SetMarker(RealT time, bool redraw=true)
    { return Body().SetMarker(time, redraw); }
    //: Set marker position.
    
    bool GUISetMarker(RealT time, bool redraw=true)
    { return Body().GUISetMarker(time, redraw); }
    //: Set marker position.
    
    bool SetMarkerSpan(RealT span, bool redraw=true)
    { return Body().SetMarkerSpan(span, redraw); }
    //: Set the span of the marker

    bool GUISetMarkerSpan(RealT span, bool redraw=true)
    { return Body().GUISetMarkerSpan(span, redraw); }
    //: Set the span

    bool SetEvents(DListC<Tuple2C<IntT,RealRangeC> > &events, bool redraw=true)
    { return Body().SetEvents(events, redraw); }
    //: Set event list.
    
    bool GUISetEvents(DListC<Tuple2C<IntT,RealRangeC> > &events, bool redraw=true)
    { return Body().GUISetEvents(events, redraw); }
    //: Set event list.

    bool SetActiveSegments(DListC<RealRangeC> & segments, bool redraw=true)
    { return Body().SetActiveSegments(segments, redraw); }
    //: Set list of inactive segments

    bool GUISetActiveSegments(DListC<RealRangeC> & segments, bool redraw=true)
    { return Body().GUISetActiveSegments(segments, redraw); }
    //: Set a list of inactive segments.

    
    bool SetDisplayRange(RealRangeC &rng, bool redraw=true)
    { return Body().SetDisplayRange(rng, redraw); }
    //: Set range of times to display
    
    bool GUISetDisplayRange(RealRangeC &rng, bool redraw=true)
    { return Body().GUISetDisplayRange(rng, redraw); }
    //: Set range of times to display
    
    bool GUISetLocalSegment(const RealRangeC &segRange, bool redraw=true)
    { return Body().GUISetLocalSegment(segRange, redraw); }
    //: Set the local segment.
    // Segments with a negative size will not be displayed
    
    bool SetDisplaySpan(RealT &size, bool redraw=true)
    { return Body().SetDisplaySpan(size, redraw); }
    //: Set the length of time to display.
    
    bool GUISetDisplaySpan(RealT &size, bool redraw=true)
    { return Body().GUISetDisplaySpan(size, redraw); }
    //: Set the length of time to display.
    
    Signal1C<RealT> &SigTimeSelected()
    { return Body().SigTimeSelected(); }
    //: Frame selected signal
    
    const RealRangeC & LocalSegment() const
    { return Body().LocalSegment(); }
    //: Access local segment.

    const RealRangeC & DisplayRange() const
    { return Body().DisplayRange(); }
    //: Access display range

    const RealT & At() const
    { return Body().At(); }
    //: Access at position.

    const DListC<Tuple2C<IntT,RealRangeC> > & Events() const
    { return Body().Events() ; }
    //: Access to events.

    friend class EventTimeLineBodyC;
  }; 
}
#endif
