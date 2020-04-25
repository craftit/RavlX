// Copyright (C) 2003, Omniperception Ltd.
// file-header-ends-here
//! lib=RavlFace

#include "ViewPage.hh"

#include "Ravl/IO.hh"

#include "Ravl/GUI/MessageBox.hh"
#include "Ravl/GUI/MarkupImageRGB.hh"
#include "Ravl/GUI/Frame.hh"
#include "Ravl/GUI/Label.hh"
#include "Ravl/GUI/ScrolledArea.hh"
#include "Ravl/GUI/PackInfo.hh"
#include "Ravl/GUI/FileSelector.hh"
#include "Ravl/RLog.hh"
#include "Ravl/Face/FaceInfoDb.hh"

#include <gdk/gdkevents.h>
//#include "Ravl/GUI/Manager.hh"
//#include "Ravl/GUI/LBox.hh"

namespace RavlN {
  namespace FaceN {

    using namespace RavlGUIN;

    bool StringCmpFunc(const StringC &E1, const StringC &E2)
    {
      return E1.CaseCmp(E2) <= 0;
    }

    /////////////////////////////////////
    //: Construct from a subject table
    //: and an xml face database
    /////////////////////////////////////

    ViewPageBodyC::ViewPageBodyC(FaceInfoDbC & db, SightingSetC & sightingSet, bool autoScale) :
        LBoxBodyC(true),
        faceDb(db),
        m_sightingSet(sightingSet),
        m_autoScale(autoScale),
        canvas(800, 800),
        m_markupMode(false),
        m_prevSelected(25)
    {

    }

    //////////////////////////////////////
    //: Create the GUI
    //////////////////////////////////////

