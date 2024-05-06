#ifndef SHANGHAI_SHADER_H
#define SHANGHAI_SHADER_H

#include <GLES2/gl2.h>

class Shader {
public:
    Shader(const char* vertexPath, const char* fragmentPath);
    void use() const;
    [[nodiscard]] bool isCompiled() const;
    void setUniform(const char* name, int value) const;
private:
    unsigned int id = 0;
    static GLuint compileShader(const char* shaderSource, GLenum shaderType);
};


#endif //SHANGHAI_SHADER_H
