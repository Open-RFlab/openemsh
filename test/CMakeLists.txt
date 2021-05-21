
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

add_test( NAME Unittest
	COMMAND $<TARGET_FILE:unittest> --use-colour=yes
	)

if( CMAKE_BUILD_TYPE MATCHES Coverage )
	if( CMAKE_CXX_COMPILER_ID MATCHES GNU )

		add_custom_target( coverage
#			COMMAND ${CMAKE_CTEST_COMMAND} --output-on-failure Unittest
			COMMAND ${CMAKE_COMMAND} -E env bash -c
			"${CMAKE_CTEST_COMMAND} --output-on-failure Unittest ;:;"
			COMMAND ${LCOV} -c
				-d "${CMAKE_CURRENT_BINARY_DIR}/unit/CMakeFiles/unittest.dir/__/__/src"
				-o "${CMAKE_CURRENT_BINARY_DIR}/coverage.tmp.info"
			COMMAND ${LCOV} -q
				-r "${CMAKE_CURRENT_BINARY_DIR}/coverage.tmp.info" '${CMAKE_CURRENT_BINARY_DIR}/unittest_autogen/*' '/usr/*'
				-o "${CMAKE_CURRENT_BINARY_DIR}/coverage.info"
			COMMAND ${GENHTML}
				"${CMAKE_CURRENT_BINARY_DIR}/coverage.info"
				-o "${CMAKE_CURRENT_BINARY_DIR}/coverage"
			COMMAND ${LCOV} -l "${CMAKE_CURRENT_BINARY_DIR}/coverage.info"
			COMMENT "Run unit tests, perform code coverage, generate lcov report"
			DEPENDS unittest
			)

		add_custom_target( coverage-ci
			COMMAND $<TARGET_FILE:unittest>
				-r sonarqube
				-o "${CMAKE_CURRENT_BINARY_DIR}/unit/sonarqube_report.xml"
			COMMAND ${GCOVR} 
				--sonarqube
				-r "${CMAKE_SOURCE_DIR}"
				-e "${CMAKE_SOURCE_DIR}/test"
				-f "${CMAKE_SOURCE_DIR}/src"
				--exclude-unreachable-branches
				--exclude-throw-branches
				-s
				--sonarqube "${CMAKE_CURRENT_BINARY_DIR}/coverage.xml"
			COMMENT "Run unit tests, perform code coverage, generate gcover report"
			DEPENDS unittest
			)

	elseif( CMAKE_CXX_COMPILER_ID MATCHES Clang )

		add_custom_target( coverage
#			COMMAND ${CMAKE_CTEST_COMMAND} --output-on-failure Unittest
#			COMMAND ${CMAKE_COMMAND} -E env bash -c
#			"${CMAKE_CTEST_COMMAND} --output-on-failure Unittest ;:;"
			COMMAND LLVM_PROFILE_FILE=${CMAKE_CURRENT_BINARY_DIR}/coverage.profraw $<TARGET_FILE:unittest>
			COMMAND ${LLVM_PROFDATA} merge
				-sparse "${CMAKE_CURRENT_BINARY_DIR}/coverage.profraw"
				-o "${CMAKE_CURRENT_BINARY_DIR}/coverage.profdata"
			COMMAND ${LLVM_COV} report $<TARGET_FILE:unittest>
				-instr-profile="${CMAKE_CURRENT_BINARY_DIR}/coverage.profdata"
				-ignore-filename-regex='${CMAKE_SOURCE_DIR}/test/*'
			COMMAND ${LLVM_COV} show $<TARGET_FILE:unittest>
				-instr-profile="${CMAKE_CURRENT_BINARY_DIR}/coverage.profdata"
				-output-dir="${CMAKE_CURRENT_BINARY_DIR}/coverage"
				-show-line-counts
				-show-expansions
				-show-regions
				-format="html"
				-ignore-filename-regex='${CMAKE_SOURCE_DIR}/test/*'
#[[
			COMMAND LLVM_PROFILE_FILE=${CMAKE_CURRENT_BINARY_DIR}/coverage.profraw $<TARGET_FILE:unittest>
				-r sonarqube
				-o "${CMAKE_CURRENT_BINARY_DIR}/unit/sonarqube_report.xml"
			COMMAND ${LLVM_COV} show $<TARGET_FILE:unittest>
				-instr-profile="${CMAKE_CURRENT_BINARY_DIR}/coverage.profdata"
				-show-line-counts
				-show-expansions
				-show-regions
				-format="text"
				-ignore-filename-regex="${CMAKE_SOURCE_DIR}/test/*"
				> ${CMAKE_CURRENT_BINARY_DIR}/coverage.txt
			COMMAND ${LLVM_COV} export $<TARGET_FILE:unittest>
				-instr-profile="${CMAKE_CURRENT_BINARY_DIR}/coverage.profdata"
				-format="text"
				-ignore-filename-regex="${CMAKE_SOURCE_DIR}/test/*"
				> ${CMAKE_CURRENT_BINARY_DIR}/coverage.json
			COMMENT "Run unit tests, perform code coverage, generate llvm-cov report"
#]]
			DEPENDS unittest
			)

		add_custom_target( coverage-ci
			COMMAND LLVM_PROFILE_FILE=${CMAKE_CURRENT_BINARY_DIR}/coverage.profraw $<TARGET_FILE:unittest>
				-r sonarqube
				-o "${CMAKE_CURRENT_BINARY_DIR}/unit/sonarqube_report.xml"
			COMMAND ${LLVM_PROFDATA} merge
				-sparse "${CMAKE_CURRENT_BINARY_DIR}/coverage.profraw"
				-o "${CMAKE_CURRENT_BINARY_DIR}/coverage.profdata"
			COMMAND ${LLVM_COV} show $<TARGET_FILE:unittest>
				-instr-profile="${CMAKE_CURRENT_BINARY_DIR}/coverage.profdata"
				-show-line-counts
				-show-expansions
				-show-regions
				-format="text"
				-ignore-filename-regex='${CMAKE_SOURCE_DIR}/test/*'
				> ${CMAKE_CURRENT_BINARY_DIR}/coverage.txt
#[[
			COMMAND ${LLVM_COV} report $<TARGET_FILE:unittest>
				-instr-profile="${CMAKE_CURRENT_BINARY_DIR}/coverage.profdata"
				-ignore-filename-regex='${CMAKE_SOURCE_DIR}/test/*'
			COMMAND ${LLVM_COV} export $<TARGET_FILE:unittest>
				-instr-profile="${CMAKE_CURRENT_BINARY_DIR}/coverage.profdata"
				-format="text"
				-ignore-filename-regex='${CMAKE_SOURCE_DIR}/test/*'
				> ${CMAKE_CURRENT_BINARY_DIR}/coverage.json
			COMMENT "Run unit tests, perform code coverage, generate llvm-cov report"
#]]
			DEPENDS unittest
			)

	endif()
endif()
