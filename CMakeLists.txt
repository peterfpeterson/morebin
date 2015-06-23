# minimum version necessary for cmake to work
cmake_minimum_required ( VERSION 2.6 )
project ( morebin )

set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)

######################################################################
# look for dependencies
include ( CheckIncludeFiles )
# stdint
check_include_files ( stdint.h stdint )
if ( NOT stdint )
  MESSAGE ( FATAL_ERROR "stdint.h required to build" )
endif ( NOT stdint )
# tclap - needs to go
#find_path ( TCLAP_INCLUDE_DIR tclap/CmdLine.h )
#include_directories ( ${TCLAP_INCLUDE_DIR} )
#message ( STATUS "TCLAP: ${TCLAP_INCLUDE_DIR}" )
# boost
find_package ( Boost REQUIRED program_options date_time )

find_program ( DATE_CMD date )
if (DATE_CMD)
   execute_process ( COMMAND ${DATE_CMD} --iso-8601=minutes
   		   OUTPUT_VARIABLE BUILD_DATE_TIME
		   OUTPUT_STRIP_TRAILING_WHITESPACE )
else (DATE_CMD)
   set ( BUILD_DATE_TIME "UNKNOWN" )
endif (DATE_CMD)

######################################################################
set ( CPACK_PACKAGE_VERSION_MAJOR 2 )
set ( CPACK_PACKAGE_VERSION_MINOR 4 )
set ( CPACK_PACKAGE_VERSION_PATCH 0 )
set ( CPACK_PACKAGE_VERSION "${CPACK_PACKAGE_VERSION_MAJOR}.${CPACK_PACKAGE_VERSION_MINOR}.${CPACK_PACKAGE_VERSION_PATCH}" )

configure_file( ${CMAKE_CURRENT_SOURCE_DIR}/version.hpp.in
		${CMAKE_CURRENT_SOURCE_DIR}/version.hpp )

######################################################################
include(ExternalProject)

######################################################################
set(gtest_version "1.7.0")
set(gtest_md5 "2d6ec8ccdf5c46b05ba54a9fd1d130d7")

option(USE_SYSTEM_GTEST "Should we use the system GTest - v${gtest_version}?" OFF)

if(USE_SYSTEM_GTEST)
  find_package(GTest REQUIRED)
else()
  include(External_gtest)
  set( GTEST_BOTH_LIBRARIES "libgtest;libgtest_main" )
#  ExternalProject_Get_Property(gtest GTEST_BOTH_LIBRARIES ) #libgtest.so libgtest_main.so )source_dir binary_dir)
endif()
message (STATUS "GTEST_LIBS=${GTEST_BOTH_LIBRARIES}")

######################################################################
# files to use for building

# since the result is a static executable, need another variable
# for all the sources that don't contain main
set ( SRC_FILES_NO_MAIN
      allowedtypes.cpp
      allowedtypes.hpp
      bin_file.cpp
      bin_file.hpp
      byte_swap.h
      prenexusrenderer.cpp
      prenexusrenderer.hpp
      prenexustypes.hpp
      renderer.cpp
      renderer.hpp
      statistics.cpp
      statistics.hpp
      string_util.cpp
      string_util.hpp
      version.hpp )

set ( SRC_FILES
      more_bin.cpp
      ${SRC_FILES_NO_MAIN})

set ( TEST_FILES
      testmain.cpp
      testallowedtypes.cpp
      teststring_util.cpp)

######################################################################
# configure coveralls
option(COVERALLS "Generate coveralls data" OFF)
option(COVERALLS_TRAVISCI "Generate coveralls data for travis-ci" OFF)
option(COVERALLS_UPLOAD "Automatically upload coveralls data" OFF)

if (COVERALLS)
    message (STATUS "Coveralls build enabled")
    include(Coveralls)
    coveralls_turn_on_coverage()

    foreach(loop_var ${SRC_FILES} ${INC_FILES} )
      set(COVERAGE_SRCS "${COVERAGE_SRCS}\n${CMAKE_CURRENT_SOURCE_DIR}/${loop_var}")
    endforeach(loop_var)
    string(SUBSTRING ${COVERAGE_SRCS} 1 -1 SRCS_FILE)
    set( SRCS_FILENAME "${CMAKE_CURRENT_BINARY_DIR}/sources.txt")
    file(WRITE ${SRCS_FILENAME} ${SRCS_FILE})

    # Create the coveralls target.
    coveralls_setup(
        ${SRCS_FILENAME}
        ${COVERALLS_UPLOAD}    # If we should upload.
        ${COVERALLS_TRAVISCI}  # building on travis-ci
        )
endif()