    bool ViewPageBodyC::Create()
    {

      //: the box that holds the path
      imagepath = TextEntryC("unknown");
      //imageSize = TextEntryC("unknown");
      //dateOfBirth = TextEntryC(DateC::NowLocal().ODBC());
      textEntryPose = TextEntryC("YPR * * *");
      textEntryRename = TextEntryC("Unknown");
      m_sightingMode = ToggleButtonC("Sighting Mode", false);

      //: Previous and next buttons
      ButtonC next = ButtonR("Next", *this, &ViewPageBodyC::NextPrevButton, 1);
      ButtonC prev = ButtonR("Prev", *this, &ViewPageBodyC::NextPrevButton, 2);
      ButtonC save = ButtonR("Save", *this, &ViewPageBodyC::SaveButton);
      ButtonC moveButton = ButtonR("Move", *this, &ViewPageBodyC::MoveButton);

      ButtonC deleteButton = ButtonR("Delete", *this, &ViewPageBodyC::DeleteSelected, true);
      ButtonC saveSelectedButton = ButtonR("Save Selected", *this, &ViewPageBodyC::SaveSelectedButton);
      ButtonC renameButton = ButtonR("Rename", *this, &ViewPageBodyC::RenameSelected);
      ButtonC autoMergeSightingButton = ButtonR("Auto Merge Sighting", *this, &ViewPageBodyC::AutoMergeSighting);
      m_createSightingButton = ButtonR("Create Sighting", *this, &ViewPageBodyC::CreateSighting);

      //: Make the tree store
      SArray1dC<AttributeTypeC> types(3);
      types[0] = AttributeTypeStringC("ID", "...");
      types[1] = AttributeTypeStringC("Face ID", "...");
      types[2] = AttributeTypeBoolC("Select", "...");
      treeStore = TreeStoreC(types);
      UpdateTreeStore();
      treeView = TreeViewC(treeStore);
      treeView.SetAttribute(0, "editable", "1", false);
      treeView.SetAttribute(0, "sortable", "1", false);
      treeView.SetAttribute(1, "sortable", "1", false);
      treeView.SetAttribute(1, "resizable", "1", false);
      ConnectRef(treeView.SelectionChanged(), *this, &ViewPageBodyC::SelectRow);
      ConnectRef(m_sightingMode.SigClicked(), *this, &ViewPageBodyC::SightingMode);

      //: Glasses selector
      noglasses = RadioButton("Yes", glassesButtonGrp);
      glasses = RadioButton("No", glassesButtonGrp);
      unknown = RadioButton("Unknown", glassesButtonGrp);

      //: Sex selector
      //male = RadioButton("Male", sexButtonGrp);
      //female = RadioButton("Female", sexButtonGrp);
      //unknownsex = RadioButton("Unknown", sexButtonGrp);

      //: date of image
      date = TextEntryC(DateC::NowLocal().ODBC());

      FrameC glassesFrame(VBox(noglasses + glasses + unknown), "Glasses");
      FrameC poseFrame(textEntryPose, "Pose");
      FrameC renameFrame(HBox(textEntryRename + PackInfoC(renameButton, false, false)), "Rename Selected");
      //FrameC sexFrame(VBox(male + female + unknownsex), "Sex");
      //LabelC dobLabel("Date of Birth");
      //FrameC dobFrame(HBox(dobLabel + dateOfBirth), "Other Info");

      //: This is the layout
      LabelC dateLabel("Capture Date");
      Add(VBox(PackInfoC(HBox(PackInfoC(VBox(PackInfoC(canvas, true)
          + HBox(save + saveSelectedButton + deleteButton + moveButton + m_sightingMode + m_createSightingButton + autoMergeSightingButton)),
          true) + PackInfoC(ScrolledAreaC(treeView, 350, 30), false)),
          true)
          + PackInfoC(HBox(LabelC("Path") + imagepath + HBox(dateLabel + date)), false)
          + PackInfoC(HBox(glassesFrame + poseFrame + renameFrame), false)));

      // Connect an event handler to the frame widget.
      ConnectRef(canvas.Signal("key_press_event"), *this, &ViewPageBodyC::ProcessKeyboard);
      ConnectRef(treeView.Signal("key_press_event"), *this, &ViewPageBodyC::ProcessKeyboard);

      // Connect the changed signal for column 1 to 'EditCallback'
      ConnectRef(treeView.ChangedSignal("ID"), *this, &ViewPageBodyC::ChangeActualId);

      m_fileSelectorSave = FileSelectorC("Save XML file", "*.xml");
      ConnectRef(m_fileSelectorSave.Selected(), *this, &ViewPageBodyC::Save);

      m_fileSelectorSaveSelected = FileSelectorC("Save XML file", "*.xml");
      ConnectRef(m_fileSelectorSaveSelected.Selected(), *this, &ViewPageBodyC::SaveSelected);

      m_fileSelectorMoveSelected = FileSelectorC("File to move faces to", "*.xml");
      ConnectRef(m_fileSelectorMoveSelected.Selected(), *this, &ViewPageBodyC::MoveSelected);

      m_createSightingButton.SetState(GTK_STATE_INSENSITIVE);

      Init();

      LoadData(faceDb.Keys().First());

      // Create window
      return LBoxBodyC::Create();
    }

    bool ViewPageBodyC::Init()
    {

      MutexLockC lock(m_mutex);

      // reset the selected list
      m_selected.Empty();

      // Sort out the sighting set
      if (!UpdateSightingSet()) {
        rWarning("Failed to update sighting set!");
        return false;
      }

      return true;
    }

