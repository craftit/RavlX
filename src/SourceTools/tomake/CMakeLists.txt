
# Library: tomake

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

set(HAVE_tomake 1)

include_directories(../../Core/RavlCore/include)
include_directories(../../OS/RavlOS/include)
include_directories(../../SourceTools/RavlSourceTools/include)





# Mains
set(MAIN_LIBS  RavlCore RavlOS RavlSourceTools  )

add_executable(tomake ${EXCLUDE_APPLICATIONS} applications/tomake.cc )
target_link_libraries(tomake  ${MAIN_LIBS} )
install(TARGETS tomake DESTINATION bin ${EXCLUDE_APPLICATIONS})




# === Files for share/RAVL/AutoPort ===
set(AUXDIR share/RAVL/AutoPort)
set(AUXFILES 
  data/Makefile.tmpl )
file(COPY ${AUXFILES} DESTINATION ${BUILD_ROOT_AUXFILE}/${AUXDIR})
install(FILES ${AUXFILES} DESTINATION ${AUXDIR})




ENDIF()
