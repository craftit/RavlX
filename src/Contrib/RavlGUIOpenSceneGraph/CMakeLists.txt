
# Library: RavlGUIOpenSceneGraph

# Requires: OpenSceneGraph OpenSceneGraphGTK libGL GTKGLExt RLog libGTK2
# Supports: 



if(OPENSCENEGRAPH_FOUND)
if(OPENSCENEGRAPHGTK_FOUND)
if(OPENGL_FOUND)
if(GTKGLExt_FOUND)
if(librlog_FOUND)
if(GTK2_FOUND)

# MustLink: src/MustLinkGtkGlExtInit.cc


set(DEPENDS_ON )
foreach(D IN LISTS DEPENDS_ON)
  if(NOT ${D}_FOUND)
    find_package(${D} REQUIRED)
  endif()
endforeach()

include(CMakePackageConfigHelpers)

include_directories(include)
include_directories(src)

set(HAVE_RavlGUIOpenSceneGraph 1)

include_directories(../../Core/RavlCore/include)
include_directories(../../Math/RavlMath/include)
include_directories(../../Image/RavlImage/include)
include_directories(../../GUI/RavlGUI/include)
include_directories(../../3D/Ravl3D/include)
include_directories(../../OS/RavlXMLFactory/include)
include_directories(../../Contrib/RavlRLog/include)
include_directories(../../OS/RavlOS/include)
include_directories(../../Image/RavlImageIO/include)
include_directories(../../Image/RavlExtImgIO/include)




add_library(RavlGUIOpenSceneGraph_Body STATIC
 include/Ravl/OpenSceneGraph/Drawable.hh include/Ravl/OpenSceneGraph/Sphere.hh include/Ravl/OpenSceneGraph/Image.hh include/Ravl/OpenSceneGraph/ImageByteRGBA.hh include/Ravl/OpenSceneGraph/ImageByteRGB.hh include/Ravl/OpenSceneGraph/Text.hh include/Ravl/OpenSceneGraph/TriMesh.hh include/Ravl/OpenSceneGraph/TexTriMesh.hh include/Ravl/OpenSceneGraph/Node.hh include/Ravl/OpenSceneGraph/Geode.hh include/Ravl/OpenSceneGraph/Group.hh include/Ravl/OpenSceneGraph/Transform.hh include/Ravl/OpenSceneGraph/ModelFile.hh include/Ravl/OpenSceneGraph/HUD.hh include/Ravl/OpenSceneGraph/OpenSceneGraphWidget.hh include/Ravl/OpenSceneGraph/Box.hh include/Ravl/OpenSceneGraph/CanvasManipulator.hh include/Ravl/OpenSceneGraph/TypeConvert.hh include/Ravl/OpenSceneGraph/Layout.hh include/Ravl/OpenSceneGraph/LayoutStack.hh include/Ravl/OpenSceneGraph/LayoutGrid.hh include/Ravl/OpenSceneGraph/NodeVisitor.hh include/Ravl/OpenSceneGraph/Cylinder.hh include/Ravl/OpenSceneGraph/Cone.hh include/Ravl/OpenSceneGraph/Capsule.hh include/Ravl/OpenSceneGraph/InfinitePlane.hh
 src/Drawable.cc src/Sphere.cc src/Image.cc src/ImageByteRGBA.cc src/ImageByteRGB.cc src/Text.cc src/TriMesh.cc src/TexTriMesh.cc src/Node.cc src/Geode.cc src/Group.cc src/Transform.cc src/ModelFile.cc src/HUD.cc src/OpenSceneGraphWidget.cc src/Box.cc src/CanvasManipulator.cc src/TypeConvert.cc src/Layout.cc src/LayoutStack.cc src/LayoutGrid.cc src/NodeVisitor.cc src/Cylinder.cc src/Cone.cc src/Capsule.cc src/InfinitePlane.cc
)
target_link_libraries(RavlGUIOpenSceneGraph_Body PUBLIC RavlCore RavlMath RavlImage RavlGUI Ravl3D ${OPENGL_LIBRARIES} ${GTK2_LIBRARIES} ${OPENSCENEGRAPHGTK_LIBRARIES} RavlXMLFactory RavlRLog ${librlog_LIBRARIES} ${OPENSCENEGRAPH_LIBRARIES} )

