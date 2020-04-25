
# Library: RavlLogicNLP

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

set(HAVE_RavlLogicNLP 1)

include_directories(../../Logic/RavlLogic/include)
include_directories(../../Core/RavlCore/include)




add_library(RavlLogicNLP STATIC include/Ravl/Logic/NLPCausalLink.hh include/Ravl/Logic/NLPStep.hh include/Ravl/Logic/NLPAgendaItem.hh include/Ravl/Logic/NLPAction.hh include/Ravl/Logic/NLPAgenda.hh include/Ravl/Logic/NonLinearPlan.hh include/Ravl/Logic/NLPTypes.hh include/Ravl/Logic/NLPlanner.hh src/NLPCausalLink.cc src/NLPStep.cc src/NLPAgendaItem.cc src/NLPAction.cc src/NLPAgenda.cc src/NonLinearPlan.cc src/NLPlanner.cc)
target_link_libraries(RavlLogicNLP PUBLIC RavlLogic RavlCore )


target_include_directories(
    RavlLogicNLP PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
    )



install(TARGETS RavlLogicNLP EXPORT RavlLogicNLPConfig
   ARCHIVE  DESTINATION lib
   LIBRARY  DESTINATION lib
   RUNTIME  DESTINATION bin)  # This is for Windows

install(EXPORT RavlLogicNLPConfig DESTINATION lib/cmake/RavlLogicNLP/Target)
install(TARGETS RavlLogicNLP DESTINATION lib)

configure_package_config_file(cmake/RavlLogicNLPConfig.cmake.in
  ${CMAKE_CURRENT_BINARY_DIR}/RavlLogicNLPConfig.cmake
  INSTALL_DESTINATION lib/cmake/RavlLogicNLP
  PATH_VARS)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/RavlLogicNLPConfig.cmake
        DESTINATION lib/cmake/RavlLogicNLP )

install(DIRECTORY include DESTINATION .)



# Examples
set(EXAMPLE_LIBS  RavlLogic RavlCore  )

add_executable(exNonlinearPlanner ${EXCLUDE_EXAMPLES} examples/exNonlinearPlanner.cc )
target_link_libraries(exNonlinearPlanner RavlLogicNLP ${EXAMPLE_LIBS} )
install(TARGETS exNonlinearPlanner DESTINATION examples COMPONENT examples ${EXCLUDE_EXAMPLES})

# Tests
set(TEST_LIBS  RavlLogic RavlCore  )

add_executable(testNonLinearPlanner ${EXCLUDE_UNITTESTS} tests/testNonLinearPlanner.cc )
target_link_libraries(testNonLinearPlanner RavlLogicNLP ${TEST_LIBS} )
add_test(testNonLinearPlanner testNonLinearPlanner)
install(TARGETS testNonLinearPlanner DESTINATION tests COMPONENT unittests ${EXCLUDE_UNITTESTS})






