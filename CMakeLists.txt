cmake_minimum_required( VERSION 3.16 )

project( openemsh
	LANGUAGES CXX
	VERSION 0.0.0
	DESCRIPTION "openEMS mesher"
	)

if( NOT CMAKE_BUILD_TYPE )
	set( CMAKE_BUILD_TYPE "Release" )
endif()
message( STATUS "Build type: ${CMAKE_BUILD_TYPE}" )

list( APPEND CMAKE_MODULE_PATH
	"${CMAKE_SOURCE_DIR}/cmake"
	)

#[[
set( CMAKE_INSTALL_DEFAULT_DIRECTORY_PERMISSIONS
	OWNER_READ
	OWNER_WRITE
	OWNER_EXECUTE
	GROUP_READ
	GROUP_EXECUTE
	WORLD_READ
	WORLD_EXECUTE
	)
#]]

include( CPM )

CPMAddPackage(
	NAME cmake-utils
	GITHUB_REPOSITORY conformism/cmake-utils
	GIT_TAG main
	DOWNLOAD_ONLY
	)

list( PREPEND CMAKE_MODULE_PATH
	"${FETCHCONTENT_BASE_DIR}/cmake-utils-src"
	)

set( CMAKE_UTILS
	CLANG_BUILD_ANALYZER
	CLANG_FORMAT
	CLANG_TIDY
	COVERAGE
	CPPCHECK
	IWYU
	LIZARD
	SANITIZERS
	)

set( COVERAGE_GLOBAL_ONLY ON )
include( CMakeUtils )

add_subdirectory( "${CMAKE_SOURCE_DIR}/src" )
add_subdirectory( "${CMAKE_SOURCE_DIR}/test" )

coverage_global()
