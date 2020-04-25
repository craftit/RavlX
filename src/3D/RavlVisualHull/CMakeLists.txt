
# Library: RavlVisualHull

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

set(HAVE_RavlVisualHull 1)

include_directories(../../3D/RavlVoxels/include)
include_directories(../../Core/RavlCore/include)
include_directories(../../3D/Ravl3D/include)
include_directories(../../Core/RavlIO/include)
include_directories(../../Math/RavlMath/include)
include_directories(../../3D/RavlCameraCal/include)
include_directories(../../Image/RavlImage/include)




add_library(RavlVisualHull STATIC include/Ravl/VisualHull/MovingLeastSquares.hh include/Ravl/VisualHull/VisualHull.hh include/Ravl/VisualHull/VoxelCarve.hh include/Ravl/VisualHull/VoxelClean.hh include/Ravl/VisualHull/VoxelColourCarve.hh include/Ravl/VisualHull/VoxelDistanceTransform.hh src/MovingLeastSquares.cc src/VisualHull.cc src/VoxelCarve.cc src/VoxelClean.cc src/VoxelColourCarve.cc src/VoxelDistanceTransform.cc)
target_link_libraries(RavlVisualHull PUBLIC RavlVoxels RavlCore Ravl3D RavlIO RavlMath RavlCameraCal RavlImage )


target_include_directories(
    RavlVisualHull PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
    )



install(TARGETS RavlVisualHull EXPORT RavlVisualHullConfig
   ARCHIVE  DESTINATION lib
   LIBRARY  DESTINATION lib
   RUNTIME  DESTINATION bin)  # This is for Windows

install(EXPORT RavlVisualHullConfig DESTINATION lib/cmake/RavlVisualHull/Target)
install(TARGETS RavlVisualHull DESTINATION lib)

configure_package_config_file(cmake/RavlVisualHullConfig.cmake.in
  ${CMAKE_CURRENT_BINARY_DIR}/RavlVisualHullConfig.cmake
  INSTALL_DESTINATION lib/cmake/RavlVisualHull
  PATH_VARS)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/RavlVisualHullConfig.cmake
        DESTINATION lib/cmake/RavlVisualHull )

install(DIRECTORY include DESTINATION .)










