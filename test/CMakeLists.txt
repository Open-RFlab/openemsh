add_subdirectory(
	"${CMAKE_CURRENT_SOURCE_DIR}/unit"
	)

include( CTest )

add_custom_target( check
	VERBATIM
	COMMAND ${FIXED_SHELL}
	"echo -e \"[----] \\033[36mRunning tests\\033[0m\""
	USES_TERMINAL
	COMMAND ${CMAKE_CTEST_COMMAND} --output-on-failure
	DEPENDS unittest
	)

add_custom_target( checkall
	VERBATIM
	COMMAND ${FIXED_SHELL}
	"echo -e \"[----] \\033[36mRunning tests\\033[0m\""
	USES_TERMINAL
	COMMAND ${CMAKE_CTEST_COMMAND} --output-on-failure -V
	DEPENDS unittest
	)

add_test( NAME Unittest
	COMMAND ${FIXED_SHELL}
	"$<TARGET_FILE:unittest> --use-colour=yes 2> /dev/null"
	)

if( CMAKE_BUILD_TYPE MATCHES Coverage )

	add_custom_target( coverage
		COMMAND ${CMAKE_CTEST_COMMAND} --output-on-failure Unittest
		COMMAND ${LCOV} -d "${CMAKE_CURRENT_BINARY_DIR}/unit/CMakeFiles/unittest.dir/__/__/src" -c -o "${CMAKE_CURRENT_BINARY_DIR}/coverage.tmp.info"
		COMMAND ${LCOV} -q -r "${CMAKE_CURRENT_BINARY_DIR}/coverage.tmp.info" '${CMAKE_CURRENT_BINARY_DIR}/unittest_autogen/*' '/usr/*' -o "${CMAKE_CURRENT_BINARY_DIR}/coverage.info"
		COMMAND ${GENHTML} "${CMAKE_CURRENT_BINARY_DIR}/coverage.info" -o "${CMAKE_CURRENT_BINARY_DIR}/coverage"
		COMMENT "Run unit tests, perform code coverage, generate lcov report"
		DEPENDS unittest
		)

endif()
