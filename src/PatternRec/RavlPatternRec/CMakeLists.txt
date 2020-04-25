
# Library: RavlPatternRec

# Requires: 
# Supports: 




# MustLink: src/linkDistance.cc src/LinkRavlPatternRecModelBasic.cc src/linkClassifier.cc


set(DEPENDS_ON )
foreach(D IN LISTS DEPENDS_ON)
  if(NOT ${D}_FOUND)
    find_package(${D} REQUIRED)
  endif()
endforeach()

include(CMakePackageConfigHelpers)

include_directories(include)
include_directories(src)

set(HAVE_RavlPatternRec 1)

include_directories(../../Core/RavlCore/include)
include_directories(../../Math/RavlMath/include)
include_directories(../../OS/RavlOSIO/include)
include_directories(../../Core/RavlIO/include)
include_directories(../../PatternRec/fann/include)
include_directories(../../OS/RavlXMLFactory/include)
include_directories(../../Math/RavlMathIO/include)
include_directories(../../PatternRec/RavlDataSet/include)
include_directories(../../PatternRec/Optimisation/include)
include_directories(../../OS/RavlThreads/include)
include_directories(../../Plot/RavlPlot/include)
include_directories(../../Image/RavlImage/include)
include_directories(../../OS/RavlOS/include)
include_directories(../../PatternRec/RavlPatternRecIO/include)
include_directories(../../PatternRec/RavlSVM/include)



if(DEFINED HAVE_RavlGnuPlot)
include_directories(../../Contrib/RavlGnuPlot/include)
endif()
if(DEFINED HAVE_RavlIntelMKL)
include_directories(../../Contrib/RavlIntelMKL/include)
endif()