    // This routine checks that the sighting set in memory is possible
    // given the face database
    bool ViewPageBodyC::UpdateSightingSet()
    {

      SightingSetC sightingSet;


      // Go through the sighting set currently held in memory
      for (DArray1dIterC<SightingC> it(m_sightingSet); it; it++) {
        bool first = true;
        StringC actualId;
        SightingC sighting("whocares");

        // Go through all the faces in the sightings

        for (DLIterC<StringC> faceIt(it.Data().FaceIds()); faceIt; faceIt++) {

          // Is the face in the database, if not just skip it
          if (!faceDb.IsElm(*faceIt)) {
            rDebug("Face ID '%s' in sighting is not in DB", faceIt.Data().data());
            continue;
          }

          // Is it first face in sighting?
          if (first) {
            actualId = faceDb[*faceIt].ActualId();
            sighting = SightingC(actualId);
            first = false;
          }

          // Check that the ID of the face is the same of the sighting being created
          // If it isn't this is usually because a sighting has been mis-marked up.
          // For now I do not fix it, I just remove the face from the sighting
          if (faceDb[*faceIt].ActualId() != actualId) {
            rWarning("Different actual ids in sighting.  Sighting ID '%s'; Face ID '%s'.  Removing face from sighting.",
                faceDb[*faceIt].ActualId().data(),
                faceIt.Data().data());
             continue;
          }

          // OK looks like we are OK to add the face to sighting
          sighting.AddFaceId(*faceIt);
        }

        // Add sighting to sighting set
        if (sighting.FaceIds().Size() > 0) {
          sightingSet.Append(sighting);
        }

      }

      // reset the internal data structure
      m_sightingSet = sightingSet;
      return true;
    }

    //////////////////////////////////////////////////////////
    //: This is called when a new subject has been selected
    //: Any information that has been updated on the page
    //: is saved into memory
    /////////////////////////////////////////////////////////

    bool ViewPageBodyC::SaveData()
    {
      //rDebug("Saving face '%s'", iter.Data().data());
      FaceInfoC info = faceDb[*iter];

      //: The eye positions might of changed
      //////////////////////////////////////
      for (HashIterC<IntT, MarkupPoint2dC> it(m_markupPoints); it; it++) {
        ImagePointFeatureC feature;
        if (!info.FeatureSet().Feature(it.Key(), feature)) {
          rWarning("Urm, could not find feature whilst trying to save!");
          continue;
        }

        if (feature.Location() != it.Data().Position()) {
          rDebug("Changed position of %s", feature.Description().data());
          info.FeatureSet().Set(it.Key(), it.Data().Position());
        } else {
          //rDebug("No need to save point '%s'", feature.Description().data());
        }

      }

      return true;
    }

    //////////////////////////////////////////////
    //: Load in data in the main data viewer page
    //////////////////////////////////////////////

