
if( Catch2_FOUND )
	add_custom_target( unittest )

	add_executable( openemsh_unittest EXCLUDE_FROM_ALL )
	add_dependencies( unittest openemsh_unittest )

	target_sources( openemsh_unittest
		PRIVATE
		"${CMAKE_CURRENT_SOURCE_DIR}/domain/geometrics/test_bounding.cpp"
		"${CMAKE_CURRENT_SOURCE_DIR}/domain/geometrics/test_coord.cpp"
		"${CMAKE_CURRENT_SOURCE_DIR}/domain/geometrics/test_point.cpp"
		"${CMAKE_CURRENT_SOURCE_DIR}/domain/geometrics/test_segment.cpp"
		"${CMAKE_CURRENT_SOURCE_DIR}/domain/geometrics/test_edge.cpp"
		"${CMAKE_CURRENT_SOURCE_DIR}/domain/geometrics/test_range.cpp"
		"${CMAKE_CURRENT_SOURCE_DIR}/domain/geometrics/test_polygon.cpp"
		"${CMAKE_CURRENT_SOURCE_DIR}/domain/conflicts/test_conflict_colinear_edges.cpp"
		"${CMAKE_CURRENT_SOURCE_DIR}/domain/conflicts/test_conflict_edge_in_polygon.cpp"
		"${CMAKE_CURRENT_SOURCE_DIR}/domain/mesh/test_meshline_policy.cpp"
		"${CMAKE_CURRENT_SOURCE_DIR}/domain/test_conflict_manager.cpp"
		"${CMAKE_CURRENT_SOURCE_DIR}/domain/test_meshline_policy_manager.cpp"
		"${CMAKE_CURRENT_SOURCE_DIR}/domain/test_board.cpp"
		"${CMAKE_CURRENT_SOURCE_DIR}/utils/test_signum.cpp"
		)

	target_compile_definitions( openemsh_unittest
		PRIVATE
		UNITTEST
		$<$<CONFIG:Debug>:DEBUG>
		)

	target_compile_features( openemsh_unittest
		PRIVATE
		cxx_std_17
		)

	target_compile_options( openemsh_unittest
		PRIVATE
		$<$<CONFIG:Coverage>:-O0>
		$<$<OR:$<CONFIG:Coverage>,$<CONFIG:Debug>>:-Wall>
		$<$<OR:$<CONFIG:Coverage>,$<CONFIG:Debug>>:-Wextra>
#		$<$<OR:$<CONFIG:Coverage>,$<CONFIG:Debug>>:-Weffc++>
		$<$<OR:$<CONFIG:Coverage>,$<CONFIG:Debug>>:-fexceptions>
		)

	target_include_directories( openemsh_unittest
		PRIVATE
		"${CMAKE_SOURCE_DIR}/src"
		)

	target_link_libraries( openemsh_unittest
		Catch2::Catch2WithMain
		openemsh
		)

	enable_clang_build_analyzer(
		TARGET openemsh_unittest
		)

	enable_sanitizers(
		TARGET openemsh_unittest
		)

	coverage(
		TARGET_TO_RUN openemsh_unittest
		TARGETS_TO_COVER openemsh
		ARGS_GCOVR
			-f "${CMAKE_SOURCE_DIR}/src"
		)
endif()
