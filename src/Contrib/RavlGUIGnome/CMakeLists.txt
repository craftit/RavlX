
# Library: RavlGUIGnome

# Requires: LibGnome libGTK2
# Supports: 



if(libgnome_FOUND)
if(GTK2_FOUND)



set(DEPENDS_ON )
foreach(D IN LISTS DEPENDS_ON)
  if(NOT ${D}_FOUND)
    find_package(${D} REQUIRED)
  endif()
endforeach()

include(CMakePackageConfigHelpers)

include_directories(include)
include_directories(src)

set(HAVE_RavlGUIGnome 1)

include_directories(../../GUI/RavlGUI/include)
include_directories(../../Core/RavlCore/include)
include_directories(../../OS/RavlThreads/include)
include_directories(../../Image/RavlImage/include)




add_library(RavlGUIGnome STATIC include/Ravl/GUI/Gnome.hh src/Gnome.cc)
target_link_libraries(RavlGUIGnome PUBLIC RavlGUI ${libgnome_LIBRARIES} RavlCore RavlThreads RavlImage )


target_include_directories(
    RavlGUIGnome PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
    )



install(TARGETS RavlGUIGnome EXPORT RavlGUIGnomeConfig
   ARCHIVE  DESTINATION lib
   LIBRARY  DESTINATION lib
   RUNTIME  DESTINATION bin)  # This is for Windows

install(EXPORT RavlGUIGnomeConfig DESTINATION lib/cmake/RavlGUIGnome/Target)
install(TARGETS RavlGUIGnome DESTINATION lib)

configure_package_config_file(cmake/RavlGUIGnomeConfig.cmake.in
  ${CMAKE_CURRENT_BINARY_DIR}/RavlGUIGnomeConfig.cmake
  INSTALL_DESTINATION lib/cmake/RavlGUIGnome
  PATH_VARS)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/RavlGUIGnomeConfig.cmake
        DESTINATION lib/cmake/RavlGUIGnome )

install(DIRECTORY include DESTINATION .)



# Examples
set(EXAMPLE_LIBS  RavlGUI ${libgnome_LIBRARIES} RavlCore RavlThreads RavlImage  )

add_executable(exGnome EXCLUDE_FROM_ALL examples/exGnome.cc )
target_link_libraries(exGnome RavlGUIGnome ${EXAMPLE_LIBS} )
install(TARGETS exGnome DESTINATION examples COMPONENT examples EXCLUDE_FROM_ALL)





ENDIF()
ENDIF()