add_library(RavlPatternRec_Body STATIC
 include/Ravl/PatternRec/Distance.hh include/Ravl/PatternRec/DistanceEuclidean.hh include/Ravl/PatternRec/DistanceMax.hh include/Ravl/PatternRec/DistanceCityBlock.hh include/Ravl/PatternRec/DistanceMahalanobis.hh include/Ravl/PatternRec/DistanceSqrEuclidean.hh include/Ravl/PatternRec/DistanceRobust.hh include/Ravl/PatternRec/DistanceChi2.hh include/Ravl/PatternRec/DistanceCorrelation.hh include/Ravl/PatternRec/Classifier.hh include/Ravl/PatternRec/DesignClassifierUnsupervised.hh include/Ravl/PatternRec/ClassifierNearestNeighbour.hh include/Ravl/PatternRec/DesignKMeans.hh include/Ravl/PatternRec/DesignMeanShiftCluster.hh include/Ravl/PatternRec/DesignCluster.hh include/Ravl/PatternRec/DesignKohonenNet.hh include/Ravl/PatternRec/ClassifierFuzzyCMeans.hh include/Ravl/PatternRec/DesignFuzzyCMeansCluster.hh include/Ravl/PatternRec/FuncLinearCoeff.hh include/Ravl/PatternRec/FuncLinear.hh include/Ravl/PatternRec/FuncQuadratic.hh include/Ravl/PatternRec/FuncOrthPolynomial.hh include/Ravl/PatternRec/DesignFuncLSQ.hh include/Ravl/PatternRec/FuncPDF.hh include/Ravl/PatternRec/FuncPDFNormal.hh include/Ravl/PatternRec/FuncMeanProjection.hh include/Ravl/PatternRec/FuncInterpolate.hh include/Ravl/PatternRec/FuncInterpolateGrid.hh include/Ravl/PatternRec/FuncSubset.hh include/Ravl/PatternRec/DesignFuncReduce.hh include/Ravl/PatternRec/DesignFuncPCA.hh include/Ravl/PatternRec/DesignFuncLDA.hh include/Ravl/PatternRec/GaussianMixture.hh include/Ravl/PatternRec/DesignGaussianMixture.hh include/Ravl/PatternRec/DesignClassifierSupervised.hh include/Ravl/PatternRec/DesignDiscriminantFunction.hh include/Ravl/PatternRec/DesignKNearestNeighbour.hh include/Ravl/PatternRec/ClassifierKNearestNeighbour.hh include/Ravl/PatternRec/ClassifierAverageNearestNeighbour.hh include/Ravl/PatternRec/ClassifierDiscriminantFunction.hh include/Ravl/PatternRec/ClassifierGaussianMixture.hh include/Ravl/PatternRec/DesignClassifierGaussianMixture.hh include/Ravl/PatternRec/ClassifierPreprocess.hh include/Ravl/PatternRec/ClassifierFunc1Threshold.hh include/Ravl/PatternRec/ClassifierWeakLinear.hh include/Ravl/PatternRec/ClassifierLinearCombination.hh include/Ravl/PatternRec/DesignWeakLinear.hh include/Ravl/PatternRec/ClassifierBayesNormalLinear.hh include/Ravl/PatternRec/DesignBayesNormalLinear.hh include/Ravl/PatternRec/DesignBayesNormalQuadratic.hh include/Ravl/PatternRec/ClassifierBayesNormalQuadratic.hh include/Ravl/PatternRec/ClassifierNeuralNetwork.hh include/Ravl/PatternRec/DesignClassifierNeuralNetwork.hh include/Ravl/PatternRec/Classifier2.hh include/Ravl/PatternRec/ClassifierOneAgainstAll.hh include/Ravl/PatternRec/DesignOneAgainstAll.hh include/Ravl/PatternRec/DesignClassifierLogisticRegression.hh include/Ravl/PatternRec/ClassifierLogisticRegression.hh include/Ravl/PatternRec/ClassifierNeuralNetwork2.hh include/Ravl/PatternRec/DesignClassifierNeuralNetwork2.hh include/Ravl/PatternRec/DesignCascade.hh include/Ravl/PatternRec/ClassifierCascade.hh include/Ravl/PatternRec/DesignClassifierSupervisedWithValidation.hh include/Ravl/PatternRec/DesignCascadeBinary.hh include/Ravl/PatternRec/ClassifierFixed.hh include/Ravl/PatternRec/CostLogisticRegression.hh include/Ravl/PatternRec/FeatureSelector.hh include/Ravl/PatternRec/FeatureSelectAsymmetricAdaBoost.hh include/Ravl/PatternRec/FeatureSelectPlusLMinusR.hh include/Ravl/PatternRec/Error.hh include/Ravl/PatternRec/OptimiseClassifierDesign.hh include/Ravl/PatternRec/ErrorBinaryClassifier.hh
 src/Distance.cc src/DistanceEuclidean.cc src/DistanceMax.cc src/DistanceCityBlock.cc src/DistanceMahalanobis.cc src/DistanceSqrEuclidean.cc src/DistanceRobust.cc src/DistanceChi2.cc src/DistanceCorrelation.cc src/Classifier.cc src/DesignClassifierUnsupervised.cc src/ClassifierNearestNeighbour.cc src/DesignKMeans.cc src/DesignMeanShiftCluster.cc src/DesignCluster.cc src/DesignKohonenNet.cc src/ClassifierFuzzyCMeans.cc src/DesignFuzzyCMeansCluster.cc src/FuncLinearCoeff.cc src/FuncLinear.cc src/FuncQuadratic.cc src/FuncOrthPolynomial.cc src/DesignFuncLSQ.cc src/FuncPDF.cc src/FuncPDFNormal.cc src/FuncMeanProjection.cc src/FuncInterpolate.cc src/FuncInterpolateGrid.cc src/SampleVectorFunc.cc src/FuncSubset.cc src/DesignFuncReduce.cc src/DesignFuncPCA.cc src/DesignFuncLDA.cc src/GaussianMixture.cc src/DesignGaussianMixture.cc src/DesignClassifierSupervised.cc src/DesignDiscriminantFunction.cc src/DesignKNearestNeighbour.cc src/ClassifierKNearestNeighbour.cc src/ClassifierAverageNearestNeighbour.cc src/ClassifierDiscriminantFunction.cc src/ClassifierGaussianMixture.cc src/DesignClassifierGaussianMixture.cc src/ClassifierPreprocess.cc src/ClassifierFunc1Threshold.cc src/ClassifierWeakLinear.cc src/ClassifierLinearCombination.cc src/DesignWeakLinear.cc src/ClassifierBayesNormalLinear.cc src/DesignBayesNormalLinear.cc src/DesignBayesNormalQuadratic.cc src/ClassifierBayesNormalQuadratic.cc src/ClassifierNeuralNetwork.cc src/DesignClassifierNeuralNetwork.cc src/Classifier2.cc src/ClassifierOneAgainstAll.cc src/DesignOneAgainstAll.cc src/DesignClassifierLogisticRegression.cc src/ClassifierLogisticRegression.cc src/ClassifierNeuralNetwork2.cc src/DesignClassifierNeuralNetwork2.cc src/DesignCascade.cc src/ClassifierCascade.cc src/DesignClassifierSupervisedWithValidation.cc src/DesignCascadeBinary.cc src/ClassifierFixed.cc src/CostLogisticRegression.cc src/FeatureSelector.cc src/FeatureSelectAsymmetricAdaBoost.cc src/FeatureSelectPlusLMinusR.cc src/Error.cc src/OptimiseClassifierDesign.cc src/ErrorBinaryClassifier.cc
)
target_link_libraries(RavlPatternRec_Body PUBLIC RavlCore RavlMath RavlOSIO RavlIO fann RavlXMLFactory RavlMathIO RavlDataSet Optimisation RavlThreads RavlPlot RavlImage RavlOS )

