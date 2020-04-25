
# Library: RavlCrypto

# Requires: libmcrypt
# Supports: !VCPP

if((NOT ${CMAKE_CXX_COMPILER_ID} MATCHES "MSVC") OR FALSE)

if(libmcrypt_FOUND)



set(DEPENDS_ON )
foreach(D IN LISTS DEPENDS_ON)
  if(NOT ${D}_FOUND)
    find_package(${D} REQUIRED)
  endif()
endforeach()

include(CMakePackageConfigHelpers)

include_directories(include)
include_directories(src)

set(HAVE_RavlCrypto 1)

include_directories(../../Core/RavlCore/include)




add_library(RavlCrypto STATIC include/Ravl/Crypto/MCrypt.hh src/MCrypt.cc)
target_link_libraries(RavlCrypto PUBLIC ${libmcrypt_LIBRARIES} RavlCore )


target_include_directories(
    RavlCrypto PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
    )



install(TARGETS RavlCrypto EXPORT RavlCryptoConfig
   ARCHIVE  DESTINATION lib
   LIBRARY  DESTINATION lib
   RUNTIME  DESTINATION bin)  # This is for Windows

install(EXPORT RavlCryptoConfig DESTINATION lib/cmake/RavlCrypto/Target)
install(TARGETS RavlCrypto DESTINATION lib)

configure_package_config_file(cmake/RavlCryptoConfig.cmake.in
  ${CMAKE_CURRENT_BINARY_DIR}/RavlCryptoConfig.cmake
  INSTALL_DESTINATION lib/cmake/RavlCrypto
  PATH_VARS)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/RavlCryptoConfig.cmake
        DESTINATION lib/cmake/RavlCrypto )

install(DIRECTORY include DESTINATION .)








ENDIF()

ENDIF()
