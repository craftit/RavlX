
# Library: mosaic

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

set(HAVE_mosaic 1)

include_directories(../../Image/RavlImageProc/include)
include_directories(../../Image/RavlVideoIO/include)
include_directories(../../GUI/RavlGUI2D/include)
include_directories(../../Image/RavlImageIO/include)
include_directories(../../Core/RavlCore/include)
include_directories(../../GUI/RavlGUI/include)
include_directories(../../OS/RavlThreads/include)
include_directories(../../Image/RavlImage/include)
include_directories(../../Math/RavlMath/include)
include_directories(../../Core/RavlIO/include)
include_directories(../../OS/RavlOSIO/include)
include_directories(../../OS/RavlOS/include)
include_directories(../../Math/RavlOptimise/include)



if(DEFINED HAVE_RavlImgIOV4L)
include_directories(../../Contrib/RavlImgIOV4L/include)
endif()
if(DEFINED HAVE_RavlDPDisplay)
include_directories(../../GUI/RavlDPDisplay/include)
endif()
if(DEFINED HAVE_RavlDV)
include_directories(../../Contrib/RavlDV/include)
endif()
if(DEFINED HAVE_RavlExtImgIO)
include_directories(../../Image/RavlExtImgIO/include)
endif()
if(DEFINED HAVE_RavlMPEG2)
include_directories(../../Contrib/RavlMPEG2/include)
endif()
if(DEFINED HAVE_CSPDriver)
include_directories(../../Contrib/CSPDriver/include)
endif()
if(DEFINED HAVE_RavlURLIO)
include_directories(../../Contrib/RavlURLIO/include)
endif()
if(DEFINED HAVE_RavlLibFFmpeg)
include_directories(../../Applications/RavlLibFFmpeg/include)
endif()


# Mains
set(MAIN_LIBS  RavlImageProc RavlVideoIO RavlGUI2D RavlImageIO RavlCore ${GTK2_LIBRARIES} RavlGUI RavlThreads RavlImage RavlMath RavlIO RavlOSIO RavlOS RavlOptimise  )

add_executable(mosaic ${EXCLUDE_APPLICATIONS} applications/mosaic.cc )
target_link_libraries(mosaic  ${MAIN_LIBS} )
install(TARGETS mosaic DESTINATION bin ${EXCLUDE_APPLICATIONS})

add_executable(xmosaic ${EXCLUDE_APPLICATIONS} applications/xmosaic.cc )
target_link_libraries(xmosaic  ${MAIN_LIBS} )
install(TARGETS xmosaic DESTINATION bin ${EXCLUDE_APPLICATIONS})






ENDIF()

ENDIF()