add_library(RavlGUIOpenSceneGraph OBJECT src/MustLinkGtkGlExtInit.cc )
target_link_libraries(RavlGUIOpenSceneGraph PUBLIC RavlGUIOpenSceneGraph_Body )


target_include_directories(
    RavlGUIOpenSceneGraph PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
    )

# MustLink fixes:
install(EXPORT RavlGUIOpenSceneGraph_BodyConfig DESTINATION lib/cmake/RavlGUIOpenSceneGraph_Body/Target)
install(TARGETS RavlGUIOpenSceneGraph_Body EXPORT RavlGUIOpenSceneGraph_BodyConfig
   ARCHIVE  DESTINATION lib
   LIBRARY  DESTINATION lib
   RUNTIME  DESTINATION bin)  # This is for Windows
install(TARGETS RavlGUIOpenSceneGraph_Body DESTINATION lib)

configure_package_config_file(cmake/RavlGUIOpenSceneGraph_BodyConfig.cmake.in
  ${CMAKE_CURRENT_BINARY_DIR}/RavlGUIOpenSceneGraph_BodyConfig.cmake
  INSTALL_DESTINATION lib/cmake/RavlGUIOpenSceneGraph_Body
  PATH_VARS)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/RavlGUIOpenSceneGraph_BodyConfig.cmake
        DESTINATION lib/cmake/RavlGUIOpenSceneGraph_Body )


install(TARGETS RavlGUIOpenSceneGraph EXPORT RavlGUIOpenSceneGraphConfig
   ARCHIVE  DESTINATION lib
   LIBRARY  DESTINATION lib
   RUNTIME  DESTINATION bin)  # This is for Windows

install(EXPORT RavlGUIOpenSceneGraphConfig DESTINATION lib/cmake/RavlGUIOpenSceneGraph/Target)
install(TARGETS RavlGUIOpenSceneGraph DESTINATION lib)

configure_package_config_file(cmake/RavlGUIOpenSceneGraphConfig.cmake.in
  ${CMAKE_CURRENT_BINARY_DIR}/RavlGUIOpenSceneGraphConfig.cmake
  INSTALL_DESTINATION lib/cmake/RavlGUIOpenSceneGraph
  PATH_VARS)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/RavlGUIOpenSceneGraphConfig.cmake
        DESTINATION lib/cmake/RavlGUIOpenSceneGraph )

install(DIRECTORY include DESTINATION .)


# Mains
set(MAIN_LIBS  RavlCore RavlMath RavlImage RavlGUI Ravl3D ${OPENGL_LIBRARIES} ${GTK2_LIBRARIES} ${OPENSCENEGRAPHGTK_LIBRARIES} RavlXMLFactory RavlRLog ${librlog_LIBRARIES} ${OPENSCENEGRAPH_LIBRARIES} RavlOS RavlImageIO RavlExtImgIO  )

add_executable(exOpenSceneGraphWidgetXML applications/exOpenSceneGraphWidgetXML.cc )
target_link_libraries(exOpenSceneGraphWidgetXML RavlGUIOpenSceneGraph ${MAIN_LIBS} )
install(TARGETS exOpenSceneGraphWidgetXML DESTINATION bin)




# === Files for share/RAVL/OpenSceneGraph ===
set(AUXDIR share/RAVL/OpenSceneGraph)
set(AUXFILES 
  data/exOpenSceneGraph.xml )
file(COPY ${AUXFILES} DESTINATION ${BUILD_ROOT_AUXFILE}/${AUXDIR})
install(FILES ${AUXFILES} DESTINATION ${AUXDIR})



ENDIF()
ENDIF()
ENDIF()
ENDIF()
ENDIF()
ENDIF()


