
# Library: VGrab

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

set(HAVE_VGrab 1)

include_directories(../../Core/RavlCore/include)
include_directories(../../OS/RavlOS/include)
include_directories(../../Core/RavlIO/include)
include_directories(../../Image/RavlVideoIO/include)
include_directories(../../Image/RavlImage/include)
include_directories(../../OS/RavlOSIO/include)
include_directories(../../Image/RavlImageIO/include)



if(DEFINED HAVE_RavlDV)
include_directories(../../Contrib/RavlDV/include)
endif()
if(DEFINED HAVE_RavlImgIOV4L)
include_directories(../../Contrib/RavlImgIOV4L/include)
endif()
if(DEFINED HAVE_RavlExtImgIO)
include_directories(../../Image/RavlExtImgIO/include)
endif()
if(DEFINED HAVE_RavlDPDisplay)
include_directories(../../GUI/RavlDPDisplay/include)
endif()
if(DEFINED HAVE_RavlMPEG2)
include_directories(../../Contrib/RavlMPEG2/include)
endif()
if(DEFINED HAVE_CSPDriver)
include_directories(../../Contrib/CSPDriver/include)
endif()


# Mains
set(MAIN_LIBS  RavlCore RavlOS RavlIO RavlVideoIO RavlImage RavlOSIO RavlImageIO  )

add_executable(VGrab applications/VGrab.cc )
target_link_libraries(VGrab  ${MAIN_LIBS} )
install(TARGETS VGrab DESTINATION bin)







ENDIF()
