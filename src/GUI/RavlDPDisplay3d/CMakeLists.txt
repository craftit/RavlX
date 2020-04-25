
# Library: RavlDPDisplay3d

# Requires: libGL libGTK2
# Supports: 



if(OPENGL_FOUND)
if(GTK2_FOUND)

# MustLink: src/linkRavlDPDisplay3d.cc


set(DEPENDS_ON )
foreach(D IN LISTS DEPENDS_ON)
  if(NOT ${D}_FOUND)
    find_package(${D} REQUIRED)
  endif()
endforeach()

include(CMakePackageConfigHelpers)

include_directories(include)
include_directories(src)

set(HAVE_RavlDPDisplay3d 1)

include_directories(../../Core/RavlIO/include)
include_directories(../../GUI/RavlGUI/include)
include_directories(../../Image/RavlImageIO/include)
include_directories(../../GUI/RavlGUI3D/include)
include_directories(../../3D/Ravl3DIO/include)
include_directories(../../3D/Ravl3D/include)
include_directories(../../Image/RavlImage/include)
include_directories(../../Core/RavlCore/include)
include_directories(../../Math/RavlMath/include)
include_directories(../../OS/RavlThreads/include)
include_directories(../../OS/RavlOS/include)




add_library(RavlDPDisplay3d_Body STATIC
 include/Ravl/GUI/DPDisplayView3d.hh include/Ravl/GUI/DPWindow3d.hh include/Ravl/GUI/DPWindow3dOPort.hh include/Ravl/GUI/DPWindow3dFormat.hh
 src/DPDisplayView3d.cc src/DPWindow3d.cc src/DPWindow3dOPort.cc src/DPWindow3dFormat.cc src/DPDisplay3dConv.cc
)
target_link_libraries(RavlDPDisplay3d_Body PUBLIC RavlIO RavlGUI RavlImageIO RavlGUI3D Ravl3DIO Ravl3D RavlImage RavlCore RavlMath RavlThreads RavlOS )

add_library(RavlDPDisplay3d OBJECT src/linkRavlDPDisplay3d.cc )
target_link_libraries(RavlDPDisplay3d PUBLIC RavlDPDisplay3d_Body )


target_include_directories(
    RavlDPDisplay3d PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
    )

# MustLink fixes:
install(EXPORT RavlDPDisplay3d_BodyConfig DESTINATION lib/cmake/RavlDPDisplay3d_Body/Target)
install(TARGETS RavlDPDisplay3d_Body EXPORT RavlDPDisplay3d_BodyConfig
   ARCHIVE  DESTINATION lib
   LIBRARY  DESTINATION lib
   RUNTIME  DESTINATION bin)  # This is for Windows
install(TARGETS RavlDPDisplay3d_Body DESTINATION lib)

configure_package_config_file(cmake/RavlDPDisplay3d_BodyConfig.cmake.in
  ${CMAKE_CURRENT_BINARY_DIR}/RavlDPDisplay3d_BodyConfig.cmake
  INSTALL_DESTINATION lib/cmake/RavlDPDisplay3d_Body
  PATH_VARS)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/RavlDPDisplay3d_BodyConfig.cmake
        DESTINATION lib/cmake/RavlDPDisplay3d_Body )


install(TARGETS RavlDPDisplay3d EXPORT RavlDPDisplay3dConfig
   ARCHIVE  DESTINATION lib
   LIBRARY  DESTINATION lib
   RUNTIME  DESTINATION bin)  # This is for Windows

install(EXPORT RavlDPDisplay3dConfig DESTINATION lib/cmake/RavlDPDisplay3d/Target)
install(TARGETS RavlDPDisplay3d DESTINATION lib)

configure_package_config_file(cmake/RavlDPDisplay3dConfig.cmake.in
  ${CMAKE_CURRENT_BINARY_DIR}/RavlDPDisplay3dConfig.cmake
  INSTALL_DESTINATION lib/cmake/RavlDPDisplay3d
  PATH_VARS)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/RavlDPDisplay3dConfig.cmake
        DESTINATION lib/cmake/RavlDPDisplay3d )

install(DIRECTORY include DESTINATION .)



# Examples
set(EXAMPLE_LIBS  RavlIO RavlGUI RavlImageIO RavlGUI3D Ravl3DIO Ravl3D RavlImage RavlCore RavlMath RavlThreads RavlOS  )

add_executable(exDPWindow3d EXCLUDE_FROM_ALL examples/exDPWindow3d.cc )
target_link_libraries(exDPWindow3d RavlDPDisplay3d ${EXAMPLE_LIBS} )
install(TARGETS exDPWindow3d DESTINATION examples COMPONENT examples EXCLUDE_FROM_ALL)





ENDIF()
ENDIF()


