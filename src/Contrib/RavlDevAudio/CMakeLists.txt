
# Library: RavlDevAudio

# Requires: 
# Supports: linux linux64

if((${CMAKE_SYSTEM_NAME} MATCHES "Linux32") OR (${CMAKE_SYSTEM_NAME} MATCHES "Linux") OR FALSE)


# MustLink: src/RavlDevAudio.cc


set(DEPENDS_ON )
foreach(D IN LISTS DEPENDS_ON)
  if(NOT ${D}_FOUND)
    find_package(${D} REQUIRED)
  endif()
endforeach()

include(CMakePackageConfigHelpers)

include_directories(include)
include_directories(src)

set(HAVE_RavlDevAudio 1)

include_directories(../../Core/RavlIO/include)
include_directories(../../Audio/RavlAudioUtil/include)
include_directories(../../OS/RavlOSIO/include)
include_directories(../../Audio/RavlAudioIO/include)
include_directories(../../Core/RavlCore/include)




add_library(RavlDevAudio_Body STATIC
 include/Ravl/Audio/DevAudioIO.hh include/Ravl/Audio/DevAudioFormat.hh
 src/DevAudioIO.cc src/DevAudioFormat.cc
)
target_link_libraries(RavlDevAudio_Body PUBLIC RavlIO RavlAudioUtil RavlOSIO RavlAudioIO RavlCore )

add_library(RavlDevAudio OBJECT src/RavlDevAudio.cc )
target_link_libraries(RavlDevAudio PUBLIC RavlDevAudio_Body )


target_include_directories(
    RavlDevAudio PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
    )

# MustLink fixes:
install(EXPORT RavlDevAudio_BodyConfig DESTINATION lib/cmake/RavlDevAudio_Body/Target)
install(TARGETS RavlDevAudio_Body EXPORT RavlDevAudio_BodyConfig
   ARCHIVE  DESTINATION lib
   LIBRARY  DESTINATION lib
   RUNTIME  DESTINATION bin)  # This is for Windows
install(TARGETS RavlDevAudio_Body DESTINATION lib)

configure_package_config_file(cmake/RavlDevAudio_BodyConfig.cmake.in
  ${CMAKE_CURRENT_BINARY_DIR}/RavlDevAudio_BodyConfig.cmake
  INSTALL_DESTINATION lib/cmake/RavlDevAudio_Body
  PATH_VARS)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/RavlDevAudio_BodyConfig.cmake
        DESTINATION lib/cmake/RavlDevAudio_Body )


install(TARGETS RavlDevAudio EXPORT RavlDevAudioConfig
   ARCHIVE  DESTINATION lib
   LIBRARY  DESTINATION lib
   RUNTIME  DESTINATION bin)  # This is for Windows

install(EXPORT RavlDevAudioConfig DESTINATION lib/cmake/RavlDevAudio/Target)
install(TARGETS RavlDevAudio DESTINATION lib)

configure_package_config_file(cmake/RavlDevAudioConfig.cmake.in
  ${CMAKE_CURRENT_BINARY_DIR}/RavlDevAudioConfig.cmake
  INSTALL_DESTINATION lib/cmake/RavlDevAudio
  PATH_VARS)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/RavlDevAudioConfig.cmake
        DESTINATION lib/cmake/RavlDevAudio )

install(DIRECTORY include DESTINATION .)



# Examples
set(EXAMPLE_LIBS  RavlIO RavlAudioUtil RavlOSIO RavlAudioIO RavlCore  )

add_executable(exAudioIO EXCLUDE_FROM_ALL examples/exAudioIO.cc )
target_link_libraries(exAudioIO RavlDevAudio ${EXAMPLE_LIBS} )
install(TARGETS exAudioIO DESTINATION examples COMPONENT examples EXCLUDE_FROM_ALL)






ENDIF()
