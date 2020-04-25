
# Library: doBatchProcess

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

set(HAVE_doBatchProcess 1)

include_directories(../../OS/RavlOSIO/include)
include_directories(../../PatternRec/RavlPatternRec/include)
include_directories(../../Audio/RavlAudioFeatures/include)
include_directories(../../Audio/RavlAudioIO/include)
include_directories(../../Core/RavlCore/include)
include_directories(../../OS/RavlXMLFactory/include)
include_directories(../../OS/RavlThreads/include)
include_directories(../../Core/RavlIO/include)
include_directories(../../OS/RavlOS/include)



if(DEFINED HAVE_RavlAudioFile)
include_directories(../../Contrib/RavlAudioFile/include)
endif()


# Mains
set(MAIN_LIBS  RavlOSIO RavlPatternRec RavlAudioFeatures RavlAudioIO RavlCore RavlXMLFactory RavlThreads RavlIO RavlOS  )

add_executable(doBatchProcess applications/doBatchProcess.cc )
target_link_libraries(doBatchProcess  ${MAIN_LIBS} )
install(TARGETS doBatchProcess DESTINATION bin)




# === Files for share/Ravl ===
set(AUXDIR share/Ravl)
set(AUXFILES 
  data/doBatchProcess.xml )
file(COPY ${AUXFILES} DESTINATION ${BUILD_ROOT_AUXFILE}/${AUXDIR})
install(FILES ${AUXFILES} DESTINATION ${AUXDIR})





