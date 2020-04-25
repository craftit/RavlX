
# Library: RavlLogic

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

set(HAVE_RavlLogic 1)

include_directories(../../Core/RavlCore/include)
include_directories(../../PatternRec/RavlPatternRec/include)
include_directories(../../Math/RavlMath/include)
include_directories(../../PatternRec/RavlDataSet/include)
include_directories(../../Core/RavlIO/include)




add_library(RavlLogic STATIC include/Ravl/Logic/Literal.hh include/Ravl/Logic/Var.hh include/Ravl/Logic/Unify.hh include/Ravl/Logic/BindSet.hh include/Ravl/Logic/Tuple.hh include/Ravl/Logic/LList.hh include/Ravl/Logic/Condition.hh include/Ravl/Logic/MinTerm.hh include/Ravl/Logic/Or.hh include/Ravl/Logic/And.hh include/Ravl/Logic/Not.hh include/Ravl/Logic/LiteralIter.hh include/Ravl/Logic/LiteralIterWrap.hh include/Ravl/Logic/LiteralMapIter.hh include/Ravl/Logic/LiteralMapListIter.hh include/Ravl/Logic/State.hh include/Ravl/Logic/LiteralIter1.hh include/Ravl/Logic/StateOrIter.hh include/Ravl/Logic/StateSet.hh include/Ravl/Logic/LiteralFilterIter.hh include/Ravl/Logic/StateAndIter.hh include/Ravl/Logic/NamedLiteral.hh include/Ravl/Logic/NamedVar.hh include/Ravl/Logic/LiteralIO.hh include/Ravl/Logic/MinTermIter.hh include/Ravl/Logic/Value.hh include/Ravl/Logic/Context.hh include/Ravl/Logic/LiteralIndexBase.hh include/Ravl/Logic/LiteralIndexElement.hh include/Ravl/Logic/LiteralIndexLeaf.hh include/Ravl/Logic/LiteralIndexNode.hh include/Ravl/Logic/LiteralIndex.hh include/Ravl/Logic/LiteralIndexIter.hh include/Ravl/Logic/LiteralIndexFilterBase.hh include/Ravl/Logic/LiteralIndexFilter.hh include/Ravl/Logic/LiteralIndexLeafIter.hh include/Ravl/Logic/BLiteralIndex.hh include/Ravl/Logic/BMinTermIndex.hh include/Ravl/Logic/BMinTermIndexIter.hh include/Ravl/Logic/BMinTermListIndex.hh include/Ravl/Logic/LiteralIndexLeafVarIter.hh include/Ravl/Logic/StateIndexed.hh include/Ravl/Logic/LiteralIndexFilterRaw.hh include/Ravl/Logic/DecisionTreeElement.hh include/Ravl/Logic/DecisionTreeLeaf.hh include/Ravl/Logic/DecisionTreeBase.hh include/Ravl/Logic/DecisionTree.hh include/Ravl/Logic/DecisionTreeBranch.hh include/Ravl/Logic/DecisionTreeBranchBinary.hh include/Ravl/Logic/DecisionExamples.hh include/Ravl/Logic/Discriminator.hh include/Ravl/Logic/SampleLiteral.hh include/Ravl/Logic/SampleState.hh src/Literal.cc src/Var.cc src/Unify.cc src/BindSet.cc src/Tuple.cc src/LList.cc src/Condition.cc src/MinTerm.cc src/Or.cc src/And.cc src/Not.cc src/LiteralIter.cc src/State.cc src/LiteralIter1.cc src/StateOrIter.cc src/StateSet.cc src/LiteralFilterIter.cc src/StateAndIter.cc src/NamedLiteral.cc src/NamedVar.cc src/LiteralIO.cc src/MinTermIter.cc src/Value.cc src/Context.cc src/LiteralIndexBase.cc src/LiteralIndexElement.cc src/LiteralIndexLeaf.cc src/LiteralIndexNode.cc src/LiteralIndexIter.cc src/LiteralIndexFilterBase.cc src/LiteralIndexLeafIter.cc src/LiteralIndexLeafVarIter.cc src/StateIndexed.cc src/LiteralIndexFilterRaw.cc src/DecisionTreeElement.cc src/DecisionTreeLeaf.cc src/DecisionTreeBase.cc src/DecisionTreeBranch.cc src/DecisionTreeBranchBinary.cc src/DecisionExamples.cc src/Discriminator.cc src/SampleLiteral.cc src/SampleState.cc)
target_link_libraries(RavlLogic PUBLIC RavlCore RavlPatternRec RavlMath RavlDataSet RavlIO )


target_include_directories(
    RavlLogic PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
    )



install(TARGETS RavlLogic EXPORT RavlLogicConfig
   ARCHIVE  DESTINATION lib
   LIBRARY  DESTINATION lib
   RUNTIME  DESTINATION bin)  # This is for Windows

install(EXPORT RavlLogicConfig DESTINATION lib/cmake/RavlLogic/Target)
install(TARGETS RavlLogic DESTINATION lib)

configure_package_config_file(cmake/RavlLogicConfig.cmake.in
  ${CMAKE_CURRENT_BINARY_DIR}/RavlLogicConfig.cmake
  INSTALL_DESTINATION lib/cmake/RavlLogic
  PATH_VARS)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/RavlLogicConfig.cmake
        DESTINATION lib/cmake/RavlLogic )

install(DIRECTORY include DESTINATION .)




# Tests
set(TEST_LIBS  RavlCore RavlPatternRec RavlMath RavlDataSet RavlIO  )

add_executable(testLogic EXCLUDE_FROM_ALL tests/testLogic.cc )
target_link_libraries(testLogic RavlLogic ${TEST_LIBS} )
add_test(testLogic testLogic)
install(TARGETS testLogic DESTINATION tests COMPONENT unittests EXCLUDE_FROM_ALL)

add_executable(testLiteralIndex EXCLUDE_FROM_ALL tests/testLiteralIndex.cc )
target_link_libraries(testLiteralIndex RavlLogic ${TEST_LIBS} )
add_test(testLiteralIndex testLiteralIndex)
install(TARGETS testLiteralIndex DESTINATION tests COMPONENT unittests EXCLUDE_FROM_ALL)

add_executable(testBLiteralIndex EXCLUDE_FROM_ALL tests/testBLiteralIndex.cc )
target_link_libraries(testBLiteralIndex RavlLogic ${TEST_LIBS} )
add_test(testBLiteralIndex testBLiteralIndex)
install(TARGETS testBLiteralIndex DESTINATION tests COMPONENT unittests EXCLUDE_FROM_ALL)

add_executable(testDecisionTree EXCLUDE_FROM_ALL tests/testDecisionTree.cc )
target_link_libraries(testDecisionTree RavlLogic ${TEST_LIBS} )
add_test(testDecisionTree testDecisionTree)
install(TARGETS testDecisionTree DESTINATION tests COMPONENT unittests EXCLUDE_FROM_ALL)






