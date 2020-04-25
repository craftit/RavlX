// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2006, OmniPerception Ltd.
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here

//! author="Charles Galambos"
//! rcsid="$Id$"
//! lib=RavlGUIUtil

#include "Ravl/GUI/EventTimeLine.hh"
#include "Ravl/GUI/Manager.hh"
#include "Ravl/GUI/EventTimeLine.hh"
#include  <gdk/gdk.h>

#define DODEBUG 0
#if DODEBUG
#define ONDEBUG(x) x
#else
#define ONDEBUG(x)
#endif

namespace RavlGUIN {

  const RealT EventTimeLineBodyC::m_arrowWidth  = 0.025;
  const RealT EventTimeLineBodyC::m_arrowBorder = 0.25;
  
  //: Default constructor.
  
  EventTimeLineBodyC::EventTimeLineBodyC()
    : RawCanvasBodyC(15,15),
      m_localSegment(3,-8),
      timeSelected(0.0),
      atMarker(0),
      m_atSpan(0),
      updateId(0),
      markerGc(0),
      segmentGc(0)
  {}
  
  EventTimeLineBodyC::EventTimeLineBodyC(IntT srow,IntT scol,const RealRangeC &rng,const DListC<Tuple2C<IntT,RealRangeC> > &_events)
    : RawCanvasBodyC(srow,scol),
      displayRange(rng),
      events(_events),
      m_localSegment(3,-8),
      timeSelected(0.0),
      atMarker(0),
      m_atSpan(0),
      updateId(0),
      markerGc(0),
      segmentGc(0)
  {}
  
  //: Constructor.
  //!param: rng - Range of times to display.
  //!param: events - List of events.
  
  EventTimeLineBodyC::EventTimeLineBodyC(const RealRangeC &rng,const DListC<Tuple2C<IntT,RealRangeC> > &_events) 
    : RawCanvasBodyC(15,15),
      displayRange(rng),
      events(_events),
      m_localSegment(3,-8),
      timeSelected(0.0),
      atMarker(0),
      m_atSpan(0),
      updateId(0),
      markerGc(0),
      segmentGc(0)
  {}

  EventTimeLineBodyC::EventTimeLineBodyC(IntT srow,IntT scol,const RealRangeC &rng)
    : RawCanvasBodyC(srow,scol),
      displayRange(rng),
      m_localSegment(3,-8),
      timeSelected(0.0),
      atMarker(0),
      m_atSpan(0),
      updateId(0),
      markerGc(0),
      segmentGc(0)
  {}
  
  //: Constructor.
  //!param: rng - Range of times to display.
  //!param: events - List of events.
  
  EventTimeLineBodyC::EventTimeLineBodyC(const RealRangeC &rng) 
    : RawCanvasBodyC(15,15),
      displayRange(rng),
      m_localSegment(3,-8),
      timeSelected(0.0),
      atMarker(0),
      m_atSpan(0),
      updateId(0),
      markerGc(0),
      segmentGc(0)
  {}

  static bool DestroyGc(GdkGC *gc) {
    g_object_unref(gc);
    return true;
  }
  
  //: Destructor.
  
  EventTimeLineBodyC::~EventTimeLineBodyC() {
    if(markerGc != 0) {
      Manager.Queue(Trigger(DestroyGc,markerGc));
      markerGc = 0;
    }
    if(segmentGc != 0) {
      Manager.Queue(Trigger(DestroyGc,segmentGc));
      segmentGc = 0;      
    }
  }
  
  //: Create the widget.
  
  bool EventTimeLineBodyC::CommonCreate(GtkWidget *_widget) {
    ConnectRef(Signal("expose_event"),*this,&EventTimeLineBodyC::EventExpose);
    ConnectRef(Signal("configure_event"),*this,&EventTimeLineBodyC::EventConfigure);
    ConnectRef(Signal("button_press_event"),*this,&EventTimeLineBodyC::EventMousePress);
    
    bool ret = false;
    if (_widget == NULL)
      ret = RawCanvasBodyC::Create();
    else
      ret = RawCanvasBodyC::Create(_widget);
    
    return ret;
  }

  //: Set range of times to display
  
  bool EventTimeLineBodyC::SetDisplayRange(RealRangeC &rng, bool redraw)
  {
    Manager.QueueOnGUI(Trigger(EventTimeLineC(*this),&EventTimeLineC::GUISetDisplayRange,rng, redraw));
    return true; 
  }

  //: Set range of times to display
  
  bool EventTimeLineBodyC::GUISetDisplayRange(RealRangeC &rng, bool redraw) {
    displayRange = rng; 
    ONDEBUG(cerr << "EventTimeLineBodyC::GUISetDisplayRange(). Range=" << displayRange << " \n");
    if (redraw)
      GUIDraw();
    return true;
  }


  
  //: Set the local segment.
  
