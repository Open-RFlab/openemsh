
if( CMakeUtils_Latex_FOUND )
	# As TikZ fadings are buggy, SVG generation requires going through a PDF
	# intermediate plus a manual edition step.
	compile_latex_file( "icon" REGISTER_TO "ICON_SVG_FILE" )
endif()

if( CMakeUtils_Icon_FOUND )
	# Warning : order matters.
	#svg_to_png( "openemsh" SIZE 512 REGISTER_TO "ICON_PNG_FILES" )
	svg_to_png( "openemsh" SIZE 256 REGISTER_TO "ICON_PNG_FILES" )
	svg_to_png( "openemsh" SIZE 128 REGISTER_TO "ICON_PNG_FILES" )
	svg_to_png( "openemsh" SIZE 64 REGISTER_TO "ICON_PNG_FILES" )
	svg_to_png( "openemsh" SIZE 48 REGISTER_TO "ICON_PNG_FILES" )
	svg_to_png( "openemsh" SIZE 32 REGISTER_TO "ICON_PNG_FILES" )
	svg_to_png( "openemsh" SIZE 22 REGISTER_TO "ICON_PNG_FILES" )
	svg_to_png( "openemsh" SIZE 16 REGISTER_TO "ICON_PNG_FILES" )

	png_to_ico( "openemsh" INPUT ${ICON_PNG_FILES} REGISTER_TO "ICON_ICO_FILE" )

	add_custom_target( "icon"
		DEPENDS
			${ICON_SVG_FILE}
			${ICON_PNG_FILES}
			${ICON_ICO_FILE}
		)

	add_custom_target( "icon_replace"
		DEPENDS
	#		${ICON_SVG_FILE}
			${ICON_PNG_FILES}
			${ICON_ICO_FILE}
		COMMAND ${CMAKE_COMMAND} -E copy
			${ICON_PNG_FILES}
			${ICON_ICO_FILE}
			"${CMAKE_CURRENT_SOURCE_DIR}/"
		COMMENT "Copying icon png and ico files back to ${CMAKE_CURRENT_SOURCE_DIR}/"
		)
endif()
