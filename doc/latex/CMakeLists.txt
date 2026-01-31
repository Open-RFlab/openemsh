if( CMakeUtils_Latex_FOUND )
	compile_latex_file( "interval" REGISTER_TO "LATEX_FILES" VERBOSE )

	add_custom_target( "latex" DEPENDS ${LATEX_FILES} )
	add_dependencies( "doc" "latex" )

	install(
		FILES
			${LATEX_FILES}
		DESTINATION "${CMAKE_INSTALL_DOCDIR}/openemsh"
		OPTIONAL
		)
endif()
