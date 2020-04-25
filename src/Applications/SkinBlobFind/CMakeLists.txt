
# Library: SkinBlobFind

# Requires: 
# Supports: !arm

if((NOT ${CMAKE_SYSTEM_PROCESSOR} MATCHES "arm") OR FALSE)




set(DEPENDS_ON )
foreach(D IN LISTS DEPENDS_ON)
  if(NOT ${D}_FOUND)
    find_package(${D} REQUIRED)
  endif()
endforeach()

include(CMakePackageConfigHelpers)

include_directories(include)
include_directories(src)

set(HAVE_SkinBlobFind 1)

include_directories(../../Image/RavlImage/include)
include_directories(../../Image/RavlImageIO/include)
include_directories(../../Image/RavlExtImgIO/include)
include_directories(../../OS/RavlOSIO/include)
include_directories(../../Image/RavlImageProc/include)
include_directories(../../Core/RavlCore/include)
include_directories(../../Core/RavlIO/include)
include_directories(../../Math/RavlMath/include)
include_directories(../../OS/RavlOS/include)





# Mains
set(MAIN_LIBS  RavlImage RavlImageIO RavlExtImgIO RavlOSIO RavlImageProc RavlCore RavlIO RavlMath RavlOS  )

add_executable(SkinBlobFind ${EXCLUDE_APPLICATIONS} applications/SkinBlobFind.cc )
target_link_libraries(SkinBlobFind  ${MAIN_LIBS} )
install(TARGETS SkinBlobFind DESTINATION bin ${EXCLUDE_APPLICATIONS})







ENDIF()
