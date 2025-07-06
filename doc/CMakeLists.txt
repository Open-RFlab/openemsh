add_custom_target( doc )

if( D2 )

	set( D2_INDIVIDUAL_STEPS_FILES
		"${CMAKE_CURRENT_BINARY_DIR}/state_management/1.svg"
		"${CMAKE_CURRENT_BINARY_DIR}/state_management/2.svg"
		"${CMAKE_CURRENT_BINARY_DIR}/state_management/3.svg"
		"${CMAKE_CURRENT_BINARY_DIR}/state_management/4.svg"
		"${CMAKE_CURRENT_BINARY_DIR}/state_management/5.svg"
		"${CMAKE_CURRENT_BINARY_DIR}/state_management/6.svg"
		"${CMAKE_CURRENT_BINARY_DIR}/state_management/7.svg"
		"${CMAKE_CURRENT_BINARY_DIR}/state_management/8.svg"
		"${CMAKE_CURRENT_BINARY_DIR}/state_management/9.svg"
		"${CMAKE_CURRENT_BINARY_DIR}/state_management/10.svg"
		"${CMAKE_CURRENT_BINARY_DIR}/state_management/11.svg"
		"${CMAKE_CURRENT_BINARY_DIR}/state_management/12.svg"
		"${CMAKE_CURRENT_BINARY_DIR}/state_management/13.svg"
		"${CMAKE_CURRENT_BINARY_DIR}/state_management/14.svg"
		)

	add_custom_command(
		OUTPUT ${D2_INDIVIDUAL_STEPS_FILES}
		BYPRODUCTS "${CMAKE_CURRENT_BINARY_DIR}/state_management/index.svg"
		DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/state_management.d2"
		COMMAND ${D2}
			"${CMAKE_CURRENT_SOURCE_DIR}/state_management.d2"
			"${CMAKE_CURRENT_BINARY_DIR}/state_management.svg"
			2> /dev/null
		COMMENT
			"Generating individual steps state_management/*.svg"
		)

	add_custom_command(
		OUTPUT "${CMAKE_CURRENT_BINARY_DIR}/state_management.svg"
		DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/state_management.d2"
		COMMAND ${D2}
			"${CMAKE_CURRENT_SOURCE_DIR}/state_management.d2"
			"${CMAKE_CURRENT_BINARY_DIR}/state_management.svg"
			--animate-interval=2000
			2> /dev/null
		COMMENT
			"Generating animated state_management.svg"
		)

	add_custom_target( d2
		DEPENDS
			${D2_INDIVIDUAL_STEPS_FILES}
			"${CMAKE_CURRENT_BINARY_DIR}/state_management.svg"
		)

	add_dependencies( doc d2 )

endif()
