// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVLGUI_CLIST_HEADER
#define RAVLGUI_CLIST_HEADER 1
////////////////////////////////////////////////////////////////
//! rcsid="$Id$" 
//! file="Ravl/GUI/GTK/CList.hh"
//! lib=RavlGUI
//! userlevel=Normal
//! author="Charles Galambos"
//! date="13/12/2000"
//! docentry="Ravl.API.Graphics.GTK.Control"
//! example=exCList.cc

#include "Ravl/GUI/Widget.hh"
#include "Ravl/Threads/RWLock.hh"
#include "Ravl/String.hh"
#include "Ravl/DList.hh"
#include "Ravl/SArray1d.hh"
#include "Ravl/GUI/Pixmap.hh"
#include "Ravl/GUI/MouseEvent.hh"
#include "Ravl/Hash.hh"
#include "Ravl/HSet.hh"
#include "Ravl/Tuple2.hh"

#include <gdk/gdkcolor.h>

namespace RavlGUIN {

  //! userlevel=Advanced
  //: Cell info.
  
  class CListCellC {
  public:
    CListCellC()
      : useFgCol(false),
	useBgCol(false)
    {}
    //: Default constructor.
    
    CListCellC(const StringC &ntext);
    //: Create a text cell.
    
    CListCellC(const PixmapC &npixmap);
    //: Create a cell with a pixmap.
    
    StringC text;  // Text to display.
    bool useFgCol;
    GdkColor fgCol;  // Text colour
    bool useBgCol;
    GdkColor fbCol;  // Background colour.
    PixmapC pixmap; // Can be invalid.
  };
  
  enum CListColumnResizeModeT {
    CLIST_COLRESIZE_FIXED,
    CLIST_COLRESIZE_USER,
    CLIST_COLRESIZE_AUTO,
    CLIST_COLRESIZE_OPTIMAL 
  };
  
  //! userlevel=Develop
  //: CList widget body.
  
