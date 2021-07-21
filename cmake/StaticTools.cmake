option( STATIC_ERROR "Do not tolerate errors in the static analysis tools" )
option( STATIC_WERROR "Change warning into errors in the static analysis tools" )
set( CMAKE_EXPORT_COMPILE_COMMANDS on )

find_program( LIZARD NAMES lizard )
if( LIZARD )
	message( STATUS "Found Lizard: ${LIZARD}" )
	add_custom_target( lizard )
else()
	message( STATUS "Not found Lizard: lizard targets disabled" )
endif()

find_program( IWYU NAMES iwyu )
if( IWYU )
	message( STATUS "Found IWYU: ${IWYU}" )
	add_custom_target( iwyu )
else()
	message( STATUS "Not found IWYU: iwyu targets disabled" )
endif()

#[[
find_program( CLANG_INCLUDE_FIXER NAMES include-fixer )
find_program( FIND_ALL_SYMBOLS NAMES find-all-symbols )
if( CLANG_INCLUDE_FIXER AND FIND_ALL_SYMBOLS )
	message( STATUS "Found Clang-Include-Fixer: ${CLANG_INCLUDE_FIXER}" )
	message( STATUS "Found Find-All-Symbols: ${FIND_ALL_SYMBOLS}" )
	add_custom_target( include_fixer )
else()
	message( STATUS "Not found Clang-Include-Fixer or Find-All-Symbols: clang-include-fixer targets disabled" )
endif()
#]]

find_program( CLANG_TIDY NAMES clang-tidy )
if( CLANG_TIDY )
	message( STATUS "Found Clang-Tidy: ${CLANG_TIDY}" )
	add_custom_target( tidy )
	add_custom_target( tidy_fix )
else()
	message( STATUS "Not found Clang-Tidy: tidy targets disabled" )
endif()

find_program( CLANG_FORMAT NAMES clang-format )
if( CLANG_FORMAT )
	message( STATUS "Found Clang-Format: ${CLANG_FORMAT}" )
	add_custom_target( format )
	add_custom_target( format_fix )
else()
	message( STATUS "Not found Clang-Format: format targets disabled" )
endif()

find_program( CPPCHECK NAMES cppcheck )
if( CPPCHECK )
	message( STATUS "Found Cppcheck: ${CPPCHECK}" )
	add_custom_target( cppcheck )
else()
	message( STATUS "Not found Cppcheck: cppcheck targets disabled" )
endif()

################################################################################
# static_analysis_lizard(
#                        [TARGET target]
#                        [ADDITIONAL_FILES file1 [file2] ...]
#                        [ARGS arg1 [arg2] ...]
#                        )
# [TARGET]
#       Target to analyse. Eavery sourcce file will be analysed.
# [ADDITIONAL_FILES]
#       Specify other files to analyse, headers for instance.
# [ARGS]
#       Specify command line arguments.
################################################################################
function( static_analysis_lizard )
	set( OPTIONS )
	set( ONEVALUEARGS TARGET )
	set( MULTIVALUEARGS ADDITIONAL_FILES ARGS )
	cmake_parse_arguments( LIZARD
		"${OPTIONS}"
		"${ONEVALUEARGS}"
		"${MULTIVALUEARGS}"
		${ARGN}
		)

	if( STATIC_ERROR )
		set( LIZARD_ERROR 1 )
	else()
		set( LIZARD_ERROR 0 )
	endif()
	if( LIZARD_TARGET )
		get_target_property( LIZARD_SRC ${LIZARD_TARGET} SOURCES )
	else()
		message( ERROR " static_analysis_lizard() : Specify a target!" )
	endif()
	foreach( ARG ${LIZARD_ARGS} )
		list( APPEND ALL_ARGS ${ARG} )
	endforeach()

	add_custom_target( ${LIZARD_TARGET}_lizard
		SOURCES ${LIZARD_SRC} ${LIZARD_ADDITIONAL_FILES}
		COMMENT "Lizard"
		COMMAND ${LIZARD}
			${ALL_ARGS}
			${LIZARD_SRC}
			${LIZARD_ADDITIONAL_FILES}
			|| exit ${LIZARD_ERROR}
		)
	add_dependencies( lizard ${LIZARD_TARGET}_lizard )
endfunction()