    bool ViewPageBodyC::LoadData(const StringC & faceId)
    {

      //: Check iter is pointing at sensible stuff
      if (!faceDb.IsElm(faceId)) {
        RavlError("Face not '%s' in database.\n", StringOf(faceId).data());
        return false;
      }

      bool selected = m_selected.IsElm(faceId);
      RavlDebug("Subject '%s'; FaceId '%s'; Selected %d", faceDb[faceId].ActualId().data(), faceId.data(), (int)selected);

      iter = DLIterC<StringC>(faceDb.Keys());
      for (; iter.IsElm(); iter++) {
        if (*iter == faceId)
          break;
      }

      //: Load in the image from file
      ///////////////////////////////
      FaceInfoC info = faceDb[faceId];
      ImageC<ByteRGBValueC> im;
      if (!Load(info.OrigImage(), im)) {
        rWarning("Image unable to be loaded %s", info.OrigImage().data());
        AlertBox("Unable to load image");
        return false;
      }

      //: Add the image to the markup
      ///////////////////////////////
      FrameMarkupC fm;
      image = im.Copy();
      fm.Markup().InsLast(MarkupImageRGBC(-1, 0, image));

      if (m_markupMode) {
        if (m_markupPoints.IsEmpty()) {
          if (info.FeatureSet().IsValid()) {
            //DListC<Point2dC> points;
            for (HashIterC<IntT, ImagePointFeatureC> it(info.FeatureSet().FeatureIterator()); it; it++) {
              rDebug("Displaying point '%s'", it.Data().Description().data());
              MarkupPoint2dC mup(1, 1, it.Data().Location(), MP2DS_CrossHair);
              fm.Markup().InsLast(mup);
              m_markupPoints.Insert(it.Key(), mup);
              //points.InsLast(it.Data().Location());
            }
          }
        } else {
          for (HashIterC<IntT, MarkupPoint2dC> it(m_markupPoints); it; it++) {
            fm.Markup().InsLast(it.Data());
          }
        }
      } else {
        if (info.FeatureSet().IsValid()) {
          UIntT id = 0;
          m_markupPoints.Empty();
          for (HashIterC<IntT, ImagePointFeatureC> it(info.FeatureSet().FeatureIterator()); it; it++) {
            //rDebug("Displaying point '%s'", it.Data().Description().data());
            MarkupPoint2dC mup(id, 1, it.Data().Location(), MP2DS_CrossHair);
            fm.Markup().InsLast(mup);
            m_markupPoints.Insert(it.Key(), mup);
            id++;
          }
        }
      }

      //: Scale if we have eye-points..turn this off just so it remembers last scale
      ////////////////////////////////////////////
      if (m_autoScale) {
        RealT canvasRows = canvas.SizeY();
        RealT canvasCols = canvas.SizeX();
        //RealT currentScale = canvas.Scale().Row();
        //rInfo("Current Scale %0.2f", currentScale);

        RealT scale;
        Point2dC offset;
        if (image.Rows() > image.Cols()) {
          scale = canvasRows / (RealT) image.Rows();
          offset = Point2dC(0.0, 0.5 * (canvasCols - (scale * image.Cols())));

        } else {
          scale = canvasCols / (RealT) image.Cols();
          offset = Point2dC(0.5 * (canvasRows - (scale * image.Rows())), 0.0);
        }
        //rInfo("Scale: %0.2f", scale);
        canvas.SetScale(Vector2dC(scale, scale));
        canvas.SetOffset(offset);
      }

      //: Update the canvas with all our information
      //////////////////////////////////////////////
      canvas.UpdateMarkup(fm);
      //StringC imageInfo = (StringC) im.Rows() + " " + (StringC) im.Cols();
      //imageSize.Text(imageInfo);

      //: Set the image path
      imagepath.Text(faceDb[faceId].OrigImage());

      //: Check whether image has glasses
      if (info.Glasses() == "no")
        noglasses.SetToggle(true);
      else if (info.Glasses() == "yes")
        glasses.SetToggle(true);
      else
        unknown.SetToggle(true);

      //: Check the pose information
      textEntryPose.Text(info.Pose());

      //: Check the date of image
      date.Text(info.Date().ODBC());

      return true;
    }

    //////////////////////////////////////////////
    //: Will sequentially walk through images in
    //: the tree store
    ///////////////////////////////////////////////

    bool ViewPageBodyC::NextPrevButton(IntT & v)
    {
      //: work out whether glasses
      StringC desc;
      if (v == 1) {
        SaveData();
        iter++;
        if (!iter) {
          // end reached
          iter.Nth(-1);
        }
      } else if (v == 2) {
        SaveData();
        iter--;
        if (!iter) {
          // beginning reached
          iter.Nth(0);
        }
      }
      LoadData(*iter);
      return true;
    }

    //////////////////////////////////////////////

    bool ViewPageBodyC::SaveButton()
    {
      m_fileSelectorSave.GUIShow();
      return true;
    }

    //////////////////////////////////////////////

    bool ViewPageBodyC::Save(const StringC & filename)
    {
      rInfo("Saving to file '%s'", filename.data());
      if (!FaceN::Save(filename, faceDb)) {
        AlertBox(StringC("trouble saving database"));
        return false;
      }

      if (!m_sightingSet.IsEmpty()) {

        FilenameC fn(filename);
        FilenameC sightingSetFile = fn.PathComponent() + "/" + fn.BaseNameComponent() + "_sightingSet.xml";

        if (!UpdateSightingSet()) {
          rWarning("Failed to update sighting set!");
          return false;
        }

        if (!RavlN::Save(sightingSetFile, m_sightingSet)) {
          rWarning("Trouble saving sighting set file '%s'", sightingSetFile.data());
          return false;
        }
        rInfo("Saved sighting set file to '%s'", sightingSetFile.data());
      }
      return true;
    }

