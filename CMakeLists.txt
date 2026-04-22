cmake_minimum_required(VERSION 3.10)
project(problem_124_code CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Optimize for speed
add_compile_options(-O2 -pipe -DNDEBUG)

add_executable(code main.cpp)

# Ensure static libstdc++/libgcc not forced to avoid CI issues
# target_link_options(code PRIVATE -static-libstdc++ -static-libgcc)
