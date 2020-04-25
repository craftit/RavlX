
# Library: RavlImageProc

# Requires: 
# Supports: 




# MustLink: src/ImagePointFeatureIO.cc src/RavlEdgeIO.cc


set(DEPENDS_ON )
foreach(D IN LISTS DEPENDS_ON)
  if(NOT ${D}_FOUND)
    find_package(${D} REQUIRED)
  endif()
endforeach()

include(CMakePackageConfigHelpers)

include_directories(include)
include_directories(src)

set(HAVE_RavlImageProc 1)

include_directories(../../Image/RavlImage/include)
include_directories(../../OS/RavlThreads/include)
include_directories(../../Math/RavlMath/include)
include_directories(../../Core/RavlIO/include)
include_directories(../../OS/RavlOS/include)
include_directories(../../OS/RavlOSIO/include)
include_directories(../../Math/RavlOptimise/include)
include_directories(../../Core/RavlCore/include)
include_directories(../../Image/RavlImageIO/include)
include_directories(../../Image/RavlExtImgIO/include)
include_directories(../../Image/RavlVideoIO/include)
include_directories(../../OS/RavlDPMT/include)
include_directories(../../Math/RavlMathIO/include)



if(DEFINED HAVE_RavlDPDisplay)
include_directories(../../GUI/RavlDPDisplay/include)
endif()
if(DEFINED HAVE_RavlImgIOV4L)
include_directories(../../Contrib/RavlImgIOV4L/include)
endif()
if(DEFINED HAVE_RavlDV)
include_directories(../../Contrib/RavlDV/include)
endif()
if(DEFINED HAVE_RavlLibFFmpeg)
include_directories(../../Image/RavlLibFFmpeg/include)
endif()
if(DEFINED HAVE_RavlExtImgIO)
include_directories(../../Image/RavlExtImgIO/include)
endif()
if(DEFINED HAVE_CSPDriver)
include_directories(../../Contrib/CSPDriver/include)
endif()

