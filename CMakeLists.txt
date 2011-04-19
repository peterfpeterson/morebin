# minimum version necessary for cmake to work
cmake_minimum_required ( VERSION 2.6 )

######################################################################
# generic project information
project ( morebin )
set ( CPACK_PACKAGE_DESCRIPTION_SUMMARY "More for binary files" )
set ( CPACK_PACKAGE_VENDOR "Peter F. Peterson" )
set ( CPACK_PACKAGE_URL "???" )
set ( CPACK_PACKAGE_CONTACT peterfpeterson@gmail.com )
#set ( CPACK_PACKAGE_ICON )
#set ( CPACK_PACKAGE_NAME morebin )
set ( CPACK_PACKAGE_VERSION_MAJOR 2 )
set ( CPACK_PACKAGE_VERSION_MINOR 0 )
set ( CPACK_PACKAGE_VERSION_PATCH "SNAPSHOT" )
set ( CPACK_PACKAGE_VERSION "${CPACK_PACKAGE_VERSION_MAJOR}.${CPACK_PACKAGE_VERSION_MINOR}.${CPACK_PACKAGE_VERSION_PATCH}" )
set ( CPACK_PACKAGING_INSTALL_PREFIX /usr/local/ )

# cpack information
set ( CPACK_GENERATOR "DEB" )
set ( CPACK_SOURCE_GENERATOR TGZ )

include (CPack)

######################################################################
# look for dependencies
include ( CheckIncludeFiles )
check_include_files ( stdint.h stdint )
if ( stdint )
  add_definitions ( -DHAVE_STDINT_H )
endif ( stdint )
find_path ( TCLAP_INCLUDE_DIR tclap/CmdLine.h )
include_directories ( ${TCLAP_INCLUDE_DIR} )
message ( STATUS "TCLAP: ${TCLAP_INCLUDE_DIR}" )

######################################################################
# files to use for building
set ( SRC_FILES
      more_bin.cpp )

add_executable( morebin ${SRC_FILES} )
