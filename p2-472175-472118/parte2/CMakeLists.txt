cmake_minimum_required(VERSION 3.22)
project( fluid LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
# Set compiler options
add_compile_options(-Wall -Wextra -Werror -pedantic -pedantic-errors)
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -march=native -O3")

# Run clang−tidy on the whole source tree
# Note this will slow down compilation.
# You may temporarily disable but do not forget to enable again.
set(CMAKE_CXX_CLANG_TIDY-18 clang-tidy-18 −header−filter=.*)
# All includes relative to source tree root.
include_directories (PUBLIC .)
add_subdirectory(C++Version)
add_executable(main main.cpp)
target_link_libraries(main things)