add_library(RavlPatternRec OBJECT src/linkDistance.cc src/LinkRavlPatternRecModelBasic.cc src/linkClassifier.cc )
target_link_libraries(RavlPatternRec PUBLIC RavlPatternRec_Body )


target_include_directories(
    RavlPatternRec PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
    )

# MustLink fixes:
install(EXPORT RavlPatternRec_BodyConfig DESTINATION lib/cmake/RavlPatternRec_Body/Target)
install(TARGETS RavlPatternRec_Body EXPORT RavlPatternRec_BodyConfig
   ARCHIVE  DESTINATION lib
   LIBRARY  DESTINATION lib
   RUNTIME  DESTINATION bin)  # This is for Windows
install(TARGETS RavlPatternRec_Body DESTINATION lib)

configure_package_config_file(cmake/RavlPatternRec_BodyConfig.cmake.in
  ${CMAKE_CURRENT_BINARY_DIR}/RavlPatternRec_BodyConfig.cmake
  INSTALL_DESTINATION lib/cmake/RavlPatternRec_Body
  PATH_VARS)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/RavlPatternRec_BodyConfig.cmake
        DESTINATION lib/cmake/RavlPatternRec_Body )


install(TARGETS RavlPatternRec EXPORT RavlPatternRecConfig
   ARCHIVE  DESTINATION lib
   LIBRARY  DESTINATION lib
   RUNTIME  DESTINATION bin)  # This is for Windows

install(EXPORT RavlPatternRecConfig DESTINATION lib/cmake/RavlPatternRec/Target)
install(TARGETS RavlPatternRec DESTINATION lib)

configure_package_config_file(cmake/RavlPatternRecConfig.cmake.in
  ${CMAKE_CURRENT_BINARY_DIR}/RavlPatternRecConfig.cmake
  INSTALL_DESTINATION lib/cmake/RavlPatternRec
  PATH_VARS)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/RavlPatternRecConfig.cmake
        DESTINATION lib/cmake/RavlPatternRec )

install(DIRECTORY include DESTINATION .)


# Mains
set(MAIN_LIBS  RavlCore RavlMath RavlOSIO RavlIO fann RavlXMLFactory RavlMathIO RavlDataSet Optimisation RavlThreads RavlPlot RavlImage RavlOS RavlPatternRecIO RavlSVM  )

add_executable(doCluster applications/doCluster.cc )
target_link_libraries(doCluster RavlPatternRec ${MAIN_LIBS} )
install(TARGETS doCluster DESTINATION bin)

