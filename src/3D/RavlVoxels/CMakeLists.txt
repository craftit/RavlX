
# Library: RavlVoxels

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

set(HAVE_RavlVoxels 1)

include_directories(../../3D/Ravl3D/include)
include_directories(../../Core/RavlCore/include)
include_directories(../../Math/RavlMath/include)




add_library(RavlVoxels STATIC include/Ravl/Voxels/Ray3d.hh include/Ravl/Voxels/VoxelGrid.hh include/Ravl/Voxels/VoxeliseMesh.hh include/Ravl/Voxels/VoxelOctree.hh include/Ravl/Voxels/VoxelSet.hh include/Ravl/Voxels/VoxelSetIter.hh include/Ravl/Voxels/VoxelRayIter.hh include/Ravl/Voxels/VoxelSkeleton.hh include/Ravl/Voxels/VoxelSurface.hh src/VoxeliseMesh.cc src/VoxelRayIter.cc src/VoxelSkeleton.cc src/VoxelSurface.cc)
target_link_libraries(RavlVoxels PUBLIC Ravl3D RavlCore RavlMath )


target_include_directories(
    RavlVoxels PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
    )



install(TARGETS RavlVoxels EXPORT RavlVoxelsConfig
   ARCHIVE  DESTINATION lib
   LIBRARY  DESTINATION lib
   RUNTIME  DESTINATION bin)  # This is for Windows

install(EXPORT RavlVoxelsConfig DESTINATION lib/cmake/RavlVoxels/Target)
install(TARGETS RavlVoxels DESTINATION lib)

configure_package_config_file(cmake/RavlVoxelsConfig.cmake.in
  ${CMAKE_CURRENT_BINARY_DIR}/RavlVoxelsConfig.cmake
  INSTALL_DESTINATION lib/cmake/RavlVoxels
  PATH_VARS)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/RavlVoxelsConfig.cmake
        DESTINATION lib/cmake/RavlVoxels )

install(DIRECTORY include DESTINATION .)