################################################################################
# static_analysis_iwyu(
#                        [TARGET target]
#                        [ADDITIONAL_FILES file1 [file2] ...]
#                        [ARGS arg1 [arg2] ...]
#                        )
# [TARGET]
#       Target to analyse. Eavery sourcce file will be analysed.
# [ADDITIONAL_FILES]
#       Specify other files to analyse, headers for instance.
# [ARGS]
#       Specify command line arguments.
################################################################################
function( static_analysis_iwyu )
	set( OPTIONS )
	set( ONEVALUEARGS TARGET )
	set( MULTIVALUEARGS ADDITIONAL_FILES ARGS )
	cmake_parse_arguments( IWYU
		"${OPTIONS}"
		"${ONEVALUEARGS}"
		"${MULTIVALUEARGS}"
		${ARGN}
		)

	if( STATIC_ERROR )
		set( IWYU_ERROR 1 )
	else()
		set( IWYU_ERROR 0 )
	endif()
	if( IWYU_TARGET )
		get_target_property( IWYU_SRC ${IWYU_TARGET} SOURCES )
	else()
		message( ERROR "Specify a target!" )
	endif()
	foreach( ARG ${IWYU_ARGS} )
		list( APPEND ALL_ARGS ${ARG} )
	endforeach()

	add_custom_target( ${IWYU_TARGET}_iwyu
		SOURCES ${IWYU_SRC} ${IWYU_ADDITIONAL_FILES}
#		COMMENT "Include What You Use"
		)

	list( APPEND IWYU_SRC ${IWYU_ADDITIONAL_FILES} )
	foreach( SRC ${IWYU_SRC} )
		add_custom_command(
			TARGET ${IWYU_TARGET}_iwyu
			PRE_BUILD
			COMMAND ${IWYU}
				${IWYU_ARGS}
				${SRC}
				|| exit ${IWYU_ERROR}
			COMMENT "IWYU ${SRC}"
			)
	endforeach()
	add_dependencies( iwyu ${IWYU_TARGET}_iwyu )
endfunction()

#[[
################################################################################
# static_analysis_include_fixer(
#                        [TARGET target]
#                        [ADDITIONAL_FILES file1 [file2] ...]
#                        [ARGS arg1 [arg2] ...]
#                        )
# [TARGET]
#       Target to analyse. Eavery sourcce file will be analysed.
# [ADDITIONAL_FILES]
#       Specify other files to analyse, headers for instance.
# [ARGS]
#       Specify command line arguments.
################################################################################
function( static_analysis_include_fixer )
	set( OPTIONS )
	set( ONEVALUEARGS TARGET )
	set( MULTIVALUEARGS ADDITIONAL_FILES ARGS )
	cmake_parse_arguments( CLANG_INCLUDE_FIXER
		"${OPTIONS}"
		"${ONEVALUEARGS}"
		"${MULTIVALUEARGS}"
		${ARGN}
		)

	if( STATIC_ERROR )
		set( CLANG_INCLUDE_FIXER_ERROR 1 )
	else()
		set( CLANG_INCLUDE_FIXER_ERROR 0 )
	endif()
	if( CLANG_INCLUDE_FIXER_TARGET )
		get_target_property( CLANG_INCLUDE_FIXER_SRC ${CLANG_INCLUDE_FIXER_TARGET} SOURCES )
	else()
		message( ERROR "Specify a target!" )
	endif()
	foreach( ARG ${CLANG_INCLUDE_FIXER_ARGS} )
		list( APPEND ALL_ARGS ${ARG} )
	endforeach()

	add_custom_target( ${CLANG_INCLUDE_FIXER_TARGET}_include_fixer
		SOURCES ${CLANG_INCLUDE_FIXER_SRC} ${CLANG_INCLUDE_FIXER_ADDITIONAL_FILES}
		COMMENT "Include What You Use"
		)

	list( APPEND CLANG_INCLUDE_FIXER_SRC ${CLANG_INCLUDE_FIXER_ADDITIONAL_FILES} )
	foreach( SRC ${CLANG_INCLUDE_FIXER_SRC} )
		add_custom_command(
			TARGET ${CLANG_INCLUDE_FIXER_TARGET}_include_fixer
			PRE_BUILD
			COMMAND ${CLANG_INCLUDE_FIXER}
				${CLANG_INCLUDE_FIXER_ARGS}
				${SRC}
				|| exit ${CLANG_INCLUDE_FIXER_ERROR}
			COMMENT "Clang-Include-Fixer ${SRC}"
			)
	endforeach()
	add_dependencies( include_fixer ${CLANG_INCLUDE_FIXER_TARGET}_include_fixer )
