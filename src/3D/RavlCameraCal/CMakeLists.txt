
# Library: RavlCameraCal

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

set(HAVE_RavlCameraCal 1)

include_directories(../../Core/RavlCore/include)
include_directories(../../Image/RavlImage/include)
include_directories(../../Math/RavlMath/include)




add_library(RavlCameraCal STATIC include/Ravl/3D/PinholeCamera.hh include/Ravl/3D/PinholeCamera0.hh include/Ravl/3D/PinholeCamera1.hh include/Ravl/3D/PinholeCamera2.hh include/Ravl/3D/PinholeCamera3.hh include/Ravl/3D/PinholeCameraArray.hh src/PinholeCamera0.cc src/PinholeCamera1.cc src/PinholeCamera2.cc src/PinholeCamera3.cc src/PinholeCameraArray.cc)
target_link_libraries(RavlCameraCal PUBLIC RavlCore RavlImage RavlMath )


target_include_directories(
    RavlCameraCal PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
    )



install(TARGETS RavlCameraCal EXPORT RavlCameraCalConfig
   ARCHIVE  DESTINATION lib
   LIBRARY  DESTINATION lib
   RUNTIME  DESTINATION bin)  # This is for Windows

install(EXPORT RavlCameraCalConfig DESTINATION lib/cmake/RavlCameraCal/Target)
install(TARGETS RavlCameraCal DESTINATION lib)

configure_package_config_file(cmake/RavlCameraCalConfig.cmake.in
  ${CMAKE_CURRENT_BINARY_DIR}/RavlCameraCalConfig.cmake
  INSTALL_DESTINATION lib/cmake/RavlCameraCal
  PATH_VARS)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/RavlCameraCalConfig.cmake
        DESTINATION lib/cmake/RavlCameraCal )

install(DIRECTORY include DESTINATION .)










