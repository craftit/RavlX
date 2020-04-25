
# Library: Ravl3D

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

set(HAVE_Ravl3D 1)

include_directories(../../Math/RavlMath/include)
include_directories(../../Image/RavlImage/include)
include_directories(../../Core/RavlCore/include)
include_directories(../../Core/RavlIO/include)



if(DEFINED HAVE_RavlDPDisplay)
include_directories(../../GUI/RavlDPDisplay/include)
endif()
if(DEFINED HAVE_RavlDPDisplay3d)
include_directories(../../GUI/RavlDPDisplay3d/include)
endif()

add_library(Ravl3D STATIC include/Ravl/3D/Vertex.hh include/Ravl/3D/Tri.hh include/Ravl/3D/TriMesh.hh include/Ravl/3D/TexTriMesh.hh include/Ravl/3D/HEMeshVertex.hh include/Ravl/3D/HEMeshEdge.hh include/Ravl/3D/HEMeshFace.hh include/Ravl/3D/HEMesh.hh include/Ravl/3D/HEMeshVertexIter.hh include/Ravl/3D/HEMeshFaceIter.hh include/Ravl/3D/VertexColourByteRGB.hh include/Ravl/3D/MeshShapes.hh include/Ravl/3D/BuildTexTriMesh.hh include/Ravl/3D/SurfacePoint3dArray.hh src/Vertex.cc src/Tri.cc src/TriMesh.cc src/TriMeshBinIO.cc src/TexTriMesh.cc src/TexTriMeshBinIO.cc src/HEMeshVertex.cc src/HEMeshEdge.cc src/HEMeshFace.cc src/HEMesh.cc src/TriMesh2HEMesh.cc src/HEMeshFaceIter.cc src/VertexColourByteRGB.cc src/MeshShapes.cc src/BuildTexTriMesh.cc src/SurfacePoint3dArray.cc)
target_link_libraries(Ravl3D PUBLIC RavlMath RavlImage RavlCore RavlIO )


target_include_directories(
    Ravl3D PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
    )



install(TARGETS Ravl3D EXPORT Ravl3DConfig
   ARCHIVE  DESTINATION lib
   LIBRARY  DESTINATION lib
   RUNTIME  DESTINATION bin)  # This is for Windows

install(EXPORT Ravl3DConfig DESTINATION lib/cmake/Ravl3D/Target)
install(TARGETS Ravl3D DESTINATION lib)

configure_package_config_file(cmake/Ravl3DConfig.cmake.in
  ${CMAKE_CURRENT_BINARY_DIR}/Ravl3DConfig.cmake
  INSTALL_DESTINATION lib/cmake/Ravl3D
  PATH_VARS)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/Ravl3DConfig.cmake
        DESTINATION lib/cmake/Ravl3D )

install(DIRECTORY include DESTINATION .)



# Examples
set(EXAMPLE_LIBS  RavlMath RavlImage RavlCore RavlIO  )

add_executable(exBuildMesh EXCLUDE_FROM_ALL examples/exBuildMesh.cc )
target_link_libraries(exBuildMesh Ravl3D ${EXAMPLE_LIBS} )
install(TARGETS exBuildMesh DESTINATION examples COMPONENT examples EXCLUDE_FROM_ALL)

# Tests
set(TEST_LIBS  RavlMath RavlImage RavlCore RavlIO  )

add_executable(testHEMesh EXCLUDE_FROM_ALL tests/testHEMesh.cc )
target_link_libraries(testHEMesh Ravl3D ${TEST_LIBS} )
add_test(testHEMesh testHEMesh)
install(TARGETS testHEMesh DESTINATION tests COMPONENT unittests EXCLUDE_FROM_ALL)

add_executable(testTriMesh EXCLUDE_FROM_ALL tests/testTriMesh.cc )
target_link_libraries(testTriMesh Ravl3D ${TEST_LIBS} )
add_test(testTriMesh testTriMesh)
install(TARGETS testTriMesh DESTINATION tests COMPONENT unittests EXCLUDE_FROM_ALL)