endfunction()
#]]

################################################################################
# static_analysis_tidy(
#                        [TARGET target]
#                        [ADDITIONAL_FILES file1 [file2] ...]
#                        [ARGS arg1 [arg2] ...]
#                        )
# [TARGET]
#       Target to analyse. Eavery sourcce file will be analysed.
# [ADDITIONAL_FILES]
#       Specify other files to analyse, headers for instance.
# [ARGS]
#       Specify command line arguments.
################################################################################
function( static_analysis_tidy )
	set( OPTIONS )
	set( ONEVALUEARGS TARGET )
	set( MULTIVALUEARGS ADDITIONAL_FILES ARGS )
	cmake_parse_arguments( TIDY
		"${OPTIONS}"
		"${ONEVALUEARGS}"
		"${MULTIVALUEARGS}"
		${ARGN}
		)

	if( STATIC_ERROR )
		set( TIDY_ERROR 1 )
	else()
		set( TIDY_ERROR 0 )
	endif()
	if( STATIC_WERROR )
		set( TIDY_WERROR -warnings-as-errors=* )
	else()
		set( TIDY_WERROR )
	endif()
	if( TIDY_TARGET )
		get_target_property( TIDY_SRC ${TIDY_TARGET} SOURCES )
	else()
		message( ERROR "Specify a target!" )
	endif()
	foreach( ARG ${TIDY_ARGS} )
		list( APPEND ALL_ARGS ${ARG} )
	endforeach()
	list( APPEND TIDY_SRC ${TIDY_ADDITIONAL_FILES} )

	add_custom_target( ${TIDY_TARGET}_tidy
		SOURCES ${TIDY_SRC}
#		COMMENT "Clang-Tidy check"
		)

	foreach( SRC ${TIDY_SRC} )
		add_custom_command(
			TARGET ${TIDY_TARGET}_tidy
			PRE_BUILD
			COMMAND ${CLANG_TIDY}
				${TIDY_WERROR}
				${TIDY_ARGS}
				${SRC}
				|| exit ${TIDY_ERROR}
			COMMENT "Clang-Tidy check ${SRC}"
			)
	endforeach()
	add_dependencies( tidy ${TIDY_TARGET}_tidy )

	add_custom_target( ${TIDY_TARGET}_tidy_fix
		SOURCES ${TIDY_SRC}
#		COMMENT "Clang-Tidy fix"
		)

	foreach( SRC ${TIDY_SRC} )
		add_custom_command(
			TARGET ${TIDY_TARGET}_tidy_fix
			PRE_BUILD
			COMMAND ${CLANG_TIDY}
				-fix
				${TIDY_WERROR}
				${TIDY_ARGS}
				${SRC}
				|| exit ${TIDY_ERROR}
			COMMENT "Clang-Tidy fix ${SRC}"
			)
	endforeach()
	add_dependencies( tidy_fix ${TIDY_TARGET}_tidy_fix )
endfunction()

