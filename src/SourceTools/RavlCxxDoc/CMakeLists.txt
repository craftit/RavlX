
# Library: RavlCxxDoc

# Requires: 
# Supports: !VCPP

if((NOT ${CMAKE_CXX_COMPILER_ID} MATCHES "MSVC") OR FALSE)




set(DEPENDS_ON )
foreach(D IN LISTS DEPENDS_ON)
  if(NOT ${D}_FOUND)
    find_package(${D} REQUIRED)
  endif()
endforeach()

include(CMakePackageConfigHelpers)

include_directories(include)
include_directories(src)

set(HAVE_RavlCxxDoc 1)

include_directories(../../Core/RavlCore/include)
include_directories(../../OS/RavlOS/include)
include_directories(../../SourceTools/RavlSourceTools/include)




add_library(RavlCxxDoc STATIC include/Ravl/CxxDoc/Object.hh include/Ravl/CxxDoc/Document.hh include/Ravl/CxxDoc/Parser.hh include/Ravl/CxxDoc/CxxElements.hh include/Ravl/CxxDoc/CxxScope.hh include/Ravl/CxxDoc/DocNode.hh include/Ravl/CxxDoc/DocTree.hh include/Ravl/CxxDoc/Strings.hh include/Ravl/CxxDoc/Executables.hh src/Object.cc src/Document.cc src/Parser.cc src/CxxElements.cc src/CxxScope.cc src/DocExe.cc src/DocNode.cc src/DocTree.cc src/Strings.cc src/Executables.cc src/cxx.tab.cc src/tokenizer.yy.cc)
target_link_libraries(RavlCxxDoc PUBLIC RavlCore RavlOS RavlSourceTools )


target_include_directories(
    RavlCxxDoc PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
    )



install(TARGETS RavlCxxDoc EXPORT RavlCxxDocConfig
   ARCHIVE  DESTINATION lib
   LIBRARY  DESTINATION lib
   RUNTIME  DESTINATION bin)  # This is for Windows

install(EXPORT RavlCxxDocConfig DESTINATION lib/cmake/RavlCxxDoc/Target)
install(TARGETS RavlCxxDoc DESTINATION lib)

configure_package_config_file(cmake/RavlCxxDocConfig.cmake.in
  ${CMAKE_CURRENT_BINARY_DIR}/RavlCxxDocConfig.cmake
  INSTALL_DESTINATION lib/cmake/RavlCxxDoc
  PATH_VARS)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/RavlCxxDocConfig.cmake
        DESTINATION lib/cmake/RavlCxxDoc )

install(DIRECTORY include DESTINATION .)


# Mains
set(MAIN_LIBS  RavlCore RavlOS RavlSourceTools  )

add_executable(CxxDoc ${EXCLUDE_APPLICATIONS} applications/CxxDoc.cc )
target_link_libraries(CxxDoc RavlCxxDoc ${MAIN_LIBS} )
install(TARGETS CxxDoc DESTINATION bin ${EXCLUDE_APPLICATIONS})







ENDIF()
