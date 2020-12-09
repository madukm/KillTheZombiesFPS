file(GLOB shader_files *.glsl)

macro(copy_shaders shader_files copied_files)
	#message("shaders: ${${shader_files}}")
	foreach(shader ${${shader_files}})
		#message("shader: ${shader}")
		get_filename_component(file_name ${shader} NAME)
		get_filename_component(full_path ${shader} ABSOLUTE)
		set(output_dir ${CMAKE_CURRENT_BINARY_DIR})
		set(output_file ${output_dir}/${file_name})
		set(${copied_files} ${${copied_files}} ${output_file})
		set(${copied_files} ${${copied_files}} PARENT_SCOPE)
		set_source_files_properties(${asset} PROPERTIES HEADER_FILE_ONLY TRUE)
		if (WIN32)
			add_custom_command(
				OUTPUT ${output_file}
				COMMAND xcopy \"${full_path}\" \"${output_file}*\" /Y /Q /F
				DEPENDS ${full_path}
			)
		else()
			add_custom_command(
				OUTPUT ${output_file}
				COMMAND mkdir --parents ${output_dir} && cp --force --link \"${full_path}\" \"${output_file}\"
				DEPENDS ${full_path}
			)
		endif()
	endforeach()
endmacro()

copy_shaders(shader_files copied_shaders)
	
add_custom_target(
	shaders 
	DEPENDS ${copied_shaders}
	SOURCES ${shader_files})
