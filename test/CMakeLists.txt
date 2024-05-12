include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/e2239ee6043f73722e7aa812a459f54a28552929.zip
)

FetchContent_MakeAvailable(googletest)

add_executable(${PROJECT_NAME}.test
  AllTests.cpp
  tests.cpp)

target_include_directories(${PROJECT_NAME}.test
  PUBLIC ${${PROJECT_NAME}_SOURCE_DIR}/include)

target_link_libraries(${PROJECT_NAME}.test
  ${PROJECT_NAME} gtest pthread)
target_compile_definitions(${PROJECT_NAME}.test 
  PRIVATE TEST_DIR="${CMAKE_CURRENT_LIST_DIR}/test")

install(TARGETS ${PROJECT_NAME}.test
        DESTINATION "${tool_dest}")
