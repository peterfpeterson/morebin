# grab gmock and gtest for testing
# We need thread support
find_package(Threads REQUIRED)

include(ExternalProject)

set(GTEST_PREFIX "googletest")
set(GTEST_DIR "${CMAKE_CURRENT_BINARY_DIR}/${GTEST_PREFIX}")
set(GTEST_INCLUDES "${GTEST_DIR}/src/googletest-${gtest_version}/include")

ExternalProject_Add(
    googletest-${gtest_version}
    DOWNLOAD_DIR "${GTEST_DIR}/src" # The only dir option which is required
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG "release-${gtest_version}"
    PREFIX "${GTEST_PREFIX}"
    TIMEOUT 10
    LOG_DOWNLOAD ON

    # Disable all other steps
    CONFIGURE_COMMAND ""
    BUILD_COMMAND ""
    INSTALL_COMMAND ""
)

# Build gtest from existing sources
ExternalProject_Add(
    gtest
    DOWNLOAD_COMMAND "" # No download required
    SOURCE_DIR "${GTEST_DIR}/src/googletest-${gtest_version}" # Use specific source dir
    PREFIX "${GTEST_PREFIX}" # But use prefix for compute other dirs
    INSTALL_COMMAND ""
    LOG_CONFIGURE ON
    LOG_BUILD ON
    CMAKE_ARGS -DBUILD_SHARED_LIBS:BOOL=ON
)

# gtest should be build after being downloaded
add_dependencies(gtest googletest-${gtest_version})

# Build gmock from existing sources
ExternalProject_Add(
    gmock
    DOWNLOAD_COMMAND "" # No download required
    SOURCE_DIR "${GTEST_DIR}/src/googletest-${gtest_version}" # Use specific source dir
    #SOURCE_DIR "${GTEST_DIR}/src/googlemock" # Use specific source dir
    PREFIX "${GTEST_PREFIX}" # But use prefix for compute other dirs
    INSTALL_COMMAND ""
    CMAKE_ARGS -DBUILD_SHARED_LIBS:BOOL=ON
    LOG_CONFIGURE ON
    LOG_BUILD ON
)

add_dependencies(gmock googletest-${gtest_version})

# variables defining the import location properties for the generated gtest and
# gtestmain libraries
if (MSVC)
    set(GTEST_IMPORTED_LOCATION
        IMPORTED_LOCATION_DEBUG     "${GTEST_DIR}/Debug/${CMAKE_STATIC_LIBRARY_PREFIX}gtest${CMAKE_STATIC_LIBRARY_SUFFIX}"
        IMPORTED_LOCATION_RELEASE   "${GTEST_DIR}/Release/${CMAKE_STATIC_LIBRARY_PREFIX}gtest${CMAKE_STATIC_LIBRARY_SUFFIX}"
        )
    set(GTESTMAIN_IMPORTED_LOCATION
        IMPORTED_LOCATION_DEBUG     "${GTEST_DIR}/Debug/${CMAKE_STATIC_LIBRARY_PREFIX}gtest_main${CMAKE_STATIC_LIBRARY_SUFFIX}"
        IMPORTED_LOCATION_RELEASE   "${GTEST_DIR}/Release/${CMAKE_STATIC_LIBRARY_PREFIX}gtest_main${CMAKE_STATIC_LIBRARY_SUFFIX}"
        )
else()
    set(GTEST_IMPORTED_LOCATION
        IMPORTED_LOCATION           "${GTEST_DIR}/${CMAKE_STATIC_LIBRARY_PREFIX}gtest${CMAKE_STATIC_LIBRARY_SUFFIX}")
    set(GTESTMAIN_IMPORTED_LOCATION
        IMPORTED_LOCATION           "${GTEST_DIR}/${CMAKE_STATIC_LIBRARY_PREFIX}gtest_main${CMAKE_STATIC_LIBRARY_SUFFIX}")
endif()

# the gtest include directory exists only after it is build, but it is used/needed
# for the set_target_properties call below, so make it to avoid an error
file(MAKE_DIRECTORY ${GTEST_INCLUDES})

# Create a libgtest target to be used as a dependency by test programs
if (CMAKE_SHARED_LIBRARY_PREFIX)
  add_library("${CMAKE_SHARED_LIBRARY_PREFIX}gtest" IMPORTED SHARED GLOBAL)
  add_dependencies("${CMAKE_SHARED_LIBRARY_PREFIX}gtest" gtest)
else()
  set_target_properties("${CMAKE_SHARED_LIBRARY_PREFIX}gtest"
    PROPERTIES IMPORTED SHARED GLOBAL)
endif()
add_library(${CMAKE_SHARED_LIBRARY_PREFIX}gtest_main IMPORTED SHARED GLOBAL)
add_dependencies(${CMAKE_SHARED_LIBRARY_PREFIX}gtest_main gtest)

# Set gtest properties
ExternalProject_Get_Property(gtest source_dir binary_dir)
set_target_properties(${CMAKE_SHARED_LIBRARY_PREFIX}gtest PROPERTIES
    "IMPORTED_LOCATION" "${binary_dir}/${CMAKE_SHARED_LIBRARY_PREFIX}gtest${CMAKE_SHARED_LIBRARY_SUFFIX}"
    "IMPORTED_LINK_INTERFACE_LIBRARIES" "${CMAKE_THREAD_LIBS_INIT}"
    "INTERFACE_INCLUDE_DIRECTORIES" "${source_dir}/include"
)
set_target_properties(${CMAKE_SHARED_LIBRARY_PREFIX}gtest_main PROPERTIES
    "IMPORTED_LOCATION" "${binary_dir}/${CMAKE_SHARED_LIBRARY_PREFIX}gtest_main${CMAKE_SHARED_LIBRARY_SUFFIX}"
    "IMPORTED_LINK_INTERFACE_LIBRARIES" "${CMAKE_THREAD_LIBS_INIT}"
    "INTERFACE_INCLUDE_DIRECTORIES" "${source_dir}/include"
)
