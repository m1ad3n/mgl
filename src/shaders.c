#include "mgl.h"

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
