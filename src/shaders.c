#include "mgl.h"
#include "utils.h"

GLint check_compile_errors(GLuint shader, const char* type) {
    int success;
    char infoLog[1024];
    if (strcmp(type, "PROGRAM")) {
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if (!success) {
            glGetShaderInfoLog(shader, 1024, NULL, infoLog);
            fprintf(stdout, "OpenGL Shader compilation error of type: %s\n", type);
            if (strlen(infoLog) > 0)
                ERROR("%s", infoLog);
        }
    } else {
        glGetProgramiv(shader, GL_LINK_STATUS, &success);
        if (!success) {
            glGetProgramInfoLog(shader, 1024, NULL, infoLog);
            fprintf(stdout, "OpenGL Program linking error of type: %s\n", type);
            if (strlen(infoLog) > 0)
                ERROR("%s\n", infoLog);
        }
    }

    return success;
}

GLuint read_shaders(char* content, char** vertex, char** fragment) {
	char* ident = "#fragment";
	char* ptr = strstr(content, ident);

	if (ptr == NULL) {
		ERROR("invalid shader file syntax");
		return 0;
	}

	// vertex shader
	size_t vert_size = (size_t)(--ptr - content);
	*vertex = (char*)malloc(vert_size + 1);
	(*vertex)[vert_size] = '\0';
	strncpy(*vertex, content, vert_size);

	// fragment shader
	ptr += strlen(ident) + 1;
	size_t frag_size = strlen(ptr);
	*fragment = (char*)malloc(frag_size + 1);
	(*fragment)[frag_size] = '\0';
	strncpy(*fragment, ptr, frag_size);

	return 1;
}

GLuint compile_shader(const char* source, GLuint type) {
	GLuint shader;
	shader = glCreateShader(type);
	glShaderSource(shader, 1, &source, NULL);
	glCompileShader(shader);
	return shader;
}

GLuint compile_program(char* path) {
	char* content = getfile_content(path);
	char* vertex_shader = NULL;
	char* fragment_shader = NULL;

	GLuint rs = read_shaders(content, &vertex_shader, &fragment_shader);
	if (!rs) return 0;

	GLuint vertex_id = compile_shader(vertex_shader, GL_VERTEX_SHADER);
	if (check_compile_errors(vertex_id, "VERTEX") == GL_FALSE) {
		glDeleteShader(vertex_id);
		free(content);
		free(vertex_shader);
		free(fragment_shader);
		return 0;
	}

	GLuint fragment_id = compile_shader(fragment_shader, GL_FRAGMENT_SHADER);
	if (check_compile_errors(fragment_id, "FRAGMENT") == GL_FALSE) {
		glDeleteShader(vertex_id);
		glDeleteShader(fragment_id);
		free(content);
		free(vertex_shader);
		free(fragment_shader);
		return 0;
	}

	GLuint shader_program;
	shader_program = glCreateProgram();
	glAttachShader(shader_program, vertex_id);
	glAttachShader(shader_program, fragment_id);
	glLinkProgram(shader_program);

	glDeleteShader(vertex_id);
	glDeleteShader(fragment_id);

	free(content);
	free(vertex_shader);
	free(fragment_shader);

	if (check_compile_errors(shader_program, "PROGRAM") == GL_FALSE) {
		return 0;
	}

	return shader_program;
}
