find_program( CLANG_BUILD_ANALYZER NAMES ClangBuildAnalyzer clang-build-analyzer )
if( CLANG_BUILD_ANALYZER )
	message( STATUS "Found ClangBuildAnalyzer: ${CLANG_BUILD_ANALYZER}" )
	if( CMAKE_CXX_COMPILER_ID MATCHES Clang )
		add_custom_target( build_analyzer
			COMMENT "Clang build statistics"
			COMMAND ${CLANG_BUILD_ANALYZER} --all
				"${CMAKE_BINARY_DIR}"
				"${CMAKE_BINARY_DIR}/build_analysis"
			COMMAND ${CLANG_BUILD_ANALYZER} --analyze
				"${CMAKE_BINARY_DIR}/build_analysis"
			BYPRODUCTS
				"${CMAKE_BINARY_DIR}/build_analysis"
			)
	endif()
else()
	message( STATUS "Not found ClangBuildAnalyzer: build_analyzer targets disabled" )
endif()

################################################################################
# enable_clang_build_analyzer(
#                             [TARGET target]
#                             )
# [TARGET]
#       Target to analyse.
################################################################################
function( enable_clang_build_analyzer )
	set( OPTIONS )
	set( ONEVALUEARGS TARGET )
	set( MULTIVALUEARGS )
	cmake_parse_arguments( CBA
		"${OPTIONS}"
		"${ONEVALUEARGS}"
		"${MULTIVALUEARGS}"
		${ARGN}
		)

	if( CBA_TARGET )
		get_target_property( CBA_BIN_DIR ${CBA_TARGET} BINARY_DIR )
	else()
		message( ERROR " enable_clang_build_analyzer() : Specify a target!" )
	endif()

	if( CMAKE_CXX_COMPILER_ID MATCHES Clang )
		target_compile_options( ${CBA_TARGET}
			PRIVATE
			-ftime-trace
			)

		add_custom_target( ${CBA_TARGET}_build_analyzer
			COMMENT "${CBA_TARGET} Clang build statistics"
			COMMAND ${CLANG_BUILD_ANALYZER} --all
				"${CBA_BIN_DIR}"
				"${CBA_BIN_DIR}/${CBA_TARGET}.build_analysis"
			COMMAND ${CLANG_BUILD_ANALYZER} --analyze
				"${CBA_BIN_DIR}/${CBA_TARGET}.build_analysis"
			BYPRODUCTS
				"${CBA_BIN_DIR}/${CBA_TARGET}.build_analysis"
			)
		add_dependencies( ${CBA_TARGET}_build_analyzer ${CBA_TARGET} )
		add_dependencies( build_analyzer ${CBA_TARGET} )
	endif()
endfunction()
