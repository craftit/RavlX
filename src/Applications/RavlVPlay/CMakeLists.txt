
# Library: RavlVPlay

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

set(HAVE_RavlVPlay 1)

include_directories(../../Core/RavlCore/include)
include_directories(../../OS/RavlOS/include)
include_directories(../../Image/RavlImage/include)
include_directories(../../OS/RavlOSIO/include)
include_directories(../../GUI/RavlGUI/include)
include_directories(../../OS/RavlDPMT/include)
include_directories(../../GUI/RavlGUI2D/include)
include_directories(../../GUI/RavlGUIUtil/include)
include_directories(../../Core/RavlIO/include)
include_directories(../../Math/RavlMath/include)
include_directories(../../GUI/RavlDPDisplay/include)
include_directories(../../OS/RavlThreads/include)
include_directories(../../Image/RavlImageIO/include)
include_directories(../../Image/RavlVideoIO/include)
include_directories(../../Image/RavlImageProc/include)
include_directories(../../OS/RavlNet/include)
include_directories(../../OS/RavlService/include)



if(DEFINED HAVE_RavlImgIOV4L)
include_directories(../../Contrib/RavlImgIOV4L/include)
endif()
if(DEFINED HAVE_RavlExtImgIO)
include_directories(../../Image/RavlExtImgIO/include)
endif()
if(DEFINED HAVE_CSPDriver)
include_directories(../../Contrib/CSPDriver/include)
endif()
if(DEFINED HAVE_RavlURLIO)
include_directories(../../Contrib/RavlURLIO/include)
endif()
if(DEFINED HAVE_RavlLibMPEG2)
include_directories(../../Contrib/RavlLibMPEG2/include)
endif()
if(DEFINED HAVE_RavlDVDRead)
include_directories(../../Contrib/RavlDVDRead/include)
endif()
if(DEFINED HAVE_RavlAVIFile)
include_directories(../../Applications/RavlAVIFile/include)
endif()
if(DEFINED HAVE_RavlImgIOJasper)
include_directories(../../Contrib/RavlImgIOJasper/include)
endif()
if(DEFINED HAVE_RavlDV)
include_directories(../../Contrib/RavlDV/include)
endif()
if(DEFINED HAVE_RavlImgIOV4L2)
include_directories(../../Contrib/RavlImgIOV4L2/include)
endif()
if(DEFINED HAVE_RavlRawVidIO)
include_directories(../../Contrib/RavlRawVidIO/include)
endif()
if(DEFINED HAVE_RavlImgIO1394dc)
include_directories(../../Contrib/RavlImgIO1394dc/include)
endif()
if(DEFINED HAVE_RavlOpenCV)
include_directories(../../Contrib/RavlOpenCV/include)
endif()
if(DEFINED HAVE_RavlLibFFmpeg)
include_directories(../../Applications/RavlLibFFmpeg/include)
endif()

add_library(RavlVPlay STATIC include/Ravl/GUI/GUIPlayControl.hh src/GUIPlayControl.cc)
target_link_libraries(RavlVPlay PUBLIC RavlCore RavlOS RavlImage RavlOSIO RavlGUI RavlDPMT RavlGUI2D RavlGUIUtil RavlIO RavlMath )


target_include_directories(
    RavlVPlay PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
    )



install(TARGETS RavlVPlay EXPORT RavlVPlayConfig
   ARCHIVE  DESTINATION lib
   LIBRARY  DESTINATION lib
   RUNTIME  DESTINATION bin)  # This is for Windows

install(EXPORT RavlVPlayConfig DESTINATION lib/cmake/RavlVPlay/Target)
install(TARGETS RavlVPlay DESTINATION lib)

configure_package_config_file(cmake/RavlVPlayConfig.cmake.in
  ${CMAKE_CURRENT_BINARY_DIR}/RavlVPlayConfig.cmake
  INSTALL_DESTINATION lib/cmake/RavlVPlay
  PATH_VARS)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/RavlVPlayConfig.cmake
        DESTINATION lib/cmake/RavlVPlay )

install(DIRECTORY include DESTINATION .)


# Mains
set(MAIN_LIBS  RavlCore RavlOS RavlImage RavlOSIO RavlGUI RavlDPMT RavlGUI2D RavlGUIUtil RavlIO RavlMath ${GTK2_LIBRARIES} RavlDPDisplay RavlThreads RavlImageIO RavlVideoIO RavlImageProc RavlNet RavlService  )

add_executable(VPlay ${EXCLUDE_APPLICATIONS} applications/VPlay.cc )
target_link_libraries(VPlay RavlVPlay ${MAIN_LIBS} )
install(TARGETS VPlay DESTINATION bin ${EXCLUDE_APPLICATIONS})

add_executable(VCopy ${EXCLUDE_APPLICATIONS} applications/VCopy.cc )
target_link_libraries(VCopy RavlVPlay ${MAIN_LIBS} )
install(TARGETS VCopy DESTINATION bin ${EXCLUDE_APPLICATIONS})






ENDIF()

ENDIF()
