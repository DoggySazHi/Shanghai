#ifndef SHANGHAI_GL_H
#define SHANGHAI_GL_H

#ifdef __WIN32__

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3native.h>

#elif __WAYLAND__

#include <GLES2/gl2.h>

#elif __X11__

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#define GLFW_EXPOSE_NATIVE_X11
#include <GLFW/glfw3native.h>

#endif

#endif //SHANGHAI_GL_H
