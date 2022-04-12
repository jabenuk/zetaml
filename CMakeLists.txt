cmake_minimum_required(VERSION 3.10)

project(zetamlc)

set(SRC "${CMAKE_CURRENT_LIST_DIR}/src")

add_executable(zmlctest
	"${SRC}/main.c"
)