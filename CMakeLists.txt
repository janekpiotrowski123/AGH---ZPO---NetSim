cmake_minimum_required(VERSION 3.27)
project(AGH_ZPO_NetSim)

set(CMAKE_CXX_STANDARD 17)

# Include directories
include_directories(include)

# Source files
set(SOURCE_FILES
        src/package.cpp
        src/helpers.cpp
        src/nodes.cpp
        src/storage_types.cpp
)

# Main executable (bez Google Test)
add_executable(AGH_ZPO_NetSim
        main.cpp
        ${SOURCE_FILES}
)

# Google Test
add_subdirectory(../googletest-1.14.0 googletest-build)

# Include Google Test directories
include_directories(../googletest-1.14.0/googletest/include)

# Enable testing
enable_testing()

# Test executable (z Google Test)
set(TEST_FILES
        tests/tests.cpp
)
add_executable(AGH_ZPO_NetSim_Tests ${TEST_FILES} ${SOURCE_FILES})
target_link_libraries(AGH_ZPO_NetSim_Tests gtest gtest_main)

# Add test to CMake testing framework
add_test(NAME AGH_ZPO_NetSim_Tests COMMAND AGH_ZPO_NetSim_Tests)
