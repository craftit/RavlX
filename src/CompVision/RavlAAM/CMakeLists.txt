
# Library: RavlAAM

# Requires: 
# Supports: !arm

if((NOT ${CMAKE_SYSTEM_PROCESSOR} MATCHES "arm") OR FALSE)


# MustLink: src/AAMActiveAppearanceModelMustLink.cc


set(DEPENDS_ON )
foreach(D IN LISTS DEPENDS_ON)
  if(NOT ${D}_FOUND)
    find_package(${D} REQUIRED)
  endif()
endforeach()

include(CMakePackageConfigHelpers)

include_directories(include)
include_directories(src)

set(HAVE_RavlAAM 1)

include_directories(../../Core/RavlCore/include)
include_directories(../../Image/RavlImage/include)
include_directories(../../Image/RavlImageProc/include)
include_directories(../../Math/RavlMath/include)
include_directories(../../OS/RavlOS/include)
include_directories(../../PatternRec/RavlPatternRec/include)
include_directories(../../Math/RavlOptimise/include)
include_directories(../../Core/RavlIO/include)
include_directories(../../PatternRec/Optimisation/include)
include_directories(../../PatternRec/RavlDataSet/include)
include_directories(../../OS/RavlXMLFactory/include)
include_directories(../../OS/RavlThreads/include)
include_directories(../../Image/RavlImageIO/include)
include_directories(../../Math/RavlMathIO/include)
include_directories(../../OS/RavlOSIO/include)



if(DEFINED HAVE_RavlExtImgIO)
include_directories(../../Image/RavlExtImgIO/include)
endif()
if(DEFINED HAVE_RavlDPDisplay)
include_directories(../../GUI/RavlDPDisplay/include)
endif()
if(DEFINED HAVE_RavlImgIOV4L2)
include_directories(../../Contrib/RavlImgIOV4L2/include)
endif()

add_library(RavlAAM_Body STATIC
 include/Ravl/Image/AAMAppearance.hh include/Ravl/Image/AAMShapeModel.hh include/Ravl/Image/AAMAffineShapeModel.hh include/Ravl/Image/AAMScaleRotationShapeModel.hh include/Ravl/Image/AAMAppearanceModel.hh include/Ravl/Image/AAMAppearanceUtil.hh include/Ravl/Image/AAMActiveAppearanceModel.hh include/Ravl/Image/AAMSampleStream.hh include/Ravl/Image/AAMMultiResActiveAppearanceModel.hh include/Ravl/Image/AAMFaceLocalisation.hh include/Ravl/Image/AAMPoseEstimateCostFunction.hh include/Ravl/Image/AAMPoseEstimate.hh include/Ravl/Image/AAMFaceCorrection.hh include/Ravl/Image/AAMSampleStreamFileList.hh
 src/AAMAppearance.cc src/AAMShapeModel.cc src/AAMAffineShapeModel.cc src/AAMScaleRotationShapeModel.cc src/AAMAppearanceModel.cc src/AAMAppearanceUtil.cc src/AAMActiveAppearanceModel.cc src/AAMSampleStream.cc src/AAMMultiResActiveAppearanceModel.cc src/AAMFaceLocalisation.cc src/AAMPoseEstimateCostFunction.cc src/AAMPoseEstimate.cc src/AAMFaceCorrection.cc src/AAMActiveAppearanceModelsIO.cc src/AAMSampleStreamFileList.cc
)
target_link_libraries(RavlAAM_Body PUBLIC RavlCore RavlImage RavlImageProc RavlMath RavlOS RavlPatternRec RavlOptimise RavlIO Optimisation RavlDataSet RavlXMLFactory RavlThreads )

add_library(RavlAAM OBJECT src/AAMActiveAppearanceModelMustLink.cc )
target_link_libraries(RavlAAM PUBLIC RavlAAM_Body )


target_include_directories(
    RavlAAM PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
    )

# MustLink fixes:
install(EXPORT RavlAAM_BodyConfig DESTINATION lib/cmake/RavlAAM_Body/Target)
install(TARGETS RavlAAM_Body EXPORT RavlAAM_BodyConfig
   ARCHIVE  DESTINATION lib
   LIBRARY  DESTINATION lib
   RUNTIME  DESTINATION bin)  # This is for Windows
install(TARGETS RavlAAM_Body DESTINATION lib)

configure_package_config_file(cmake/RavlAAM_BodyConfig.cmake.in
  ${CMAKE_CURRENT_BINARY_DIR}/RavlAAM_BodyConfig.cmake
  INSTALL_DESTINATION lib/cmake/RavlAAM_Body
  PATH_VARS)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/RavlAAM_BodyConfig.cmake
        DESTINATION lib/cmake/RavlAAM_Body )


