
# Library: RavlXMLFactory

# Requires: 
# Supports: 




# MustLink: src/LinkXMLFactory.cc


set(DEPENDS_ON )
foreach(D IN LISTS DEPENDS_ON)
  if(NOT ${D}_FOUND)
    find_package(${D} REQUIRED)
  endif()
endforeach()

include(CMakePackageConfigHelpers)

include_directories(include)
include_directories(src)

set(HAVE_RavlXMLFactory 1)

include_directories(../../Core/RavlCore/include)
include_directories(../../Core/RavlIO/include)
include_directories(../../OS/RavlThreads/include)
include_directories(../../OS/RavlOS/include)




add_library(RavlXMLFactory_Body STATIC
 include/Ravl/XMLFactory.hh include/Ravl/XMLFactoryRegister.hh include/Ravl/XMLFactoryAutoStart.hh include/Ravl/ConfigParameter.hh
 src/XMLFactory.cc src/XMLFactoryRegister.cc src/XMLFactoryAutoStart.cc src/ConfigParameter.cc src/DPxxxSetup.cc
)
target_link_libraries(RavlXMLFactory_Body PUBLIC RavlCore RavlIO RavlThreads RavlOS )

add_library(RavlXMLFactory OBJECT src/LinkXMLFactory.cc )
target_link_libraries(RavlXMLFactory PUBLIC RavlXMLFactory_Body )


target_include_directories(
    RavlXMLFactory PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
    )

# MustLink fixes:
install(EXPORT RavlXMLFactory_BodyConfig DESTINATION lib/cmake/RavlXMLFactory_Body/Target)
install(TARGETS RavlXMLFactory_Body EXPORT RavlXMLFactory_BodyConfig
   ARCHIVE  DESTINATION lib
   LIBRARY  DESTINATION lib
   RUNTIME  DESTINATION bin)  # This is for Windows
install(TARGETS RavlXMLFactory_Body DESTINATION lib)

configure_package_config_file(cmake/RavlXMLFactory_BodyConfig.cmake.in
  ${CMAKE_CURRENT_BINARY_DIR}/RavlXMLFactory_BodyConfig.cmake
  INSTALL_DESTINATION lib/cmake/RavlXMLFactory_Body
  PATH_VARS)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/RavlXMLFactory_BodyConfig.cmake
        DESTINATION lib/cmake/RavlXMLFactory_Body )


install(TARGETS RavlXMLFactory EXPORT RavlXMLFactoryConfig
   ARCHIVE  DESTINATION lib
   LIBRARY  DESTINATION lib
   RUNTIME  DESTINATION bin)  # This is for Windows

install(EXPORT RavlXMLFactoryConfig DESTINATION lib/cmake/RavlXMLFactory/Target)
install(TARGETS RavlXMLFactory DESTINATION lib)

configure_package_config_file(cmake/RavlXMLFactoryConfig.cmake.in
  ${CMAKE_CURRENT_BINARY_DIR}/RavlXMLFactoryConfig.cmake
  INSTALL_DESTINATION lib/cmake/RavlXMLFactory
  PATH_VARS)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/RavlXMLFactoryConfig.cmake
        DESTINATION lib/cmake/RavlXMLFactory )

install(DIRECTORY include DESTINATION .)



# Examples
set(EXAMPLE_LIBS  RavlCore RavlIO RavlThreads RavlOS  )

add_executable(exXMLFactory ${EXCLUDE_EXAMPLES} examples/exXMLFactory.cc )
target_link_libraries(exXMLFactory RavlXMLFactory ${EXAMPLE_LIBS} )
install(TARGETS exXMLFactory DESTINATION examples COMPONENT examples ${EXCLUDE_EXAMPLES})

# Tests
set(TEST_LIBS  RavlCore RavlIO RavlThreads RavlOS  )

add_executable(testXMLFactory ${EXCLUDE_UNITTESTS} tests/testXMLFactory.cc )
target_link_libraries(testXMLFactory RavlXMLFactory ${TEST_LIBS} )
add_test(testXMLFactory testXMLFactory)
install(TARGETS testXMLFactory DESTINATION tests COMPONENT unittests ${EXCLUDE_UNITTESTS})