################################################################################
# static_analysis_format(
#                        [TARGET target]
#                        [ADDITIONAL_FILES file1 [file2] ...]
#                        [ARGS arg1 [arg2] ...]
#                        )
# [TARGET]
#       Target to analyse. Eavery sourcce file will be analysed.
# [ADDITIONAL_FILES]
#       Specify other files to analyse, headers for instance.
# [ARGS]
#       Specify command line arguments.
################################################################################
function( static_analysis_format )
	set( OPTIONS )
	set( ONEVALUEARGS TARGET )
	set( MULTIVALUEARGS ADDITIONAL_FILES ARGS )
	cmake_parse_arguments( FORMAT
		"${OPTIONS}"
		"${ONEVALUEARGS}"
		"${MULTIVALUEARGS}"
		${ARGN}
		)

	if( STATIC_ERROR )
		set( FORMAT_ERROR 1 )
	else()
		set( FORMAT_ERROR 0 )
	endif()
	if( STATIC_WERROR )
		set( FORMAT_WERROR -Werror )
	else()
		set( FORMAT_WERROR )
	endif()
	if( FORMAT_TARGET )
		get_target_property( FORMAT_SRC ${FORMAT_TARGET} SOURCES )
	else()
		message( ERROR "Specify a target!" )
	endif()
	foreach( ARG ${FORMAT_ARGS} )
		list( APPEND ALL_ARGS ${ARG} )
	endforeach()
	list( APPEND FORMAT_SRC ${FORMAT_ADDITIONAL_FILES} )

	add_custom_target( ${FORMAT_TARGET}_format
		SOURCES ${FORMAT_SRC}
#		COMMENT "Clang-Format check"
		)

	foreach( SRC ${FORMAT_SRC} )
		add_custom_command(
			TARGET ${FORMAT_TARGET}_format
			PRE_BUILD
			COMMAND ${CLANG_FORMAT}
				--dry-run
				${FORMAT_WERROR}
				${FORMAT_ARGS}
				${SRC}
				|| exit ${FORMAT_ERROR}
			COMMENT "Clang-Format check ${SRC}"
			)
	endforeach()
	add_dependencies( format ${FORMAT_TARGET}_format )

	add_custom_target( ${FORMAT_TARGET}_format_fix
		SOURCES ${FORMAT_SRC}
#		COMMENT "Clang-Format fix"
		)

	foreach( SRC ${FORMAT_SRC} )
		add_custom_command(
			TARGET ${FORMAT_TARGET}_format_fix
			PRE_BUILD
			COMMAND ${CLANG_FORMAT}
				-i
				${FORMAT_WERROR}
				${FORMAT_ARGS}
				${SRC}
				|| exit ${FORMAT_ERROR}
			COMMENT "Clang-Format fix ${SRC}"
			)
	endforeach()
	add_dependencies( format_fix ${FORMAT_TARGET}_format_fix )
endfunction()

################################################################################
# static_analysis_cppcheck(
#                        [TARGET target]
#                        [ADDITIONAL_FILES file1 [file2] ...]
#                        [ARGS arg1 [arg2] ...]
#                        )
# [TARGET]
#       Target to analyse. Eavery sourcce file will be analysed.
# [ADDITIONAL_FILES]
#       Specify other files to analyse, headers for instance.
# [ARGS]
#       Specify command line arguments.
################################################################################
function( static_analysis_cppcheck )
	set( OPTIONS )
	set( ONEVALUEARGS TARGET )
	set( MULTIVALUEARGS ADDITIONAL_FILES ARGS )
	cmake_parse_arguments( CPPCHECK
		"${OPTIONS}"
		"${ONEVALUEARGS}"
		"${MULTIVALUEARGS}"
		${ARGN}
		)

	if( STATIC_ERROR )
		set( CPPCHECK_ERROR 1 )
	else()
		set( CPPCHECK_ERROR 0 )
	endif()
	if( CPPCHECK_TARGET )
		get_target_property( CPPCHECK_SRC ${CPPCHECK_TARGET} SOURCES )
		get_target_property( CPPCHECK_STD ${CPPCHECK_TARGET} CXX_STANDARD )
	else()
		message( ERROR "Specify a target!" )
	endif()
	foreach( ARG ${CPPCHECK_ARGS} )
		list( APPEND ALL_ARGS ${ARG} )
	endforeach()

	add_custom_target( ${CPPCHECK_TARGET}_cppcheck
		SOURCES ${CPPCHECK_SRC} ${CPPCHECK_ADDITIONAL_FILES}
#		COMMENT "Cppcheck"
		)

	list( APPEND CPPCHECK_SRC ${CPPCHECK_ADDITIONAL_FILES} )
	foreach( SRC ${CPPCHECK_SRC} )
		add_custom_command(
			TARGET ${CPPCHECK_TARGET}_cppcheck
			PRE_BUILD
			COMMAND ${CPPCHECK}
				--error-exitcode=1
				--std=c++${CPPCHECK_STD}
				${CPPCHECK_ARGS}
				${SRC}
				|| exit ${CPPCHECK_ERROR}
			COMMENT "Cppcheck ${SRC}"
			)
	endforeach()
	add_dependencies( cppcheck ${CPPCHECK_TARGET}_cppcheck )
endfunction()
