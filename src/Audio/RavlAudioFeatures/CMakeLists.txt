
# Library: RavlAudioFeatures

# Requires: 
# Supports: 




# MustLink: src/linkRavlAudioFeatures.cc


set(DEPENDS_ON )
foreach(D IN LISTS DEPENDS_ON)
  if(NOT ${D}_FOUND)
    find_package(${D} REQUIRED)
  endif()
endforeach()

include(CMakePackageConfigHelpers)

include_directories(include)
include_directories(src)

set(HAVE_RavlAudioFeatures 1)

include_directories(../../Audio/RavlAudioUtil/include)
include_directories(../../Math/RavlMath/include)
include_directories(../../OS/RavlOS/include)
include_directories(../../Audio/RavlAudioIO/include)
include_directories(../../Core/RavlCore/include)
include_directories(../../Core/RavlIO/include)
include_directories(../../OS/RavlXMLFactory/include)
include_directories(../../OS/RavlThreads/include)
include_directories(../../OS/RavlOSIO/include)



if(DEFINED HAVE_RavlDevAudio)
include_directories(../../Contrib/RavlDevAudio/include)
endif()
if(DEFINED HAVE_RavlAudioFile)
include_directories(../../Contrib/RavlAudioFile/include)
endif()

add_library(RavlAudioFeatures_Body STATIC
 include/Ravl/Audio/MelSpectrum.hh include/Ravl/Audio/MelCepstrum.hh include/Ravl/Audio/PreEmphasis.hh include/Ravl/Audio/FeatureMFCC.hh include/Ravl/Audio/VectorDelta012.hh
 src/MelSpectrum.cc src/MelCepstrum.cc src/PreEmphasis.cc src/FeatureMFCC.cc src/VectorDelta012.cc
)
target_link_libraries(RavlAudioFeatures_Body PUBLIC RavlAudioUtil RavlMath RavlOS RavlAudioIO RavlCore RavlIO RavlXMLFactory RavlThreads )

add_library(RavlAudioFeatures OBJECT src/linkRavlAudioFeatures.cc )
target_link_libraries(RavlAudioFeatures PUBLIC RavlAudioFeatures_Body )


target_include_directories(
    RavlAudioFeatures PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
    )

# MustLink fixes:
install(EXPORT RavlAudioFeatures_BodyConfig DESTINATION lib/cmake/RavlAudioFeatures_Body/Target)
install(TARGETS RavlAudioFeatures_Body EXPORT RavlAudioFeatures_BodyConfig
   ARCHIVE  DESTINATION lib
   LIBRARY  DESTINATION lib
   RUNTIME  DESTINATION bin)  # This is for Windows
install(TARGETS RavlAudioFeatures_Body DESTINATION lib)

configure_package_config_file(cmake/RavlAudioFeatures_BodyConfig.cmake.in
  ${CMAKE_CURRENT_BINARY_DIR}/RavlAudioFeatures_BodyConfig.cmake
  INSTALL_DESTINATION lib/cmake/RavlAudioFeatures_Body
  PATH_VARS)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/RavlAudioFeatures_BodyConfig.cmake
        DESTINATION lib/cmake/RavlAudioFeatures_Body )


install(TARGETS RavlAudioFeatures EXPORT RavlAudioFeaturesConfig
   ARCHIVE  DESTINATION lib
   LIBRARY  DESTINATION lib
   RUNTIME  DESTINATION bin)  # This is for Windows

install(EXPORT RavlAudioFeaturesConfig DESTINATION lib/cmake/RavlAudioFeatures/Target)
install(TARGETS RavlAudioFeatures DESTINATION lib)

configure_package_config_file(cmake/RavlAudioFeaturesConfig.cmake.in
  ${CMAKE_CURRENT_BINARY_DIR}/RavlAudioFeaturesConfig.cmake
  INSTALL_DESTINATION lib/cmake/RavlAudioFeatures
  PATH_VARS)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/RavlAudioFeaturesConfig.cmake
        DESTINATION lib/cmake/RavlAudioFeatures )

install(DIRECTORY include DESTINATION .)



# Examples
set(EXAMPLE_LIBS  RavlAudioUtil RavlMath RavlOS RavlAudioIO RavlCore RavlIO RavlXMLFactory RavlThreads RavlOSIO  )

add_executable(exFeatureMFCC ${EXCLUDE_EXAMPLES} examples/exFeatureMFCC.cc )
target_link_libraries(exFeatureMFCC RavlAudioFeatures ${EXAMPLE_LIBS} )
install(TARGETS exFeatureMFCC DESTINATION examples COMPONENT examples ${EXCLUDE_EXAMPLES})

# Tests
set(TEST_LIBS  RavlAudioUtil RavlMath RavlOS RavlAudioIO RavlCore RavlIO RavlXMLFactory RavlThreads RavlOSIO  )

add_executable(testAudioFeatures ${EXCLUDE_UNITTESTS} tests/testAudioFeatures.cc )
target_link_libraries(testAudioFeatures RavlAudioFeatures ${TEST_LIBS} )
add_test(testAudioFeatures testAudioFeatures)
install(TARGETS testAudioFeatures DESTINATION tests COMPONENT unittests ${EXCLUDE_UNITTESTS})






