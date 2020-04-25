
# Library: RavlPulseAudio

# Requires: PulseAudioSimple
# Supports: 



if(DEFINED libpulse-simple_FOUND)

# MustLink: src/RavlPulseAudio.cc


set(DEPENDS_ON )
foreach(D IN LISTS DEPENDS_ON)
  if(NOT ${D}_FOUND)
    find_package(${D} REQUIRED)
  endif()
endforeach()

include(CMakePackageConfigHelpers)

include_directories(include)
include_directories(src)

set(HAVE_RavlPulseAudio 1)

include_directories(../../Core/RavlIO/include)
include_directories(../../Audio/RavlAudioUtil/include)
include_directories(../../Audio/RavlAudioIO/include)
include_directories(../../OS/RavlThreads/include)
include_directories(../../Core/RavlCore/include)
include_directories(../../OS/RavlOSIO/include)
include_directories(../../OS/RavlOS/include)



if(DEFINED HAVE_RavlAudioFile)
include_directories(../../Contrib/RavlAudioFile/include)
endif()

add_library(RavlPulseAudio_Body STATIC
 include/Ravl/Audio/PulseAudioIO.hh include/Ravl/Audio/PulseAudioFormat.hh
 src/PulseAudioIO.cc src/PulseAudioFormat.cc
)
target_link_libraries(RavlPulseAudio_Body PUBLIC RavlIO RavlAudioUtil RavlAudioIO ${libpulse-simple_LIBRARIES} RavlThreads RavlCore RavlOSIO )

add_library(RavlPulseAudio OBJECT src/RavlPulseAudio.cc )
target_link_libraries(RavlPulseAudio PUBLIC RavlPulseAudio_Body )


target_include_directories(
    RavlPulseAudio PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
    )

# MustLink fixes:
install(EXPORT RavlPulseAudio_BodyConfig DESTINATION lib/cmake/RavlPulseAudio_Body/Target)
install(TARGETS RavlPulseAudio_Body EXPORT RavlPulseAudio_BodyConfig
   ARCHIVE  DESTINATION lib
   LIBRARY  DESTINATION lib
   RUNTIME  DESTINATION bin)  # This is for Windows
install(TARGETS RavlPulseAudio_Body DESTINATION lib)

configure_package_config_file(cmake/RavlPulseAudio_BodyConfig.cmake.in
  ${CMAKE_CURRENT_BINARY_DIR}/RavlPulseAudio_BodyConfig.cmake
  INSTALL_DESTINATION lib/cmake/RavlPulseAudio_Body
  PATH_VARS)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/RavlPulseAudio_BodyConfig.cmake
        DESTINATION lib/cmake/RavlPulseAudio_Body )


install(TARGETS RavlPulseAudio EXPORT RavlPulseAudioConfig
   ARCHIVE  DESTINATION lib
   LIBRARY  DESTINATION lib
   RUNTIME  DESTINATION bin)  # This is for Windows

install(EXPORT RavlPulseAudioConfig DESTINATION lib/cmake/RavlPulseAudio/Target)
install(TARGETS RavlPulseAudio DESTINATION lib)

configure_package_config_file(cmake/RavlPulseAudioConfig.cmake.in
  ${CMAKE_CURRENT_BINARY_DIR}/RavlPulseAudioConfig.cmake
  INSTALL_DESTINATION lib/cmake/RavlPulseAudio
  PATH_VARS)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/RavlPulseAudioConfig.cmake
        DESTINATION lib/cmake/RavlPulseAudio )

install(DIRECTORY include DESTINATION .)



# Examples
set(EXAMPLE_LIBS  RavlIO RavlAudioUtil RavlAudioIO ${libpulse-simple_LIBRARIES} RavlThreads RavlCore RavlOSIO RavlOS  )

add_executable(exPulseAudio ${EXCLUDE_EXAMPLES} examples/exPulseAudio.cc )
target_link_libraries(exPulseAudio RavlPulseAudio ${EXAMPLE_LIBS} )
install(TARGETS exPulseAudio DESTINATION examples COMPONENT examples ${EXCLUDE_EXAMPLES})





ENDIF()


