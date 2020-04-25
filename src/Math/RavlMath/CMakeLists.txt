
# Library: RavlMath

# Requires: 
# Supports: 




# MustLink: src/LinkRavlSignal1.cc


set(DEPENDS_ON )
foreach(D IN LISTS DEPENDS_ON)
  if(NOT ${D}_FOUND)
    find_package(${D} REQUIRED)
  endif()
endforeach()

include(CMakePackageConfigHelpers)

include_directories(include)
include_directories(src)

set(HAVE_RavlMath 1)

include_directories(../../Core/RavlCore/include)
include_directories(../../CCMath/ccmath/include)
include_directories(../../Core/RavlIO/include)
include_directories(../../OS/RavlOSIO/include)
include_directories(../../OS/RavlXMLFactory/include)
include_directories(../../OS/RavlOS/include)
include_directories(../../Image/RavlImage/include)
include_directories(../../Image/RavlImageIO/include)
include_directories(../../OS/RavlThreads/include)



if(DEFINED HAVE_RavlI386SSE)
include_directories(../../Math/RavlI386SSE/include)
endif()
if(DEFINED HAVE_RavlDPDisplay)
include_directories(../../GUI/RavlDPDisplay/include)
endif()

add_library(RavlMath_Body STATIC
 include/Ravl/CCMath.hh include/Ravl/TVector.hh include/Ravl/TMatrix.hh include/Ravl/Matrix.hh include/Ravl/Vector.hh include/Ravl/MatrixRUT.hh include/Ravl/MatrixRS.hh include/Ravl/LeastSquares.hh include/Ravl/VectorMatrix.hh include/Ravl/MatrixDecomposition.hh include/Ravl/TSMatrix.hh include/Ravl/TSMatrixFull.hh include/Ravl/TSMatrixPartial.hh include/Ravl/TSMatrixDiagonal.hh include/Ravl/TSMatrixRightUpper.hh include/Ravl/TSMatrixLeftLower.hh include/Ravl/TSMatrixSparse.hh include/Ravl/TSMatrixScaledIdentity.hh include/Ravl/TSMatrixSymmetric.hh include/Ravl/TSMatrixTranspose.hh include/Ravl/SMatrix.hh include/Ravl/SMatrixSparse.hh include/Ravl/SMatrixSymmetric.hh include/Ravl/SMatrixDiagonal.hh include/Ravl/SMatrixLeftLower.hh include/Ravl/SMatrixRightUpper.hh include/Ravl/SMatrixScaledIdentity.hh include/Ravl/SVD.hh include/Ravl/RawMatrix.hh include/Ravl/Eigen.hh include/Ravl/TSMatrixSparseIter.hh include/Ravl/LAHooks.hh include/Ravl/FVector.hh include/Ravl/Vector2d.hh include/Ravl/Vector3d.hh include/Ravl/Vector4d.hh include/Ravl/FMatrix.hh include/Ravl/Matrix2d.hh include/Ravl/Matrix3d.hh include/Ravl/Matrix4d.hh include/Ravl/FVectorMatrix.hh include/Ravl/VectorMatrix2d.hh include/Ravl/VectorMatrix3d.hh include/Ravl/VectorMatrix4d.hh include/Ravl/TFMatrixDecomposition.hh include/Ravl/FPoint.hh include/Ravl/FLinePP.hh include/Ravl/Point4d.hh include/Ravl/FAffine.hh include/Ravl/LineABC2d.hh include/Ravl/Curve2d.hh include/Ravl/Curve2dLine.hh include/Ravl/Curve2dLineSegment.hh include/Ravl/Line2dIter.hh include/Ravl/Affine2d.hh include/Ravl/Moments2d2.hh include/Ravl/LinePP2d.hh include/Ravl/Projection2d.hh include/Ravl/Point2d.hh include/Ravl/PointSet2d.hh include/Ravl/Polygon2d.hh include/Ravl/Circle2d.hh include/Ravl/Arc2d.hh include/Ravl/Curve2dCircle.hh include/Ravl/Curve2dArc.hh include/Ravl/HEMesh2d.hh include/Ravl/DelaunayTriangulation2d.hh include/Ravl/TriMesh2d.hh include/Ravl/Conic2d.hh include/Ravl/Ellipse2d.hh include/Ravl/ScanPolygon2d.hh include/Ravl/PolyLine2d.hh include/Ravl/Triangulate2d.hh include/Ravl/LinePP3d.hh include/Ravl/LinePV3d.hh include/Ravl/PlaneABCD3d.hh include/Ravl/PlanePVV3d.hh include/Ravl/Point3d.hh include/Ravl/Quatern3d.hh include/Ravl/RigidTransform3d.hh include/Ravl/EulerAngle.hh include/Ravl/AxisAngle.hh include/Ravl/PointSet3d.hh include/Ravl/HEMesh3d.hh include/Ravl/Affine3d.hh include/Ravl/GPSCoordinate.hh include/Ravl/CrackCode.hh include/Ravl/BVertex.hh include/Ravl/Crack.hh include/Ravl/Boundary.hh include/Ravl/Edge.hh include/Ravl/PPoint1d.hh include/Ravl/PProjection1d.hh include/Ravl/PPointLine2d.hh include/Ravl/PPoint2d.hh include/Ravl/PLine2d.hh include/Ravl/PProjection2d.hh include/Ravl/PPerspective2d.hh include/Ravl/FundamentalMatrix2d.hh include/Ravl/PPointPlane3d.hh include/Ravl/PPoint3d.hh include/Ravl/PPlane3d.hh include/Ravl/PProjection3d.hh include/Ravl/PPerspective3d.hh include/Ravl/PLinePV3d.hh include/Ravl/MeanVariance.hh include/Ravl/MeanNd.hh include/Ravl/MeanCovariance.hh include/Ravl/Statistics.hh include/Ravl/Mean.hh include/Ravl/FMean.hh include/Ravl/FMeanCovariance.hh include/Ravl/MeanCovariance2d.hh include/Ravl/MeanCovariance3d.hh include/Ravl/Normal.hh include/Ravl/Sums1d2.hh include/Ravl/SumsNd2.hh include/Ravl/WeightedMeanVariance.hh include/Ravl/Histogram.hh include/Ravl/RealHistogram1d.hh include/Ravl/RealHistogram2d.hh include/Ravl/RealHistogram3d.hh include/Ravl/Parzen.hh include/Ravl/RealHistogramNd.hh include/Ravl/PrimitiveBinaryPolynomial.hh include/Ravl/SobolSequence.hh include/Ravl/CircleIter.hh include/Ravl/PermutationIter.hh include/Ravl/CombinationIter.hh include/Ravl/SquareIter.hh include/Ravl/ZigZagIter.hh include/Ravl/Polygon2dIter.hh include/Ravl/Array2dPolygon2dIter.hh include/Ravl/ThinPlateSpline1d.hh include/Ravl/FFT1d.hh include/Ravl/PowerSpectrum1d.hh include/Ravl/Correlation1d.hh include/Ravl/RealCepstral.hh include/Ravl/Convolution1d.hh include/Ravl/GenerateSignal1d.hh include/Ravl/FFT2d.hh
 src/TMatrix.cc src/Matrix.cc src/MatrixInverse.cc src/MatrixSVD.cc src/MatrixEigen.cc src/MatrixSolve.cc src/MatrixRUT.cc src/MatrixRS.cc src/Vector.cc src/MatrixNearSingular.cc src/LeastSquares.cc src/VectorMatrix.cc src/MatrixRandom.cc src/MatrixDeterminant.cc src/MatrixLUDecomposition.cc src/TSMatrixSparse.cc src/SMatrix.cc src/RawMatrix.cc src/FastMatrixEigen.cc src/LAHooks.cc src/Vector2d.cc src/Matrix3d.cc src/VectorMatrix2d.cc src/VectorMatrix3d.cc src/VectorMatrix4d.cc src/Matrix2d.cc src/LineABC2d.cc src/Curve2d.cc src/Curve2dLine.cc src/Curve2dLineFit.cc src/Curve2dLineSegment.cc src/Line2dIter.cc src/Affine2d.cc src/Moments2d2.cc src/LinePP2d.cc src/Projection2d.cc src/Point2d.cc src/PointSet2d.cc src/Polygon2d.cc src/Circle2d.cc src/FitCircle2d.cc src/Arc2d.cc src/Curve2dCircle.cc src/Curve2dArc.cc src/FitLineABC2d.cc src/ConvexHullOfPointSet.cc src/HEMesh2d.cc src/DelaunayTriangulation2d.cc src/TriMesh2d.cc src/AffineTransformPolygon2d.cc src/ProjectiveTransformPolygon2d.cc src/Conic2d.cc src/Ellipse2d.cc src/ScanPolygon2d.cc src/PolyLine2d.cc src/Poly2dApprox.cc src/Triangulate2d.cc src/LinePP3d.cc src/LinePV3d.cc src/PlaneABCD3d.cc src/PlanePVV3d.cc src/Quatern3d.cc src/RigidTransform3d.cc src/EulerAngle.cc src/AxisAngle.cc src/PointSet3d.cc src/HEMesh3d.cc src/Affine3d.cc src/FitPlaneABCD3d.cc src/FitPlanePVV3d.cc src/GPSCoordinate.cc src/CrackCode.cc src/BVertex.cc src/Crack.cc src/Boundary.cc src/ConvexHull.cc src/Edge.cc src/PPoint1d.cc src/PProjection1d.cc src/PPointLine2d.cc src/PPoint2d.cc src/PLine2d.cc src/PLinePP2d.cc src/PProjection2d.cc src/PPerspective2d.cc src/FundamentalMatrix2d.cc src/FitPProjection2d.cc src/PPointPlane3d.cc src/PPoint3d.cc src/PPlane3d.cc src/PProjection3d.cc src/PPerspective3d.cc src/PLinePV3d.cc src/FitPPlane3d.cc src/PPoint3dAffine.cc src/MeanVariance.cc src/MeanNd.cc src/MeanCovariance.cc src/Statistics.cc src/Mean.cc src/Normal.cc src/Sums1d2.cc src/SumsNd2.cc src/WeightedMeanVariance.cc src/RealHistogram1d.cc src/RealHistogram2d.cc src/RealHistogram3d.cc src/RealHist1dBinaryIO.cc src/RealHist2dBinaryIO.cc src/RealHist3dBinaryIO.cc src/Parzen.cc src/RealHistogramNd.cc src/PrimitiveBinaryPolynomial.cc src/SobolSequence.cc src/CircleIter.cc src/SquareIter.cc src/ZigZagIter.cc src/Polygon2dIter.cc src/ThinPlateSpline1d.cc src/FFT1d.cc src/PowerSpectrum1d.cc src/Correlation1d.cc src/RealCepstral.cc src/GenerateSignal1d.cc src/FFT2d.cc
)
target_link_libraries(RavlMath_Body PUBLIC RavlCore ccmath RavlIO RavlOSIO RavlXMLFactory )

