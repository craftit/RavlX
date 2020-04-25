
# Library: RavlOS

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

set(HAVE_RavlOS 1)

include_directories(../../Core/RavlCore/include)
include_directories(../../Core/RavlIO/include)




add_library(RavlOS STATIC include/Ravl/OS/Date.hh include/Ravl/OS/DateTZ.hh include/Ravl/OS/DeadLineTimer.hh include/Ravl/OS/DateRange.hh include/Ravl/OS/FilePermission.hh include/Ravl/OS/Filename.hh include/Ravl/OS/Directory.hh include/Ravl/OS/UserInfo.hh include/Ravl/OS/FileStream.hh include/Ravl/OS/UnixStreamIO.hh include/Ravl/OS/WinStreamIO.hh include/Ravl/OS/DMABuffer.hh include/Ravl/OS/SysLog.hh include/Ravl/OS/CharIO.hh include/Ravl/OS/OSProcess.hh include/Ravl/OS/ChildOSProcess.hh include/Ravl/OS/CPUInfo.hh include/Ravl/Text/TextFileLine.hh include/Ravl/Text/TextBuffer.hh include/Ravl/Text/TextFile.hh include/Ravl/Text/TextCursor.hh include/Ravl/Text/TemplateFile.hh include/Ravl/Text/TextFragment.hh include/Ravl/Text/ConfigFile.hh include/Ravl/Text/TemplateComplex.hh src/Date.cc src/DateTZ.cc src/DateIO.cc src/DeadLineTimer.cc src/DateRange.cc src/FilePermission.cc src/Filename.cc src/Directory.cc src/FileSearch.cc src/UserInfo.cc src/FileOwner.cc src/FileStream.cc src/UnixStreamIO.cc src/WinStreamIO.cc src/CharIO.cc src/OSProcess.cc src/ChildOSProcess.cc src/ChildOSProcessWait.cc src/ChildOSProcessStream.cc src/MailUser.cc src/FileUser.cc src/CPUInfo.cc src/TextFileLine.cc src/TextBuffer.cc src/TextFile.cc src/TextCursor.cc src/TemplateFile.cc src/TextFragment.cc src/ConfigFile.cc src/TemplateComplex.cc)
target_link_libraries(RavlOS PUBLIC RavlCore RavlIO )


target_include_directories(
    RavlOS PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
    )



install(TARGETS RavlOS EXPORT RavlOSConfig
   ARCHIVE  DESTINATION lib
   LIBRARY  DESTINATION lib
   RUNTIME  DESTINATION bin)  # This is for Windows

install(EXPORT RavlOSConfig DESTINATION lib/cmake/RavlOS/Target)
install(TARGETS RavlOS DESTINATION lib)

configure_package_config_file(cmake/RavlOSConfig.cmake.in
  ${CMAKE_CURRENT_BINARY_DIR}/RavlOSConfig.cmake
  INSTALL_DESTINATION lib/cmake/RavlOS
  PATH_VARS)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/RavlOSConfig.cmake
        DESTINATION lib/cmake/RavlOS )

install(DIRECTORY include DESTINATION .)


# Mains
set(MAIN_LIBS  RavlCore RavlIO  )

add_executable(exCPUInfo applications/exCPUInfo.cc )
target_link_libraries(exCPUInfo RavlOS ${MAIN_LIBS} )
install(TARGETS exCPUInfo DESTINATION bin)

# Examples
set(EXAMPLE_LIBS  RavlCore RavlIO  )

add_executable(exFilename EXCLUDE_FROM_ALL examples/exFilename.cc )
target_link_libraries(exFilename RavlOS ${EXAMPLE_LIBS} )
install(TARGETS exFilename DESTINATION examples COMPONENT examples EXCLUDE_FROM_ALL)

add_executable(exReadWithoutReturn EXCLUDE_FROM_ALL examples/exReadWithoutReturn.cc )
target_link_libraries(exReadWithoutReturn RavlOS ${EXAMPLE_LIBS} )
install(TARGETS exReadWithoutReturn DESTINATION examples COMPONENT examples EXCLUDE_FROM_ALL)

