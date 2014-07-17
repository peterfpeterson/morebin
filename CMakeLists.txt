# minimum version necessary for cmake to work
cmake_minimum_required ( VERSION 2.6 )
project ( morebin )

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
set ( CPACK_PACKAGE_VERSION_PATCH "0SNAPSHOT" ) # dev versions are SNAPSHOT like java
set ( CPACK_PACKAGE_VERSION "${CPACK_PACKAGE_VERSION_MAJOR}.${CPACK_PACKAGE_VERSION_MINOR}.${CPACK_PACKAGE_VERSION_PATCH}" )

configure_file( ${CMAKE_CURRENT_SOURCE_DIR}/version.hpp.in
		${CMAKE_CURRENT_BINARY_DIR}/version.hpp )

######################################################################
# files to use for building
set ( SRC_FILES
      allowedtypes.cpp
      allowedtypes.hpp
      bin_file.cpp
      bin_file.hpp
      byte_swap.h
      more_bin.cpp
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

include_directories(${CMAKE_CURRENT_BINARY_DIR})
add_executable( morebin ${SRC_FILES} )
target_link_libraries ( morebin ${Boost_LIBRARIES} )
install ( TARGETS morebin RUNTIME DESTINATION bin )

######################################################################
# deteremine the unix version
find_program (LSB_CMD lsb_release)
if ( LSB_CMD )
  # get the distribution
  execute_process ( COMMAND ${LSB_CMD} -i
    OUTPUT_VARIABLE UNIX_DIST
    OUTPUT_STRIP_TRAILING_WHITESPACE )
  string ( REGEX REPLACE "Distributor ID:" "" UNIX_DIST ${UNIX_DIST} )
  string ( STRIP ${UNIX_DIST} UNIX_DIST )
  string ( REGEX REPLACE "RedHatEnterpriseClient" "RedHatEnterprise" UNIX_DIST ${UNIX_DIST} )
  string ( REGEX REPLACE "RedHatEnterpriseWorkstation" "RedHatEnterprise" UNIX_DIST ${UNIX_DIST} )
  # Make Scientific Linux and CentOS look like RHEL6
  string ( REGEX REPLACE "Scientific" "RedHatEnterprise" UNIX_DIST ${UNIX_DIST} )
  string ( REGEX REPLACE "CentOS" "RedHatEnterprise" UNIX_DIST ${UNIX_DIST} )
  # get the codename
  execute_process ( COMMAND ${LSB_CMD} -c
    OUTPUT_VARIABLE UNIX_CODENAME
    OUTPUT_STRIP_TRAILING_WHITESPACE )
  string ( REGEX REPLACE "Codename:" "" UNIX_CODENAME ${UNIX_CODENAME} )
  string ( STRIP ${UNIX_CODENAME} UNIX_CODENAME )
  # Make Scientific Linux and CentOS look like RHEL6
  string ( REGEX REPLACE "Carbon" "Santiago" UNIX_DIST ${UNIX_DIST} )
  string ( REGEX REPLACE "Final" "Santiago" UNIX_DIST ${UNIX_DIST} )
  # get the release
  execute_process ( COMMAND ${LSB_CMD} -r
    OUTPUT_VARIABLE UNIX_RELEASE
    OUTPUT_STRIP_TRAILING_WHITESPACE )
  string ( REGEX REPLACE "Release:" "" UNIX_RELEASE ${UNIX_RELEASE} )
  string ( STRIP ${UNIX_RELEASE} UNIX_RELEASE )
else ( LSB_CMD )
  set ( UNIX_DIST "" )
  set ( UNIX_CODENAME "" )
  set ( UNIX_RELEASE "" )
endif ( LSB_CMD )

######################################################################
# generic project information
set ( CPACK_PACKAGE_DESCRIPTION_SUMMARY "More for binary files" )
set ( CPACK_PACKAGE_VENDOR "Peter F. Peterson" )
set ( CPACK_PACKAGE_URL "http://github.com/peterfpeterson/morebin" )
set ( CPACK_PACKAGE_CONTACT peterfpeterson@gmail.com )
#set ( CPACK_PACKAGE_ICON )
set ( CPACK_PACKAGE_NAME morebin )
set ( CPACK_PACKAGING_INSTALL_PREFIX ${CMAKE_INSTALL_PREFIX} )
set ( CPACK_RPM_PACKAGE_LICENSE "The MIT License" )

set ( CPACK_GENERATOR "RPM" CACHE STRING "Set package type")

######################################################################
# build an rpm
find_program ( RPMBUILD_CMD rpmbuild )
if ( RPMBUILD_CMD )
  set ( CPACK_RPM_PACKAGE_URL "http://www.github.com/peterfpeterson/morebin" )
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
  message ( WARNING " Use the target package_rpm instead")
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
  message (status " CPACK_PACKAGE_FILE_NAME = ${CPACK_PACKAGE_FILE_NAME}" )
  include (CPack) # have cpack do its thing
endif ( ${CPACK_GENERATOR} MATCHES "RPM" )
