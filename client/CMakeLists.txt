cmake_minimum_required(VERSION 3.2)
project(KillTheZombiesFPS VERSION 0.1.0 LANGUAGES C CXX)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/bin)

if (UNIX)
    #add_definitions(-DUNIX)
    add_compile_options("-std=c++17")
    add_compile_options("-Wall")
	#add_link_options("-fsanitize=address")
    add_compile_options("-g")
endif ()

find_package(GLEW REQUIRED)
find_package(glfw3 REQUIRED)
find_package(glm CONFIG REQUIRED)
find_package(nlohmann_json CONFIG REQUIRED)
find_path(STB_INCLUDE_DIRS "stb.h")

set(MAIN_PROJECT "KillTheZombiesFPS")

# Create libs
add_library(killTheZombiesLib INTERFACE)
target_include_directories(killTheZombiesLib INTERFACE lib/)

# Add subdirectories
add_subdirectory(assets)
add_subdirectory(src)