  bool EventTimeLineBodyC::GUISetLocalSegment(const RealRangeC &segRange, bool redraw) {
    ONDEBUG(cerr << "EventTimeLineBodyC::GUISetLocalSegment(const RealRangeC &segRange="<<segRange<<")\n");
    if(m_localSegment != segRange) {
      m_localSegment = segRange;
      if (redraw)
        GUIDraw();
    }
    return true;
  }
  
  //: Set marker position.
  
  bool EventTimeLineBodyC::SetMarker(RealT time, bool redraw) {
    Manager.Queue(Trigger(EventTimeLineC(*this),&EventTimeLineC::GUISetMarker,time, redraw));
    return true;
  }
  
  //: Set marker position.
  
  bool EventTimeLineBodyC::GUISetMarker(RealT time, bool redraw) {
    ONDEBUG(cerr << "EventTimeLineBodyC::GUISetMarker(RealT time=" << time << ")\n");
    RavlAssertMsg(Manager.IsGUIThread(), "GUI Thread only");
    atMarker = time;
    if (redraw)
      GUIDraw();
    return true;
  }
  
  //: Centre on a specific time.


  bool EventTimeLineBodyC::SetMarkerSpan(RealT span, bool redraw){
    Manager.Queue(Trigger(EventTimeLineC(*this), &EventTimeLineC::GUISetMarkerSpan, span, redraw));
    return true;
  }


  bool EventTimeLineBodyC::GUISetMarkerSpan(RealT span, bool redraw){
    RavlAssertMsg(Manager.IsGUIThread(), "GUI Thread only");
    m_atSpan = span;
    if (redraw)
      GUIDraw();
    return true;
  }

  bool EventTimeLineBodyC::Goto(RealT &time, bool redraw) {
    Manager.Queue(Trigger(EventTimeLineC(*this),&EventTimeLineC::GUIGoto,time, redraw));
    return true;
  }
  
  //: Centre on a specific time.
  
  bool EventTimeLineBodyC::GUIGoto(RealT &time, bool redraw) {
    ONDEBUG(cerr << "EventTimeLineBodyC::GUIGoto(). Time=" << time << " \n");
    RealT size = displayRange.Size()/2;
    displayRange = RealRangeC(time - size ,time + size);
    //atMarker = time;
    if (redraw)
      GUIDraw();
    return true;
  }
  
  //: Set the length of time to display.
  
  bool EventTimeLineBodyC::SetDisplaySpan(RealT &size, bool redraw) {
    Manager.Queue(Trigger(EventTimeLineC(*this),&EventTimeLineC::GUISetDisplaySpan,size, redraw));
    return true;
  }
  
  //: Set the length of time to display.
  
  bool EventTimeLineBodyC::GUISetDisplaySpan(RealT &size, bool redraw) {
    ONDEBUG(cerr << "bool EventTimeLineBodyC::GUISetDisplaySpan(RealT &size=" << size << ")\n");
    RealT time = displayRange.Center();
    RealT val = size / 2;
    displayRange = RealRangeC(time - val,time + val);
    if (redraw)
      GUIDraw();
    return true;
  }


  //: Set event list.
  
  bool EventTimeLineBodyC::SetEvents(DListC<Tuple2C<IntT,RealRangeC> > &_events, bool redraw) {
    Manager.Queue(Trigger(EventTimeLineC(*this),&EventTimeLineC::GUISetEvents,_events, redraw));
    return true;
  }
  
  //: Set event list.
  
  bool EventTimeLineBodyC::GUISetEvents(DListC<Tuple2C<IntT,RealRangeC> > &_events, bool redraw) {
    events = _events;
    if (redraw)
      GUIDraw();
    return true;
  }

  //: Set the inactive segment list
  bool EventTimeLineBodyC::SetActiveSegments(DListC<RealRangeC> & _segments, bool redraw)
  {
    Manager.Queue(Trigger(EventTimeLineC(*this), &EventTimeLineC::GUISetActiveSegments, _segments, redraw));
    return true;
  }


  //: Set the inactive segment list
  bool EventTimeLineBodyC::GUISetActiveSegments(DListC<RealRangeC> & _segments, bool redraw)
  {
    m_activeSegments = _segments;
    if (redraw)
      GUIDraw();
    return true;
  }



  //: Handle configure event.
  
