// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//////////////////////////////////////////////////////
//! rcsid="$Id$"
//! lib=RavlDPDisplay
//! file="Ravl/GUI/DPDisplay/DPDisplayView.cc"

#include "Ravl/GUI/DPDisplayView.hh"
#include "Ravl/GUI/Manager.hh"
#include "Ravl/GUI/MouseEvent.hh"
#include "Ravl/GUI/LBox.hh"
#include "Ravl/GUI/PackInfo.hh"
#include "Ravl/GUI/DPDisplayImage.hh"

#define DODEBUG 0
#if DODEBUG
#define ONDEBUG(x) x
#else
#define ONDEBUG(x)
#endif

namespace RavlGUIN {
  
  
  //: Default constructor.
  
  DPDisplayViewBodyC::DPDisplayViewBodyC(const IndexRange2dC &size) 
    : TableBodyC(3,5),
      winSize(size),
      refreshQueued(false),
      vRuler(true),
      hRuler(false),
      displaySize(Index2dC(0,0),Index2dC(128,128)), // Setup a default size.
      lastMousePos(-1000,-1000),
      backMenu("BackMenu"),
      m_resizeOnNextObject(false)
  {}
  
  //: Need virtual destructor for class with virtual methods
  
  DPDisplayViewBodyC::~DPDisplayViewBodyC() {
    connections.DisconnectAll();  
  }
  
  //: Create the widget.
  
  bool DPDisplayViewBodyC::Create() {
    static const ByteRGBValueC layerColours[8] =
      { ByteRGBValueC(255,255,255),
	ByteRGBValueC(255,0,0),
	ByteRGBValueC(0,255,0),
	ByteRGBValueC(0,0,255),
	ByteRGBValueC(255,0,255),
	ByteRGBValueC(0,255,255),
	ByteRGBValueC(255,255,0),
	ByteRGBValueC(255,255,255)
      };
	
    ONDEBUG(std::cerr << "DPDisplayViewBodyC::Create(), Called \n");
    
    
    int rows = winSize.Rows();
    int cols = winSize.Cols();
    // Limit maximum inital size.
    if(rows > 800)
      rows = 800;
    if(rows < 30)
      rows = 30;
    if(cols > 700)
      cols = 700;
    if(cols < 30)
       cols = 30;
    
    canvas = GUIMarkupCanvasC(rows,cols);
    
    canvas.GUISetOffset(-Point2dC(displaySize.Origin()));
    
    backMenu.GUIAdd(MenuItemR("Save",*this, &DPDisplayViewBodyC::CallbackStartSave));
    backMenu.GUIAdd(MenuItem("Layer Editor",canvas, &GUIMarkupCanvasC::GUIShowLayerDialog));
    menuBar.GUIAdd(MenuItemR("Save",*this, &DPDisplayViewBodyC::CallbackStartSave));
    menuBar.GUIAdd(MenuItem("Layer Editor",canvas, &GUIMarkupCanvasC::GUIShowLayerDialog));    
    //menuBar.GUIAdd(MenuItemR("Fit To Window",*this, &DPDisplayViewBodyC::CallbackFitToWindow));
    
    fileSelector = FileSelectorC("@X Save");
    connections += ConnectRef(fileSelector.Selected(),*this,&DPDisplayViewBodyC::CallbackSave);
    
    // Setup Display layers.
    MarkupLayerInfoC infoImage(0, "Image", ByteRGBValueC(255, 255, 255), MLI_LineNormal, true);
    infoImage.AddZOrder(0,true);
    canvas.GUIAddLayerInfo(infoImage);
    
    for(int i = 1;i < 8;i++) {
      MarkupLayerInfoC infoMarkup(i, StringC("Markup-") + StringC(i), layerColours[i], MLI_LineNormal, true);
      infoMarkup.AddZOrder(i,true);
      canvas.GUIAddLayerInfo(infoMarkup);
    }
    
    colPos=LabelC("0");
    rowPos=LabelC("0");
    info=LabelC("-");
    //xpos = TextEntryC("",3,false);
    //ypos = TextEntryC("",3,false);
    //info = TextEntryC("",-1,false);
    
    DPDisplayViewC thisH(*this);
    
    TableBodyC::GUIAddObject(menuBar,0,3,0,1,(GtkAttachOptions)(GTK_EXPAND|GTK_FILL|GTK_SHRINK),(GtkAttachOptions) (GTK_SHRINK|GTK_FILL));    
    TableBodyC::GUIAddObject(vRuler,0,1,2,3,GTK_FILL,(GtkAttachOptions) (GTK_EXPAND|GTK_SHRINK|GTK_FILL));
    TableBodyC::GUIAddObject(hRuler,1,2,1,2,(GtkAttachOptions) (GTK_EXPAND|GTK_SHRINK|GTK_FILL),GTK_FILL);
    //TableBodyC::GUIAddObject(vSlider,2,3,2,3,GTK_FILL,(GtkAttachOptions) (GTK_EXPAND|GTK_SHRINK|GTK_FILL));
    //TableBodyC::GUIAddObject(hSlider,1,2,3,4,(GtkAttachOptions) (GTK_EXPAND|GTK_SHRINK|GTK_FILL),GTK_FILL);
    TableBodyC::GUIAddObject(HBox(PackInfoC(Label(" Row="),false,false) + PackInfoC(rowPos,false,false) +
                                  PackInfoC(Label(" Col="),false,false) + PackInfoC(colPos,false,false) + 
                                  PackInfoC(Label("  Value="),false,false) + PackInfoC(info,true,false)),
                             0,3,4,5,
                             (GtkAttachOptions)(GTK_FILL),
                             (GtkAttachOptions)(GTK_FILL));
    
    TableBodyC::GUIAddObject(canvas,1,2,2,3,
                             (GtkAttachOptions)(GTK_FILL|GTK_SHRINK),
                             (GtkAttachOptions)(GTK_FILL|GTK_SHRINK)
                             );
    connections += ConnectRef(canvas.Signal("expose_event"),*this,&DPDisplayViewBodyC::CallbackExpose);
    connections += ConnectRef(canvas.Signal("configure_event"),*this,&DPDisplayViewBodyC::CallbackConfigure,(GdkEvent *) 0);    
    connections += ConnectRef(canvas.Signal("motion_notify_event"),*this,&DPDisplayViewBodyC::CallbackMouseMotion);    
    connections += ConnectRef(canvas.Signal("button_press_event"),*this,&DPDisplayViewBodyC::CallbackMousePress); 
    connections += ConnectRef(canvas.SignalDisplayRange(),*this,&DPDisplayViewBodyC::HandleUpdateDisplayRange);
    if(!TableBodyC::Create())
      return false;
    
    return true;
  }

