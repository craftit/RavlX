// This file is part of CxxDoc, The RAVL C++ Documentation tool 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU General 
// Public License (GPL). See the gpl.licence file for details or
// see http://www.gnu.org/copyleft/gpl.html
// file-header-ends-here
//////////////////////////////////////////////////////////////
//! rcsid="$Id$"
//! lib=RavlSourceTools
//! file="Ravl/SourceTools/CodeManager/amma2ravl.cc"
//! userlevel=Normal
//! docentry="Ravl.API.Source Tools"
//! author="Charles Galambos"

#include "Ravl/Option.hh"
#include "Ravl/SourceTools/DefsFile.hh"
#include "Ravl/SourceTools/SourceCodeManager.hh"
#include "Ravl/OS/Filename.hh"
#include "Ravl/SourceTools/SourceFile.hh"

using namespace RavlN;

bool verb = false;
bool dryRun = false;
bool guiUpdates = false;
bool guiSrc = false;

static bool UpdateFile(FilenameC fn) {
  bool c_source = false;
  if(!fn.Exists()) {
    std::cerr << "ERROR: Source file '" << fn << "' does not exist. \n";
    return false;
  }
  if(!fn.IsReadable()) {
    std::cerr << "ERROR: Source file '" << fn << "' is not readable \n";
    return false;
  }
  if(fn.Extension() == "c" || fn.Extension() == "h")
    c_source = true;
  SourceFileC theFile;
  if(!theFile.Load(fn)) {
    std::cerr << "Failed to load file '" << fn << "'\n";
    return false;
  }
  if(c_source) {
    // ccmath stuff.
    theFile.GlobalSubst("include \"complex.h\"","include \"ccmath/complex.h\"");
    theFile.GlobalSubst("include \"ccmath.h\"","include \"ccmath/ccmath.h\"");
    theFile.GlobalSubst("include \"orpol.h\"","include \"ccmath/orpol.h\"");
    theFile.GlobalSubst("include \"matutl.h\"","include \"ccmath/matutl.h\"");
    theFile.GlobalSubst("include \"tree.h\"","include \"ccmath/tree.h\"");
    theFile.GlobalSubst("include \"merge.h\"","include \"ccmath/merge.h\"");
    theFile.GlobalSubst("include \"arma.h\"","include \"ccmath/arma.h\"");
    theFile.GlobalSubst("include \"armaf.h\"","include \"ccmath/armaf.h\"");
    theFile.GlobalSubst("include \"xpre.h\"","include \"ccmath/xpre.h\"");
    theFile.GlobalSubst("include \"xpre.h\"","include \"ccmath/xpre.h\"");
      
    if(theFile.IsModified()) {
      std::cerr << "Updated file :" << fn << "\n";
      if(!dryRun)
	theFile.Save();
    }
    return true;
  }
  theFile.GlobalSubst("\"amma/StdType.hh\"","\"Ravl/Types.hh\"");
  theFile.GlobalSubst("\"amma/DeadLine.hh\"","\"Ravl/OS/DeadLineTimer.hh\"");
  theFile.GlobalSubst("\"amma/RCHandle.hh\"","\"Ravl/RefCounter.hh\"");
  theFile.GlobalSubst("\"amma/Filename.hh\"","\"Ravl/OS/Filename.hh\"");
  theFile.GlobalSubst("\"amma/HashAR.hh\"","\"Ravl/Hash.hh\"");
  theFile.GlobalSubst("<assert.h>","\"Ravl/Assert.hh\"");
  theFile.GlobalSubst("\"amma/DP/Signal.hh\"","\"Ravl/Threads/Signal.hh\"");
  theFile.GlobalSubst("\"amma/DP/Signal1.hh\"","\"Ravl/Threads/Signal1.hh\"");
  theFile.GlobalSubst("\"amma/DP/Signal2.hh\"","\"Ravl/Threads/Signal2.hh\"");
  theFile.GlobalSubst("\"amma/DP/Signal3.hh\"","\"Ravl/Threads/Signal3.hh\"");
  theFile.GlobalSubst("\"amma/StrList.hh\"","\"Ravl/StringList.hh\"");
  theFile.GlobalSubst("\"amma/Date.hh\"","\"Ravl/OS/Date.hh\"");
  theFile.GlobalSubst("\"amma/StdTypes.hh\"","\"Ravl/Types.hh\"");
  theFile.GlobalSubst("\"amma/PThread/MsgPipe.hh\"","\"Ravl/Threads/MessageQueue.hh\"");
  theFile.GlobalSubst("\"amma/PThread/MsgPipe.hh\"","\"Ravl/Threads/MessageQueue.hh\"");
  theFile.GlobalSubst("\"amma/PThread/","\"Ravl/Threads/");
  theFile.GlobalSubst("\"amma/Date.hh\"","\"Ravl/OS/Date.hh\"");
  theFile.GlobalSubst("\"amma/ImageIter.hh\"","\"Ravl/Array2dIter.hh\"");
  theFile.GlobalSubst("\"amma/Image2Iter.hh\"","\"Ravl/Array2dIter2.hh\"");
  theFile.GlobalSubst("\"amma/Image3Iter.hh\"","\"Ravl/Array2dIter3.hh\"");
  theFile.GlobalSubst("\"amma/Image.hh\"","\"Ravl/Image/Image.hh\"");
  theFile.GlobalSubst("\"amma/ByteImag.hh\"","\"Ravl/Image/Image.hh\"");
  theFile.GlobalSubst("\"amma/RGBImage.hh\"","\"Ravl/Image/ByteRGBValue.hh\"");
  theFile.GlobalSubst("\"amma/YUVImage.hh\"","\"Ravl/Image/ByteYUVValue.hh\"");
  theFile.GlobalSubst("\"amma/YUVValue.hh\"","\"Ravl/Image/ByteYUVValue.hh\"");
  theFile.GlobalSubst("\"amma/RGBValue.hh\"","\"Ravl/Image/ByteRGBValue.hh\"");
  theFile.GlobalSubst("\"amma/GreyVal.hh\"","\"Ravl/Types.hh\"");
  theFile.GlobalSubst("\"amma/VecMat2d.hh\"","\"Ravl/VectorMatrix2d.hh\"");
  theFile.GlobalSubst("\"amma/VecMat3d.hh\"","\"Ravl/VectorMatrix3d.hh\"");
  theFile.GlobalSubst("\"amma/VecMat4d.hh\"","\"Ravl/VectorMatrix4d.hh\"");
  theFile.GlobalSubst("\"amma/Motion/LMSOpticFlow.hh\"","\"Ravl/Image/LMSOpticFlow.hh\"");
  theFile.GlobalSubst("\"amma/IndexR1d.hh\"","\"Ravl/IndexRange1d.hh\"");
  theFile.GlobalSubst("\"amma/SArr1Iter.hh\"","\"Ravl/SArray1dIter.hh\"");
  
  if(guiUpdates) {
    theFile.GlobalSubst("GUIDEBUG","DODEBUG");
      
    theFile.GlobalSubst("GUIManager","Manager");
    theFile.GlobalSubst("GUIWidget","Widget");
    theFile.GlobalSubst("GUIMouseEvent","MouseEvent");
    theFile.GlobalSubst("GUICListEvent","CListEvent");
    theFile.GlobalSubst("GUICursor","Cursor");
    theFile.GlobalSubst("GUIWindow","Window");
    theFile.GlobalSubst("GUIToolTips","ToolTips");
    theFile.GlobalSubst("GUIContainerWidget","ContainerWidget");
    theFile.GlobalSubst("GUIButton","Button");
    theFile.GlobalSubst("GUIToggleButton","ToggleButton");
    theFile.GlobalSubst("GUICheckButton","CheckButton");
    theFile.GlobalSubst("GUICanvas","Canvas");
    theFile.GlobalSubst("GUIMenu","Menu");
    theFile.GlobalSubst("GUILabel","Label");
    theFile.GlobalSubst("GUIPixmap","Pixmap");
    theFile.GlobalSubst("GUIFileSelector","FileSelector");
    theFile.GlobalSubst("GUITextEntry","TextEntry");
    theFile.GlobalSubst("GUISlider","Slider");
    theFile.GlobalSubst("GUIOneChild","OneChild");
    theFile.GlobalSubst("GUIScrolledArea","ScrolledArea");
    theFile.GlobalSubst("GUIFixed","Fixed");
    theFile.GlobalSubst("GUIStatusBar","StatusBar");
    theFile.GlobalSubst("GUICombo","Combo");
    theFile.GlobalSubst("GUIList","List");
    theFile.GlobalSubst("GUIRawCanvas","RawCanvas");
    theFile.GlobalSubst("GUITable","Table");
    theFile.GlobalSubst("GUIRuler","Ruler");
    theFile.GlobalSubst("GUISpinButton","SpinButton");
    theFile.GlobalSubst("GUITree","Tree");
    theFile.GlobalSubst("GUIPackInfo","PackInfo");
    theFile.GlobalSubst("GUISeparator","Separator");
    theFile.GlobalSubst("GUIBitmap","Bitmap");
    theFile.GlobalSubst("GUINotebook","Notebook");
    theFile.GlobalSubst("GUICList","CList");
    theFile.GlobalSubst("GUIVLabel","VLabel");
    theFile.GlobalSubst("GUIGraph1d","Graph1d");
    theFile.GlobalSubst("GUIRadioButton","RadioButton");
    theFile.GlobalSubst("GUILBox","LBox");
    theFile.GlobalSubst("GUIEventBox","EventBox");
    theFile.GlobalSubst("GUIFrame","Frame");
    theFile.GlobalSubst("GUIProgressBar","ProgressBar");

    // Other stuff.
    theFile.GlobalSubst("StdGUI","RavlGUIN");
    theFile.GlobalSubst("#define GUI","#define RAVLGUI");
    theFile.GlobalSubst("#ifndef GUI","#ifndef RAVLGUI");

    // Helper functions.
    theFile.GlobalSubst("MenuCheckItemRef(","MenuCheckItemR(");
      
  }
  if(guiSrc) {
    theFile.GlobalSubst("DPEntityC::Body()","WidgetC::Body()");
    theFile.GlobalSubst("return dynamic_cast<","return static_cast<");
  }
    
  // Image.
    
  theFile.GlobalSubst("ImageIterC","Array2dIterC");
  theFile.GlobalSubst("Image2IterC","Array2dIter2C");
  theFile.GlobalSubst("Image3IterC","Array2dIter3C");
  theFile.GlobalSubst("ByteImageC ","ImageC<ByteT> ");
  theFile.GlobalSubst("RGBImageC "," ImageC<ByteRGBValueC> ");
  theFile.GlobalSubst("YUVImageC "," ImageC<ByteYUVValueC> ");
  theFile.GlobalSubst("RGBPointC","RealRGBValueC");
  theFile.GlobalSubst("YUVPointC","RealYUVValueC");
  theFile.GlobalSubst("DoubleImageValueT","RealT");
  theFile.GlobalSubst("ByteGreyValueT","ByteT");
  theFile.GlobalSubst("namespace StdImageN","namespace RavlImageN");
  theFile.GlobalSubst("PixelC","Index2dC");
  theFile.GlobalSubst("NumImageC<","ImageC<");
  theFile.GlobalSubst("NumImageC ","ImageC ");

  // PatternRec
  
  theFile.GlobalSubst("NumMagnitudeC","DistanceC");
  theFile.GlobalSubst("NumLabelC","UIntT");
  
  // Core.
  
  theFile.GlobalSubst("BodyRefCounterC","RCBodyC");
  theFile.GlobalSubst("BodyRefCounterVC","RCBodyVC");
  theFile.GlobalSubst("Launch(","ThreadLaunch(");
  theFile.GlobalSubst("MsgPipeC<","MessageQueueC<");
  theFile.GlobalSubst("SignalEvent(","Trigger(");
  theFile.GlobalSubst("PThread::","");
  theFile.GlobalSubst("StdDP::","");
  theFile.GlobalSubst("StdIO::","");
  theFile.GlobalSubst("StdConstC::","RavlConstN::");
  theFile.GlobalSubst("BooleanT","bool");
  theFile.GlobalSubst("assert(","RavlAssert(");
  theFile.GlobalSubst("IndexT","IndexC");
  theFile.GlobalSubst("aTRUE","true");
  theFile.GlobalSubst("aFALSE","false");
  theFile.GlobalSubst("TRUE","true");
  theFile.GlobalSubst("FALSE","false");
  theFile.GlobalSubst("amma","Ravl");
  theFile.GlobalSubst("AMMA","RAVL");
  theFile.GlobalSubst("HashARC<","HashC<");
  theFile.GlobalSubst("AMMA_CHECK","QMAKE_CHECK");
  theFile.GlobalSubst("Matrix2d2C","Matrix2dC");
  theFile.GlobalSubst("Matrix3d3C","Matrix3dC");
  theFile.GlobalSubst("IStrStreamC","StrIStreamC");
  theFile.GlobalSubst("OStrStreamC","StrOStreamC");
  theFile.GlobalSubst("IBitStreamC","BitIStreamC");
  theFile.GlobalSubst("OBitStreamC","BitOStreamC");
  theFile.GlobalSubst(".RDim()",".Rows()");
  theFile.GlobalSubst(".CDim()",".Cols()");
  theFile.GlobalSubst(".Rnum()",".Rows()");
  theFile.GlobalSubst(".Cnum()",".Cols()");
  theFile.GlobalSubst(".A00()","[0][0]");
  theFile.GlobalSubst(".A01()","[0][1]");
  theFile.GlobalSubst(".A02()","[0][2]");
  theFile.GlobalSubst(".A03()","[0][3]");
  theFile.GlobalSubst(".A10()","[1][0]");
  theFile.GlobalSubst(".A11()","[1][1]");
  theFile.GlobalSubst(".A12()","[1][2]");
  theFile.GlobalSubst(".A13()","[1][3]");
  theFile.GlobalSubst(".A20()","[2][0]");
  theFile.GlobalSubst(".A21()","[2][1]");
  theFile.GlobalSubst(".A22()","[2][2]");
  theFile.GlobalSubst(".A22()","[2][3]");
  theFile.GlobalSubst(".A30()","[3][0]");
  theFile.GlobalSubst(".A31()","[3][1]");
  theFile.GlobalSubst(".A32()","[3][2]");
  theFile.GlobalSubst(".A32()","[3][3]");
  theFile.GlobalSubst("ConstDLIterC","DLIterC");
  
  if(theFile.IsModified()) {
    std::cerr << "Updated file :" << fn << "\n";
    if(!dryRun)
      theFile.Save();
  }
  return true;
}


