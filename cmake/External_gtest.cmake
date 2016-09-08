# grab gmock and gtest for testing

# We need thread support
find_package(Threads REQUIRED)

include(ExternalProject)

set(GTEST_PREFIX "googletest")
set(GTEST_DIR "${CMAKE_CURRENT_BINARY_DIR}/${GTEST_PREFIX}")
set(GTEST_INCLUDES "${GTEST_DIR}/src/googletest-${gtest_version}/include")

if (MSVC)
  # appveyor doesn't understand git
  set( GTEST_SOURCE_DOWNLOAD
       URL https://github.com/google/googletest/archive/release-${gtest_version}.tar.gz
       URL_MD5 4ff6353b2560df0afecfbda3b2763847 )
else()
  # location that works on linux
  set( GTEST_SOURCE_DOWNLOAD
       GIT_REPOSITORY https://github.com/google/googletest.git
       GIT_TAG "release-${gtest_version}" )
endif()

ExternalProject_Add(
    googletest-${gtest_version}
    DOWNLOAD_DIR "${GTEST_DIR}/src" # The only dir option which is required
    PREFIX "${GTEST_PREFIX}"
    TIMEOUT 10
    LOG_DOWNLOAD ON
    LOG_CONFIGURE ON
    LOG_BUILD ON
    CMAKE_ARGS -DBUILD_SHARED_LIBS:BOOL=ON -Dgtest_force_shared_crt=ON
    # Disable all other steps
    INSTALL_COMMAND ""
    ${GTEST_SOURCE_DOWNLOAD}
)

# the gtest include directory exists only after it is build, but it is used/needed
# for the set_target_properties call below, so make it to avoid an error
file(MAKE_DIRECTORY ${GTEST_INCLUDES})

# variables defining the import location properties for the generated gtest and
# gtestmain libraries
if (MSVC)
    set(GTEST_IMPORTED_LOCATION
        IMPORTED_LOCATION_DEBUG     "${GTEST_DIR}/Debug/${CMAKE_SHARED_LIBRARY_PREFIX}gtest${CMAKE_SHARED_LIBRARY_SUFFIX}"
        IMPORTED_LOCATION_RELEASE   "${GTEST_DIR}/Release/${CMAKE_SHARED_LIBRARY_PREFIX}gtest${CMAKE_SHARED_LIBRARY_SUFFIX}"
        )
    set(GTESTMAIN_IMPORTED_LOCATION
        IMPORTED_LOCATION_DEBUG     "${GTEST_DIR}/Debug/${CMAKE_SHARED_LIBRARY_PREFIX}gtest_main${CMAKE_SHARED_LIBRARY_SUFFIX}"
        IMPORTED_LOCATION_RELEASE   "${GTEST_DIR}/Release/${CMAKE_SHARED_LIBRARY_PREFIX}gtest_main${CMAKE_SHARED_LIBRARY_SUFFIX}"
        )
else()
    set(GTEST_IMPORTED_LOCATION
        IMPORTED_LOCATION           "${GTEST_DIR}/src/googletest-${gtest_version}-build/${CMAKE_SHARED_LIBRARY_PREFIX}gtest${CMAKE_SHARED_LIBRARY_SUFFIX}")
    set(GTESTMAIN_IMPORTED_LOCATION
        IMPORTED_LOCATION           "${GTEST_DIR}/src/googletest-${gtest_version}-build/${CMAKE_SHARED_LIBRARY_PREFIX}gtest_main${CMAKE_SHARED_LIBRARY_SUFFIX}")
endif()

# Create a libgtest target to be used as a dependency by test programs
add_library(gtest IMPORTED SHARED GLOBAL)
add_dependencies(gtest googletest-${gtest_version})
add_library(gtest_main IMPORTED SHARED GLOBAL)
add_dependencies(gtest_main googletest-${gtest_version})

# Set gtest properties
ExternalProject_Get_Property(googletest-${gtest_version} source_dir binary_dir)
set_target_properties(gtest PROPERTIES
    INTERFACE_INCLUDE_DIRECTORIES     "${GTEST_INCLUDES}"
    IMPORTED_LINK_INTERFACE_LIBRARIES "${CMAKE_THREAD_LIBS_INIT}"
    ${GTEST_IMPORTED_LOCATION}
)
set_target_properties(gtest_main PROPERTIES
   INTERFACE_INCLUDE_DIRECTORIES     "${GTEST_INCLUDES}"
    IMPORTED_LINK_INTERFACE_LIBRARIES "${CMAKE_THREAD_LIBS_INIT}"
    ${GTEST_IMPORTED_LOCATION}
)

# I couldn't make it work with INTERFACE_INCLUDE_DIRECTORIES
include_directories("${source_dir}/include")
