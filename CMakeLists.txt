cmake_minimum_required( VERSION 3.16 )

project( openemsh
	LANGUAGES CXX
	VERSION 0.0.0
	DESCRIPTION "openEMS mesher"
	HOMEPAGE_URL "https://github.com/Open-RFlab/openemsh"
	)
message( STATUS "Version: ${PROJECT_VERSION}" )

set( OEMSH_BUGREPORT    "https://github.com/Open-RFlab/openemsh/issues" )
set( OEMSH_FUNDING      "https://liberapay.com/thomaslepoix" )
set( OEMSH_OEMS_MESHING "https://wiki.openems.de/index.php/FDTD_Mesh.html" )
#file( READ "${CMAKE_SOURCE_DIR}/CHANGELOG" OEMSH_CHANGELOG )

if( NOT CMAKE_BUILD_TYPE )
	set( CMAKE_BUILD_TYPE "Release" )
endif()
message( STATUS "Build type: ${CMAKE_BUILD_TYPE}" )

option( CPM_DISABLE "Don't use CPM to retrieve dependencies" "$ENV{CPM_DISABLE}" )
option( OEMSH_PORTABILITY_TWEAKS "Enable some portability tweaks" OFF )

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

	set( CMakeUtils_DIR "${cmake-utils_SOURCE_DIR}" )

	CPMAddPackage(
		NAME pugixml
		GITHUB_REPOSITORY zeux/pugixml
		VERSION 1.15
		OPTIONS
			"BUILD_SHARED_LIBS ON"
		)

	set( pugixml_DIR "${pugixml_BINARY_DIR}" )

	CPMAddPackage(
		NAME CLI11
		GITHUB_REPOSITORY CLIUtils/CLI11
		VERSION 2.5.0
		)

	set( CLI11_DIR "${CLI11_BINARY_DIR}" )
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

find_package( CLI11 2.4.0 REQUIRED )
if( CLI11_FOUND )
	message( STATUS "Found CLI11: ${CLI11_DIR} ${CLI11_VERSION}" )
endif()

find_package( Qt6
	COMPONENTS
	Core          REQUIRED
	Gui           REQUIRED
	Widgets       REQUIRED
	)
if( Qt6_FOUND )
	message( STATUS "Found Qt: ${Qt6_DIR} ${Qt6_VERSION}" )
endif()

find_program( D2 NAMES d2 )
if( D2 )
	message( STATUS "Found D2: ${D2}" )
else()
	message( STATUS "Not found D2: Some parts of doc disabled" )
endif()

if( CMakeUtils_FOUND )
	set( CMAKE_UTILS * )
	set( COVERAGE_GLOBAL_ONLY ON )
	include( CMakeUtils )
endif()

include( GNUInstallDirs )

set( CMAKE_INSTALL_DEFAULT_DIRECTORY_PERMISSIONS
	OWNER_READ
	OWNER_WRITE
	OWNER_EXECUTE
	GROUP_READ
	GROUP_EXECUTE
	WORLD_READ
	WORLD_EXECUTE
	)

add_subdirectory( "${CMAKE_SOURCE_DIR}/src" )
add_subdirectory( "${CMAKE_SOURCE_DIR}/test" )
add_subdirectory( "${CMAKE_SOURCE_DIR}/icon" )
add_subdirectory( "${CMAKE_SOURCE_DIR}/doc" )

if( CMakeUtils_FOUND )
	coverage_global()
endif()
