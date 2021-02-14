
set( SOURCES
	"${CMAKE_CURRENT_SOURCE_DIR}/domain/board.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/domain/conflict.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/domain/edge.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/domain/meshline_manager.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/domain/point.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/domain/polygon.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/ui/main.cpp"
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
	${SOURCES}
	)

#[[
target_compile_definitions( ${PROJECT_NAME}
	PRIVATE
	${QT_DEPRECATED_WARNINGS}
	QRFL_VERSION="${PROJECT_VERSION}"
	$<$<QT:false>:NO_QT>
	)
#]]

target_compile_features( ${PROJECT_NAME}
	PRIVATE
	cxx_std_17
	)

target_compile_options( ${PROJECT_NAME}
	PRIVATE
	$<$<CONFIG:Debug>:-Wall>
	$<$<CONFIG:Debug>:-Wextra>
	$<$<CONFIG:Debug>:-fexceptions>
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
