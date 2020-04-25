
# Library: RavlHMM

# Requires: 
# Supports: 






set(DEPENDS_ON )
foreach(D IN LISTS DEPENDS_ON)
  if(NOT ${D}_FOUND)
    find_package(${D} REQUIRED)
  endif()
endforeach()

include(CMakePackageConfigHelpers)

include_directories(include)
include_directories(src)

set(HAVE_RavlHMM 1)

include_directories(../../Core/RavlCore/include)
include_directories(../../Math/RavlMath/include)




add_library(RavlHMM STATIC include/Ravl/HiddenMarkovModel.hh src/HiddenMarkovModel.cc)
target_link_libraries(RavlHMM PUBLIC RavlCore RavlMath )


target_include_directories(
    RavlHMM PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
    )



install(TARGETS RavlHMM EXPORT RavlHMMConfig
   ARCHIVE  DESTINATION lib
   LIBRARY  DESTINATION lib
   RUNTIME  DESTINATION bin)  # This is for Windows

install(EXPORT RavlHMMConfig DESTINATION lib/cmake/RavlHMM/Target)
install(TARGETS RavlHMM DESTINATION lib)

configure_package_config_file(cmake/RavlHMMConfig.cmake.in
  ${CMAKE_CURRENT_BINARY_DIR}/RavlHMMConfig.cmake
  INSTALL_DESTINATION lib/cmake/RavlHMM
  PATH_VARS)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/RavlHMMConfig.cmake
        DESTINATION lib/cmake/RavlHMM )

install(DIRECTORY include DESTINATION .)



# Examples
set(EXAMPLE_LIBS  RavlCore RavlMath  )

add_executable(exHiddenMarkovModel EXCLUDE_FROM_ALL examples/exHiddenMarkovModel.cc )
target_link_libraries(exHiddenMarkovModel RavlHMM ${EXAMPLE_LIBS} )
install(TARGETS exHiddenMarkovModel DESTINATION examples COMPONENT examples EXCLUDE_FROM_ALL)

# Tests
set(TEST_LIBS  RavlCore RavlMath  )

add_executable(testHiddenMarkovModel EXCLUDE_FROM_ALL tests/testHiddenMarkovModel.cc )
target_link_libraries(testHiddenMarkovModel RavlHMM ${TEST_LIBS} )
add_test(testHiddenMarkovModel testHiddenMarkovModel)
install(TARGETS testHiddenMarkovModel DESTINATION tests COMPONENT unittests EXCLUDE_FROM_ALL)






