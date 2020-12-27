cmake_minimum_required(VERSION 3.5)
project(models)

# Default to C99
if(NOT CMAKE_C_STANDARD)
  set(CMAKE_C_STANDARD 99)
endif()

# Default to C++17
if(NOT CMAKE_CXX_STANDARD)
  set(CMAKE_CXX_STANDARD 17)
endif()

set(Boost_USE_STATIC_LIBS ON)

find_package(Boost 1.68 REQUIRED COMPONENTS thread)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic -Werror)
endif()


# create library
add_library(${PROJECT_NAME} STATIC garbage_collector.cpp storage.cpp value_map.cpp)

# add include directories
target_include_directories(${PROJECT_NAME}
                           PUBLIC
                           ${CMAKE_CURRENT_SOURCE_DIR}
)

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/utils)

target_link_libraries(${PROJECT_NAME} ${Boost_LIBRARIES} utils)
