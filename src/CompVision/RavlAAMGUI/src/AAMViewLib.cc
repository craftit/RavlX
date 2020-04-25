// This file is part of RAVL, Recognition And Vision Library
// Copyright (C) 2005, OmniPerception Ltd.
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//! lib=RavlAAMGUI

#include "Ravl/Image/AAMViewLib.hh"
#include "Ravl/GUI/Canvas.hh"
#include "Ravl/GUI/Slider.hh"
#include "Ravl/GUI/Window.hh"
#include "Ravl/GUI/SpinButton.hh"
#include "Ravl/GUI/LBox.hh"
#include "Ravl/GUI/Manager.hh"
#include "Ravl/Image/AAMShapeModel.hh"
#include "Ravl/Image/AAMAppearanceModel.hh"

namespace RavlImageN {

  using namespace RavlGUIN;

  static CanvasC g_canvas;
  static SliderC g_slider;

  static AAMShapeModelC g_sm; // Shape model.
  static AAMAppearanceModelC g_am; // appearance model.

  static VectorC g_params(3);
  static VectorC g_variance(3);
  static VectorC g_fixedMeans(3);
  static IntT g_currentParam = 0;

  RealT g_scale = 1;

  static void RenderModel() {
    bool x = false;
    g_canvas.GUIAutoRefresh(x);

    if(g_sm.IsValid()) {
      SArray1dC<Point2dC> points = g_sm.Synthesize(g_params);
      Index2dC off(2,2);
      IntT col = 0;
      g_canvas.GUIClear();
      for(SArray1dIterC<Point2dC> it(points);it;it++) {
        Index2dC mid(*it);
        Index2dC tr(mid - off);
        Index2dC bl(mid + off);

        IntT tr1 = tr[1].V(); IntT tr0 = tr[0].V();
        IntT bl1 = bl[1].V(); IntT bl0 = bl[0].V();
        g_canvas.GUIDrawLine(tr1, tr0, bl1, bl0, col);
        g_canvas.GUIDrawLine(tr1, tr0, bl1, bl0, col);
        g_canvas.GUIDrawLine(tr1, bl0, bl1, tr0, col);
        g_canvas.GUIDrawLine(bl1, tr0, tr1, bl0, col);
      }
    }
    if(g_am.IsValid()) {
      //cerr << "Synthesising appearance. ";
      AAMAppearanceC appear = g_am.Synthesize(g_params,g_scale);
      //cerr << "Image =  " << appear.Image().Frame() << "\n";
      Index2dC off(0,0);
      //cout << g_currentParam << endl;
      //Save ("/vol/vssp/vampiredb/people/mohammad/appmodel//all/xm_turntable/xm_turntable_noglasses/tmp/app_"+StringC(counter)+".pgm", appear.Image());
      //counter++;
      g_canvas.GUIDrawImage(appear.Image(),off);
    }

    g_canvas.GUIRefresh();
  }

  static bool ChangeParameter(RealT &parm) {
    g_currentParam = Round(parm);
    if((UIntT) g_currentParam >= g_params.Size())
      return true;
    if(g_currentParam < (IntT) g_fixedMeans.Size()) {
      g_slider.UpdateValue(((g_params[g_currentParam] - g_fixedMeans[g_currentParam]) * 20) / Sqrt(g_variance[g_currentParam]));
    } else {
      g_slider.UpdateValue((g_params[g_currentParam] * 20) / Sqrt(g_variance[g_currentParam]));
    }
    return true;
  }

  static bool ChangeValue(RealT &parm) {
    if((UIntT) g_currentParam >= g_params.Size())
      return true;
    RealT value;
    if(g_currentParam >= (IntT) g_fixedMeans.Size()) {
      value = (parm / 20) * Sqrt(g_variance[g_currentParam]);
    } else {
      value = g_fixedMeans[g_currentParam] + (parm / 20) * Sqrt(g_variance[g_currentParam]);
    }
    //cerr << "Setting " << g_currentParam << " to " << value << "\n";
    g_params[g_currentParam] = value;
    RenderModel();
    return true;
  }

  static void SetShapeModel(const AAMShapeModelC &shapeModel) {
    g_sm = shapeModel;
    if(g_sm.IsValid()) {
      g_params = VectorC(g_sm.Dimensions());
      g_params.Fill(0);
      for(UIntT i = 0;i < g_sm.FixedMean().Size();i++)
        g_params[i] = g_sm.FixedMean()[i];
      g_variance = g_sm.EigenValues();
      g_fixedMeans = g_sm.FixedMean();
      cerr << "Means=" << g_fixedMeans << "\n";
    }
  }

  static void SetAppearanceModel(const AAMAppearanceModelC &appearanceModel) {
    g_am = appearanceModel;
    if(g_am.IsValid()) {
      g_params = VectorC(g_am.Dimensions());
      g_params.Fill(0);
      for(UIntT i = 0;i < g_am.FixedMean().Size();i++)
        g_params[i] = g_am.FixedMean()[i];
      g_variance = g_am.EigenValues();
      g_fixedMeans = g_am.FixedMean();
    }
  }

  void AAMDisplay(const AAMShapeModelC &shapeModel, const AAMAppearanceModelC &appearanceModel, IntT varRange, RealT scale) {
    g_scale = scale;
    SetShapeModel(shapeModel);
    SetAppearanceModel(appearanceModel);
    WindowC win(100,100,"Shape Model Viewer");
    g_canvas = CanvasC(600,600);
    g_canvas.AutoRefresh(false);
    SpinButtonC spinButton(0,1,0.0,0.0,g_params.Size()-1,1.0);

    g_slider = SliderC(false,0,(RealT) -varRange,(RealT) varRange,0.1);
    Connect(spinButton.SigChanged(),ChangeParameter);
    Connect(g_slider.SigChanged(),ChangeValue);
    win.Add(VBox(g_canvas + HBox(spinButton + g_slider)));

    win.Show();
    RenderModel();
    Manager.Execute();

    Manager.Wait();
  }


}
