
# Library: RavlDPDisplay

# Requires: libGTK2
# Supports: 



if(GTK2_FOUND)

# MustLink: src/RavlGUIDisplay.cc


set(DEPENDS_ON )
foreach(D IN LISTS DEPENDS_ON)
  if(NOT ${D}_FOUND)
    find_package(${D} REQUIRED)
  endif()
endforeach()

include(CMakePackageConfigHelpers)

include_directories(include)
include_directories(src)

set(HAVE_RavlDPDisplay 1)

include_directories(../../Core/RavlIO/include)
include_directories(../../GUI/RavlGUI/include)
include_directories(../../Image/RavlImageIO/include)
include_directories(../../GUI/RavlGUI2D/include)
include_directories(../../Image/RavlImage/include)
include_directories(../../Core/RavlCore/include)
include_directories(../../OS/RavlThreads/include)
include_directories(../../Math/RavlMath/include)
include_directories(../../Image/RavlImageProc/include)
include_directories(../../OS/RavlOS/include)




add_library(RavlDPDisplay_Body STATIC
 include/Ravl/GUI/DPWindow.hh include/Ravl/GUI/DPDisplayObj.hh include/Ravl/GUI/DPDisplayImage.hh include/Ravl/GUI/DPWindowOPort.hh include/Ravl/GUI/DPWindowFormat.hh include/Ravl/GUI/DPDisplayView.hh include/Ravl/GUI/DPDisplayPoints.hh include/Ravl/GUI/DPDisplayPolygon.hh include/Ravl/GUI/DPDisplay.hh
 src/DPWindow.cc src/DPDisplayObj.cc src/DPDisplayImage.cc src/DPWindowOPort.cc src/DPWindowFormat.cc src/DPDisplayView.cc src/DPDisplayPoints.cc src/DPDisplayPolygon.cc
)
target_link_libraries(RavlDPDisplay_Body PUBLIC RavlIO RavlGUI RavlImageIO RavlGUI2D RavlImage ${GTK2_LIBRARIES} RavlCore RavlThreads RavlMath RavlImageProc RavlOS )

add_library(RavlDPDisplay OBJECT src/RavlGUIDisplay.cc )
target_link_libraries(RavlDPDisplay PUBLIC RavlDPDisplay_Body )


target_include_directories(
    RavlDPDisplay PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
    )

# MustLink fixes:
install(EXPORT RavlDPDisplay_BodyConfig DESTINATION lib/cmake/RavlDPDisplay_Body/Target)
install(TARGETS RavlDPDisplay_Body EXPORT RavlDPDisplay_BodyConfig
   ARCHIVE  DESTINATION lib
   LIBRARY  DESTINATION lib
   RUNTIME  DESTINATION bin)  # This is for Windows
install(TARGETS RavlDPDisplay_Body DESTINATION lib)

configure_package_config_file(cmake/RavlDPDisplay_BodyConfig.cmake.in
  ${CMAKE_CURRENT_BINARY_DIR}/RavlDPDisplay_BodyConfig.cmake
  INSTALL_DESTINATION lib/cmake/RavlDPDisplay_Body
  PATH_VARS)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/RavlDPDisplay_BodyConfig.cmake
        DESTINATION lib/cmake/RavlDPDisplay_Body )


install(TARGETS RavlDPDisplay EXPORT RavlDPDisplayConfig
   ARCHIVE  DESTINATION lib
   LIBRARY  DESTINATION lib
   RUNTIME  DESTINATION bin)  # This is for Windows

install(EXPORT RavlDPDisplayConfig DESTINATION lib/cmake/RavlDPDisplay/Target)
install(TARGETS RavlDPDisplay DESTINATION lib)

configure_package_config_file(cmake/RavlDPDisplayConfig.cmake.in
  ${CMAKE_CURRENT_BINARY_DIR}/RavlDPDisplayConfig.cmake
  INSTALL_DESTINATION lib/cmake/RavlDPDisplay
  PATH_VARS)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/RavlDPDisplayConfig.cmake
        DESTINATION lib/cmake/RavlDPDisplay )

install(DIRECTORY include DESTINATION .)



# Examples
set(EXAMPLE_LIBS  RavlIO RavlGUI RavlImageIO RavlGUI2D RavlImage ${GTK2_LIBRARIES} RavlCore RavlThreads RavlMath RavlImageProc RavlOS  )

add_executable(exDPWindow ${EXCLUDE_EXAMPLES} examples/exDPWindow.cc )
target_link_libraries(exDPWindow RavlDPDisplay ${EXAMPLE_LIBS} )
install(TARGETS exDPWindow DESTINATION examples COMPONENT examples ${EXCLUDE_EXAMPLES})





ENDIF()


