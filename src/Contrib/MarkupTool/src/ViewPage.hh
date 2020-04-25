// Copyright (C) 2003, Omniperception Ltd.
// file-header-ends-here
#ifndef VIEWPAGE_HEADER
#define VIEWPAGE_HEADER 1
//! docentry = "Ravl.API.Applications.Mainment Tool"
//! author   = "Kieron Messer"
//! userlevel=Develop
//! lib=RavlFace

#include "Ravl/GUI/LBox.hh"

#include "Ravl/Face/FaceInfoDb.hh"
#include "Ravl/Face/SightingSet.hh"
#include "Ravl/GUI/Menu.hh"
#include "Ravl/GUI/GUIMarkupCanvas.hh"
#include "Ravl/GUI/MarkupPoint2d.hh"
#include "Ravl/GUI/RadioButton.hh"
#include "Ravl/GUI/TextEntry.hh"
#include "Ravl/GUI/ComboBoxEntryText.hh"
#include "Ravl/GUI/TreeStore.hh"
#include "Ravl/GUI/TreeView.hh"
#include "Ravl/GUI/FileSelector.hh"
#include "Ravl/GUI/ToggleButton.hh"
#include "Ravl/Threads/Mutex.hh"
#include "Ravl/Cache.hh"

namespace RavlN {
  namespace FaceN {

    using namespace RavlGUIN;
    
    class ViewPageBodyC : public LBoxBodyC
    {

    public:

      ViewPageBodyC(FaceInfoDbC & db, SightingSetC & sightingSet, bool autoScale);
      //: Constructor.

      bool Create();
      // Creates the window

      bool Init();
      //: Init stuff

      bool UpdateSightingSet();
      //: Update the sighting set

      bool SaveData();
      //: Save data

      bool LoadData(const StringC & faceId); 
      //: Load data

      bool NextPrevButton(IntT & v);

      bool SelectRow();
      //: Select row

      bool ProcessKeyboard(GdkEventKey *KeyEvent);
      //: process keyboard event

      bool UpdateTreeStore();
      //: Update the tree store

      bool Save(const StringC & filename);
      //: Save the xml file

      bool SaveButton();
      //: Save the xml file

      bool SaveSelectedButton();
      //: Just save the XML which have been selected

      bool SaveSelected(const StringC & filename);
      //: Just save the XML which have been selected

      bool DeleteSelected(bool removeFromDatabase=true);
      //: Save the xml file

      bool RenameSelected();
      //: Rename the selected faces

      bool ChangeActualId(TreeModelIterC & at, StringC & change);

      //: Change to sighting mode
      bool SightingMode(bool sightingMode);

      //: Attempt to perform an automatic merging of the sightings
      bool AutoMergeSighting();

      //: Create a new sighting out of the selected faces
      bool CreateSighting();

      // Show file dialog when Move button selected
      bool MoveButton();

      // Move selected faces into this xml-file
      bool MoveSelected(const StringC & xmlFile);

    protected:
      FaceInfoDbC faceDb;
      //: A database of the faces

      SightingSetC m_sightingSet;
      //: The sighting set, if available

      bool m_autoScale;
      //: Do we want to scale the images

      DLIterC<StringC> iter;
      //: The iterator which points at the image we are looking at

      StringC m_currentFaceId;

      RCHashC<StringC, TreeModelIterC>m_selected;

      StringC faceDbFile;
      //: The faceDb file to save as

      //: The GUI to display the marked up face
      GUIMarkupCanvasC canvas;

      RCHashC<IntT, MarkupPoint2dC> m_markupPoints;
      ImageC<ByteRGBValueC> image; // copy of the current image


      TextEntryC imagepath;
      TreeViewC treeView;
      TreeStoreC treeStore;

      //: Glasses
      RadioButtonGroupT glassesButtonGrp;
      RadioButtonC noglasses;
      RadioButtonC glasses;
      RadioButtonC unknown;
      ToggleButtonC m_sightingMode;
      ButtonC m_createSightingButton;

      //: Pose
      TextEntryC textEntryPose;
      TextEntryC textEntryRename;


      //: Date of image
      TextEntryC date;
      FileSelectorC m_fileSelectorSave;
      FileSelectorC m_fileSelectorSaveSelected;
      FileSelectorC m_fileSelectorMoveSelected;

      bool m_markupMode;

      MutexC m_mutex;

      TreeModelIterC m_lastParent;
      CacheC<StringC, TreeModelIterC> m_prevSelected;

      friend class ViewPageC;

    };

    //! userlevel=Basic

    class ViewPageC : public LBoxC
    {

    public:
      ViewPageC()
      {
      }
      //: Default constructor.
      // Creates an invalid handle.

      ViewPageC(FaceInfoDbC & db, SightingSetC & sightingSet, bool autoScale) :
          LBoxC(*new ViewPageBodyC(db, sightingSet, autoScale))
      {
      }
      //: Constructor.

    protected:

      ViewPageC(ViewPageBodyC &bod) :
          LBoxC(bod)
      {
      }
      //: Body Constructor.

      ViewPageBodyC &Body()
      {
        return static_cast<ViewPageBodyC &>(LBoxC::Body());
      }
      //: Access body.

      const ViewPageBodyC &Body() const
      {
        return static_cast<const ViewPageBodyC &>(LBoxC::Body());
      }
      //: Access body.

    public:

      friend class ViewPageBodyC;

    };

  }
}
#endif //MAIN_WINDOW_HEADER
