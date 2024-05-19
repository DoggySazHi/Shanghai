#ifndef SHANGHAI_GL_H
#define SHANGHAI_GL_H

#ifdef __WIN32__

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#elif __WAYLAND__

#include <GLES2/gl2.h>

#endif

#endif //SHANGHAI_GL_H
