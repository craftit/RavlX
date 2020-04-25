// Copyright (C) 2003, Omniperception Ltd.
// file-header-ends-here
#ifndef RAVL_FACE_CONTROLWIN_HEADER
#define RAVL_FACE_CONTROLWIN_HEADER 1
//! docentry = "Ravl.API.Applications.Mainment Tool"
//! author   = "Kieron Messer"
//! userlevel=Develop
//! lib=RavlFace

#include "ViewPage.hh"

#include "Ravl/Face/FaceInfoDb.hh"
#include "Ravl/Face/SightingSet.hh"
#include "Ravl/GUI/Window.hh"
#include "Ravl/GUI/Notebook.hh"
#include "Ravl/StringList.hh"


namespace RavlN {
  namespace FaceN {

    class ControlWinBodyC : public WindowBodyC
    {

    public:

      ControlWinBodyC(const FaceInfoDbC & db, const StringListC & sightingSets, bool autoScale);
      //: Constructor.

      bool Create();
      // Creates the window

      bool Quit();
      //: "Quit" handler

      bool Refresh();
      //: refresh

    protected:
      NotebookC notebook;
      //: The notebook

      DListC<WidgetC> pages;
      //: The pages

      FaceInfoDbC faceDb;
      //: A database of the faces

      SightingSetC m_sightingSet;
      //: The sighting set for the faces

      ViewPageC viewPage;
      bool m_autoScale;
      //: Page for view databases

      friend class ControlWinC;

    };

    //! userlevel=Basic

    class ControlWinC : public WindowC
    {

    public:
      ControlWinC()
      {
      }
      //: Default constructor.
      // Creates an invalid handle.

      ControlWinC(FaceInfoDbC & db, const StringListC & sightingSets, bool autoScale) :
          WindowC(*new ControlWinBodyC(db, sightingSets, autoScale))
      {
      }
      //: Constructor.

    protected:

      ControlWinC(ControlWinBodyC &bod) :
          WindowC(bod)
      {
      }
      //: Body Constructor.

      ControlWinBodyC &Body()
      {
        return static_cast<ControlWinBodyC &>(WindowC::Body());
      }
      //: Access body.

      const ControlWinBodyC &Body() const
      {
        return static_cast<const ControlWinBodyC &>(WindowC::Body());
      }
      //: Access body.

    public:

      friend class ControlWinBodyC;

    };
  }
}

#endif //MAIN_WINDOW_HEADER
