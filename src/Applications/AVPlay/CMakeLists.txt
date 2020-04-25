
# Library: AVPlay

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

set(HAVE_AVPlay 1)

include_directories(../../Core/RavlCore/include)
include_directories(../../OS/RavlOS/include)
include_directories(../../Core/RavlIO/include)
include_directories(../../GUI/RavlDPDisplay/include)
include_directories(../../Image/RavlImageIO/include)
include_directories(../../Image/RavlImage/include)
include_directories(../../Image/RavlVideoIO/include)
include_directories(../../Image/RavlImageProc/include)
include_directories(../../OS/RavlNet/include)
include_directories(../../GUI/RavlGUI/include)
include_directories(../../GUI/RavlGUI2D/include)
include_directories(../../OS/RavlThreads/include)
include_directories(../../Math/RavlMath/include)
include_directories(../../OS/RavlOSIO/include)
include_directories(../../OS/RavlDPMT/include)
include_directories(../../Image/RavlAV/include)
include_directories(../../Applications/RavlVPlay/include)
include_directories(../../Audio/RavlAudioIO/include)
include_directories(../../Audio/RavlAudioUtil/include)



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
if(DEFINED HAVE_RavlLibFFmpeg)
include_directories(../../Applications/RavlLibFFmpeg/include)
endif()
if(DEFINED HAVE_RavlImgIO1394dc)
include_directories(../../Contrib/RavlImgIO1394dc/include)
endif()
if(DEFINED HAVE_RavlAudioFile)
include_directories(../../Contrib/RavlAudioFile/include)
endif()
if(DEFINED HAVE_RavlDevAudio)
include_directories(../../Contrib/RavlDevAudio/include)
endif()


# Mains
set(MAIN_LIBS  RavlCore RavlOS RavlIO RavlDPDisplay RavlImageIO RavlImage RavlVideoIO RavlImageProc RavlNet RavlGUI RavlGUI2D RavlThreads RavlMath RavlOSIO RavlDPMT RavlAV RavlVPlay RavlAudioIO ${GTK2_LIBRARIES} RavlAudioUtil  )

add_executable(AVPlay ${EXCLUDE_APPLICATIONS} applications/AVPlay.cc )
target_link_libraries(AVPlay  ${MAIN_LIBS} )
install(TARGETS AVPlay DESTINATION bin ${EXCLUDE_APPLICATIONS})






ENDIF()

ENDIF()
