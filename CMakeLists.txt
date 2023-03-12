cmake_minimum_required(VERSION 3.10)

set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)

project(
  FastUI
  VERSION 1.0
  LANGUAGES CXX
  )

add_subdirectory(src lib)
add_subdirectory(example)
