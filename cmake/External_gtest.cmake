# grab gmock and gtest for testing
# We need thread support
find_package(Threads REQUIRED)

include(ExternalProject)

set(GTEST_PREFIX "googletest")
set(GTEST_DIR "${CMAKE_CURRENT_BINARY_DIR}/${GTEST_PREFIX}")
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

# Create a libgtest target to be used as a dependency by test programs
add_library(libgtest IMPORTED SHARED GLOBAL)
add_dependencies(libgtest gtest)
add_library(libgtest_main IMPORTED SHARED GLOBAL)
add_dependencies(libgtest_main gtest)

# Set gtest properties
ExternalProject_Get_Property(gtest source_dir binary_dir)
set_target_properties(libgtest PROPERTIES
    "IMPORTED_LOCATION" "${binary_dir}/${CMAKE_SHARED_LIBRARY_PREFIX}gtest${CMAKE_SHARED_LIBRARY_SUFFIX}"
    "IMPORTED_LINK_INTERFACE_LIBRARIES" "${CMAKE_THREAD_LIBS_INIT}"
#    "INTERFACE_INCLUDE_DIRECTORIES" "${source_dir}/include"
)
set_target_properties(libgtest_main PROPERTIES
    "IMPORTED_LOCATION" "${binary_dir}/${CMAKE_SHARED_LIBRARY_PREFIX}gtest_main${CMAKE_SHARED_LIBRARY_SUFFIX}"
    "IMPORTED_LINK_INTERFACE_LIBRARIES" "${CMAKE_THREAD_LIBS_INIT}"
#    "INTERFACE_INCLUDE_DIRECTORIES" "${source_dir}/include"
)

# I couldn't make it work with INTERFACE_INCLUDE_DIRECTORIES
include_directories("${source_dir}/include")