  bool EventTimeLineBodyC::EventConfigure(GdkEvent* &event) {
    ONDEBUG(cerr << "EventTimeLineBodyC::EventConfigure(). \n");
    IndexRange2dC newRec;
    TranslateConfigureEvent(event,newRec);
    if(newRec == displayArea)
      return true; // Same size, nothing to do!
    displayArea = newRec;
    // Other config...
    if(markerGc == 0) {
      markerGc = gdk_gc_new(DrawArea()); 
      gdk_gc_copy(markerGc,GUIDrawGC());
      
      GdkColor colour;
      colour.pixel = 0;
      colour.red = 255 * 255;
      colour.green = 0;
      colour.blue = 0;
      gdk_gc_set_rgb_fg_color (markerGc,&colour);
    }
    if(segmentGc == 0) {
      segmentGc = gdk_gc_new(DrawArea()); 
      gdk_gc_copy(segmentGc,GUIDrawGC());
      
      GdkColor colour;
      colour.pixel = 0;
      colour.red = 128 * 255;
      colour.green = 128 * 255;
      colour.blue = 255 * 255;
      gdk_gc_set_rgb_fg_color (segmentGc,&colour);      
    }
    return true;
  }
  
  //: Expose of area.
  
  bool EventTimeLineBodyC::EventExpose(GdkEvent* &event) {
    ONDEBUG(cerr << "EventTimeLineBodyC::EventExpose(). \n");
    IntT toFollow;
    IndexRange2dC newRec;
    TranslateExposeEvent(event,newRec,toFollow);
    GUIDraw();
    return true;
  }
  
  //: Event mouse press.
  
  bool EventTimeLineBodyC::EventMousePress(MouseEventC &mousePress) {
    ONDEBUG(cerr << "EventTimeLineBodyC::EventMousePress(). \n");
    IndexC mouseCol = mousePress.At()[1];
    IntT arrowWidth = Floor(displayArea.Range2().Size() * m_arrowWidth);

    ONDEBUG(cerr << "mouseCol:" << mouseCol << "\tarrowWidth: " << arrowWidth);
    if (mouseCol < displayArea.Range2().Min() + arrowWidth) {
      // Left arrow clicked
      DLIterC< Tuple2C<IntT, RealRangeC> > it(events);
      for (it.Last(); it; it--) {
        if (it->Data2().Min() < atMarker)
          break;
      }
      
      if (it) {
        if (it->Data2().Contains(atMarker))
          atMarker--;
        else
          atMarker = it->Data2().Max() - 1;
        timeSelected(atMarker);
      }
      
      return true;
    }
    
    if (mouseCol > displayArea.Range2().Max() - arrowWidth) {
      // Right arrow clicked
      DLIterC< Tuple2C<IntT, RealRangeC> > it(events);
      for(; it; it++) {
        if (it->Data2().Min() > atMarker)
          break;
        
      }
      
      if (it) {
        if (it->Data2().Contains(atMarker))
          atMarker++;
        else
          atMarker = it->Data2().Min();
        timeSelected(atMarker);
      }
      
      return true;
    }

    // Get the time
    RealT scale = static_cast<RealT>(displayArea.Range2().Size()) / displayRange.Size();
    RealT time = (static_cast<RealT>(mouseCol.V()) / scale) + displayRange.Min();

#if 0
    // Dump range click is in.
    for(DLIterC<Tuple2C<IntT,RealRangeC> > it(events);it;it++) {
      if(it->Data2().Contains(time))
        cerr << "Time " << time << " in " << it->Data2() << "\n";
    }
#endif
    
    //cerr << "Press time=" << time << "\n";
    timeSelected(time);
    return true;
  }


  //: Draw widget on screen.
  
