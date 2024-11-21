#ifndef TEX_HEADER
#define TEX_HEADER

#define GLFW_INCLUDE_NONE
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <time.h>

#define ERROR(msg, ...) {\
	fprintf(stderr, "[ERROR]: " msg "\n", ##__VA_ARGS__);\
	exit(1);\
}

#define INFO(msg, ...) fprintf(stdout, "[INFO]: " msg "\n", ##__VA_ARGS__);

GLuint read_shaders(char* content, char** vertex, char** fragment);
GLuint compile_shader(const char* source, GLuint type);
GLuint compile_program(char* path);
GLint check_compile_errors(GLuint shader, const char* type);

void time_it(const char* msg);

#endif // TEX_HEADER
