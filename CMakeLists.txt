cmake_minimum_required(VERSION 3.10)
project(zetaml)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)

option(ZML_BUILD_LIB "Build base zetaml library." ON)
if (ZML_BUILD_LIB)
	add_subdirectory("src")
endif()

option(ZML_BUILD_TESTS "Build zetaml test executable(s)." OFF)
if (ZML_BUILD_TESTS)
	add_subdirectory("tests")
endif()

option(BUILD_SHARED_LIBS "Build shared libraries." OFF)