
# Library: RavlOSIO

# Requires: 
# Supports: 




# MustLink: src/RavlOSIOMustLink.cc


set(DEPENDS_ON )
foreach(D IN LISTS DEPENDS_ON)
  if(NOT ${D}_FOUND)
    find_package(${D} REQUIRED)
  endif()
endforeach()

include(CMakePackageConfigHelpers)

include_directories(include)
include_directories(src)

set(HAVE_RavlOSIO 1)

include_directories(../../Core/RavlCore/include)
include_directories(../../OS/RavlOS/include)
include_directories(../../Core/RavlIO/include)
include_directories(../../OS/RavlXMLFactory/include)
include_directories(../../OS/RavlThreads/include)




add_library(RavlOSIO_Body STATIC
 include/Ravl/DP/FileSequence.hh include/Ravl/DP/SequenceIO.hh include/Ravl/DP/FileListIO.hh include/Ravl/DP/CompositeStream.hh include/Ravl/DP/FileFormatComposite.hh include/Ravl/DP/PlayList.hh include/Ravl/DP/SubSequenceSpec.hh include/Ravl/DP/EditSpec.hh include/Ravl/DP/FileFormatDynamic.hh include/Ravl/DP/DynamicLink.hh include/Ravl/DP/ProcessPlayList.hh include/Ravl/DP/DPNetwork.hh include/Ravl/DP/DynamicIO.hh
 src/FileSequence.cc src/SequenceIO.cc src/FileListIO.cc src/CompositeStream.cc src/FileFormatComposite.cc src/PlayList.cc src/SubSequenceSpec.cc src/EditSpec.cc src/FileFormatDynamic.cc src/DynamicLink.cc src/ProcessPlayList.cc src/DPNetwork.cc src/DPStreamOpSetup.cc
)
target_link_libraries(RavlOSIO_Body PUBLIC RavlCore RavlOS RavlIO dl RavlXMLFactory RavlThreads )

add_library(RavlOSIO OBJECT src/RavlOSIOMustLink.cc )
target_link_libraries(RavlOSIO PUBLIC RavlOSIO_Body )


target_include_directories(
    RavlOSIO PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
    )

# MustLink fixes:
install(EXPORT RavlOSIO_BodyConfig DESTINATION lib/cmake/RavlOSIO_Body/Target)
install(TARGETS RavlOSIO_Body EXPORT RavlOSIO_BodyConfig
   ARCHIVE  DESTINATION lib
   LIBRARY  DESTINATION lib
   RUNTIME  DESTINATION bin)  # This is for Windows
install(TARGETS RavlOSIO_Body DESTINATION lib)

configure_package_config_file(cmake/RavlOSIO_BodyConfig.cmake.in
  ${CMAKE_CURRENT_BINARY_DIR}/RavlOSIO_BodyConfig.cmake
  INSTALL_DESTINATION lib/cmake/RavlOSIO_Body
  PATH_VARS)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/RavlOSIO_BodyConfig.cmake
        DESTINATION lib/cmake/RavlOSIO_Body )


install(TARGETS RavlOSIO EXPORT RavlOSIOConfig
   ARCHIVE  DESTINATION lib
   LIBRARY  DESTINATION lib
   RUNTIME  DESTINATION bin)  # This is for Windows

install(EXPORT RavlOSIOConfig DESTINATION lib/cmake/RavlOSIO/Target)
install(TARGETS RavlOSIO DESTINATION lib)

configure_package_config_file(cmake/RavlOSIOConfig.cmake.in
  ${CMAKE_CURRENT_BINARY_DIR}/RavlOSIOConfig.cmake
  INSTALL_DESTINATION lib/cmake/RavlOSIO
  PATH_VARS)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/RavlOSIOConfig.cmake
        DESTINATION lib/cmake/RavlOSIO )

install(DIRECTORY include DESTINATION .)



# Examples
set(EXAMPLE_LIBS  RavlCore RavlOS RavlIO dl RavlXMLFactory RavlThreads  )

add_executable(exCompositeStream ${EXCLUDE_EXAMPLES} examples/exCompositeStream.cc )
target_link_libraries(exCompositeStream RavlOSIO ${EXAMPLE_LIBS} )
install(TARGETS exCompositeStream DESTINATION examples COMPONENT examples ${EXCLUDE_EXAMPLES})

add_executable(exDynamicLink ${EXCLUDE_EXAMPLES} examples/exDynamicLink.cc )
target_link_libraries(exDynamicLink RavlOSIO ${EXAMPLE_LIBS} )
install(TARGETS exDynamicLink DESTINATION examples COMPONENT examples ${EXCLUDE_EXAMPLES})

add_executable(exDynamicFormat ${EXCLUDE_EXAMPLES} examples/exDynamicFormat.cc )
target_link_libraries(exDynamicFormat RavlOSIO ${EXAMPLE_LIBS} )
install(TARGETS exDynamicFormat DESTINATION examples COMPONENT examples ${EXCLUDE_EXAMPLES})

# Tests
set(TEST_LIBS  RavlCore RavlOS RavlIO dl RavlXMLFactory RavlThreads  )

add_executable(testSequenceIO ${EXCLUDE_UNITTESTS} tests/testSequenceIO.cc )
target_link_libraries(testSequenceIO RavlOSIO ${TEST_LIBS} )
add_test(testSequenceIO testSequenceIO)
install(TARGETS testSequenceIO DESTINATION tests COMPONENT unittests ${EXCLUDE_UNITTESTS})