  //: Resize on display of next object.
  
  bool DPDisplayViewBodyC::SetResizeOnNextObject() {
    RWLockHoldC hold(lockDisplayList,RWLOCK_WRITE);
    m_resizeOnNextObject = true;
    return true;
  }
  
  //: Called when the underlying widget it destroyed.
  // The default version of this method simpily 0's the widget ptr.
  
  void DPDisplayViewBodyC::Destroy() {
    connections.DisconnectAll();  
    TableBodyC::Destroy();
  }
  
  //: Update range.
  
  bool DPDisplayViewBodyC::HandleUpdateDisplayRange(RealRange2dC &rng) {
    //cerr << "DPDisplayViewBodyC::HandleUpdateDisplayRange(),  \n";
    //GUIUpdateRuler();    
    return true;
  }
  
  //: Add object to the display list.

  bool DPDisplayViewBodyC::AddObject(const DPDisplayObjC &obj) {
    ONDEBUG(std::cerr << "DPDisplayViewBodyC::AddObject(), Called \n");
    RWLockHoldC hold(lockDisplayList,RWLOCK_WRITE);
    if(displayList.IsEmpty()) {
      displaySize = obj.Frame();
      if(m_resizeOnNextObject) {
        if(canvas.IsValid()) {
          canvas.SetOffset(-Point2dC(displaySize.Origin()));
          canvas.SetScale(Vector2dC(1.0,1.0));
        }
        
        m_resizeOnNextObject = false;
      }
      
    } else
      displaySize.Involve(obj.Frame());
    displayList.InsFirst(obj);
    hold.Unlock();
    Refresh();
    return true;
  }
  
  //: Clear the display list.
  
  bool DPDisplayViewBodyC::Clear() {
    ONDEBUG(std::cerr << "DPDisplayViewBodyC::Clear(), Called \n");
    RWLockHoldC hold(lockDisplayList,RWLOCK_WRITE);
    displayList.Empty(); 
    return true;
  }
  
  //: Refresh the display.
  
  bool DPDisplayViewBodyC::Refresh() {
    ONDEBUG(std::cerr << "DPDisplayViewBodyC::Refresh(), Called \n");
    if(refreshQueued)
      return true;
    refreshQueued = true;
    Manager.Queue(Trigger(DPDisplayViewC(*this),&DPDisplayViewC::GUIRefresh));
    return true;
  }
  
  //: Refresh the display.
  
  bool DPDisplayViewBodyC::GUIRefresh() {
    ONDEBUG(std::cerr << "DPDisplayViewBodyC::GUIRefresh(), Called \n");
    if(!canvas.IsValid() || canvas.Widget() == 0) {
      refreshQueued = false;
      return false;
    }
    FrameMarkupC frameMarkup;
    RWLockHoldC hold(lockDisplayList,RWLOCK_READONLY);
    refreshQueued = false;
    for(DLIterC<DPDisplayObjC> it(displayList);it;it++)
      it->Draw(frameMarkup);
    hold.Unlock();
    canvas.GUIUpdateMarkup(frameMarkup);
    UpdateInfo(lastMousePos);
    return true;
  }

