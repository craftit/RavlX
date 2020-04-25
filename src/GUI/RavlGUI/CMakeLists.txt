
# Library: RavlGUI

# Requires: libGTK2
# Supports: 



if(GTK2_FOUND)

# MustLink: src/LinkRavlGUI.cc


set(DEPENDS_ON )
foreach(D IN LISTS DEPENDS_ON)
  if(NOT ${D}_FOUND)
    find_package(${D} REQUIRED)
  endif()
endforeach()

include(CMakePackageConfigHelpers)

include_directories(include)
include_directories(src)

set(HAVE_RavlGUI 1)

include_directories(../../Core/RavlCore/include)
include_directories(../../Core/RavlIO/include)
include_directories(../../OS/RavlOS/include)
include_directories(../../OS/RavlThreads/include)
include_directories(../../OS/RavlXMLFactory/include)
include_directories(../../Image/RavlImage/include)
include_directories(../../Math/RavlMath/include)
include_directories(../../Image/RavlImageIO/include)
include_directories(../../Image/RavlExtImgIO/include)




add_library(RavlGUI_Body STATIC
 include/Ravl/GUI/Manager.hh include/Ravl/GUI/Widget.hh include/Ravl/GUI/GTKTypes.hh include/Ravl/GUI/SignalInfo.hh include/Ravl/GUI/MouseEvent.hh include/Ravl/GUI/ScrollEvent.hh include/Ravl/GUI/Window.hh include/Ravl/GUI/Cursor.hh include/Ravl/GUI/ToolTips.hh include/Ravl/GUI/CWidget.hh include/Ravl/GUI/Button.hh include/Ravl/GUI/Pixmap.hh include/Ravl/GUI/OneChild.hh include/Ravl/GUI/PackInfo.hh include/Ravl/GUI/LBox.hh include/Ravl/GUI/Menu.hh include/Ravl/GUI/Label.hh include/Ravl/GUI/FileSelector.hh include/Ravl/GUI/FileChooser.hh include/Ravl/GUI/FileChooserDialog.hh include/Ravl/GUI/FileChooserButton.hh include/Ravl/GUI/TextEntry.hh include/Ravl/GUI/Slider.hh include/Ravl/GUI/ScrolledArea.hh include/Ravl/GUI/Fixed.hh include/Ravl/GUI/EventBox.hh include/Ravl/GUI/Frame.hh include/Ravl/GUI/ToggleButton.hh include/Ravl/GUI/StatusBar.hh include/Ravl/GUI/Combo.hh include/Ravl/GUI/List.hh include/Ravl/GUI/MenuCheck.hh include/Ravl/GUI/RawCanvas.hh include/Ravl/GUI/Table.hh include/Ravl/GUI/Ruler.hh include/Ravl/GUI/SpinButton.hh include/Ravl/GUI/Tree.hh include/Ravl/GUI/Separator.hh include/Ravl/GUI/Bitmap.hh include/Ravl/GUI/Notebook.hh include/Ravl/GUI/CList.hh include/Ravl/GUI/VLabel.hh include/Ravl/GUI/RadioButton.hh include/Ravl/GUI/ProgressBar.hh include/Ravl/GUI/TextBox.hh include/Ravl/GUI/TextView.hh include/Ravl/GUI/DragAndDrop.hh include/Ravl/GUI/Paned.hh include/Ravl/GUI/MessageBox.hh include/Ravl/GUI/WidgetStyle.hh include/Ravl/GUI/ReadBack.hh include/Ravl/GUI/WaitForExit.hh include/Ravl/GUI/TreeModel.hh include/Ravl/GUI/TreeStore.hh include/Ravl/GUI/TreeView.hh include/Ravl/GUI/ListStore.hh include/Ravl/GUI/Pixbuf.hh include/Ravl/GUI/AspectFrame.hh include/Ravl/GUI/ButtonBox.hh include/Ravl/GUI/ColourSelector.hh include/Ravl/GUI/DebugAssertDialog.hh include/Ravl/GUI/IconView.hh include/Ravl/GUI/ComboBox.hh include/Ravl/GUI/ComboBoxText.hh include/Ravl/GUI/ComboBoxEntry.hh include/Ravl/GUI/ComboBoxEntryText.hh include/Ravl/GUI/TriggerAction.hh
 src/Manager.cc src/Widget.cc src/MouseEvent.cc src/ScrollEvent.cc src/Window.cc src/Cursor.cc src/ToolTips.cc src/CWidget.cc src/Button.cc src/Pixmap.cc src/PackInfo.cc src/OneChild.cc src/LBox.cc src/Menu.cc src/Label.cc src/FileSelector.cc src/FileChooser.cc src/FileChooserDialog.cc src/FileChooserButton.cc src/TextEntry.cc src/Slider.cc src/ScrolledArea.cc src/Fixed.cc src/EventBox.cc src/Frame.cc src/ToggleButton.cc src/StatusBar.cc src/Combo.cc src/List.cc src/MenuCheck.cc src/RawCanvas.cc src/Table.cc src/Ruler.cc src/SpinButton.cc src/Tree.cc src/Separator.cc src/Bitmap.cc src/Notebook.cc src/CList.cc src/VLabel.cc src/RadioButton.cc src/ProgressBar.cc src/TextBox.cc src/TextView.cc src/DragAndDrop.cc src/Paned.cc src/MessageBox.cc src/WidgetStyle.cc src/ReadBack.cc src/WaitForExit.cc src/TreeModel.cc src/TreeStore.cc src/TreeView.cc src/ListStore.cc src/Pixbuf.cc src/AspectFrame.cc src/ButtonBox.cc src/ColourSelector.cc src/DebugAssertDialog.cc src/IconView.cc src/ComboBox.cc src/ComboBoxText.cc src/ComboBoxEntry.cc src/ComboBoxEntryText.cc src/TriggerAction.cc
)
target_link_libraries(RavlGUI_Body PUBLIC RavlCore RavlIO RavlOS ${GTK2_LIBRARIES} RavlThreads RavlXMLFactory RavlImage RavlMath )

