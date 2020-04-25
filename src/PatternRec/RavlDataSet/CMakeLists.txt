
# Library: RavlDataSet

# Requires: 
# Supports: 




# MustLink: src/linkDataSet.cc


set(DEPENDS_ON )
foreach(D IN LISTS DEPENDS_ON)
  if(NOT ${D}_FOUND)
    find_package(${D} REQUIRED)
  endif()
endforeach()

include(CMakePackageConfigHelpers)

include_directories(include)
include_directories(src)

set(HAVE_RavlDataSet 1)

include_directories(../../Core/RavlCore/include)
include_directories(../../Math/RavlMath/include)
include_directories(../../OS/RavlOSIO/include)
include_directories(../../Core/RavlIO/include)
include_directories(../../PatternRec/fann/include)
include_directories(../../OS/RavlXMLFactory/include)
include_directories(../../Math/RavlMathIO/include)
include_directories(../../OS/RavlThreads/include)
include_directories(../../OS/RavlOS/include)
include_directories(../../PatternRec/RavlPatternRecIO/include)
include_directories(../../PatternRec/RavlPatternRec/include)
include_directories(../../Plot/RavlPlot/include)
include_directories(../../Image/RavlImage/include)



if(DEFINED HAVE_RavlGnuPlot)
include_directories(../../Contrib/RavlGnuPlot/include)
endif()

add_library(RavlDataSet_Body STATIC
 include/Ravl/PatternRec/Sample.hh include/Ravl/PatternRec/SampleIter.hh include/Ravl/PatternRec/SampleVector.hh include/Ravl/PatternRec/SampleLabel.hh include/Ravl/PatternRec/SampleDiscrete.hh include/Ravl/PatternRec/DataSetBase.hh include/Ravl/PatternRec/DataSet1.hh include/Ravl/PatternRec/DataSet1Iter.hh include/Ravl/PatternRec/DataSet2.hh include/Ravl/PatternRec/DataSet2Iter.hh include/Ravl/PatternRec/DataSet3.hh include/Ravl/PatternRec/DataSet3Iter.hh include/Ravl/PatternRec/DataSet4.hh include/Ravl/PatternRec/DataSet4Iter.hh include/Ravl/PatternRec/SampleIO.hh include/Ravl/PatternRec/Function.hh include/Ravl/PatternRec/Function1.hh include/Ravl/PatternRec/Designer.hh include/Ravl/PatternRec/DesignFunctionUnsupervised.hh include/Ravl/PatternRec/DesignFunctionSupervised.hh include/Ravl/PatternRec/VectorLabelIO.hh include/Ravl/PatternRec/DataSetVectorLabel.hh include/Ravl/PatternRec/DataSetVectorLabelWeight.hh include/Ravl/PatternRec/SampleStream.hh include/Ravl/PatternRec/SampleStreamVector.hh include/Ravl/PatternRec/SampleVectorFloat.hh include/Ravl/PatternRec/SampleStreamVectorFloat.hh include/Ravl/PatternRec/SampleReal.hh include/Ravl/PatternRec/SampleStreamFromSample.hh include/Ravl/PatternRec/ProcessVectorFunction.hh include/Ravl/PatternRec/ProcessVectorLabelFunction.hh include/Ravl/PatternRec/FunctionSlice.hh include/Ravl/PatternRec/FunctionConcatenate.hh include/Ravl/PatternRec/FunctionCascade.hh include/Ravl/PatternRec/SampleStreamVectorLabel.hh include/Ravl/PatternRec/DataSetIO.hh include/Ravl/PatternRec/FieldInfo.hh include/Ravl/PatternRec/TestFunctions.hh include/Ravl/PatternRec/FunctionExpand.hh include/Ravl/PatternRec/DPProcessFunction.hh
 src/SampleVector.cc src/SampleLabel.cc src/DataSetBase.cc src/Function.cc src/Function1.cc src/Designer.cc src/DesignFunctionUnsupervised.cc src/DesignFunctionSupervised.cc src/VectorLabelIO.cc src/DataSetVectorLabel.cc src/SampleStreamVector.cc src/DataSetVectorLabelWeight.cc src/SampleVectorFloat.cc src/SampleStreamVectorFloat.cc src/SampleReal.cc src/FunctionSlice.cc src/FunctionConcatenate.cc src/FunctionCascade.cc src/SampleStreamVectorLabel.cc src/SampleIO.cc src/DataSetIO.cc src/FieldInfo.cc src/TestFunctions.cc src/FunctionExpand.cc src/DPProcessFunction.cc
)
target_link_libraries(RavlDataSet_Body PUBLIC RavlCore RavlMath RavlOSIO RavlIO fann RavlXMLFactory RavlMathIO RavlThreads )

