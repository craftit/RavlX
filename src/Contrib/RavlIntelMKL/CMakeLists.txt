
# Library: RavlIntelMKL

# Requires: libmkl
# Supports: !VCPP

if((NOT ${CMAKE_CXX_COMPILER_ID} MATCHES "MSVC") OR FALSE)

if(MKL_FOUND)

# MustLink: src/linkIntelMKL.cc


set(DEPENDS_ON )
foreach(D IN LISTS DEPENDS_ON)
  if(NOT ${D}_FOUND)
    find_package(${D} REQUIRED)
  endif()
endforeach()

include(CMakePackageConfigHelpers)

include_directories(include)
include_directories(src)

set(HAVE_RavlIntelMKL 1)

include_directories(../../Math/RavlMath/include)
include_directories(../../Core/RavlCore/include)
include_directories(../../OS/RavlOS/include)




add_library(RavlIntelMKL_Body STATIC
 include/Ravl/IntelMKL.hh include/Ravl/IntelFFT2d.hh
 src/IntelMKL.cc src/IntelFFT2d.cc
)
target_link_libraries(RavlIntelMKL_Body PUBLIC ${MKL_LIBRARIES} RavlMath RavlCore )

add_library(RavlIntelMKL OBJECT src/linkIntelMKL.cc )
target_link_libraries(RavlIntelMKL PUBLIC RavlIntelMKL_Body )


target_include_directories(
    RavlIntelMKL PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
    )

# MustLink fixes:
install(EXPORT RavlIntelMKL_BodyConfig DESTINATION lib/cmake/RavlIntelMKL_Body/Target)
install(TARGETS RavlIntelMKL_Body EXPORT RavlIntelMKL_BodyConfig
   ARCHIVE  DESTINATION lib
   LIBRARY  DESTINATION lib
   RUNTIME  DESTINATION bin)  # This is for Windows
install(TARGETS RavlIntelMKL_Body DESTINATION lib)

configure_package_config_file(cmake/RavlIntelMKL_BodyConfig.cmake.in
  ${CMAKE_CURRENT_BINARY_DIR}/RavlIntelMKL_BodyConfig.cmake
  INSTALL_DESTINATION lib/cmake/RavlIntelMKL_Body
  PATH_VARS)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/RavlIntelMKL_BodyConfig.cmake
        DESTINATION lib/cmake/RavlIntelMKL_Body )


install(TARGETS RavlIntelMKL EXPORT RavlIntelMKLConfig
   ARCHIVE  DESTINATION lib
   LIBRARY  DESTINATION lib
   RUNTIME  DESTINATION bin)  # This is for Windows

install(EXPORT RavlIntelMKLConfig DESTINATION lib/cmake/RavlIntelMKL/Target)
install(TARGETS RavlIntelMKL DESTINATION lib)

configure_package_config_file(cmake/RavlIntelMKLConfig.cmake.in
  ${CMAKE_CURRENT_BINARY_DIR}/RavlIntelMKLConfig.cmake
  INSTALL_DESTINATION lib/cmake/RavlIntelMKL
  PATH_VARS)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/RavlIntelMKLConfig.cmake
        DESTINATION lib/cmake/RavlIntelMKL )

install(DIRECTORY include DESTINATION .)




# Tests
set(TEST_LIBS  ${MKL_LIBRARIES} RavlMath RavlCore RavlOS  )

add_executable(testIntelFFT2d ${EXCLUDE_UNITTESTS} tests/testIntelFFT2d.cc )
target_link_libraries(testIntelFFT2d RavlIntelMKL ${TEST_LIBS} )
add_test(testIntelFFT2d testIntelFFT2d)
install(TARGETS testIntelFFT2d DESTINATION tests COMPONENT unittests ${EXCLUDE_UNITTESTS})

add_executable(testIntelMKL ${EXCLUDE_UNITTESTS} tests/testIntelMKL.cc )
target_link_libraries(testIntelMKL RavlIntelMKL ${TEST_LIBS} )
add_test(testIntelMKL testIntelMKL)
install(TARGETS testIntelMKL DESTINATION tests COMPONENT unittests ${EXCLUDE_UNITTESTS})




ENDIF()

ENDIF()
