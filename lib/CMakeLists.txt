include_directories(
	lib/glfw-3.2.1/include/GLFW/
	lib/glew-2.0.0/include/
	lib/SOIL/src/
)

### GLFW ###

add_subdirectory (glfw-3.2.1)

set(OPENGL_LIBRARY
	${OPENGL_LIBRARY}
	-lGL -lGLU -lXrandr -lXext -lX11 -lrt
	${CMAKE_DL_LIBS}
	${GLFW_LIBRARIES}
)

### GLEW ###

set(GLEW_SOURCE
	glew-2.0.0/src/glew.c
)

add_library( GLEW STATIC
	${GLEW_SOURCE}
)



### SOIL ###

# Source Files
SET(SOIL_SRCS
	SOIL/src/image_helper.c
    SOIL/src/stb_image_aug.c
    SOIL/src/image_DXT.c
    SOIL/src/SOIL.h
)

# Header Files
SET(SOIL_HDRS
	SOIL/src/image_helper.h
    SOIL/src/stb_image_aug.h
    SOIL/src/image_DXT.h
    SOIL/src/stbi_DDS_aug_c.h
    SOIL/src/stbi_DDS_aug.h
    SOIL/src/SOIL.c
)

ADD_LIBRARY(SOIL_Static STATIC ${SOIL_SRCS} ${SOIL_HDRS})