    //////////////////////////////////////////////
    bool ViewPageBodyC::SaveSelectedButton()
    {
      m_fileSelectorSaveSelected.GUIShow();
      return true;
    }

    //////////////////////////////////////////////

    bool ViewPageBodyC::SaveSelected(const StringC & filename)
    {
      rInfo("Saving selected data to file '%s'", filename.data());

      FaceInfoDbC newDb;
      for (HashIterC<StringC, TreeModelIterC> it(m_selected); it; it++) {
        newDb.Insert(it.Key(), faceDb[it.Key()]);
      }

      if (!FaceN::Save(filename, newDb)) {
        AlertBox(StringC("trouble saving database"));
        return false;
      }

      return true;
    }

    //////////////////////////////////////////////

    bool ViewPageBodyC::DeleteSelected(bool removeFromDatabase)
    {
      MutexLockC lock(m_mutex);

      RCHashC<StringC, TreeModelIterC> withChildren;
      for (HashIterC<StringC, TreeModelIterC> it(m_selected); it; it++) {

        if (!faceDb.IsElm(it.Key())) {
          rWarning("Face not in database");
          return false;
        }

        // Delete it from database
        if(removeFromDatabase)
          faceDb.Del(it.Key());

        // need to delete from tree
        if (it.Data().HasChildren()) {
          withChildren.Insert(it.Key(), it.Data());

        } else {
          treeStore.GUIDeleteRow(it.Data());
        }
      }

      for (HashIterC<StringC, TreeModelIterC> it(withChildren); it; it++) {

        // it might not have any children anymore
        if (!it.Data().HasChildren()) {
          treeStore.GUIDeleteRow(it.Data());
          continue;
        }

        // Urm what to do here
        TreeModelIterC childIt = it.Data().Children();

        StringC id;
        if (!treeStore.GetValue(childIt, 0, id)) {
          continue;
        }

        StringC faceId;
        if (!treeStore.GetValue(childIt, 1, faceId)) {
          continue;
        }

        bool selected;
        if (!treeStore.GetValue(childIt, 2, selected)) {
          continue;
        }

        treeStore.GUISetValue(it.Data(), 0, id);
        treeStore.GUISetValue(it.Data(), 1, faceId);
        treeStore.GUISetValue(it.Data(), 2, selected);

        treeStore.DeleteRow(childIt);

      }

      lock.Unlock();
      Init(); // this resets the sighting set

      //treeStore.GUIEmpty();
      //UpdateTreeStore();
      LoadData(faceDb.Keys().First());
      return true;
    }

    //////////////////////////////////////////////////////
    //: Called when a new row is selectd in the tree store
    //////////////////////////////////////////////////////

    bool ViewPageBodyC::SelectRow()
    {

      StringC faceId;
      DListC<TreeModelIterC> selected = treeView.GUISelected();
      if (selected.IsEmpty()) {
        return false; // none selected, do not do anything
      }
      SaveData();

      if (!treeStore.GetValue(selected.First(), 1, faceId))
        return false; // had trouble getting value, do nothing

      // put it into our cache of recently selected
      m_prevSelected.Insert(faceId, selected.First());
      return LoadData(faceId);

    }

