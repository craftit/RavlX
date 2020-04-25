
# Library: RavlGUI2D

# Requires: libGTK2
# Supports: 



if(GTK2_FOUND)



set(DEPENDS_ON )
foreach(D IN LISTS DEPENDS_ON)
  if(NOT ${D}_FOUND)
    find_package(${D} REQUIRED)
  endif()
endforeach()

include(CMakePackageConfigHelpers)

include_directories(include)
include_directories(src)

set(HAVE_RavlGUI2D 1)

include_directories(../../GUI/RavlGUI/include)
include_directories(../../Image/RavlImage/include)
include_directories(../../Core/RavlIO/include)
include_directories(../../Math/RavlMath/include)
include_directories(../../Core/RavlCore/include)
include_directories(../../OS/RavlThreads/include)
include_directories(../../Image/RavlImageProc/include)
include_directories(../../OS/RavlOS/include)




add_library(RavlGUI2D STATIC include/Ravl/GUI/Canvas.hh include/Ravl/GUI/RawZoomCanvas.hh include/Ravl/GUI/GUIMarkupLayerEditor.hh include/Ravl/GUI/MarkupImageRGBA.hh include/Ravl/GUI/MarkupInfo.hh include/Ravl/GUI/MarkupLine2d.hh include/Ravl/GUI/MarkupPoint2d.hh include/Ravl/GUI/FrameMarkup.hh include/Ravl/GUI/GUIMarkupCanvas.hh include/Ravl/GUI/MarkupImageByte.hh include/Ravl/GUI/MarkupImageRGB.hh include/Ravl/GUI/MarkupLayerInfo.hh include/Ravl/GUI/MarkupNote.hh include/Ravl/GUI/MarkupPolygon2d.hh src/Canvas.cc src/RawCanvas2d.cc src/RawZoomCanvas.cc src/FrameMarkup.cc src/GUIMarkupLayerEditor.cc src/MarkupImageRGB.cc src/MarkupLine2d.cc src/MarkupPolygon2d.cc src/MarkupImageByte.cc src/MarkupInfo.cc src/MarkupNote.cc src/GUIMarkupCanvas.cc src/MarkupImageRGBA.cc src/MarkupLayerInfo.cc src/MarkupPoint2d.cc)
target_link_libraries(RavlGUI2D PUBLIC RavlGUI RavlImage RavlIO RavlMath ${GTK2_LIBRARIES} RavlCore RavlThreads RavlImageProc RavlOS )


target_include_directories(
    RavlGUI2D PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
    )



install(TARGETS RavlGUI2D EXPORT RavlGUI2DConfig
   ARCHIVE  DESTINATION lib
   LIBRARY  DESTINATION lib
   RUNTIME  DESTINATION bin)  # This is for Windows

install(EXPORT RavlGUI2DConfig DESTINATION lib/cmake/RavlGUI2D/Target)
install(TARGETS RavlGUI2D DESTINATION lib)

configure_package_config_file(cmake/RavlGUI2DConfig.cmake.in
  ${CMAKE_CURRENT_BINARY_DIR}/RavlGUI2DConfig.cmake
  INSTALL_DESTINATION lib/cmake/RavlGUI2D
  PATH_VARS)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/RavlGUI2DConfig.cmake
        DESTINATION lib/cmake/RavlGUI2D )

install(DIRECTORY include DESTINATION .)



# Examples
set(EXAMPLE_LIBS  RavlGUI RavlImage RavlIO RavlMath ${GTK2_LIBRARIES} RavlCore RavlThreads RavlImageProc RavlOS  )

add_executable(exCanvas ${EXCLUDE_EXAMPLES} examples/exCanvas.cc )
target_link_libraries(exCanvas RavlGUI2D ${EXAMPLE_LIBS} )
install(TARGETS exCanvas DESTINATION examples COMPONENT examples ${EXCLUDE_EXAMPLES})

add_executable(exRawZoomCanvas ${EXCLUDE_EXAMPLES} examples/exRawZoomCanvas.cc )
target_link_libraries(exRawZoomCanvas RavlGUI2D ${EXAMPLE_LIBS} )
install(TARGETS exRawZoomCanvas DESTINATION examples COMPONENT examples ${EXCLUDE_EXAMPLES})

add_executable(exGUIMarkupCanvas ${EXCLUDE_EXAMPLES} examples/exGUIMarkupCanvas.cc )
target_link_libraries(exGUIMarkupCanvas RavlGUI2D ${EXAMPLE_LIBS} )
install(TARGETS exGUIMarkupCanvas DESTINATION examples COMPONENT examples ${EXCLUDE_EXAMPLES})





ENDIF()