add_library(RavlMath OBJECT src/LinkRavlSignal1.cc )
target_link_libraries(RavlMath PUBLIC RavlMath_Body )


target_include_directories(
    RavlMath PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
    )

# MustLink fixes:
install(EXPORT RavlMath_BodyConfig DESTINATION lib/cmake/RavlMath_Body/Target)
install(TARGETS RavlMath_Body EXPORT RavlMath_BodyConfig
   ARCHIVE  DESTINATION lib
   LIBRARY  DESTINATION lib
   RUNTIME  DESTINATION bin)  # This is for Windows
install(TARGETS RavlMath_Body DESTINATION lib)

configure_package_config_file(cmake/RavlMath_BodyConfig.cmake.in
  ${CMAKE_CURRENT_BINARY_DIR}/RavlMath_BodyConfig.cmake
  INSTALL_DESTINATION lib/cmake/RavlMath_Body
  PATH_VARS)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/RavlMath_BodyConfig.cmake
        DESTINATION lib/cmake/RavlMath_Body )


install(TARGETS RavlMath EXPORT RavlMathConfig
   ARCHIVE  DESTINATION lib
   LIBRARY  DESTINATION lib
   RUNTIME  DESTINATION bin)  # This is for Windows

install(EXPORT RavlMathConfig DESTINATION lib/cmake/RavlMath/Target)
install(TARGETS RavlMath DESTINATION lib)

