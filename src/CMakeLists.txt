
set( SOURCES_DOMAIN
	"${CMAKE_CURRENT_SOURCE_DIR}/domain/relation.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/domain/point.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/domain/segment.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/domain/edge.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/domain/range.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/domain/polygon.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/domain/meshline_manager.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/domain/conflict.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/domain/conflict_colinear_edges.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/domain/conflict_edge_in_polygon.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/domain/conflict_manager.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/domain/board.cpp"
	)

set_property( GLOBAL
	PROPERTY "GLOBAL_SOURCES_DOMAIN" "${SOURCES_DOMAIN}"
	)

#[[
set( HEADERS
	"${CMAKE_CURRENT_SOURCE_DIR}/domain/board.hpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/domain/edge.hpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/domain/point.hpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/domain/polygon.hpp"
	)
#]]

add_executable( ${PROJECT_NAME} )

target_sources( ${PROJECT_NAME}
	PRIVATE
	${SOURCES_DOMAIN}
	"${CMAKE_CURRENT_SOURCE_DIR}/main.cpp"
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
	$<$<CONFIG:Debug>:-Wall>
	$<$<CONFIG:Debug>:-Wextra>
	$<$<CONFIG:Debug>:-fexceptions>
	$<$<CONFIG:Coverage>:-Wall>
	$<$<CONFIG:Coverage>:-Wextra>
	$<$<CONFIG:Coverage>:-fexceptions>
	$<$<CONFIG:Coverage>:--coverage>
	$<$<CONFIG:Coverage>:-O0>
	)

target_include_directories( ${PROJECT_NAME}
	PRIVATE
	${CMAKE_CURRENT_SOURCE_DIR}
	)

#[[
qt5_use_modules( ${PROJECT_NAME}
	Core
	Gui
	OpenGL
	)
#]]

#[[
set_target_properties( ${PROJECT_NAME} PROPERTIES
	OUTPUT_NAME "qucsrflayout"
	RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/out"
	)
#]]

install(
	TARGETS ${PROJECT_NAME}
	CONFIGURATIONS Release
	RUNTIME DESTINATION "bin" #"${CMAKE_INSTALL_BINDIR}"
	)