add_library(RavlGUI OBJECT src/LinkRavlGUI.cc )
target_link_libraries(RavlGUI PUBLIC RavlGUI_Body )


target_include_directories(
    RavlGUI PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
    )

# MustLink fixes:
install(EXPORT RavlGUI_BodyConfig DESTINATION lib/cmake/RavlGUI_Body/Target)
install(TARGETS RavlGUI_Body EXPORT RavlGUI_BodyConfig
   ARCHIVE  DESTINATION lib
   LIBRARY  DESTINATION lib
   RUNTIME  DESTINATION bin)  # This is for Windows
install(TARGETS RavlGUI_Body DESTINATION lib)

configure_package_config_file(cmake/RavlGUI_BodyConfig.cmake.in
  ${CMAKE_CURRENT_BINARY_DIR}/RavlGUI_BodyConfig.cmake
  INSTALL_DESTINATION lib/cmake/RavlGUI_Body
  PATH_VARS)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/RavlGUI_BodyConfig.cmake
        DESTINATION lib/cmake/RavlGUI_Body )


install(TARGETS RavlGUI EXPORT RavlGUIConfig
   ARCHIVE  DESTINATION lib
   LIBRARY  DESTINATION lib
   RUNTIME  DESTINATION bin)  # This is for Windows

install(EXPORT RavlGUIConfig DESTINATION lib/cmake/RavlGUI/Target)
install(TARGETS RavlGUI DESTINATION lib)

configure_package_config_file(cmake/RavlGUIConfig.cmake.in
  ${CMAKE_CURRENT_BINARY_DIR}/RavlGUIConfig.cmake
  INSTALL_DESTINATION lib/cmake/RavlGUI
  PATH_VARS)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/RavlGUIConfig.cmake
        DESTINATION lib/cmake/RavlGUI )

install(DIRECTORY include DESTINATION .)


# Mains
set(MAIN_LIBS  RavlCore RavlIO RavlOS ${GTK2_LIBRARIES} RavlThreads RavlXMLFactory RavlImage RavlMath RavlImageIO RavlExtImgIO  )

add_executable(rdialog ${EXCLUDE_APPLICATIONS} applications/rdialog.cc )
target_link_libraries(rdialog RavlGUI ${MAIN_LIBS} )
install(TARGETS rdialog DESTINATION bin ${EXCLUDE_APPLICATIONS})

# Examples
set(EXAMPLE_LIBS  RavlCore RavlIO RavlOS ${GTK2_LIBRARIES} RavlThreads RavlXMLFactory RavlImage RavlMath RavlImageIO RavlExtImgIO  )

add_executable(exButton ${EXCLUDE_EXAMPLES} examples/exButton.cc )
target_link_libraries(exButton RavlGUI ${EXAMPLE_LIBS} )
install(TARGETS exButton DESTINATION examples COMPONENT examples ${EXCLUDE_EXAMPLES})

add_executable(exMenu ${EXCLUDE_EXAMPLES} examples/exMenu.cc )
target_link_libraries(exMenu RavlGUI ${EXAMPLE_LIBS} )
install(TARGETS exMenu DESTINATION examples COMPONENT examples ${EXCLUDE_EXAMPLES})

