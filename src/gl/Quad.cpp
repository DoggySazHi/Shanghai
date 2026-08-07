#include "Quad.h"

Quad::Quad(const Layout layout) : layout(layout) {
#ifdef SHANGHAI_GL_CORE
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
#endif
}

Quad::~Quad() {
#ifdef SHANGHAI_GL_CORE
    glDeleteBuffers(1, &vbo);
    glDeleteVertexArrays(1, &vao);
#endif
}

GLsizei Quad::stride() const {
    const GLsizei floatsPerVertex = layout == Layout::POSITION_AND_UV ? 4 : 2;
    return floatsPerVertex * static_cast<GLsizei>(sizeof(GLfloat));
}

void Quad::draw(const GLfloat* vertices) const {
    const GLsizei vertexStride = stride();
    const bool textured = layout == Layout::POSITION_AND_UV;

#ifdef SHANGHAI_GL_CORE
    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, VERTEX_COUNT * vertexStride, vertices, GL_STREAM_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, vertexStride, nullptr);
    glEnableVertexAttribArray(0);

    if (textured) {
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, vertexStride,
                              reinterpret_cast<const void*>(2 * sizeof(GLfloat)));
        glEnableVertexAttribArray(1);
    }
#else
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, vertexStride, vertices);
    glEnableVertexAttribArray(0);

    if (textured) {
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, vertexStride, vertices + 2);
        glEnableVertexAttribArray(1);
    }
#endif

    glDrawArrays(GL_TRIANGLE_STRIP, 0, VERTEX_COUNT);

    glDisableVertexAttribArray(0);
    if (textured) {
        glDisableVertexAttribArray(1);
    }

#ifdef SHANGHAI_GL_CORE
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
#endif
}
