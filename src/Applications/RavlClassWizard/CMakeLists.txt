
# Library: RavlClassWizard

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

set(HAVE_RavlClassWizard 1)

include_directories(../../SourceTools/RavlCxxDoc/include)
include_directories(../../Core/RavlCore/include)
include_directories(../../OS/RavlOS/include)
include_directories(../../SourceTools/RavlSourceTools/include)




add_library(RavlClassWizard STATIC include/Ravl/SourceTools/ClassWizard.hh include/Ravl/SourceTools/SourceCursor.hh src/ClassWizard.cc src/SourceCursor.cc)
target_link_libraries(RavlClassWizard PUBLIC RavlCxxDoc RavlCore RavlOS RavlSourceTools )


target_include_directories(
    RavlClassWizard PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
    )



install(TARGETS RavlClassWizard EXPORT RavlClassWizardConfig
   ARCHIVE  DESTINATION lib
   LIBRARY  DESTINATION lib
   RUNTIME  DESTINATION bin)  # This is for Windows

install(EXPORT RavlClassWizardConfig DESTINATION lib/cmake/RavlClassWizard/Target)
install(TARGETS RavlClassWizard DESTINATION lib)

configure_package_config_file(cmake/RavlClassWizardConfig.cmake.in
  ${CMAKE_CURRENT_BINARY_DIR}/RavlClassWizardConfig.cmake
  INSTALL_DESTINATION lib/cmake/RavlClassWizard
  PATH_VARS)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/RavlClassWizardConfig.cmake
        DESTINATION lib/cmake/RavlClassWizard )

install(DIRECTORY include DESTINATION .)


# Mains
set(MAIN_LIBS  RavlCxxDoc RavlCore RavlOS RavlSourceTools  )

add_executable(cwiz ${EXCLUDE_APPLICATIONS} applications/cwiz.cc )
target_link_libraries(cwiz RavlClassWizard ${MAIN_LIBS} )
install(TARGETS cwiz DESTINATION bin ${EXCLUDE_APPLICATIONS})







ENDIF()
