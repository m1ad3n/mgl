#ifndef TEX_HEADER
#define TEX_HEADER

#define GLFW_INCLUDE_NONE
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* usefull macros */

#define ERROR(msg, ...) {\
	fprintf(stderr, "[ERROR]: " msg "\n", ##__VA_ARGS__);\
	exit(1);\
}

#define INFO(msg, ...) fprintf(stdout, "[INFO]: " msg "\n", ##__VA_ARGS__);

/* shader functions */
GLuint read_shaders(char* content, char** vertex, char** fragment);
GLuint compile_shader(const char* source, GLuint type);
GLuint compile_program(char* path);
GLint check_compile_errors(GLuint shader, const char* type);

/* buffer functions */
GLuint mgl_gen_buffer(GLenum type, void* data, size_t data_len);

#endif // TEX_HEADER

