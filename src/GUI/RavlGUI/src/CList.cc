// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
////////////////////////////////////////////////////////////////
//! rcsid="$Id$" 
//! lib=RavlGUI
//! file="Ravl/GUI/GTK/CList.cc"

#include "Ravl/GUI/CList.hh"
#include "Ravl/GUI/Manager.hh"
#include "Ravl/SArray1dIter2.hh"
#include "Ravl/GUI/ReadBack.hh"
#include <gtk/gtk.h>

#define DODEBUG 0
#if DODEBUG
#define ONDEBUG(x) x
#else
#define ONDEBUG(x)
#endif

namespace RavlGUIN {
  
  //: Create a text cell.
  
  CListCellC::CListCellC(const StringC &ntext)
    : text(ntext),
      useFgCol(false),
      useBgCol(false)
  {}
    
  //: Create a cell with a pixmap.
  
  CListCellC::CListCellC(const PixmapC &npixmap)
    : useFgCol(false),
      useBgCol(false),
      pixmap(npixmap)
  {}
  
  //////////////////////////////////////////////////////////////////////
  
  CListBodyC::CListBodyC(const DListC<StringC> &ntitles,GtkSelectionMode nselMode)
    : selMode(nselMode),
      titles(ntitles),
      selectionChanged(0),
      m_iHeight(0)
  {
    cols = titles.Size();
    ONDEBUG(std::cerr << "CListBodyC::CListBodyC(), Cols : " << cols << "\n");
    widths = SArray1dC<IntT>(cols);
    widths.Fill(-1);
  }
  
  CListBodyC::CListBodyC(const SArray1dC<StringC> &ntitles,GtkSelectionMode nselMode)
    : selMode(nselMode),
      selectionChanged(0),
      m_iHeight(0)
  {
    for (UIntT i(0); i<ntitles.Size(); ++i) titles.InsLast(ntitles[i]);
    cols = titles.Size();
    ONDEBUG(std::cerr << "CListBodyC::CListBodyC(), Cols : " << cols << "\n");
    widths = SArray1dC<IntT>(cols);
    widths.Fill(-1);
  }
  
  
  CListBodyC::CListBodyC(const char *ntitles[],int *colWidths,GtkSelectionMode nselMode)
    : selMode(nselMode),
      selectionChanged(0),
      m_iHeight(0)
  {
    int i = 0;
    if(ntitles != 0) {
      for(;ntitles[i] != 0;i++)
	titles.InsLast(StringC(ntitles[i]));
    } else {
      titles.InsLast("Unknown");
      i++;
    }
    cols = i;
    widths = SArray1dC<IntT>(cols);
    widths.Fill(-1);
    if(colWidths != 0) { // Got some widths ?
      for(i = 0;i < cols;i++)
	widths[i] = colWidths[i];
    }
    ONDEBUG(std::cerr << "CListBodyC::CListBodyC(), Cols : " << cols << "\n");
  }
  
  //: Called when row is unselected.
  
  bool CListBodyC::GUIRowUnselected(CListEventC &cle) {
    ONDEBUG(std::cerr << "CListBodyC::GUIRowUnselected. " << cle.RowID() << "\n");
    RWLockHoldC hold(access,false);
    selection -= cle.RowID();
    hold.Unlock();
    selectionChanged(cle.RowID());
    return true;
  }

  //: Called when row is unselected.
  
  bool CListBodyC::GUIRowSelected(CListEventC &cle) {
    ONDEBUG(std::cerr << "CListBodyC::GUIRowSelected. " << cle.RowID() << "\n");
    RWLockHoldC hold(access,false);
    selection += cle.RowID();
    hold.Unlock();
    selectionChanged(cle.RowID());
    return true;
  }
  
  //: Access current selection.
  
  DListC<IntT> CListBodyC::Selection() {
    DListC<IntT> ret;
    RWLockHoldC hold(access,true);
    for(HSetIterC<IntT> it(selection);it;it++)
      ret += *it;
    return ret;
  }
  
  
  //: Create the widget.
  
