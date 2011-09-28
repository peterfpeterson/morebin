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
set ( CPACK_PACKAGE_VERSION_MINOR 1 )
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
# generic project information
set ( CPACK_PACKAGE_DESCRIPTION_SUMMARY "More for binary files" )
set ( CPACK_PACKAGE_VENDOR "Peter F. Peterson" )
set ( CPACK_PACKAGE_URL "http://github.com/peterfpeterson/morebin" )
set ( CPACK_PACKAGE_CONTACT peterfpeterson@gmail.com )
#set ( CPACK_PACKAGE_ICON )
set ( CPACK_PACKAGE_NAME morebin )
set ( CPACK_PACKAGING_INSTALL_PREFIX /usr/local )
set ( CPACK_RPM_PACKAGE_LICENSE "The MIT License" )

set ( CPACK_GENERATOR "RPM" CACHE STRING "Set pakage type")
set ( CPACK_RPM_PACKAGE_RELEASE "1.el6" CACHE STRING "RPM release name" ) # should depend on host os
set ( CPACK_DEBIAN_PACKAGE_RELEASE "0" CACHE STRING "DEB release name" ) # should depend on host os
    
# cpack information
if ( ${CPACK_GENERATOR} MATCHES "RPM" )
  find_program ( RPMBUILD_CMD rpmbuild )
  set ( CPACK_RPM_PACKAGE_ARCHITECTURE "${CMAKE_SYSTEM_PROCESSOR}" )
  set ( CPACK_PACKAGE_FILE_NAME
      "${CPACK_PACKAGE_NAME}-${CPACK_PACKAGE_VERSION}-${CPACK_RPM_PACKAGE_RELEASE}.${CPACK_RPM_PACKAGE_ARCHITECTURE}" )
elseif ( ${CPACK_GENERATOR} MATCHES "DEB" )
  find_program (DPKG_CMD dpkg)
  if ( DPKG_CMD )
    execute_process( COMMAND ${DPKG_CMD} --print-architecture
      OUTPUT_VARIABLE CPACK_DEBIAN_PACKAGE_ARCHITECTURE
      OUTPUT_STRIP_TRAILING_WHITESPACE )
  else ()
    set ( CPACK_DEBIAN_PACKAGE_ARCHITECTURE "i686" )
  endif ( DPKG_CMD )
  set( CPACK_PACKAGE_FILE_NAME
      "${CPACK_PACKAGE_NAME}_${CPACK_PACKAGE_VERSION}-${CPACK_DEBIAN_PACKAGE_RELEASE}_${CPACK_DEBIAN_PACKAGE_ARCHITECTURE}")
endif ( ${CPACK_GENERATOR} MATCHES "RPM" )

#set ( CPACK_SOURCE_GENERATOR "TGZ" )
#set ( CPACK_RPM_PACKAGE_REQUIRES "boost")
#set ( CPACK_DEBIAN_PACKAGE_DEPENDS "boost")

message (status " CPACK_PACKAGE_FILE_NAME = ${CPACK_PACKAGE_FILE_NAME}" )
include (CPack) # have cpack do its thing

