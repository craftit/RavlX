// This file is part of RAVL, Recognition And Vision Library
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVLGUI_WIDGET_HEADER
#define RAVLGUI_WIDGET_HEADER 1
/////////////////////////////////////////////////
//! file="Ravl/GUI/GTK/Widget.hh"
//! lib=RavlGUI
//! author="Charles Galambos"
//! date="23/03/1999"
//! userlevel=Normal
//! docentry="Ravl.API.Graphics.GTK.Internal"

#include "Ravl/Threads/Signal1.hh"
#include "Ravl/DList.hh"
#include "Ravl/GUI/GTKTypes.hh"
#include "Ravl/Index2d.hh"
#include "Ravl/IndexRange2d.hh"
#include "Ravl/Hash.hh"
#include "Ravl/String.hh"
#include "Ravl/GUI/WidgetStyle.hh"
#include <gtk/gtkenums.h>

// Namespace fix for Visual C++
#if RAVL_COMPILER_VISUALCPP6
#ifndef RavlGUIN
#error EEeek!
#define RavlGUIN RavlN
#endif
#endif

namespace RavlN {
  class XMLFactoryContextC;
}

namespace RavlImageN {
  class ByteRGBValueC;
}

namespace RavlGUIN {
  using namespace RavlImageN;
  using namespace RavlN;

  class WidgetC;
  class TreeBodyC;
  class IndexRectangleC;
  class MenuC;
  struct GTKSignalInfoC;
  class TargetEntryC;
  class WidgetDndInfoC;
  class WidgetStyleC;
  class DNDDataInfoC;

  //! userlevel=Develop
  //: Widget base body.

