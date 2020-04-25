
# Library: RavlAudioIO

# Requires: 
# Supports: 




# MustLink: src/RavlAudioSample.cc


set(DEPENDS_ON )
foreach(D IN LISTS DEPENDS_ON)
  if(NOT ${D}_FOUND)
    find_package(${D} REQUIRED)
  endif()
endforeach()

include(CMakePackageConfigHelpers)

include_directories(include)
include_directories(src)

set(HAVE_RavlAudioIO 1)

include_directories(../../Audio/RavlAudioUtil/include)
include_directories(../../OS/RavlOS/include)
include_directories(../../OS/RavlOSIO/include)
include_directories(../../Core/RavlCore/include)
include_directories(../../Core/RavlIO/include)
include_directories(../../Math/RavlMath/include)



if(DEFINED HAVE_RavlDevAudio)
include_directories(../../Contrib/RavlDevAudio/include)
endif()
if(DEFINED HAVE_RavlAudioFile)
include_directories(../../Contrib/RavlAudioFile/include)
endif()
if(DEFINED HAVE_RavlPortAudio)
include_directories(../../Contrib/RavlPortAudio/include)
endif()

add_library(RavlAudioIO_Body STATIC
 include/Ravl/Audio/SphereIO.hh include/Ravl/Audio/SphereFormat.hh include/Ravl/Audio/TranscriptionFiles.hh include/Ravl/Audio/TranscriptionStream.hh include/Ravl/Audio/PlaySound.hh
 src/SphereIO.cc src/SphereFormat.cc src/TranscriptionFiles.cc src/TranscriptionStream.cc src/PlaySound.cc src/AudioSample.cc
)
target_link_libraries(RavlAudioIO_Body PUBLIC RavlAudioUtil RavlOS RavlOSIO RavlCore RavlIO RavlMath )

add_library(RavlAudioIO OBJECT src/RavlAudioSample.cc )
target_link_libraries(RavlAudioIO PUBLIC RavlAudioIO_Body )


target_include_directories(
    RavlAudioIO PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
    )

# MustLink fixes:
install(EXPORT RavlAudioIO_BodyConfig DESTINATION lib/cmake/RavlAudioIO_Body/Target)
install(TARGETS RavlAudioIO_Body EXPORT RavlAudioIO_BodyConfig
   ARCHIVE  DESTINATION lib
   LIBRARY  DESTINATION lib
   RUNTIME  DESTINATION bin)  # This is for Windows
install(TARGETS RavlAudioIO_Body DESTINATION lib)

configure_package_config_file(cmake/RavlAudioIO_BodyConfig.cmake.in
  ${CMAKE_CURRENT_BINARY_DIR}/RavlAudioIO_BodyConfig.cmake
  INSTALL_DESTINATION lib/cmake/RavlAudioIO_Body
  PATH_VARS)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/RavlAudioIO_BodyConfig.cmake
        DESTINATION lib/cmake/RavlAudioIO_Body )


install(TARGETS RavlAudioIO EXPORT RavlAudioIOConfig
   ARCHIVE  DESTINATION lib
   LIBRARY  DESTINATION lib
   RUNTIME  DESTINATION bin)  # This is for Windows

install(EXPORT RavlAudioIOConfig DESTINATION lib/cmake/RavlAudioIO/Target)
install(TARGETS RavlAudioIO DESTINATION lib)

configure_package_config_file(cmake/RavlAudioIOConfig.cmake.in
  ${CMAKE_CURRENT_BINARY_DIR}/RavlAudioIOConfig.cmake
  INSTALL_DESTINATION lib/cmake/RavlAudioIO
  PATH_VARS)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/RavlAudioIOConfig.cmake
        DESTINATION lib/cmake/RavlAudioIO )

install(DIRECTORY include DESTINATION .)


# Mains
set(MAIN_LIBS  RavlAudioUtil RavlOS RavlOSIO RavlCore RavlIO RavlMath  )

add_executable(audioconv applications/audioconv.cc )
target_link_libraries(audioconv RavlAudioIO ${MAIN_LIBS} )
install(TARGETS audioconv DESTINATION bin)

# Examples
set(EXAMPLE_LIBS  RavlAudioUtil RavlOS RavlOSIO RavlCore RavlIO RavlMath  )

add_executable(exTranscriptionStream EXCLUDE_FROM_ALL examples/exTranscriptionStream.cc )
target_link_libraries(exTranscriptionStream RavlAudioIO ${EXAMPLE_LIBS} )
install(TARGETS exTranscriptionStream DESTINATION examples COMPONENT examples EXCLUDE_FROM_ALL)







