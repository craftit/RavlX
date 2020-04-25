
# Library: RavlZLib

# Requires: Zlib
# Supports: 



if(ZLIB_FOUND)



set(DEPENDS_ON )
foreach(D IN LISTS DEPENDS_ON)
  if(NOT ${D}_FOUND)
    find_package(${D} REQUIRED)
  endif()
endforeach()

include(CMakePackageConfigHelpers)

include_directories(include)
include_directories(src)

set(HAVE_RavlZLib 1)

include_directories(../../Core/RavlCore/include)




add_library(RavlZLib STATIC include/Ravl/Compress.hh src/Compress.cc)
target_link_libraries(RavlZLib PUBLIC RavlCore ${ZLIB_LIBRARIES} )


target_include_directories(
    RavlZLib PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
    )



install(TARGETS RavlZLib EXPORT RavlZLibConfig
   ARCHIVE  DESTINATION lib
   LIBRARY  DESTINATION lib
   RUNTIME  DESTINATION bin)  # This is for Windows

install(EXPORT RavlZLibConfig DESTINATION lib/cmake/RavlZLib/Target)
install(TARGETS RavlZLib DESTINATION lib)

configure_package_config_file(cmake/RavlZLibConfig.cmake.in
  ${CMAKE_CURRENT_BINARY_DIR}/RavlZLibConfig.cmake
  INSTALL_DESTINATION lib/cmake/RavlZLib
  PATH_VARS)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/RavlZLibConfig.cmake
        DESTINATION lib/cmake/RavlZLib )

install(DIRECTORY include DESTINATION .)




# Tests
set(TEST_LIBS  RavlCore ${ZLIB_LIBRARIES}  )

add_executable(testCompress EXCLUDE_FROM_ALL tests/testCompress.cc )
target_link_libraries(testCompress RavlZLib ${TEST_LIBS} )
add_test(testCompress testCompress)
install(TARGETS testCompress DESTINATION tests COMPONENT unittests EXCLUDE_FROM_ALL)




ENDIF()


