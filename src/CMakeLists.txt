
add_library( openemsh SHARED )

target_sources( openemsh
	PRIVATE
	"${CMAKE_CURRENT_SOURCE_DIR}/domain/geometrics/relation.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/domain/geometrics/bounding.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/domain/geometrics/coord.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/domain/geometrics/point.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/domain/geometrics/segment.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/domain/geometrics/edge.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/domain/geometrics/range.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/domain/geometrics/polygon.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/domain/conflicts/conflict.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/domain/conflicts/conflict_colinear_edges.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/domain/conflicts/conflict_edge_in_polygon.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/domain/conflicts/conflict_too_close_meshline_policies.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/domain/conflicts/i_conflict_origin.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/domain/mesh/i_meshline_origin.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/domain/mesh/meshline.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/domain/mesh/meshline_policy.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/domain/meshline_policy_manager.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/domain/conflict_manager.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/domain/board.cpp"
	)

target_compile_definitions( openemsh
	PRIVATE
	$<$<CONFIG:Debug>:DEBUG>
	)

target_compile_features( openemsh
	PRIVATE
	cxx_std_17
	)

target_compile_options( openemsh
	PRIVATE
	$<$<CONFIG:Coverage>:-O0>
	$<$<OR:$<CONFIG:Coverage>,$<CONFIG:Debug>>:-Wall>
	$<$<OR:$<CONFIG:Coverage>,$<CONFIG:Debug>>:-Wextra>
#	$<$<OR:$<CONFIG:Coverage>,$<CONFIG:Debug>>:-Weffc++>
	$<$<OR:$<CONFIG:Coverage>,$<CONFIG:Debug>>:-fexceptions>
	)

target_include_directories( openemsh
	PRIVATE
	${CMAKE_CURRENT_SOURCE_DIR}
	)

#[[
install(
	TARGETS openemsh
	CONFIGURATIONS Release
	RUNTIME DESTINATION "bin" #"${CMAKE_INSTALL_BINDIR}"
	)
#]]

if( CMakeUtils_FOUND )
	file( GLOB_RECURSE OPENEMSH_HEADERS "*.hpp" )

	lizard(
		TARGET openemsh
		ADDITIONAL_FILES ${OPENEMSH_HEADERS}
		ARGS
			--languages cpp
			--sort cyclomatic_complexity
		)

	iwyu(
		TARGET openemsh
		ARGS
			-p "${CMAKE_BINARY_DIR}"
			--
			-Xiwyu
			--no_comments
			--cxx17ns
		)

	clang_tidy(
		TARGET openemsh
		ARGS
			-p="${CMAKE_BINARY_DIR}"
			-checks=*
		)

	clang_format(
		TARGET openemsh
		ADDITIONAL_FILES ${OPENEMSH_HEADERS}
		ARGS
		)

	cppcheck(
		TARGET openemsh
		ADDITIONAL_FILES ${OPENEMSH_HEADERS}
		ARGS
			--enable=all
			-v
		)

	uncrustify(
		TARGET openemsh
		ADDITIONAL_FILES ${OPENEMSH_HEADERS}
		ARGS
			-c "${CMAKE_SOURCE_DIR}/.uncrustify_0.cfg"
		)

	enable_clang_build_analyzer(
		TARGET openemsh
		)

	enable_sanitizers(
		TARGET openemsh
		)
endif()
