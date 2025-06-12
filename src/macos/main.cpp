#define GLFW_INCLUDE_NONE
#define STB_IMAGE_IMPLEMENTATION

#include <iostream>
#include "stb_image.h"

#include "../gl.h"
#include "../state.h"
#include "../config/ShanghaiConfiguration.h"
#include "../Background.h"
#include "../Shanghai.h"

// Windows stuff
GLFWwindow* glfwWindow;

// Actual Shanghai stuff
EGLState eglState;
ShanghaiConfiguration* config;
Background* background;
Shanghai* shanghai;

// Callback handlers

void resizeHandler([[maybe_unused]] GLFWwindow* window, const int width, const int height) {
    eglState.width = width;
    eglState.height = height;

    std::cout << "Resizing to " << width << "x" << height << '\n';

    if (shanghai != nullptr) {
        shanghai->setScreenGeometry(eglState.width, eglState.height);
    }

    if (background != nullptr) {
        background->setScreenGeometry(eglState.width, eglState.height);
    }

    glViewport(0, 0, width, height);
}

void mouseHandler([[maybe_unused]] GLFWwindow* window, double xpos, double ypos) {
    eglState.curX = (int) xpos;
    eglState.curY = (int) ypos;
}

void mouseButtonHandler(GLFWwindow* window, int button, int action, [[maybe_unused]] int mods) {
    double xx, yy;
    glfwGetCursorPos(window, &xx, &yy);

    if (action == GLFW_PRESS) {
        eglState.buttons = 1;
    } else {
        eglState.buttons = 0;
    }
}

// Draw loop
void draw() {
    glViewport(0, 0, (int) eglState.width, (int) eglState.height);

    glClearColor(0, 0, 0, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    if (eglState.curX != -1 && eglState.curY != -1) {
        glEnable(GL_SCISSOR_TEST);
        glScissor(eglState.curX, (int) (eglState.height - eglState.curY), 5, 5);
        glClearColor(0, 0, 0, 0.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        glDisable(GL_SCISSOR_TEST);
    }

    if (config->isBackgroundEnabled()) {
        background->draw(&eglState);
    }

    shanghai->draw(&eglState);

    glfwSwapBuffers(glfwWindow);
}

int main() {
    std::cout << "Loading configuration...\n";
    config = ShanghaiConfiguration::getInstance();

    glfwSetErrorCallback([](int error, const char* description) {
        std::cerr << "GLFW error " << error << ": " << description << '\n';
    });

    glfwInit();

    int monitorCount;
    auto* monitors = glfwGetMonitors(&monitorCount);
    // Default to the primary monitor - due to Linux compat, UINT32_MAX is used to indicate no monitor preference
    uint32_t monitorIndex = config->getOutput() == UINT32_MAX ? 0 : config->getOutput();

    if (monitorCount == 0) {
        std::cerr << "No outputs found\n";
        return -1;
    }

    if (config->getOutput() >= monitorCount && config->getOutput() != UINT32_MAX) {
        std::cerr << "Invalid output index\n";
        return -1;
    }

    auto* monitor = monitors[monitorIndex];

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, 1);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#ifndef __APPLE__
    glfwWindowHint(GLFW_DECORATED, GLFW_FALSE);
    glfwWindowHint(GLFW_TRANSPARENT_FRAMEBUFFER, GLFW_TRUE);
    glfwWindowHint(GLFW_FLOATING, GLFW_TRUE);
//    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
#endif

    auto* videoMode = glfwGetVideoMode(monitor);
    eglState.width = videoMode->width;
    eglState.height = videoMode->height;

    glfwWindow = glfwCreateWindow((int) eglState.width, (int) eglState.height, "Shanghai", nullptr, nullptr);

    if (glfwWindow == nullptr)
    {
        std::cerr << "Failed to create GLFW window\n";
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(glfwWindow);
    gladLoadGLLoader((GLADloadproc) glfwGetProcAddress);
    glfwSwapInterval(1);

    if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress)))
    {
        std::cerr << "Failed to initialize GLAD\n";
        return -1;
    }

    glfwSetFramebufferSizeCallback(glfwWindow, resizeHandler);
    glfwSetWindowSizeCallback(glfwWindow, resizeHandler);
    glfwSetCursorPosCallback(glfwWindow, mouseHandler);
    glfwSetMouseButtonCallback(glfwWindow, mouseButtonHandler);

    std::cout << "OpenGL version: " << glGetString(GL_VERSION) << '\n';
    std::cout << "OpenGL renderer: " << glGetString(GL_RENDERER) << '\n';
    std::cout << "OpenGL vendor: " << glGetString(GL_VENDOR) << '\n';
    std::cout << "OpenGL shading language version: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << "\n\n";

    // Enable blending
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    if (config->isBackgroundEnabled()) {
        background = new Background();
    }

    shanghai = new Shanghai();

    std::cout << "Starting output...\n";

    glfwSetWindowPos(glfwWindow, 0, 0);
    glfwSetWindowSize(glfwWindow, 1280, 760);

    // Set viewport size on retina displays
    int width, height;
    glfwGetFramebufferSize(glfwWindow, &width, &height);
    resizeHandler(glfwWindow, width, height);

    while (!glfwWindowShouldClose(glfwWindow))
    {
        glfwPollEvents();
        draw();
    }

    glfwTerminate();

    return 0;
}