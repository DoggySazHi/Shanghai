#ifndef SHANGHAI_SHADER_H
#define SHANGHAI_SHADER_H

#include "gl.h"

class Shader {
public:
    Shader(const char* vertexPath, const char* fragmentPath);
    ~Shader();
    void use() const;
    [[nodiscard]] bool isCompiled() const;
    void setUniform(const char* name, int value) const;
    void setUniform(const char* name, float value) const;
    void setUniform(const char* name, float a, float b) const;
    void setUniform(const char* name, bool value) const;
private:
    unsigned int id = 0;
    static GLuint compileShader(const char* shaderSource, GLenum shaderType);
};


#endif //SHANGHAI_SHADER_H
