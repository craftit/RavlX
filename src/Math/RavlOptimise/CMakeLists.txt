
# Library: RavlOptimise

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

set(HAVE_RavlOptimise 1)

include_directories(../../Core/RavlCore/include)
include_directories(../../Math/RavlMath/include)




add_library(RavlOptimise STATIC include/Ravl/ObsVector.hh include/Ravl/ObsVectorBiGaussian.hh include/Ravl/ObsVectorPoint2dPair.hh include/Ravl/StateVector.hh include/Ravl/StateVectorLine2d.hh include/Ravl/StateVectorQuadratic.hh include/Ravl/StateVectorHomog2d.hh include/Ravl/StateVectorRot3d.hh include/Ravl/Observation.hh include/Ravl/ObservationExplicit.hh include/Ravl/ObservationQuadraticPoint.hh include/Ravl/ObservationImplicit.hh include/Ravl/ObservationLine2dPoint.hh include/Ravl/ObservationImpQuadraticPoint.hh include/Ravl/ObservationHomog2dPoint.hh include/Ravl/ObservationImpHomog2dPoint.hh include/Ravl/StoredState.hh include/Ravl/LevenbergMarquardt.hh include/Ravl/Ransac.hh include/Ravl/ObservationManager.hh include/Ravl/FitToSample.hh include/Ravl/EvaluateSolution.hh include/Ravl/EvaluateNumInliers.hh include/Ravl/EvaluateLikelihood.hh include/Ravl/FitLine2dPoints.hh include/Ravl/FitQuadraticPoints.hh include/Ravl/FitHomog2dPoints.hh include/Ravl/ObservationAffine2dPoint.hh include/Ravl/StateVectorAffine2d.hh include/Ravl/FitAffine2dPoints.hh include/Ravl/OptimiseQuadraticCurve.hh include/Ravl/OptimiseOrthogonalRegression.hh include/Ravl/Optimise2dHomography.hh include/Ravl/Optimise2dAffineHomography.hh include/Ravl/Point2dObs.hh include/Ravl/Point2dPairObs.hh src/ObsVector.cc src/ObsVectorBiGaussian.cc src/ObsVectorPoint2dPair.cc src/StateVector.cc src/StateVectorLine2d.cc src/StateVectorQuadratic.cc src/StateVectorHomog2d.cc src/StateVectorRot3d.cc src/Observation.cc src/ObservationExplicit.cc src/ObservationQuadraticPoint.cc src/ObservationImplicit.cc src/ObservationLine2dPoint.cc src/ObservationImpQuadraticPoint.cc src/ObservationHomog2dPoint.cc src/ObservationImpHomog2dPoint.cc src/StoredState.cc src/LevenbergMarquardt.cc src/Ransac.cc src/ObservationManager.cc src/FitToSample.cc src/EvaluateSolution.cc src/EvaluateNumInliers.cc src/EvaluateLikelihood.cc src/FitLine2dPoints.cc src/FitQuadraticPoints.cc src/FitHomog2dPoints.cc src/ObservationAffine2dPoint.cc src/StateVectorAffine2d.cc src/FitAffine2dPoints.cc src/OptimiseQuadraticCurve.cc src/OptimiseOrthogonalRegression.cc src/Optimise2dHomography.cc src/Optimise2dAffineHomography.cc)
target_link_libraries(RavlOptimise PUBLIC RavlCore RavlMath )


target_include_directories(
    RavlOptimise PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
    )



install(TARGETS RavlOptimise EXPORT RavlOptimiseConfig
   ARCHIVE  DESTINATION lib
   LIBRARY  DESTINATION lib
   RUNTIME  DESTINATION bin)  # This is for Windows

install(EXPORT RavlOptimiseConfig DESTINATION lib/cmake/RavlOptimise/Target)
install(TARGETS RavlOptimise DESTINATION lib)

configure_package_config_file(cmake/RavlOptimiseConfig.cmake.in
  ${CMAKE_CURRENT_BINARY_DIR}/RavlOptimiseConfig.cmake
  INSTALL_DESTINATION lib/cmake/RavlOptimise
  PATH_VARS)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/RavlOptimiseConfig.cmake
        DESTINATION lib/cmake/RavlOptimise )

install(DIRECTORY include DESTINATION .)




# Tests
set(TEST_LIBS  RavlCore RavlMath  )

add_executable(OrthogonalRegressionTest ${EXCLUDE_UNITTESTS} tests/OrthogonalRegressionTest.cc )
target_link_libraries(OrthogonalRegressionTest RavlOptimise ${TEST_LIBS} )
add_test(OrthogonalRegressionTest OrthogonalRegressionTest)
install(TARGETS OrthogonalRegressionTest DESTINATION tests COMPONENT unittests ${EXCLUDE_UNITTESTS})

add_executable(QuadraticFitTest ${EXCLUDE_UNITTESTS} tests/QuadraticFitTest.cc )
target_link_libraries(QuadraticFitTest RavlOptimise ${TEST_LIBS} )
add_test(QuadraticFitTest QuadraticFitTest)
install(TARGETS QuadraticFitTest DESTINATION tests COMPONENT unittests ${EXCLUDE_UNITTESTS})

add_executable(Homography2dFitTest ${EXCLUDE_UNITTESTS} tests/Homography2dFitTest.cc )
target_link_libraries(Homography2dFitTest RavlOptimise ${TEST_LIBS} )
add_test(Homography2dFitTest Homography2dFitTest)
install(TARGETS Homography2dFitTest DESTINATION tests COMPONENT unittests ${EXCLUDE_UNITTESTS})

add_executable(Affine2dFitTest ${EXCLUDE_UNITTESTS} tests/Affine2dFitTest.cc )
target_link_libraries(Affine2dFitTest RavlOptimise ${TEST_LIBS} )
add_test(Affine2dFitTest Affine2dFitTest)
install(TARGETS Affine2dFitTest DESTINATION tests COMPONENT unittests ${EXCLUDE_UNITTESTS})