add_executable(doPolyFit applications/doPolyFit.cc )
target_link_libraries(doPolyFit RavlPatternRec ${MAIN_LIBS} )
install(TARGETS doPolyFit DESTINATION bin)

add_executable(doDimensionReduction applications/doDimensionReduction.cc )
target_link_libraries(doDimensionReduction RavlPatternRec ${MAIN_LIBS} )
install(TARGETS doDimensionReduction DESTINATION bin)

add_executable(doLeaveOneOut applications/doLeaveOneOut.cc )
target_link_libraries(doLeaveOneOut RavlPatternRec ${MAIN_LIBS} )
install(TARGETS doLeaveOneOut DESTINATION bin)

add_executable(doTrainClassifier applications/doTrainClassifier.cc )
target_link_libraries(doTrainClassifier RavlPatternRec ${MAIN_LIBS} )
install(TARGETS doTrainClassifier DESTINATION bin)

add_executable(doTestClassifier applications/doTestClassifier.cc )
target_link_libraries(doTestClassifier RavlPatternRec ${MAIN_LIBS} )
install(TARGETS doTestClassifier DESTINATION bin)

add_executable(doTrainCascadeClassifier applications/doTrainCascadeClassifier.cc )
target_link_libraries(doTrainCascadeClassifier RavlPatternRec ${MAIN_LIBS} )
install(TARGETS doTrainCascadeClassifier DESTINATION bin)

add_executable(doClassify applications/doClassify.cc )
target_link_libraries(doClassify RavlPatternRec ${MAIN_LIBS} )
install(TARGETS doClassify DESTINATION bin)

add_executable(doAsymmetricAdaBoost applications/doAsymmetricAdaBoost.cc )
target_link_libraries(doAsymmetricAdaBoost RavlPatternRec ${MAIN_LIBS} )
install(TARGETS doAsymmetricAdaBoost DESTINATION bin)

# Examples
set(EXAMPLE_LIBS  RavlCore RavlMath RavlOSIO RavlIO fann RavlXMLFactory RavlMathIO RavlDataSet Optimisation RavlThreads RavlPlot RavlImage RavlOS RavlPatternRecIO RavlSVM  )

add_executable(exKMeansCluster EXCLUDE_FROM_ALL examples/exKMeansCluster.cc )
target_link_libraries(exKMeansCluster RavlPatternRec ${EXAMPLE_LIBS} )
install(TARGETS exKMeansCluster DESTINATION examples COMPONENT examples EXCLUDE_FROM_ALL)

add_executable(exFuzzyCMeansCluster EXCLUDE_FROM_ALL examples/exFuzzyCMeansCluster.cc )
target_link_libraries(exFuzzyCMeansCluster RavlPatternRec ${EXAMPLE_LIBS} )
install(TARGETS exFuzzyCMeansCluster DESTINATION examples COMPONENT examples EXCLUDE_FROM_ALL)

add_executable(exDesignMeanShiftCluster EXCLUDE_FROM_ALL examples/exDesignMeanShiftCluster.cc )
target_link_libraries(exDesignMeanShiftCluster RavlPatternRec ${EXAMPLE_LIBS} )
install(TARGETS exDesignMeanShiftCluster DESTINATION examples COMPONENT examples EXCLUDE_FROM_ALL)

add_executable(exDesignGaussianMixture EXCLUDE_FROM_ALL examples/exDesignGaussianMixture.cc )
target_link_libraries(exDesignGaussianMixture RavlPatternRec ${EXAMPLE_LIBS} )
install(TARGETS exDesignGaussianMixture DESTINATION examples COMPONENT examples EXCLUDE_FROM_ALL)

add_executable(exKNearestNeighbour EXCLUDE_FROM_ALL examples/exKNearestNeighbour.cc )
target_link_libraries(exKNearestNeighbour RavlPatternRec ${EXAMPLE_LIBS} )
install(TARGETS exKNearestNeighbour DESTINATION examples COMPONENT examples EXCLUDE_FROM_ALL)

