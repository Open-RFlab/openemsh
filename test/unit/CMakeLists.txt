#include( Catch )

set( TESTS
	"${CMAKE_CURRENT_SOURCE_DIR}/domain/test_point.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/domain/test_edge.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/domain/test_polygon.cpp"
	)

set( SOURCES
	"${CMAKE_SOURCE_DIR}/src/domain/point.cpp"
	"${CMAKE_SOURCE_DIR}/src/domain/edge.cpp"
	"${CMAKE_SOURCE_DIR}/src/domain/polygon.cpp"
	)

add_executable( unittest EXCLUDE_FROM_ALL )

target_sources( unittest
	PRIVATE
	${SOURCES}
	${TESTS}
	)

target_compile_definitions( unittest
	PRIVATE
	UNITTEST
	)

target_compile_features( unittest
	PRIVATE
	cxx_std_17
	)

target_compile_options( unittest
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

target_include_directories( unittest
	PRIVATE
	"${CMAKE_SOURCE_DIR}/src"
	)

target_link_libraries( unittest
	Catch2::Catch2WithMain
	$<$<CONFIG:Coverage>:-lgcov>
	)

set_target_properties( unittest PROPERTIES
	OUTPUT_NAME "unittest_openemsh"
#	RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/out"
	)