  bool CListBodyC::Create() {
    if(widget != 0)
      return true; // Done already!
    
    cols = titles.Size();
    char **tlist = new char *[cols];
    int i = 0;
    for(DLIterC<StringC> it(titles);it;it++,i++) 
      tlist[i] = (char *) it->chars();
    widget = gtk_clist_new_with_titles(cols,tlist);  
    delete [] tlist; // Hope I don't need this now.
    for(int i = 0;i < cols;i++) {
      if(widths[i] < 0)
	continue;
      gtk_clist_set_column_width(GTK_CLIST(widget),i,widths[i]);
    }
    gtk_clist_set_shadow_type (GTK_CLIST(widget), GTK_SHADOW_OUT);    
    gtk_clist_set_selection_mode(GTK_CLIST(widget),selMode);
    // Append lines that we've stored up.
    for(DLIterC<Tuple2C<IntT,SArray1dC<CListCellC> > > it2(data);it2;it2++)
      GUIAppendCLine(it2->Data1(),it2->Data2());
    
    CListEventC defaultInit;
    ConnectRef(Signal("select_row"),*this,&CListBodyC::GUIRowSelected,defaultInit);
    ConnectRef(Signal("unselect_row"),*this,&CListBodyC::GUIRowUnselected,defaultInit);
    ConnectSignals();
    
    while(!toDo.IsEmpty()) {
      ONDEBUG(std::cerr << "Catching up with stuff. \n");
      toDo.Last().Invoke();
      toDo.DelLast();
    }
    
    return true;
  }
  
  //: Append a line entry.  
  // GUI thread only.
  
  bool CListBodyC::GUIAppendCLine(int id,SArray1dC<CListCellC> line) {
    if(widget == 0) {
      data.InsLast(Tuple2C<IntT,SArray1dC<CListCellC> >(id,line));
      return true;
    }
    // This doesn't to any fancy stuff yet...
    char **tlist = new char *[cols];
    for(int i = 0;i < cols;i++)
      tlist[i] = (char *) line[i].text.chars();  
    int rowNo = gtk_clist_append(GTK_CLIST(widget),tlist);
    for(int i = 0;i < cols;i++) {
      // Look for pixmaps.
      PixmapC &pm = line[i].pixmap;
      if(pm.IsValid()) {
	ONDEBUG(std::cerr << "CListBodyC::GUIAppendCLine(), Setting pixmap.Row=" << rowNo << " Cell=" << i << "\n");
	if(!pm.Create())
	  std::cerr << "CListBodyC::GUIAppendCLine(), Failed to create pixmap. \n";
	else {
	  gtk_clist_set_pixmap (GTK_CLIST(widget),rowNo,i,pm.Pixmap(),pm.Mask());
	  int width, height;
	  if (pm.GUIGetSize(width,height)) {
	    if (height > m_iHeight) {
	      m_iHeight = height;
	      gtk_clist_set_row_height(GTK_CLIST(widget),m_iHeight);	      
	    }
	  }    
	}
      }
    }
    
    gtk_clist_set_row_data (GTK_CLIST(widget),rowNo,(void *) id);
    delete [] tlist;
    return true;
  }
  
  //: Append a line entry.  
  // GUI thread only.
  
  bool CListBodyC::GUIAppendLine(int id,const SArray1dC<StringC> &line) {
    if(widget == 0) {
      SArray1dC<CListCellC> newLine(cols);
      for(SArray1dIter2C<CListCellC,StringC> it(newLine,line);it;it++)
	it.Data1().text = it.Data2();
      data.InsLast(Tuple2C<IntT,SArray1dC<CListCellC> >(id,newLine));
      return true;
    }
    RavlAssert(((int) line.Size()) >= cols); // Do something friendlier ?
    int rowNo = gtk_clist_find_row_from_data (GTK_CLIST(widget),(void *) id);
    if(rowNo >= 0) { // Got row already ?
      for(int i = 0;i< ((int) line.Size());i++)
	gtk_clist_set_text(GTK_CLIST(widget),rowNo,i,line[i]);
    } else {
      char **tlist = new char *[cols];
      for(int i = 0;i < cols;i++)
	tlist[i] = (char *) line[i].chars();  
      rowNo = gtk_clist_append(GTK_CLIST(widget),tlist);
      gtk_clist_set_row_data (GTK_CLIST(widget),rowNo,(void *) id);
      delete [] tlist;
    }
    return true;
  }
  
