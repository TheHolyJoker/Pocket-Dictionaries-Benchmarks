cmake_minimum_required(VERSION 3.0.0)
project(Bench VERSION 0.1.0)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -mavx512bw")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -faligned-new")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}  -mbmi -mbmi2")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}  -msse2")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -mlzcnt")

set(PROJECT_SOURCES
    # pd320.hpp pd320.cpp
    pd512.hpp pd512.cpp
    utils.hpp utils.cpp
)
include(CTest)
enable_testing()

add_executable(Bench tests.hpp tests.cpp main.cpp ${PROJECT_SOURCES})
# Add the compile options ONLY for Filters.
target_compile_options(Bench PRIVATE -O3 -DNDEBUG)
add_executable(valid_test validate.hpp validate.cpp ${PROJECT_SOURCES})
# Add the compile options ONLY for valid_test.
target_compile_options(valid_test PRIVATE -O0 -Werror -Wextra)


# add_executable(Bench main.cpp)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