configure_package_config_file(cmake/RavlMathConfig.cmake.in
  ${CMAKE_CURRENT_BINARY_DIR}/RavlMathConfig.cmake
  INSTALL_DESTINATION lib/cmake/RavlMath
  PATH_VARS)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/RavlMathConfig.cmake
        DESTINATION lib/cmake/RavlMath )

install(DIRECTORY include DESTINATION .)


# Mains
set(MAIN_LIBS  RavlCore ccmath RavlIO RavlOSIO RavlXMLFactory RavlOS RavlImage RavlImageIO RavlThreads  )

add_executable(benchmarkSMatrix applications/benchmarkSMatrix.cc )
target_link_libraries(benchmarkSMatrix RavlMath ${MAIN_LIBS} )
install(TARGETS benchmarkSMatrix DESTINATION bin)

# Examples
set(EXAMPLE_LIBS  RavlCore ccmath RavlIO RavlOSIO RavlXMLFactory RavlOS RavlImage RavlImageIO RavlThreads  )

add_executable(exFAffine EXCLUDE_FROM_ALL examples/exFAffine.cc )
target_link_libraries(exFAffine RavlMath ${EXAMPLE_LIBS} )
install(TARGETS exFAffine DESTINATION examples COMPONENT examples EXCLUDE_FROM_ALL)