  //: Remove a line entry.  
  // GUI thread only.
  
  bool CListBodyC::GUIRemoveLine(int id) {
    if(widget == 0) {
      //cerr << "CListBodyC::GUIRemoveLine(), WARNING: Called before widget constructed. \n";
      for(DLIterC<Tuple2C<IntT,SArray1dC<CListCellC> > > it2(data);it2;it2++) {
	if(it2.Data().Data1() == id) {
	  it2.Del();
	  break;
	}
      }
      return true;
    }
    int rowNo = gtk_clist_find_row_from_data (GTK_CLIST(widget),(void *) id);
    ONDEBUG(std::cerr << "CListBodyC::GUIRemoveLine(), ID:" << id << "  Row:" << rowNo << "\n");
    if(rowNo < 0)
      return true; // Not found.
    gtk_clist_remove (GTK_CLIST(widget),rowNo);
    return true;
  }
  
  //: Remove a line entry.  
  
  void CListBodyC::RemoveLine(int id) {
    Manager.Queue(Trigger(CListC(*this),&CListC::GUIRemoveLine,id));
  }

  //: Insert a line entry at the given row.
  // Each line in the CList has to be given a unique 'id'.
  // If the 'id' is already present that row will be replaced by this method.
  
  bool CListBodyC::GUIInsertCLine(int id,int rowNo,SArray1dC<CListCellC> line) {
    if(widget == 0) {
      toDo.InsLast(Trigger(CListC(*this),&CListC::GUIInsertCLine,id,rowNo,line));
      return true;
    }    
    
    int oldRowNo = gtk_clist_find_row_from_data (GTK_CLIST(widget),(void *) id);
    if(rowNo >= 0) { // Got row already ? 
      // if so remove it.
      gtk_clist_remove (GTK_CLIST(widget),oldRowNo);
      if(oldRowNo < rowNo)
	rowNo--; // Correct target row number.
    }
    
    // Add new row.
    char **tlist = new char *[cols];
    for(int i = 0;i < cols;i++)
      tlist[i] = (char *) line[i].text.chars();  

    rowNo = gtk_clist_insert(GTK_CLIST(widget),rowNo,tlist);

    for(int i = 0;i < cols;i++) {
      // Look for pixmaps.
      PixmapC &pm = line[i].pixmap;
      if(pm.IsValid()) {
	ONDEBUG(std::cerr << "CListBodyC::GUIAppendCLine(), Setting pixmap.Row=" << rowNo << " Cell=" << i << "\n");
	if(!pm.Create())
	  std::cerr << "CListBodyC::GUIAppendCLine(), Failed to create pixmap. \n";
	else {
	  gtk_clist_set_pixmap (GTK_CLIST(widget),rowNo,i,pm.Pixmap(),pm.Mask());
	  int width, height;
	  if (pm.GUIGetSize(width,height)) {
	    if (height > m_iHeight) {
	      m_iHeight = height;
	      gtk_clist_set_row_height(GTK_CLIST(widget),m_iHeight);
	    }
	  }    
	}
      }
    }

    gtk_clist_set_row_data (GTK_CLIST(widget),rowNo,(void *) id);
    delete [] tlist;

    
    
    return true;
  }
  
  //: Insert a line entry at the given row.
  // Each line in the CList has to be given a unique 'id'.
  // If the 'id' is already present that row will be replaced by this method.
  