  bool EventTimeLineBodyC::GUIDraw() {
    ONDEBUG(cerr << "EventTimeLineBodyC::GUIDraw(). Range=" << displayRange << " Events=" << events.Size() << "\n");

    if(displayArea.Cols() < 1 || displayArea.Rows() < 1)
      return true; // No display area.
    
    // Clear the box
    GUIDrawRectangle(GUIDrawGCGrey(), displayArea, true);
    
    RealT scale = static_cast<RealT>(displayArea.Range2().Size()) / displayRange.Size();
    
    // Render visible part.
    IndexC midV = displayArea.Range1().Center();
    IntT arrowWidth = Floor(displayArea.Range2().Size() * m_arrowWidth);
    IntT arrowBorder = Floor(static_cast<RealT>(arrowWidth) * m_arrowBorder);
    IndexC minCol = displayArea.Range2().Min() + arrowWidth;
    IndexC maxCol = displayArea.Range2().Max() - arrowWidth;
    IndexRangeC vertRange(displayArea.Range1().Shrink(4));
    
    // How much time do the arrow obscure?
    RealT arrowTime = m_arrowWidth * displayRange.Size();
    IndexRange2dC arrowBox(vertRange, displayArea.Range2().Shrink(arrowWidth));
    const RealT minLimit = displayRange.Min() + arrowTime;
    const RealT maxLimit = displayRange.Max() - arrowTime;


    // draw active segments
    for (DLIterC<RealRangeC> iter(m_activeSegments) ; iter.IsElm() ; iter.Next())
    {
      // skip any out of range.
      if ((iter->Max()<minLimit) || (iter->Min()>maxLimit))
        continue;

      RealRangeC clipped(iter.Data());
      clipped.ClipBy(displayRange);
      IndexRangeC rng2((clipped.Min() - displayRange.Min()) * scale,
                       (clipped.Max() - displayRange.Min()) * scale);
      IndexRangeC rng1 = displayArea.Range1();
      IndexRange2dC box(rng1, rng2);
      box.Range2().ClipBy(arrowBox.Range2());
      GUIDrawRectangle(GUIDrawGCWhite(),box,true);
    }


    if(markerGc != 0) {
      IndexRangeC horizRange ((atMarker - displayRange.Min())  * scale,
                                          (atMarker+ (m_atSpan) - displayRange.Min()) * scale);
      IndexRange2dC markRange(displayArea.Range1(),
                              horizRange);
      if(markRange.Range2().Size() < 3) {// Make tiny bars are big enough to see.
        markRange.Range2().Expand((3 - markRange.Range2().Size())/2);
      }
      //markRange.Range2().ClipBy(arrowBox.Range2());
      GUIDrawRectangle(markerGc,markRange,true);
    }



    // Draw segment.
//    if(segmentGc != 0 && m_localSegment.Size() > 0 && m_localSegment.IsOverlapping(displayRange)) {
    RealRangeC clippedLocalSegment(m_localSegment);
    clippedLocalSegment.ClipBy(displayRange);
    if(segmentGc != 0 && clippedLocalSegment.Size() > 0 && clippedLocalSegment.IsOverlapping(displayRange)) {
      RealRangeC dispSegment((clippedLocalSegment.Min() - displayRange.Min()) * scale,
                             (clippedLocalSegment.Max() - displayRange.Min()) * scale);
      
      if(dispSegment.Size() == 0) // Make tiny bars at least 1 pixel wide.
        dispSegment.Max()++;
      IndexRange2dC markRange(displayArea.Range1().Shrink(3),
                              dispSegment.IndexRange());
      
      markRange.Range2().ClipBy(arrowBox.Range2());
      if(markRange.Range2().Size() > 0)
        GUIDrawRectangle(segmentGc,markRange,true);
    }
    
    // Draw the centre line
    GUIDrawLine(GUIDrawGCBlack(),Index2dC(midV,minCol),Index2dC(midV,maxCol));

    // Draw the arrow (left)
    GUIDrawLine(GUIDrawGCBlack(), Index2dC(midV, displayArea.Range2().Min() + arrowBorder), Index2dC(vertRange.Min(), minCol - arrowBorder));
    GUIDrawLine(GUIDrawGCBlack(), Index2dC(midV, displayArea.Range2().Min() + arrowBorder), Index2dC(vertRange.Max(), minCol - arrowBorder));
    GUIDrawLine(GUIDrawGCBlack(), Index2dC(vertRange.Min(), minCol - arrowBorder),          Index2dC(vertRange.Max(), minCol - arrowBorder));
    // Draw the arrow (right)
    GUIDrawLine(GUIDrawGCBlack(), Index2dC(midV, displayArea.Range2().Max() - arrowBorder), Index2dC(vertRange.Min(), maxCol + arrowBorder));
    GUIDrawLine(GUIDrawGCBlack(), Index2dC(midV, displayArea.Range2().Max() - arrowBorder), Index2dC(vertRange.Max(), maxCol + arrowBorder));
    GUIDrawLine(GUIDrawGCBlack(), Index2dC(vertRange.Min(), maxCol + arrowBorder),          Index2dC(vertRange.Max(), maxCol + arrowBorder));
    
 
    // Draw segment withing the displayed time
    for( DLIterC< Tuple2C<IntT, RealRangeC> > it(events); it ; it++)
    {
      // skip segments that start after our max, or end before our min.
      if ((it->Data2().Max() < minLimit) || (it->Data2().Min() > maxLimit))
        continue;

      // Truncate the clip to the display range.
      RealRangeC clipped(it->Data2());
      clipped.ClipBy(displayRange);


      IndexRangeC rng2((clipped.Min() - displayRange.Min()) * scale,
                       (clipped.Max() - displayRange.Min()) * scale);
      //      ONDEBUG(cerr << "Elm=" << rng2 << "\n");
      IndexRange2dC box(vertRange, rng2);
      if(box.Range2().Size() == 0) // Make tiny bars at least 1 pixel wide.
        box.Range2().Max()++; 
      box.ClipBy(arrowBox);
      GUIDrawRectangle(GUIDrawGCBlack(),box,true);
    }
#if 0
    IndexC midH = displayArea.Range2().Center();
    GUIDrawLine(GUIDrawGCGrey(),Index2dC(displayArea.Range1().Min(),midH),Index2dC(displayArea.Range1().Max(),midH));
#endif
    return true;
  }

  
}
