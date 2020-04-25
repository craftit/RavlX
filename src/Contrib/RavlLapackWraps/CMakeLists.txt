
# Library: RavlLapackWraps

# Requires: LAPACK
# Supports: 



if(LAPACK_FOUND)



set(DEPENDS_ON )
foreach(D IN LISTS DEPENDS_ON)
  if(NOT ${D}_FOUND)
    find_package(${D} REQUIRED)
  endif()
endforeach()

include(CMakePackageConfigHelpers)

include_directories(include)
include_directories(src)

set(HAVE_RavlLapackWraps 1)

include_directories(../../Core/RavlCore/include)
include_directories(../../OS/RavlOS/include)
include_directories(../../Core/RavlIO/include)
include_directories(../../Math/RavlMath/include)




add_library(RavlLapackWraps STATIC include/Ravl/Lapack/ev_c.hh include/Ravl/Lapack/blas2.hh include/Ravl/Lapack/blas2_c.hh include/Ravl/Lapack/lapack.hh src/ev_c.cc src/blas2.cc src/blas2_c.cc src/lapack.cc)
target_link_libraries(RavlLapackWraps PUBLIC ${LAPACK_LIBRARIES} RavlCore )


target_include_directories(
    RavlLapackWraps PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
    )



install(TARGETS RavlLapackWraps EXPORT RavlLapackWrapsConfig
   ARCHIVE  DESTINATION lib
   LIBRARY  DESTINATION lib
   RUNTIME  DESTINATION bin)  # This is for Windows

install(EXPORT RavlLapackWrapsConfig DESTINATION lib/cmake/RavlLapackWraps/Target)
install(TARGETS RavlLapackWraps DESTINATION lib)

configure_package_config_file(cmake/RavlLapackWrapsConfig.cmake.in
  ${CMAKE_CURRENT_BINARY_DIR}/RavlLapackWrapsConfig.cmake
  INSTALL_DESTINATION lib/cmake/RavlLapackWraps
  PATH_VARS)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/RavlLapackWrapsConfig.cmake
        DESTINATION lib/cmake/RavlLapackWraps )

install(DIRECTORY include DESTINATION .)




# Tests
set(TEST_LIBS  ${LAPACK_LIBRARIES} RavlCore RavlOS RavlIO RavlMath  )

add_executable(testLapack EXCLUDE_FROM_ALL tests/testLapack.cc )
target_link_libraries(testLapack RavlLapackWraps ${TEST_LIBS} )
add_test(testLapack testLapack)
install(TARGETS testLapack DESTINATION tests COMPONENT unittests EXCLUDE_FROM_ALL)




ENDIF()