  bool CListBodyC::GUIInsertLine(int id,int rowNo,const SArray1dC<StringC> &line) {
    if(widget == 0) {
      toDo.InsLast(Trigger(CListC(*this),&CListC::GUIInsertLine,id,rowNo,line));
      return true;
    }    
    
    int oldRowNo = gtk_clist_find_row_from_data (GTK_CLIST(widget),(void *) id);
    if(rowNo >= 0) { // Got row already ? 
      // if so remove it.
      gtk_clist_remove (GTK_CLIST(widget),oldRowNo);
      if(oldRowNo < rowNo)
	rowNo--; // Correct target row number.
    }
    
    // Add new row.
    char **tlist = new char *[cols];
    for(int i = 0;i < cols;i++)
      tlist[i] = (char *) line[i].chars();  
    rowNo = gtk_clist_insert(GTK_CLIST(widget),rowNo,tlist);
    gtk_clist_set_row_data (GTK_CLIST(widget),rowNo,(void *) id);
    delete [] tlist;
    
    return true;
  }
  
  //: Insert a line entry at the given row.
  // Each line in the CList has to be given a unique 'id'.
  // If the 'id' is already present that row will be replaced by this method.
  
  bool CListBodyC::InsertLine(int id,int rowNo,const SArray1dC<CListCellC> &line) {
    Manager.Queue(Trigger(CListC(*this),&CListC::GUIInsertCLine,id,rowNo,line));
    return true;
  }

  bool CListBodyC::InsertLine(int id,int rowNo,const SArray1dC<StringC> &line) {
    Manager.Queue(Trigger(CListC(*this),&CListC::GUIInsertLine,id,rowNo,line));
    return true;
  }
  
  
  //: Test if an ID exists.
  
  IntT CListBodyC::RowOfId(int id) {
    ReadBackLockC lock; // Get lock on GUI thread.
    return gtk_clist_find_row_from_data (GTK_CLIST(widget),(void *) id);
  }
  
  //: Append a line entry.  
  // GUI thread only.
  
  void CListBodyC::AppendLine(int id,const SArray1dC<CListCellC> &line) {
    Manager.Queue(Trigger(CListC(*this),&CListC::GUIAppendCLine,id,line));
  }
  
  //: Append a line entry.  
  // GUI thread only.
  
  void CListBodyC::AppendLine(int id,const SArray1dC<StringC> &line) {
    Manager.Queue(Trigger(CListC(*this),&CListC::GUIAppendLine,id,line));
  }
  
  //: Set the contents of a cell to a string.
  
  bool CListBodyC::SetCellText(int row,int col,const StringC &val) {
    Manager.Queue(Trigger(CListC(*this),&CListC::GUISetCellText,row,col,val));
    return true;
  }
  
  //: Set the contents of a cell to a string.
  
  bool CListBodyC::GUISetCellText(int row,int col,const StringC &val) {
    if(widget == 0) {
      toDo.InsLast(Trigger(CListC(*this),&CListC::GUISetCellText,row,col,val));
      return true;
    }
    gtk_clist_set_text(GTK_CLIST(widget),row,col,val.chars());
    return true;
  }
  
  //: Set the contents of a cell to a string.
  // Select row by ID.
  
  bool CListBodyC::SetCellIDText(int rowId,int col,const StringC &val) {
    Manager.Queue(Trigger(CListC(*this),&CListC::GUISetCellIDText,rowId,col,val));
    return true;
  }
  
  //: Set the contents of a cell to a string.
  // Select row by ID.
  
  bool CListBodyC::GUISetCellIDText(int rowId,int col,const StringC &val) {
    if(widget == 0) {
      toDo.InsLast(Trigger(CListC(*this),&CListC::GUISetCellIDText,rowId,col,val));
      return true;
    }
    int rowNo = gtk_clist_find_row_from_data (GTK_CLIST(widget),(void *) rowId);
    if(rowNo < 0)
      return true; // Not found.
    gtk_clist_set_text(GTK_CLIST(widget),rowNo,col,val.chars());
    return true;
  }
  
  //: Set the contents of a cell to a string.
  
  bool CListBodyC::SetCellPixmap(int row,int col,const PixmapC &pm) {
    Manager.Queue(Trigger(CListC(*this),&CListC::GUISetCellPixmap,row,col,pm));
    return true;
  }
  
  //: Set the contents of a cell to a string.
  