  //: Update ruler info.
  
  bool DPDisplayViewBodyC::GUIUpdateRuler() {
    ONDEBUG(std::cerr << "DPDisplayViewBodyC::UpdateRuler(), Called. \n");
    Point2dC origin = canvas.GUI2World(Point2dC(0,0));
    Point2dC canSize = canvas.GUI2World(canvas.Size());
    vRuler.GUISetRangeAndPosition(origin[0],canSize[0],lastMousePos[0]);
    hRuler.GUISetRangeAndPosition(origin[1],canSize[1],lastMousePos[1]);
    return true;
  }
  
  //: Update ruler info.
  
  bool DPDisplayViewBodyC::UpdateRuler() {
    Manager.QueueOnGUI(Trigger(DPDisplayViewC(*this),&DPDisplayViewC::GUIUpdateRuler));    
    return true;
  }
  
  //: Query position,
  
  bool DPDisplayViewBodyC::Query(Vector2dC pos,StringC &info) { 
    //ONDEBUG(std::cerr <<"DPDisplayViewBodyC::Query(), Position=" << pos << "\n");
    RWLockHoldC hold(lockDisplayList,RWLOCK_READONLY); // To protect access to displaySize.
    for(DLIterC<DPDisplayObjC> it(displayList);it;it++) {
      if(it->Query(pos,info)) {
	//ONDEBUG(std::cerr <<"DPDisplayViewBodyC::Query(), Position=" << pos << " Result=" << info <<"\n");
	return true;
      }
    }
    //ONDEBUG(std::cerr <<"DPDisplayViewBodyC::Query(), Position=" << pos << " No result.\n");
    return false;
  }
  
  //: Handle an expose event.
  
  bool DPDisplayViewBodyC::CallbackExpose(GdkEvent *&event) {
    ONDEBUG(std::cerr << "DPDisplayViewBodyC::Expose(), Called \n");
#if 0
    // The markup canvas handles this itself.
    RWLockHoldC hold(lockDisplayList,RWLOCK_READONLY);
    FrameMarkupC frameMarkup;
    for(DLIterC<DPDisplayObjC> it(displayList);it;it++)
      it->Draw(frameMarkup);
    hold.Unlock();
    canvas.GUIUpdateMarkup(frameMarkup);    
#endif
    return true;
  }

  //: Handle configure callback.
  
  bool DPDisplayViewBodyC::CallbackConfigure(GdkEvent *&event) {
    ONDEBUG(std::cerr << "DPDisplayViewBodyC::CallbackConfigure(), Called. \n");
    GUIUpdateRuler();
    return true;
  }
  
  //: Call back for mouse movements in the window.
  
  bool DPDisplayViewBodyC::CallbackMouseMotion(MouseEventC &mouseEvent) {
    Index2dC idx ( mouseEvent.At() ) ;  // gets the position of the mose event in RAVL co-ordinates not GTK
    Vector2dC pos = canvas.GUI2World(idx) ; 
    
    UpdateInfo(pos);
    lastMousePos = pos;
    GUIUpdateRuler();
    return true;
  }
  
  //: Call back for mouse press events.
  
  bool DPDisplayViewBodyC::CallbackMousePress(MouseEventC &mouseEvent) {
    ONDEBUG(std::cerr << "DPDisplayViewBodyC::CallbackMousePress(), Called. \n");
    if(mouseEvent.HasChanged(2))
      backMenu.Popup();
    return true;
  }

  //: Fit image to window.
  
  bool DPDisplayViewBodyC::CallbackFitToWindow() {
    if(canvas.IsValid())
      canvas.FitImageToScreen();
    return true;
  }
  
  //: Update info for mouse position.
  
  bool DPDisplayViewBodyC::UpdateInfo(const Vector2dC &at) {
    Vector2dC pos = at; // ravl co-ords
    StringC rowps((IndexC) pos[0]);
    rowPos.Label(rowps);
    StringC colps((IndexC) pos[1]);
    colPos.Label(colps);
    StringC infos("-");
    Query(pos,infos);
    info.Label(infos);
    return true;
  }

  //: Start image save.
  
  bool DPDisplayViewBodyC::CallbackStartSave() {
    ONDEBUG(std::cerr << "DPDisplayViewBodyC::CallbackStartSave(), Called. \n");
    fileSelector.Show();
    return true;
  }

  //: Save image to a file.
  
  bool DPDisplayViewBodyC::CallbackSave(StringC &str) {
    ONDEBUG(std::cerr << "DPDisplayViewBodyC::CallbackSave(), Called. File='" << str << "' \n");
    for(DLIterC<DPDisplayObjC> it(displayList);it;it++) {
      if(it->Save(str))
	break;
    }
    return true;
  }

}