  class CListBodyC
    : public WidgetBodyC
  {
  public:
    CListBodyC(const SArray1dC<StringC> &titles,GtkSelectionMode nselMode = GTK_SELECTION_SINGLE);
    //: Constructor from SArray1dC
    // Values for nselMode are listed <A HREF="http://library.gnome.org/devel/gtk/stable/gtk-Standard-Enumerations.html#GTKSELECTIONMODE">here</A>.
    
    CListBodyC(const DListC<StringC> &titles,GtkSelectionMode nselMode = GTK_SELECTION_SINGLE);
    //: Constructor from DList
    // Values for nselMode are listed <A HREF="http://library.gnome.org/devel/gtk/stable/gtk-Standard-Enumerations.html#GTKSELECTIONMODE">here</A>.
    
    CListBodyC(const char *titles[],int *colWidths = 0,GtkSelectionMode nselMode = GTK_SELECTION_SINGLE);
    //: Constructor from char*[]
    // Values for nselMode are listed <A HREF="http://library.gnome.org/devel/gtk/stable/gtk-Standard-Enumerations.html#GTKSELECTIONMODE">here</A>.
    
    virtual ~CListBodyC() { }
    //: Have virtual methods - need virtual destructor

    virtual bool Create();
    //: Create the widget.
    
    bool GUIAppendLine(int id,const SArray1dC<StringC> &line);
    //: Append a line entry.  
    // GUI thread only. <br>
    // Each line in the CList has to be given a unique 'id'.
    // If the 'id' is already present that row will be replaced by this method.
    
    bool GUIAppendCLine(int id,SArray1dC<CListCellC> line);
    //: Append a line entry.  
    // GUI thread only. <br>
    // Each line in the CList has to be given a unique 'id'.
    // If the 'id' is already present that row will be replaced by this method.
    
    void AppendLine(int id,const SArray1dC<CListCellC> &line);
    //: Append a line entry.  
    // Each line in the CList has to be given a unique 'id'.
    // If the 'id' is already present that row will be replaced by this method.
    
    void AppendLine(int id,const SArray1dC<StringC> &line);
    //: Append a line entry.  
    // Each line in the CList has to be given a unique 'id'.
    // If the 'id' is already present that row will be replaced by this method.
    
    bool GUIInsertCLine(int id,int rowNo,SArray1dC<CListCellC> line);
    //: Insert a line entry at the given row.
    // Each line in the CList has to be given a unique 'id'.
    // If the 'id' is already present that row will be replaced by this method.
    
    bool GUIInsertLine(int id,int rowNo,const SArray1dC<StringC> &line);
    //: Insert a line entry at the given row.
    // Each line in the CList has to be given a unique 'id'.
    // If the 'id' is already present that row will be replaced by this method.
    
    bool InsertLine(int id,int rowNo,const SArray1dC<CListCellC> &line);
    //: Insert a line entry at the given row.
    // Each line in the CList has to be given a unique 'id'.
    // If the 'id' is already present that row will be replaced by this method.
    
    bool InsertLine(int id,int rowNo,const SArray1dC<StringC> &line);
    //: Insert a line entry at the given row.
    // Each line in the CList has to be given a unique 'id'.
    // If the 'id' is already present that row will be replaced by this method.
    
    bool GUIRemoveLine(int id);
    //: Remove a line with given 'id'.  
    // GUI thread only.
    
    void RemoveLine(int id);
    //: Remove a line with given 'id'.  
    
    IntT RowOfId(int id);
    //: Find the row number an id is associated with.
    // If the id is unknown -1 is returned.
    
    bool SetCellText(int row,int col,const StringC &val);
    //: Set the contents of a cell to a string.
    
    bool GUISetCellText(int row,int col,const StringC &val);
    //: Set the contents of a cell to a string.
    
    bool SetCellIDText(int rowId,int col,const StringC &val);
    //: Set the contents of a cell to a string.
    // Select row by ID.
    
    bool GUISetCellIDText(int rowId,int col,const StringC &val);
    //: Set the contents of a cell to a string.
    // Select row by ID.
    
    bool SetCellPixmap(int row,int col,const PixmapC &val);
    //: Set the contents of a cell to a string.
    
    bool GUISetCellPixmap(int row,int col,const PixmapC &val);
    //: Set the contents of a cell to a string.
    
    bool SetCellIDPixmap(int rowId,int col,const PixmapC &val);
    //: Set the contents of a cell to a string.
    // Select row by ID.
    
    bool GUISetCellIDPixmap(int rowId,int col,const PixmapC &val);
    //: Set the contents of a cell to a string.
    // Select row by ID.
    
    bool GUISelect(int id);
    //: Force an item to be selected.
    // Use from the GUI thread only.
    
    bool GUIUnselect(int id);
    //: Force line with given 'id' to be unselected.
    // Use from the GUI thread only.
    
    bool GUIUnselectAll();
    //: Remove all entries from the selection.
    // Use from the GUI thread only.
    
    bool GUISelectAll();
    //: Remove all entries from the selection.
    // Use from the GUI thread only.

    bool Select(int id);
    //: Force an item to be selected.

    bool SelectAll();
    //: Force all items to be selected.
    
    bool Unselect(int id);
    //: Force an item to be unselected.
    
    bool UnselectAll();
    //: Remove all entries from the selection.
    
    IntT Cols() const
    { return cols ; }
    //: Get the number of cols in the clist.
    
    IntT Rows() const;
    //: Get the number of rows in CListC.
    
    DListC<IntT> Selection();
    //: Access current selection.
    
    bool GUIMoveID2Row(int id,int rowNo);
    //: Move the row with the given id to the rowNo .
    // Use from the GUI thread only.
    
    bool MoveID2Row(int id,int rowNo);
    //: Move the row with the given id to the rowNo .

    bool Freeze();
    //: Stop rendering updates to CList.
    // Calling this before doing many changes to the list
    // and then calling Thaw() will speed up the updates.

    bool GUIFreeze();
    //: Stop rendering updates to CList.
    // Calling this before doing many changes to the list
    // and then calling Thaw() will speed up the updates. <p>
    // Use from the GUI thread only.
    
    bool Thaw();
    //: Start rendering updates to CList.
    
    bool GUIThaw();
    //: Start rendering updates to CList.
    // Use from the GUI thread only.
    
    bool Clear();
    //: Clear all entries from the list.
    
    bool GUIClear();
    //: Clear all entries from the list.
    // Use from the GUI thread only.
    
    IntT GUIFindRowID(const Index2dC &at);
    //: Find row ID at position.
    
    IntT RowID(IntT rowNo);
    //: Retrieve the ID of a given row.
    
    bool SetColumnResizePolicy(IntT colNo,CListColumnResizeModeT policy);
    //: Set the column resize policy for 'colNo'
    // If colNo is -1 all columns are changed.
    
    bool GUISetColumnResizePolicy(IntT colNo,CListColumnResizeModeT policy);
    //: Set the column resize policy for 'colNo'
    // If colNo is -1 all columns are changed.
    // Use from the GUI thread only.
    
    bool SetColumnWidth(IntT colNo,IntT width);
    //: Set column width.
    
    bool GUISetColumnWidth(IntT colNo,IntT width);
    //: Set column width.
    
    bool SetColumnVisibility(IntT colNo,bool bVisible);
    //: Set column visibility
    //!bug: Not working at the moment - seems to in fact hide the entire widget
    
  protected:
    virtual void Destroy();
    //: Undo all references.
    
    bool GUIRowUnselected(CListEventC &);
    //: Called when row is unselected.
    
    bool GUIRowSelected(CListEventC &);
    //: Called when row is selected.
    
    bool GUISetColumnVisibility(IntT& colNo,bool& bVisible);
    //: Set column visibility
    //!bug: Not working at the moment - seems to in fact hide the entire widget
    
    GtkSelectionMode  selMode;
    
    RWLockC access;
    
    IntT cols;
    DListC<StringC> titles;
    SArray1dC<IntT> widths;
    
    DListC<Tuple2C<IntT,SArray1dC<CListCellC> > > data;
    HSetC<IntT> selection; // current selection.    
    
    Signal1C<IntT> selectionChanged;
    //: A signal that is sent when the selection changes. 
    // It contains the Row ID of the most recently-changed row
    
    DListC<TriggerC> toDo; // List of things to do as soon as we're initalised.

    int m_iHeight; // row height
    
    friend class CListC;
  };
  