add_library(RavlImageProc_Body STATIC
 include/Ravl/Image/Rectangle2dIter.hh include/Ravl/Image/SobolImage.hh include/Ravl/Image/SubSample.hh include/Ravl/Image/SummedAreaTable.hh include/Ravl/Image/SummedAreaTable2.hh include/Ravl/Image/PyramidScan.hh include/Ravl/Image/PeakDetector.hh include/Ravl/Image/ImagePointFeatureSet.hh include/Ravl/Image/ImagePointFeature.hh include/Ravl/Image/TakeSubImage.hh include/Ravl/Image/HomomorphicFilter.hh include/Ravl/Image/HistogramEqualise.hh include/Ravl/Image/PixelMixer.hh include/Ravl/Image/RemoveDistortion.hh include/Ravl/Image/Deinterlace.hh include/Ravl/Image/DeinterlaceStream.hh include/Ravl/Image/DCT2d.hh include/Ravl/Image/ImageExtend.hh include/Ravl/Image/ImagePyramid.hh include/Ravl/Image/MedianFilter.hh include/Ravl/Image/Convolve2d.hh include/Ravl/Image/ConvolveSeparable2d.hh include/Ravl/Image/ConvolveHorz2d.hh include/Ravl/Image/ConvolveVert2d.hh include/Ravl/Image/GaussConvolve2d.hh include/Ravl/Image/GaussConvolve.hh include/Ravl/Image/Averaging2d.hh include/Ravl/Image/SpatialDifference.hh include/Ravl/Image/SumRectangles.hh include/Ravl/Image/GaborComplex.hh include/Ravl/Image/GaborFilter2d.hh include/Ravl/Image/Correlate2d.hh include/Ravl/Image/NormalisedCorrelation.hh include/Ravl/Image/Matching.hh include/Ravl/Image/WarpScale.hh include/Ravl/Image/WarpAffine.hh include/Ravl/Image/WarpProjective.hh include/Ravl/Image/WarpThinPlateSpline.hh include/Ravl/Image/WarpMesh2d.hh include/Ravl/Image/WarpScale2d.hh include/Ravl/Image/EdgeSobel.hh include/Ravl/Image/EdgeDeriche.hh include/Ravl/Image/SqrComposition.hh include/Ravl/Image/EdgeNonMaxSuppression.hh include/Ravl/Image/Edgel.hh include/Ravl/Image/EdgeLink.hh include/Ravl/Image/EdgeDetector.hh include/Ravl/Image/EdgeDetect.hh include/Ravl/Image/Gradient.hh include/Ravl/Image/Segmentation.hh include/Ravl/Image/RegionSet.hh include/Ravl/Image/ConnectedComponents.hh include/Ravl/Image/ChromaThreshold.hh include/Ravl/Image/BlueScreen.hh include/Ravl/Image/SegmentExtrema.hh include/Ravl/Image/FloodRegion.hh include/Ravl/Image/SegmentRegion.hh include/Ravl/Image/RegionGrow.hh include/Ravl/Image/RegionGrowSteal.hh include/Ravl/Image/PixelSelectStack.hh include/Ravl/Image/PixelClassifyGrey.hh include/Ravl/Image/PixelClassifyColour.hh include/Ravl/Image/PixelClassifyMotion.hh include/Ravl/Image/Erode.hh include/Ravl/Image/Dilate.hh include/Ravl/Image/DilateSubSample2X.hh include/Ravl/Image/MorphOpen.hh include/Ravl/Image/MorphClose.hh include/Ravl/Image/PixelMap.hh include/Ravl/Image/PixelMapSearch.hh include/Ravl/Image/PCPixel.hh include/Ravl/Image/PCPixelList.hh include/Ravl/Image/PCMapping.hh include/Ravl/Image/PPHT.hh include/Ravl/Image/ArcDetector.hh include/Ravl/Image/WhiteLineDetector.hh include/Ravl/Image/Corner.hh include/Ravl/Image/CornerDetector.hh include/Ravl/Image/CornerDetectorHarris.hh include/Ravl/Image/CornerDetectorSusan.hh include/Ravl/Image/LMSOpticFlow.hh include/Ravl/Image/LMSMultiScaleMotion.hh include/Ravl/Image/LMSRegionMotion.hh include/Ravl/Image/PointTrack.hh include/Ravl/Image/PointTrackModel.hh include/Ravl/Image/PointTracker.hh include/Ravl/Image/MosaicBuilder.hh include/Ravl/Image/MatchPatch.hh include/Ravl/Image/ImageTracker.hh include/Ravl/Image/ImageMatcher.hh include/Ravl/Image/MatchNormalisedCorrelation.hh include/Ravl/Image/ForegroundSep.hh include/Ravl/Image/TrackingHomog.hh
 src/Rectangle2dIter.cc src/SobolImage.cc src/PyramidScan.cc src/ImagePointFeatureSet.cc src/ImagePointFeature.cc src/HomomorphicFilter.cc src/DCT2d.cc src/DeinterlaceStream.cc src/MedianFilter.cc src/GaborComplex.cc src/GaborFilter2d.cc src/Correlate2d.cc src/Matching.cc src/WarpThinPlateSpline.cc src/WarpMesh2d.cc src/EdgeSobel.cc src/EdgeDeriche.cc src/SqrComposition.cc src/EdgeNonMaxSuppression.cc src/Edgel.cc src/EdgeLink.cc src/EdgeDetector.cc src/EdgeDetect.cc src/EdgeIO.cc src/ChromaThreshold.cc src/Segmentation.cc src/ConnectedComponents.cc src/BlueScreen.cc src/SegmentExtrema.cc src/PixelSelectStack.cc src/PixelMap.cc src/PixelMapSearch.cc src/PCPixel.cc src/PCPixelList.cc src/PCMapping.cc src/PPHT.cc src/ArcDetector.cc src/WhiteLineDetector.cc src/Corner.cc src/CornerDetector.cc src/CornerDetectorHarris.cc src/CornerDetectorSusan.cc src/LMSOpticFlow.cc src/LMSMultiScaleMotion.cc src/LMSRegionMotion.cc src/PointTrack.cc src/PointTrackModel.cc src/PointTracker.cc src/MosaicBuilder.cc src/ImageTracker.cc src/ImageMatcher.cc src/MatchNormalisedCorrelation.cc src/ForegroundSep.cc src/TrackingHomog.cc
)
target_link_libraries(RavlImageProc_Body PUBLIC RavlImage RavlThreads RavlMath RavlIO RavlOS RavlOSIO RavlOptimise RavlCore )

