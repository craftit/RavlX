
# Library: RavlAudioFile

# Requires: libAudioFile
# Supports: !VCPP

if((NOT ${CMAKE_CXX_COMPILER_ID} MATCHES "MSVC") OR FALSE)

if(audiofile_FOUND)

# MustLink: src/RavlAudioFile.cc


set(DEPENDS_ON )
foreach(D IN LISTS DEPENDS_ON)
  if(NOT ${D}_FOUND)
    find_package(${D} REQUIRED)
  endif()
endforeach()

include(CMakePackageConfigHelpers)

include_directories(include)
include_directories(src)

set(HAVE_RavlAudioFile 1)

include_directories(../../Core/RavlIO/include)
include_directories(../../Audio/RavlAudioUtil/include)
include_directories(../../Audio/RavlAudioIO/include)
include_directories(../../Core/RavlCore/include)
include_directories(../../OS/RavlOSIO/include)
include_directories(../../Contrib/RavlDevAudio/include)




add_library(RavlAudioFile_Body STATIC
 include/Ravl/Audio/AudioFileIO.hh include/Ravl/Audio/AudioFileFormat.hh
 src/AudioFileIO.cc src/AudioFileFormat.cc
)
target_link_libraries(RavlAudioFile_Body PUBLIC RavlIO ${audiofile_LIBRARIES} RavlAudioUtil RavlAudioIO RavlCore RavlOSIO )

add_library(RavlAudioFile OBJECT src/RavlAudioFile.cc )
target_link_libraries(RavlAudioFile PUBLIC RavlAudioFile_Body )


target_include_directories(
    RavlAudioFile PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
    )

# MustLink fixes:
install(EXPORT RavlAudioFile_BodyConfig DESTINATION lib/cmake/RavlAudioFile_Body/Target)
install(TARGETS RavlAudioFile_Body EXPORT RavlAudioFile_BodyConfig
   ARCHIVE  DESTINATION lib
   LIBRARY  DESTINATION lib
   RUNTIME  DESTINATION bin)  # This is for Windows
install(TARGETS RavlAudioFile_Body DESTINATION lib)

configure_package_config_file(cmake/RavlAudioFile_BodyConfig.cmake.in
  ${CMAKE_CURRENT_BINARY_DIR}/RavlAudioFile_BodyConfig.cmake
  INSTALL_DESTINATION lib/cmake/RavlAudioFile_Body
  PATH_VARS)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/RavlAudioFile_BodyConfig.cmake
        DESTINATION lib/cmake/RavlAudioFile_Body )


install(TARGETS RavlAudioFile EXPORT RavlAudioFileConfig
   ARCHIVE  DESTINATION lib
   LIBRARY  DESTINATION lib
   RUNTIME  DESTINATION bin)  # This is for Windows

install(EXPORT RavlAudioFileConfig DESTINATION lib/cmake/RavlAudioFile/Target)
install(TARGETS RavlAudioFile DESTINATION lib)

configure_package_config_file(cmake/RavlAudioFileConfig.cmake.in
  ${CMAKE_CURRENT_BINARY_DIR}/RavlAudioFileConfig.cmake
  INSTALL_DESTINATION lib/cmake/RavlAudioFile
  PATH_VARS)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/RavlAudioFileConfig.cmake
        DESTINATION lib/cmake/RavlAudioFile )

install(DIRECTORY include DESTINATION .)



# Examples
set(EXAMPLE_LIBS  RavlIO ${audiofile_LIBRARIES} RavlAudioUtil RavlAudioIO RavlCore RavlOSIO RavlDevAudio  )

add_executable(exAudioFile EXCLUDE_FROM_ALL examples/exAudioFile.cc )
target_link_libraries(exAudioFile RavlAudioFile ${EXAMPLE_LIBS} )
install(TARGETS exAudioFile DESTINATION examples COMPONENT examples EXCLUDE_FROM_ALL)





ENDIF()

ENDIF()
