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
    CMAKE_ARGS -DBUILD_SHARED_LIBS:BOOL=ON
    # Disable all other steps
    INSTALL_COMMAND ""
)

# the gtest include directory exists only after it is build, but it is used/needed
# for the set_target_properties call below, so make it to avoid an error
file(MAKE_DIRECTORY ${GTEST_INCLUDES})

# Create a libgtest target to be used as a dependency by test programs
add_library(${CMAKE_SHARED_LIBRARY_PREFIX}gtest IMPORTED SHARED GLOBAL)
add_dependencies(${CMAKE_SHARED_LIBRARY_PREFIX}gtest googletest-${gtest_version})
add_library(${CMAKE_SHARED_LIBRARY_PREFIX}gtest_main IMPORTED SHARED GLOBAL)
add_dependencies(${CMAKE_SHARED_LIBRARY_PREFIX}gtest_main ${CMAKE_SHARED_LIBRARY_PREFIX}gtest googletest-${gtest_version})

# Set gtest properties
ExternalProject_Get_Property(googletest-${gtest_version} source_dir binary_dir)
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
