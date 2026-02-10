
################################################################################
function( configure_file_build_time INPUT OUTPUT )
	set( OPTIONS )
	set( ONEVALUEARGS )
	set( MULTIVALUEARGS VARS )
#	set( MULTIVALUEARGS ARGS VARS )
	cmake_parse_arguments( CFBT
		"${OPTIONS}"
		"${ONEVALUEARGS}"
		"${MULTIVALUEARGS}"
		${ARGN}
		)

	while( CFBT_VARS )
		list( POP_FRONT CFBT_VARS VAR VAL )
		list( APPEND VARS "-D${VAR}=\"${VAL}\"" )
	endwhile()

	file( WRITE
		"${OUTPUT}.cmake"
		"configure_file(\"${INPUT}\" \"${OUTPUT}\" ${CFBT_UNPARSED_ARGUMENTS})"
#		"configure_file(\"${INPUT}\" \"${OUTPUT}\" ${CFBT_ARGS})"
		)

	add_custom_command(
		OUTPUT ${OUTPUT}
		DEPENDS ${INPUT}
		COMMAND ${CMAKE_COMMAND}
			${VARS}
			-P "${OUTPUT}.cmake"
		)
endfunction()
