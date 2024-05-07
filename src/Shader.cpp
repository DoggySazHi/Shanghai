#include <fstream>
#include <iostream>
#include "Shader.h"

Shader::Shader(const char *vertexPath, const char *fragmentPath) {
    // Compile shader code
    std::cout << "Compiling vertex shader " << vertexPath << "...\n";
    GLuint vertexShader = compileShader(vertexPath, GL_VERTEX_SHADER);
    std::cout << "Compiling fragment shader " << fragmentPath << "...\n";
    GLuint fragmentShader = compileShader(fragmentPath, GL_FRAGMENT_SHADER);

    if (vertexShader == 0 || fragmentShader == 0) {
        return;
    }

    // Link shaders
    id = glCreateProgram();
    glAttachShader(id, vertexShader);
    glAttachShader(id, fragmentShader);
    glLinkProgram(id);

    // Check for linking errors
    int success;
    char infoLog[512];
    glGetProgramiv(id, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(id, 512, nullptr, infoLog);
        std::cout << "Failed to link GLES shader program\n" << infoLog << '\n';
    }

    std::cout << "Cleaning up shader...\n";
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
}

Shader::~Shader() {
    glDeleteProgram(id);
}

void Shader::use() const {
    glUseProgram(id);
}

bool Shader::isCompiled() const {
    return id != 0;
}

GLuint Shader::compileShader(const char *shaderSource, GLenum shaderType) {
    // Load shader file
    std::ifstream vertexShaderFile(shaderSource);

    if (vertexShaderFile.fail()) {
        std::cerr << "Failed to load " << shaderSource << " shader file\n";
        return 0;
    }

    // Read shader code
    std::string shaderCode((std::istreambuf_iterator<char>(vertexShaderFile)), std::istreambuf_iterator<char>());
    vertexShaderFile.close();
    const char* shaderSourceIndirection = shaderCode.c_str();

    // Compile shader
    GLuint shader = glCreateShader(shaderType);
    glShaderSource(shader, 1, &shaderSourceIndirection, nullptr);
    glCompileShader(shader);

    // Check for compile errors
    int success;
    char infoLog[512];
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(shader, 512, nullptr, infoLog);
        std::cerr << "Failed to compile GLES shader " << shaderSource << '\n' << infoLog << '\n';
        return 0;
    }

    return shader;
}

void Shader::setUniform(const char *name, int value) const {
    glUniform1i(glGetUniformLocation(id, name), value);
}

void Shader::setUniform(const char *name, float a, float b) const {
    glUniform2f(glGetUniformLocation(id, name), a, b);
}