    bool ViewPageBodyC::ProcessKeyboard(GdkEventKey *KeyEvent)
    {
      //cerr << "Key pressed : " << KeyEvent->keyval << "\n";
      if (KeyEvent->keyval == 65364) {
        DListC<TreeModelIterC> selected = treeView.GUISelected();
        if (selected.IsEmpty())
          return true; // none selected, do not do anything
        TreeModelIterC &tvIter = selected.First();
        if (tvIter.HasChildren()) {
          m_lastParent = tvIter.Copy();
          treeView.GUIExpand(tvIter);
          treeView.GUISelectIter(tvIter.Children());

        } else if (tvIter.Next()) {
          treeView.GUISelectIter(tvIter);
        } else {
          if (m_lastParent.IsElm()) {
            if (m_lastParent.Next()) {
              treeView.GUISelectIter(m_lastParent);
            }
          }

        }
      }
      // right key
      else if (KeyEvent->keyval == 65363) {
        treeView.GUIExpandAll();
      }
      // left key
      else if (KeyEvent->keyval == 65361) {
        treeView.GUICollapseAll();
      }
      // m key - toggle markup mode
      else if (KeyEvent->keyval == 109) {
        if (m_markupMode) {
          rInfo("Turning off mark-up mode!");
          m_markupMode = false;
          m_autoScale = true;
        } else {
          rInfo("Turning on mark-up mode!");
          m_markupMode = true;
          m_autoScale = false;
        }
      }
      // Select Enter or Space
      else if (KeyEvent->keyval == 65293 || KeyEvent->keyval == 32) {
        DListC<TreeModelIterC> selected = treeView.GUISelected();
        if (selected.IsEmpty())
          return true; // none selected, do not do anything
        StringC faceId;
        if (!treeStore.GetValue(selected.First(), 1, faceId))
          return false; // had trouble getting value, do nothing
        bool selectedBox;
        if (!treeStore.GetValue(selected.First(), 2, selectedBox))
          return false;

        if (selectedBox) {
          treeStore.SetValue(selected.First(), 2, false);
          if (m_selected.IsElm(faceId)) {
            m_selected.Del(faceId);
          }
        } else {
          treeStore.SetValue(selected.First(), 2, true);
          m_selected.Insert(faceId, selected.First());
        }

      }

      return true; //event processed
    }

    /////////////////////////////////////////////////
    //: Called when we need to update the information
    //: held by the tree store
    //////////////////////////////////////////////////

    bool ViewPageBodyC::UpdateTreeStore()
    {

      if (m_sightingSet.IsEmpty() || !m_sightingMode.IsActive()) {
        TreeModelIterC iter1;
        TreeModelIterC iter2;
        RCHashC<StringC, DListC<FaceInfoC> > sorted = faceDb.Sort(true); // only get marked up images
        rInfo("Number of Clients %s and Number of Images %s", StringOf(sorted.Size()).data(), StringOf(faceDb.Size()).data());
        DListC<StringC> clients = faceDb.Clients();
        for (DLIterC<StringC> it(clients); it; it++) {
          DListC<FaceInfoC> faces = sorted[*it];
          if (!faces.IsEmpty()) {
            DLIterC<FaceInfoC> faceIt(faces);
            treeStore.AppendRow(iter1);
            //: do the first row
            treeStore.GUISetValue(iter1, 0, faceIt.Data().ActualId());
            treeStore.GUISetValue(iter1, 1, faceIt.Data().FaceId());
            treeStore.GUISetValue(iter1, 2, m_selected.IsElm(faceIt.Data().FaceId()));
            for (faceIt++; faceIt; faceIt++) {
              treeStore.AppendRow(iter2, iter1);
              treeStore.GUISetValue(iter2, 0, faceIt.Data().ActualId());
              treeStore.GUISetValue(iter2, 1, faceIt.Data().FaceId());
              treeStore.GUISetValue(iter2, 2, m_selected.IsElm(faceIt.Data().FaceId()));
            }

          }
        }
      } else {
        TreeModelIterC iter1;
        TreeModelIterC iter2;
        rInfo("Number of sightings %s", StringOf(m_sightingSet.Size()).data());
        for (DArray1dIterC<SightingC> it(m_sightingSet); it; it++) {
          DListC<StringC> faces = it.Data().FaceIds();
          if (!faces.IsEmpty()) {
            DLIterC<StringC> faceIt(faces);
            //: do the first row
            if (!faceDb.IsElm(faceIt.Data())) {
              AlertBox("Face ID specified in sighting is not in Face DB", &Manager.GetRootWindow());
              continue;
            }
            treeStore.AppendRow(iter1);
            FaceInfoC faceInfo = faceDb[faceIt.Data()];
            treeStore.GUISetValue(iter1, 0, faceInfo.ActualId());
            treeStore.GUISetValue(iter1, 1, faceInfo.FaceId());
            treeStore.GUISetValue(iter1, 2, m_selected.IsElm(faceInfo.FaceId()));
            for (faceIt++; faceIt; faceIt++) {

              if (!faceDb.IsElm(faceIt.Data())) {
                AlertBox("Face ID specified in sighting is not in Face DB", &Manager.GetRootWindow());
                continue;
              }
              FaceInfoC faceInfo = faceDb[faceIt.Data()];
              treeStore.AppendRow(iter2, iter1);
              treeStore.GUISetValue(iter2, 0, faceInfo.ActualId());
              treeStore.GUISetValue(iter2, 1, faceInfo.FaceId());
              treeStore.GUISetValue(iter2, 2, m_selected.IsElm(faceInfo.FaceId()));
            }

          }

        }
      }
      return true;
    }

