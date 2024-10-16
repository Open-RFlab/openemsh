
compile_latex_file( "icon" REGISTER_TO "ICON_FILES" )
add_custom_target( "icon" DEPENDS ${ICON_FILES} )