add_executable(exCList ${EXCLUDE_EXAMPLES} examples/exCList.cc )
target_link_libraries(exCList RavlGUI ${EXAMPLE_LIBS} )
install(TARGETS exCList DESTINATION examples COMPONENT examples ${EXCLUDE_EXAMPLES})

add_executable(exList ${EXCLUDE_EXAMPLES} examples/exList.cc )
target_link_libraries(exList RavlGUI ${EXAMPLE_LIBS} )
install(TARGETS exList DESTINATION examples COMPONENT examples ${EXCLUDE_EXAMPLES})

add_executable(exSlider ${EXCLUDE_EXAMPLES} examples/exSlider.cc )
target_link_libraries(exSlider RavlGUI ${EXAMPLE_LIBS} )
install(TARGETS exSlider DESTINATION examples COMPONENT examples ${EXCLUDE_EXAMPLES})

add_executable(exRadioButton ${EXCLUDE_EXAMPLES} examples/exRadioButton.cc )
target_link_libraries(exRadioButton RavlGUI ${EXAMPLE_LIBS} )
install(TARGETS exRadioButton DESTINATION examples COMPONENT examples ${EXCLUDE_EXAMPLES})

add_executable(exRuler ${EXCLUDE_EXAMPLES} examples/exRuler.cc )
target_link_libraries(exRuler RavlGUI ${EXAMPLE_LIBS} )
install(TARGETS exRuler DESTINATION examples COMPONENT examples ${EXCLUDE_EXAMPLES})

add_executable(exWheel ${EXCLUDE_EXAMPLES} examples/exWheel.cc )
target_link_libraries(exWheel RavlGUI ${EXAMPLE_LIBS} )
install(TARGETS exWheel DESTINATION examples COMPONENT examples ${EXCLUDE_EXAMPLES})

add_executable(exNotebook ${EXCLUDE_EXAMPLES} examples/exNotebook.cc )
target_link_libraries(exNotebook RavlGUI ${EXAMPLE_LIBS} )
install(TARGETS exNotebook DESTINATION examples COMPONENT examples ${EXCLUDE_EXAMPLES})

add_executable(exCursor ${EXCLUDE_EXAMPLES} examples/exCursor.cc )
target_link_libraries(exCursor RavlGUI ${EXAMPLE_LIBS} )
install(TARGETS exCursor DESTINATION examples COMPONENT examples ${EXCLUDE_EXAMPLES})

add_executable(exFixed ${EXCLUDE_EXAMPLES} examples/exFixed.cc )
target_link_libraries(exFixed RavlGUI ${EXAMPLE_LIBS} )
install(TARGETS exFixed DESTINATION examples COMPONENT examples ${EXCLUDE_EXAMPLES})

add_executable(exTree ${EXCLUDE_EXAMPLES} examples/exTree.cc )
target_link_libraries(exTree RavlGUI ${EXAMPLE_LIBS} )
install(TARGETS exTree DESTINATION examples COMPONENT examples ${EXCLUDE_EXAMPLES})

add_executable(exTextBox ${EXCLUDE_EXAMPLES} examples/exTextBox.cc )
target_link_libraries(exTextBox RavlGUI ${EXAMPLE_LIBS} )
install(TARGETS exTextBox DESTINATION examples COMPONENT examples ${EXCLUDE_EXAMPLES})

add_executable(exTextView ${EXCLUDE_EXAMPLES} examples/exTextView.cc )
target_link_libraries(exTextView RavlGUI ${EXAMPLE_LIBS} )
install(TARGETS exTextView DESTINATION examples COMPONENT examples ${EXCLUDE_EXAMPLES})

add_executable(exMessageBox ${EXCLUDE_EXAMPLES} examples/exMessageBox.cc )
target_link_libraries(exMessageBox RavlGUI ${EXAMPLE_LIBS} )
install(TARGETS exMessageBox DESTINATION examples COMPONENT examples ${EXCLUDE_EXAMPLES})

add_executable(exStyle ${EXCLUDE_EXAMPLES} examples/exStyle.cc )
target_link_libraries(exStyle RavlGUI ${EXAMPLE_LIBS} )
install(TARGETS exStyle DESTINATION examples COMPONENT examples ${EXCLUDE_EXAMPLES})

add_executable(exLBox ${EXCLUDE_EXAMPLES} examples/exLBox.cc )
target_link_libraries(exLBox RavlGUI ${EXAMPLE_LIBS} )
install(TARGETS exLBox DESTINATION examples COMPONENT examples ${EXCLUDE_EXAMPLES})

add_executable(exFrame ${EXCLUDE_EXAMPLES} examples/exFrame.cc )
target_link_libraries(exFrame RavlGUI ${EXAMPLE_LIBS} )
install(TARGETS exFrame DESTINATION examples COMPONENT examples ${EXCLUDE_EXAMPLES})

