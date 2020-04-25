
# Library: RavlGUI3D

# Requires: libGL libGTK2 gtkglext
# Supports: 



if(OPENGL_FOUND)
if(GTK2_FOUND)
if(gtkglext_FOUND)



set(DEPENDS_ON )
foreach(D IN LISTS DEPENDS_ON)
  if(NOT ${D}_FOUND)
    find_package(${D} REQUIRED)
  endif()
endforeach()

include(CMakePackageConfigHelpers)

include_directories(include)
include_directories(src)

set(HAVE_RavlGUI3D 1)

include_directories(../../Core/RavlCore/include)
include_directories(../../Math/RavlMath/include)
include_directories(../../GUI/RavlGUI/include)
include_directories(../../3D/Ravl3D/include)
include_directories(../../Image/RavlImage/include)
include_directories(../../OS/RavlThreads/include)
include_directories(../../3D/RavlCameraCal/include)
include_directories(../../Image/RavlImageProc/include)
include_directories(../../OS/RavlOS/include)
include_directories(../../Core/RavlIO/include)




add_library(RavlGUI3D STATIC include/Ravl/GUI/DObject3D.hh include/Ravl/GUI/DLight3D.hh include/Ravl/GUI/Util.hh include/Ravl/GUI/Canvas3D.hh include/Ravl/GUI/DViewPoint3D.hh include/Ravl/GUI/DCube3D.hh include/Ravl/GUI/DTriMesh3D.hh include/Ravl/GUI/DPointSet3d.hh include/Ravl/GUI/DPinholeCamera03D.hh include/Ravl/GUI/DTexTriMesh3D.hh include/Ravl/GUI/View3D.hh include/Ravl/GUI/DSurfacePointVertexArray.hh include/Ravl/GUI/DSurfacePointVertexByteRGBArray.hh include/Ravl/GUI/GLContext.hh src/DObject3D.cc src/DLight3D.cc src/Util.cc src/Canvas3D.cc src/DViewPoint3D.cc src/DCube3D.cc src/DTriMesh3D.cc src/DPointSet3d.cc src/DPinholeCamera03D.cc src/DTexTriMesh3D.cc src/View3D.cc src/DSurfacePointVertexArray.cc src/DSurfacePointVertexByteRGBArray.cc src/GLContext.cc)
target_link_libraries(RavlGUI3D PUBLIC RavlCore RavlMath RavlGUI ${OPENGL_LIBRARIES} Ravl3D ${X11_LIBRARIES} ${GTK2_LIBRARIES} RavlImage RavlThreads RavlCameraCal RavlImageProc RavlOS RavlIO ${gtkglext_LIBRARIES} )


target_include_directories(
    RavlGUI3D PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
    )



install(TARGETS RavlGUI3D EXPORT RavlGUI3DConfig
   ARCHIVE  DESTINATION lib
   LIBRARY  DESTINATION lib
   RUNTIME  DESTINATION bin)  # This is for Windows

install(EXPORT RavlGUI3DConfig DESTINATION lib/cmake/RavlGUI3D/Target)
install(TARGETS RavlGUI3D DESTINATION lib)

configure_package_config_file(cmake/RavlGUI3DConfig.cmake.in
  ${CMAKE_CURRENT_BINARY_DIR}/RavlGUI3DConfig.cmake
  INSTALL_DESTINATION lib/cmake/RavlGUI3D
  PATH_VARS)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/RavlGUI3DConfig.cmake
        DESTINATION lib/cmake/RavlGUI3D )

install(DIRECTORY include DESTINATION .)



# Examples
set(EXAMPLE_LIBS  RavlCore RavlMath RavlGUI ${OPENGL_LIBRARIES} Ravl3D ${X11_LIBRARIES} ${GTK2_LIBRARIES} RavlImage RavlThreads RavlCameraCal RavlImageProc RavlOS RavlIO ${gtkglext_LIBRARIES}  )

add_executable(exCanvas3D ${EXCLUDE_EXAMPLES} examples/exCanvas3D.cc )
target_link_libraries(exCanvas3D RavlGUI3D ${EXAMPLE_LIBS} )
install(TARGETS exCanvas3D DESTINATION examples COMPONENT examples ${EXCLUDE_EXAMPLES})

add_executable(exView3D ${EXCLUDE_EXAMPLES} examples/exView3D.cc )
target_link_libraries(exView3D RavlGUI3D ${EXAMPLE_LIBS} )
install(TARGETS exView3D DESTINATION examples COMPONENT examples ${EXCLUDE_EXAMPLES})

add_executable(exDTexTriMesh3d ${EXCLUDE_EXAMPLES} examples/exDTexTriMesh3d.cc )
target_link_libraries(exDTexTriMesh3d RavlGUI3D ${EXAMPLE_LIBS} )
install(TARGETS exDTexTriMesh3d DESTINATION examples COMPONENT examples ${EXCLUDE_EXAMPLES})





ENDIF()
ENDIF()
ENDIF()


