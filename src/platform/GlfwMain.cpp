#define GLFW_INCLUDE_NONE

#include <iostream>

#include "Platform.h"
#include "../gl.h"
#include "../state.h"
#include "../config/ShanghaiConfiguration.h"
#include "../Background.h"
#include "../Shanghai.h"
#include "../fonts/FontRenderer.h"
#include "../Random.h"
#include "../9patch/NinePatch.h"

// HiDPI compensation for mouse inputs
double cursorScaleX = 1.0, cursorScaleY = 1.0;

// Windows stuff
GLFWwindow* glfwWindow;

// Actual Shanghai stuff
EGLState eglState;
ShanghaiConfiguration* config;
Background* background;
std::vector<Shanghai*> shanghais;

// Callback handlers

void updateCursorScale(GLFWwindow* window) {
    int windowWidth, windowHeight, framebufferWidth, framebufferHeight;
    glfwGetWindowSize(window, &windowWidth, &windowHeight);
    glfwGetFramebufferSize(window, &framebufferWidth, &framebufferHeight);

    cursorScaleX = windowWidth > 0 ? (double) framebufferWidth / windowWidth : 1.0;
    cursorScaleY = windowHeight > 0 ? (double) framebufferHeight / windowHeight : 1.0;
}

void resizeHandler(GLFWwindow* window, [[maybe_unused]] const int width, [[maybe_unused]] const int height) {
    int framebufferWidth, framebufferHeight;
    glfwGetFramebufferSize(window, &framebufferWidth, &framebufferHeight);

    eglState.width = framebufferWidth;
    eglState.height = framebufferHeight;
    updateCursorScale(window);

    for (auto& shanghai : shanghais) {
        shanghai->setScreenGeometry(eglState.width, eglState.height);
    }

    if (background != nullptr) {
        background->setScreenGeometry(eglState.width, eglState.height);
    }

    glViewport(0, 0, framebufferWidth, framebufferHeight);
}

void mouseHandler([[maybe_unused]] GLFWwindow* window, double xpos, double ypos) {
    eglState.curX = (int) (xpos * cursorScaleX);
    eglState.curY = (int) (ypos * cursorScaleY);
}

void mouseButtonHandler(GLFWwindow* window, int button, int action, [[maybe_unused]] int mods) {
    double xx, yy;
    glfwGetCursorPos(window, &xx, &yy);
    eglState.curX = (int) (xx * cursorScaleX);
    eglState.curY = (int) (yy * cursorScaleY);

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

    for (const auto& shanghai : shanghais) {
        shanghai->draw(&eglState);
    }

    Shanghai::updateCursor(shanghais, &eglState);

#ifdef DEBUG
    GLenum err;
    while ((err = glGetError()) != GL_NO_ERROR) {
        std::cerr << "OpenGL error: " << err << '\n';
    }
#endif

    glfwSwapBuffers(glfwWindow);
    platform::afterSwap(glfwWindow);
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

    if (config->getOutput() >= static_cast<uint32_t>(monitorCount) && config->getOutput() != UINT32_MAX) {
        std::cerr << "Invalid output index\n";
        return -1;
    }

    auto* monitor = monitors[monitorIndex];

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_DECORATED, GLFW_FALSE);
    glfwWindowHint(GLFW_TRANSPARENT_FRAMEBUFFER, GLFW_TRUE);
    platform::applyWindowHints();

    auto* videoMode = glfwGetVideoMode(monitor);
    eglState.width = videoMode->width;
    eglState.height = videoMode->height;

    int monitorX, monitorY;
    glfwGetMonitorPos(monitor, &monitorX, &monitorY);

    // Monitor is nullptr because we actually want a borderless window, not full screen
    glfwWindow = glfwCreateWindow((int) eglState.width, (int) eglState.height, "Shanghai", nullptr, nullptr);

    if (glfwWindow == nullptr)
    {
        std::cerr << "Failed to create GLFW window\n";
        glfwTerminate();
        return -1;
    }

    glfwSetWindowPos(glfwWindow, monitorX, monitorY);
    platform::afterWindowCreated(glfwWindow, (int) eglState.width, (int) eglState.height);

    glfwMakeContextCurrent(glfwWindow);
    glfwSwapInterval(1);

    if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress)))
    {
        std::cerr << "Failed to initialize GLAD\n";
        return -1;
    }

    glfwSetFramebufferSizeCallback(glfwWindow, resizeHandler);
    glfwSetCursorPosCallback(glfwWindow, mouseHandler);
    glfwSetMouseButtonCallback(glfwWindow, mouseButtonHandler);
    platform::installCallbacks(glfwWindow);

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

    shanghais.push_back(new Shanghai());

    std::cout << "Starting output...\n";

    // Trigger the DPI check
    resizeHandler(glfwWindow, 0, 0);

    platform::beforeMainLoop(glfwWindow, monitorX, monitorY, (int) eglState.width, (int) eglState.height);

    while (!glfwWindowShouldClose(glfwWindow))
    {
        glfwPollEvents();
        draw();
    }

    for (const auto& deadShanghai : shanghais) {
        delete deadShanghai;
    }
    shanghais.clear();

    delete background;
    Shanghai::releaseSharedResources();

    glfwTerminate();

    return 0;
}
