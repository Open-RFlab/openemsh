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

option( SONAR "The coverage target will produce SonarQube reports instead of console / HTML" OFF )

if( CMAKE_BUILD_TYPE MATCHES Coverage )
	if( CMAKE_CXX_COMPILER_ID MATCHES GNU )
		find_program( GCOV NAMES gcov )
		if( GCOV )
			message( STATUS "Found gcov: ${GCOV}" )
		else()
			message( ERROR "Not found gcov: install it" )
		endif()

		if( SONAR )
			find_program( GCOVR NAMES gcovr )
			if( GCOVR )
				message( STATUS "Found gcovr: ${GCOVR}" )
			else()
				message( ERROR "Not found gcovr: install it" )
			endif()
		else()
			find_program( LCOV NAMES lcov )
			if( LCOV )
				message( STATUS "Found lcov: ${LCOV}" )
			else()
				message( ERROR "Not found lcov: install it" )
			endif()

			find_program( GENHTML NAMES genhtml )
			if( GENHTML )
				message( STATUS "Found genhtml: ${GENHTML}" )
			else()
				message( ERROR "Not found genhtml: install it" )
			endif()
		endif()

	elseif( CMAKE_CXX_COMPILER_ID MATCHES Clang )

		find_program( LLVM_COV NAMES llvm-cov )
		if( LLVM_COV )
			message( STATUS "Found llvm-cov: ${LLVM_COV}" )
		else()
			message( ERROR "Not found llvm-cov: install it" )
		endif()

		find_program( LLVM_PROFDATA NAMES llvm-profdata )
		if( LLVM_PROFDATA )
			message( STATUS "Found llvm-profdata: ${LLVM_PROFDATA}" )
		else()
			message( ERROR "Not found llvm-profdata: install it" )
		endif()
	endif()

	find_package( Catch2 3.0.0 REQUIRED )
	if( Catch2_FOUND )
		message( STATUS "Found Catch2: ${Catch2_VERSION}" )
	endif()

else()

	find_package( Catch2 3.0.0 QUIET )
	if( Catch2_FOUND )
		message( STATUS "Found Catch2: ${Catch2_VERSION}" )
	else()
		message( STATUS "Not found Catch2: unittest target disabled" )
	endif()
endif()

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

list( APPEND CMAKE_MODULE_PATH
	"${FETCHCONTENT_BASE_DIR}/cmake-utils-src"
	)

include( StaticTools )
include( ClangBuildAnalyzer )

add_subdirectory( "${CMAKE_SOURCE_DIR}/src" )
add_subdirectory( "${CMAKE_SOURCE_DIR}/test" )
