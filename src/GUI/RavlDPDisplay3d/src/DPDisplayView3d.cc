// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2008, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//////////////////////////////////////////////////////
//! rcsid="$Id$"
//! lib=RavlDPDisplay3d
//! file="Ravl/GUI/DPDisplay/DPDisplayView3d.cc"

#include "Ravl/GUI/DPDisplayView3d.hh"
#include "Ravl/GUI/Manager.hh"
#include "Ravl/GUI/MouseEvent.hh"
#include "Ravl/GUI/LBox.hh"
#include "Ravl/GUI/PackInfo.hh"

#define DODEBUG 0
#if DODEBUG
#define ONDEBUG(x) x
#else
#define ONDEBUG(x)
#endif

namespace RavlGUIN {
  
  //: Default constructor.
  
  DPDisplayView3dBodyC::DPDisplayView3dBodyC(const IndexRange2dC &size) 
    : TableBodyC(3,5),
      winSize(size),
      refreshQueued(false),
      vRuler(true),
      hRuler(false),
      displaySize(Index2dC(0,0),Index2dC(512,512)), // Setup a default size.
      lastMousePos(-1000,-1000),
      m_resizeOnNextObject(false)
  {}
  
  //: Need virtual destructor for class with virtual methods
  
  DPDisplayView3dBodyC::~DPDisplayView3dBodyC() {
    connections.DisconnectAll();  
  }
  
  //: Create the widget.
  
  bool DPDisplayView3dBodyC::Create() {    
    ONDEBUG(cerr << "DPDisplayView3dBodyC::Create(), Called \n");
    
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
    
    m_view3d = View3DC(cols,rows);
    
#if 0
    // Stuff we may want to make functional at some point.
    menuBar.GUIAdd(MenuItemR("Save",*this, &DPDisplayView3dBodyC::CallbackStartSave));
    fileSelector = FileSelectorC("@GL Save");
    connections += ConnectRef(fileSelector.Selected(),*this,&DPDisplayView3dBodyC::CallbackSave);
#endif
    
    colPos=LabelC("0");
    rowPos=LabelC("0");
    info=LabelC("-");
    
    DPDisplayView3dC thisH(*this);
    
    TableBodyC::GUIAddObject(menuBar,0,3,0,1,(GtkAttachOptions)(GTK_EXPAND|GTK_FILL|GTK_SHRINK),(GtkAttachOptions) (GTK_SHRINK|GTK_FILL));    
    TableBodyC::GUIAddObject(vRuler,0,1,2,3,GTK_FILL,(GtkAttachOptions) (GTK_EXPAND|GTK_SHRINK|GTK_FILL));
    TableBodyC::GUIAddObject(hRuler,1,2,1,2,(GtkAttachOptions) (GTK_EXPAND|GTK_SHRINK|GTK_FILL),GTK_FILL);
    TableBodyC::GUIAddObject(HBox(PackInfoC(Label(" Row="),false,false) + PackInfoC(rowPos,false,false) +
                                  PackInfoC(Label(" Col="),false,false) + PackInfoC(colPos,false,false) + 
                                  PackInfoC(Label("  "),false,false) + PackInfoC(info,true,false)),
                             0,3,4,5,
                             (GtkAttachOptions)(GTK_FILL),
                             (GtkAttachOptions)(GTK_FILL));
    
    TableBodyC::GUIAddObject(m_view3d,1,2,2,3,
                             (GtkAttachOptions)(GTK_FILL|GTK_SHRINK),
                             (GtkAttachOptions)(GTK_FILL|GTK_SHRINK)
                             );
    connections += ConnectRef(m_view3d.Signal("configure_event"),*this,&DPDisplayView3dBodyC::CallbackConfigure,(GdkEvent *) 0);    
    connections += ConnectRef(m_view3d.Signal("motion_notify_event"),*this,&DPDisplayView3dBodyC::CallbackMouseMotion);    
    connections += ConnectRef(m_view3d.Signal("button_press_event"),*this,&DPDisplayView3dBodyC::CallbackMousePress); 
    if(!TableBodyC::Create())
      return false;
    
    return true;
  }

  //: Resize on display of next object.
  
  bool DPDisplayView3dBodyC::SetResizeOnNextObject() {
    RWLockHoldC hold(lockDisplayList,RWLOCK_WRITE);
    m_resizeOnNextObject = true;
    return true;
  }
  
  //: Called when the underlying widget it destroyed.
  // The default version of this method simpily 0's the widget ptr.
  
  void DPDisplayView3dBodyC::Destroy() {
    connections.DisconnectAll();  
    TableBodyC::Destroy();
  }
  
  //: Update range.
  
  bool DPDisplayView3dBodyC::HandleUpdateDisplayRange(RealRange2dC &rng) {
    //cerr << "DPDisplayView3dBodyC::HandleUpdateDisplayRange(),  \n";
    //GUIUpdateRuler();    
    return true;
  }
  
  //: Add object to the display list.

  bool DPDisplayView3dBodyC::AddObject(const DObject3DC &obj) {
    ONDEBUG(cerr << "DPDisplayView3dBodyC::AddObject(), Called \n");
    RWLockHoldC hold(lockDisplayList,RWLOCK_WRITE);
    displayList.InsFirst(obj);
    hold.Unlock();
    Refresh();
    return true;
  }
  
  //: Clear the display list.
  
