// This file is part of RAVL, Recognition And Vision Library
// Copyright (C) 2001-12, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//! lib=RavlGUI

namespace RavlGUIN {
  extern void LinkTriggerAction();
  extern void LinkFileChooserButton();
  extern void LinkFileChooserDialog();

  void LinkRavlGUI() {
    LinkTriggerAction();
    LinkFileChooserButton();
    LinkFileChooserDialog();
  }
}