add_library(RavlImageProc OBJECT src/ImagePointFeatureIO.cc src/RavlEdgeIO.cc )
target_link_libraries(RavlImageProc PUBLIC RavlImageProc_Body )


target_include_directories(
    RavlImageProc PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
    )

# MustLink fixes:
install(EXPORT RavlImageProc_BodyConfig DESTINATION lib/cmake/RavlImageProc_Body/Target)
install(TARGETS RavlImageProc_Body EXPORT RavlImageProc_BodyConfig
   ARCHIVE  DESTINATION lib
   LIBRARY  DESTINATION lib
   RUNTIME  DESTINATION bin)  # This is for Windows
install(TARGETS RavlImageProc_Body DESTINATION lib)

configure_package_config_file(cmake/RavlImageProc_BodyConfig.cmake.in
  ${CMAKE_CURRENT_BINARY_DIR}/RavlImageProc_BodyConfig.cmake
  INSTALL_DESTINATION lib/cmake/RavlImageProc_Body
  PATH_VARS)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/RavlImageProc_BodyConfig.cmake
        DESTINATION lib/cmake/RavlImageProc_Body )


install(TARGETS RavlImageProc EXPORT RavlImageProcConfig
   ARCHIVE  DESTINATION lib
   LIBRARY  DESTINATION lib
   RUNTIME  DESTINATION bin)  # This is for Windows

install(EXPORT RavlImageProcConfig DESTINATION lib/cmake/RavlImageProc/Target)
install(TARGETS RavlImageProc DESTINATION lib)

configure_package_config_file(cmake/RavlImageProcConfig.cmake.in
  ${CMAKE_CURRENT_BINARY_DIR}/RavlImageProcConfig.cmake
  INSTALL_DESTINATION lib/cmake/RavlImageProc
  PATH_VARS)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/RavlImageProcConfig.cmake
        DESTINATION lib/cmake/RavlImageProc )

install(DIRECTORY include DESTINATION .)


# Mains
set(MAIN_LIBS  RavlImage RavlThreads RavlMath RavlIO RavlOS RavlOSIO RavlOptimise RavlCore RavlImageIO RavlExtImgIO RavlVideoIO RavlDPMT RavlMathIO  )

add_executable(convertFeatureSet applications/convertFeatureSet.cc )
target_link_libraries(convertFeatureSet RavlImageProc ${MAIN_LIBS} )
install(TARGETS convertFeatureSet DESTINATION bin)

add_executable(doHomomorphicFilter applications/doHomomorphicFilter.cc )
target_link_libraries(doHomomorphicFilter RavlImageProc ${MAIN_LIBS} )
install(TARGETS doHomomorphicFilter DESTINATION bin)

add_executable(unDistort applications/unDistort.cc )
target_link_libraries(unDistort RavlImageProc ${MAIN_LIBS} )
install(TARGETS unDistort DESTINATION bin)

add_executable(Deinterlace applications/Deinterlace.cc )
target_link_libraries(Deinterlace RavlImageProc ${MAIN_LIBS} )
install(TARGETS Deinterlace DESTINATION bin)

add_executable(doConvolve applications/doConvolve.cc )
target_link_libraries(doConvolve RavlImageProc ${MAIN_LIBS} )
install(TARGETS doConvolve DESTINATION bin)

add_executable(doThinPlateWarp applications/doThinPlateWarp.cc )
target_link_libraries(doThinPlateWarp RavlImageProc ${MAIN_LIBS} )
install(TARGETS doThinPlateWarp DESTINATION bin)

add_executable(doEdgeDet applications/doEdgeDet.cc )
target_link_libraries(doEdgeDet RavlImageProc ${MAIN_LIBS} )
install(TARGETS doEdgeDet DESTINATION bin)

add_executable(extrema applications/extrema.cc )
target_link_libraries(extrema RavlImageProc ${MAIN_LIBS} )
install(TARGETS extrema DESTINATION bin)

add_executable(doPPHT applications/doPPHT.cc )
target_link_libraries(doPPHT RavlImageProc ${MAIN_LIBS} )
install(TARGETS doPPHT DESTINATION bin)