  //! userlevel=Normal
  //: CList widget.
  // Note, this class is now OBSOLETE. It is recommended that TreeViewC/TreeStoreC be used in all
  // new code.
  class CListC
    : public WidgetC
  {
 public:
    CListC()
      {}
    //: Default constructor
    // Creates an invalid handle.
    
    CListC(const SArray1dC<StringC> &titles,GtkSelectionMode nselMode = GTK_SELECTION_SINGLE)
      : WidgetC(* new CListBodyC(titles,nselMode))
    {}
    //: Constructor from column titles as array of strings
    // Values for nselMode are listed <A HREF="http://library.gnome.org/devel/gtk/stable/gtk-Standard-Enumerations.html#GTKSELECTIONMODE">here</A>.
    
    CListC(const DListC<StringC> &titles,GtkSelectionMode nselMode = GTK_SELECTION_SINGLE)
      : WidgetC(* new CListBodyC(titles,nselMode))
    {}
    //: Constructor from column titles as list of strings
    // Values for nselMode are listed <A HREF="http://library.gnome.org/devel/gtk/stable/gtk-Standard-Enumerations.html#GTKSELECTIONMODE">here</A>.
    
    CListC(const char *titles[],int *colWidths = 0,GtkSelectionMode nselMode = GTK_SELECTION_SINGLE)
      : WidgetC(* new CListBodyC(titles,colWidths,nselMode))
    {}
    //: Constructor from column titles as char* array
    // This constructor allows initialisation from simple C-style array of string constants.</br>
    // Values for nselMode are listed <A HREF="http://library.gnome.org/devel/gtk/stable/gtk-Standard-Enumerations.html#GTKSELECTIONMODE">here</A>.
    
    
  protected:
    CListC(CListBodyC &bod)
      : WidgetC(bod)
    {}
    //: Body constructor.
    
    CListBodyC &Body()
      { return static_cast<CListBodyC &>(WidgetC::Body()); }
    //: Access body.
    
    const CListBodyC &Body() const
      { return static_cast<const CListBodyC &>(WidgetC::Body()); }
    //: Access body.
    
  public:
    bool GUIAppendLine(int id,const SArray1dC<StringC> &line)
    { return Body().GUIAppendLine(id,line); }
    //: Append a line entry.
    // GUI thread only.
    
    bool GUIAppendCLine(int id,const SArray1dC<CListCellC> &line)
    { return Body().GUIAppendCLine(id,line); }
    //: Append a line entry.
    // GUI thread only.
    
    void AppendLine(int id,const SArray1dC<CListCellC> &line)
    { Body().AppendLine(id,line); }
    //: Append a line entry.  
    
    void AppendLine(int id,const SArray1dC<StringC> &line)
    { Body().AppendLine(id,line); }
    //: Append a line entry.  
    
    bool GUIInsertCLine(int id,int rowNo,const SArray1dC<CListCellC> &line)
    { return Body().GUIInsertCLine(id,rowNo,line); }
    //: Insert a line entry at the given row.
    // Each line in the CList has to be given a unique 'id'.
    // If the 'id' is already present that row will be replaced by this method.
    
    bool GUIInsertLine(int id,int rowNo,const SArray1dC<StringC> &line)
    { return Body().GUIInsertLine(id,rowNo,line); }
    //: Insert a line entry at the given row.
    // Each line in the CList has to be given a unique 'id'.
    // If the 'id' is already present that row will be replaced by this method.
    
    bool InsertLine(int id,int rowNo,const SArray1dC<CListCellC> &line)
    { return Body().InsertLine(id,rowNo,line); }
    //: Insert a line entry at the given row.
    // Each line in the CList has to be given a unique 'id'.
    // If the 'id' is already present that row will be replaced by this method.
    
    bool InsertLine(int id,int rowNo,const SArray1dC<StringC> &line)
    { return Body().InsertLine(id,rowNo,line); }
    //: Insert a line entry at the given row.
    // Each line in the CList has to be given a unique 'id'.
    // If the 'id' is already present that row will be replaced by this method.
    
    bool GUIRemoveLine(int id)
    { return Body().GUIRemoveLine(id); }
    //: Remove a line entry.  
    // GUI thread only.
    
    void RemoveLine(int id)
    { Body().RemoveLine(id); }
    //: Remove a line entry.  
    
    IntT RowOfId(int id)
    { return Body().RowOfId(id); }
    //: Find the row number an id is associated with.
    // If the id is unknown -1 is returned.
    // Note, this function suspends the GUI for a brief time and should be used
    // sparingly.
    
    bool SetCellText(int row,int col,const StringC &val)
    { return Body().SetCellText(row,col,val); }
    //: Set the contents of a cell to a string.
    
    bool GUISetCellText(int row,int col,const StringC &val)
    { return Body().GUISetCellText(row,col,val); }
    //: Set the contents of a cell to a string.
    
    bool SetCellIDText(int rowId,int col,const StringC &val)
    { return Body().SetCellText(rowId,col,val); }
    //: Set the contents of a cell to a string.
    // Row is indexed by ID.
    
    bool GUISetCellIDText(int rowId,int col,const StringC &val)
    { return Body().SetCellIDText(rowId,col,val); }
    //: Set the contents of a cell to a string.
    // Row is indexed by ID.
    
    bool SetCellPixmap(int row,int col,const PixmapC &val)
    { return Body().SetCellPixmap(row,col,val); }
    //: Set the contents of a cell to a string.
    
    bool GUISetCellPixmap(int row,int col,const PixmapC &val)
    { return Body().GUISetCellPixmap(row,col,val); }
    //: Set the contents of a cell to a string.
    
    bool SetCellIDPixmap(int rowId,int col,const PixmapC &val)
    { return Body().SetCellPixmap(rowId,col,val); }
    //: Set the contents of a cell to a string.
    // Row is indexed by ID.
    
    bool GUISetCellIDPixmap(int rowId,int col,const PixmapC &val)
    { return Body().SetCellIDPixmap(rowId,col,val); }
    //: Set the contents of a cell to a string.
    // Row is indexed by ID.
    
    bool GUISelect(int id)
    { return Body().GUISelect(id); }
    //: Force an item to be selected.
    
    bool GUIUnselect(int id)
    { return Body().GUIUnselect(id); }
    //: Force an item to be unselected.
    
    bool GUIUnselectAll()
    { return Body().GUIUnselectAll(); }
    //: Remove all entries from the selection.
    
    bool GUISelectAll()
    { return Body().GUISelectAll(); }
    //: Add all entries to the selection.
    
    bool Select(int id)
    { return Body().Select(id); }
    //: Force an item to be selected.
    
    bool Unselect(int id)
    { return Body().Unselect(id); }
    //: Force an item to be unselected.
    
    bool UnselectAll()
    { return Body().UnselectAll(); }
    //: Remove all entries from the selection.
    
    bool SelectAll()
    { return Body().SelectAll(); }
    //: Add all entries to the selection.
    
    IntT Cols() const
    { return Body().Cols(); }
    //: Number of cols.
    
    IntT Rows() const
    { return Body().Rows(); }
    //: Get the number of rows.
    
    DListC<IntT> Selection()
    { return Body().Selection(); }
    //: Access current selection.
    
    bool GUIMoveID2Row(int id,int rowNo)
    { return Body().GUIMoveID2Row(id,rowNo); }
    //: Move the row with the given id to the rowNo .
    
    bool MoveID2Row(int id,int rowNo)
    { return Body().MoveID2Row(id,rowNo); }
    //: Move the row with the given id to the rowNo .

    bool Freeze()
    { return Body().Freeze(); }
    //: Stop rendering updates to CList.
    // Calling this before doing many changes to the list
    // and then calling Thaw() will speed up the updates.

    bool GUIFreeze()
    { return Body().GUIFreeze(); }
    //: Stop rendering updates to CList.
    // Calling this before doing many changes to the list
    // and then calling Thaw() will speed up the updates.
    
    bool Thaw()
    { return Body().Thaw(); }
    //: Start rendering updates to CList.
    
    bool GUIThaw()
    { return Body().GUIThaw(); }
    //: Start rendering updates to CList.

    bool Clear()
    { return Body().Clear(); } 
    //: Clear all entries from the list.
    
    bool GUIClear()
    { return Body().GUIClear(); }
    //: Clear all entries from the list.
    
    IntT GUIFindRowID(const Index2dC &at)
    { return Body().GUIFindRowID(at); }
    //: Find row ID at position.
    
    IntT RowID(IntT rowNo)
    { return Body().RowID(rowNo); }
    //: Retrieve the ID of a given row.
    
    Signal1C<IntT>& SelectionChanged()
    { return Body().selectionChanged; }
    //: This signal is sent when the selection is changed
    // This should be used rather than connecting directly to select_row or unselect_row events
    
    bool SetColumnResizePolicy(IntT colNo,CListColumnResizeModeT policy)
    { return Body().SetColumnResizePolicy(colNo,policy); }
    //: Set the column resize policy for 'colNo'
    // If colNo is -1 all columns are changed. <br>
    // Policies: <br>
    // CLIST_COLRESIZE_FIXED - User fixed size <br>
    // CLIST_COLRESIZE_USER - Allow user to change size <br>
    // CLIST_COLRESIZE_AUTO - Automaticly set width to fit data <br>
    // CLIST_COLRESIZE_OPTIMAL - Resize to current optimal now, (don't change the current mode.) <br>
    
    bool GUISetColumnResizePolicy(IntT colNo,CListColumnResizeModeT policy)
    { return Body().GUISetColumnResizePolicy(colNo,policy); }
    //: Set the column resize policy for 'colNo'
    // If colNo is -1 all columns are changed. <br>
    // Policies: <br>
    // CLIST_COLRESIZE_FIXED - User fixed size <br> 
    // CLIST_COLRESIZE_USER - Allow user to change size <br>
    // CLIST_COLRESIZE_AUTO - Automaticly set width to fit data <br>
    // CLIST_COLRESIZE_OPTIMAL - Resize to current optimal now, (don't change the current mode.) <br>
    
    bool SetColumnVisibility(IntT colNo,bool bVisible)
    { return Body().SetColumnVisibility(colNo,bVisible); }
    //: Set column visibility
    //!bug: Not working at the moment - seems to in fact hide the entire widget

    bool SetColumnWidth(IntT colNo,IntT width)
    { return Body().SetColumnWidth(colNo,width); }
    //: Set column width.
    
    bool GUISetColumnWidth(IntT colNo,IntT width)
    { return Body().GUISetColumnWidth(colNo,width); }
    //: Set column width.
    // Use from the GUI thread only.

  protected:
    
    bool GUISetColumnVisibility(IntT& colNo,bool& bVisible)
    { return Body().GUISetColumnVisibility(colNo,bVisible); }
    //: Set column visibility
    //!bug: Not working at the moment - seems to in fact hide the entire widget

    friend class CListBodyC;
  };
}
#endif