install(TARGETS RavlAAM EXPORT RavlAAMConfig
   ARCHIVE  DESTINATION lib
   LIBRARY  DESTINATION lib
   RUNTIME  DESTINATION bin)  # This is for Windows

install(EXPORT RavlAAMConfig DESTINATION lib/cmake/RavlAAM/Target)
install(TARGETS RavlAAM DESTINATION lib)

configure_package_config_file(cmake/RavlAAMConfig.cmake.in
  ${CMAKE_CURRENT_BINARY_DIR}/RavlAAMConfig.cmake
  INSTALL_DESTINATION lib/cmake/RavlAAM
  PATH_VARS)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/RavlAAMConfig.cmake
        DESTINATION lib/cmake/RavlAAM )

install(DIRECTORY include DESTINATION .)


# Mains
set(MAIN_LIBS  RavlCore RavlImage RavlImageProc RavlMath RavlOS RavlPatternRec RavlOptimise RavlIO Optimisation RavlDataSet RavlXMLFactory RavlThreads RavlImageIO RavlMathIO RavlOSIO  )

add_executable(aamBuildShapeModel ${EXCLUDE_APPLICATIONS} applications/aamBuildShapeModel.cc )
target_link_libraries(aamBuildShapeModel RavlAAM ${MAIN_LIBS} )
install(TARGETS aamBuildShapeModel DESTINATION bin ${EXCLUDE_APPLICATIONS})

add_executable(aamBuildAppearanceModel ${EXCLUDE_APPLICATIONS} applications/aamBuildAppearanceModel.cc )
target_link_libraries(aamBuildAppearanceModel RavlAAM ${MAIN_LIBS} )
install(TARGETS aamBuildAppearanceModel DESTINATION bin ${EXCLUDE_APPLICATIONS})

add_executable(aamBuildActiveAppearanceModel ${EXCLUDE_APPLICATIONS} applications/aamBuildActiveAppearanceModel.cc )
target_link_libraries(aamBuildActiveAppearanceModel RavlAAM ${MAIN_LIBS} )
install(TARGETS aamBuildActiveAppearanceModel DESTINATION bin ${EXCLUDE_APPLICATIONS})

add_executable(aamCombineActiveAppearanceModel ${EXCLUDE_APPLICATIONS} applications/aamCombineActiveAppearanceModel.cc )
target_link_libraries(aamCombineActiveAppearanceModel RavlAAM ${MAIN_LIBS} )
install(TARGETS aamCombineActiveAppearanceModel DESTINATION bin ${EXCLUDE_APPLICATIONS})

add_executable(aamLocaliseFace ${EXCLUDE_APPLICATIONS} applications/aamLocaliseFace.cc )
target_link_libraries(aamLocaliseFace RavlAAM ${MAIN_LIBS} )
install(TARGETS aamLocaliseFace DESTINATION bin ${EXCLUDE_APPLICATIONS})

add_executable(aamBuildPoseEstimator ${EXCLUDE_APPLICATIONS} applications/aamBuildPoseEstimator.cc )
target_link_libraries(aamBuildPoseEstimator RavlAAM ${MAIN_LIBS} )
install(TARGETS aamBuildPoseEstimator DESTINATION bin ${EXCLUDE_APPLICATIONS})

add_executable(aamSynthesizeFrontal ${EXCLUDE_APPLICATIONS} applications/aamSynthesizeFrontal.cc )
target_link_libraries(aamSynthesizeFrontal RavlAAM ${MAIN_LIBS} )
install(TARGETS aamSynthesizeFrontal DESTINATION bin ${EXCLUDE_APPLICATIONS})

add_executable(aamTrackFace ${EXCLUDE_APPLICATIONS} applications/aamTrackFace.cc )
target_link_libraries(aamTrackFace RavlAAM ${MAIN_LIBS} )
install(TARGETS aamTrackFace DESTINATION bin ${EXCLUDE_APPLICATIONS})




# === Files for share/doc/Ravl/Images/AAM ===
set(AUXDIR share/doc/Ravl/Images/AAM)
set(AUXFILES 
  data/000_1_2_090.png 
  data/000_1_2_090.xml )
file(COPY ${AUXFILES} DESTINATION ${BUILD_ROOT_AUXFILE}/${AUXDIR})
install(FILES ${AUXFILES} DESTINATION ${AUXDIR})




ENDIF()