add_executable(exDelaunayTriangulation2d EXCLUDE_FROM_ALL examples/exDelaunayTriangulation2d.cc )
target_link_libraries(exDelaunayTriangulation2d RavlMath ${EXAMPLE_LIBS} )
install(TARGETS exDelaunayTriangulation2d DESTINATION examples COMPONENT examples EXCLUDE_FROM_ALL)

add_executable(exHEMesh2d EXCLUDE_FROM_ALL examples/exHEMesh2d.cc )
target_link_libraries(exHEMesh2d RavlMath ${EXAMPLE_LIBS} )
install(TARGETS exHEMesh2d DESTINATION examples COMPONENT examples EXCLUDE_FROM_ALL)

add_executable(exHistogram EXCLUDE_FROM_ALL examples/exHistogram.cc )
target_link_libraries(exHistogram RavlMath ${EXAMPLE_LIBS} )
install(TARGETS exHistogram DESTINATION examples COMPONENT examples EXCLUDE_FROM_ALL)

add_executable(exPrimiteBinaryPolynomial EXCLUDE_FROM_ALL examples/exPrimiteBinaryPolynomial.cc )
target_link_libraries(exPrimiteBinaryPolynomial RavlMath ${EXAMPLE_LIBS} )
install(TARGETS exPrimiteBinaryPolynomial DESTINATION examples COMPONENT examples EXCLUDE_FROM_ALL)

add_executable(testSquareIterFill EXCLUDE_FROM_ALL examples/testSquareIterFill.cc )
target_link_libraries(testSquareIterFill RavlMath ${EXAMPLE_LIBS} )
install(TARGETS testSquareIterFill DESTINATION examples COMPONENT examples EXCLUDE_FROM_ALL)

add_executable(exThinPlateSpline1d EXCLUDE_FROM_ALL examples/exThinPlateSpline1d.cc )
target_link_libraries(exThinPlateSpline1d RavlMath ${EXAMPLE_LIBS} )
install(TARGETS exThinPlateSpline1d DESTINATION examples COMPONENT examples EXCLUDE_FROM_ALL)

add_executable(exFFT1d EXCLUDE_FROM_ALL examples/exFFT1d.cc )
target_link_libraries(exFFT1d RavlMath ${EXAMPLE_LIBS} )
install(TARGETS exFFT1d DESTINATION examples COMPONENT examples EXCLUDE_FROM_ALL)