add_library(RavlDataSet OBJECT src/linkDataSet.cc )
target_link_libraries(RavlDataSet PUBLIC RavlDataSet_Body )


target_include_directories(
    RavlDataSet PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
    )

# MustLink fixes:
install(EXPORT RavlDataSet_BodyConfig DESTINATION lib/cmake/RavlDataSet_Body/Target)
install(TARGETS RavlDataSet_Body EXPORT RavlDataSet_BodyConfig
   ARCHIVE  DESTINATION lib
   LIBRARY  DESTINATION lib
   RUNTIME  DESTINATION bin)  # This is for Windows
install(TARGETS RavlDataSet_Body DESTINATION lib)

configure_package_config_file(cmake/RavlDataSet_BodyConfig.cmake.in
  ${CMAKE_CURRENT_BINARY_DIR}/RavlDataSet_BodyConfig.cmake
  INSTALL_DESTINATION lib/cmake/RavlDataSet_Body
  PATH_VARS)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/RavlDataSet_BodyConfig.cmake
        DESTINATION lib/cmake/RavlDataSet_Body )


install(TARGETS RavlDataSet EXPORT RavlDataSetConfig
   ARCHIVE  DESTINATION lib
   LIBRARY  DESTINATION lib
   RUNTIME  DESTINATION bin)  # This is for Windows

install(EXPORT RavlDataSetConfig DESTINATION lib/cmake/RavlDataSet/Target)
install(TARGETS RavlDataSet DESTINATION lib)

configure_package_config_file(cmake/RavlDataSetConfig.cmake.in
  ${CMAKE_CURRENT_BINARY_DIR}/RavlDataSetConfig.cmake
  INSTALL_DESTINATION lib/cmake/RavlDataSet
  PATH_VARS)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/RavlDataSetConfig.cmake
        DESTINATION lib/cmake/RavlDataSet )

install(DIRECTORY include DESTINATION .)


# Mains
set(MAIN_LIBS  RavlCore RavlMath RavlOSIO RavlIO fann RavlXMLFactory RavlMathIO RavlThreads RavlOS RavlPatternRecIO RavlPatternRec RavlPlot RavlImage  )

add_executable(doDataSet ${EXCLUDE_APPLICATIONS} applications/doDataSet.cc )
target_link_libraries(doDataSet RavlDataSet ${MAIN_LIBS} )
install(TARGETS doDataSet DESTINATION bin ${EXCLUDE_APPLICATIONS})

add_executable(exPlotTestFunctions ${EXCLUDE_APPLICATIONS} applications/exPlotTestFunctions.cc )
target_link_libraries(exPlotTestFunctions RavlDataSet ${MAIN_LIBS} )
install(TARGETS exPlotTestFunctions DESTINATION bin ${EXCLUDE_APPLICATIONS})

# Examples
set(EXAMPLE_LIBS  RavlCore RavlMath RavlOSIO RavlIO fann RavlXMLFactory RavlMathIO RavlThreads RavlOS RavlPatternRecIO RavlPatternRec RavlPlot RavlImage  )

add_executable(exSample ${EXCLUDE_EXAMPLES} examples/exSample.cc )
target_link_libraries(exSample RavlDataSet ${EXAMPLE_LIBS} )
install(TARGETS exSample DESTINATION examples COMPONENT examples ${EXCLUDE_EXAMPLES})

add_executable(exSampleStream ${EXCLUDE_EXAMPLES} examples/exSampleStream.cc )
target_link_libraries(exSampleStream RavlDataSet ${EXAMPLE_LIBS} )
install(TARGETS exSampleStream DESTINATION examples COMPONENT examples ${EXCLUDE_EXAMPLES})

# Tests
set(TEST_LIBS  RavlCore RavlMath RavlOSIO RavlIO fann RavlXMLFactory RavlMathIO RavlThreads RavlOS RavlPatternRecIO RavlPatternRec RavlPlot RavlImage  )

add_executable(testDataSet ${EXCLUDE_UNITTESTS} tests/testDataSet.cc )
target_link_libraries(testDataSet RavlDataSet ${TEST_LIBS} )
add_test(testDataSet testDataSet)
install(TARGETS testDataSet DESTINATION tests COMPONENT unittests ${EXCLUDE_UNITTESTS})

add_executable(testDataSetXMLFactory ${EXCLUDE_UNITTESTS} tests/testDataSetXMLFactory.cc )
target_link_libraries(testDataSetXMLFactory RavlDataSet ${TEST_LIBS} )
add_test(testDataSetXMLFactory testDataSetXMLFactory)
install(TARGETS testDataSetXMLFactory DESTINATION tests COMPONENT unittests ${EXCLUDE_UNITTESTS})






