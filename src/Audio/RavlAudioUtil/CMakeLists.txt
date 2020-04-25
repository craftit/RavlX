
# Library: RavlAudioUtil

# Requires: 
# Supports: 




# MustLink: src/linkRavlAudioUtil.cc


set(DEPENDS_ON )
foreach(D IN LISTS DEPENDS_ON)
  if(NOT ${D}_FOUND)
    find_package(${D} REQUIRED)
  endif()
endforeach()

include(CMakePackageConfigHelpers)

include_directories(include)
include_directories(src)

set(HAVE_RavlAudioUtil 1)

include_directories(../../Core/RavlIO/include)
include_directories(../../OS/RavlOSIO/include)
include_directories(../../OS/RavlXMLFactory/include)
include_directories(../../Core/RavlCore/include)
include_directories(../../OS/RavlThreads/include)




add_library(RavlAudioUtil_Body STATIC
 include/Ravl/Audio/AudioIO.hh include/Ravl/Audio/Types.hh include/Ravl/Audio/WindowSignal.hh include/Ravl/Audio/Segment.hh include/Ravl/Audio/Transcription.hh include/Ravl/Audio/AudioFrame.hh
 src/AudioIO.cc src/WindowSignal.cc src/Segment.cc src/Transcription.cc src/AudioFrame.cc
)
target_link_libraries(RavlAudioUtil_Body PUBLIC RavlIO RavlOSIO RavlXMLFactory RavlCore RavlThreads )

add_library(RavlAudioUtil OBJECT src/linkRavlAudioUtil.cc )
target_link_libraries(RavlAudioUtil PUBLIC RavlAudioUtil_Body )


target_include_directories(
    RavlAudioUtil PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
    )

# MustLink fixes:
install(EXPORT RavlAudioUtil_BodyConfig DESTINATION lib/cmake/RavlAudioUtil_Body/Target)
install(TARGETS RavlAudioUtil_Body EXPORT RavlAudioUtil_BodyConfig
   ARCHIVE  DESTINATION lib
   LIBRARY  DESTINATION lib
   RUNTIME  DESTINATION bin)  # This is for Windows
install(TARGETS RavlAudioUtil_Body DESTINATION lib)

configure_package_config_file(cmake/RavlAudioUtil_BodyConfig.cmake.in
  ${CMAKE_CURRENT_BINARY_DIR}/RavlAudioUtil_BodyConfig.cmake
  INSTALL_DESTINATION lib/cmake/RavlAudioUtil_Body
  PATH_VARS)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/RavlAudioUtil_BodyConfig.cmake
        DESTINATION lib/cmake/RavlAudioUtil_Body )


install(TARGETS RavlAudioUtil EXPORT RavlAudioUtilConfig
   ARCHIVE  DESTINATION lib
   LIBRARY  DESTINATION lib
   RUNTIME  DESTINATION bin)  # This is for Windows

install(EXPORT RavlAudioUtilConfig DESTINATION lib/cmake/RavlAudioUtil/Target)
install(TARGETS RavlAudioUtil DESTINATION lib)

configure_package_config_file(cmake/RavlAudioUtilConfig.cmake.in
  ${CMAKE_CURRENT_BINARY_DIR}/RavlAudioUtilConfig.cmake
  INSTALL_DESTINATION lib/cmake/RavlAudioUtil
  PATH_VARS)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/RavlAudioUtilConfig.cmake
        DESTINATION lib/cmake/RavlAudioUtil )

install(DIRECTORY include DESTINATION .)




# Tests
set(TEST_LIBS  RavlIO RavlOSIO RavlXMLFactory RavlCore RavlThreads  )

add_executable(testAudioUtil ${EXCLUDE_UNITTESTS} tests/testAudioUtil.cc )
target_link_libraries(testAudioUtil RavlAudioUtil ${TEST_LIBS} )
add_test(testAudioUtil testAudioUtil)
install(TARGETS testAudioUtil DESTINATION tests COMPONENT unittests ${EXCLUDE_UNITTESTS})