add_executable(exScrolledArea ${EXCLUDE_EXAMPLES} examples/exScrolledArea.cc )
target_link_libraries(exScrolledArea RavlGUI ${EXAMPLE_LIBS} )
install(TARGETS exScrolledArea DESTINATION examples COMPONENT examples ${EXCLUDE_EXAMPLES})

add_executable(exLabel ${EXCLUDE_EXAMPLES} examples/exLabel.cc )
target_link_libraries(exLabel RavlGUI ${EXAMPLE_LIBS} )
install(TARGETS exLabel DESTINATION examples COMPONENT examples ${EXCLUDE_EXAMPLES})

add_executable(exTreeStore ${EXCLUDE_EXAMPLES} examples/exTreeStore.cc )
target_link_libraries(exTreeStore RavlGUI ${EXAMPLE_LIBS} )
install(TARGETS exTreeStore DESTINATION examples COMPONENT examples ${EXCLUDE_EXAMPLES})

add_executable(exFileSelector ${EXCLUDE_EXAMPLES} examples/exFileSelector.cc )
target_link_libraries(exFileSelector RavlGUI ${EXAMPLE_LIBS} )
install(TARGETS exFileSelector DESTINATION examples COMPONENT examples ${EXCLUDE_EXAMPLES})

add_executable(exFileChooser ${EXCLUDE_EXAMPLES} examples/exFileChooser.cc )
target_link_libraries(exFileChooser RavlGUI ${EXAMPLE_LIBS} )
install(TARGETS exFileChooser DESTINATION examples COMPONENT examples ${EXCLUDE_EXAMPLES})

add_executable(exCombo ${EXCLUDE_EXAMPLES} examples/exCombo.cc )
target_link_libraries(exCombo RavlGUI ${EXAMPLE_LIBS} )
install(TARGETS exCombo DESTINATION examples COMPONENT examples ${EXCLUDE_EXAMPLES})

add_executable(exGUIThread ${EXCLUDE_EXAMPLES} examples/exGUIThread.cc )
target_link_libraries(exGUIThread RavlGUI ${EXAMPLE_LIBS} )
install(TARGETS exGUIThread DESTINATION examples COMPONENT examples ${EXCLUDE_EXAMPLES})

add_executable(exKeyboard ${EXCLUDE_EXAMPLES} examples/exKeyboard.cc )
target_link_libraries(exKeyboard RavlGUI ${EXAMPLE_LIBS} )
install(TARGETS exKeyboard DESTINATION examples COMPONENT examples ${EXCLUDE_EXAMPLES})

add_executable(exPixmap ${EXCLUDE_EXAMPLES} examples/exPixmap.cc )
target_link_libraries(exPixmap RavlGUI ${EXAMPLE_LIBS} )
install(TARGETS exPixmap DESTINATION examples COMPONENT examples ${EXCLUDE_EXAMPLES})

add_executable(exPixbuf ${EXCLUDE_EXAMPLES} examples/exPixbuf.cc )
target_link_libraries(exPixbuf RavlGUI ${EXAMPLE_LIBS} )
install(TARGETS exPixbuf DESTINATION examples COMPONENT examples ${EXCLUDE_EXAMPLES})

add_executable(exComboBox ${EXCLUDE_EXAMPLES} examples/exComboBox.cc )
target_link_libraries(exComboBox RavlGUI ${EXAMPLE_LIBS} )
install(TARGETS exComboBox DESTINATION examples COMPONENT examples ${EXCLUDE_EXAMPLES})

add_executable(exComboBoxEntry ${EXCLUDE_EXAMPLES} examples/exComboBoxEntry.cc )
target_link_libraries(exComboBoxEntry RavlGUI ${EXAMPLE_LIBS} )
install(TARGETS exComboBoxEntry DESTINATION examples COMPONENT examples ${EXCLUDE_EXAMPLES})

add_executable(exComboBoxText ${EXCLUDE_EXAMPLES} examples/exComboBoxText.cc )
target_link_libraries(exComboBoxText RavlGUI ${EXAMPLE_LIBS} )
install(TARGETS exComboBoxText DESTINATION examples COMPONENT examples ${EXCLUDE_EXAMPLES})

add_executable(exComboBoxEntryText ${EXCLUDE_EXAMPLES} examples/exComboBoxEntryText.cc )
target_link_libraries(exComboBoxEntryText RavlGUI ${EXAMPLE_LIBS} )
install(TARGETS exComboBoxEntryText DESTINATION examples COMPONENT examples ${EXCLUDE_EXAMPLES})





ENDIF()