  bool CListBodyC::GUISetCellPixmap(int row,int col,const PixmapC &pm) {
    if(widget == 0) {
      toDo.InsLast(Trigger(CListC(*this),&CListC::GUISetCellPixmap,row,col,pm));
      return true;
    }
    PixmapC &xpm = const_cast<PixmapC &>(pm);
    gtk_clist_set_pixmap (GTK_CLIST(widget),row,col,xpm.Pixmap(),xpm.Mask());
    return true;
  }
  
  //: Set the contents of a cell to a string.
  // Select row by ID.
  
  bool CListBodyC::SetCellIDPixmap(int rowId,int col,const PixmapC &pm) {
    Manager.Queue(Trigger(CListC(*this),&CListC::GUISetCellIDPixmap,rowId,col,pm));
    return true;
  }
  
  //: Set the contents of a cell to a string.
  // Select row by ID.
  
  bool CListBodyC::GUISetCellIDPixmap(int rowId,int col,const PixmapC &pm) {
    if(widget == 0) {
      toDo.InsLast(Trigger(CListC(*this),&CListC::GUISetCellIDPixmap,rowId,col,pm));
      return true;
    }
    int rowNo = gtk_clist_find_row_from_data (GTK_CLIST(widget),(void *) rowId);
    if(rowNo < 0)
      return true; // Not found.
    PixmapC &xpm = const_cast<PixmapC &>(pm);
    gtk_clist_set_pixmap (GTK_CLIST(widget),rowNo,col,xpm.Pixmap(),xpm.Mask());
    return true;
  }
  
  //: Force an item to be selected.
  
  bool CListBodyC::GUISelect(int id) {
    if(widget == 0) {
      // FIXME:- This should be handled properly....
      std::cerr << "WARNING: CListBodyC::GUISelect() called before widget created. \n";
      return true;
    }
    int rowNo = gtk_clist_find_row_from_data (GTK_CLIST(widget),(void *) id);
    if(rowNo < 0)
      return true; // Not found.
    gtk_clist_select_row (GTK_CLIST(widget),rowNo,0);
    return true;
  }
  
  //: Force an item to be unselected.
  
  bool CListBodyC::GUIUnselect(int id) {
    if(widget == 0) {
      // FIXME:- This should be handled properly.
      std::cerr << "WARNING: CListBodyC::GUIUnselect() called before widget created. \n";
      return true;
    }
    int rowNo = gtk_clist_find_row_from_data (GTK_CLIST(widget),(void *) id);
    if(rowNo < 0)
      return true; // Not found.
    gtk_clist_unselect_row (GTK_CLIST(widget),rowNo,0);
    return true;
  }

  //: Remove all entries from the selection.
  
  bool CListBodyC::GUIUnselectAll() {
    if(widget == 0) {
      // FIXME:- This should be handled properly.
      std::cerr << "WARNING: CListBodyC::GUIUnselectAll() called before widget created. \n";
      return true;
    }
    gtk_clist_unselect_all(GTK_CLIST(widget));
    return true;
  }

  //: Add all entries to the selection.
  
  bool CListBodyC::GUISelectAll() {
    if(widget == 0) {
      // FIXME:- This should be handled properly.
      std::cerr << "WARNING: CListBodyC::GUISelectAll() called before widget created. \n";
      return true;
    }
    gtk_clist_select_all(GTK_CLIST(widget));
    return true;
  }
  
  
  //: Force an item to be selected.
  
  bool CListBodyC::Select(int id) {
    Manager.Queue(Trigger(CListC(*this),&CListC::GUISelect,id));
    return true;
  }
  
  //: Force an item to be unselected.
  
  bool CListBodyC::Unselect(int id) {
    Manager.Queue(Trigger(CListC(*this),&CListC::GUIUnselect,id));
    return true;
  }

  //: Remove all entries from the selection.
  
  bool CListBodyC::UnselectAll() {
    Manager.Queue(Trigger(CListC(*this),&CListC::GUIUnselectAll));
    return true;
  }

  //: Add all entries to the selection.
  
  bool CListBodyC::SelectAll() {
    Manager.Queue(Trigger(CListC(*this),&CListC::GUISelectAll));
    return true;
  }

  //: Move the row with the given id to the rowNo .
  
