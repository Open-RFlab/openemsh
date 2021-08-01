
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
		if( SONAR )

			add_custom_target( coverage
				COMMAND $<TARGET_FILE:unittest>
					-r sonarqube
					-o "${CMAKE_CURRENT_BINARY_DIR}/unit/sonarqube_report.xml"
				COMMAND ${GCOVR}
					--sonarqube "${CMAKE_CURRENT_BINARY_DIR}/coverage.xml"
					-r "${CMAKE_SOURCE_DIR}"
					-e "${CMAKE_SOURCE_DIR}/test"
					-f "${CMAKE_SOURCE_DIR}/src"
					--exclude-unreachable-branches
					--exclude-throw-branches
					-s
				COMMENT "Run unit tests, perform code coverage, generate sonarqube report"
				)

		else()

			add_custom_target( coverage
#				COMMAND ${CMAKE_COMMAND} -E env bash -c
#				"${CMAKE_CTEST_COMMAND} --output-on-failure Unittest ;:;"
				COMMAND $<TARGET_FILE:unittest>
				COMMAND ${LCOV} -c
					-d "${CMAKE_BINARY_DIR}/src/CMakeFiles/${PROJECT_NAME}.dir"
					-o "${CMAKE_CURRENT_BINARY_DIR}/coverage.tmp.info"
					--rc lcov_branch_coverage=1
				COMMAND ${LCOV} -r
					"${CMAKE_CURRENT_BINARY_DIR}/coverage.tmp.info"
#					'${CMAKE_CURRENT_BINARY_DIR}/unittest_autogen/*'
					'/usr/*'
					-o "${CMAKE_CURRENT_BINARY_DIR}/coverage.info"
					--rc lcov_branch_coverage=1
					-q
				COMMAND ${GENHTML}
					"${CMAKE_CURRENT_BINARY_DIR}/coverage.info"
					-o "${CMAKE_CURRENT_BINARY_DIR}/coverage"
					--branch-coverage
					--function-coverage
				COMMAND ${LCOV}
					-l "${CMAKE_CURRENT_BINARY_DIR}/coverage.info"
					--rc lcov_branch_coverage=1
				COMMENT "Run unit tests, perform code coverage, generate lcov report"
				)

		endif()
	elseif( CMAKE_CXX_COMPILER_ID MATCHES Clang )
		if( SONAR )

#[[
			# llvm-cov reports seems to be false and incomplete on sonarcloud.
			# Prefer the GCC version.
			add_custom_target( coverage_sonar
				COMMAND LLVM_PROFILE_FILE=${CMAKE_CURRENT_BINARY_DIR}/coverage.profraw
					$<TARGET_FILE:unittest>
					-r sonarqube
					-o "${CMAKE_CURRENT_BINARY_DIR}/unit/sonarqube_report.xml"
				COMMAND ${LLVM_PROFDATA} merge
					-sparse "${CMAKE_CURRENT_BINARY_DIR}/coverage.profraw"
					-o "${CMAKE_CURRENT_BINARY_DIR}/coverage.profdata"
				COMMAND ${LLVM_COV} show $<TARGET_FILE:${PROJECT_NAME}>
					-instr-profile="${CMAKE_CURRENT_BINARY_DIR}/coverage.profdata"
					-show-line-counts
					-show-expansions
					-show-regions
					-format="text"
#					-ignore-filename-regex='${CMAKE_SOURCE_DIR}/test/*'
					> ${CMAKE_CURRENT_BINARY_DIR}/coverage.txt
				COMMENT "Run unit tests, perform code coverage, generate sonarqube report"
				)
#]]

		else()

			add_custom_target( coverage
				COMMAND LLVM_PROFILE_FILE=${CMAKE_CURRENT_BINARY_DIR}/coverage.profraw
					$<TARGET_FILE:unittest>
				COMMAND ${LLVM_PROFDATA} merge
					-sparse "${CMAKE_CURRENT_BINARY_DIR}/coverage.profraw"
					-o "${CMAKE_CURRENT_BINARY_DIR}/coverage.profdata"
				COMMAND ${LLVM_COV} report $<TARGET_FILE:${PROJECT_NAME}>
					-instr-profile="${CMAKE_CURRENT_BINARY_DIR}/coverage.profdata"
#					-ignore-filename-regex='${CMAKE_SOURCE_DIR}/test/*'
				COMMAND ${LLVM_COV} show $<TARGET_FILE:${PROJECT_NAME}>
					-instr-profile="${CMAKE_CURRENT_BINARY_DIR}/coverage.profdata"
					-output-dir="${CMAKE_CURRENT_BINARY_DIR}/coverage"
					-show-line-counts
					-show-expansions
					-show-regions
					-format="html"
#					-ignore-filename-regex='${CMAKE_SOURCE_DIR}/test/*'
				COMMENT "Run unit tests, perform code coverage, generate llvm-cov report"
				)

		endif()
	endif()
endif()