  class WidgetBodyC
    : public RCBodyVC
  {
  public:
    WidgetBodyC();
    //: Default constructor.
    // reg == true then register widget for service events.

    WidgetBodyC(const char *ntooltip);
    //: Default constructor.
    // reg == true then register widget for service events.

    WidgetBodyC(const XMLFactoryContextC &factory);
    //: Factory constructor

    virtual ~WidgetBodyC();
    //: Destructor.

    virtual bool Create();
    //: Create the widget.

    virtual bool Create(GtkWidget *widget);
    //: Create with a widget supplied from elsewhere.

    GtkWidget *Widget() { return widget; }
    //: Access widget.

    int WidgetID() const { return widgetId; }
    //: Get widget id.

    StringC GUIWidgetName() const;
    //: Get widget's name.
    // Call only from GUI thread.

    virtual StringC Name() const;
    //: Get name of widget.
    // Defaults to WidgetName(), but may be differnt
    // for Lables etc.

    bool Show();
    //: Show widget to the world.
    // Thread safe.

    bool Hide();
    //: Hide widget from the world.
    // Thread safe.

    Index2dC Position() const;
    //: Widget position within its parent window.

    Index2dC Size() const;
    //: Access Size of wiget.

    bool SetUSize(IntT x,IntT y);
    //: Set size of widget.
    // Setting values to -1 will leave it value unspecified

    IndexRange2dC Rectangle() const;
    //: Widget position and extent within its parent window.

    void SetState(GtkStateType state);
    //: Set state
    // Values for state are listed <A HREF="http://library.gnome.org/devel/gtk/stable/gtk-Standard-Enumerations.html#GTKSTATETYPE">here</A>.

    virtual void WidgetDestroy();
    //: Called when the underlying widget it destroyed.
    // The default version of this method simpily 0's the widget ptr.

    Signal0C &Signal(const char *);
    //: Get handle for named signal.
    // See handle class for description.

    void SetToolTip(const char *text,const char *ctxt = 0);
    //: Set the tool tip for the widget.
    // NB. Not all widgets can display tooltips.

    void AddEventMask(IntT event);
    //: Add to the event mask.

    void DelEventMask(IntT event);
    //: Remove from the event mask.

    bool GUIShapeCombineMask(GdkBitmap *mask,int off_x = 0,int off_y = 0);
    //: Make a shape mask for the widget.

    void SetStyle(WidgetStyleC& style);
    //: Set style of widget.

    bool SetUPosition(int &width, int &height);
    //: Set the widget position

    static SArray1dC<GtkTargetEntry> CommonTargetEntries();
    //: Get a list of common target entries.
    // name="text/plain" info=0 <br>
    // name="text/uri-list" info=1 <br>
    // name="STRING" info=2 <br>

    virtual bool GUIShow();
    //: Show widget to the world.
    // Call only from GUI thread.

    virtual bool GUIHide();
    //: Hide widget from the world.
    // Call only from GUI thread.

    bool SetBackgroundColour(const ByteRGBValueC &colour,GtkStateType &state);
    //: Set background colour.

    bool SetForgroundColour(const ByteRGBValueC &colour,GtkStateType &state);
    //: Set background colour.

    bool SetTextColour(const ByteRGBValueC &colour,GtkStateType &state);
    //: Set text colour.

    bool SetBaseColour(const ByteRGBValueC &colour,GtkStateType &state);
    //: Set base colour.

    virtual bool GUISetTextColour(const ByteRGBValueC &colour,GtkStateType &state);
    //: Set text colour.

    virtual bool GUISetBaseColour(const ByteRGBValueC &colour,GtkStateType &state);
    //: Set base colour.

    virtual bool GUISetBackgroundColour(const ByteRGBValueC &colour,GtkStateType &state);
    //: Set background colour.

    virtual bool GUISetForgroundColour(const ByteRGBValueC &colour,GtkStateType &state);
    //: Set forground colour.

    static Tuple2C<const char *,GTKSignalInfoC> *SigInfoInit();
    //: Get init information about signals.

    static GTKSignalInfoC &SigInfo(const char *nm) ;
    //: Get information about a named signal.

    void GrabFocus();
    //: Grab keyboard focus.

    bool GUIIsFocus();
    //: Is the widget the focus?

  protected:

    virtual bool GUISetStyle(WidgetStyleC& style);
    //: Set style of widget.

    bool GUISetState(GtkStateType state);
    //: Set state of widget.
    // Values for state are listed <A HREF="http://library.gnome.org/devel/gtk/stable/gtk-Standard-Enumerations.html#GTKSTATETYPE">here</A>.

    bool GUISetUPosition(int &width, int &height);
    //: Set the widget position

    bool GUISetUSize(IntT& x,IntT& y);
    //: Set size of widget.
    // GUI thread only.
    // Setting values to -1 will leave it value unspecified

    void GUIGrabFocus();
    //: Grab keyboard focus.

    virtual bool GUIDNDSource(ModifierTypeT flags,const SArray1dC<GtkTargetEntry> &entries,DragActionT actions);
    //: Setup widget as drag and drop source.

    virtual bool GUIDNDSourceDisable();
    //: Disable widget as a drag and drop source.

    virtual bool GUIDNDTarget(DestDefaultsT flags,const SArray1dC<GtkTargetEntry> &entries,DragActionT actions);
    //: Setup widget as drag and drop target.

    virtual bool GUIDNDTargetDisable();
    //: Disable widget as a drag and drop source.

    bool GUIDNDBegin(IntT button,GdkEvent *event);
    //: Initiate a drag operation.
    //!param: button - Button that started the drag.
    //!param: event - Event that started the drag.
    //!return: true, Drag started ok.

    virtual void Destroy();
    //: Undo all references.
    // Used to avoid problems with circluar references.
    // This should undo all references to RAVL and GUI
    // objects which are not in the destroy tree.
    // The function is called by the root window in its
    // destructor.

    void ConnectSignals();
    //: Connect the default signals to the underly widget.

    IntT ConnectUp(const char *nm,Signal0C &sig);
    //: Connect up a signal.

    HashC<StringC,Tuple2C<Signal0C,IntT> > signals;
    //: Table of created signals.

    GtkWidget *widget;
    int widgetId;      // Id for widget.

    GtkStateType reqState; // requested state.
    IntT  eventMask; // Event mask for widget.

    StringC tooltip;

    bool gotRef; // Do we have a reference to the object.

    WidgetDndInfoC *dndInfo; // Drag and drop info.

    IntT destroySigId;
    
    WidgetStyleC m_style;
    
  private:
    WidgetBodyC(const WidgetBodyC &)
    { RavlAssertMsg(0,"WidgetBodyC's can't be copied. "); }
    //: Disable copy constructor.

    // Some call backs.

    static int gtkDestroy(GtkWidget *widget,WidgetBodyC * data);
    static int gtkEvent(GtkWidget *widget,GdkEvent *event,Signal0C *data);
    static int gtkEventDelete(GtkWidget *widget,GdkEvent *event,Signal0C *data);
    static int gtkEventMouseButton(GtkWidget *widget,GdkEvent *event,Signal0C *data);
    static int gtkEventMouseMotion(GtkWidget *widget,GdkEvent *event,Signal0C *data);
    static int gtkEventScroll(GtkWidget *widget,GdkEvent *event,Signal0C *data);
    static int gtkEventKeyboard(GtkWidget *widget,GdkEvent *event,Signal0C *data);
    static int gtkGeneric(GtkWidget *widget,Signal0C *data);
    static int gtkString(GtkWidget *widget,Signal0C *data);
    static int gtkEventFocus(GtkWidget *widget,GdkEvent *focus,Signal0C *data);

    static int gtkCListSelect(GtkWidget *widget,IntT row,IntT column,GdkEventButton *event,Signal0C *data);
    static int gtkCListCol(GtkWidget *widget,IntT column,Signal0C *data);

    static int gtkDNDContext(GtkWidget *widget,GdkDragContext *context,Signal0C *data);
    static int gtkDNDContextTime(GtkWidget *widget,GdkDragContext *context,unsigned int time,Signal0C *data);
    static int gtkDNDPosition(GtkWidget *widget,GdkDragContext *context,int x,int y,unsigned int time,Signal0C *data);

    static int gtkDNDDataGet(GtkWidget *widget,GdkDragContext *context,GtkSelectionData *sel,unsigned int info,unsigned int time,Signal0C *data);
    static int gtkDNDDataRecieved(GtkWidget *widget,GdkDragContext *context,int x,int y,GtkSelectionData *sel,unsigned int info,unsigned int time,Signal0C *data);
    static int gtkInt(GtkWidget *widget,int page, Signal0C *sigptr);
    static int gtkWidgetInt(GtkWidget *widget,GtkWidget *widgetarg, unsigned int page, Signal0C *sigptr);

    static int gtkTreeRow(GtkWidget *widget, GtkTreeIter *iter, GtkTreePath *path, Signal0C *sigptr);
    static int gtkTreePathCol(GtkWidget *widget, GtkTreePath *path, GtkTreeViewColumn *col, Signal0C *sigptr);
    static int gtkTreePath(GtkWidget *widget, GtkTreePath *path,Signal0C *sigptr);

    friend class ManagerC;
    friend class WidgetC;
    friend class WindowC;
    friend struct GTKSignalInfoC;
    friend class ComboBodyC;
  };

