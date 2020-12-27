cmake_minimum_required(VERSION 3.5)
project(redis)

# Default to C99
if(NOT CMAKE_C_STANDARD)
  set(CMAKE_C_STANDARD 99)
endif()

set(CMAKE_CXX_STANDARD 17)
set(Boost_USE_STATIC_LIBS ON)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic -Werror)
endif()

find_package(GTest REQUIRED)
find_package(Threads REQUIRED)
find_package(Boost 1.68 REQUIRED COMPONENTS thread)

if(${TEST_MODE})
  add_executable(
    ${PROJECT_NAME} 
    ${CMAKE_SOURCE_DIR}/tests/main.cpp
  )
else()
  add_executable(
    ${PROJECT_NAME} 
    ${CMAKE_SOURCE_DIR}/src/main.cpp
  )
endif()

target_include_directories(
  ${PROJECT_NAME}
  PUBLIC
  ${GTEST_INCLUDE_DIRS}
  ${Threads_INCLUDE_DIR}
)

add_subdirectory(src/models)

target_link_libraries(${PROJECT_NAME} ${GTEST_BOTH_LIBRARIES} ${Boost_LIBRARIES} Threads::Threads models)
