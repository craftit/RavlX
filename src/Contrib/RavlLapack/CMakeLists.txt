
# Library: RavlLapack

# Requires: LAPACK
# Supports: 



if(LAPACK_FOUND)

# MustLink: src/linkLAHooksLAPACK.cc


set(DEPENDS_ON )
foreach(D IN LISTS DEPENDS_ON)
  if(NOT ${D}_FOUND)
    find_package(${D} REQUIRED)
  endif()
endforeach()

include(CMakePackageConfigHelpers)

include_directories(include)
include_directories(src)

set(HAVE_RavlLapack 1)

include_directories(../../Math/RavlMath/include)
include_directories(../../Contrib/RavlLapackWraps/include)
include_directories(../../Core/RavlCore/include)
include_directories(../../OS/RavlOS/include)




add_library(RavlLapack_Body STATIC
 
 src/LAHooksLAPACK.cc
)
target_link_libraries(RavlLapack_Body PUBLIC RavlMath RavlLapackWraps RavlCore )

add_library(RavlLapack OBJECT src/linkLAHooksLAPACK.cc )
target_link_libraries(RavlLapack PUBLIC RavlLapack_Body )


target_include_directories(
    RavlLapack PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
    )

# MustLink fixes:
install(EXPORT RavlLapack_BodyConfig DESTINATION lib/cmake/RavlLapack_Body/Target)
install(TARGETS RavlLapack_Body EXPORT RavlLapack_BodyConfig
   ARCHIVE  DESTINATION lib
   LIBRARY  DESTINATION lib
   RUNTIME  DESTINATION bin)  # This is for Windows
install(TARGETS RavlLapack_Body DESTINATION lib)

configure_package_config_file(cmake/RavlLapack_BodyConfig.cmake.in
  ${CMAKE_CURRENT_BINARY_DIR}/RavlLapack_BodyConfig.cmake
  INSTALL_DESTINATION lib/cmake/RavlLapack_Body
  PATH_VARS)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/RavlLapack_BodyConfig.cmake
        DESTINATION lib/cmake/RavlLapack_Body )


install(TARGETS RavlLapack EXPORT RavlLapackConfig
   ARCHIVE  DESTINATION lib
   LIBRARY  DESTINATION lib
   RUNTIME  DESTINATION bin)  # This is for Windows

install(EXPORT RavlLapackConfig DESTINATION lib/cmake/RavlLapack/Target)
install(TARGETS RavlLapack DESTINATION lib)

configure_package_config_file(cmake/RavlLapackConfig.cmake.in
  ${CMAKE_CURRENT_BINARY_DIR}/RavlLapackConfig.cmake
  INSTALL_DESTINATION lib/cmake/RavlLapack
  PATH_VARS)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/RavlLapackConfig.cmake
        DESTINATION lib/cmake/RavlLapack )

install(DIRECTORY include DESTINATION .)




# Tests
set(TEST_LIBS  RavlMath RavlLapackWraps RavlCore RavlOS  )

add_executable(testMatrixLapack EXCLUDE_FROM_ALL tests/testMatrixLapack.cc )
target_link_libraries(testMatrixLapack RavlLapack ${TEST_LIBS} )
add_test(testMatrixLapack testMatrixLapack)
install(TARGETS testMatrixLapack DESTINATION tests COMPONENT unittests EXCLUDE_FROM_ALL)




ENDIF()


