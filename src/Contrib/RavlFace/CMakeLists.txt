
# Library: RavlFace

# Requires: RLog
# Supports: 



if(librlog_FOUND)

# MustLink: src/linkFaceExperiment.cc


set(DEPENDS_ON )
foreach(D IN LISTS DEPENDS_ON)
  if(NOT ${D}_FOUND)
    find_package(${D} REQUIRED)
  endif()
endforeach()

include(CMakePackageConfigHelpers)

include_directories(include)
include_directories(src)

set(HAVE_RavlFace 1)

include_directories(../../Math/RavlMath/include)
include_directories(../../Image/RavlImageProc/include)
include_directories(../../Core/RavlCore/include)
include_directories(../../Core/RavlIO/include)
include_directories(../../Image/RavlImage/include)
include_directories(../../OS/RavlOS/include)
include_directories(../../OS/RavlXMLFactory/include)
include_directories(../../Contrib/RavlRLog/include)
include_directories(../../Contrib/RavlGnuPlot/include)
include_directories(../../PatternRec/RavlDataSet/include)
include_directories(../../OS/RavlThreads/include)
include_directories(../../Contrib/RavlTwoWayMap/include)
include_directories(../../Plot/RavlPlot/include)
include_directories(../../PatternRec/RavlPatternRecIO/include)
include_directories(../../PatternRec/RavlSVM/include)
include_directories(../../OS/RavlOSIO/include)
include_directories(../../PatternRec/RavlPatternRec/include)



if(DEFINED HAVE_RavlExtImgIO)
include_directories(../../Image/RavlExtImgIO/include)
endif()

add_library(RavlFace_Body STATIC
 include/Ravl/Face/FaceInfo.hh include/Ravl/Face/Enrol.hh include/Ravl/Face/EnrolSession.hh include/Ravl/Face/Expert.hh include/Ravl/Face/Score.hh include/Ravl/Face/ScoreSession.hh include/Ravl/Face/Protocol.hh include/Ravl/Face/FaceInfoDb.hh include/Ravl/Face/FaceInfoStream.hh include/Ravl/Face/Sighting.hh include/Ravl/Face/SightingSet.hh include/Ravl/Face/FacePair.hh include/Ravl/Face/FacePairSet.hh include/Ravl/Face/Claim.hh include/Ravl/Face/ClaimSession.hh include/Ravl/Face/ScoresTable.hh include/Ravl/Face/ResultsInfo.hh include/Ravl/Face/Experiment.hh include/Ravl/Face/Roc.hh
 src/FaceInfo.cc src/Enrol.cc src/EnrolSession.cc src/Expert.cc src/Score.cc src/ScoreSession.cc src/Protocol.cc src/FaceInfoDb.cc src/FaceInfoStream.cc src/Sighting.cc src/SightingSet.cc src/FacePair.cc src/FacePairSet.cc src/Claim.cc src/ClaimSession.cc src/ScoresTable.cc src/ResultsInfo.cc src/Experiment.cc src/Roc.cc
)
target_link_libraries(RavlFace_Body PUBLIC RavlMath RavlImageProc RavlCore RavlIO RavlImage RavlOS RavlXMLFactory RavlRLog ${librlog_LIBRARIES} RavlGnuPlot RavlDataSet RavlThreads RavlTwoWayMap RavlPlot )

add_library(RavlFace OBJECT src/linkFaceExperiment.cc )
target_link_libraries(RavlFace PUBLIC RavlFace_Body )


target_include_directories(
    RavlFace PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
    )

# MustLink fixes:
install(EXPORT RavlFace_BodyConfig DESTINATION lib/cmake/RavlFace_Body/Target)
install(TARGETS RavlFace_Body EXPORT RavlFace_BodyConfig
   ARCHIVE  DESTINATION lib
   LIBRARY  DESTINATION lib
   RUNTIME  DESTINATION bin)  # This is for Windows
