set(exe_name ${MAIN_PROJECT})

set(src_files
        client.cpp
)

add_executable(${exe_name}
                ${src_files})

target_link_libraries(${exe_name} PRIVATE GLEW::GLEW glfw glm nlohmann_json::nlohmann_json killTheZombiesLib)
