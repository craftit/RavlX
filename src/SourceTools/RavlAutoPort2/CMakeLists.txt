
# Library: RavlAutoPort2

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

set(HAVE_RavlAutoPort2 1)

include_directories(../../OS/RavlOS/include)
include_directories(../../SourceTools/RavlSourceTools/include)
include_directories(../../Core/RavlCore/include)
include_directories(../../Core/RavlIO/include)
include_directories(../../Math/RavlMath/include)




add_library(RavlAutoPort2 STATIC include/Ravl/AutoPort2/AutoPortSource.hh include/Ravl/AutoPort2/AutoPortGenerator.hh include/Ravl/AutoPort2/ExtLib.hh include/Ravl/AutoPort2/ExtLibTable.hh src/AutoPortSource.cc src/AutoPortGenerator.cc src/ExtLib.cc src/ExtLibTable.cc)
target_link_libraries(RavlAutoPort2 PUBLIC RavlOS RavlSourceTools RavlCore RavlIO RavlMath )


target_include_directories(
    RavlAutoPort2 PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
    )



install(TARGETS RavlAutoPort2 EXPORT RavlAutoPort2Config
   ARCHIVE  DESTINATION lib
   LIBRARY  DESTINATION lib
   RUNTIME  DESTINATION bin)  # This is for Windows

install(EXPORT RavlAutoPort2Config DESTINATION lib/cmake/RavlAutoPort2/Target)
install(TARGETS RavlAutoPort2 DESTINATION lib)

configure_package_config_file(cmake/RavlAutoPort2Config.cmake.in
  ${CMAKE_CURRENT_BINARY_DIR}/RavlAutoPort2Config.cmake
  INSTALL_DESTINATION lib/cmake/RavlAutoPort2
  PATH_VARS)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/RavlAutoPort2Config.cmake
        DESTINATION lib/cmake/RavlAutoPort2 )

install(DIRECTORY include DESTINATION .)


# Mains
set(MAIN_LIBS  RavlOS RavlSourceTools RavlCore RavlIO RavlMath  )

add_executable(autoport2 ${EXCLUDE_APPLICATIONS} applications/autoport2.cc )
target_link_libraries(autoport2 RavlAutoPort2 ${MAIN_LIBS} )
install(TARGETS autoport2 DESTINATION bin ${EXCLUDE_APPLICATIONS})




# === Files for share/RAVL/AutoPort2 ===
set(AUXDIR share/RAVL/AutoPort2)
set(AUXFILES 
  data/header.tmpl 
  data/guid.tmpl 
  data/externalLib.xml 
  data/sln.tmpl 
  data/vcproj.tmpl 
  data/solution.guid.tmpl 
  data/testProgram.guid.tmpl 
  data/testProgram.tmpl 
  data/exampleProgram.tmpl 
  data/mainProgram.tmpl 
  data/program.stmpl 
  data/mainProgram.guid.tmpl 
  data/exampleProgram.guid.tmpl )
file(COPY ${AUXFILES} DESTINATION ${BUILD_ROOT_AUXFILE}/${AUXDIR})
install(FILES ${AUXFILES} DESTINATION ${AUXDIR})