add_executable(exPowerSpectrum EXCLUDE_FROM_ALL examples/exPowerSpectrum.cc )
target_link_libraries(exPowerSpectrum RavlMath ${EXAMPLE_LIBS} )
install(TARGETS exPowerSpectrum DESTINATION examples COMPONENT examples EXCLUDE_FROM_ALL)

add_executable(exFFT2d EXCLUDE_FROM_ALL examples/exFFT2d.cc )
target_link_libraries(exFFT2d RavlMath ${EXAMPLE_LIBS} )
install(TARGETS exFFT2d DESTINATION examples COMPONENT examples EXCLUDE_FROM_ALL)

# Tests
set(TEST_LIBS  RavlCore ccmath RavlIO RavlOSIO RavlXMLFactory RavlOS RavlImage RavlImageIO RavlThreads  )

add_executable(testTMatrix EXCLUDE_FROM_ALL tests/testTMatrix.cc )
target_link_libraries(testTMatrix RavlMath ${TEST_LIBS} )
add_test(testTMatrix testTMatrix)
install(TARGETS testTMatrix DESTINATION tests COMPONENT unittests EXCLUDE_FROM_ALL)

add_executable(testMatInv EXCLUDE_FROM_ALL tests/testMatInv.cc )
target_link_libraries(testMatInv RavlMath ${TEST_LIBS} )
add_test(testMatInv testMatInv)
install(TARGETS testMatInv DESTINATION tests COMPONENT unittests EXCLUDE_FROM_ALL)

add_executable(testMatrix EXCLUDE_FROM_ALL tests/testMatrix.cc )
target_link_libraries(testMatrix RavlMath ${TEST_LIBS} )
add_test(testMatrix testMatrix)
install(TARGETS testMatrix DESTINATION tests COMPONENT unittests EXCLUDE_FROM_ALL)

add_executable(testTSMatrix EXCLUDE_FROM_ALL tests/testTSMatrix.cc )
target_link_libraries(testTSMatrix RavlMath ${TEST_LIBS} )
add_test(testTSMatrix testTSMatrix)
install(TARGETS testTSMatrix DESTINATION tests COMPONENT unittests EXCLUDE_FROM_ALL)

add_executable(testSpecificFMatrix EXCLUDE_FROM_ALL tests/testSpecificFMatrix.cc )
target_link_libraries(testSpecificFMatrix RavlMath ${TEST_LIBS} )
add_test(testSpecificFMatrix testSpecificFMatrix)
install(TARGETS testSpecificFMatrix DESTINATION tests COMPONENT unittests EXCLUDE_FROM_ALL)

add_executable(testLine2d EXCLUDE_FROM_ALL tests/testLine2d.cc )
target_link_libraries(testLine2d RavlMath ${TEST_LIBS} )
add_test(testLine2d testLine2d)
install(TARGETS testLine2d DESTINATION tests COMPONENT unittests EXCLUDE_FROM_ALL)

add_executable(testGeom2d EXCLUDE_FROM_ALL tests/testGeom2d.cc )
target_link_libraries(testGeom2d RavlMath ${TEST_LIBS} )
add_test(testGeom2d testGeom2d)
install(TARGETS testGeom2d DESTINATION tests COMPONENT unittests EXCLUDE_FROM_ALL)

add_executable(testGeom3d EXCLUDE_FROM_ALL tests/testGeom3d.cc )
target_link_libraries(testGeom3d RavlMath ${TEST_LIBS} )
add_test(testGeom3d testGeom3d)
install(TARGETS testGeom3d DESTINATION tests COMPONENT unittests EXCLUDE_FROM_ALL)

add_executable(testBoundry EXCLUDE_FROM_ALL tests/testBoundry.cc )
target_link_libraries(testBoundry RavlMath ${TEST_LIBS} )
add_test(testBoundry testBoundry)
install(TARGETS testBoundry DESTINATION tests COMPONENT unittests EXCLUDE_FROM_ALL)

