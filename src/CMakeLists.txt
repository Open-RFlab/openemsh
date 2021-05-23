
set( SOURCES_DOMAIN
	"${CMAKE_CURRENT_SOURCE_DIR}/domain/relation.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/domain/bounding.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/domain/coord.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/domain/point.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/domain/segment.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/domain/edge.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/domain/range.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/domain/polygon.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/domain/meshline.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/domain/meshline_policy.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/domain/conflict.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/domain/conflict_colinear_edges.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/domain/conflict_edge_in_polygon.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/domain/i_conflict_origin.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/domain/i_meshline_origin.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/domain/meshline_policy_manager.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/domain/conflict_manager.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/domain/board.cpp"
	)

#[[
set( HEADERS
	"${CMAKE_CURRENT_SOURCE_DIR}/domain/board.hpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/domain/edge.hpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/domain/point.hpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/domain/polygon.hpp"
	)
#]]

add_library( ${PROJECT_NAME} SHARED )

target_sources( ${PROJECT_NAME}
	PRIVATE
	${SOURCES_DOMAIN}
#	"${CMAKE_CURRENT_SOURCE_DIR}/main.cpp"
	)

target_compile_definitions( ${PROJECT_NAME}
	PRIVATE
	$<$<CONFIG:Debug>:DEBUG>
	)

target_compile_features( ${PROJECT_NAME}
	PRIVATE
	cxx_std_17
	)

target_compile_options( ${PROJECT_NAME}
	PRIVATE
	$<$<CONFIG:Coverage>:-O0>
	$<$<OR:$<CONFIG:Coverage>,$<CONFIG:Debug>>:-Wall>
	$<$<OR:$<CONFIG:Coverage>,$<CONFIG:Debug>>:-Wextra>
#	$<$<OR:$<CONFIG:Coverage>,$<CONFIG:Debug>>:-Weffc++>
	$<$<OR:$<CONFIG:Coverage>,$<CONFIG:Debug>>:-fexceptions>
	$<$<AND:$<CONFIG:Coverage>,$<CXX_COMPILER_ID:GNU>>:--coverage>
	$<$<AND:$<CONFIG:Coverage>,$<CXX_COMPILER_ID:Clang>>:-fprofile-instr-generate>
	$<$<AND:$<CONFIG:Coverage>,$<CXX_COMPILER_ID:Clang>>:-fcoverage-mapping>
	)

target_include_directories( ${PROJECT_NAME}
	PRIVATE
	${CMAKE_CURRENT_SOURCE_DIR}
	)

target_link_libraries( ${PROJECT_NAME}
	$<$<AND:$<CONFIG:Coverage>,$<CXX_COMPILER_ID:GNU>>:-lgcov>
	)

target_link_options( ${PROJECT_NAME}
	PRIVATE
	$<$<AND:$<CONFIG:Coverage>,$<CXX_COMPILER_ID:Clang>>:-fprofile-instr-generate>
	)

#[[
set_target_properties( ${PROJECT_NAME} PROPERTIES
	OUTPUT_NAME "openemsh"
	)

install(
	TARGETS ${PROJECT_NAME}
	CONFIGURATIONS Release
	RUNTIME DESTINATION "bin" #"${CMAKE_INSTALL_BINDIR}"
	)
#]]