  bool DPDisplayView3dBodyC::Clear() {
    ONDEBUG(cerr << "DPDisplayView3dBodyC::Clear(), Called \n");
    RWLockHoldC hold(lockDisplayList,RWLOCK_WRITE);
    displayList.Empty(); 
    return true;
  }
  
  //: Refresh the display.
  
  bool DPDisplayView3dBodyC::Refresh() {
    ONDEBUG(cerr << "DPDisplayView3dBodyC::Refresh(), Called \n");
    if(refreshQueued)
      return true;
    refreshQueued = true;
    Manager.Queue(Trigger(DPDisplayView3dC(*this),&DPDisplayView3dC::GUIRefresh));
    return true;
  }
  
  //: Refresh the display.
  
  bool DPDisplayView3dBodyC::GUIRefresh() {
    ONDEBUG(cerr << "DPDisplayView3dBodyC::GUIRefresh(), Called \n");
    if(!m_view3d.IsValid() || m_view3d.Widget() == 0) {
      refreshQueued = false;
      return false;
    }
    DObjectSet3DC objectSet(true);
    RWLockHoldC hold(lockDisplayList,RWLOCK_READONLY);
    refreshQueued = false;
    for(DLIterC<DObject3DC> it(displayList);it;it++) {
      objectSet += *it;
    }
    hold.Unlock();
    m_view3d.GUIAdd(objectSet);
    m_view3d.GUISceneComplete();
    
    UpdateInfo(lastMousePos);
    return true;
  }

  //: Update ruler info.
  
  bool DPDisplayView3dBodyC::GUIUpdateRuler() {
    ONDEBUG(cerr << "DPDisplayView3dBodyC::UpdateRuler(), Called. \n");
    
    Point2dC origin = Point2dC(0,0);
    Point2dC canSize = m_view3d.Size();
    vRuler.GUISetRangeAndPosition(origin[0],canSize[0],lastMousePos[0]);
    hRuler.GUISetRangeAndPosition(origin[1],canSize[1],lastMousePos[1]);
    return true;
  }
  
  //: Update ruler info.
  
  bool DPDisplayView3dBodyC::UpdateRuler() {
    Manager.QueueOnGUI(Trigger(DPDisplayView3dC(*this),&DPDisplayView3dC::GUIUpdateRuler));    
    return true;
  }
  
  //: Query position,
  
  bool DPDisplayView3dBodyC::Query(Vector2dC pos,StringC &info) { 
    //ONDEBUG(cerr <<"DPDisplayView3dBodyC::Query(), Position=" << pos << "\n");
#if 0
    // TODO: Maybe useful to report 3d position of intersection with scene ?
    
    RWLockHoldC hold(lockDisplayList,RWLOCK_READONLY); // To protect access to displaySize.
    for(DLIterC<DObject3dC> it(displayList);it;it++) {
      if(it->Query(pos,info)) {
	//ONDEBUG(cerr <<"DPDisplayView3dBodyC::Query(), Position=" << pos << " Result=" << info <<"\n");
	return true;
      }
    }
#endif
    //ONDEBUG(cerr <<"DPDisplayView3dBodyC::Query(), Position=" << pos << " No result.\n");
    return false;
  }
  

  //: Handle configure callback.
  
  bool DPDisplayView3dBodyC::CallbackConfigure(GdkEvent *&event) {
    ONDEBUG(cerr << "DPDisplayView3dBodyC::CallbackConfigure(), Called. \n");
    GUIUpdateRuler();
    return true;
  }
  
  //: Call back for mouse movements in the window.
  
  bool DPDisplayView3dBodyC::CallbackMouseMotion(MouseEventC &mouseEvent) {
#if 1
    Index2dC idx ( mouseEvent.At() ) ;  // gets the position of the mose event in RAVL co-ordinates not GTK
    Vector2dC pos(idx[0],idx[1]) ; 
    
    UpdateInfo(pos);
    lastMousePos = pos;
    GUIUpdateRuler();
#endif
    return true;
  }
  
  //: Call back for mouse press events.
  
  bool DPDisplayView3dBodyC::CallbackMousePress(MouseEventC &mouseEvent) {
    ONDEBUG(cerr << "DPDisplayView3dBodyC::CallbackMousePress(), Called. \n");
    return true;
  }

  
  //: Update info for mouse position.
  
  bool DPDisplayView3dBodyC::UpdateInfo(const Vector2dC &at) {
    Vector2dC pos = at; // ravl co-ords
    StringC rowps((int) pos[0]);
    rowPos.Label(rowps);
    StringC colps((int) pos[1]);
    colPos.Label(colps);
    StringC infos("-");
    Query(pos,infos);
    info.Label(infos);
    return true;
  }

  //: Start image save.
  
  bool DPDisplayView3dBodyC::CallbackStartSave() {
    ONDEBUG(cerr << "DPDisplayView3dBodyC::CallbackStartSave(), Called. \n");
    fileSelector.Show();
    return true;
  }

  //: Save image to a file.
  
  bool DPDisplayView3dBodyC::CallbackSave(StringC &str) {
    ONDEBUG(cerr << "DPDisplayView3dBodyC::CallbackSave(), Called. File='" << str << "' \n");
#if 0
    for(DLIterC<DObject3DC> it(displayList);it;it++) {
      if(it->Save(str))
	break;
    }
#endif
    return true;
  }

}