  //! userlevel=Normal
  //: Base Widget handle.

  class WidgetC
    : public RCHandleC<WidgetBodyC>
  {
  public:
    WidgetC()
    {}
    //: Default constructor.

    WidgetC(const WidgetC &oth)
      : RCHandleC<WidgetBodyC>(oth)
    {}
    //: Copy constructor.

    WidgetC(bool)
      : RCHandleC<WidgetBodyC>(*new WidgetBodyC())
    {}
    //: Create an abstract widget handle.

    WidgetC(const WidgetBodyC *bod)
      : RCHandleC<WidgetBodyC>(bod)
    {}
    //: Body Constructor.

    WidgetC(const XMLFactoryContextC &factory)
      : RCHandleC<WidgetBodyC>(*new WidgetBodyC(factory))
    {}
    //: Factory constructor

  protected:
    WidgetC(WidgetBodyC &bod)
      : RCHandleC<WidgetBodyC>(bod)
    {}
    //: Body Constructor.


    WidgetBodyC &Body()
    { return RCHandleC<WidgetBodyC>::Body(); }
    //: Access body.

    const WidgetBodyC &Body() const
    { return RCHandleC<WidgetBodyC>::Body(); }
    //: Access body.

  public:

    bool GUISetState(GtkStateType state)
    { return Body().GUISetState(state); }
    //: Set state of widget.
    // Values for state are listed <A HREF="http://library.gnome.org/devel/gtk/stable/gtk-Standard-Enumerations.html#GTKSTATETYPE">here</A>.

    bool GUISetStyle(WidgetStyleC &style)
    { return Body().GUISetStyle(style); }
    //: Set style of widget.

    bool GUISetUSize(IntT x,IntT y)
    { return Body().GUISetUSize(x,y); }
    //: Set size of widget.
    // GUI thread only.
    // Setting values to -1 will leave it value unspecified

    bool GUISetUPosition(int width, int height)
    { return Body().GUISetUPosition(width, height); }
    //: Set the widget position

    void GrabFocus()
    { Body().GrabFocus(); }
    //: Grab keyboard focus

    void GUIGrabFocus()
    { Body().GUIGrabFocus(); }
    //: Grab keyboard focus.

    bool GUIIsFocus()
    { return Body().GUIIsFocus(); }
    //: Is the widget the focus?

    bool GUIDNDSource(ModifierTypeT flags,const SArray1dC<GtkTargetEntry> &entries,DragActionT actions)
    { return Body().GUIDNDSource(flags,entries,actions); }
    //: Setup widget as drag and drop source.
    // e.g. GDK_BUTTON1_MASK GDK_BUTTON2_MASK or GDK_BUTTON3_MASK

    bool GUIDNDSourceDisable()
    { return Body().GUIDNDSourceDisable(); }
    //: Disable widget as a drag and drop source.

    bool GUIDNDTarget(DestDefaultsT flags,const SArray1dC<GtkTargetEntry> &entries,DragActionT actions)
    { return Body().GUIDNDTarget(flags,entries,actions); }
    //: Setup widget as drag and drop target.

    bool GUIDNDTargetDisable()
    { return Body().GUIDNDTargetDisable(); }
    //: Disable widget as a drag and drop source.

    bool GUIDNDBegin(IntT button,GdkEvent *event)
    { return Body().GUIDNDBegin(button,event); }
    //: Initiate a drag operation.
    //!param: button - Button that started the drag.
    //!param: event - Event that started the drag.
    //!return: true : Drag started ok.

    bool GUIShow()
    { return Body().GUIShow(); }
    //: Show widget on the display.
    // Call only from GUI thread.

    bool GUIHide()
    { return Body().GUIHide(); }
    //: Hide widget on the display.
    // Call only from GUI thread.

    void Destroy()
    { Body().Destroy(); }
    //: See body class for details.

    GtkWidget *Widget()
    { return Body().Widget(); }
    //: Access widget.

    StringC GUIWidgetName() const
    { return Body().GUIWidgetName(); }
    //: Get widget's name.
    // Call only from GUI thread.

    StringC Name() const
    { return Body().Name(); }
    //: Get name of widget.
    // Defaults to WidgetName(), but may be differnt
    // for Lables etc.

    int WidgetID() const
    { return Body().WidgetID(); }
    //: Get widget id.

    bool Create()
    { return Body().Create(); }
    //: Create the widget.

    bool Create(GtkWidget *widget)
    { return Body().Create(widget); }
    //: Create with a widget supplied from elsewhere.

    bool Show()
    { return Body().Show(); }
    //: Show widget on the display.
    // Thread safe.

    bool Hide()
    { return Body().Hide(); }
    //: Hide widget on the display.
    // Thread safe.

    Index2dC Position() const
    { return Body().Position(); }
    //: Widget position.

    Index2dC Size() const
    { return Body().Size(); }
    //: Size of wiget in pixels

    bool SetUSize(IntT x,IntT y)
    { return Body().SetUSize(x,y); }
    //: Set size of widget.
    // Setting values to -1 will leave it value unspecified

    void SetState(GtkStateType state)
    { Body().SetState(state); }
    //: Set state
    // Values for state are listed <A HREF="http://library.gnome.org/devel/gtk/stable/gtk-Standard-Enumerations.html#GTKSTATETYPE">here</A>.

    void SetStyle(WidgetStyleC& style)
    { Body().SetStyle(style); }

    IndexRange2dC Rectangle() const
    { return Body().Rectangle(); }
    //: Widget position and extent within its parent window.

    DListC<WidgetC> operator+ (const WidgetC &widge) const {
      DListC<WidgetC> ret;
      ret.InsLast(*this);
      ret.InsLast(widge);
      return ret;
    }
    //: Add a widget into a list.

    Signal0C &Signal(const char *nm)
    { return Body().Signal(nm); }
    //: Get handle for named signal.
    // Signal is created and connected when this
    // function is first called. There after the same
    // signal is returned.<p>
    // If the signal is unrecognised an invalid handle
    // is returned. <p>
    // The exact signal type depends on the signal requested,
    // if not otherwise specified it is a Signal1C<WidgetC>.

    void SetToolTip(const char *text,const char *ctxt = 0)
    { Body().SetToolTip(text,ctxt); }
    //: Set the tool tip for the widget.
    // NB. Not all widgets can display tooltips.

    void AddEventMask(IntT event)
    { Body().AddEventMask(event); }
    //: Add to the event mask.

    void DelEventMask(IntT event)
    { Body().DelEventMask(event); }
    //: Remove from the event mask.

    bool GUIShapeCombineMask(GdkBitmap *mask,int off_x = 0,int off_y = 0)
    { return Body().GUIShapeCombineMask(mask,off_x,off_y); }
    //: Make a shape mask for the widget.

    bool SetUPosition(int width, int height)
    { return Body().SetUPosition(width, height); }
    //: Set the widget position

    friend class WidgetBodyC;
    friend class ManagerC;
    friend class WindowC;
    friend class MenuC; // Hack to make IsA() work. What to do about this ?
    friend class TreeBodyC; // Another hack

    bool SetBackgroundColour(const ByteRGBValueC &colour,GtkStateType state = GTK_STATE_NORMAL)
    { return Body().SetBackgroundColour(const_cast<ByteRGBValueC &>(colour),state); }
    //: Set the widgets background colour.

    bool SetForgroundColour(const ByteRGBValueC &colour,GtkStateType state  = GTK_STATE_NORMAL)
    { return Body().SetForgroundColour(const_cast<ByteRGBValueC &>(colour),state); }
    //: Set the widgets forground colour.

    bool SetTextColour(const ByteRGBValueC &colour,GtkStateType state)
    { return Body().SetTextColour(colour,state); }
    //: Set text colour.

    bool SetBaseColour(const ByteRGBValueC &colour,GtkStateType state)
    { return Body().SetBaseColour(colour,state); }
    //: Set base colour.

    bool GUISetTextColour(const ByteRGBValueC &colour,GtkStateType state)
    { return Body().GUISetTextColour(colour,state); }
    //: Set text colour.

    bool GUISetBaseColour(const ByteRGBValueC &colour,GtkStateType state)
    { return Body().GUISetBaseColour(colour,state); }
    //: Set base colour.

    bool GUISetBackgroundColour(const ByteRGBValueC &colour,GtkStateType state)
    { return Body().GUISetBackgroundColour(colour,state); }
    //: Set the widgets background colour.
    // GUI thread only.

    bool GUISetForgroundColour(const ByteRGBValueC &colour,GtkStateType state)
    { return Body().GUISetForgroundColour(colour,state); }
    //: Set the widgets forground colour.
    // GUI thread only.

  };

  std::ostream &operator<<(std::ostream &strm,const WidgetC &out);
  //: Dummy function to keep templates happy.

  std::istream &operator>>(std::istream &strm,WidgetC &out);
  //: Dummy function to keep templates happy.

  BinOStreamC &operator<<(BinOStreamC &strm,const WidgetC &out);
  //: Dummy function to keep templates happy.

  BinIStreamC &operator>>(BinIStreamC &strm,WidgetC &out);
  //: Dummy function to keep templates happy.

  inline
  DListC<WidgetC> operator+ (DListC<WidgetC> lst,const WidgetC &widge) {
    lst.InsLast(widge);
    return lst;
  }
  //! docentry="Ravl.API.Graphics.GTK.Layout"
  //! userlevel=Normal
  //: Add a widget into a widget list.

  void GUIBeep();
  //: Emit a short beep

}
#endif