  bool CListBodyC::GUIMoveID2Row(int id,int rowNo) {
    if(widget == 0) {
      // FIXME:- This should be handled properly.
      ONDEBUG(std::cerr << "WARNING: CListBodyC::GUIMoveID2Row() called before widget created. \n");
      // Find the data we want to move.
      DLIterC<Tuple2C<IntT,SArray1dC<CListCellC> > > it(data);
      for(;it && (it.Data().Data1() != id);it++) ;
      if(!it) {
	cerr << "CListBodyC::GUIMoveID2Row(), WARNING: Can't find element with id " << id << "\n";
	return true;
      }
      DLIterC<Tuple2C<IntT,SArray1dC<CListCellC> > > itx(data);
      //if(itx == it)
      //continue; // Already there.
      itx.Nth(rowNo);
      itx.MoveBef(it);
      return true;
    }
    int newRowNo = gtk_clist_find_row_from_data (GTK_CLIST(widget),(void *) id);
    if(rowNo < 0)
      return true; // Not found.
    gtk_clist_row_move(GTK_CLIST(widget),newRowNo,rowNo);
    return true;
  }
  
  //: Move the row with the given id to the rowNo .
  
  bool CListBodyC::MoveID2Row(int id,int rowNo) {
    Manager.Queue(Trigger(CListC(*this),&CListC::GUIMoveID2Row,id,rowNo));
    return true;
  }

  //: Stop rendering updates to CList.
  // Calling this before doing many changes to the list
  // and then calling Thaw() will speed up the updates.
  bool CListBodyC::Freeze() {
    Manager.Queue(Trigger(CListC(*this),&CListC::GUIFreeze));    
    return true;
  }
  
  //: Stop rendering updates to CList.
  // Calling this before doing many changes to the list
  // and then calling Thaw() will speed up the updates.
  bool CListBodyC::GUIFreeze() {
    if(widget == 0) 
      return true;    
    gtk_clist_freeze(GTK_CLIST(widget));
    return true;
  }
  
  //: Start rendering updates to CList.
  bool CListBodyC::Thaw() {
    Manager.Queue(Trigger(CListC(*this),&CListC::GUIThaw));
    return true;
  }
  
  //: Start rendering updates to CList.
  bool CListBodyC::GUIThaw() {
    if(widget == 0) 
      return true;
    gtk_clist_thaw(GTK_CLIST(widget));
    return true;
  }

  //: Clear all entries from the list.
  
  bool CListBodyC::Clear() {
    Manager.Queue(Trigger(CListC(*this),&CListC::GUIClear));
    return true;
  }
  
  //: Clear all entries from the list.
  
  bool CListBodyC::GUIClear() {
    if(widget == 0) {
      data.Empty();
      return true;
    }
    gtk_clist_clear(GTK_CLIST(widget));    
    // Reset row height
    m_iHeight=0;
    gtk_clist_set_row_height(GTK_CLIST(widget),0);
    return true;
  }
  
  //: Find row ID at position.
  
  IntT CListBodyC::GUIFindRowID(const Index2dC &at) {
    if(widget == 0)
      return 0;
    IntT row,col;
    gtk_clist_get_selection_info (GTK_CLIST(widget),at[0].V(),at[1].V(),&row,&col);
#if RAVL_CPUTYPE_64
    return  (Int64T) gtk_clist_get_row_data(GTK_CLIST(widget),row);
#else
    return  (IntT) gtk_clist_get_row_data(GTK_CLIST(widget),row);
#endif
  }
  
  //: Retrieve the ID of a given row.
  
  IntT CListBodyC::RowID(IntT rowNo) {
    ReadBackLockC lock; // Get lock on GUI thread.    
    if(widget == 0) {
      if(data.Size() <= (UIntT) rowNo)
	return -1;
      return data.Nth(rowNo).Data1();
    }
#if RAVL_CPUTYPE_64
    return (Int64T) gtk_clist_get_row_data(GTK_CLIST(widget),rowNo);
#else
    return (IntT) gtk_clist_get_row_data(GTK_CLIST(widget),rowNo);
#endif
  }
  
  //: Get the number of rows in CListC.
  
