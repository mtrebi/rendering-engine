# CMake entry point
cmake_minimum_required (VERSION 2.6)
project (engine)

find_package(OpenGL REQUIRED)

# Compile lib dependencies 
add_subdirectory (lib)


include_directories(
	lib/glfw-3.2.1/include/GLFW/
	lib/glew-1.13.0/include/
)

set(ALL_LIBS
	${OPENGL_LIBRARY}
	glfw
	GLEW_1130
)

#file(GLOB_RECURSE _HDRS "include/*.h")
#file(GLOB_RECURSE _SRCS "src/*.cpp")

add_definitions(
	-DTW_STATIC
	-DTW_NO_LIB_PRAGMA
	-DTW_NO_DIRECT3D
	-DGLEW_STATIC
	-D_CRT_SECURE_NO_WARNINGS
)
#add_library(main ${_HDRS} ${_SRCS})

# main.cpp
add_executable(main src/main.cpp)
target_link_libraries(main ${ALL_LIBS})



#[[
SOURCE_GROUP(common REGULAR_EXPRESSION ".*/common/.*" )
SOURCE_GROUP(shaders REGULAR_EXPRESSION ".*/.*shader$" )
]]
