
# Library: RavlLogicAgent

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

set(HAVE_RavlLogicAgent 1)

include_directories(../../Logic/RavlLogic/include)
include_directories(../../OS/RavlDPMT/include)
include_directories(../../Logic/RavlLogicNLP/include)
include_directories(../../Core/RavlCore/include)
include_directories(../../Core/RavlIO/include)
include_directories(../../OS/RavlThreads/include)
include_directories(../../OS/RavlOS/include)




add_library(RavlLogicAgent STATIC include/Ravl/Logic/ActiveBlackboard.hh include/Ravl/Logic/Agent.hh include/Ravl/Logic/AgentPlan.hh src/ActiveBlackboard.cc src/Agent.cc src/AgentPlan.cc)
target_link_libraries(RavlLogicAgent PUBLIC RavlLogic RavlDPMT RavlLogicNLP RavlCore RavlIO RavlThreads RavlOS )


target_include_directories(
    RavlLogicAgent PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
    )



install(TARGETS RavlLogicAgent EXPORT RavlLogicAgentConfig
   ARCHIVE  DESTINATION lib
   LIBRARY  DESTINATION lib
   RUNTIME  DESTINATION bin)  # This is for Windows

install(EXPORT RavlLogicAgentConfig DESTINATION lib/cmake/RavlLogicAgent/Target)
install(TARGETS RavlLogicAgent DESTINATION lib)

configure_package_config_file(cmake/RavlLogicAgentConfig.cmake.in
  ${CMAKE_CURRENT_BINARY_DIR}/RavlLogicAgentConfig.cmake
  INSTALL_DESTINATION lib/cmake/RavlLogicAgent
  PATH_VARS)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/RavlLogicAgentConfig.cmake
        DESTINATION lib/cmake/RavlLogicAgent )

install(DIRECTORY include DESTINATION .)



# Examples
set(EXAMPLE_LIBS  RavlLogic RavlDPMT RavlLogicNLP RavlCore RavlIO RavlThreads RavlOS  )

add_executable(exAgent ${EXCLUDE_EXAMPLES} examples/exAgent.cc )
target_link_libraries(exAgent RavlLogicAgent ${EXAMPLE_LIBS} )
install(TARGETS exAgent DESTINATION examples COMPONENT examples ${EXCLUDE_EXAMPLES})

# Tests
set(TEST_LIBS  RavlLogic RavlDPMT RavlLogicNLP RavlCore RavlIO RavlThreads RavlOS  )

add_executable(testActiveBlackboard ${EXCLUDE_UNITTESTS} tests/testActiveBlackboard.cc )
target_link_libraries(testActiveBlackboard RavlLogicAgent ${TEST_LIBS} )
add_test(testActiveBlackboard testActiveBlackboard)
install(TARGETS testActiveBlackboard DESTINATION tests COMPONENT unittests ${EXCLUDE_UNITTESTS})






