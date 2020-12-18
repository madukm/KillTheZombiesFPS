set(exe_name ${MAIN_PROJECT})

set(src_files
        main.cpp
        client.cpp
        render/window.cpp
        render/shader.cpp
        render/mesh.cpp
        render/texture.cpp
        render/camera.cpp
        render/ui.cpp
        objects/object.cpp
        objects/survivor.cpp
        objects/zombie.cpp
        objects/block.cpp
        objects/sceneZero.cpp
)

add_executable(${exe_name}
                ${src_files})

target_link_libraries(${exe_name} PRIVATE GLEW::GLEW glfw glm nlohmann_json::nlohmann_json killTheZombiesLib)
add_subdirectory(shaders)
add_dependencies(${exe_name} shaders assets)
