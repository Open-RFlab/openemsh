
add_custom_target( unittest )

if( Catch2_FOUND )
	add_executable( openemsh_unittest EXCLUDE_FROM_ALL )
	add_dependencies( unittest openemsh_unittest )

	target_sources( openemsh_unittest
		PRIVATE
		"${CMAKE_CURRENT_SOURCE_DIR}/app/test_openemsh.cpp"
		"${CMAKE_CURRENT_SOURCE_DIR}/domain/geometrics/test_bounding.cpp"
		"${CMAKE_CURRENT_SOURCE_DIR}/domain/geometrics/test_space.cpp"
		"${CMAKE_CURRENT_SOURCE_DIR}/domain/geometrics/test_coord.cpp"
		"${CMAKE_CURRENT_SOURCE_DIR}/domain/geometrics/test_point.cpp"
		"${CMAKE_CURRENT_SOURCE_DIR}/domain/geometrics/test_segment.cpp"
		"${CMAKE_CURRENT_SOURCE_DIR}/domain/geometrics/test_edge.cpp"
		"${CMAKE_CURRENT_SOURCE_DIR}/domain/geometrics/test_range.cpp"
		"${CMAKE_CURRENT_SOURCE_DIR}/domain/geometrics/test_polygon.cpp"
		"${CMAKE_CURRENT_SOURCE_DIR}/domain/conflicts/test_conflict_colinear_edges.cpp"
		"${CMAKE_CURRENT_SOURCE_DIR}/domain/conflicts/test_conflict_edge_in_polygon.cpp"
		"${CMAKE_CURRENT_SOURCE_DIR}/domain/conflicts/test_conflict_too_close_meshline_policies.cpp"
		"${CMAKE_CURRENT_SOURCE_DIR}/domain/mesh/test_interval.cpp"
		"${CMAKE_CURRENT_SOURCE_DIR}/domain/mesh/test_meshline_policy.cpp"
		"${CMAKE_CURRENT_SOURCE_DIR}/domain/test_conflict_manager.cpp"
		"${CMAKE_CURRENT_SOURCE_DIR}/domain/test_meshline_policy_manager.cpp"
		"${CMAKE_CURRENT_SOURCE_DIR}/domain/test_board.cpp"
		"${CMAKE_CURRENT_SOURCE_DIR}/infra/serializers/test_serializer_to_plantuml.cpp"
		"${CMAKE_CURRENT_SOURCE_DIR}/infra/utils/test_to_string.cpp"
		"${CMAKE_CURRENT_SOURCE_DIR}/utils/test_down_up_cast.cpp"
		"${CMAKE_CURRENT_SOURCE_DIR}/utils/test_signum.cpp"
		"${CMAKE_CURRENT_SOURCE_DIR}/utils/test_vector_utils.cpp"
		"${CMAKE_CURRENT_SOURCE_DIR}/utils/test_tree_node.cpp"
		"${CMAKE_CURRENT_SOURCE_DIR}/utils/test_state_management.cpp"
		)

	target_compile_definitions( openemsh_unittest
		PRIVATE
		UNITTEST
		$<$<CONFIG:Debug>:DEBUG>
		OEMSH_UNITTEST_DIR="${CMAKE_CURRENT_BINARY_DIR}/CMakeFiles/openemsh_unittest.dir"
		)

	target_compile_features( openemsh_unittest
		PRIVATE
		cxx_std_23
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
		PRIVATE
		Catch2::Catch2WithMain
		openemsh
		)

	if( CMakeUtils_FOUND )
		enable_clang_build_analyzer(
			TARGET openemsh_unittest
			)

		enable_sanitizers(
			TARGET openemsh_unittest
			)

		coverage(
			TARGET_TO_RUN openemsh_unittest
			TARGETS_TO_COVER openemsh openemsh_unittest
#			ARGS_GCOVR
#				-f "${CMAKE_SOURCE_DIR}/src"
#				-e "${CMAKE_SOURCE_DIR}/src/domain/board.cpp"
#				-f "${CMAKE_CURRENT_BINARY_DIR}"
			)
	endif()
endif()