add_executable(testFundamentalMatrix2d EXCLUDE_FROM_ALL tests/testFundamentalMatrix2d.cc )
target_link_libraries(testFundamentalMatrix2d RavlMath ${TEST_LIBS} )
add_test(testFundamentalMatrix2d testFundamentalMatrix2d)
install(TARGETS testFundamentalMatrix2d DESTINATION tests COMPONENT unittests EXCLUDE_FROM_ALL)

add_executable(testProjective2d EXCLUDE_FROM_ALL tests/testProjective2d.cc )
target_link_libraries(testProjective2d RavlMath ${TEST_LIBS} )
add_test(testProjective2d testProjective2d)
install(TARGETS testProjective2d DESTINATION tests COMPONENT unittests EXCLUDE_FROM_ALL)

add_executable(testMeanCovar EXCLUDE_FROM_ALL tests/testMeanCovar.cc )
target_link_libraries(testMeanCovar RavlMath ${TEST_LIBS} )
add_test(testMeanCovar testMeanCovar)
install(TARGETS testMeanCovar DESTINATION tests COMPONENT unittests EXCLUDE_FROM_ALL)

add_executable(testHistogram EXCLUDE_FROM_ALL tests/testHistogram.cc )
target_link_libraries(testHistogram RavlMath ${TEST_LIBS} )
add_test(testHistogram testHistogram)
install(TARGETS testHistogram DESTINATION tests COMPONENT unittests EXCLUDE_FROM_ALL)

add_executable(testCircleIter EXCLUDE_FROM_ALL tests/testCircleIter.cc )
target_link_libraries(testCircleIter RavlMath ${TEST_LIBS} )
add_test(testCircleIter testCircleIter)
install(TARGETS testCircleIter DESTINATION tests COMPONENT unittests EXCLUDE_FROM_ALL)

add_executable(testCombinationIter EXCLUDE_FROM_ALL tests/testCombinationIter.cc )
target_link_libraries(testCombinationIter RavlMath ${TEST_LIBS} )
add_test(testCombinationIter testCombinationIter)
install(TARGETS testCombinationIter DESTINATION tests COMPONENT unittests EXCLUDE_FROM_ALL)

add_executable(testPolygon2dIter EXCLUDE_FROM_ALL tests/testPolygon2dIter.cc )
target_link_libraries(testPolygon2dIter RavlMath ${TEST_LIBS} )
add_test(testPolygon2dIter testPolygon2dIter)
install(TARGETS testPolygon2dIter DESTINATION tests COMPONENT unittests EXCLUDE_FROM_ALL)

add_executable(testThinPlateSpline1d EXCLUDE_FROM_ALL tests/testThinPlateSpline1d.cc )
target_link_libraries(testThinPlateSpline1d RavlMath ${TEST_LIBS} )
add_test(testThinPlateSpline1d testThinPlateSpline1d)
install(TARGETS testThinPlateSpline1d DESTINATION tests COMPONENT unittests EXCLUDE_FROM_ALL)

add_executable(testGenerateSignal1d EXCLUDE_FROM_ALL tests/testGenerateSignal1d.cc )
target_link_libraries(testGenerateSignal1d RavlMath ${TEST_LIBS} )
add_test(testGenerateSignal1d testGenerateSignal1d)
install(TARGETS testGenerateSignal1d DESTINATION tests COMPONENT unittests EXCLUDE_FROM_ALL)

add_executable(testSignal1d EXCLUDE_FROM_ALL tests/testSignal1d.cc )
target_link_libraries(testSignal1d RavlMath ${TEST_LIBS} )
add_test(testSignal1d testSignal1d)
install(TARGETS testSignal1d DESTINATION tests COMPONENT unittests EXCLUDE_FROM_ALL)

add_executable(testFFT2d EXCLUDE_FROM_ALL tests/testFFT2d.cc )
target_link_libraries(testFFT2d RavlMath ${TEST_LIBS} )
add_test(testFFT2d testFFT2d)
install(TARGETS testFFT2d DESTINATION tests COMPONENT unittests EXCLUDE_FROM_ALL)






