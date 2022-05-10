
add_subdirectory( "${CMAKE_CURRENT_SOURCE_DIR}/unit" )

include( CTest )

add_custom_target( check
	VERBATIM
	USES_TERMINAL
	COMMAND ${CMAKE_CTEST_COMMAND} --output-on-failure
	COMMENT "Running tests"
	DEPENDS unittest
	)

add_custom_target( checkall
	VERBATIM
	USES_TERMINAL
	COMMAND ${CMAKE_CTEST_COMMAND} --output-on-failure -V
	COMMENT "Running tests"
	DEPENDS unittest
	)

if( Catch2_FOUND )
	add_test( NAME OpenemshUnittest
		COMMAND $<TARGET_FILE:openemsh_unittest> --use-colour=yes
		)
endif()
