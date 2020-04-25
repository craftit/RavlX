
# Library: RavlMemModules

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

set(HAVE_RavlMemModules 1)

include_directories(../../Core/RavlCore/include)
include_directories(../../Image/RavlImage/include)
include_directories(../../OS/RavlOS/include)
include_directories(../../OS/RavlThreads/include)
include_directories(../../Image/RavlImageIO/include)
include_directories(../../Core/RavlIO/include)
include_directories(../../Math/RavlMath/include)
include_directories(../../Image/RavlExtImgIO/include)




add_library(RavlMemModules STATIC include/Ravl/Threads/MemModule.hh include/Ravl/Threads/MemModules.hh include/Ravl/Threads/MemItem.hh include/Ravl/Threads/MemUnit.hh include/Ravl/Threads/MemIter.hh src/MemModule.cc src/MemModules.cc src/MemItem.cc src/MemUnit.cc src/MemIter.cc)
target_link_libraries(RavlMemModules PUBLIC RavlCore RavlImage RavlOS RavlThreads RavlImageIO RavlIO RavlMath )


target_include_directories(
    RavlMemModules PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
    )



install(TARGETS RavlMemModules EXPORT RavlMemModulesConfig
   ARCHIVE  DESTINATION lib
   LIBRARY  DESTINATION lib
   RUNTIME  DESTINATION bin)  # This is for Windows

install(EXPORT RavlMemModulesConfig DESTINATION lib/cmake/RavlMemModules/Target)
install(TARGETS RavlMemModules DESTINATION lib)

configure_package_config_file(cmake/RavlMemModulesConfig.cmake.in
  ${CMAKE_CURRENT_BINARY_DIR}/RavlMemModulesConfig.cmake
  INSTALL_DESTINATION lib/cmake/RavlMemModules
  PATH_VARS)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/RavlMemModulesConfig.cmake
        DESTINATION lib/cmake/RavlMemModules )

install(DIRECTORY include DESTINATION .)




# Tests
set(TEST_LIBS  RavlCore RavlImage RavlOS RavlThreads RavlImageIO RavlIO RavlMath RavlExtImgIO  )

add_executable(testMemModules EXCLUDE_FROM_ALL tests/testMemModules.cc )
target_link_libraries(testMemModules RavlMemModules ${TEST_LIBS} )
add_test(testMemModules testMemModules)
install(TARGETS testMemModules DESTINATION tests COMPONENT unittests EXCLUDE_FROM_ALL)


# === Files for share/doc/Ravl/Images ===
set(AUXDIR share/doc/Ravl/Images)
set(AUXFILES 
  data/ModuleGraph.pdf 
  data/Memory.pdf )
file(COPY ${AUXFILES} DESTINATION ${BUILD_ROOT_AUXFILE}/${AUXDIR})
install(FILES ${AUXFILES} DESTINATION ${AUXDIR})





