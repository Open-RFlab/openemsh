
if( Catch2_FOUND )
	set( TESTS
		"${CMAKE_CURRENT_SOURCE_DIR}/domain/test_bounding.cpp"
		"${CMAKE_CURRENT_SOURCE_DIR}/domain/test_coord.cpp"
		"${CMAKE_CURRENT_SOURCE_DIR}/domain/test_point.cpp"
		"${CMAKE_CURRENT_SOURCE_DIR}/domain/test_segment.cpp"
		"${CMAKE_CURRENT_SOURCE_DIR}/domain/test_edge.cpp"
		"${CMAKE_CURRENT_SOURCE_DIR}/domain/test_range.cpp"
		"${CMAKE_CURRENT_SOURCE_DIR}/domain/test_polygon.cpp"
		"${CMAKE_CURRENT_SOURCE_DIR}/domain/test_conflict_manager.cpp"
		"${CMAKE_CURRENT_SOURCE_DIR}/domain/test_meshline_policy.cpp"
		"${CMAKE_CURRENT_SOURCE_DIR}/domain/test_meshline_policy_manager.cpp"
		"${CMAKE_CURRENT_SOURCE_DIR}/domain/test_board.cpp"
		"${CMAKE_CURRENT_SOURCE_DIR}/domain/test_conflict_colinear_edges.cpp"
		"${CMAKE_CURRENT_SOURCE_DIR}/domain/test_conflict_edge_in_polygon.cpp"
		)

	add_executable( unittest EXCLUDE_FROM_ALL )

	target_sources( unittest
		PRIVATE
		${TESTS}
		)

	target_compile_definitions( unittest
		PRIVATE
		UNITTEST
		$<$<CONFIG:Debug>:DEBUG>
		)

	target_compile_features( unittest
		PRIVATE
		cxx_std_17
		)

	target_compile_options( unittest
		PRIVATE
		$<$<CONFIG:Coverage>:-O0>
		$<$<OR:$<CONFIG:Coverage>,$<CONFIG:Debug>>:-Wall>
		$<$<OR:$<CONFIG:Coverage>,$<CONFIG:Debug>>:-Wextra>
#		$<$<OR:$<CONFIG:Coverage>,$<CONFIG:Debug>>:-Weffc++>
		$<$<OR:$<CONFIG:Coverage>,$<CONFIG:Debug>>:-fexceptions>
		)

	target_include_directories( unittest
		PRIVATE
		"${CMAKE_SOURCE_DIR}/src"
		)

	target_link_libraries( unittest
		Catch2::Catch2WithMain
		${PROJECT_NAME}
		)

	set_target_properties( unittest PROPERTIES
		OUTPUT_NAME "unittest_openemsh"
#		RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/out"
		)

	enable_clang_build_analyzer(
		TARGET unittest
		)
endif()
