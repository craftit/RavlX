
# Library: ccmath

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

set(HAVE_ccmath 1)

include_directories(../../Core/RavlCore/include)




add_library(ccmath STATIC include/ccmath/ccmath.h include/ccmath/orpol.h src/atou1.c src/atovm.c src/chouse.c src/chousv.c src/cmattr.c src/cmcpy.c src/cminv.c src/cmmul.c src/cmmult.c src/cmprt.c src/csolv.c src/cvmul.c src/eigen.c src/eigval.c src/evmax.c src/hconj.c src/heigval.c src/heigvec.c src/hevmax.c src/hmgen.c src/house.c src/housev.c src/ldumat.c src/ldvmat.c src/matprt.c src/mattr.c src/mcopy.c src/minv.c src/mmul.c src/ortho.c src/otrma.c src/otrsm.c src/psinv.c src/qrbdi.c src/qrbdu1.c src/qrbdv.c src/qrecvc.c src/qreval.c src/qrevec.c src/rmmult.c src/ruinv.c src/smgen.c src/solvps.c src/solvru.c src/solvtd.c src/sv2u1v.c src/sv2uv.c src/sv2val.c src/svdu1v.c src/svduv.c src/svdval.c src/trncm.c src/trnm.c src/unitary.c src/utrncm.c src/utrnhm.c src/vmul.c src/unfl.c src/lrand.c src/lrana.c src/solv.c src/fft2.c src/fft2_d.c src/fftgc.c src/fftgr.c src/ftuns.c src/pfac.c src/pshuf.c src/pwspec.c src/smoo.c src/chcof.c src/csfit.c src/csplp.c src/evpsq.c src/fitval.c src/gnlsq.c src/plsq.c src/psqcf.c src/qrbdbv.c src/qrvar.c src/sv2lsq.c src/tnsfit.c src/chpade.c src/cspl.c src/dcspl.c src/evpsqv.c src/ftch.c src/lsqsv.c src/pplsq.c src/psqvar.c src/qrlsq.c src/seqlsq.c src/svdlsq.c src/gaml.c src/pctb.c src/pctbn.c src/pctg.c src/pctgn.c src/pctn.c src/qbeta.c src/qbnc.c src/qgama.c src/qgnc.c src/qnorm.c)
target_link_libraries(ccmath PUBLIC RavlCore )


target_include_directories(
    ccmath PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
    )



install(TARGETS ccmath EXPORT ccmathConfig
   ARCHIVE  DESTINATION lib
   LIBRARY  DESTINATION lib
   RUNTIME  DESTINATION bin)  # This is for Windows

install(EXPORT ccmathConfig DESTINATION lib/cmake/ccmath/Target)
install(TARGETS ccmath DESTINATION lib)

configure_package_config_file(cmake/ccmathConfig.cmake.in
  ${CMAKE_CURRENT_BINARY_DIR}/ccmathConfig.cmake
  INSTALL_DESTINATION lib/cmake/ccmath
  PATH_VARS)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/ccmathConfig.cmake
        DESTINATION lib/cmake/ccmath )

install(DIRECTORY include DESTINATION .)




# Tests
set(TEST_LIBS  RavlCore  )

add_executable(testValS EXCLUDE_FROM_ALL tests/testValS.cc )
target_link_libraries(testValS ccmath ${TEST_LIBS} )
add_test(testValS testValS)
install(TARGETS testValS DESTINATION tests COMPONENT unittests EXCLUDE_FROM_ALL)






