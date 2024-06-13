#include <fstream>
#include <iostream>
#include "Shader.h"

Shader::Shader(const char *vertexPath, const char *fragmentPath, const char *commonPath) {
    // Compile shader1 code
    std::cout << "Compiling vertex shader1 " << vertexPath << "...\n";
    GLuint vertexShader = compileShader(vertexPath, nullptr, GL_VERTEX_SHADER);
    std::cout << "Compiling fragment shader1 " << fragmentPath << "...\n";
    GLuint fragmentShader = compileShader(fragmentPath, commonPath, GL_FRAGMENT_SHADER);

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
        std::cout << "Failed to link GLES shader1 program\n" << infoLog << '\n';
    }

    std::cout << "Cleaning up shader1...\n";
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

GLuint Shader::compileShader(const char *shaderSource, const char *shaderSourceCommon, GLenum shaderType) {
    // Load shader1 file
    std::ifstream vertexShaderFile(shaderSource);

    if (vertexShaderFile.fail()) {
        std::cerr << "Failed to load " << shaderSource << " shader1 file\n";
        return 0;
    }

    // Read shader1 code
    std::string shaderCode((std::istreambuf_iterator<char>(vertexShaderFile)), std::istreambuf_iterator<char>());
    vertexShaderFile.close();

    // Load common, if exists
    if (shaderSourceCommon != nullptr) {
        std::ifstream commonShaderFile(shaderSourceCommon);

        if (commonShaderFile.fail()) {
            std::cerr << "Failed to load " << shaderSourceCommon << " shader1 file\n";
            return 0;
        }

        // Read shader1 code
        std::string commonShaderCode((std::istreambuf_iterator<char>(commonShaderFile)), std::istreambuf_iterator<char>());
        commonShaderFile.close();
        shaderCode = commonShaderCode + shaderCode;
    }

    const char* shaderSourceIndirection = shaderCode.c_str();

    // Compile shader1
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
        std::cerr << "Failed to compile GLES shader1 " << shaderSource << '\n' << infoLog << '\n';
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

void Shader::setUniform(const char *name, bool value) const {
    setUniform(name, value ? 1 : 0);
}

void Shader::setUniform(const char *name, float value) const {
    glUniform1f(glGetUniformLocation(id, name), value);
}

void Shader::setUniform(const char *name, float a, float b, float c, float d) const {
    glUniform4f(glGetUniformLocation(id, name), a, b, c, d);
}
