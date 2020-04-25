
# Library: RavlPortAudio

# Requires: PortAudio
# Supports: 



if(portaudio_FOUND)

# MustLink: src/RavlPortAudio.cc


set(DEPENDS_ON )
foreach(D IN LISTS DEPENDS_ON)
  if(NOT ${D}_FOUND)
    find_package(${D} REQUIRED)
  endif()
endforeach()

include(CMakePackageConfigHelpers)

include_directories(include)
include_directories(src)

set(HAVE_RavlPortAudio 1)

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

add_library(RavlPortAudio_Body STATIC
 include/Ravl/Audio/PortAudioIO.hh include/Ravl/Audio/PortAudioFormat.hh
 src/PortAudioIO.cc src/PortAudioFormat.cc
)
target_link_libraries(RavlPortAudio_Body PUBLIC RavlIO RavlAudioUtil RavlAudioIO ${portaudio_LIBRARIES} RavlThreads RavlCore RavlOSIO RavlOS )

add_library(RavlPortAudio OBJECT src/RavlPortAudio.cc )
target_link_libraries(RavlPortAudio PUBLIC RavlPortAudio_Body )


target_include_directories(
    RavlPortAudio PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
    )

# MustLink fixes:
install(EXPORT RavlPortAudio_BodyConfig DESTINATION lib/cmake/RavlPortAudio_Body/Target)
install(TARGETS RavlPortAudio_Body EXPORT RavlPortAudio_BodyConfig
   ARCHIVE  DESTINATION lib
   LIBRARY  DESTINATION lib
   RUNTIME  DESTINATION bin)  # This is for Windows
install(TARGETS RavlPortAudio_Body DESTINATION lib)

configure_package_config_file(cmake/RavlPortAudio_BodyConfig.cmake.in
  ${CMAKE_CURRENT_BINARY_DIR}/RavlPortAudio_BodyConfig.cmake
  INSTALL_DESTINATION lib/cmake/RavlPortAudio_Body
  PATH_VARS)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/RavlPortAudio_BodyConfig.cmake
        DESTINATION lib/cmake/RavlPortAudio_Body )


install(TARGETS RavlPortAudio EXPORT RavlPortAudioConfig
   ARCHIVE  DESTINATION lib
   LIBRARY  DESTINATION lib
   RUNTIME  DESTINATION bin)  # This is for Windows

install(EXPORT RavlPortAudioConfig DESTINATION lib/cmake/RavlPortAudio/Target)
install(TARGETS RavlPortAudio DESTINATION lib)

configure_package_config_file(cmake/RavlPortAudioConfig.cmake.in
  ${CMAKE_CURRENT_BINARY_DIR}/RavlPortAudioConfig.cmake
  INSTALL_DESTINATION lib/cmake/RavlPortAudio
  PATH_VARS)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/RavlPortAudioConfig.cmake
        DESTINATION lib/cmake/RavlPortAudio )

install(DIRECTORY include DESTINATION .)



# Examples
set(EXAMPLE_LIBS  RavlIO RavlAudioUtil RavlAudioIO ${portaudio_LIBRARIES} RavlThreads RavlCore RavlOSIO RavlOS  )

add_executable(exPortAudio ${EXCLUDE_EXAMPLES} examples/exPortAudio.cc )
target_link_libraries(exPortAudio RavlPortAudio ${EXAMPLE_LIBS} )
install(TARGETS exPortAudio DESTINATION examples COMPONENT examples ${EXCLUDE_EXAMPLES})





ENDIF()


