
# Library: Ravl3DIO

# Requires: 
# Supports: 




# MustLink: src/RavlMeshIO.cc


set(DEPENDS_ON )
foreach(D IN LISTS DEPENDS_ON)
  if(NOT ${D}_FOUND)
    find_package(${D} REQUIRED)
  endif()
endforeach()

include(CMakePackageConfigHelpers)

include_directories(include)
include_directories(src)

set(HAVE_Ravl3DIO 1)

include_directories(../../3D/Ravl3D/include)
include_directories(../../Core/RavlIO/include)
include_directories(../../OS/RavlOS/include)
include_directories(../../Core/RavlCore/include)
include_directories(../../Math/RavlMath/include)
include_directories(../../Image/RavlImage/include)




add_library(Ravl3DIO_Body STATIC
 include/Ravl/3D/RavlMeshIO.hh include/Ravl/3D/VRMLFileIO.hh include/Ravl/3D/FormatVRMLFile.hh include/Ravl/3D/CTriFileIO.hh include/Ravl/3D/FormatCTriFile.hh include/Ravl/3D/TriFileIO.hh include/Ravl/3D/FormatTriFile.hh include/Ravl/3D/MeshIOObj.hh include/Ravl/3D/FormatMeshObj.hh include/Ravl/3D/POVRayFileIO.hh include/Ravl/3D/FormatPOVRayFile.hh
 src/VRMLFileIO.cc src/FormatVRMLFile.cc src/CTriFileIO.cc src/FormatCTriFile.cc src/TriFileIO.cc src/FormatTriFile.cc src/MeshIOObj.cc src/FormatMeshObj.cc src/TriMeshIO.cc src/TexTriMeshIO.cc src/POVRayFileIO.cc src/FormatPOVRayFile.cc src/MeshConv.cc
)
target_link_libraries(Ravl3DIO_Body PUBLIC Ravl3D RavlIO RavlOS RavlCore RavlMath RavlImage )

add_library(Ravl3DIO OBJECT src/RavlMeshIO.cc )
target_link_libraries(Ravl3DIO PUBLIC Ravl3DIO_Body )


target_include_directories(
    Ravl3DIO PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
    )

# MustLink fixes:
install(EXPORT Ravl3DIO_BodyConfig DESTINATION lib/cmake/Ravl3DIO_Body/Target)
install(TARGETS Ravl3DIO_Body EXPORT Ravl3DIO_BodyConfig
   ARCHIVE  DESTINATION lib
   LIBRARY  DESTINATION lib
   RUNTIME  DESTINATION bin)  # This is for Windows
install(TARGETS Ravl3DIO_Body DESTINATION lib)

configure_package_config_file(cmake/Ravl3DIO_BodyConfig.cmake.in
  ${CMAKE_CURRENT_BINARY_DIR}/Ravl3DIO_BodyConfig.cmake
  INSTALL_DESTINATION lib/cmake/Ravl3DIO_Body
  PATH_VARS)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/Ravl3DIO_BodyConfig.cmake
        DESTINATION lib/cmake/Ravl3DIO_Body )


install(TARGETS Ravl3DIO EXPORT Ravl3DIOConfig
   ARCHIVE  DESTINATION lib
   LIBRARY  DESTINATION lib
   RUNTIME  DESTINATION bin)  # This is for Windows

install(EXPORT Ravl3DIOConfig DESTINATION lib/cmake/Ravl3DIO/Target)
install(TARGETS Ravl3DIO DESTINATION lib)

configure_package_config_file(cmake/Ravl3DIOConfig.cmake.in
  ${CMAKE_CURRENT_BINARY_DIR}/Ravl3DIOConfig.cmake
  INSTALL_DESTINATION lib/cmake/Ravl3DIO
  PATH_VARS)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/Ravl3DIOConfig.cmake
        DESTINATION lib/cmake/Ravl3DIO )

install(DIRECTORY include DESTINATION .)




# Tests
set(TEST_LIBS  Ravl3D RavlIO RavlOS RavlCore RavlMath RavlImage  )

add_executable(testTriFileIO ${EXCLUDE_UNITTESTS} tests/testTriFileIO.cc )
target_link_libraries(testTriFileIO Ravl3DIO ${TEST_LIBS} )
add_test(testTriFileIO testTriFileIO)
install(TARGETS testTriFileIO DESTINATION tests COMPONENT unittests ${EXCLUDE_UNITTESTS})


# === Files for share/RAVL/data ===
set(AUXDIR share/RAVL/data)
set(AUXFILES 
  data/cube.tri )
file(COPY ${AUXFILES} DESTINATION ${BUILD_ROOT_AUXFILE}/${AUXDIR})
install(FILES ${AUXFILES} DESTINATION ${AUXDIR})





