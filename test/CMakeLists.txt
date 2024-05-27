cmake_minimum_required(VERSION 3.10)
project(SupermarketSimulation)

set(CMAKE_CXX_STANDARD 17)

# Add include directory
include_directories(include)

# Add source files
add_executable(main src/main.cpp src/task.cpp)

# Add Google Test
enable_testing()
find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})

# Add test executable
add_executable(tests test/tests.cpp src/task.cpp)
target_link_libraries(tests ${GTEST_LIBRARIES} pthread)

# Enable tests
add_test(AllTests tests)