# Tests
set(TEST_LIBS  RavlCore RavlMath RavlOSIO RavlIO fann RavlXMLFactory RavlMathIO RavlDataSet Optimisation RavlThreads RavlPlot RavlImage RavlOS RavlPatternRecIO RavlSVM  )

add_executable(testMeasure EXCLUDE_FROM_ALL tests/testMeasure.cc )
target_link_libraries(testMeasure RavlPatternRec ${TEST_LIBS} )
add_test(testMeasure testMeasure)
install(TARGETS testMeasure DESTINATION tests COMPONENT unittests EXCLUDE_FROM_ALL)

add_executable(testDistanceXMLFactory EXCLUDE_FROM_ALL tests/testDistanceXMLFactory.cc )
target_link_libraries(testDistanceXMLFactory RavlPatternRec ${TEST_LIBS} )
add_test(testDistanceXMLFactory testDistanceXMLFactory)
install(TARGETS testDistanceXMLFactory DESTINATION tests COMPONENT unittests EXCLUDE_FROM_ALL)

add_executable(testPatternRecCluster EXCLUDE_FROM_ALL tests/testPatternRecCluster.cc )
target_link_libraries(testPatternRecCluster RavlPatternRec ${TEST_LIBS} )
add_test(testPatternRecCluster testPatternRecCluster)
install(TARGETS testPatternRecCluster DESTINATION tests COMPONENT unittests EXCLUDE_FROM_ALL)

add_executable(testBasicModels EXCLUDE_FROM_ALL tests/testBasicModels.cc )
target_link_libraries(testBasicModels RavlPatternRec ${TEST_LIBS} )
add_test(testBasicModels testBasicModels)
install(TARGETS testBasicModels DESTINATION tests COMPONENT unittests EXCLUDE_FROM_ALL)

add_executable(testGaussianMixture EXCLUDE_FROM_ALL tests/testGaussianMixture.cc )
target_link_libraries(testGaussianMixture RavlPatternRec ${TEST_LIBS} )
add_test(testGaussianMixture testGaussianMixture)
install(TARGETS testGaussianMixture DESTINATION tests COMPONENT unittests EXCLUDE_FROM_ALL)

add_executable(testClassifier EXCLUDE_FROM_ALL tests/testClassifier.cc )
target_link_libraries(testClassifier RavlPatternRec ${TEST_LIBS} )
add_test(testClassifier testClassifier)
install(TARGETS testClassifier DESTINATION tests COMPONENT unittests EXCLUDE_FROM_ALL)

add_executable(testClassifierXMLFactory EXCLUDE_FROM_ALL tests/testClassifierXMLFactory.cc )
target_link_libraries(testClassifierXMLFactory RavlPatternRec ${TEST_LIBS} )
add_test(testClassifierXMLFactory testClassifierXMLFactory)
install(TARGETS testClassifierXMLFactory DESTINATION tests COMPONENT unittests EXCLUDE_FROM_ALL)

add_executable(testClassifierOneAgainstAll EXCLUDE_FROM_ALL tests/testClassifierOneAgainstAll.cc )
target_link_libraries(testClassifierOneAgainstAll RavlPatternRec ${TEST_LIBS} )
add_test(testClassifierOneAgainstAll testClassifierOneAgainstAll)
install(TARGETS testClassifierOneAgainstAll DESTINATION tests COMPONENT unittests EXCLUDE_FROM_ALL)


# === Files for share/Ravl/PatternRec ===
set(AUXDIR share/Ravl/PatternRec)
set(AUXFILES 
  data/classifier.xml )
file(COPY ${AUXFILES} DESTINATION ${BUILD_ROOT_AUXFILE}/${AUXDIR})
install(FILES ${AUXFILES} DESTINATION ${AUXDIR})

# === Files for share/RAVL/data ===
set(AUXDIR share/RAVL/data)
set(AUXFILES 
  data/class5.strm )
file(COPY ${AUXFILES} DESTINATION ${BUILD_ROOT_AUXFILE}/${AUXDIR})
install(FILES ${AUXFILES} DESTINATION ${AUXDIR})