add_executable(doArcs applications/doArcs.cc )
target_link_libraries(doArcs RavlImageProc ${MAIN_LIBS} )
install(TARGETS doArcs DESTINATION bin)

add_executable(exLMSMultiScale applications/exLMSMultiScale.cc )
target_link_libraries(exLMSMultiScale RavlImageProc ${MAIN_LIBS} )
install(TARGETS exLMSMultiScale DESTINATION bin)

add_executable(tracker applications/tracker.cc )
target_link_libraries(tracker RavlImageProc ${MAIN_LIBS} )
install(TARGETS tracker DESTINATION bin)

# Examples
set(EXAMPLE_LIBS  RavlImage RavlThreads RavlMath RavlIO RavlOS RavlOSIO RavlOptimise RavlCore RavlImageIO RavlExtImgIO RavlVideoIO RavlDPMT RavlMathIO  )

add_executable(exFeatureSet EXCLUDE_FROM_ALL examples/exFeatureSet.cc )
target_link_libraries(exFeatureSet RavlImageProc ${EXAMPLE_LIBS} )
install(TARGETS exFeatureSet DESTINATION examples COMPONENT examples EXCLUDE_FROM_ALL)

add_executable(exImagePyramid EXCLUDE_FROM_ALL examples/exImagePyramid.cc )
target_link_libraries(exImagePyramid RavlImageProc ${EXAMPLE_LIBS} )
install(TARGETS exImagePyramid DESTINATION examples COMPONENT examples EXCLUDE_FROM_ALL)

add_executable(exGaussConvolve EXCLUDE_FROM_ALL examples/exGaussConvolve.cc )
target_link_libraries(exGaussConvolve RavlImageProc ${EXAMPLE_LIBS} )
install(TARGETS exGaussConvolve DESTINATION examples COMPONENT examples EXCLUDE_FROM_ALL)

add_executable(exGaborFilter EXCLUDE_FROM_ALL examples/exGaborFilter.cc )
target_link_libraries(exGaborFilter RavlImageProc ${EXAMPLE_LIBS} )
install(TARGETS exGaborFilter DESTINATION examples COMPONENT examples EXCLUDE_FROM_ALL)

add_executable(exGaborComplex EXCLUDE_FROM_ALL examples/exGaborComplex.cc )
target_link_libraries(exGaborComplex RavlImageProc ${EXAMPLE_LIBS} )
install(TARGETS exGaborComplex DESTINATION examples COMPONENT examples EXCLUDE_FROM_ALL)

add_executable(exSegmentation EXCLUDE_FROM_ALL examples/exSegmentation.cc )
target_link_libraries(exSegmentation RavlImageProc ${EXAMPLE_LIBS} )
install(TARGETS exSegmentation DESTINATION examples COMPONENT examples EXCLUDE_FROM_ALL)

add_executable(exConnectedComponents EXCLUDE_FROM_ALL examples/exConnectedComponents.cc )
target_link_libraries(exConnectedComponents RavlImageProc ${EXAMPLE_LIBS} )
install(TARGETS exConnectedComponents DESTINATION examples COMPONENT examples EXCLUDE_FROM_ALL)

add_executable(exMorphology EXCLUDE_FROM_ALL examples/exMorphology.cc )
target_link_libraries(exMorphology RavlImageProc ${EXAMPLE_LIBS} )
install(TARGETS exMorphology DESTINATION examples COMPONENT examples EXCLUDE_FROM_ALL)

add_executable(exCorner EXCLUDE_FROM_ALL examples/exCorner.cc )
target_link_libraries(exCorner RavlImageProc ${EXAMPLE_LIBS} )
install(TARGETS exCorner DESTINATION examples COMPONENT examples EXCLUDE_FROM_ALL)

add_executable(exLMSOptic EXCLUDE_FROM_ALL examples/exLMSOptic.cc )
target_link_libraries(exLMSOptic RavlImageProc ${EXAMPLE_LIBS} )
install(TARGETS exLMSOptic DESTINATION examples COMPONENT examples EXCLUDE_FROM_ALL)