  IntT CListBodyC::Rows() const {
    ReadBackLockC lock; // Get lock on GUI thread.    
    if(widget == 0)
      return data.Size();
    return GTK_CLIST(widget)->rows;
  }
  
  //: Set the column resize policy for 'colNo'
  // If colNo is -1 all columns are changed.
  
  bool CListBodyC::SetColumnResizePolicy(IntT colNo,CListColumnResizeModeT policy) {
    Manager.Queue(Trigger(CListC(*this),&CListC::GUISetColumnResizePolicy,colNo,policy));
    return true;
  }
  
  //: Set the column resize policy for 'colNo'
  // If colNo is -1 all columns are changed.
  
  bool CListBodyC::GUISetColumnResizePolicy(IntT colNo,CListColumnResizeModeT policy) {
    if(widget == 0) {
      ONDEBUG(std::cerr << "CListBodyC::GUISetColumnResizePolicy(), WARNING: Widget not ready. ");
      toDo.InsLast(Trigger(CListC(*this),&CListC::GUISetColumnResizePolicy,colNo,policy));
      return true;
    }
    if(colNo < 0 && policy == CLIST_COLRESIZE_OPTIMAL) {
      gtk_clist_columns_autosize(GTK_CLIST(widget));
      return true;
    }
    int val,i = colNo;
    int max = colNo;
    if(colNo < 0) {
      i = 0;
      max = GTK_CLIST(widget)->columns;
    }
    for(;i < max;i++) {
      switch(policy) {
      case CLIST_COLRESIZE_FIXED: 
	gtk_clist_set_column_auto_resize(GTK_CLIST(widget),i,false);
	gtk_clist_set_column_resizeable (GTK_CLIST(widget),i,false);
	break;
      case CLIST_COLRESIZE_USER:
	gtk_clist_set_column_resizeable (GTK_CLIST(widget),i,true);
	gtk_clist_set_column_auto_resize(GTK_CLIST(widget),i,false);
	break;
      case CLIST_COLRESIZE_AUTO:
	gtk_clist_set_column_resizeable (GTK_CLIST(widget),i,false);
	gtk_clist_set_column_auto_resize(GTK_CLIST(widget),i,true);
	break;
      case CLIST_COLRESIZE_OPTIMAL:
	val = gtk_clist_optimal_column_width (GTK_CLIST(widget),i);
	gtk_clist_set_column_width (GTK_CLIST(widget),i,val);
	break;
      }
    }
    return true;
  }

  //: Set column width.
  
  bool CListBodyC::SetColumnWidth(IntT colNo,IntT width) {
    Manager.Queue(Trigger(CListC(*this),&CListC::GUISetColumnWidth,colNo,width));
    return true;
  }
  
  //: Set column width.
  // Use from the GUI thread only.
  
  bool CListBodyC::GUISetColumnWidth(IntT colNo,IntT width) {
    if(widget == 0) {
      ONDEBUG(std::cerr << "CListBodyC::GUISetColumnWidth(), WARNING: Widget not ready. ");
      toDo.InsLast(Trigger(CListC(*this),&CListC::GUISetColumnWidth,colNo,width));
      return true;
    }
    gtk_clist_set_column_width (GTK_CLIST(widget),colNo,width);
    return true;
  }

  bool CListBodyC::SetColumnVisibility(IntT colNo,bool bVisible) {
    Manager.Queue(Trigger(CListC(*this),&CListC::GUISetColumnVisibility,colNo,bVisible));
    return true ; 
  }
  
  bool CListBodyC::GUISetColumnVisibility(IntT& colNo,bool& bVisible) {
    if(widget == 0) {
      ONDEBUG(std::cerr << "CListBodyC::GUISetColumnVisibility(), WARNING: Widget not ready. ");
      toDo.InsLast(Trigger(CListC(*this),&CListC::GUISetColumnVisibility,colNo,bVisible));
      return true;
    }
    gtk_clist_set_column_visibility (GTK_CLIST(widget),colNo,bVisible);
    return true;
  }
  
  //: Undo all references.
  
  void CListBodyC::Destroy() {
    WidgetBodyC::Destroy(); 
  }


}
