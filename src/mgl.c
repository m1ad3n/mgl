#include "mgl.h"
#include "utils.h"

GLFWwindow* window;

int width = 600;
int height = 400;

float vertecies[] = {
	 0.5f, -0.5f, 0.0f,
	 0.5f,  0.5f, 0.0f,
	-0.5f, -0.5f, 0.0f,
	-0.5f,  0.5f, 0.0f,
};

GLuint indicies[] = {
	0, 1, 3,
	3, 2, 0
};

int main(void) {

	GLuint vbo = mgl_gen_buffer(GL_ARRAY_BUFFER, vertecies, sizeof(vertecies));
	GLuint ebo = mgl_gen_buffer(GL_ELEMENT_ARRAY_BUFFER, indicies, sizeof(indicies));

	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	time_it(NULL);
	GLuint shader = compile_program("shaders/basic");
	if (!shader) return -1;
	time_it("compile_program");

	while (!glfwWindowShouldClose(window)) {

		glClear(GL_COLOR_BUFFER_BIT);

		glUseProgram(shader);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		/*glDrawArrays(GL_TRIANGLES, 0, 3);*/

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

GLuint mgl_gen_buffer(GLenum type, void* data, size_t data_len) {
	GLuint buff;
	glGenBuffers(1, &buff);
	glBindBuffer(type, buff);
	glBufferData(type, data_len, data, GL_STATIC_DRAW);
	glBindBuffer(type, 0);
	return buff;
}

