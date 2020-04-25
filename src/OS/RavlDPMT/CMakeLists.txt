
# Library: RavlDPMT

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

set(HAVE_RavlDPMT 1)

include_directories(../../Core/RavlCore/include)
include_directories(../../OS/RavlOS/include)
include_directories(../../Core/RavlIO/include)
include_directories(../../OS/RavlThreads/include)




add_library(RavlDPMT STATIC include/Ravl/DP/MTIOConnect.hh include/Ravl/DP/PlayControl.hh include/Ravl/DP/ThreadPipe.hh include/Ravl/DP/Buffer.hh include/Ravl/DP/FixedBuffer.hh include/Ravl/DP/Governor.hh include/Ravl/DP/Hold.hh include/Ravl/DP/SerialisePort.hh include/Ravl/DP/Blackboard.hh include/Ravl/DP/SPortShare.hh include/Ravl/DP/SplitI.hh include/Ravl/DP/TailIStream.hh include/Ravl/DP/OpenPortInventory.hh src/MTIOConnect.cc src/PlayControl.cc src/Governor.cc src/Blackboard.cc src/TailIStream.cc src/OpenPortInventory.cc)
target_link_libraries(RavlDPMT PUBLIC RavlCore RavlOS RavlIO RavlThreads )


target_include_directories(
    RavlDPMT PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
    )



install(TARGETS RavlDPMT EXPORT RavlDPMTConfig
   ARCHIVE  DESTINATION lib
   LIBRARY  DESTINATION lib
   RUNTIME  DESTINATION bin)  # This is for Windows

install(EXPORT RavlDPMTConfig DESTINATION lib/cmake/RavlDPMT/Target)
install(TARGETS RavlDPMT DESTINATION lib)

configure_package_config_file(cmake/RavlDPMTConfig.cmake.in
  ${CMAKE_CURRENT_BINARY_DIR}/RavlDPMTConfig.cmake
  INSTALL_DESTINATION lib/cmake/RavlDPMT
  PATH_VARS)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/RavlDPMTConfig.cmake
        DESTINATION lib/cmake/RavlDPMT )

install(DIRECTORY include DESTINATION .)



# Examples
set(EXAMPLE_LIBS  RavlCore RavlOS RavlIO RavlThreads  )

add_executable(exMTDataProc EXCLUDE_FROM_ALL examples/exMTDataProc.cc )
target_link_libraries(exMTDataProc RavlDPMT ${EXAMPLE_LIBS} )
install(TARGETS exMTDataProc DESTINATION examples COMPONENT examples EXCLUDE_FROM_ALL)

add_executable(exDPSplit EXCLUDE_FROM_ALL examples/exDPSplit.cc )
target_link_libraries(exDPSplit RavlDPMT ${EXAMPLE_LIBS} )
install(TARGETS exDPSplit DESTINATION examples COMPONENT examples EXCLUDE_FROM_ALL)

# Tests
set(TEST_LIBS  RavlCore RavlOS RavlIO RavlThreads  )

add_executable(testOSDataProc EXCLUDE_FROM_ALL tests/testOSDataProc.cc )
target_link_libraries(testOSDataProc RavlDPMT ${TEST_LIBS} )
add_test(testOSDataProc testOSDataProc)
install(TARGETS testOSDataProc DESTINATION tests COMPONENT unittests EXCLUDE_FROM_ALL)






