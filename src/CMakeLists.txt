
add_library( openemsh SHARED )

target_sources( openemsh
	PRIVATE
	"${CMAKE_CURRENT_SOURCE_DIR}/domain/geometrics/space.cpp"
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
	"${CMAKE_CURRENT_SOURCE_DIR}/domain/mesh/interval.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/domain/mesh/meshline.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/domain/mesh/meshline_policy.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/domain/meshline_policy_manager.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/domain/conflict_manager.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/domain/board.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/infra/parsers/csxcad_layer/point_3d.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/infra/parsers/parser_from_csx.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/infra/serializers/serializer_to_csx.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/infra/serializers/serializer_to_plantuml.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/infra/serializers/serializer_to_prettyprint.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/infra/utils/to_string.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/app/openemsh.cpp"
	)

set_target_properties( openemsh
	PROPERTIES
	POSITION_INDEPENDENT_CODE ON
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
	$<$<OR:$<CONFIG:Coverage>,$<CONFIG:Debug>>:-fno-exceptions>
	)

target_include_directories( openemsh
	PRIVATE
	${CMAKE_CURRENT_SOURCE_DIR}
	)

target_link_libraries( openemsh
	PRIVATE
	pugixml::shared
	)

add_executable( openemsh_bin )

target_sources( openemsh_bin
	PRIVATE
	"${CMAKE_CURRENT_SOURCE_DIR}/ui/cli.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/ui/qt/about_dialog.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/ui/qt/main_window.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/ui/qt/resources.qrc"
	"${CMAKE_CURRENT_SOURCE_DIR}/main.cpp"
	)

target_compile_definitions( openemsh_bin
	PRIVATE
	OEMSH_VERSION="${PROJECT_VERSION}"
	OEMSH_HOMEPAGE="${PROJECT_HOMEPAGE_URL}"
	OEMSH_FUNDING="${OEMSH_FUNDING}"
	OEMSH_BUGREPORT="${OEMSH_BUGREPORT}"
	)

target_compile_features( openemsh_bin
	PRIVATE
	cxx_std_17
	)

target_compile_options( openemsh_bin
	PRIVATE
	$<$<CONFIG:Coverage>:-O0>
	$<$<OR:$<CONFIG:Coverage>,$<CONFIG:Debug>>:-Wall>
	$<$<OR:$<CONFIG:Coverage>,$<CONFIG:Debug>>:-Wextra>
#	$<$<OR:$<CONFIG:Coverage>,$<CONFIG:Debug>>:-Weffc++>
	$<$<OR:$<CONFIG:Coverage>,$<CONFIG:Debug>>:-fexceptions>
	)

target_include_directories( openemsh_bin
	PRIVATE
	${CMAKE_CURRENT_SOURCE_DIR}
	)

target_link_libraries( openemsh_bin
	PRIVATE
	openemsh
	CLI11::CLI11
	Qt6::Core
	Qt6::Gui
	Qt6::Widgets
	)

#[[
install(
	TARGETS openemsh
	CONFIGURATIONS Release
	RUNTIME DESTINATION "bin" #"${CMAKE_INSTALL_BINDIR}"
	)
#]]

set_target_properties( openemsh_bin
	PROPERTIES
	AUTOMOC ON
	AUTORCC ON
	AUTOUIC ON
	)

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
