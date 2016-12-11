# CMake entry point
cmake_minimum_required (VERSION 3.5)
project (engine)

find_package(OpenGL REQUIRED)

# Compile lib dependencies 
add_subdirectory (lib)

## Add headers files
include_directories (
	include/
)


set(ALL_LIBS
	SOIL
	assimp
	glfw
	GLEW
		${OPENGL_LIBRARY}

)

set(SOURCES src/main.cpp src/Camera.cpp src/Mesh.cpp src/Model.cpp)


add_definitions(
	-DTW_STATIC
	-DTW_NO_LIB_PRAGMA
	-DTW_NO_DIRECT3D
	-DGLEW_STATIC
	-D_CRT_SECURE_NO_WARNINGS
)

add_compile_options(
	-std=c++11
        -g
) 


# main.cpp
add_executable(main ${SOURCES})
target_link_libraries(main ${ALL_LIBS})