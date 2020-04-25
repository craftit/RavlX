// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2004, OmniPerception Ltd. 
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//! rcsid="$Id$"
//! lib=RavlLibGlade
//! file="Ravl/GUI/LibGlade/exLibGladeCustom.cc"
//! docentry="Ravl.API.Graphics.GTK.LibGlade"

#include "Ravl/GUI/Window.hh"
#include "Ravl/GUI/Button.hh"
#include "Ravl/GUI/Manager.hh"
#include "Ravl/GUI/Menu.hh"
#include "Ravl/GUI/GladeWidget.hh"
#include "Ravl/Threads/Signal.hh"

using namespace RavlGUIN;

//#===--- Custom widget class

class CustomWidgetBodyC :
  public GladeWidgetBodyC
{
public:
  CustomWidgetBodyC() :
    GladeWidgetBodyC("vbox1", true),
    m_signal(true)    
  {
    // Demonstrate deferred GladeXML set
    SetXML(GladeXMLC(PROJECT_OUT "/share/RAVL/Glade/exlibgladecustomwidget.glade"));
  }
  //: Constructor.
  
  virtual ~CustomWidgetBodyC() { }
  //: Virtual destructor

  virtual bool Create()
  {
    AddObject("button1", ButtonR("custom", *this, &CustomWidgetBodyC::OnClicked));
    return GladeWidgetBodyC::Create();
  }
  //: Create the widget.
  
  virtual bool Create(GtkWidget *widget)
  {
    AddObject("button1", ButtonR("custom", *this, &CustomWidgetBodyC::OnClicked));
    return GladeWidgetBodyC::Create(widget);
  }
  //: Create with a widget supplied from elsewhere.
  
  bool OnClicked()
  {
    m_signal();
    return true;
  }
  
  Signal0C &SigClicked()
  { return m_signal; }
  
protected:
  Signal0C m_signal;
};

class CustomWidgetC :
  public GladeWidgetC
{
public:
  CustomWidgetC()
  {}
  //: Default constructor.
  // Creates an invalid handle.

  CustomWidgetC(bool) :
    GladeWidgetC(*new CustomWidgetBodyC())
  {}
  //: Constructor.
  
  Signal0C &SigClicked()
  { return Body().SigClicked(); }
  
protected:    
  CustomWidgetC(CustomWidgetBodyC &body) :
    GladeWidgetC(body)
  {}
  //: Body constructor.
  
  CustomWidgetBodyC &Body()
  { return static_cast<CustomWidgetBodyC &>(WidgetC::Body()); }
  //: Access body.
  
  const CustomWidgetBodyC &Body() const
  { return static_cast<const CustomWidgetBodyC &>(WidgetC::Body()); }
  //: Access body.
};
  
//#===--- Application callbacks

bool OnPress1() {
  std::cerr << "Pressed 1. \n";
  return true;
}

bool OnPressC1() {
  std::cerr << "Pressed custom1. \n";
  return true;
}

bool OnPressC2() {
  std::cerr << "Pressed custom2. \n";
  return true;
}

bool OnQuit() 
{
  Manager.Quit(); // Initate shutdown.
  return true;
}

//#===--- Main application

int main(int nargs,char **argv) {
  Manager.Init(nargs,argv);
  
  std::cerr << "Create widgets." << std::endl;

  // Create the main window
  GladeXMLC glade(PROJECT_OUT "/share/RAVL/Glade/exlibgladecustommain.glade");
  GladeWidgetC mainWindow(glade, "window1");
  Connect(mainWindow.Signal("delete_event"), &OnQuit);

  // Create the custom widgets
  CustomWidgetC custom1(true);
  CustomWidgetC custom2(true);
  Connect(custom1.SigClicked(), &OnPressC1);
  Connect(custom2.SigClicked(), &OnPressC2);
  
  // Add the widgets
  mainWindow.AddObject("button1",Button("Test",&OnPress1));
  mainWindow.AddObject("quit1",MenuItem("Quit",&OnQuit));
  mainWindow.AddObject("custom1",custom1);
  mainWindow.AddObject("custom2",custom2);
  
  std::cerr << "Show widgets. \n";
  
  mainWindow.GUIShow();
  
  std::cerr << "Start GUI. \n";
  
  Manager.Start();

  std::cerr << "Done. \n";
  
  return 0;
}