add_executable(exLMSRegionMotion EXCLUDE_FROM_ALL examples/exLMSRegionMotion.cc )
target_link_libraries(exLMSRegionMotion RavlImageProc ${EXAMPLE_LIBS} )
install(TARGETS exLMSRegionMotion DESTINATION examples COMPONENT examples EXCLUDE_FROM_ALL)

add_executable(exImageTracker EXCLUDE_FROM_ALL examples/exImageTracker.cc )
target_link_libraries(exImageTracker RavlImageProc ${EXAMPLE_LIBS} )
install(TARGETS exImageTracker DESTINATION examples COMPONENT examples EXCLUDE_FROM_ALL)

# Tests
set(TEST_LIBS  RavlImage RavlThreads RavlMath RavlIO RavlOS RavlOSIO RavlOptimise RavlCore RavlImageIO RavlExtImgIO RavlVideoIO RavlDPMT RavlMathIO  )

add_executable(testImageTools EXCLUDE_FROM_ALL tests/testImageTools.cc )
target_link_libraries(testImageTools RavlImageProc ${TEST_LIBS} )
add_test(testImageTools testImageTools)
install(TARGETS testImageTools DESTINATION tests COMPONENT unittests EXCLUDE_FROM_ALL)

add_executable(testDeinterlace EXCLUDE_FROM_ALL tests/testDeinterlace.cc )
target_link_libraries(testDeinterlace RavlImageProc ${TEST_LIBS} )
add_test(testDeinterlace testDeinterlace)
install(TARGETS testDeinterlace DESTINATION tests COMPONENT unittests EXCLUDE_FROM_ALL)

add_executable(testConvolve EXCLUDE_FROM_ALL tests/testConvolve.cc )
target_link_libraries(testConvolve RavlImageProc ${TEST_LIBS} )
add_test(testConvolve testConvolve)
install(TARGETS testConvolve DESTINATION tests COMPONENT unittests EXCLUDE_FROM_ALL)

add_executable(testWarp EXCLUDE_FROM_ALL tests/testWarp.cc )
target_link_libraries(testWarp RavlImageProc ${TEST_LIBS} )
add_test(testWarp testWarp)
install(TARGETS testWarp DESTINATION tests COMPONENT unittests EXCLUDE_FROM_ALL)

add_executable(testSubsample EXCLUDE_FROM_ALL tests/testSubsample.cc )
target_link_libraries(testSubsample RavlImageProc ${TEST_LIBS} )
add_test(testSubsample testSubsample)
install(TARGETS testSubsample DESTINATION tests COMPONENT unittests EXCLUDE_FROM_ALL)

add_executable(testEdges EXCLUDE_FROM_ALL tests/testEdges.cc )
target_link_libraries(testEdges RavlImageProc ${TEST_LIBS} )
add_test(testEdges testEdges)
install(TARGETS testEdges DESTINATION tests COMPONENT unittests EXCLUDE_FROM_ALL)

add_executable(testConnectedComponents EXCLUDE_FROM_ALL tests/testConnectedComponents.cc )
target_link_libraries(testConnectedComponents RavlImageProc ${TEST_LIBS} )
add_test(testConnectedComponents testConnectedComponents)
install(TARGETS testConnectedComponents DESTINATION tests COMPONENT unittests EXCLUDE_FROM_ALL)

add_executable(testMorphology EXCLUDE_FROM_ALL tests/testMorphology.cc )
target_link_libraries(testMorphology RavlImageProc ${TEST_LIBS} )
add_test(testMorphology testMorphology)
install(TARGETS testMorphology DESTINATION tests COMPONENT unittests EXCLUDE_FROM_ALL)

add_executable(testLineDet EXCLUDE_FROM_ALL tests/testLineDet.cc )
target_link_libraries(testLineDet RavlImageProc ${TEST_LIBS} )
add_test(testLineDet testLineDet)
install(TARGETS testLineDet DESTINATION tests COMPONENT unittests EXCLUDE_FROM_ALL)

add_executable(testTracking EXCLUDE_FROM_ALL tests/testTracking.cc )
target_link_libraries(testTracking RavlImageProc ${TEST_LIBS} )
add_test(testTracking testTracking)
install(TARGETS testTracking DESTINATION tests COMPONENT unittests EXCLUDE_FROM_ALL)






