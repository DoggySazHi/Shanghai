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

// Windows stuff
GLFWwindow* glfwWindow;

// Actual Shanghai stuff
EGLState eglState;
ShanghaiConfiguration* config;
Background* background;
FontRenderer* fontRenderer;
NinePatch* ninePatch;
std::vector<Shanghai*> shanghais;

// Callback handlers

void resizeHandler([[maybe_unused]] GLFWwindow* window, const int width, const int height) {
    eglState.width = width;
    eglState.height = height;

    for (auto& shanghai : shanghais) {
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

    for (const auto& shanghai : shanghais) {
        shanghai->draw(&eglState);
    }

    // ninePatch->render(&eglState, 200, 100, 100, 100);

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

    glfwWindow = glfwCreateWindow((int) eglState.width, (int) eglState.height, "Shanghai", monitor, nullptr);

    if (glfwWindow == nullptr)
    {
        std::cerr << "Failed to create GLFW window\n";
        glfwTerminate();
        return -1;
    }

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

    auto shanghai = new Shanghai();
    fontRenderer = new FontRenderer("TamzenForPowerline10x20r.bdf");
    ninePatch = new NinePatch("img/message.png", 12, 12, 12, 12);
    // shanghai->positionX = -40;
    // shanghai->positionY = 1300;
    // shanghai->flip = false;
    // shanghai->getStateMachine()->setState(ShanghaiState::WALL_CLIMB);
    shanghai->getStateMachine()->setState(ShanghaiState::THROWING);
    shanghais.push_back(shanghai);
    // ShanghaiState states[] = {ShanghaiState::CRAWLING, ShanghaiState::SITTING_AND_LOOKING, ShanghaiState::SITTING, ShanghaiState::WALKING, ShanghaiState::JUMP};
    // // ShanghaiState states[] = {ShanghaiState::WALL_HOLD, ShanghaiState::WALL_CLIMB};
    // for (int i = 0; i < 40; ++i) {
    //     auto* shanghai = new Shanghai();
    //     shanghai->positionX = i * 128;
    //     shanghais.push_back(shanghai);
    //
    //     shanghai->getStateMachine()->setState(states[(int) (Random::rand() * std::size(states))]);
    //     shanghai->flip = Random::rand() < 0.5;
    // }

    std::cout << "Starting output...\n";

    // The framebuffer is not the window on a HiDPI display, and it is the
    // framebuffer that the viewport and the shaders care about.
    int framebufferWidth, framebufferHeight;
    glfwGetFramebufferSize(glfwWindow, &framebufferWidth, &framebufferHeight);
    resizeHandler(glfwWindow, framebufferWidth, framebufferHeight);

    platform::beforeMainLoop(glfwWindow, (int) eglState.width, (int) eglState.height);

    while (!glfwWindowShouldClose(glfwWindow))
    {
        glfwPollEvents();
        draw();
    }

    for (const auto& deadShanghai : shanghais) {
        delete deadShanghai;
    }
    shanghais.clear();

    delete ninePatch;
    delete fontRenderer;
    delete background;
    Shanghai::releaseSharedResources();

    glfwTerminate();

    return 0;
}
