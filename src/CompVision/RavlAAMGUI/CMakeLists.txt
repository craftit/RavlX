
# Library: RavlAAMGUI

# Requires: libGTK2
# Supports: !arm

if((NOT ${CMAKE_SYSTEM_PROCESSOR} MATCHES "arm") OR FALSE)

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

set(HAVE_RavlAAMGUI 1)

include_directories(../../CompVision/RavlAAM/include)
include_directories(../../GUI/RavlGUI2D/include)
include_directories(../../Core/RavlCore/include)
include_directories(../../Image/RavlImage/include)
include_directories(../../Image/RavlImageProc/include)
include_directories(../../Math/RavlMath/include)
include_directories(../../OS/RavlOS/include)
include_directories(../../PatternRec/RavlPatternRec/include)
include_directories(../../Math/RavlOptimise/include)
include_directories(../../Core/RavlIO/include)
include_directories(../../PatternRec/Optimisation/include)
include_directories(../../OS/RavlXMLFactory/include)
include_directories(../../OS/RavlThreads/include)
include_directories(../../GUI/RavlGUI/include)
include_directories(../../PatternRec/RavlDataSet/include)
include_directories(../../Image/RavlImageIO/include)
include_directories(../../Image/RavlExtImgIO/include)
include_directories(../../Math/RavlMathIO/include)



if(DEFINED HAVE_RavlDPDisplay)
include_directories(../../GUI/RavlDPDisplay/include)
endif()
if(DEFINED HAVE_RavlImgIOV4L2)
include_directories(../../Contrib/RavlImgIOV4L2/include)
endif()

add_library(RavlAAMGUI STATIC include/Ravl/Image/AAMViewLib.hh src/AAMViewLib.cc)
target_link_libraries(RavlAAMGUI PUBLIC RavlAAM RavlGUI2D RavlCore RavlImage RavlImageProc RavlMath RavlOS RavlPatternRec RavlOptimise RavlIO Optimisation RavlXMLFactory RavlThreads RavlGUI RavlDataSet )


target_include_directories(
    RavlAAMGUI PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
    )



install(TARGETS RavlAAMGUI EXPORT RavlAAMGUIConfig
   ARCHIVE  DESTINATION lib
   LIBRARY  DESTINATION lib
   RUNTIME  DESTINATION bin)  # This is for Windows

install(EXPORT RavlAAMGUIConfig DESTINATION lib/cmake/RavlAAMGUI/Target)
install(TARGETS RavlAAMGUI DESTINATION lib)

configure_package_config_file(cmake/RavlAAMGUIConfig.cmake.in
  ${CMAKE_CURRENT_BINARY_DIR}/RavlAAMGUIConfig.cmake
  INSTALL_DESTINATION lib/cmake/RavlAAMGUI
  PATH_VARS)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/RavlAAMGUIConfig.cmake
        DESTINATION lib/cmake/RavlAAMGUI )

install(DIRECTORY include DESTINATION .)


# Mains
set(MAIN_LIBS  RavlAAM RavlGUI2D RavlCore RavlImage RavlImageProc RavlMath RavlOS RavlPatternRec RavlOptimise RavlIO Optimisation RavlXMLFactory RavlThreads RavlGUI RavlDataSet RavlImageIO RavlExtImgIO RavlMathIO  )

add_executable(aamViewShapeModel applications/aamViewShapeModel.cc )
target_link_libraries(aamViewShapeModel RavlAAMGUI ${MAIN_LIBS} )
install(TARGETS aamViewShapeModel DESTINATION bin)






ENDIF()

ENDIF()