add_executable(exChild EXCLUDE_FROM_ALL examples/exChild.cc )
target_link_libraries(exChild RavlOS ${EXAMPLE_LIBS} )
install(TARGETS exChild DESTINATION examples COMPONENT examples EXCLUDE_FROM_ALL)

add_executable(exTemplateFile EXCLUDE_FROM_ALL examples/exTemplateFile.cc )
target_link_libraries(exTemplateFile RavlOS ${EXAMPLE_LIBS} )
install(TARGETS exTemplateFile DESTINATION examples COMPONENT examples EXCLUDE_FROM_ALL)

add_executable(exTemplateComplex EXCLUDE_FROM_ALL examples/exTemplateComplex.cc )
target_link_libraries(exTemplateComplex RavlOS ${EXAMPLE_LIBS} )
install(TARGETS exTemplateComplex DESTINATION examples COMPONENT examples EXCLUDE_FROM_ALL)

add_executable(exConfigFile EXCLUDE_FROM_ALL examples/exConfigFile.cc )
target_link_libraries(exConfigFile RavlOS ${EXAMPLE_LIBS} )
install(TARGETS exConfigFile DESTINATION examples COMPONENT examples EXCLUDE_FROM_ALL)

# Tests
set(TEST_LIBS  RavlCore RavlIO  )

add_executable(testDate EXCLUDE_FROM_ALL tests/testDate.cc )
target_link_libraries(testDate RavlOS ${TEST_LIBS} )
add_test(testDate testDate)
install(TARGETS testDate DESTINATION tests COMPONENT unittests EXCLUDE_FROM_ALL)

add_executable(testDeadLineTimer EXCLUDE_FROM_ALL tests/testDeadLineTimer.cc )
target_link_libraries(testDeadLineTimer RavlOS ${TEST_LIBS} )
add_test(testDeadLineTimer testDeadLineTimer)
install(TARGETS testDeadLineTimer DESTINATION tests COMPONENT unittests EXCLUDE_FROM_ALL)

add_executable(testFileFormat EXCLUDE_FROM_ALL tests/testFileFormat.cc )
target_link_libraries(testFileFormat RavlOS ${TEST_LIBS} )
add_test(testFileFormat testFileFormat)
install(TARGETS testFileFormat DESTINATION tests COMPONENT unittests EXCLUDE_FROM_ALL)

add_executable(testOSMisc EXCLUDE_FROM_ALL tests/testOSMisc.cc )
target_link_libraries(testOSMisc RavlOS ${TEST_LIBS} )
add_test(testOSMisc testOSMisc)
install(TARGETS testOSMisc DESTINATION tests COMPONENT unittests EXCLUDE_FROM_ALL)

add_executable(testChildProc EXCLUDE_FROM_ALL tests/testChildProc.cc )
target_link_libraries(testChildProc RavlOS ${TEST_LIBS} )
add_test(testChildProc testChildProc)
install(TARGETS testChildProc DESTINATION tests COMPONENT unittests EXCLUDE_FROM_ALL)

add_executable(testTextFile EXCLUDE_FROM_ALL tests/testTextFile.cc )
target_link_libraries(testTextFile RavlOS ${TEST_LIBS} )
add_test(testTextFile testTextFile)
install(TARGETS testTextFile DESTINATION tests COMPONENT unittests EXCLUDE_FROM_ALL)

add_executable(testTC EXCLUDE_FROM_ALL tests/testTC.cc )
target_link_libraries(testTC RavlOS ${TEST_LIBS} )
add_test(testTC testTC)
install(TARGETS testTC DESTINATION tests COMPONENT unittests EXCLUDE_FROM_ALL)


# === Files for share/RAVL/testData ===
set(AUXDIR share/RAVL/testData)
set(AUXFILES 
  data/testTC.tmpl 
  data/testTC.inc )
file(COPY ${AUXFILES} DESTINATION ${BUILD_ROOT_AUXFILE}/${AUXDIR})
install(FILES ${AUXFILES} DESTINATION ${AUXDIR})





