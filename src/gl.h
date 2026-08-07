#ifndef SHANGHAI_GL_H
#define SHANGHAI_GL_H

#ifdef SHANGHAI_PLATFORM_WAYLAND

#include <GLES3/gl3.h>

#else

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#if defined(SHANGHAI_PLATFORM_WINDOWS)
#define GLFW_EXPOSE_NATIVE_WIN32
#elif defined(SHANGHAI_PLATFORM_MACOS)
#define GLFW_EXPOSE_NATIVE_COCOA
#elif defined(SHANGHAI_PLATFORM_X11)
#define GLFW_EXPOSE_NATIVE_X11
#endif

#include <GLFW/glfw3native.h>

#endif

#endif //SHANGHAI_GL_H
