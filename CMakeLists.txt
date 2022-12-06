cmake_minimum_required( VERSION 3.16 )

project( openemsh
	LANGUAGES CXX
	VERSION 0.0.0
	DESCRIPTION "openEMS mesher"
	HOMEPAGE_URL "https://github.com/Open-RFlab/openemsh"
	)

if( NOT CMAKE_BUILD_TYPE )
	set( CMAKE_BUILD_TYPE "Release" )
endif()
message( STATUS "Build type: ${CMAKE_BUILD_TYPE}" )

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

option( CPM_DISABLE "Don't use CPM to retrieve dependencies" OFF )

if( NOT CPM_DISABLE )
	message( STATUS "Downloading dependencies with CPM" )

	list( APPEND CMAKE_MODULE_PATH
		"${CMAKE_SOURCE_DIR}/cmake"
		)

	include( CPM )

	CPMAddPackage(
		NAME cmake-utils
		GITHUB_REPOSITORY conformism/cmake-utils
		GIT_TAG main
		DOWNLOAD_ONLY
		)

	set( CMakeUtils_DIR "${FETCHCONTENT_BASE_DIR}/cmake-utils-src" )

	CPMAddPackage(
		NAME pugixml
		GITHUB_REPOSITORY zeux/pugixml
		VERSION 1.12.1
		OPTIONS
			"BUILD_SHARED_LIBS ON"
		)

	set( pugixml_DIR "${FETCHCONTENT_BASE_DIR}/pugixml-build" )
endif()

find_package( CMakeUtils QUIET )
if( CMakeUtils_FOUND )
	message( STATUS "Found CMakeUtils: ${CMakeUtils_DIR}" )
else()
	message( STATUS "Not found CMakeUtils: Only minimal build enabled" )
endif()

find_package( pugixml REQUIRED )
if( pugixml_FOUND )
	message( STATUS "Found pugixml: ${pugixml_DIR} ${PUGIXML_VERSION}" )
endif()

if( CMakeUtils_FOUND )
	set( CMAKE_UTILS * )
	set( COVERAGE_GLOBAL_ONLY ON )
	include( CMakeUtils )
endif()

add_subdirectory( "${CMAKE_SOURCE_DIR}/src" )
add_subdirectory( "${CMAKE_SOURCE_DIR}/test" )
add_subdirectory( "${CMAKE_SOURCE_DIR}/icon" )

if( CMakeUtils_FOUND )
	coverage_global()
endif()
