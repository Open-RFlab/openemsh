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

if( CMAKE_BUILD_TYPE MATCHES Coverage )
	find_program( GCOV NAMES gcov )
	if( GCOV )
		message( STATUS "Found gcov: ${GCOV}" )
	else()
		message( ERROR "Not found gcov: install it" )
	endif()

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

	find_package( Catch2 3.0.0 REQUIRED )
	if( Catch2_FOUND )
		message( STATUS "Found Catch2: ${Catch2_VERSION}" )
	endif()
else()
	find_package( Catch2 3.0.0 QUIET )
	if( Catch2_FOUND )
		message( STATUS "Found Catch2: ${Catch2_VERSION}" )
	else()
		message( STATUS "Not found Catch2: unittest disabled" )
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

add_subdirectory( "${CMAKE_SOURCE_DIR}/src" )
add_subdirectory( "${CMAKE_SOURCE_DIR}/test" )
