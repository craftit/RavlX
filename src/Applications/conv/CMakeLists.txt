
# Library: conv

# Requires: 
# Supports: 






set(DEPENDS_ON )
foreach(D IN LISTS DEPENDS_ON)
  if(NOT ${D}_FOUND)
    find_package(${D} REQUIRED)
  endif()
endforeach()

include(CMakePackageConfigHelpers)

include_directories(include)
include_directories(src)

set(HAVE_conv 1)

include_directories(../../Core/RavlCore/include)
include_directories(../../OS/RavlOSIO/include)
include_directories(../../Image/RavlImage/include)
include_directories(../../OS/RavlOS/include)
include_directories(../../Image/RavlImageIO/include)
include_directories(../../Image/RavlVideoIO/include)
include_directories(../../3D/Ravl3DIO/include)
include_directories(../../Math/RavlMathIO/include)
include_directories(../../PatternRec/RavlPatternRecIO/include)
include_directories(../../Core/RavlIO/include)



if(DEFINED HAVE_RavlDPDisplay)
include_directories(../../GUI/RavlDPDisplay/include)
endif()
if(DEFINED HAVE_RavlDPDisplay3d)
include_directories(../../GUI/RavlDPDisplay3d/include)
endif()
if(DEFINED HAVE_RavlExtImgIO)
include_directories(../../Image/RavlExtImgIO/include)
endif()
if(DEFINED HAVE_RavlDV)
include_directories(../../Contrib/RavlDV/include)
endif()
if(DEFINED HAVE_RavlImgIOV4L)
include_directories(../../Contrib/RavlImgIOV4L/include)
endif()
if(DEFINED HAVE_CSPDriver)
include_directories(../../Contrib/CSPDriver/include)
endif()
if(DEFINED HAVE_RavlURLIO)
include_directories(../../Contrib/RavlURLIO/include)
endif()
if(DEFINED HAVE_RavlDevAudio)
include_directories(../../Contrib/RavlDevAudio/include)
endif()
if(DEFINED HAVE_RavlAudioFile)
include_directories(../../Contrib/RavlAudioFile/include)
endif()
if(DEFINED HAVE_RavlDMeda)
include_directories(../../Applications/RavlDMeda/include)
endif()
if(DEFINED HAVE_RavlImgIOMeteor1)
include_directories(../../Contrib/RavlImgIOMeteor1/include)
endif()
if(DEFINED HAVE_RavlDPGraph)
include_directories(../../Contrib/RavlDPGraph/include)
endif()
if(DEFINED HAVE_RavlAudioIO)
include_directories(../../Audio/RavlAudioIO/include)
endif()
if(DEFINED HAVE_RavlImgIO1394dc)
include_directories(../../Contrib/RavlImgIO1394dc/include)
endif()
if(DEFINED HAVE_RavlLibMPEG2)
include_directories(../../Contrib/RavlLibMPEG2/include)
endif()
if(DEFINED HAVE_RavlImgIOJasper)
include_directories(../../Contrib/RavlImgIOJasper/include)
endif()
if(DEFINED HAVE_RavlLibFFmpeg)
include_directories(../../Applications/RavlLibFFmpeg/include)
endif()
if(DEFINED HAVE_RavlRawVidIO)
include_directories(../../Contrib/RavlRawVidIO/include)
endif()
if(DEFINED HAVE_RavlImgIOV4L2)
include_directories(../../Contrib/RavlImgIOV4L2/include)
endif()
if(DEFINED HAVE_RavlOpenCV)
include_directories(../../Contrib/RavlOpenCV/include)
endif()


# Mains
set(MAIN_LIBS  RavlCore RavlOSIO RavlImage RavlOS RavlImageIO RavlVideoIO Ravl3DIO RavlMathIO RavlPatternRecIO RavlIO  )

add_executable(conv ${EXCLUDE_APPLICATIONS} applications/conv.cc )
target_link_libraries(conv  ${MAIN_LIBS} )
install(TARGETS conv DESTINATION bin ${EXCLUDE_APPLICATIONS})


# Tests
set(TEST_LIBS  RavlCore RavlOSIO RavlImage RavlOS RavlImageIO RavlVideoIO Ravl3DIO RavlMathIO RavlPatternRecIO RavlIO  )

add_executable(testFileConverter ${EXCLUDE_UNITTESTS} tests/testFileConverter.cc )
target_link_libraries(testFileConverter  ${TEST_LIBS} )
add_test(testFileConverter testFileConverter)
install(TARGETS testFileConverter DESTINATION tests COMPONENT unittests ${EXCLUDE_UNITTESTS})


# === Files for share/RAVL/testData ===
set(AUXDIR share/RAVL/testData)
set(AUXFILES 
  data/in0.ppm 
  data/in1.ppm )
file(COPY ${AUXFILES} DESTINATION ${BUILD_ROOT_AUXFILE}/${AUXDIR})
install(FILES ${AUXFILES} DESTINATION ${AUXDIR})