######################################################################
if (WIN32)
  add_definitions(-DBOOST_ALL_NO_LIB -DBOOST_ALL_DYN_LINK )
  set ( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4" )
endif()

include_directories(${CMAKE_CURRENT_BINARY_DIR} ${Boost_INCLUDE_DIR})
add_executable( morebin MACOSX_BUNDLE ${SRC_FILES} )
target_link_libraries ( morebin ${Boost_LIBRARIES} )

add_executable( runUnitTests ${TEST_FILES} ${SRC_FILES_NO_MAIN})
target_link_libraries ( runUnitTests
                        ${Boost_LIBRARIES}
#                        libgtest
#                        libgmock
                        ${GTEST_BOTH_LIBRARIES}
                        )
enable_testing()
add_test(morebin_version morebin --version)
add_test(AllowedTypes runUnitTests --gtest_filter=AllowedTypes.*)
add_test(string_util runUnitTests --gtest_filter=string_util.*)

INSTALL(TARGETS morebin
    BUNDLE DESTINATION . COMPONENT Runtime
    RUNTIME DESTINATION bin COMPONENT Runtime
    )

######################################################################
# generic project information
set ( CPACK_PACKAGE_DESCRIPTION_SUMMARY "More for binary files" )
set ( CPACK_PACKAGE_VENDOR "Peter F. Peterson" )
set ( CPACK_PACKAGE_URL "http://github.com/peterfpeterson/morebin" )
set ( CPACK_PACKAGE_CONTACT peterfpeterson@gmail.com )
set ( CPACK_PACKAGE_URL "http://www.github.com/peterfpeterson/morebin" )
#set ( CPACK_PACKAGE_ICON )
set ( CPACK_PACKAGE_NAME morebin )
# on Apple we don't want the packaging prefix to be /usr/local
if (NOT APPLE)
  set ( CPACK_PACKAGING_INSTALL_PREFIX ${CMAKE_INSTALL_PREFIX} )
endif ()
set ( CPACK_RPM_PACKAGE_LICENSE "The MIT License" )
set ( CPACK_RPM_PACKAGE_RELEASE 1 )
set ( CPACK_RPM_PACKAGE_GROUP "Development/Tools" )

if (APPLE)
  set ( CPACK_GENERATOR DragNDrop CACHE STRING "Set package type")
else ()
  set ( CPACK_GENERATOR "RPM" CACHE STRING "Set package type")
endif ()


######################################################################
# build an rpm
find_program ( RPMBUILD_CMD rpmbuild )
if ( RPMBUILD_CMD )
  set ( CPACK_RPM_PACKAGE_URL "${CPACK_PACKAGE_URL}" )
  set ( CPACK_RPM_PACKAGE_REQUIRES "boost-devel cmake >= 2.6 gcc-c++" )

  # TODO CPACK_PACKAGE_DESCRIPTION_FILE
  configure_file( ${CMAKE_CURRENT_SOURCE_DIR}/morebin.spec.in
                 ${CMAKE_CURRENT_BINARY_DIR}/morebin.spec )
  add_custom_command( OUTPUT morebin-${CPACK_PACKAGE_VERSION}.tar.gz
                     COMMAND git archive --format=tar --prefix=morebin-${CPACK_PACKAGE_VERSION}/ HEAD | gzip -c > ${CMAKE_CURRENT_BINARY_DIR}/morebin-${CPACK_PACKAGE_VERSION}.tar.gz
                     WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
                      )
  add_custom_target( package_rpm
                    DEPENDS morebin-${CPACK_PACKAGE_VERSION}.tar.gz
                            ${CMAKE_CURRENT_BINARY_DIR}/morebin.spec
                    COMMAND rpmbuild -ba morebin.spec --define "_sourcedir ${CMAKE_CURRENT_BINARY_DIR}" --define "_topdir ${CMAKE_CURRENT_BINARY_DIR}/rpmbuild")
endif ( RPMBUILD_CMD )

######################################################################
# cpack information - only for deb files and not proven to work
if ( ${CPACK_GENERATOR} MATCHES "RPM" )
  message ( STATUS "Use the target 'package_rpm' instead of 'package' to generate rpms")
elseif ( ${CPACK_GENERATOR} MATCHES "DEB" )
  find_program (DPKG_CMD dpkg)
  if ( DPKG_CMD )
    execute_process( COMMAND ${DPKG_CMD} --print-architecture
      OUTPUT_VARIABLE CPACK_DEBIAN_PACKAGE_ARCHITECTURE
      OUTPUT_STRIP_TRAILING_WHITESPACE )
  else ()
    set ( CPACK_DEBIAN_PACKAGE_ARCHITECTURE "i686" )
  endif ( DPKG_CMD )
  set ( CPACK_DEBIAN_PACKAGE_RELEASE "0" CACHE STRING "DEB release name" ) # should depend on host os
  set( CPACK_PACKAGE_FILE_NAME
      "${CPACK_PACKAGE_NAME}_${CPACK_PACKAGE_VERSION}-${CPACK_DEBIAN_PACKAGE_RELEASE}_${CPACK_DEBIAN_PACKAGE_ARCHITECTURE}")
  message (STATUS "CPACK_PACKAGE_FILE_NAME = ${CPACK_PACKAGE_FILE_NAME}" )
elseif (${CPACK_GENERATOR} MATCHES "DragNDrop")
  set(APPS "\${CMAKE_INSTALL_PREFIX}/morebin.app")
  INSTALL(CODE "
    set(BU_CHMOD_BUNDLE_ITEMS ON)
    include(BundleUtilities)
    fixup_bundle(\"${APPS}\" \"\" \"${Boost_LIBRARY_DIR}\")
    " COMPONENT Runtime)
else ()
  message (STATUS "CPACK_GENERATOR = ${CPACK_GENERATOR} not configuring to package")
endif ()

include (CPack) # have cpack do its thing
