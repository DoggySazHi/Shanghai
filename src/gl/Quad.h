#ifndef SHANGHAI_QUAD_H
#define SHANGHAI_QUAD_H

#include "../gl.h"

class Quad {
public:
    enum class Layout {
        POSITION,
        POSITION_AND_UV,
    };

    explicit Quad(Layout layout);
    ~Quad();

    Quad(const Quad&) = delete;
    Quad& operator=(const Quad&) = delete;

    void draw(const GLfloat* vertices) const;

private:
    static constexpr GLsizei VERTEX_COUNT = 4;

    [[nodiscard]] GLsizei stride() const;

    Layout layout;
#ifdef SHANGHAI_GL_CORE
    GLuint vao = 0;
    GLuint vbo = 0;
#endif
};


#endif //SHANGHAI_QUAD_H
