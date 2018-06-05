
# win-decaffeinate
#
# Copyright iorate 2018.
# Distributed under the Boost Software License, Version 1.0.
# (See accompanying file LICENSE_1_0.txt or copy at
# http://www.boost.org/LICENSE_1_0.txt)

cmake_minimum_required(VERSION 3.0.2)

project(win-decaffeinate)

if(CMAKE_CXX_COMPILER_ID MATCHES MSVC)
  add_compile_options(/std:c++17 /WX /W4)
else(CMAKE_CXX_COMPILER_ID MATCHES GNU|Clang)
  add_compile_options(-std=c++17 -Werror -Wall -Wextra -Wpedantic)
endif()

include_directories(${PROJECT_SOURCE_DIR}/nonsugar)

add_executable(decaffeinate main.cpp)

target_link_libraries(decaffeinate PowrProf)