install(TARGETS RavlFace_Body DESTINATION lib)

configure_package_config_file(cmake/RavlFace_BodyConfig.cmake.in
  ${CMAKE_CURRENT_BINARY_DIR}/RavlFace_BodyConfig.cmake
  INSTALL_DESTINATION lib/cmake/RavlFace_Body
  PATH_VARS)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/RavlFace_BodyConfig.cmake
        DESTINATION lib/cmake/RavlFace_Body )


install(TARGETS RavlFace EXPORT RavlFaceConfig
   ARCHIVE  DESTINATION lib
   LIBRARY  DESTINATION lib
   RUNTIME  DESTINATION bin)  # This is for Windows

install(EXPORT RavlFaceConfig DESTINATION lib/cmake/RavlFace/Target)
install(TARGETS RavlFace DESTINATION lib)

configure_package_config_file(cmake/RavlFaceConfig.cmake.in
  ${CMAKE_CURRENT_BINARY_DIR}/RavlFaceConfig.cmake
  INSTALL_DESTINATION lib/cmake/RavlFace
  PATH_VARS)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/RavlFaceConfig.cmake
        DESTINATION lib/cmake/RavlFace )

install(DIRECTORY include DESTINATION .)


# Mains
set(MAIN_LIBS  RavlMath RavlImageProc RavlCore RavlIO RavlImage RavlOS RavlXMLFactory RavlRLog ${librlog_LIBRARIES} RavlGnuPlot RavlDataSet RavlThreads RavlTwoWayMap RavlPlot RavlPatternRecIO RavlSVM RavlOSIO RavlPatternRec  )

add_executable(readFaceXml applications/readFaceXml.cc )
target_link_libraries(readFaceXml RavlFace ${MAIN_LIBS} )
install(TARGETS readFaceXml DESTINATION bin)

add_executable(copyFaceXml applications/copyFaceXml.cc )
target_link_libraries(copyFaceXml RavlFace ${MAIN_LIBS} )
install(TARGETS copyFaceXml DESTINATION bin)

add_executable(modifyFaceXml applications/modifyFaceXml.cc )
target_link_libraries(modifyFaceXml RavlFace ${MAIN_LIBS} )
install(TARGETS modifyFaceXml DESTINATION bin)

add_executable(splitFaceXml applications/splitFaceXml.cc )
target_link_libraries(splitFaceXml RavlFace ${MAIN_LIBS} )
install(TARGETS splitFaceXml DESTINATION bin)

add_executable(faceInfoDb2enrolSession applications/faceInfoDb2enrolSession.cc )
target_link_libraries(faceInfoDb2enrolSession RavlFace ${MAIN_LIBS} )
install(TARGETS faceInfoDb2enrolSession DESTINATION bin)

add_executable(insertFeatureSets applications/insertFeatureSets.cc )
target_link_libraries(insertFeatureSets RavlFace ${MAIN_LIBS} )
install(TARGETS insertFeatureSets DESTINATION bin)

add_executable(mergeSightingSets applications/mergeSightingSets.cc )
target_link_libraries(mergeSightingSets RavlFace ${MAIN_LIBS} )
install(TARGETS mergeSightingSets DESTINATION bin)

add_executable(perturbFaceXml applications/perturbFaceXml.cc )
target_link_libraries(perturbFaceXml RavlFace ${MAIN_LIBS} )
install(TARGETS perturbFaceXml DESTINATION bin)

add_executable(faceInfoDb2SightingSet applications/faceInfoDb2SightingSet.cc )
target_link_libraries(faceInfoDb2SightingSet RavlFace ${MAIN_LIBS} )
install(TARGETS faceInfoDb2SightingSet DESTINATION bin)

add_executable(doClassifierRoc applications/doClassifierRoc.cc )
target_link_libraries(doClassifierRoc RavlFace ${MAIN_LIBS} )
install(TARGETS doClassifierRoc DESTINATION bin)






ENDIF()


