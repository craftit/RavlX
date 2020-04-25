
# Library: RavlSVM

# Requires: 
# Supports: 




# MustLink: src/linkRavlSVM.cc


set(DEPENDS_ON )
foreach(D IN LISTS DEPENDS_ON)
  if(NOT ${D}_FOUND)
    find_package(${D} REQUIRED)
  endif()
endforeach()

include(CMakePackageConfigHelpers)

include_directories(include)
include_directories(src)

set(HAVE_RavlSVM 1)

include_directories(../../Core/RavlCore/include)
include_directories(../../PatternRec/RavlPatternRec/include)
include_directories(../../PatternRec/RavlDataSet/include)
include_directories(../../Math/RavlMath/include)
include_directories(../../OS/RavlXMLFactory/include)
include_directories(../../OS/RavlThreads/include)
include_directories(../../Core/RavlIO/include)
include_directories(../../PatternRec/ArffIO/include)
include_directories(../../OS/RavlOS/include)
include_directories(../../Image/RavlImage/include)
include_directories(../../Image/RavlImageIO/include)
include_directories(../../Math/RavlMathIO/include)




add_library(RavlSVM_Body STATIC
 include/Ravl/PatternRec/DesignSvm.hh include/Ravl/PatternRec/SvmClassifier.hh include/Ravl/PatternRec/SvmLinearClassifier.hh include/Ravl/PatternRec/SvmQuadraticClassifier.hh include/Ravl/PatternRec/KernelFunc.hh include/Ravl/PatternRec/CommonKernels.hh include/Ravl/PatternRec/DesignClassifierSvmSmo.hh include/Ravl/PatternRec/DesignSvmSmoLarge.hh include/Ravl/PatternRec/OneClass.hh include/Ravl/PatternRec/DesignOneClass.hh include/Ravl/PatternRec/OneClassLinear.hh include/Ravl/PatternRec/DesignOneClassLarge.hh
 src/DesignSvm.cc src/SvmClassifier.cc src/SvmLinearClassifier.cc src/SvmQuadraticClassifier.cc src/KernelFunc.cc src/CommonKernels.cc src/DesignClassifierSvmSmo.cc src/DesignSvmSmoLarge.cc src/OneClass.cc src/DesignOneClass.cc src/OneClassLinear.cc src/DesignOneClassLarge.cc src/SvmVirtualConstr.cc src/SvmVirtualConstr2.cc
)
target_link_libraries(RavlSVM_Body PUBLIC RavlCore RavlPatternRec RavlDataSet RavlMath RavlXMLFactory RavlThreads RavlIO )

add_library(RavlSVM OBJECT src/linkRavlSVM.cc )
target_link_libraries(RavlSVM PUBLIC RavlSVM_Body )


target_include_directories(
    RavlSVM PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
    )

# MustLink fixes:
install(EXPORT RavlSVM_BodyConfig DESTINATION lib/cmake/RavlSVM_Body/Target)
install(TARGETS RavlSVM_Body EXPORT RavlSVM_BodyConfig
   ARCHIVE  DESTINATION lib
   LIBRARY  DESTINATION lib
   RUNTIME  DESTINATION bin)  # This is for Windows
install(TARGETS RavlSVM_Body DESTINATION lib)

configure_package_config_file(cmake/RavlSVM_BodyConfig.cmake.in
  ${CMAKE_CURRENT_BINARY_DIR}/RavlSVM_BodyConfig.cmake
  INSTALL_DESTINATION lib/cmake/RavlSVM_Body
  PATH_VARS)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/RavlSVM_BodyConfig.cmake
        DESTINATION lib/cmake/RavlSVM_Body )


install(TARGETS RavlSVM EXPORT RavlSVMConfig
   ARCHIVE  DESTINATION lib
   LIBRARY  DESTINATION lib
   RUNTIME  DESTINATION bin)  # This is for Windows

install(EXPORT RavlSVMConfig DESTINATION lib/cmake/RavlSVM/Target)
install(TARGETS RavlSVM DESTINATION lib)

configure_package_config_file(cmake/RavlSVMConfig.cmake.in
  ${CMAKE_CURRENT_BINARY_DIR}/RavlSVMConfig.cmake
  INSTALL_DESTINATION lib/cmake/RavlSVM
  PATH_VARS)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/RavlSVMConfig.cmake
        DESTINATION lib/cmake/RavlSVM )

install(DIRECTORY include DESTINATION .)


# Mains
set(MAIN_LIBS  RavlCore RavlPatternRec RavlDataSet RavlMath RavlXMLFactory RavlThreads RavlIO ArffIO RavlOS RavlImage RavlImageIO RavlMathIO  )

add_executable(oneClassTest applications/oneClassTest.cc )
target_link_libraries(oneClassTest RavlSVM ${MAIN_LIBS} )
install(TARGETS oneClassTest DESTINATION bin)

add_executable(oneClassLargeTest applications/oneClassLargeTest.cc )
target_link_libraries(oneClassLargeTest RavlSVM ${MAIN_LIBS} )
install(TARGETS oneClassLargeTest DESTINATION bin)

add_executable(createClassifier applications/createClassifier.cc )
target_link_libraries(createClassifier RavlSVM ${MAIN_LIBS} )
install(TARGETS createClassifier DESTINATION bin)

add_executable(classifyArff applications/classifyArff.cc )
target_link_libraries(classifyArff RavlSVM ${MAIN_LIBS} )
install(TARGETS classifyArff DESTINATION bin)

add_executable(svmTest applications/svmTest.cc )
target_link_libraries(svmTest RavlSVM ${MAIN_LIBS} )
install(TARGETS svmTest DESTINATION bin)

add_executable(svmTest2 applications/svmTest2.cc )
target_link_libraries(svmTest2 RavlSVM ${MAIN_LIBS} )
install(TARGETS svmTest2 DESTINATION bin)

add_executable(svmTest3 applications/svmTest3.cc )
target_link_libraries(svmTest3 RavlSVM ${MAIN_LIBS} )
install(TARGETS svmTest3 DESTINATION bin)

add_executable(svmTest4 applications/svmTest4.cc )
target_link_libraries(svmTest4 RavlSVM ${MAIN_LIBS} )
install(TARGETS svmTest4 DESTINATION bin)

add_executable(svmTest5 applications/svmTest5.cc )
target_link_libraries(svmTest5 RavlSVM ${MAIN_LIBS} )
install(TARGETS svmTest5 DESTINATION bin)

add_executable(classifyList applications/classifyList.cc )
target_link_libraries(classifyList RavlSVM ${MAIN_LIBS} )
install(TARGETS classifyList DESTINATION bin)


# Tests
set(TEST_LIBS  RavlCore RavlPatternRec RavlDataSet RavlMath RavlXMLFactory RavlThreads RavlIO ArffIO RavlOS RavlImage RavlImageIO RavlMathIO  )

add_executable(testSvmXMLFactory EXCLUDE_FROM_ALL tests/testSvmXMLFactory.cc )
target_link_libraries(testSvmXMLFactory RavlSVM ${TEST_LIBS} )
add_test(testSvmXMLFactory testSvmXMLFactory)
install(TARGETS testSvmXMLFactory DESTINATION tests COMPONENT unittests EXCLUDE_FROM_ALL)


# === Files for share/RAVL/testData/svm ===
set(AUXDIR share/RAVL/testData/svm)
set(AUXFILES 
  data/testClsSL32.abs 
  data/testClsSL64.abs )
file(COPY ${AUXFILES} DESTINATION ${BUILD_ROOT_AUXFILE}/${AUXDIR})
install(FILES ${AUXFILES} DESTINATION ${AUXDIR})





