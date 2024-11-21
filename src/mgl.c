#include "mgl.h"

GLFWwindow* window;

int width = 600;
int height = 400;

float vertecies[] = {
	 0.5f, -0.5f, 0.0f,
	 0.0f,  0.5f, 0.0f,
	-0.5f, -0.5f, 0.0f
};

int main(void) {

	GLuint vbo;
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertecies), vertecies, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	time_it(NULL);
	GLuint shader = compile_program("shaders/basic");
	if (!shader) return -1;
	time_it("compile_program");

	while (!glfwWindowShouldClose(window)) {

		glClear(GL_COLOR_BUFFER_BIT);

		glUseProgram(shader);
		glDrawArrays(GL_TRIANGLES, 0, 3);

		glfwSwapBuffers(window);
		glfwPollEvents();

	}

	return 0;
}

__attribute__((constructor)) void tex_init(void) {
	if (!glfwInit()) {
		ERROR("glfwInit failed");
	}

	window = glfwCreateWindow(width, height, "tex", NULL, NULL);
	if (!window) {
		glfwTerminate();
		exit(1);
	}

	glfwMakeContextCurrent(window);
	glfwSwapInterval(1);

	if (glewInit() != GLEW_OK) {
		ERROR("glewInit failed")
	}
	
	const GLubyte* renderer = glGetString(GL_RENDERER);
	const GLubyte* version = glewGetString(GLEW_VERSION);
	INFO("Renderer: %s", renderer);
	INFO("OpenGL version supported %s", version);

}

__attribute__((destructor)) void tex_destroy(void) {
	glfwDestroyWindow(window);
	glfwTerminate();
}

void time_it(const char* msg) {
	static clock_t start, end;
	if (!start) {
		start = clock();
		return;
	}

	end = clock();
	fprintf(stdout, "[TIME_IT]: %s took %.5f secs\n", msg, (double)(end - start) / CLOCKS_PER_SEC);
	start = 0;
}

/* returned pointer points to the heap allocated memory */
static char* getfile_contents(char* path) {
	FILE* fptr = fopen(path, "r");
	fseek(fptr, 0, SEEK_END);

	size_t size = ftell(fptr);
	char* content = (char*)malloc(size + 1);
	content[size] = '\0';

	fseek(fptr, 0, SEEK_SET);
	for (size_t i = 0; i < size; i++)
		content[i] = fgetc(fptr);

	return content;
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
	char* content = getfile_contents(path);
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