    bool ViewPageBodyC::ChangeActualId(TreeModelIterC & at, StringC & to)
    {

      StringC faceId;
      treeStore.GetValue(at, 1, faceId);
      rInfo("Changing ID from %s to %s", faceId.data(), to.data());
      if (!faceDb.IsElm(faceId)) {
        AlertBox("Face ID is not in Face DB. Unable to change Actual ID.", &Manager.GetRootWindow());
        return false;
      }
      faceDb[faceId].ActualId() = to;
      treeStore.GUISetValue(at, 0, to);
      for (TreeModelIterC it(at.Children()); it.IsElm(); it.Next()) {
        StringC faceId;
        treeStore.GetValue(it, 1, faceId);
        rInfo("Changing ID from %s to %s", faceId.data(), to.data());
        if (!faceDb.IsElm(faceId)) {
          AlertBox("Face ID is not in Face DB.  Unable to change Actual ID.", &Manager.GetRootWindow());
          return false;
        }
        faceDb[faceId].ActualId() = to;
        treeStore.GUISetValue(it, 0, to);
      }
      return true;
    }

    bool ViewPageBodyC::RenameSelected()
    {
      MutexLockC lock(m_mutex);
      StringC to = textEntryRename.GUIText();
      if (to.IsEmpty()) {
        rWarning("Rename box is empty");
        return false;
      }

      FaceInfoDbC newDb;
      for (HashIterC<StringC, FaceInfoC> it(faceDb); it; it++) {

        if (m_selected.IsElm(it.Key())) {
          rInfo("Renaming from '%s' to '%s'", it.Key().data(), to.data());
          faceDb[it.Key()].ActualId() = to;
        }

      }
      m_selected.Empty();
      treeStore.GUIEmpty();
      UpdateTreeStore();
      LoadData(*iter);

      return true;
    }

    bool ViewPageBodyC::SightingMode(bool sightingMode)
    {

      if (m_sightingSet.IsEmpty() && m_sightingMode.GUIIsActive()) {
        m_sightingMode.GUISetToggle(false);
        AlertBox("No sighting set specified.  Unable to switch to Sighting Mode", &Manager.GetRootWindow());
        return false;
      }

      if(m_sightingMode.GUIIsActive()) {
        m_createSightingButton.GUISetState(GTK_STATE_NORMAL);
      } else {
        m_createSightingButton.SetState(GTK_STATE_INSENSITIVE);
      }

      // We need a complete refresh if we have changed mode

      treeStore.GUIEmpty();
      UpdateTreeStore();
      LoadData(faceDb.Keys().First());
      return true;

    }

