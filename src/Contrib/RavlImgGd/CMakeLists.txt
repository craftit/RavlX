
# Library: RavlImgGd

# Requires: LibGd
# Supports: !VCPP

if((NOT ${CMAKE_CXX_COMPILER_ID} MATCHES "MSVC") OR FALSE)

if(libgd_FOUND)



set(DEPENDS_ON )
foreach(D IN LISTS DEPENDS_ON)
  if(NOT ${D}_FOUND)
    find_package(${D} REQUIRED)
  endif()
endforeach()

include(CMakePackageConfigHelpers)

include_directories(include)
include_directories(src)

set(HAVE_RavlImgGd 1)

include_directories(../../Image/RavlImage/include)
include_directories(../../Core/RavlIO/include)
include_directories(../../Core/RavlCore/include)




add_library(RavlImgGd STATIC include/Ravl/Image/Gd.hh src/Gd.cc)
target_link_libraries(RavlImgGd PUBLIC RavlImage ${libgd_LIBRARIES} RavlIO RavlCore )


target_include_directories(
    RavlImgGd PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
    )



install(TARGETS RavlImgGd EXPORT RavlImgGdConfig
   ARCHIVE  DESTINATION lib
   LIBRARY  DESTINATION lib
   RUNTIME  DESTINATION bin)  # This is for Windows

install(EXPORT RavlImgGdConfig DESTINATION lib/cmake/RavlImgGd/Target)
install(TARGETS RavlImgGd DESTINATION lib)

configure_package_config_file(cmake/RavlImgGdConfig.cmake.in
  ${CMAKE_CURRENT_BINARY_DIR}/RavlImgGdConfig.cmake
  INSTALL_DESTINATION lib/cmake/RavlImgGd
  PATH_VARS)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/RavlImgGdConfig.cmake
        DESTINATION lib/cmake/RavlImgGd )

install(DIRECTORY include DESTINATION .)




# Tests
set(TEST_LIBS  RavlImage ${libgd_LIBRARIES} RavlIO RavlCore  )

add_executable(testGd ${EXCLUDE_UNITTESTS} tests/testGd.cc )
target_link_libraries(testGd RavlImgGd ${TEST_LIBS} )
add_test(testGd testGd)
install(TARGETS testGd DESTINATION tests COMPONENT unittests ${EXCLUDE_UNITTESTS})




ENDIF()

ENDIF()
