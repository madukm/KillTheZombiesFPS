file(GLOB texture_files textures/*)
file(GLOB model_files models/*)

macro(copy_files files folder copied_files)
	#message("files: ${${files}}")
	foreach(file ${${files}})
		#message("file: ${file}") 
		get_filename_component(file_name ${file} NAME)
		get_filename_component(full_path ${file} ABSOLUTE)
		set(output_dir ${CMAKE_CURRENT_BINARY_DIR}/${folder})
		set(output_file ${output_dir}/${file_name})
		set(${copied_files} ${${copied_files}} ${output_file})
		set(${copied_files} ${${copied_files}} PARENT_SCOPE)
		set_source_files_properties(${file} PROPERTIES HEADER_FILE_ONLY TRUE)
		if (WIN32)
			add_custom_command(
				OUTPUT ${output_file}
				COMMAND xcopy \"${full_path}\" \"${output_file}*\" /Y /Q /F
				DEPENDS ${full_path}
			)
		else()
			add_custom_command(
				OUTPUT ${output_file}
				COMMAND mkdir --parents ${output_dir} && cp -r --force --link \"${full_path}\" \"${output_file}\"
				DEPENDS ${full_path}
			)
		endif()
	endforeach()
endmacro()

copy_files(texture_files textures copied_textures)
copy_files(model_files models copied_models)

add_custom_target(
	assets 
	DEPENDS ${copied_textures} ${copied_models}
	SOURCES ${texture_files} ${model_files})
