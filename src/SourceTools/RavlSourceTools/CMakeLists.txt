
# Library: RavlSourceTools

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

set(HAVE_RavlSourceTools 1)

include_directories(../../Core/RavlCore/include)
include_directories(../../OS/RavlOS/include)




add_library(RavlSourceTools STATIC include/Ravl/SourceTools/DefsFile.hh include/Ravl/SourceTools/SourceCodeManager.hh include/Ravl/SourceTools/RCSFile.hh include/Ravl/SourceTools/SourceFile.hh include/Ravl/SourceTools/LibInfo.hh include/Ravl/SourceTools/ProgInfo.hh include/Ravl/SourceTools/BuildInfo.hh src/DefsFile.cc src/SourceCodeManager.cc src/RCSFile.cc src/SourceFile.cc src/LibInfo.cc src/ProgInfo.cc src/BuildInfo.cc)
target_link_libraries(RavlSourceTools PUBLIC RavlCore RavlOS )


target_include_directories(
    RavlSourceTools PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
    )



install(TARGETS RavlSourceTools EXPORT RavlSourceToolsConfig
   ARCHIVE  DESTINATION lib
   LIBRARY  DESTINATION lib
   RUNTIME  DESTINATION bin)  # This is for Windows

install(EXPORT RavlSourceToolsConfig DESTINATION lib/cmake/RavlSourceTools/Target)
install(TARGETS RavlSourceTools DESTINATION lib)

configure_package_config_file(cmake/RavlSourceToolsConfig.cmake.in
  ${CMAKE_CURRENT_BINARY_DIR}/RavlSourceToolsConfig.cmake
  INSTALL_DESTINATION lib/cmake/RavlSourceTools
  PATH_VARS)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/RavlSourceToolsConfig.cmake
        DESTINATION lib/cmake/RavlSourceTools )

install(DIRECTORY include DESTINATION .)


# Mains
set(MAIN_LIBS  RavlCore RavlOS  )

add_executable(defsCheck ${EXCLUDE_APPLICATIONS} applications/defsCheck.cc )
target_link_libraries(defsCheck RavlSourceTools ${MAIN_LIBS} )
install(TARGETS defsCheck DESTINATION bin ${EXCLUDE_APPLICATIONS})

add_executable(annotate ${EXCLUDE_APPLICATIONS} applications/annotate.cc )
target_link_libraries(annotate RavlSourceTools ${MAIN_LIBS} )
install(TARGETS annotate DESTINATION bin ${EXCLUDE_APPLICATIONS})

add_executable(amma2ravl ${EXCLUDE_APPLICATIONS} applications/amma2ravl.cc )
target_link_libraries(amma2ravl RavlSourceTools ${MAIN_LIBS} )
install(TARGETS amma2ravl DESTINATION bin ${EXCLUDE_APPLICATIONS})

add_executable(docSubst ${EXCLUDE_APPLICATIONS} applications/docSubst.cc )
target_link_libraries(docSubst RavlSourceTools ${MAIN_LIBS} )
install(TARGETS docSubst DESTINATION bin ${EXCLUDE_APPLICATIONS})

add_executable(docRename ${EXCLUDE_APPLICATIONS} applications/docRename.cc )
target_link_libraries(docRename RavlSourceTools ${MAIN_LIBS} )
install(TARGETS docRename DESTINATION bin ${EXCLUDE_APPLICATIONS})

# Examples
set(EXAMPLE_LIBS  RavlCore RavlOS  )

add_executable(exBuildInfo ${EXCLUDE_EXAMPLES} examples/exBuildInfo.cc )
target_link_libraries(exBuildInfo RavlSourceTools ${EXAMPLE_LIBS} )
install(TARGETS exBuildInfo DESTINATION examples COMPONENT examples ${EXCLUDE_EXAMPLES})



# === Files for share/RAVL/CodeManager ===
set(AUXDIR share/RAVL/CodeManager)
set(AUXFILES 
  data/fixSource.cfg )
file(COPY ${AUXFILES} DESTINATION ${BUILD_ROOT_AUXFILE}/${AUXDIR})
install(FILES ${AUXFILES} DESTINATION ${AUXDIR})