    //: Attempt to perform an automatic merging of the sightings
    bool ViewPageBodyC::AutoMergeSighting()
    {
      rInfo("Automerging the sightings!");
      if (m_sightingSet.IsEmpty()) {
        AlertBox("No sighting set specified.  Unable to switch to Sighting Mode", &Manager.GetRootWindow());
        return false;
      }

      if (!m_sightingMode.GUIIsActive()) {
        AlertBox("Can only perform this operation in sighting mode!", &Manager.GetRootWindow());
        return false;
      }

      return true;
    }

    //: Create a new sighting out of the selected faces
    bool ViewPageBodyC::CreateSighting()
    {
      RavlInfo("Create a new sighting!");

      // Next we need to add faces as a new sighting
      StringC unknownId = "0:0";
      SightingC newSighting(unknownId);
      TreeModelIterC newRow;
      TreeModelIterC newerRow;
      treeStore.AppendRow(newRow);
      bool first = true;
      for (HashIterC<StringC, TreeModelIterC> it(m_selected); it; it++) {

        // check it exists
        if (!faceDb.IsElm(it.Key())) {
          rWarning("Face ID not in database.");
          continue;
        }

        rInfo("Adding face id '%s'", it.Key().data());
        newSighting.AddFaceId(it.Key());
        faceDb[it.Key()].ActualId() = unknownId;

        if (first) {
          treeStore.GUISetValue(newRow, 0, unknownId);
          treeStore.GUISetValue(newRow, 1, it.Key());
          treeStore.GUISetValue(newRow, 2, false);
          first = false;

        } else {

          treeStore.AppendRow(newerRow, newRow);
          treeStore.GUISetValue(newerRow, 0, unknownId);
          treeStore.GUISetValue(newerRow, 1, it.Key());
          treeStore.GUISetValue(newerRow, 2, false);
        }

      }

      // First lets delete them from the current sighting set
      if (!m_sightingSet.DeleteFaceIds(newSighting.FaceIds())) {
        rWarning("Failed to delete all the face ids.  Going to continue anyway.");
      }

      if (!m_sightingSet.Append(newSighting)) {
        rWarning("Failed to append new sighting");
      }

      // Deleted the selected entries from the tree view
      // However we do not want to delete the entries from the database
      DeleteSelected(false);

      LoadData(faceDb.Keys().First());
      return true;
    }

    // Show file dialog when Move button selected
    bool ViewPageBodyC::MoveButton()
    {
      m_fileSelectorMoveSelected.GUIShow();

      return true;
    }

    // Move selected faces into this xml-file
    bool ViewPageBodyC::MoveSelected(const StringC & filename)
    {

      rInfo("Moving faces to selected to file '%s'", filename.data());

      FilenameC fname(filename);
      FaceInfoDbC faceInfoDb;
      if (fname.Exists()) {
        if (!Load(filename, faceInfoDb)) {
          AlertBox("Failed to load database", &Manager.GetRootWindow());
          return false;
        }
        rInfo("Loaded '%s' with %d subjects and %d faces", filename.data(), faceInfoDb.NoClients(), faceInfoDb.NoFaces());
      } else {
        rInfo("Saving to new XML file'%s'", filename.data());
      }

      for (HashIterC<StringC, TreeModelIterC> it(m_selected); it; it++) {
        rInfo("Adding subject '%s' and face id '%s'", faceDb[it.Key()].ActualId().data(), it.Key().data());
        faceInfoDb.Insert(it.Key(), faceDb[it.Key()]);
      }

      rInfo("Saving to '%s' with %d subjects and %d faces", filename.data(), faceInfoDb.NoClients(), faceInfoDb.NoFaces());
      if (!FaceN::Save(filename, faceInfoDb)) {
        AlertBox(StringC("trouble saving database"));
        return false;
      }

      return DeleteSelected();
    }

  }
}
