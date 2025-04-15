# Version minimale de CMake requise
cmake_minimum_required(VERSION 3.12)
project(scrabble)

# Sources
set(test_sources
  src/board.hpp
  src/board.cpp
  src/bonus.hpp
  src/bonus.cpp
  src/spot.hpp
  src/spot.cpp
)

# Applications
add_executable(test_board ${test_sources} src/test_board.cpp)
