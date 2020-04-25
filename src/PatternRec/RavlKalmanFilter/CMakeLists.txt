
# Library: RavlKalmanFilter

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

set(HAVE_RavlKalmanFilter 1)

include_directories(../../Core/RavlCore/include)
include_directories(../../PatternRec/RavlPatternRec/include)
include_directories(../../Math/RavlMath/include)
include_directories(../../OS/RavlXMLFactory/include)
include_directories(../../OS/RavlThreads/include)
include_directories(../../PatternRec/RavlDataSet/include)
include_directories(../../Core/RavlIO/include)




add_library(RavlKalmanFilter STATIC include/Ravl/KalmanFilter.hh include/Ravl/LinearKalmanFilter.hh include/Ravl/ExtendedKalmanFilter.hh include/Ravl/KalmanTwoWheelDifferentialMotionModel.hh include/Ravl/KalmanNullMeasurementModel.hh src/KalmanFilter.cc src/LinearKalmanFilter.cc src/ExtendedKalmanFilter.cc src/KalmanTwoWheelDifferentialMotionModel.cc src/KalmanNullMeasurementModel.cc)
target_link_libraries(RavlKalmanFilter PUBLIC RavlCore RavlPatternRec RavlMath RavlXMLFactory RavlThreads RavlDataSet RavlIO )


target_include_directories(
    RavlKalmanFilter PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
    )



install(TARGETS RavlKalmanFilter EXPORT RavlKalmanFilterConfig
   ARCHIVE  DESTINATION lib
   LIBRARY  DESTINATION lib
   RUNTIME  DESTINATION bin)  # This is for Windows

install(EXPORT RavlKalmanFilterConfig DESTINATION lib/cmake/RavlKalmanFilter/Target)
install(TARGETS RavlKalmanFilter DESTINATION lib)

configure_package_config_file(cmake/RavlKalmanFilterConfig.cmake.in
  ${CMAKE_CURRENT_BINARY_DIR}/RavlKalmanFilterConfig.cmake
  INSTALL_DESTINATION lib/cmake/RavlKalmanFilter
  PATH_VARS)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/RavlKalmanFilterConfig.cmake
        DESTINATION lib/cmake/RavlKalmanFilter )

install(DIRECTORY include DESTINATION .)




# Tests
set(TEST_LIBS  RavlCore RavlPatternRec RavlMath RavlXMLFactory RavlThreads RavlDataSet RavlIO  )

add_executable(test_kalman EXCLUDE_FROM_ALL tests/test_kalman.cc )
target_link_libraries(test_kalman RavlKalmanFilter ${TEST_LIBS} )
add_test(test_kalman test_kalman)
install(TARGETS test_kalman DESTINATION tests COMPONENT unittests EXCLUDE_FROM_ALL)