static bool CheckDirectory(StringC &dir,DefsMkFileC &defs) {
  if(verb)
    std::cerr << "Checking '" << dir << "' \n";
  StringListC source = defs.AllSources();
  for(DLIterC<StringC> it(source);it;it++) {
    FilenameC fn(dir + filenameSeperator + *it);
    UpdateFile(fn);
  }
  return true;
}

int main(int nargs,char **argv) {
  OptionC option(nargs,argv);
  FilenameC fn = option.String("i",".","Input filename. ");
  bool all = option.Boolean("a",false,"Go into inactive directories as well. ");
  dryRun = option.Boolean("d",false,"Do a dry run.");
  guiSrc = option.Boolean("guis",true,"do gui renaming's");
  guiUpdates = option.Boolean("gui",false,"do renaming for GUI source code");
  verb = option.Boolean("v",false,"Verbose mode.");
  option.Check();
  
  if(!fn.Exists()) {
    std::cerr << "Can't find file " << fn << "\n";
    return 1;
  }
  
  if(fn.IsDirectory()) {
    SourceCodeManagerC chkit(fn);
    if(verb)
      chkit.SetVerbose(true);  
    chkit.ForAllDirs(CallFunc2C<StringC&,DefsMkFileC&,bool>(&CheckDirectory),all);
  } else
    UpdateFile(fn);
  
  return 0;
}
