cmake_minimum_required(VERSION 2.8 FATAL_ERROR)
set(CMAKE_LEGACY_CYGWIN32 0)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")

project(rgl)

set(VERSION_MAJOR 0)
set(VERSION_MINOR 1)
set(VERSION_PATCH 0)

set(VERSION_EXTRA "")

set(RGL_VERSION "${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_PATCH}")

if ( VERSION_EXTRA )
	set(RGL_VERSION "${RGL_VERSION}-${VERSION_EXTRA}")
endif ()

message("Conifguring rgl version ${RGL_VERSION}.")

configure_file (
	"${PROJECT_SOURCE_DIR}/gen/rgl_version.hpp.in"
	"${PROJECT_BINARY_DIR}/rgl_version.hpp"
)

set(INITIAL_FLAGS ${CMAKE_CXX_FLAGS_BASE})

add_subdirectory(src)
