
# Library: q2cmake

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

set(HAVE_q2cmake 1)

include_directories(../../OS/RavlOS/include)
include_directories(../../SourceTools/RavlSourceTools/include)





# Mains
set(MAIN_LIBS  RavlOS RavlSourceTools  )

add_executable(q2cmake ${EXCLUDE_APPLICATIONS} applications/q2cmake.cc )
target_link_libraries(q2cmake  ${MAIN_LIBS} )
install(TARGETS q2cmake DESTINATION bin ${EXCLUDE_APPLICATIONS})




# === Files for share/RAVL/AutoPort ===
set(AUXDIR share/RAVL/AutoPort)
set(AUXFILES 
  data/CMakeLists.txt.tmpl 
  data/CMakeRoot.txt.tmpl 
  data/CMakeMid.txt.tmpl 
  data/CMakeConfig.cmake.tmpl )
file(COPY ${AUXFILES} DESTINATION ${BUILD_ROOT_AUXFILE}/${AUXDIR})
install(FILES ${AUXFILES} DESTINATION ${AUXDIR})




ENDIF()
